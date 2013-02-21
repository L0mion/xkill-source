#pragma once

#include <QtGui/QMainWindow>
#include <xkill-utilities/Util.h>

#include "Menu_Main.h"
#include <xkill-utilities/WeaponStructs.h>
#include <QtGui/QProgressBar>
#include <vector>
class Attribute_SplitScreen;

class Menu_HUD : public QMainWindow
{
private:

public:
	Menu_HUD(QWidget* parent, int id)
	{

	}
	void refresh(AttributePtr<Attribute_SplitScreen> splitScreen)
	{

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
	
	void computeNumHuds();
	void computeNewPosition(Event_WindowMove* e)
	{
		e->pos;
	}
	void computeNewResolution(Event_WindowResize* e)
	{
	}

	void onEvent(Event* e);
};