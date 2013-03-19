#include "MainWindow.h"

#include <QtGui/QLayout> 
#include <QtGui/QPushButton>
#include <QtGui/qplastiquestyle.h> 

#include <xkill-utilities/AttributeManager.h>

// Stuff used to allocate console
// no idea what most of it does
#include <io.h>
#include <fcntl.h>
#include <Windows.h>

#include "ui_MainWindow.h"

ATTRIBUTES_DECLARE_ALL

MainWindow::MainWindow()
{
	QWidget::installEventFilter(this);
	menu = NULL;
	gameWidget = NULL;

	// Create console
#ifdef XKILL_DEBUG
	AllocConsole();
	SetConsoleTitle(L"Debug console");
	int hConHandle;
	long lStdHandle;
	FILE *fp;   // redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );
#endif

	// Init iterators
	ATTRIBUTES_INIT_ALL;

	// subscribe to events
	SUBSCRIBE_TO_EVENT(this, EVENT_SHOW_FULLSCREEN);
	SUBSCRIBE_TO_EVENT(this, EVENT_SHOW_MESSAGEBOX);
	SUBSCRIBE_TO_EVENT(this, EVENT_QUIT_TO_DESKTOP);
	SUBSCRIBE_TO_EVENT(this, EVENT_TOGGLE_EDITOR);

	// create UI generated from XML file
	ui.setupUi(this);
	ui.mainToolBar->hide();
	MainWindow::setWindowTitle("XKILL");
	resize(1000, 600);
	QWidget::setAttribute(Qt::WA_PaintOnScreen);

	// init game
	gameWidget = new GameWidget(this);
	this->setCentralWidget(gameWidget);

	// init tools
	new Menu_Editor(ui, this);
	menu = new Menu_Main(this);
	
	// setup signals and slots
	connect(ui.actionFullscreen,			SIGNAL(triggered()),					this,			SLOT(slot_toggleFullScreen()));
	connect(ui.actionCap_FPS,				SIGNAL(toggled(bool)),					gameWidget,		SLOT(slot_toggleCapFPS(bool)));
	ui.actionCap_FPS->setChecked(true);
	connect(ui.actionQuit,					SIGNAL(triggered()),					this,			SLOT(close()));
	connect(gameWidget,						SIGNAL(signal_fpsChanged(QString)),		this,			SLOT(slot_setTitle(QString)));
	connect(gameWidget,						SIGNAL(signal_fpsChanged(QString)),		this,			SLOT(slot_setTitle(QString)));

	// Listen to incomming event
	this->installEventFilter(this);

	slot_toggleFullScreen();			//Fullscreen
	// DEBUG build specific settings (setAlwaysOnTopAndShow(false) is set in Menu_Main2::Menu_Main2() if DEBUG)
#if defined(DEBUG) || defined(_DEBUG)
	slot_toggleFullScreen();			//Windowed
	SEND_EVENT(&Event(EVENT_STARTGAME));//Skips menu in DEBUG
#endif

	{
		int width = gameWidget->width();
		int height = gameWidget->height();
		
		SETTINGS->render_width = width;
		SETTINGS->render_height = height;
	}
	
}

MainWindow::~MainWindow()
{
	UNSUBSCRIBE_TO_EVENTS(this);
}

void MainWindow::onUpdate( float delta )
{
}

void MainWindow::onEvent( Event* e )
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_SHOW_FULLSCREEN:
		setFullScreen(((Event_SetFullscreen*)e)->on);
		break;
	case EVENT_QUIT_TO_DESKTOP:
		QWidget::close();
		break;
	case EVENT_SHOW_MESSAGEBOX:
		event_showMessageBox((Event_ShowMessageBox*)e);
		break;
	case EVENT_TOGGLE_EDITOR:
		{
			ui.dockWidget->toggleViewAction()->activate(QAction::Trigger);
			if(!ui.dockWidget->isHidden())
				SEND_EVENT(&Event_SetMouseLock(false));

		}
		break;
	default:
		break;
	}
}

void MainWindow::keyPressEvent( QKeyEvent* e )
{

	// Toggle mouse lock
	if(e->key() == Qt::Key_Alt)
		SEND_EVENT(&Event_SetMouseLock(false));

	
	// Toggle full screen
	if((e->key()==Qt::Key_Return) && (e->modifiers()==Qt::AltModifier))
		slot_toggleFullScreen();

	// Exit program
	if((e->key()==Qt::Key_F4)  && (e->modifiers()==Qt::AltModifier))
		SEND_EVENT(&Event(EVENT_QUIT_TO_DESKTOP));

	// Toggle editor
	if((e->key()==Qt::Key_F1))
		SEND_EVENT(&Event(EVENT_TOGGLE_EDITOR));
	
	// Skip menu
	if((e->key()==Qt::Key_F2))
		SEND_EVENT(&Event(EVENT_STARTGAME));

	// Toggle cap fps
	if((e->key()==Qt::Key_F3))
	{
		if(!ui.actionCap_FPS->isChecked())
			ui.actionCap_FPS->setChecked(true);
		else
			ui.actionCap_FPS->setChecked(false);
	}

	// Show debug out print
	if((e->key()==Qt::Key_F5))
	{
		SEND_EVENT(&Event(EVENT_TOGGLE_DEBUG_MESSAGES));
	}
	if((e->key()==Qt::Key_F6))
	{
		SETTINGS->showDebugPhysics = !SETTINGS->showDebugPhysics;
	}

	if((e->key()==Qt::Key_1))
	{
		{Event_PostHudMessage e("Punish them all"); e.receiver = Event_PostHudMessage::RECEIVER_ALL; e.setStyle(Event_PostHudMessage::STYLE_WARNING); SEND_EVENT(&e);}
		{Event_PostHudMessage e("");  e.receiver = Event_PostHudMessage::RECEIVER_ALL; e.setHtmlMessage("","NullProcess", "is executing"); SEND_EVENT(&e);}
	}
	if((e->key()==Qt::Key_2))
	{
		// Post hud messages
		{Event_PostHudMessage e("");  e.receiver = Event_PostHudMessage::RECEIVER_ALL;e.setHtmlMessage("Now running in", "Kernel Mode"); SEND_EVENT(&e);}
		{Event_PostHudMessage e(""); e.receiver = Event_PostHudMessage::RECEIVER_ALL; e.setHtmlMessage("Chosen by Scheduler"); SEND_EVENT(&e);}
		{Event_PostHudMessage e("");  e.receiver = Event_PostHudMessage::RECEIVER_ALL;e.setHtmlMessage("","Blarrghh", "is executing"); e.receiver = Event_PostHudMessage::RECEIVER_ALL_BUT_SUBJECT; SEND_EVENT(&e);}
	}
	if((e->key()==Qt::Key_3))
	{
		Event_PostHudMessage e("");
		e.receiver = Event_PostHudMessage::RECEIVER_ALL;
		e.setStyle(Event_PostHudMessage::STYLE_WARNING);
		e.message = "Punish them all";
		SEND_EVENT(&e);
	}
	if((e->key()==Qt::Key_4))
	{
		Event_PostHudMessage e("");
		e.setHtmlMessage("You terminated", "Blarrhgh", "", "+2 priority");
		e.receiver = Event_PostHudMessage::RECEIVER_ALL;
		SEND_EVENT(&e);
	}
	

	//
	// Menu controls during in-game
	//

	if(GET_STATE() == STATE_DEATHMATCH)
	{
		switch (e->key()) 
		{
		case Qt::Key_Escape:
			SEND_EVENT(&Event(EVENT_ENDGAME));
			break;
		case Qt::Key_F7:
			{
				// Toggle enable free look
				SETTINGS->freeLookMode = !SETTINGS->freeLookMode;
			}
			break;
		case Qt::Key_F8:
			{
				static bool enableMenu = true;
				enableMenu = !enableMenu;

				// Toggle hide menu and enable free look
				SEND_EVENT(&Event_EnableHud(enableMenu));

				// Disattach camera position from player to get 
				// ridd of weapon view
				AttributeIterator<Attribute_Player> itr_player = ATTRIBUTE_MANAGER->player.getIterator();
				if(itr_player.hasNext())
				{
					AttributePtr<Attribute_Player> ptr_player = itr_player.getNext();
					bool disableCamera = !enableMenu;
					ptr_player->ptr_camera->ptr_offset->isDisabled = disableCamera;
				}
			}
			break;
		default:
			break;
		}
	}
	
	if(GET_STATE() == STATE_MAINMENU)
	{
		//switch(e->key())
		//{
		//case Qt::Key_Escape:
		//	menu->setAlwaysOnTopAndShow(false);	//check
		//	SEND_EVENT(&Event_EnableMenu(false)); //check
		//	break;
		//default:
		//	break;
		//}
	}
	if(GET_STATE() == STATE_GAMEOVER)
	{
		switch (e->key())
		{
		case Qt::Key_Escape:
			SEND_EVENT(&Event(EVENT_ENDGAME));
			break;
		default:
			break;
		}
	}

	// Inform about key press
	SEND_EVENT(&Event_KeyPress(e->key(), true, (e->key() == Qt::Key::Key_Shift), (e->key() == Qt::Key::Key_Tab)));
}

void MainWindow::showMenu()
{
	static bool first = true;
	static QWidget* menu;

	if(first)
	{
		first = false;

		menu = new QDialog(this); 
		menu->setAttribute(Qt::WA_ShowWithoutActivating);
		menu->setWindowFlags(Qt::ToolTip);
		QVBoxLayout* layout = new QVBoxLayout(this);
		for (int i=0; i<10; i++)
		{
			QPushButton* buttons = new QPushButton(tr("Button %1").arg(i + 1));
			layout->addWidget(buttons);
		}
		menu->setLayout(layout);
	}

	static bool show = true;
	if(show)
		menu->show();
	else
		menu->hide();
	show = !show;
}

void MainWindow::slot_toggleFullScreen()
{
	if(this->isFullScreen())
	{
		this->showNormal();
	}
	else
	{
		this->showFullScreen();
	}
}

void MainWindow::keyReleaseEvent( QKeyEvent* e )
{
	// Inform about key release
	SEND_EVENT(&Event_KeyPress(e->key(), false, (e->key() == Qt::Key::Key_Shift), (e->key() == Qt::Key::Key_Tab)));
}

void MainWindow::slot_setTitle( QString title )
{
	MainWindow::setWindowTitle("XKILL  [" + title + "]");
}

void MainWindow::resizeEvent( QResizeEvent* e )
{
	QWidget::resizeEvent(e);
}

void MainWindow::moveEvent( QMoveEvent *e )
{
	if(gameWidget != NULL)
	{
		gameWidget->sendPositionEvent();
	}
}

void MainWindow::event_showMessageBox( Event_ShowMessageBox* e )
{
	// Count errors
	SETTINGS->numErrors++;

	// Turn off fullscreen to prevent freezeup
	SEND_EVENT(&Event_SetFullscreen(false));

	// Show message
	QString message(e->message.c_str());
	QMessageBox::information(0, "Error", message);
}

void MainWindow::closeEvent( QCloseEvent *event )
{
	delete gameWidget;
	delete menu;
}

void MainWindow::setFullScreen( bool on )
{
	if(on)
		this->showFullScreen();
	else
		this->showNormal();
}

bool MainWindow::eventFilter( QObject* object, QEvent* event )
{
	QEvent::Type type = event->type();

	/*if(type == QEvent::NonClientAreaMouseMove)
	return false;
	if(type == QEvent::WindowTitleChange)
	return false;
	DEBUGPRINT("Event: " << type);*/

	if(type == QEvent::NonClientAreaMouseButtonPress)
	{
		POST_DELAYED_EVENT(new Event(EVENT_WINDOW_FOCUS_CHANGED), 0.0f);
		DEBUGPRINT("Event: NonClientAreaMouseButtonPress"); 
	}

	if(type == QEvent::WindowActivate)
	{
		SEND_EVENT(&Event(EVENT_WINDOW_FOCUS_CHANGED));
		DEBUGPRINT("Event: WindowActivate"); 
	}

	if(type == QEvent::WindowDeactivate)
	{
		SEND_EVENT(&Event_SetMouseLock(false));
	}

	return false;
}

void MainWindow::wheelEvent( QWheelEvent* e )
{
	int value = 0;
	if(e->delta() > 0)
		value = 1;
	if(e->delta() < 0)
		value = -1;

	SEND_EVENT(&Event_MouseWheel(value));
}
