#include "Menu_HUD.h"

#include <QtCore/QDateTime>
#include <xkill-utilities/Converter.h>
#include <QtGui/QMovie>
ATTRIBUTES_DECLARE_ALL;




Menu_HUD::Menu_HUD( AttributePtr<Attribute_SplitScreen> splitScreen, QWidget* parent ) : QWidget(parent)
{
	ui.setupUi(this);
	ptr_splitScreen = splitScreen;

	Float2 pos(ptr_splitScreen->ssTopLeftX, ptr_splitScreen->ssTopLeftY);
	move(ptr_splitScreen->ssTopLeftX, ptr_splitScreen->ssTopLeftY);
	resize(ptr_splitScreen->ssWidth, ptr_splitScreen->ssHeight);
	hide();

	QWidget::setAttribute(Qt::WA_ShowWithoutActivating);
	QWidget::setAttribute(Qt::WA_TransparentForMouseEvents);

	SUBSCRIBE_TO_EVENT(this, EVENT_UPDATE);
	SUBSCRIBE_TO_EVENT(this, EVENT_PLAYER_TARGET_HIT);
	SUBSCRIBE_TO_EVENT(this, EVENT_PLAYER_TAKING_DAMAGE);

	hudMessage_manager.init(this, ptr_splitScreen);
	ui.label_hudMessageTemplate->hide();
	mapToSplitscreen();
	initScoreboard();

	hitOverlayFade				= 0.0f;
	crosshair_targetHitFade		= 0.0f;
	scoreboardFade				= 0.0f;
	healthFade					= 0.0f;
	ammoFade					= 0.0f;
	firingModeFade				= 0.0f;
}

Menu_HUD::~Menu_HUD()
{
	UNSUBSCRIBE_TO_EVENTS(this);
}

void Menu_HUD::mapToSplitscreen()
{
	Float2 screenSize;
	screenSize.x = ptr_splitScreen->ssWidth;
	screenSize.y = ptr_splitScreen->ssHeight;

	Float2 centerPos;
	centerPos.x = screenSize.x * 0.5f;
	centerPos.y = screenSize.y * 0.5f;

	// Hide unused labels
	ui.label_helper->hide();
	ui.frame_test->hide();

	// Move stretch death overlay across screen
	ui.label_deathOverlay->move(0, 0);
	ui.label_deathOverlay->resize(ptr_splitScreen->ssWidth, ptr_splitScreen->ssHeight);
	ui.label_deathOverlay->hide();
	ui.label_hitOverlay->move(0, 0);
	ui.label_hitOverlay->resize(ptr_splitScreen->ssWidth, ptr_splitScreen->ssHeight);
	ui.label_hitOverlay->hide();

	// Move center HUD to center
	ui.label_aim->move(centerPos.x - ui.label_aim->width()* 0.5f, centerPos.y - ui.label_aim->height()* 0.5f);
	ui.label_aim_targetHit->move(centerPos.x - ui.label_aim_targetHit->width()* 0.5f, centerPos.y - ui.label_aim_targetHit->height()* 0.5f);
	ui.label_firingMode->move(centerPos.x - ui.label_firingMode->width()* 0.5f + 10, centerPos.y - ui.label_firingMode->height()* 0.5f);


	// Move scoreboard to center
	ui.frame_scoreboard->hide();
	ui.frame_scoreboard->move(centerPos.x - ui.frame_scoreboard->width()* 0.5f, centerPos.y - ui.frame_scoreboard->height()* 0.5f);
	/*int test2 = ui.frame_scoreboard->y();
	if(ui.frame_scoreboard->y() < 0)
		ui.frame_scoreboard->move(ui.frame_scoreboard->x(), 0);*/


	// Place statusbars
	Float2 barPos;
	barPos.x = screenSize.x * 0.08f;
	barPos.y = screenSize.y * 0.01f;
	Float2 barMiddle(ui.progressBar_health->width() * 0.5f, ui.progressBar_health->height() * 0.5f);
	ui.progressBar_health->move(centerPos.x - barPos.x - barMiddle.x, centerPos.y + barPos.y - barMiddle.y);
	ui.progressBar_ammo->move(centerPos.x + barPos.x - barMiddle.x, centerPos.y + barPos.y - barMiddle.y);

	// Move botton HUD to bottom
	Float2 bottomPos;
	ui.frame_bottom->resize(screenSize.x - screenSize.x*0.00f * 2, ui.frame_bottom->height());
	bottomPos.x = screenSize.x * 0.5f - ui.frame_bottom->width()* 0.5f;
	int test = screenSize.x*0.005f;
	bottomPos.y = screenSize.y - ui.frame_bottom->height();
	//bottomPos.y = 0;
	ui.frame_bottom->move(bottomPos.x, bottomPos.y);

	// Move top HUD to top
	Float2 topPos;
	ui.frame_top->resize(screenSize.x - screenSize.x*0.00f * 2, ui.frame_top->height());
	topPos.x = screenSize.x * 0.5f - ui.frame_top->width()* 0.5f;
	topPos.y = screenSize.x*0.005f;
	ui.frame_top->move(topPos.x, topPos.y);

	// Move HUD messages to center
	hudMessage_manager.move(centerPos);


	// Create weapon info
	weaponInfoHud.init(ui.groupBox_weaponInfo, ui.verticalLayout_weaponInfo, ui.groupBox_weaponInfo_subHud, ui.horizontalLayout_weaponInfo_subHud);
	ui.groupBox_weaponInfo2->hide();
	Float2 weaponInfoPos;
	weaponInfoPos.x = ui.frame_bottom->parentWidget()->width() + 1;
	weaponInfoPos.y = ui.frame_bottom->pos().y();
	weaponInfoHud.setPosition(weaponInfoPos);

	// Create scheduling hud
	schedulingHud.init(&ui, ptr_splitScreen->ptr_player);

	// Create power up displayer
	hudPowerupDispayer.init(ui.frame_bottom, ptr_splitScreen->ptr_player);
}

void Menu_HUD::refresh()
{
	AttributePtr<Attribute_Player>		ptr_player		=	ptr_splitScreen->ptr_player;
	AttributePtr<Attribute_Health>		ptr_health		=	ptr_player->ptr_health;
	AttributePtr<Attribute_WeaponStats>	ptr_weaponStats	=	ptr_player->ptr_weaponStats;

	Ammunition* ammunition = &ptr_weaponStats->ammunition[ptr_weaponStats->currentAmmunitionType];
	FiringMode* firingMode = &ptr_weaponStats->firingMode[ptr_weaponStats->currentFiringModeType];
	int firingIndex = firingMode->type;
	int ammoIndex = ammunition->type;
	float fadeTime = 1.0f;


	//
	// Update powerup display
	//

	hudPowerupDispayer.update();


	//
	// Update scheduling hud
	//

	schedulingHud.refresh();


	//
	// Update weapon info hud
	//

	weaponInfoHud.update(firingIndex, ammoIndex);


	//
	// Show ammunition info
	//

	int numAmmo = firingMode->nrOfShotsLeftInClip[ammoIndex];
	int clipSize = firingMode->clipSize;
	int ammoLeft = ammunition->currentTotalNrOfShots;
	ui.label_ammo->setNum(numAmmo);
	ui.label_totalAmmo->setNum((int)ptr_weaponStats->ammunition[ammoIndex].currentTotalNrOfShots);
	int prev_ammoRatio = ui.progressBar_ammo->value();
	int numFullyReloaded = clipSize;
	if(clipSize > ammoLeft && ammoLeft > 0)
		numFullyReloaded = ammoLeft; 
	int ammoRatio = (int)((numAmmo / (float)numFullyReloaded) * 100);
	float f_reloadRatio =firingMode->reloadTimeLeft / (float)firingMode->reloadTime;
	int reloadRatio = (int)((firingMode->reloadTimeLeft / (float)firingMode->reloadTime) * 100);
	
	// Compute reload feedback size
	{
		// Show 
		if(f_reloadRatio < 1.0f)
		{
			if(ui.label_reloadFeedback->isHidden())
				ui.label_reloadFeedback->show();

			// Hide
			Float2 size;
			float sizeModifier = f_reloadRatio;
			size.x = ptr_splitScreen->ssWidth * 0.4f * sizeModifier;
			size.y = ptr_splitScreen->ssWidth * 0.03f * sizeModifier;
			ui.label_reloadFeedback->resize(size.x, size.y);
			ui.label_reloadFeedback->move((ptr_splitScreen->ssWidth - ui.label_reloadFeedback->width()) * 0.5f, (ptr_splitScreen->ssHeight - ui.label_reloadFeedback->height()) * 0.5f);
			ui.label_reloadFeedback->update();

		}
		else
		{
			if(!ui.label_reloadFeedback->isHidden())
				ui.label_reloadFeedback->hide();
		}
	}
	

	// Show menu if ammo has changed otherwise fade after a few seconds
	if(reloadRatio == 100 && ammoRatio != prev_ammoRatio)
	{
		prev_ammoRatio = ammoRatio;
		ammoFade = fadeTime;
	}
	else if(reloadRatio != prev_reloadRatio)
	{
		prev_reloadRatio = reloadRatio;
		ammoFade = fadeTime;
	}

	int largestRatio = ammoRatio;
	if(ammoRatio < (100 - reloadRatio))
		largestRatio = (100 - reloadRatio);

	ui.progressBar_ammo->setValue(largestRatio);

	// Hide bar if full
	if(ammoRatio == 100)
		ammoFade = 0.0f;
	ammoFade = 1.0f;

	// Perform fade
	if(ammoFade > 0.0f)
	{
		ammoFade -= SETTINGS->trueDeltaTime;

		// Show progress bar if hidden
		if(ui.progressBar_ammo->isHidden())
			ui.progressBar_ammo->show();


		ui.progressBar_ammo->update();
	}
	else
	{
		// Hide progress bar if shown
		if(!ui.progressBar_ammo->isHidden())
			ui.progressBar_ammo->hide();
	}


	//
	// Show health info
	//

	if(ptr_health->health < 1.0f && ptr_health->health > 0.0f) //Prevent player health showing as 0 when the player is still alive
	{
		ui.label_health->setNum(1);
	}
	else
	{
		ui.label_health->setNum(static_cast<int>(ptr_health->health));
	}
	

	int healthRatio = (int)((ptr_health->health / ptr_health->maxHealth) * 100);

	// Show menu if health has changed otherwise fade after a few seconds
	if(healthRatio != ui.progressBar_health->value())
	{
		healthFade = fadeTime;
	}

	// Hide bar if dead
	if(ptr_player->detectedAsDead)
		healthFade = 0.0f;

	if(healthFade > 0.0f)
	{
		healthFade -= SETTINGS->trueDeltaTime;

		// Show progress bar if hidden
		if(ui.progressBar_health->isHidden())
			ui.progressBar_health->show();


		// Update value
		ui.progressBar_health->setValue(healthRatio);
		ui.progressBar_health->update();
	}
	else
	{
		// Hide progress-bar if shown
		if(!ui.progressBar_health->isHidden())
			ui.progressBar_health->hide();
	}


	//
	// Display HUD-messages
	//

	hudMessage_manager.update();


	// Show hit effects

	// Player is damaged
	if(hitOverlayFade > 0.0f)
	{
		hitOverlayFade -= SETTINGS->trueDeltaTime;
		if(ui.label_hitOverlay->isHidden())
		{
			ui.label_hitOverlay->show();
		}
	}
	else
	{
		if(!ui.label_hitOverlay->isHidden())
		{
			ui.label_hitOverlay->hide();
		}
	}

	// Player damages another player
	if(crosshair_targetHitFade > 0.0f)
	{
		crosshair_targetHitFade -= SETTINGS->trueDeltaTime;
		if(ui.label_aim_targetHit->isHidden())
		{
			ui.label_aim_targetHit->show();
		}
	}
	else
	{
		if(!ui.label_aim_targetHit->isHidden())
		{
			ui.label_aim_targetHit->hide();
		}
	}


	// Show death effects
	if(ptr_player->detectedAsDead)
	{
		if(ui.label_deathOverlay->isHidden())
		{
			ui.label_deathOverlay->show();
			ui.frame_top->hide();
			ui.label_aim->hide();
			ui.label_firingMode->hide();
			ui.frame_bottom->hide();
			schedulingHud.hide();

			ui.progressBar_health->hide();
			ui.progressBar_ammo->hide();
			
		}
	}
	else
	{
		if(!ui.label_deathOverlay->isHidden())
		{
			ui.label_deathOverlay->hide();
			ui.frame_top->show();
			ui.label_aim->show();
			ui.label_firingMode->show();
			ui.frame_bottom->show();
			schedulingHud.show();
		}
	}

	//
	// Update Scoreboard
	//

	scoreboard.refresh();
	if(ptr_player->detectedAsDead || GET_STATE() == STATE_GAMEOVER || ptr_player->showScoreboard)
	{
		if(ptr_player->showScoreboard)
			scoreboardFade = 0.0f;

		// Show scoreboard if delay has expired
		if(scoreboardFade > 0.0f)
			scoreboardFade -= SETTINGS->trueDeltaTime;
		if(scoreboardFade <= 0.0f)
		{
			// Show scoreboard if hidden
			if(ui.frame_scoreboard->isHidden())
			{
				hudMessage_manager.silenceAllMessages();
				ui.frame_scoreboard->show();
				ptr_player->isScoreBoardVisible = true;

				// Hide aim
				if(!ui.label_aim->isHidden())
				{
					ui.label_aim->hide();
					ui.label_firingMode->hide();
				}
			}
		}
	}
	else
	{
		// Reset scoreboard timer
		scoreboardFade = 2.0f;

		// Hide scoreboard if shown
		if(!ui.frame_scoreboard->isHidden())
		{
			ui.frame_scoreboard->hide();
			ptr_player->isScoreBoardVisible = false;

			// Show aim if it was hidden
			if(ui.label_aim->isHidden())
			{
				ui.label_aim->show();
				ui.label_firingMode->show();
			}
		}
	}




	//
	// Scheduling info
	//

	ui.label_priority_advantage->setNum(ptr_player->priority);

	// Determine priority advantage
	//{
	//	std::string str_priorityAdvantage = "";

	//	int scoreDiff = scoreboard.maxPriority - ptr_player->priority;

	//	// If first player, show score difference to second closest
	//	if(scoreDiff == 0)
	//		scoreDiff = scoreboard.maxPriority - scoreboard.secondMaxPriority;
	//	// ELSE: Show score difference to second closest
	//	else
	//		scoreDiff = ptr_player->cycles - scoreboard.maxPriority;

	//	// Set label
	//	if(scoreDiff >= 0)
	//		ui.label_priority_advantage->setText("+" +QString::number(scoreDiff));
	//	else
	//		ui.label_priority_advantage->setNum(scoreDiff);

	//}





	/*QString str_time = QDateTime::fromTime_t(SETTINGS->timeUntilScheduling).toString("mm:ss");
	ui.label_schedulingTimer->setText(str_time);*/


	//
	// Set cross-hair based on selected ammo
	//

	// Only change if weapon has been switched
	if(index_crosshair != ammoIndex)
	{
		index_crosshair = ammoIndex;

		// Determine image
		QString path;
		switch(index_crosshair) 
		{
		case XKILL_Enums::BULLET:
			path = ":/xkill/images/icons/cross_hairs/crosshair_bullet.png";
			break;
		case XKILL_Enums::SCATTER:
			path = ":/xkill/images/icons/cross_hairs/crosshair_scatter.png";
			break;
		case XKILL_Enums::EXPLOSIVE:
			path = ":/xkill/images/icons/cross_hairs/crosshair_explosive.png";
			break;
		default:
			path = ":/xkill/images/icons/default.png";
			break;
		}

		// Set image to label
		ui.label_aim->setPixmap(path);
	}


	//
	// Set indicator based on firiring-mode
	//

	// Only change if weapon has been switched
	if(index_firingMode != firingIndex)
	{
		index_firingMode = firingIndex;
		firingModeFade = fadeTime;

		// Determine image
		QString path;
		switch(index_firingMode)
		{
		case XKILL_Enums::SINGLE:
			path = ":/xkill/images/icons/cross_hairs/firing_single.png";
			break;
		case XKILL_Enums::SEMI:
			path = ":/xkill/images/icons/cross_hairs/firing_semi.png";
			break;
		case XKILL_Enums::AUTO:
			path = ":/xkill/images/icons/cross_hairs/firing_auto.png";
			break;
		default:
			path = ":/xkill/images/icons/default.png";
			break;
		}

		// Set image to label
		ui.label_firingMode->setPixmap(path);
	}

	//if(firingModeFade > 0.0f)
	//{
	//	firingModeFade -= SETTINGS->trueDeltaTime;

	//	// Show progress bar if hidden
	//	if(ui.label_firingMode->isHidden())
	//		ui.label_firingMode->show();
	//}
	//else
	//{
	//	// Hide progress-bar if shown
	//	if(!ui.label_firingMode->isHidden())
	//		ui.label_firingMode->hide();
	//}

	 
	// Update scoreboard progress bars
	bool isEnabled_cycleLimit = SETTINGS->cycleLimit != 0;
	bool isEnabled_timeLimit = SETTINGS->timeLimit != 0;
	int highestCycles = scoreboard.maxCycles;
	if(isEnabled_cycleLimit && scoreboard.previousMaxCycles != highestCycles)
	{
		scoreboard.previousMaxCycles = highestCycles;

		// Perform update
		int cyclesLimit = SETTINGS->cycleLimit;
		int cycleRatio = (int)((highestCycles/(float)cyclesLimit) * 100);
		ui.progressBar_cycleRatio->setValue(cycleRatio);
		ui.progressBar_cycleRatio->update();
		QString str_cycleRatio = QString::number(highestCycles) + "/" + QString::number(cyclesLimit)  + " Cycles";
		ui.label_cycleRatio->setText(str_cycleRatio);
	}
	int currentTime = SETTINGS->timeLimit - SETTINGS->timeLeft;
	if(isEnabled_timeLimit && scoreboard.previousTime != currentTime)
	{
		scoreboard.previousTime = currentTime;

		// Perform update
		int timeLimit = SETTINGS->timeLimit;
		int timeRatio = (int)((currentTime/(float)timeLimit) * 100);
		ui.progressBar_timeRatio->setValue(timeRatio);
		ui.progressBar_timeRatio->update();
		QString str_timeRatio = QString::number(currentTime) + "/" + QString::number(timeLimit)  + " Time";
		ui.label_timeRatio->setText(str_timeRatio);
	}
}

void Menu_HUD::initScoreboard()
{
	// Init helper class
	scoreboard.init(ptr_splitScreen->ptr_player, ui.frame_scoreboard);

	// Build scoreboard
	while(itrPlayer.hasNext())
	{
		// Add entry into GUI
		AttributePtr<Attribute_Player> ptr_player =	itrPlayer.getNext();

		QHBoxLayout* layout_entry = new QHBoxLayout();

		QLabel* label_process = new QLabel();
		QLabel* label_cycles = new QLabel();
		QLabel* label_priority = new QLabel();

		//label_process->setMaximumWidth(100);
		label_cycles->setMaximumWidth(80);
		label_priority->setMaximumWidth(80);

		layout_entry->addWidget(label_process);
		layout_entry->addWidget(label_cycles);
		layout_entry->addWidget(label_priority);

		ui.verticalLayout_scoreboard->addLayout(layout_entry);


		// Add entry into scoreBoard class
		ScoreboardEntry entry;
		entry.label_process = label_process;
		entry.label_cycles = label_cycles;
		entry.label_priority = label_priority;
		entry.ptr_player = ptr_player;
		scoreboard.addEntry(entry);
	}
}

void Menu_HUD::onEvent( Event* e )
{
	EventType type = e->getType();
	switch(type) 
	{
	case EVENT_UPDATE:
		refresh();
		break;
	case EVENT_PLAYER_TARGET_HIT:
		{
			// If player hitting a target is THIS player
			// display hit feedback for a while
			if(((Event_PlayerTargetHit*)e)->ptr_player == ptr_splitScreen->ptr_player)
			{
				crosshair_targetHitFade = 0.1f;
			}
		}
		break;
	case EVENT_PLAYER_TAKING_DAMAGE:
		{
			// If player getting hit is THIS player
			// display hit feedback for a while
			if(((Event_PlayerTakingDamage*)e)->ptr_player == ptr_splitScreen->ptr_player)
			{
				hitOverlayFade = 0.1f;
			}
		}
		break;
	default:
		break;
	}
}

Menu_HUDManager::Menu_HUDManager( QWidget* parent ) : QObject(parent)
{
	ATTRIBUTES_INIT_ALL;

	this->parent = parent;
	isEnabled = false;

	// Events
	SUBSCRIBE_TO_EVENT(this, EVENT_ENABLE_HUD);
	SUBSCRIBE_TO_EVENT(this, EVENT_SPLITSCREEN_CHANGED);
}

void Menu_HUDManager::mapHudsToSplitscreen()
{
	// Delete previous huds
	for(int i=0; i<huds.size(); i++)
	{
		delete huds[i];
	}
	huds.clear();

	// Map new HUDs to split screens
	while(itrSplitScreen.hasNext())
	{
		AttributePtr<Attribute_SplitScreen> ptr_splitScreen = itrSplitScreen.getNext();
		huds.push_back(new Menu_HUD(ptr_splitScreen, parent));
		if(isEnabled)
			huds.back()->show();
	}
}

void Menu_HUDManager::onEvent( Event* e )
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_ENABLE_HUD:

		if(((Event_EnableHud*)e)->enableHud)
		{
			isEnabled = true;

			for(int i=0; i<huds.size(); i++)
				huds[i]->show();
		}
		else
		{
			isEnabled = false;

			for(int i=0; i<huds.size(); i++)
				huds[i]->hide();
		}


		break;
	case EVENT_SPLITSCREEN_CHANGED:
		mapHudsToSplitscreen();
		break;
	default:
		break;
	}
}
