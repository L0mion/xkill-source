#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMouseEvent> // needed to grab mouse input
#include <QMessageBox> // used to display info dialogs
#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventManager.h>
#include "ui_MainWindow.h"

#include "GameWidget.h"
#include "MenuManager.h"

class MainWindow : public QMainWindow, public IObserver
{
	Q_OBJECT

private:
	Ui::MainWindowClass ui;
	GameWidget* gameWidget;
	MenuManager* menuManager;
	bool hasMouseLock;

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0) : QMainWindow(parent, flags)
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
		connect(ui.actionFullscreen, SIGNAL(toggled(bool)), this, SLOT(toggleFullScreen(bool)));
		connect(ui.actionCap_FPS, SIGNAL(toggled(bool)), gameWidget, SLOT(slot_toggleCapFPS(bool)));
		ui.actionCap_FPS->setChecked(true);
		connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));
		connect(gameWidget, SIGNAL(signal_fpsChanged(QString)), this, SLOT(slot_setTitle(QString)));
	}

	~MainWindow()
	{
		delete gameWidget;
		delete menuManager;
	}

	void onUpdate(float delta)
	{
	}

	void onEvent(Event* e)
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

	void event_showMessageBox(Event_ShowMessageBox* e)
	{
		QString message(e->message.c_str());
		
		QMessageBox::information(0, "Error", message);
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
	};

	void moveEvent(QMoveEvent *e)
	{
		menuManager->moveEvent();
	}

	void resizeEvent(QResizeEvent* e)
	{
		QWidget::resizeEvent(e);

		// Reposition menu
		menuManager->moveEvent();
	};

	void showMenu()
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
	};

	void keyReleaseEvent(QKeyEvent* e)
	{
		// Inform about key release
		SEND_EVENT(&Event_KeyRelease(e->key()));
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
