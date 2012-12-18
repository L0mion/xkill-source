#include "MenuManager.h"

MenuManager::MenuManager( QWidget* parent )
{
	scoreBoard = new Menu_ScoreBoard(parent);
	main = new Menu_Main(parent);

	SUBSCRIBE_TO_EVENT(this, EVENT_UPDATE);
}

