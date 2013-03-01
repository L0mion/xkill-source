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

	// Move hud messages to center
	hudMessage_manager.move(centerPos);
}

void Menu_HUD::refresh()
{
	AttributePtr<Attribute_Player>		player		=	splitScreen->ptr_player;
	AttributePtr<Attribute_Health>		health		=	player->ptr_health;
	AttributePtr<Attribute_WeaponStats>	weaponStats	=	player->ptr_weaponStats;

	Ammunition* ammunition = &weaponStats->ammunition[weaponStats->currentAmmunitionType];
	FiringMode* firingMode = &weaponStats->firingMode[weaponStats->currentFiringModeType];
	int ammoIndex = ammunition->type;
	float fadeTime = 1.0f;


	//
	// Show ammunition info
	//

	int numAmmo = firingMode->nrOfShotsLeftInClip[ammoIndex];
	int clipSize = firingMode->clipSize;
	ui.label_ammo->setNum(numAmmo);
	ui.label_totalAmmo->setNum((int)weaponStats->ammunition[ammoIndex].currentTotalNrOfShots);
	int prev_ammoRatio = ui.progressBar_ammo->value();
	int ammoRatio = (int)((numAmmo / (float)clipSize) * 100);
	int reloadRatio = (int)((firingMode->reloadTimeLeft / (float)firingMode->reloadTime) * 100);
	
	// Show menu if health has changed otherwise fade after a few seconds
	if(ammoRatio != prev_ammoRatio)
	{
		ammoFade = fadeTime;

		// Show reload bar
		if(reloadRatio < 100)
		{
			ui.progressBar_ammo->setValue(100 - reloadRatio);
		}
		// Show ammo bar
		else
		{
			ui.progressBar_ammo->setValue(ammoRatio);
		}
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

	ui.label_health->setNum((int)health->health);
	int healthRatio = (int)((health->health / health->maxHealth) * 100);

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

HudMessage::HudMessage( Event_PostHudMessage* e, QWidget* parent )
{
	// Apply message
	message = new QLabel(e->message.c_str());
	message->setAlignment(Qt::AlignHCenter);

	// Draw label in parent window
	message->setParent(parent);

	// Apply stylesheet
	message->setStyleSheet(e->styleSheet.c_str());

	// Display for a certain time
	lifetime = 3.0f;

	// Show label
	message->show();
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
	// to 5 to not overwhelm the player
	if(stack.count() + 1 > 5)
		removeTopMessage();

	// Add message to stack
	stack.push(new HudMessage(e, parent));

	// Show new messages above old messages
	int numStacks = stack.count();
	int offset = 20*3.8f;
	for(int i=numStacks-1; i>=0; i--)
	{
		int height = stack.at(i)->getHeight();
		offset += height;

		Float2 newPos;
		newPos.x = position.x;
		newPos.y = position.y + offset - height * 0.5f;

		stack.at(i)->move(newPos);
	}
}
