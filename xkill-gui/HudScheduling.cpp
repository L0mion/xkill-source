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
	parent = ui->label_scheduling_background->parentWidget();
	window = ui->label_scheduling_background;
	subWindow = ui->label_scheduling_subbackground;
	progressbar = ui->progressBar_scheduling_progressbar;
	advantageLabel = ui->label_priority_advantage;

	Float2 screenSize;
	screenSize.x = window->parentWidget()->width();
	screenSize.y = window->parentWidget()->height();

	// Compute sizes
	itemHeight = subWindow->height();
	itemWidth = itemHeight*0.8f;
	standardMargin = itemHeight * 0.5f;

	int maxWindowSize = parent->width() - (ui->groupBox_health->width() + ui->groupBox_ammo->width() * 3.3f);
	if(window->width() > maxWindowSize)
		window->resize( maxWindowSize , window->height());
	subWindow->resize(window->width() - 2*standardMargin, subWindow->height());
	progressbar->resize(window->width() - 2*standardMargin, progressbar->height());
	int windowHeight = standardMargin*2 + itemHeight + subWindow->height() + progressbar->height();
	window->resize(window->width(), windowHeight);
	advantageLabel->resize(itemWidth, advantageLabel->height());

	// Compute positions
	window->move(screenSize.x*0.5f - window->width()*0.5, screenSize.y - window->height() - 9);
	progressbar->move(window->x() + standardMargin, window->y() + standardMargin);
	subWindow->move(window->x() + standardMargin, progressbar->y() + progressbar->height() + standardMargin);
	
	advantageLabel->move(subWindow->x(), subWindow->y());


	//hide();
	ui->label_7->hide();
	ui->label_8->hide();
	//ui->label_priority_advantage->hide();
	progressbar->setMaximum(500);


	// Add players
	AttributeIterator<Attribute_Player> itrPlayer = ATTRIBUTE_MANAGER->player.getIterator();
	while(itrPlayer.hasNext())
	{
		AttributePtr<Attribute_Player> ptr_player = itrPlayer.getNext();
		//ptr_player->priority = 5;
		QLabel* l = new QLabel(parent);
		l->resize(itemWidth, standardMargin);
		l->move(subWindow->x() + 0.5f, subWindow->y() + 0.5f);

		// Set color based on player's color
		Float3 color = ptr_player->avatarColor;
		std::string str_backgroundColor	= "rgba("+Converter::IntToStr((int)(color.x * 255))+", "+Converter::IntToStr((int)(color.y * 255))+", "+ Converter::IntToStr((int)(color.z * 255)) +", 200);";
		std::string str_borderColor		= "rgba("+Converter::IntToStr((int)(color.x * 255))+", "+Converter::IntToStr((int)(color.y * 255))+", "+ Converter::IntToStr((int)(color.z * 255)) +", 255);";
		std::string str_sheet = "background: " + str_backgroundColor + "border: 1px solid rgba(0, 0, 0, 30); border-top: 2px solid rgba(255, 255, 255, 30); border-right: 2px solid rgba(255, 255, 255, 30);";
		l->setStyleSheet(QString(str_sheet.c_str()));

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
	float timeUntilScheduling = SETTINGS->timeUntilScheduling;
	if(timeUntilScheduling < 0.0f)
		timeUntilScheduling = 0.0f;
	float schedulerTime = SETTINGS->schedulerTime;

	float schedulingRatio = timeUntilScheduling /schedulerTime;
	int test = timeUntilScheduling /schedulerTime * kPrecition;
	int schedulingProgressbarRatio = kPrecition - test;
	if(progressbar->value() != schedulingProgressbarRatio)
	{
		progressbar->setValue(schedulingProgressbarRatio);
		progressbar->update();

		// Turn bar red if at scheduling
		if(schedulingProgressbarRatio == kPrecition)
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
		int newItemHeight = itemHeight * (1.0f - kCycleRatioInfluence) + itemHeight * kCycleRatioInfluence * cycleRatio;
			
		if(item->label->height() != newItemHeight)
			item->label->resize(itemWidth, newItemHeight);
		

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
		target.y = subWindow->y() + (itemHeight - newItemHeight);
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
	if(advantageLabel->x() !=  ownerLabel->x() || advantageLabel->y() !=  ownerLabel->y() + ownerLabel->height())
		advantageLabel->move(ownerLabel->x(), ownerLabel->y() + ownerLabel->height());
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

void HudScheduling_Item::update()
{
	// Interpolate position
	float factor = 2.0f * SETTINGS->trueDeltaTime;
	if(factor > 1.0f)
		factor = 1.0f;
	Float2 newPos = Float2::lerp(&position, &targetPosition, factor);
	newPos.y = targetPosition.y;

	setPosition(newPos);
}
