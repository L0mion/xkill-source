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
	menu = new Menu_Main2(this);
	new Menu_Editor(ui, this);

	// setup signals and slots
	connect(ui.actionFullscreen,			SIGNAL(triggered()),					this,			SLOT(slot_toggleFullScreen()));
	connect(ui.actionCap_FPS,				SIGNAL(toggled(bool)),					gameWidget,		SLOT(slot_toggleCapFPS(bool)));
	ui.actionCap_FPS->setChecked(true);
	connect(ui.actionQuit,					SIGNAL(triggered()),					this,			SLOT(close()));
	connect(gameWidget,						SIGNAL(signal_fpsChanged(QString)),		this,			SLOT(slot_setTitle(QString)));
	connect(gameWidget,						SIGNAL(signal_fpsChanged(QString)),		this,			SLOT(slot_setTitle(QString)));

	// Listen to incomming event
	this->installEventFilter(this);
	
	
	// Start RELEASE in fullscreen, and DEBUG in Windowed, also avoid menu if DEBUG
	slot_toggleFullScreen();
#if defined(DEBUG) || defined(_DEBUG)
	slot_toggleFullScreen();
	//SEND_EVENT(&Event(EVENT_STARTGAME)); //Skips menu in DEBUG
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
	// Toggle fullscreen
	if((e->key()==Qt::Key_Return) && (e->modifiers()==Qt::AltModifier))
		slot_toggleFullScreen();

	// Exit program
	if((e->key()==Qt::Key_F4)  && (e->modifiers()==Qt::AltModifier))
		SEND_EVENT(&Event(EVENT_QUIT_TO_DESKTOP));

	// Toggle editor
	if((e->key()==Qt::Key_F1))
		ui.dockWidget->toggleViewAction()->activate(QAction::Trigger);

	
	//
	// Menu controlls during in-game
	//

	if(GET_STATE() == STATE_DEATHMATCH)
	{
		switch (e->key()) 
		{
			// Return to menu
		case Qt::Key_Escape:
			SEND_EVENT(&Event_EnableMenu(true));
			break;
		default:
			break;
		}
	}
	if(GET_STATE() == STATE_GAMEOVER)
	{
		switch (e->key())
		{
			// Return to menu
		case Qt::Key_Escape:
			GET_STATE() = STATE_MAINMENU;
			SEND_EVENT(&Event_EndDeathmatch());
			SEND_EVENT(&Event_StartDeathmatch(0));	//To get a black background, for now run the game with zero players
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
	gameWidget->sendPositionEvent();
}

void MainWindow::event_showMessageBox( Event_ShowMessageBox* e )
{
	QString message(e->message.c_str());

	QMessageBox::information(0, "Error", message);
}

void MainWindow::closeEvent( QCloseEvent *event )
{
	delete gameWidget;
	delete menu;
}
