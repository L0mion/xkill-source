#include "HudPowerupDispayer.h"


void HudPowerupDispayer::HudItem::init( HudPowerupDispayer* manager, std::string str_icon, std::string str_info = "")
{
	this->manager = manager;
	position = manager->getPosition();
	manager->registerItem(this);

	// Get a unique index, used to
	// offset our position later on
	index = manager->createIndex();

	QWidget* parent = manager->getParent();
	std::string str_path = ":/xkill/images/icons/hacks/";

	const int ICON_MARGIN = 5;

	// Create icon label
	{
		icon = new QLabel(parent);
		icon->hide();
		icon->setPixmap(QString((str_path + str_icon).c_str()));
		icon->resize(icon->sizeHint());
	}

	// Create progressbar
	{
		bar = new QProgressBar(parent);
		bar->hide();
		bar->setOrientation(Qt::Vertical);
		bar->setMaximumSize(5, 32);
		bar->setTextVisible(false);
		bar->setStyleSheet("QProgressBar{ background-color: rgba(0, 0, 0, 0); padding: 0px;} QProgressBar::chunk{ background-color: rgba(255, 255, 255, 130);}");
		bar->setValue(0);
	}


	// Create info label
	// Only create if we have
	// something to fill it with
	hasInfo = false;
	if(str_info != "")
		hasInfo = true;
	if(hasInfo)
	{
		info = new QLabel(parent);
		info->hide();
		info->setStyleSheet("font-family: 'Arial Black'; background-color: rgb(0, 0, 0, 50); font-size:12pt; padding: 1px 1px;");
		info->setText(str_info.c_str());
		info->resize(info->sizeHint());
	}
}


void HudPowerupDispayer::init( QWidget* bottomFrame, AttributePtr<Attribute_Player> ptr_player )
{
	this->ptr_player = ptr_player;
	parent = bottomFrame->parentWidget();
	position = Float2(bottomFrame->x(), bottomFrame->y()); 

	
	pow_running		.init(this, "state_running.png",		"");
	pow_speed		.init(this, "hack_speed.png",			"");
	pow_cycleSteal	.init(this, "hack_cyclesteal.png",		"Steal <span style='color:#40b32b;'>cycles</span> from others");
	pow_power		.init(this, "hack_power.png",			"<span style='color:#40b32b;'>2x damage</span>");
	pow_jet			.init(this, "hack_jet.png",				"Press <span style='color:#40b32b;'>Jump</span> to use jethack");
	//pow_executing	.init(this, "state_executing.png",		"Executing in <span style='color:#40b32b;'>Kernel Mode</span>");
	pow_executing	.init(this, "state_executing.png",		"<span style='color:#40b32b;'>KernelMode</span> granted");
}

void HudPowerupDispayer::update()
{
	{
		float ratio = ptr_player->currentSprintTime/ptr_player->sprintTime;
		if(pow_running.updateActive(ratio < 1.0f))
		{
			pow_running.setProgress(ratio);
		}
	}
	

	if(pow_executing.updateActive(ptr_player->executing))
	{
		// Nothing
	}


	if(pow_speed.updateActive(ptr_player->speedHackPair.first))
	{
		float ratio = ptr_player->speedHackPair.second->getTimeLeft() / ptr_player->speedHackPair.second->getStartTime();;
		pow_speed.setProgress(ratio);
	}
		
	if(pow_jet.updateActive(ptr_player->jetHackPair.first))
	{
		float ratio = ptr_player->jetHackPair.second->getTimeLeft() / ptr_player->jetHackPair.second->getStartTime();;
		pow_jet.setProgress(ratio);
	}

	if(pow_cycleSteal.updateActive(ptr_player->cycleHackPair.first))
	{
		float ratio = ptr_player->cycleHackPair.second->getTimeLeft() / ptr_player->cycleHackPair.second->getStartTime();;
		pow_cycleSteal.setProgress(ratio);
	}

	if(pow_power.updateActive(ptr_player->powerHackPair.first))
	{
		float ratio = ptr_player->powerHackPair.second->getTimeLeft() / ptr_player->powerHackPair.second->getStartTime();;
		pow_power.setProgress(ratio);
	}


	// Update labels
	int index = 0;
	int infoIndex = 0;
	for(int i=0; i<items.size(); i++)
	{
		if(items[i]->isActive())
		{
			items[i]->update(index, infoIndex);
			if(items[i]->hasInfo)
				infoIndex++;
			index++; 
		}
	}
}


void HudPowerupDispayer::HudItem::update( int index, int infoIndex)
{
	if(_isActive)
	{
		const int ICON_MARGIN = 5;

		Float2 targetPos;
		float factor = 5.0f * SETTINGS->trueDeltaTime;
		if(factor > 1.0f)
			factor = 1.0f;
		

		// Interpolate icon position
		targetPos.x = position.x + index*(icon->width() + (ICON_MARGIN + 5)) + 9;
		targetPos.y = position.y - icon->height();
		position_icon = Float2::lerp(&position_icon, &targetPos, factor);
		icon->move(position_icon.x + 0.5f, position_icon.y + 0.5f);

		// Move bar next to icon
		bar->move(icon->x() + icon->width(), icon->y());
		
		// Interpolate info position
		if(hasInfo)
		{
			targetPos.x = position.x + ICON_MARGIN;
			targetPos.y = position.y - icon->height() - 9 - info->height() * (infoIndex + 1);
			position_info = Float2::lerp(&position_info, &targetPos, factor);
			info->move(position_info.x + 0.5f, position_info.y + 0.5f);
		}
	}
}

bool HudPowerupDispayer::HudItem::updateActive( bool isActive )
{
	if(_isActive != isActive)
	{
		_isActive = isActive;

		// Hide show label
		if(_isActive)
		{
			icon->show();
			const int ICON_MARGIN = 5;
			position_icon.x = position.x + 9;
			//position_icon.x = 0 - icon->width() - 5;
			position_icon.y = position.y - icon->height();
			icon->move(position_icon.x + 0.5f, position_icon.y + 0.5f);
			bar->show();
			if(hasInfo)
			{
				//position_info.x = position.x + ICON_MARGIN;
				position_info.x = 0 - info->width() - 5;
				position_info.y = position.y - icon->height() - 9 - info->height();
				info->move(position_info.x + 0.5f, position_info.y + 0.5f);
				info->show();
			}
		}
		else
		{
			icon->hide();
			bar->hide();
			if(hasInfo)
				info->hide();
		}
	}
	return _isActive;
}
