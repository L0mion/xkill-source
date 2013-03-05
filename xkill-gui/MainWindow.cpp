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

	// Init iterators
	ATTRIBUTES_INIT_ALL;

	// subscribe to events
	SUBSCRIBE_TO_EVENT(this, EVENT_SHOW_FULLSCREEN);
	SUBSCRIBE_TO_EVENT(this, EVENT_SHOW_MESSAGEBOX);
	SUBSCRIBE_TO_EVENT(this, EVENT_QUIT_TO_DESKTOP);

	// create UI generated from XML file
	ui.setupUi(this);
	ui.mainToolBar->hide();
	MainWindow::setWindowTitle("XKILL");
	resize(800, 600);
	QWidget::setAttribute(Qt::WA_PaintOnScreen);

	// init game
	gameWidget = new GameWidget(this);
	this->setCentralWidget(gameWidget);

	// init tools
	new Menu_Editor(ui, this);
	menu = new Menu_Main2(this);
	
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
	default:
		break;
	}
}

void MainWindow::keyPressEvent( QKeyEvent* e )
{
	// Toggle full screen
	if((e->key()==Qt::Key_Return) && (e->modifiers()==Qt::AltModifier))
		slot_toggleFullScreen();

	// Exit program
	if((e->key()==Qt::Key_F4)  && (e->modifiers()==Qt::AltModifier))
		SEND_EVENT(&Event(EVENT_QUIT_TO_DESKTOP));

	// Toggle editor
	if((e->key()==Qt::Key_F1))
		ui.dockWidget->toggleViewAction()->activate(QAction::Trigger);
	
	// Skip menu
	if((e->key()==Qt::Key_F2))
		SEND_EVENT(&Event(EVENT_STARTGAME));

	if((e->key()==Qt::Key_1))
	{
		{Event_PostHudMessage e("Punish them all"); e.receiver = Event_PostHudMessage::RECEIVER_ALL; e.setStyle(Event_PostHudMessage::STYLE_WARNING); SEND_EVENT(&e);}
		{Event_PostHudMessage e("NullProcess is executing"); e.receiver = Event_PostHudMessage::RECEIVER_ALL; e.setStyle(Event_PostHudMessage::STYLE_SUBTILE); SEND_EVENT(&e);}
	}
	if((e->key()==Qt::Key_2))
	{
		// Post hud messages
		{Event_PostHudMessage e("");  e.receiver = Event_PostHudMessage::RECEIVER_ALL;e.setHtmlMessage("Now running in", "Kernel Mode"); SEND_EVENT(&e);}
		{Event_PostHudMessage e(""); e.receiver = Event_PostHudMessage::RECEIVER_ALL; e.setHtmlMessage("Chosen by Scheduler"); SEND_EVENT(&e);}
		{Event_PostHudMessage e("");  e.receiver = Event_PostHudMessage::RECEIVER_ALL;e.setHtmlMessage("Blarrghh", "is executing"); e.receiver = Event_PostHudMessage::RECEIVER_ALL_BUT_SUBJECT; SEND_EVENT(&e);}
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
		e.setHtmlMessage("You terminated", "Blarrhgh", "+2 priority");
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
		case Qt::Key_F11:
			{
				static bool hideHud = false;
				hideHud = !hideHud;
				SEND_EVENT(&Event_EnableHud(!hideHud));
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
	SEND_EVENT(&Event_KeyPress(e->key(), true));
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
	SEND_EVENT(&Event_KeyPress(e->key(), false));
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

	return false;
}
