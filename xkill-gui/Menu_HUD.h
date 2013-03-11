#pragma once

#include <QtGui/QMainWindow>
#include <xkill-utilities/Util.h>

#include <QtGui/QMainWindow>
#include <xkill-utilities/WeaponStructs.h>
#include <QtGui/QProgressBar>
#include <vector>

#include "WeaponInfoHud.h"
#include "ScoreBoard.h"
#include "HudMessage.h"

#include "ui_Menu_HUD.h"

class Attribute_SplitScreen;

class Menu_HUD : public QWidget, IObserver
{
private:
	AttributePtr<Attribute_SplitScreen> ptr_splitScreen;
	Ui::Menu_HUD ui;
	HudMessage_Manager hudMessage_manager;
	ScoreBoard scoreboard;
	WeaponInfoHud weaponInfoHud;

	float hitOverlayFade;
	float scoreboardFade;
	float healthFade;
	float ammoFade;
	float firingModeFade;
	int index_crosshair;
	int index_firingMode;
	int prev_ammoRatio;
	int prev_reloadRatio;

public:
	Menu_HUD(AttributePtr<Attribute_SplitScreen> splitScreen, QWidget* parent);
	~Menu_HUD();

	void mapToSplitscreen();
	void refresh();
	void initScoreboard();
	void onEvent(Event* e);
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
	void mapHudsToSplitscreen();
	void onEvent(Event* e);
};