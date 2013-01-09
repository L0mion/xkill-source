#pragma once

#include <QtGui/QKeyEvent> // needed to grab mouse input
#include <xkill-utilities/IObserver.h>

#include "Menu_Main.h"
#include "Menu_ScoreBoard.h"
#include "Menu_InGame.h"
#include <vector>


class HUDWindow : public QMainWindow
{
private:
	QLabel *label_health;
    QLabel *label_ammo;
	QLabel *label_weaponType;
    QLabel *label_ammoType;
	
	QVBoxLayout *verticalLayout;
	QHBoxLayout *horizontalLayout;
	QSpacerItem *verticalSpacer;

public:
	HUDWindow(QWidget* parent) : QMainWindow(parent)
	{
		QWidget::setAttribute(Qt::WA_TranslucentBackground, true);
		QWidget::setWindowFlags(Qt::SplashScreen);
		show();

		// build menu
		verticalLayout = new QVBoxLayout(this);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        label_health = new QLabel(this);
        label_health->setObjectName("label_health");
		verticalLayout->addWidget(label_health);

		
		//setWindowFlags(Qt::WindowStaysOnBottomHint);
		QTabWidget* tab = new QTabWidget(this);
		setCentralWidget(tab);
		
	}
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

	HUDManager(QWidget* parent)
	{
		this->parent = parent;

		createHUD();
	}

	void createHUD()
	{
		for(int i=0; i<5; i++)
		{
			huds.push_back(new HUDWindow(parent));
		}
	}

	void parentMoveEvent()
	{
		for(int i=0; i<(int)huds.size(); i++)
		{
			QPoint pos = parent->pos();
			pos = QPoint();
			int x = pos.x() + parent->width()/2 - huds[i]->width()/2;
			int y = pos.y() + parent->height()/2 - huds[i]->height()/2;
			huds[i]->move(x, y);
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
		hudManager.parentMoveEvent();
	}
};
