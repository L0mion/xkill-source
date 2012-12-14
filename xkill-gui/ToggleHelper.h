#pragma once

#include <QWidget>
#include <xkill-utilities/IObserver.h>

class Event;

class ToggleHelper
{
protected:
	QWidget* owner;
	bool showMenu;

public:
	ToggleHelper(QWidget* owner)
	{
		this->owner = owner;

		// Hide menu by default
		// show is set to false on Toggle
		showMenu = true;
		toggleMenu();
	}

	void toggleMenu()
	{
		showMenu = !showMenu;
		if(showMenu)
		{
			owner->show();
			parentMoveEvent();
		}
		else
			owner->hide();
	}

	void virtual parentMoveEvent() = 0;
};