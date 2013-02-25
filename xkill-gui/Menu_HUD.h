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
	Menu_HUD(AttributePtr<Attribute_SplitScreen> splitScreen, QWidget* parent) : QWidget(parent)
	{
		ui.setupUi(this);
		QWidget::setAttribute(Qt::WA_ShowWithoutActivating);
		QWidget::setAttribute(Qt::WA_TransparentForMouseEvents);
		

		this->splitScreen = splitScreen;
		hide();
		refresh();
	}
	void mapPositionToParent()
	{
		int x = 20 + splitScreen->ssTopLeftX;
		int y = splitScreen->ssTopLeftY + splitScreen->ssHeight - this->height();
		move(x, y);
	}
	void refresh()
	{
		move(splitScreen->ssTopLeftX,splitScreen->ssTopLeftY);
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
			refresh();
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