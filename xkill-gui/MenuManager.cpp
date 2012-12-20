#include "MenuManager.h"

#include <xkill-utilities/EventManager.h>

MenuManager::MenuManager( QWidget* parent )
{
	scoreBoard = new Menu_ScoreBoard(parent);
	inGameMenu = new Menu_InGame(parent);
	mainMenu = new Menu_Main(parent);
	mainMenu->toggleMenu(true);

	SUBSCRIBE_TO_EVENT(this, EVENT_UPDATE);
	SUBSCRIBE_TO_EVENT(this, EVENT_END_DEATHMATCH);
}

void MenuManager::keyPressEvent( QKeyEvent* e )
{
	if(GET_STATE() == STATE_DEATHMATCH)
	{
		switch (e->key()) 
		{
		case Qt::Key_Escape:
			inGameMenu->toggleMenu();
			break;
		case Qt::Key_Tab:
			scoreBoard->toggleMenu(true);
			break;
		default:
			break;
		}
	}
}

void MenuManager::keyReleaseEvent( QKeyEvent* e )
{
	switch (e->key()) 
	{

	case Qt::Key_Tab:
		scoreBoard->toggleMenu(false);
		break;
	default:
		break;
	}
}

