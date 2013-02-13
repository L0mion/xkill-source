#pragma once

#include <QtGui/QMainWindow>
#include <QtGui/QKeyEvent>
#include <xkill-utilities/Util.h>
#include "ui_MainMenu2.h"

class Menu_Main2 : public QMainWindow, IObserver
{
	Q_OBJECT

private:
	Ui::MainWindow ui;
	QWidget* parent;

public:
	Menu_Main2(QWidget* parent) : QMainWindow()
	{
		this->parent = parent;
		ui.setupUi(this);
		QWidget::setWindowFlags(Qt::FramelessWindowHint);
		QWidget::setAttribute(Qt::WA_TranslucentBackground);
		QWidget::show();
		
		SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_MOVE);
		SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_RESIZE);
	}
	~Menu_Main2();
	void onEvent(Event* e)
	{
		EventType type = e->getType();
		switch (type) 
		{
		case EVENT_WINDOW_MOVE:
			event_windowMove((Event_WindowMove*)e);
			break;
		case EVENT_WINDOW_RESIZE:
			event_windowResize((Event_WindowResize*)e);
			break;
		default:
			break;
		}
	}
	void event_windowMove(Event_WindowMove* e)
	{
		move(e->pos.x, e->pos.y);
	}

	void event_windowResize(Event_WindowResize* e)
	{
		resize(e->width, e->height);
		ui.label_background->resize(width(), height());
		ui.label_background->lower();
	}

	void keyPressEvent(QKeyEvent *e)
	{
		QCoreApplication::sendEvent(parent, e);
	}
	void keyReleaseEvent(QKeyEvent *e)
	{
		QCoreApplication::sendEvent(parent, e);
	}
};