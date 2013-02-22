#pragma once

#include <QtGui/QMainWindow>
#include <xkill-utilities/Util.h>

#include "Menu_Main.h"
#include <xkill-utilities/WeaponStructs.h>
#include <QtGui/QProgressBar>
#include <vector>
class Attribute_SplitScreen;

class Menu_HUD : public QMainWindow , IObserver
{
private:
	AttributePtr<Attribute_SplitScreen> splitScreen;

public:
	Menu_HUD(AttributePtr<Attribute_SplitScreen> splitScreen)
	{
		this->splitScreen = splitScreen;
	}
	void mapPositionToParent()
	{
		int x = 20 + splitScreen->ssTopLeftX;
		int y = splitScreen->ssTopLeftY + splitScreen->ssHeight - this->height();
		move(x, y);
	}
	void refresh()
	{
	}
	void computeNewResolution()
	{
		/*resize();*/
	}

	void onEvent(Event* e)
	{
		EventType type = e->getType();
		switch (type) 
		{
		case EVENT_WINDOW_MOVE:
			mapPositionToParent();
			break;
		case EVENT_SPLITSCREEN_CHANGED:
			computeNewResolution();
			break;
		default:
			break;
		}
	}
};

class Menu_HUDManager : public QObject , IObserver
{
	Q_OBJECT

private:
	std::vector<Menu_HUD*> huds;
	QWidget* parent;

public:
	Menu_HUDManager(QWidget* parent);
	void updateHuds();
	
	void mapHudsToSplitscreen();
	void computeNewPosition(Event_WindowMove* e)
	{
		e->pos;
	}
	void computeNewResolution(Event_WindowResize* e)
	{
	}

	void onEvent(Event* e);
};