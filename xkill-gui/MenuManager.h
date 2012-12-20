#pragma once

#include <QtGui>
#include <QKeyEvent> // needed to grab mouse input
#include <xkill-utilities/IObserver.h>


#include "Menu_Main.h"
#include "Menu_ScoreBoard.h"
#include "Menu_InGame.h"

class MenuManager : public IObserver
{
private:
	Menu_Main* mainMenu;
	Menu_ScoreBoard* scoreBoard;
	Menu_InGame* inGameMenu;
	

public:
	MenuManager(QWidget* parent);

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
		case EVENT_END_DEATHMATCH:
			scoreBoard->toggleMenu(false);
			inGameMenu->toggleMenu(false);
			mainMenu->toggleMenu(true);
			break;
		default:
			break;
		}
	}

	void onUpdate(float delta)
	{

	}
	// Behavior on keyboard input
	void keyPressEvent(QKeyEvent* e);

	void keyReleaseEvent(QKeyEvent* e);

	void moveEvent()
	{
		mainMenu->parentMoveEvent();
		scoreBoard->parentMoveEvent();
		inGameMenu->parentMoveEvent();
	}
};
