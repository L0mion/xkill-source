#include "ToggleHelper.h"

ToggleHelper::ToggleHelper( QWidget* owner )
{
	this->owner = owner;

	// Hide menu by default
	// show is set to false on Toggle
	showMenu = true;
	toggleMenu();
}

void ToggleHelper::toggleMenu()
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

void ToggleHelper::toggleMenu( bool show )
{
	if(show != showMenu)
	{
		toggleMenu();
	}
}
