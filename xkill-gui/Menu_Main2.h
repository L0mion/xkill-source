#pragma once

#include <QtGui/QMainWindow>
#include <QtGui/QKeyEvent>
#include <QtGui/QMovie>
#include <xkill-utilities/Util.h>
#include "ui_MainMenu2.h"
#include <vector>

class Menu_Main2 : public QMainWindow, IObserver
{
	Q_OBJECT

private:
	Ui::MainWindow ui;
	QWidget* parent;
	std::vector<QFrame*> menuStack; // Used as state machine to return to the previous menu
	QMovie* openingAnimation;

public:
	Menu_Main2(QWidget* parent);
	~Menu_Main2()
	{
		UNSUBSCRIBE_TO_EVENTS(this);
	}
	void onEvent(Event* e)
	{
		EventType type = e->getType();
		switch (type) 
		{
		case EVENT_ENABLE_MENU:
			if(((Event_EnableMenu*)e)->enableMenu)
				this->show();
			else
				this->hide();
			break;
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
	void push_menu(QFrame* menu)
	{
		// Hide previous menu, if any
		if(menuStack.size() > 0)
		{
			QFrame* topMenu = menuStack.back();
			topMenu->hide();
		}

		// Show new menu
		menu->move(0,0);
		menu->resize(width(), height());
		menu->show();
		menuStack.push_back(menu);

		SEND_EVENT(&Event_PlaySound(Event_PlaySound::SOUND_BUTTON_CLICK));
	}
	void pop_menu()
	{
		// Make sure rot-menu is not poped
		if(menuStack.size() > 2)
		{
			// Pop current menu
			QFrame* topMenu = menuStack.back();
			topMenu->hide();
			menuStack.pop_back();

			// Show previous menu
			QFrame* menu = menuStack.back();
			menu->move(0,0);
			menu->resize(width(), height());
			menu->show();
		}
	}
	void menuResize()
	{
		QFrame* topMenu = menuStack.back();

		// Resize background
		ui.label_openingAnimation->resize(width(), height());
		ui.label_background->resize(width(), height());
		ui.label_openingAnimation->lower();
		ui.label_background->lower();
		
		// Resize current menu
		topMenu->resize(width(), height());
	}


	void event_windowMove(Event_WindowMove* e)
	{
		move(e->pos.x, e->pos.y);
	}

	void alwaysOnTop(bool on)
	{
		if(on)
		{
			// Enable Window Stay on Top flag
			this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
		}
		else
		{
			// Disable Window Stay on Top flag
			this->setWindowFlags(this->windowFlags() & ~Qt::WindowStaysOnTopHint);
		}
	}

	void event_windowResize(Event_WindowResize* e)
	{
		// Resize window
		resize(e->width, e->height);

		// Make sure menu is resized as well
		menuResize();
	}

	void keyPressEvent(QKeyEvent *e)
	{
		QCoreApplication::sendEvent(parent, e);
	}
	void keyReleaseEvent(QKeyEvent *e)
	{
		QCoreApplication::sendEvent(parent, e);
	}

	void closeEvent(QCloseEvent* event)
	{
		SEND_EVENT(&Event(EVENT_QUIT_TO_DESKTOP));
	}

protected:
	void mousePressEvent(QMouseEvent *e);

private slots:
	void openingAnimation_frameChanged(int frameNumber )
	{
		int lastFrame = openingAnimation->frameCount()-1;

		// Stop animaiton when last frame is reached
		if(frameNumber >= lastFrame)
		{
			endOpening();
		}
	}
	void endOpening()
	{
		int lastFrame = openingAnimation->frameCount()-1;
		openingAnimation->jumpToFrame(lastFrame);
		openingAnimation->stop();
		ui.label_openingAnimation->hide();

		slot_menu_main();
	}
	void setNumPlayers(int numPlayers)
	{
		SETTINGS->numPlayers = numPlayers;
	}
	void slot_menu_main()
	{
		push_menu(ui.frame_main);
	}
	void slot_menu_start()
	{
		push_menu(ui.frame_start);
	}
	void slot_menu_settings()
	{
		push_menu(ui.frame_settings);
	}
	void slot_menu_video()
	{
		push_menu(ui.frame_video);
	}
	void slot_menu_audio()
	{
		push_menu(ui.frame_audio);
	}
	void slot_menu_startgame()
	{
		SEND_EVENT(&Event(EVENT_STARTGAME));
	}
	void slot_menu_customize()
	{
		push_menu(ui.frame_customize);
	}
	void slot_menu_input()
	{
		push_menu(ui.frame_input);
	}
	void slot_menu_credits()
	{
		push_menu(ui.frame_credits);
	}
	void slot_menu_quit()
	{
		SEND_EVENT(&Event(EVENT_QUIT_TO_DESKTOP));
	}
};