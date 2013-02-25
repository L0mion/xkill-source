#pragma once

#include <QtGui/QMainWindow>
#include <xkill-utilities/Util.h>

#include <QtGui/QMainWindow>
#include <xkill-utilities/WeaponStructs.h>
#include <QtGui/QProgressBar>
#include <vector>

#include "ui_Menu_HUD.h"

class Attribute_SplitScreen;

class Menu_HUD : public QWidget , IObserver
{
private:
	AttributePtr<Attribute_SplitScreen> splitScreen;
	Ui::Menu_HUD ui;

public:
	Menu_HUD(AttributePtr<Attribute_SplitScreen> splitScreen, QWidget* parent);
	void mapPositionToParent()
	{
		int x = 20 + splitScreen->ssTopLeftX;
		int y = splitScreen->ssTopLeftY + splitScreen->ssHeight - this->height();
		move(x, y);

		SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_MOVE);
		SUBSCRIBE_TO_EVENT(this, EVENT_SPLITSCREEN_CHANGED);
	}
	void refresh();
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
	bool isEnabled;

public:
	Menu_HUDManager(QWidget* parent);
	void updateHuds();
	
	void mapHudsToSplitscreen();
	void computeNewPosition(Event_WindowMove* e)
	{

	}
	void computeNewResolution(Event_WindowResize* e)
	{
	}

	void onEvent(Event* e);
};