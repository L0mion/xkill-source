#include "Menu_HUD.h"

ATTRIBUTES_DECLARE_ALL;

Menu_HUDManager::Menu_HUDManager( QWidget* parent ) : QObject(parent)
{
	ATTRIBUTES_INIT_ALL;

	this->parent = parent;
}

void Menu_HUDManager::onEvent( Event* e )
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_ENABLE_HUD:
		if(((Event_EnableHud*)e)->enableHud)
		{
		}
		else
		{
		}
	
		break;
	case EVENT_WINDOW_MOVE:
		computeNewPosition((Event_WindowMove*)e);
		break;
	case EVENT_STARTGAME:
		computeNumHuds();
		break;
	case EVENT_WINDOW_RESIZE:
		computeNewResolution((Event_WindowResize*)e);
		break;
	default:
		break;
	}
}

void Menu_HUDManager::computeNumHuds()
{
	// Balance attributes / vs huds
	int numSplitScreen = itrSplitScreen.count();
	while(numSplitScreen > huds.size())
	{
		huds.push_back(new Menu_HUD(parent, huds.size()));
	}
	while(numSplitScreen < huds.size())
	{
		delete huds.back();
		huds.pop_back();
	}	
}

void Menu_HUDManager::updateHuds()
{
	// Update huds
	int index = 0;
	while(itrSplitScreen.hasNext())
	{
		AttributePtr<Attribute_SplitScreen> ptr_splitScreen = itrSplitScreen.getNext();
		huds[index]->refresh(ptr_splitScreen);
		index++;
	}
}
