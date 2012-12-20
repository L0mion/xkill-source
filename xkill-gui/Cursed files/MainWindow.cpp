#include "MainWindow.h"

MainWindow::MainWindow( QWidget *parent /*= 0*/, Qt::WFlags flags /*= 0*/ ) : QMainWindow(parent, flags)
{
	// Create console
	AllocConsole();
	SetStdHandle(STD_INPUT_HANDLE |STD_OUTPUT_HANDLE, this->winId());

	// create UI generated from XML file
	ui.setupUi(this);
	QApplication::setStyle(new QCleanlooksStyle);
	MainWindow::setWindowTitle("XKILL");
	resize(800, 600);

	// subscribe to events
	SUBSCRIBE_TO_EVENT(this, EVENT_SHOW_MESSAGEBOX);

	// init game
	gameWidget = new GameWidget(this);
	this->setCentralWidget(gameWidget);
	setMouseTracking(true);
	hasMouseLock = false;
	menuManager = new MenuManager(this);

	// setup signals and slots
	connect(ui.actionFullscreen, SIGNAL(triggered()), this, SLOT(slot_toggleFullScreen()));
	connect(ui.actionCap_FPS, SIGNAL(toggled(bool)), gameWidget, SLOT(slot_toggleCapFPS(bool)));
	ui.actionCap_FPS->setChecked(true);
	connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));
	connect(gameWidget, SIGNAL(signal_fpsChanged(QString)), this, SLOT(slot_setTitle(QString)));
}
