#include "MainWindow.h"
#include <Windows.h>

#include <QtGui/QLayout> 
#include <QtGui/QPushButton>
#include <QtGui/qplastiquestyle.h> 

#include <xkill-utilities/AttributeManager.h>

// Stuff used to allocate console
// no idea what most of it does
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>

#include "ui_MainWindow.h"

ATTRIBUTES_DECLARE_ALL

struct Test
{
	//const static int attributeType = ATTRIBUTE_POSITION;


	Float3 up;			//!< Always aims up from the camera, perpendicular to look.
	Float3 right;		//!< Always aims to the right of the camera, perpendicular to look.
	Float3 look;		//!< The direction in which the camera is aimed. 
};

MainWindow::MainWindow()
{
	ATTRIBUTES_INIT_ALL

	// subscribe to events
	SUBSCRIBE_TO_EVENT(this, EVENT_SHOW_MESSAGEBOX);
	SUBSCRIBE_TO_EVENT(this, EVENT_QUIT_TO_DESKTOP);

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

	// create UI generated from XML file
	ui.setupUi(this);
	QApplication::setStyle(new QPlastiqueStyle);
	MainWindow::setWindowTitle("XKILL");
	resize(800, 600);
	QWidget::setAttribute(Qt::WA_PaintOnScreen);

	// init game
	gameWidget = new GameWidget(this);
	this->setCentralWidget(gameWidget);
	
	menuManager = new MenuManager(gameWidget);

	// setup signals and slots
	connect(ui.actionFullscreen,			SIGNAL(triggered()),					this,			SLOT(slot_toggleFullScreen()));
	connect(ui.actionCap_FPS,				SIGNAL(toggled(bool)),					gameWidget,		SLOT(slot_toggleCapFPS(bool)));
	ui.actionCap_FPS->setChecked(true);
	connect(ui.actionQuit,					SIGNAL(triggered()),					this,			SLOT(close()));
	connect(gameWidget,						SIGNAL(signal_fpsChanged(QString)),		this,			SLOT(slot_setTitle(QString)));

	
	new Menu_Editor(ui, this);
	
}

MainWindow::~MainWindow()
{
	delete gameWidget;
	delete menuManager;
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
	if((e->key()==Qt::Key_Return) && (e->modifiers()==Qt::AltModifier))
		slot_toggleFullScreen();

	if((e->key()==Qt::Key_F4) && (e->modifiers()==Qt::AltModifier))
		MainWindow::close();

	if((e->key()==Qt::Key_F1))
		ui.dockWidget->toggleViewAction()->activate(QAction::Trigger);


	//switch (e->key()) 
	//{
	//case Qt::Key_Escape:
	//	// TRUE: Quit program
	//	if(ui.actionFullscreen->isChecked())
	//		ui.actionFullscreen->setChecked(false);
	//	// ELSE: Quit fullscreen
	//	else
	//		MainWindow::close();
	//	break;
	//case Qt::Key_Tab:
	//	menu->toggleMenu();
	//	break;
	//default:
	//	break;
	//}

	// Detect keypress in menu
	menuManager->keyPressEvent(e);

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
		ui.mainToolBar->show();
		this->showNormal();
	}
	else
	{
		ui.mainToolBar->hide();
		this->showFullScreen();
	}
}

void MainWindow::keyReleaseEvent( QKeyEvent* e )
{
	// Detect keypress in menu
	menuManager->keyReleaseEvent(e);

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

	// Reposition menu
	menuManager->moveEvent();

	QSize size = e->size();


	while(itrSplitScreen.hasNext())
	{
		Attribute_SplitScreen* splitScreen = itrSplitScreen.getNext();

		//splitScreen->
	}

	SEND_EVENT(&Event(EVENT_SPLITSCREEN_CHANGED));
}

void MainWindow::moveEvent( QMoveEvent *e )
{
	menuManager->moveEvent();
}

void MainWindow::event_showMessageBox( Event_ShowMessageBox* e )
{
	QString message(e->message.c_str());

	QMessageBox::information(0, "Error", message);
}
