#include "Menu_HUD.h"
#include <xkill-utilities/Converter.h>
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
	screenSize.x = splitScreen->ssWidth;
	screenSize.y = splitScreen->ssHeight;

	Float2 centerPos;
	centerPos.x = screenSize.x * 0.5f;
	centerPos.y = screenSize.y * 0.5f;


	// Move center HUD to center
	ui.frame_center->move(centerPos.x - ui.frame_center->width()* 0.5f, centerPos.y - ui.frame_center->height()* 0.5f);
	
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

	// Move HUD messages to center
	hudMessage_manager.move(centerPos);
}

void Menu_HUD::refresh()
{
	AttributePtr<Attribute_Player>		ptr_player		=	splitScreen->ptr_player;
	AttributePtr<Attribute_Health>		ptr_health		=	ptr_player->ptr_health;
	AttributePtr<Attribute_WeaponStats>	ptr_weaponStats	=	ptr_player->ptr_weaponStats;

	Ammunition* ammunition = &ptr_weaponStats->ammunition[ptr_weaponStats->currentAmmunitionType];
	FiringMode* firingMode = &ptr_weaponStats->firingMode[ptr_weaponStats->currentFiringModeType];
	int ammoIndex = ammunition->type;
	float fadeTime = 1.0f;


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


	// Scheduling
	/*ui.label_priority_advantage->setNum((int)ptr_health->health);

	ui.label_schedulingTimer->setNum((int)ptr_health->health);*/

	//ptr_player
	//while(itrPlayer.hasNext())	// Loop through all player and find if anyone has top priority
	//{
	//	//Attribute_Player* player = itrPlayer.getNext();
	//	AttributePtr<Attribute_Player> player = itrPlayer.getNext();

	//	if(player->priority > 0)
	//	{
	//		if(player->priority > topPriority)		// Current player had higher priority than last top player
	//		{
	//			topPlayerIndex = itrPlayer.storageIndex();
	//			topPriority = player->priority;
	//			topPriorityIsTied = false;
	//		}
	//		else if(player->priority == topPriority)	// Current player had the same priority as last top player
	//		{
	//			topPriorityIsTied = true;
	//		}
	//	}
	//}
}

Menu_HUD::Menu_HUD( AttributePtr<Attribute_SplitScreen> splitScreen, QWidget* parent ) : QWidget(parent)
{
	ui.setupUi(this);
	this->splitScreen = splitScreen;
	hudMessage_manager.init(this, splitScreen);

	Float2 pos(splitScreen->ssTopLeftX, splitScreen->ssTopLeftY);
	move(splitScreen->ssTopLeftX, splitScreen->ssTopLeftY);
	resize(splitScreen->ssWidth, splitScreen->ssHeight);
	hide();

	QWidget::setAttribute(Qt::WA_ShowWithoutActivating);
	QWidget::setAttribute(Qt::WA_TransparentForMouseEvents);

	SUBSCRIBE_TO_EVENT(this, EVENT_UPDATE);
	
	mapToSplitscreen();
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
