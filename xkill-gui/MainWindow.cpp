#include "MainWindow.h"
#include <Windows.h> //check. Separates this file into .cpp and .h file to remove windows.h inclusion from this file.

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
	case EVENT_SHOW_MESSAGEBOX:
		event_showMessageBox((Event_ShowMessageBox*)e);
		break;
	default:
		break;
	}
}

void MainWindow::mouseMoveEvent( QMouseEvent* e )
{
	if(hasMouseLock)
	{
		// calculate change (delta) in mouse position
		QPoint mouseAnchor = QWidget::mapToGlobal(QPoint(this->width()*0.5f,this->height()*0.5f));
		QCursor::setPos(mouseAnchor.x(), mouseAnchor.y()); // anchor mouse again
		int dx = e->globalX() - mouseAnchor.x();
		int dy = e->globalY() - mouseAnchor.y();

		// send mouse move event to relevant observers
		Event_MouseMove e(dx, dy);
		EventManager::getInstance()->sendEvent(&e);
	}
	else 
	{
		// TODO: Handle menu and other stuff
	}
}

void MainWindow::keyPressEvent( QKeyEvent* e )
{
	if((e->key()==Qt::Key_Return) && (e->modifiers()==Qt::AltModifier))
		slot_toggleFullScreen();

	if((e->key()==Qt::Key_F4) && (e->modifiers()==Qt::AltModifier))
		MainWindow::close();


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
	SEND_EVENT(&Event_KeyPress(e->key()));
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

void MainWindow::toggleMouseLock()
{
	// locking / releasing mouse cursor to widget
	hasMouseLock = !hasMouseLock;
	if(hasMouseLock)
	{
		// hide cursor and set new anchor point
		QWidget::setCursor(Qt::BlankCursor);
		QWidget::grabMouse();

		// move mouse to middle
		QPoint mouseAnchor = QWidget::mapToGlobal(QPoint(this->width()*0.5f,this->height()*0.5f));
		QCursor::setPos(mouseAnchor.x(), mouseAnchor.y()); // anchor mouse again

		// set focus to this widget
		QWidget::setFocus(Qt::MouseFocusReason);
	}
	else
	{
		// show cursor again and release mouse cursor
		QWidget::setCursor(Qt::ArrowCursor);	
		QWidget::releaseMouse();
	}
}

void MainWindow::keyReleaseEvent( QKeyEvent* e )
{
	// Detect keypress in menu
	menuManager->keyReleaseEvent(e);

	// Inform about key release
	SEND_EVENT(&Event_KeyRelease(e->key()));
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
}

void MainWindow::moveEvent( QMoveEvent *e )
{
	menuManager->moveEvent();
}

void MainWindow::mousePressEvent( QMouseEvent *e )
{
	// lock / release mouse
	if(e->button() == Qt::LeftButton)
		toggleMouseLock();
}

void MainWindow::event_showMessageBox( Event_ShowMessageBox* e )
{
	QString message(e->message.c_str());

	QMessageBox::information(0, "Error", message);
}
