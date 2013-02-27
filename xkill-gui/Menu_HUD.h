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
	float healthFade;
	float ammoFade;
	float healthRatio;
	int ammoRatio;

public:
	Menu_HUD(AttributePtr<Attribute_SplitScreen> splitScreen, QWidget* parent);
	~Menu_HUD();

	void mapToSplitscreen();
	void refresh();
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