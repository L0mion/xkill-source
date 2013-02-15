#pragma once

#include <QtGui/QKeyEvent> // needed to grab mouse input
#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/WeaponStructs.h>
#include <QtGui/QProgressBar>

#include "Menu_Main.h"
#include "Menu_ScoreBoard.h"
#include "Menu_InGame.h"
#include <vector>

class Attribute_SplitScreen;

class HUDWindow : public QMainWindow
{
private:
	int id;
	QProgressBar *progressBar_health;
    QProgressBar *progressBar_ammo;
	QLabel *label_weaponType;
    QLabel *label_ammoType;
	QLabel *label_totalAmmoLeft;
	XKILL_Enums::AmmunitionType ammo;
	XKILL_Enums::FiringModeType weapon;
	
	QHBoxLayout *horizontalLayout;
	 QVBoxLayout *verticalLayout;

public:
	HUDWindow(QWidget* parent, int id);

	void parentMoveEvent(AttributePtr<Attribute_SplitScreen> splitScreen);
	void update(AttributePtr<Attribute_SplitScreen> player);
};

class HUDManager
{
private:
	std::vector<HUDWindow*> huds;
	QWidget* parent;
public:
	HUDManager();
	HUDManager(QWidget* parent);

	void update();

	void createHUD();

	void parentMoveEvent();
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

	void onEvent(Event* e);

	void onUpdate(float delta);

	// Behavior on keyboard input
	void keyPressEvent(QKeyEvent* e);

	void keyReleaseEvent(QKeyEvent* e);

	void moveEvent();
};
