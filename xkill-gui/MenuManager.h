#pragma once

#include <QtGui/QKeyEvent> // needed to grab mouse input
#include <xkill-utilities/IObserver.h>

#include "Menu_Main.h"
#include "Menu_ScoreBoard.h"
#include "Menu_InGame.h"
#include <vector>


class Attribute_Player;

class HUDWindow : public QMainWindow
{
private:
	int id;
	QLabel *label_health;
    QLabel *label_ammo;
	QLabel *label_weaponType;
    QLabel *label_ammoType;
	Attribute_WeaponStats::AmmunitionType ammo;
	Attribute_WeaponStats::FiringMode weapon;
	
	QHBoxLayout *horizontalLayout;

public:
	HUDWindow(QWidget* parent, int id);

	void parentMoveEvent();
	void update(Attribute_Player* player);
};

class HUDManager
{
private:
	std::vector<HUDWindow*> huds;
	QWidget* parent;
public:
	HUDManager()
	{
	}
	void update();

	HUDManager(QWidget* parent);

	void createHUD()
	{
		for(int i=0; i<5; i++)
		{
			huds.push_back(new HUDWindow(parent, i));
		}
	}

	void parentMoveEvent()
	{
		for(int i=0; i<(int)huds.size(); i++)
		{
			huds[i]->parentMoveEvent();
		}
	}
};


class MenuManager : public IObserver
{
private:
	Menu_Main* mainMenu;
	Menu_ScoreBoard* scoreBoard;
	Menu_InGame* inGameMenu;
	HUDManager hudManager;

public:
	MenuManager(QWidget* parent);

	void onEvent(Event* e)
	{
		EventType type = e->getType();
		static int refreshRate = 2;
		static int test = refreshRate;
		switch(type) 
		{
		case EVENT_UPDATE:
			// HACK: Makes the menu update every 20 frame
			test--;
			if(test<0)
			{
				hudManager.update();
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
		hudManager.parentMoveEvent();
	}
};
