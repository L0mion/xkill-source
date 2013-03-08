#include "Menu_HUD.h"

#include <QtCore/QDateTime>
#include <xkill-utilities/Converter.h>
#include <QtGui/QMovie>
ATTRIBUTES_DECLARE_ALL;

Menu_HUDManager::Menu_HUDManager( QWidget* parent ) : QObject(parent)
{
	ATTRIBUTES_INIT_ALL;

	this->parent = parent;
	isEnabled = false;

	// Events
	SUBSCRIBE_TO_EVENT(this, EVENT_ENABLE_HUD);
	SUBSCRIBE_TO_EVENT(this, EVENT_SPLITSCREEN_CHANGED);
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

	// Move center HUD to center
	ui.label_aim->move(centerPos.x - ui.label_aim->width()* 0.5f, centerPos.y - ui.label_aim->height()* 0.5f);
	ui.label_firingMode->move(centerPos.x - ui.label_firingMode->width()* 0.5f + 10, centerPos.y - ui.label_firingMode->height()* 0.5f);

	// Move scoreboard to center
	ui.frame_scoreboard->hide();
	ui.frame_scoreboard->move(centerPos.x - ui.frame_scoreboard->width()* 0.5f, centerPos.y - ui.frame_scoreboard->height()* 0.5f);

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
	bottomPos.y = screenSize.y - screenSize.x*0.005f - ui.frame_bottom->height()* 1.0f;
	ui.frame_bottom->move(bottomPos.x, bottomPos.y);
	Float2 bottomCenterPos = bottomPos;
	bottomCenterPos.x = screenSize.x * 0.5f - ui.groupBox_bottomCenter->width()* 0.5f;
	ui.groupBox_bottomCenter->move(bottomCenterPos.x, bottomCenterPos.y);

	// Move top HUD to top
	Float2 topPos;
	ui.frame_top->resize(screenSize.x - screenSize.x*0.00f * 2, ui.frame_top->height());
	topPos.x = screenSize.x * 0.5f - ui.frame_top->width()* 0.5f;
	topPos.y = screenSize.x*0.005f;
	ui.frame_top->move(topPos.x, topPos.y);

	// Move HUD messages to center
	hudMessage_manager.move(centerPos);


	// Create weapon info
	weaponInfoHud.init(ui.groupBox_weaponInfo, ui.verticalLayout_weaponInfo);
	ui.groupBox_weaponInfo2->hide();
	Float2 weaponInfoPos;
	weaponInfoPos.x = ui.frame_bottom->pos().x() + ui.frame_bottom->width() - 15 - weaponInfoHud.width();
	weaponInfoPos.y = ui.frame_bottom->pos().y();
	weaponInfoHud.setPosition(weaponInfoPos);
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
	// Update weapon info hud
	//

	weaponInfoHud.update(firingIndex);

	//
	// Show ammunition info
	//

	int numAmmo = firingMode->nrOfShotsLeftInClip[ammoIndex];
	int clipSize = firingMode->clipSize;
	ui.label_ammo->setNum(numAmmo);
	ui.label_totalAmmo->setNum((int)ptr_weaponStats->ammunition[ammoIndex].currentTotalNrOfShots);
	int prev_ammoRatio = ui.progressBar_ammo->value();
	int ammoRatio = (int)((numAmmo / (float)clipSize) * 100);
	int reloadRatio = (int)((firingMode->reloadTimeLeft / (float)firingMode->reloadTime) * 100);
	
	// Show menu if health has changed otherwise fade after a few seconds
	if(reloadRatio == 100 && ammoRatio != prev_ammoRatio)
	{
		prev_ammoRatio = ammoRatio;
		ammoFade = fadeTime;
		ui.progressBar_ammo->setValue(ammoRatio);
	}
	if(reloadRatio != prev_reloadRatio)
	{
		prev_reloadRatio = reloadRatio;
		ammoFade = fadeTime;
		ui.progressBar_ammo->setValue(100 - reloadRatio);
	}
	
	// Hide bar if full
	if(ammoRatio == 100)
		ammoFade = 0.0f;

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

	ui.label_health->setNum((int)ptr_health->health);
	ui.label_priority->setNum(ptr_player->priority);
	ui.label_cycles->setNum(ptr_player->cycles);
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
		}
	}

	//
	// Update Scoreboard
	//
	
	scoreboard.refresh();
	if(ptr_player->detectedAsDead)
	{
		// Show scoreboard if delay has expired
		if(scoreboardFade > 0.0f)
			scoreboardFade -= SETTINGS->trueDeltaTime;
		if(scoreboardFade <= 0.0f)
		{
			// Show scoreboard if hidden
			if(ui.frame_scoreboard->isHidden())
			{
				ui.frame_scoreboard->show();
			}
		}
	}
	else
	{
		// Reset scoreboard timer
		scoreboardFade = 1.0f;

		// Hide scoreboard if shown
		if(!ui.frame_scoreboard->isHidden())
		{
			ui.frame_scoreboard->hide();
		}
	}

	


	//
	// Scheduling info
	//

	// Determine priority advantage
	{
		std::string str_priorityAdvantage = "";

		int scoreDiff = scoreboard.maxCycles - ptr_player->cycles;

		// If first player, show score difference to second closest
		if(scoreDiff == 0)
			scoreDiff = scoreboard.maxCycles - scoreboard.secondMaxPriority;
		// ELSE: Show score difference to second closest
		else
			scoreDiff = ptr_player->cycles - scoreboard.maxCycles;

		// Set label
		
		if(scoreDiff >= 0)
			ui.label_priority_advantage->setText("+" +QString::number(scoreDiff));
		else
			ui.label_priority_advantage->setNum(scoreDiff);
			
	}
	
	
	QString str_time = QDateTime::fromTime_t(SETTINGS->timeUntilScheduling).toString("mm:ss");
	ui.label_schedulingTimer->setText(str_time);


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

		 // EASTER EGG
		 if(index_crosshair == XKILL_Enums::EXPLOSIVE)
		 {
			 // If a specific user
			 std::string username = getenv( "USERNAME" );
			 if(username == "Professor Membrane")
			 {
				 QMovie* movie = new QMovie(this);
				 movie->setCacheMode(QMovie::CacheAll);
				 movie->setFileName("../../xkill-resources/xkill-gui/images/animations/tmp.gif");
				 ui.label_xAmmo->setMovie(movie);
				 ui.label_xAmmo->setScaledContents(true);
				 QSize sizeLimit(100, 100);
				 ui.label_xAmmo->setMinimumSize(sizeLimit);
				 ui.label_xAmmo->setMaximumSize(sizeLimit);
				 movie->start();
			 }
		 }
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

	if(firingModeFade > 0.0f)
	{
		firingModeFade -= SETTINGS->trueDeltaTime;

		// Show progress bar if hidden
		if(ui.label_firingMode->isHidden())
			ui.label_firingMode->show();

		// Update value
		ui.progressBar_health->setValue(healthRatio);
		ui.progressBar_health->update();
	}
	else
	{
		// Hide progress-bar if shown
		if(!ui.label_firingMode->isHidden())
			ui.label_firingMode->hide();
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
	
	hudMessage_manager.init(this, ptr_splitScreen);
	mapToSplitscreen();
	initScoreboard();
}

void Menu_HUD::onEvent(Event* e)
{
	EventType type = e->getType();
	switch(type) 
	{
	case EVENT_UPDATE:
		refresh();
		break;
	default:
		break;
	}
}

Menu_HUD::~Menu_HUD()
{
	UNSUBSCRIBE_TO_EVENTS(this);
}


HudMessage::HudMessage( Event_PostHudMessage* e, QWidget* parent)
{
	// Create label
	_label = new QLabel(e->message.c_str());
	_label->setAlignment(Qt::AlignHCenter);
	_label->setParent(parent);

	// Apply stylesheet
	_label->setStyleSheet(e->styleSheet.c_str());

	// Display for a certain time
	_lifetime = 3.0f;

	// Show label
	_label->show();
}

void HudMessage::updatePosition()
{
	// Interpolate position
	Float2 currentPos;
	currentPos.x = _label->pos().x();
	currentPos.y = _label->pos().y();

	float factor = 5.0f * SETTINGS->trueDeltaTime;
	if(factor > 1.0f)
		factor = 1.0f;
	Float2 newPos = Float2::lerp(&currentPos, &_targetPosition, factor);

	_label->move(newPos.x, newPos.y);
}

void HudMessage::setPosition(Float2 position)
{
	setTargetPosition(position);

	position = _targetPosition;

	// An additional offset,
	// creates a cool interpolation
	// when a new label is created
	position = position + Float2(_label->width() * 0.0f, -_label->height() * 1.0f);

	// Move
	_label->move(position.x, position.y);
}

void HudMessage_Manager::addMessage( Event_PostHudMessage* e )
{
	Event_PostHudMessage::Receiver re= e->receiver;

	// Ignore messages aimed at other players
	if(e->receiver == Event_PostHudMessage::RECEIVER_ONLY_SUBJECT)
	{
		if(splitScreen->ptr_player != e->ptr_subject_player)
			return;
	}
	if(e->receiver == Event_PostHudMessage::RECEIVER_ALL_BUT_SUBJECT)
	{
		if(splitScreen->ptr_player == e->ptr_subject_player)
			return;
	}

	// Limit simultaneous show messages 
	// to not overwhelm the player
	const int kMaxMessages = 8;
	if(stack.count() + 1 > kMaxMessages)
		removeTopMessage();

	// Add message to stack
	HudMessage* m = new HudMessage(e, parent);
	stack.push(m);

	// Show new messages above old messages
	int numStacks = stack.count();
	int offset = 20*3.8f;
	Float2 newPos;
	newPos.x = position.x;
	newPos.y = position.y + offset;
	m->setPosition(newPos);
	for(int i=numStacks-1; i>=0; i--)
	{
		int height = stack.at(i)->getHeight();
		offset += height;

		Float2 newPos;
		newPos.x = position.x;
		newPos.y = position.y + offset - height * 0.5f;

		stack.at(i)->setTargetPosition(newPos);
	}
}

void ScoreBoard::syncLabelsWithPlayers()
{
	for(int i=0; i<entries.size(); i++)
	{
		ScoreboardEntry* e = &entries.at(i);

		// Detect if label has changed
		if(e->ptr_player->playerName != e->playerName)
			e->isChanged = true;
		if(e->ptr_player->cycles != e->cycles)
			e->isChanged = true;
		if(e->ptr_player->priority != e->priority)
			e->isChanged = true;
		e->isChanged = true;

		// Update label
		if(e->isChanged)
		{
			e->isChanged = false;

			// Set text
			e->label_process->setText(e->ptr_player->playerName.c_str());
			e->label_cycles->setNum(e->ptr_player->cycles);
			e->label_priority->setNum(e->ptr_player->priority);

			// Empty style sheets
			std::string sheet_process = "";
			std::string sheet_cycles = "";
			std::string sheet_priority = "";

			// Apply extra stuff if we're at the current player
			if(e->ptr_player == ptr_current_player)
			{
				sheet_process += "background-color: rgba(255, 255, 255, 100); font-weight: bold;";
				sheet_cycles += "background-color: rgba(255, 255, 255, 100); font-weight: bold;";
				sheet_priority += "background-color: rgba(255, 255, 255, 100); font-weight: bold;";
			}

			// Apply extra stuff if we have most cycles
			if(e->ptr_player->cycles == maxCycles)
			{
				sheet_cycles += "background-color: rgba(0, 255, 0, 100);";
			}

			// Apply extra stuff if we have most priority
			if(e->ptr_player->priority == maxPriority)
			{
				sheet_priority += "background-color: rgba(0, 255, 0, 100);";
			}

			// Apply style sheet
			e->label_process->setStyleSheet(sheet_process.c_str());
			e->label_cycles->setStyleSheet(sheet_cycles.c_str());
			e->label_priority->setStyleSheet(sheet_priority.c_str());


			// Resize scoreboard to fit long
			// player names if needed
			const int kMinLabelSize = 150;
			int labelSize = e->label_process->sizeHint().width();
			if(labelSize < kMinLabelSize)
				labelSize = kMinLabelSize;
			if(labelSize > maxLabelSize)
			{
				maxLabelSize = labelSize;

				const int kPadding = 75;
				const int kColumnWidth = 100;
				int new_scoreboardWidth = kPadding;
				int a = e->label_process->sizeHint().width();
				a = e->label_process->width();

				new_scoreboardWidth += labelSize;
				new_scoreboardWidth += kColumnWidth;
				new_scoreboardWidth += kColumnWidth;

				// Resize scorboard
				frame_scoreboard->resize(new_scoreboardWidth, frame_scoreboard->height());

				// Reposition scoreboard to center to acommodate change in size
				QWidget* parent_scoreboard = frame_scoreboard->parentWidget();
				Float2 centerPos;
				centerPos.x = parent_scoreboard->width() * 0.5f;
				centerPos.y = parent_scoreboard->height() * 0.5f;
				frame_scoreboard->move(centerPos.x - frame_scoreboard->width()* 0.5f, centerPos.y - frame_scoreboard->height()* 0.5f);
			}
		}
	}
}

void WeaponInfoHud::update( int firingIndex )
{
	// Detect change in firing mode
	if(_firingIndex != firingIndex)
	{
		// Clear previous selection
		if(_firingIndex >= 0)
			_firingIcons.at(_firingIndex)->setStyleSheet("");

		// Select new
		_firingIndex = firingIndex;
		_firingIcons.at(_firingIndex)->setStyleSheet("background: rgba(0, 255, 255, 100);");

		// Set face timer
		_firingFade = 1.0f;
	}


	// Fade box
	if(_firingFade > 0.0f)
	{
		_firingFade -= SETTINGS->trueDeltaTime;

		// Show box if hidden
		if(_firingGroupBox->isHidden())
		{

			_firingGroupBox->hide();
		}
	}
	else
	{
		// Hide box if show
		if(!_firingGroupBox->isHidden())
		{
			_firingGroupBox->hide();
		}
	}
}
