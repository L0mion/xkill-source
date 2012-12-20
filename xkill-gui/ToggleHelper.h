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
	ToggleHelper(QWidget* owner);

	void toggleMenu();
	void toggleMenu(bool show);

	void virtual parentMoveEvent() = 0;
};