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
		int x = pos.x() + parentWidget()->width()/2 - this->width()/2;
		int y = pos.y() + parentWidget()->height()/2 - this->height()/2;
		move(x, y);
	}

protected:
	// Behavior on keyboard input
	void keyPressEvent(QKeyEvent* e);
};
