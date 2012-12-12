#pragma once

#include <QtGui>
#include <QKeyEvent> // needed to grab mouse input
#include <xkill-utilities/IObserver.h>

class Event;

class Menu : public QDialog, public IObserver
{
private:
	bool show;
	QStandardItemModel* model;
	QTableView* view;

public:
	Menu(QWidget* parent = 0, Qt::WFlags flags = 0);

	void onUpdate(float delta);

	void toggleMenu()
	{
		show = !show;
		if(show)
		{
			QWidget::show();
			onUpdate(1.0f);
		}
		else
			QWidget::hide();
	}

	void onEvent(Event* e);

	void parentMoveEvent()
	{
		QPoint pos = parentWidget()->pos();
		int x = pos.x() + 50;
		int y = pos.y() + 60;
		move(x, y);
	}

protected:
	// Behavior on keyboard input
	void keyPressEvent(QKeyEvent* e)
	{
		switch (e->key()) 
		{
		case Qt::Key_Tab:
			// Quit menu
			QWidget::hide();
			show = false;
			break;
		case Qt::Key_Escape:
			// Quit menu
			QWidget::hide();
			show = false;
			break;
		default:
			break;
		}
	}
};
