#include "HudScheduling.h"


void HudScheduling::hide()
{
	window->hide();
	subWindow->hide();
	progressbar->hide();
	advantageLabel->hide();
	for(int i=0; i<items.size(); i++)
			items[i].label->hide();
}

void HudScheduling::show()
{
	window->show();
	subWindow->show();
	progressbar->show();
	advantageLabel->show();
	for(int i=0; i<items.size(); i++)
		items[i].label->show();
}

void HudScheduling::init( Ui::Menu_HUD* ui, AttributePtr<Attribute_Player> ptr_owner_player )
{
	ui->groupBox_scoreInfo->hide();
	parent = ui->label_scheduling_background->parentWidget();
	window = ui->label_scheduling_background;
	subWindow = ui->label_scheduling_subbackground;
	progressbar = ui->progressBar_scheduling_progressbar;
	advantageLabel = ui->label_priority_advantage;

	Float2 screenSize;
	screenSize.x = window->parentWidget()->width();
	screenSize.y = window->parentWidget()->height();

	// Compute sizes
	standardMargin = subWindow->height();
	itemWidth = standardMargin*0.8f;
	subWindow->resize(window->width() - 2*standardMargin, subWindow->height());
	progressbar->resize(window->width() - 2*standardMargin, progressbar->height());
	int windowHeight = standardMargin*3 + subWindow->height() + progressbar->height();
	window->resize(window->width(), windowHeight);

	// Compute positions
	window->move(screenSize.x*0.5f - window->width()*0.5, screenSize.y - window->height() - standardMargin*1.5f);
	subWindow->move(window->x() + standardMargin, window->y() + standardMargin);
	progressbar->move(subWindow->x(), subWindow->y() + subWindow->height() + standardMargin);
	advantageLabel->move(subWindow->x(), subWindow->y());
	//hide();
	ui->label_7->hide();
	ui->label_8->hide();
	ui->groupBox_bottomCenter->hide();
	//ui->label_priority_advantage->hide();
	progressbar->setMaximum(500);


	// Add players
	AttributeIterator<Attribute_Player> itrPlayer = ATTRIBUTE_MANAGER->player.getIterator();
	while(itrPlayer.hasNext())
	{
		AttributePtr<Attribute_Player> ptr_player = itrPlayer.getNext();
		ptr_player->priority = 1;
		QLabel* l = new QLabel(parent);
		l->resize(itemWidth, standardMargin);
		l->move(subWindow->x() + 0.5f, subWindow->y() + 0.5f);

		// Set color based on player's color
		Float3 color = ptr_player->avatarColor;
		std::string str_color = "background-color: rgba("+Converter::IntToStr((int)(color.x * 255))+", "+Converter::IntToStr((int)(color.y * 255))+", "+ Converter::IntToStr((int)(color.z * 255)) +", 150);";
		l->setStyleSheet(QString(str_color.c_str()));

		l->show();

		// Save ref to owner player
		if(ptr_player == ptr_owner_player)
			ownerIndex = items.size();

		// Create item
		HudScheduling_Item item;
		item.label = l;
		item.ptr_player = ptr_player;
		item.setPosition(Float2(subWindow->x(), subWindow->y()));
		items.push_back(item);
	}

	// Show advantage label on top
	advantageLabel->raise();
}

void HudScheduling::refresh()
{
	int highestPriority = findHighestPriority();
	int lowestCycles = findLowestCycles();
	int highestCycles = findHighestCycles();

	// Update progressbar
	const int kPrecition = 500;
	int schedulingRatio = kPrecition - (int)(SETTINGS->timeUntilScheduling / SETTINGS->schedulerTime * kPrecition);
	if(progressbar->value() != schedulingRatio)
	{
		progressbar->setValue(schedulingRatio);
		progressbar->update();

		// Turn bar red if at scheduling
		if(schedulingRatio == kPrecition)
		{
			if(!isScheduling)
			{
				isScheduling = true;
				progressbar->setStyleSheet("QProgressBar::chunk{background-color: rgba(255, 0, 0, 75);}");
			}
		}
		else if(isScheduling)
		{
			isScheduling = false;
			progressbar->setStyleSheet("");
		}
	}


	// Put each item (player) into the correct place relative 
	// to each other based on their priority

	int labelSize = subWindow->height();
	int areaWidth = subWindow->width() - itemWidth;
	float sectionSize = 0; 
	if(highestPriority > 0)
		sectionSize = areaWidth/highestPriority;


	// Determine size and location for each label
	for(int i=0; i<items.size(); i++)
	{
		HudScheduling_Item* item = &items[i];

		int priority = item->ptr_player->priority;
		int cycles = item->ptr_player->cycles;


		// Find our relative index and number of neighbours
		CountAndIndex countAndIndex = findRelativeIndex(priority, i);
		int numNeighbours = countAndIndex.count;
		int relativeIndex = countAndIndex.index;


		// Compute size
		float cycleRatio = 1.0f;
		int deltaCycles = highestCycles - lowestCycles;
		if(deltaCycles > 0)
			cycleRatio = ((float)(cycles - lowestCycles)) / deltaCycles;
		const float kCycleRatioInfluence = 0.4f;
		int itemHeight = standardMargin * (1.0f - kCycleRatioInfluence) + standardMargin * kCycleRatioInfluence * cycleRatio;
			
		if(item->label->height() != itemHeight)
			item->label->resize(itemWidth, itemHeight);
		

		// Compute location
		int index = item->ptr_player->priority;
		if(index < 0)
			index = 0;

		// Make sure all indicators fits by 
		// flipping them if they reach the left
		// edge (reaches max priority)
		if(priority > 0 && priority == highestPriority)
			relativeIndex = -relativeIndex;

		float crowdOffset = relativeIndex * itemWidth;
		int offset = (int)(sectionSize * index + crowdOffset);
		Float2 target;
		target.x = subWindow->x() + offset;
		target.y = subWindow->y() + (standardMargin - itemHeight);
		item->targetPosition = target;
	}

	// Update labels so they can
	// interpolate to the correct
	// position
	for(int i=0; i<items.size(); i++)
	{
		HudScheduling_Item* item = &items[i];
		item->update();
	}

	// Update advantage label
	QLabel* ownerLabel = items[ownerIndex].label;
	if(advantageLabel->x() !=  ownerLabel->x() || advantageLabel->y() !=  ownerLabel->y())
		advantageLabel->move(ownerLabel->x(), ownerLabel->y());
}

int HudScheduling::findHighestPriority()
{
	int highest = 0;
	for(int i=0; i<items.size(); i++)
	{
		if(items[i].ptr_player->priority > highest)
			highest = items[i].ptr_player->priority;
	}

	return highest;
}
