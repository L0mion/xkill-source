#pragma once

#include <QtGui/QMainWindow>
#include <QtGui/QKeyEvent>
#include <QtGui/QMovie>
#include <QtGui/QFontDatabase>
#include <xkill-utilities/Util.h>
#include "ui_MainMenu.h"
#include <vector>

#include "Menu_Input.h"
#include "Menu_Ammo.h"
#include "Menu_FiringMode.h"
#include "Menu_Sound.h"
#include "Menu_HUD.h"


class Menu_Main : public QMainWindow, IObserver
{
	Q_OBJECT

private:
	Ui::MainWindow ui;
	QWidget* parent;
	std::vector<QFrame*> menuStack; // Used as state machine to return to the previous menu
	QMovie* openingAnimation;

	// Level menu
	std::vector<std::string> levelNames;
	int levelCurrent;
	QStandardItemModel* levelListModel;
	QString filePath;

	Menu_Input*			input_Menu;
	Menu_Ammo*			ammo_Menu;
	Menu_FiringMode*	firingMode_Menu;
	Menu_Sound*			sound_Menu;
	Menu_HUDManager*	hud;

	void loadCustomFonts();
	void loadOpeningGif();
	void push_menu(QFrame* menu);
	void pop_menu();
	void menuResize(); 
	void hideMenu()
	{
		endOpening();

		if(menuStack.size() > 0)
		{
			QFrame* topMenu = menuStack.back();
			topMenu->hide();
			ui.label_background->hide();
		}
		raise();
	}
	void showMenu()
	{
		if(menuStack.size() > 0)
		{
			QFrame* topMenu = menuStack.back();
			topMenu->show();
			this->activateWindow();
		}

		ui.label_background->show();
		raise();
	}
	void updateLevelSelectionInterface();

public:
	Menu_Main(QWidget* parent);
	~Menu_Main();

	void onEvent(Event* e);
	void event_windowMove(Event_WindowMove* e);

	/**
	Causes the overlay to always be shown on top
	even on top of other applications.
	DEPRICATED: Shouldn't be needed anymore. Overlay
	is instead updated every time MainWindow gains focus.
	*/
	void setAlwaysOnTop(bool on);

	void closeEvent(QCloseEvent* event);
	void event_windowResize(Event_WindowResize* e);

protected:
	void mousePressEvent(QMouseEvent* e);
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);

private slots:
	void slot_menu_startgame()
	{
		// Apply menu settings
		SETTINGS->cycleLimit = ui.horizontalSlider_cycleLimit->value();
		SETTINGS->timeLimit = ui.horizontalSlider_timeLimit->value() * 60.0f;
		SETTINGS->numPlayers = ui.horizontalSlider_numPlayers->value();
		SETTINGS->schedulerTime = ui.horizontalSlider_executionFrequency->value();
		SETTINGS->cycleTime = ui.horizontalSlider_priorityToCycleRate->value();
		SETTINGS->respawnTime = ui.horizontalSlider_respawnTime->value();
		SETTINGS->nullprocessExecutionTime = ui.horizontalSlider_nullProcessDuration->value();

		AttributeIterator<Attribute_Player> itrPlayer = ATTRIBUTE_MANAGER->player.getIterator();
		while(itrPlayer.hasNext())
		{
			itrPlayer.getNext()->respawnTimer.setStartTime(static_cast<float>(SETTINGS->respawnTime));
		}

		// Load selected level
		std::string levelName = levelNames[levelCurrent];
		SEND_EVENT(&Event_LoadLevel(levelName));

		// Start game
		SEND_EVENT(&Event(EVENT_STARTGAME));
	}
	void openingAnimation_frameChanged(int frameNumber )
	{
		int lastFrame = openingAnimation->frameCount()-1;

		// Stop animation when last frame is reached
		if(frameNumber >= lastFrame)
		{
			endOpening();
		}
	}
	void endOpening()
	{
		// Skip opening, if at opening (index 0)
		if(menuStack.size()==1)
		{
			int lastFrame = openingAnimation->frameCount()-1;
			openingAnimation->jumpToFrame(lastFrame);
			openingAnimation->stop();
			ui.label_openingAnimation->hide();

			slot_menu_main();
		}
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
	void slot_menu_next_level();
	void slot_menu_previous_level();
};