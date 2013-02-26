#include "Menu_HUD.h"

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

void Menu_HUDManager::updateHuds()
{
	// Update huds
	int index = 0;
	while(itrSplitScreen.hasNext())
	{
		AttributePtr<Attribute_SplitScreen> ptr_splitScreen = itrSplitScreen.getNext();
		//huds[index]->refresh(ptr_splitScreen);
		index++;
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
	/*if(ui.progressBar_health->value() == 100)
		healthFade = 0.0f;*/
		
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
		// Hide progress bar if shown
		if(!ui.progressBar_health->isHidden())
			ui.progressBar_health->hide();
	}
}

class HudMessage
{
private:
	float lifetime;

public:
	HudMessage()
	{
		lifetime = 5.0f;
	}
	bool isExpired()
	{
		// Decrement timer
		if(lifetime > 0.0f)
			lifetime -= SETTINGS->trueDeltaTime;

		// Check expiration condition
		if(lifetime <= 0.0f)
			return true;

		return false;
	}
};

class HudMessage_Manager
{
private:
	SimpleQueue<HudMessage*> stack;

public:
	HudMessage_Manager()
	{
		stack.push(new HudMessage());
	}
	~HudMessage_Manager()
	{
		for(int i=0; i<stack.count(); i++)
			delete stack.at(i);
	}

	void update()
	{
		for(int i=0; i<stack.count(); i++)
		{
			if(stack.at(i)->isExpired())
			{
				delete stack.pop();
			}
		}
	}
};

Menu_HUD::Menu_HUD( AttributePtr<Attribute_SplitScreen> splitScreen, QWidget* parent ) : QWidget(parent)
{
	ui.setupUi(this);
	this->splitScreen = splitScreen;
	Float2 pos(splitScreen->ssTopLeftX, splitScreen->ssTopLeftY);
	move(splitScreen->ssTopLeftX, splitScreen->ssTopLeftY);
	resize(splitScreen->ssWidth, splitScreen->ssHeight);
	hide();

	QWidget::setAttribute(Qt::WA_ShowWithoutActivating);
	QWidget::setAttribute(Qt::WA_TransparentForMouseEvents);

	SUBSCRIBE_TO_EVENT(this, EVENT_UPDATE);
	
	mapToSplitscreen();
}

void Menu_HUD::onEvent( Event* e )
{
	EventType type = e->getType();
	switch (type) 
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
