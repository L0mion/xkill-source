#pragma once

#include <QtGui>
#include <QKeyEvent> // needed to grab mouse input
#include <xkill-utilities/IObserver.h>

#include "Menu_Main.h"
#include "Menu_ScoreBoard.h"

class MenuManager : public IObserver
{
private:
	Menu_Main* main;
	Menu_ScoreBoard* scoreBoard;

public:
	MenuManager(QWidget* parent)
	{
		scoreBoard = new Menu_ScoreBoard(parent);
		main = new Menu_Main(parent);

		SUBSCRIBE_TO_EVENT(this, EVENT_UPDATE);
	}

	void onEvent(Event* e)
	{
		EventType type = e->getType();
		static int refreshRate = 20;
		static int test = refreshRate;
		switch(type) 
		{
		case EVENT_UPDATE:
			// HACK: Makes the menu update every 20 frame
			test--;
			if(test<0)
			{
				scoreBoard->onUpdate(1.0f);
				test = refreshRate;
			}
			break;
		default:
			break;
		}
	}

	void onUpdate(float delta)
	{

	}

	// Behavior on keyboard input
	void keyPressEvent(QKeyEvent* e)
	{
		switch (e->key()) 
		{
		case Qt::Key_Escape:
			main->toggleMenu();
			break;
		case Qt::Key_Tab:
			scoreBoard->toggleMenu();
			break;
		default:
			break;
		}
	}

	void moveEvent()
	{
		main->parentMoveEvent();
		scoreBoard->parentMoveEvent();
	}
};
