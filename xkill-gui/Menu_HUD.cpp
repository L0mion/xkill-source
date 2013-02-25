#include "Menu_HUD.h"

ATTRIBUTES_DECLARE_ALL;

Menu_HUDManager::Menu_HUDManager( QWidget* parent ) : QObject(parent)
{
	ATTRIBUTES_INIT_ALL;

	this->parent = parent;
	isEnabled = false;

	// Events
	SUBSCRIBE_TO_EVENT(this, EVENT_UPDATE);
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

void Menu_HUD::refresh()
{
	Float2 screenSize;
	screenSize.x = splitScreen->ssWidth;
	screenSize.y = splitScreen->ssHeight;


	// Move center HUD to center
	Float2 centerPos;
	centerPos.x = screenSize.x * 0.5f - ui.frame_center->width()* 0.5f;
	centerPos.y = screenSize.y * 0.5f - ui.frame_center->height()* 0.5f;
	ui.frame_center->move(centerPos.x, centerPos.y);

	// Move botton HUD to bottom
	Float2 bottomPos;
	ui.frame_bottom->resize(screenSize.x - screenSize.x*0.00f * 2, ui.frame_bottom->height());
	bottomPos.x = screenSize.x * 0.5f - ui.frame_bottom->width()* 0.5f;
	bottomPos.y = screenSize.y - screenSize.x*0.00f - ui.frame_bottom->height()* 1.0f;
	ui.frame_bottom->move(bottomPos.x, bottomPos.y);


}

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
	
	refresh();
}
