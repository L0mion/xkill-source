#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QMouseEvent> // needed to grab mouse input
#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventManager.h>
#include "ui_MainWindow.h"

#include "GameWidget.h"

class MainWindow : public QMainWindow, public IObserver
{
	Q_OBJECT

private:
	Ui::MainWindowClass ui;
	QWidget* gameWidget;
	bool hasMouseLock;

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0) : QMainWindow(parent, flags)
	{
		// enable mouse tracking
		setMouseTracking(true);
		hasMouseLock = false;

		// create UI generated from XML file
		ui.setupUi(this);
		MainWindow::setWindowTitle("XKILL");
		resize(800, 600);

		// init console
		AllocConsole();

		// init gameWidget
		gameWidget = new GameWidget(this);
		this->setCentralWidget(gameWidget);

		SetStdHandle(STD_INPUT_HANDLE |STD_OUTPUT_HANDLE, this->winId());

		// show fps-counter on title bar
		connect(gameWidget, SIGNAL(signal_fpsChanged(QString)), this, SLOT(slot_setTitle(QString)));
		
		// connect functionality with buttons
		connect(ui.actionFullscreen, SIGNAL(toggled(bool)), this, SLOT(toggleFullScreen(bool)));
		connect(ui.actionCap_FPS, SIGNAL(toggled(bool)), gameWidget, SLOT(slot_toggleCapFPS(bool)));
		ui.actionCap_FPS->setChecked(true);
		connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));
	}
	~MainWindow()
	{
		delete gameWidget;
	}

	void onUpdate(float delta)
	{
	}

	void onEvent(Event* e)
	{
	}

protected:
	// Sends mouse movement as an event.
	void mouseMoveEvent(QMouseEvent* e)
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
	};

	// Behavior on mouse press
	void mousePressEvent(QMouseEvent *e)
	{
		// lock / release mouse
		if(e->button() == Qt::LeftButton)
			toggleMouseLock();
	};

	// Behavior on keyboard input
	void keyPressEvent(QKeyEvent* e)
	{
		if(e->key() == Qt::Key_Escape)
		{
			ui.actionFullscreen->setChecked(false);
		}
		if(e->key() == Qt::Key_W)
		{
		}
		if(e->key() == Qt::Key_A)
		{
		}
		if(e->key() == Qt::Key_S)
		{
		}
		if(e->key() == Qt::Key_D)
		{
		}
	};

	public slots:
		void slot_setTitle(QString title)
		{
			MainWindow::setWindowTitle("XKILL  [" + title + "]");
		};

	void toggleFullScreen(bool isChecked)
	{
		if(isChecked)
		{
			ui.mainToolBar->hide();
			this->showFullScreen();

		}
		else
		{
			ui.mainToolBar->show();
			this->showNormal();
		}
	};

private:
	void toggleMouseLock()
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
		}
		else
		{
			// show cursor again and release mouse cursor
			QWidget::setCursor(Qt::ArrowCursor);	
			QWidget::releaseMouse();
		}
	};
};

#endif // MAINWINDOW_H
