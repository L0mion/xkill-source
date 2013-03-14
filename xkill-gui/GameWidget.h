#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QtGui/QWidget.h>
#include <QtGui/QResizeEvent>
#include <QtGui/QMouseEvent>
#include <QtCore/QTime>
#include <QtCore/QTimer> // needed to implement frame rate
#include <xkill-architecture/GameManager.h>
#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/Util.h>
#include <xkill-utilities/StopWatch.h>

#include "ui_MainWindow.h"
#include "GameTimer.h"

class GameWidget : public QWidget, public IObserver
{
	Q_OBJECT

private:
	Ui::MainWindowClass ui;
	GameManager gameManager;
	GameTimer gameTimer;
	QTimer* updateTimer;
	bool hasMouseLock;

public:
	GameWidget(QWidget* parent) : QWidget(parent)
	{
		setMouseTracking(true);
		hasMouseLock = false;

		// make widget non-transparent & draw directly onto screen
		QWidget::setAttribute(Qt::WA_OpaquePaintEvent);
		QWidget::setAttribute(Qt::WA_PaintOnScreen);
		resize(800, 800);
		
		// subscribe to events
		SUBSCRIBE_TO_EVENT(this, EVENT_GET_WINDOW_RESOLUTION);
		SUBSCRIBE_TO_EVENT(this, EVENT_SET_MOUSELOCK);
		SUBSCRIBE_TO_EVENT(this, EVENT_FOCUS_MAINWINDOW);

		// init game
		HWND parentWindowHandle = this->winId();
		if(parent != 0)
			parentWindowHandle = this->parentWidget()->winId();

		gameManager.init(this->winId(), parentWindowHandle);
		gameTimer.reset();
		updateTimer = new QTimer(this);
		updateTimer->setInterval(0);
		connect(updateTimer, SIGNAL(timeout()), this, SLOT(slot_onUpdate()));
		updateTimer->start();
	};
	~GameWidget()
	{
	};
	
	QPaintEngine* paintEngine() const {return 0;}; // overrides Qt:s own paint engine; prevents flicker

	void onEvent(Event* e)
	{
		EventType type = e->getType();
		switch (type) 
		{
		case EVENT_GET_WINDOW_RESOLUTION:
			event_getWindowResolution((Event_GetWindowResolution*)e);
			break;
		case EVENT_SET_MOUSELOCK:
			event_setMouseLock(((Event_SetMouseLock*)e)->isLock);
			break;
		case EVENT_FOCUS_MAINWINDOW:
			this->activateWindow();
			break;
		default:
			break;
		}
	}

	Int2 getGlobalPosition()
	{
		QPoint qPos = mapToGlobal(QPoint(pos().x(),pos().y()));

		return Int2(qPos.x(), qPos.y());
	}

	void sendPositionEvent()
	{
		SEND_EVENT(&Event_WindowMove(getGlobalPosition(), Int2()));
	}

public slots:
	// Runs every time gameTimer times out
	void slot_onUpdate()
	{
		gameTimer.tick();
		float delta = gameTimer.getDeltaTime();
		
		// Keep the game stable by preventing 
		// delta time from getting to big, which
		// usually happens during loading times.
		float deltaLimit = 1.0f / 10; // always simulate at least 1.0 / x fps
		if(delta > deltaLimit)
			delta = deltaLimit;

		// add time manipulation
		SETTINGS->trueDeltaTime = delta;
		delta *= SETTINGS->timeScale();

		// Average fps through lerp of previous times
		static float averageDelta = 0.0f;
		const float factor = 0.05f;
		averageDelta = (1.0f - factor) * averageDelta + delta * factor;
		SETTINGS->averageDeltaTime = averageDelta;

		computeFPS();
		gameManager.update(delta);
	};
	void slot_toggleCapFPS(bool isChecked)
	{
		if(isChecked)
		{
			updateTimer->setInterval(10);
		}
		else
		{
			updateTimer->setInterval(0);
		}
	};

protected:
	void paintEvent(QPaintEvent* e)
	{
		raise();

	}; // should not be implemented
	void resizeEvent(QResizeEvent* e)
	{
		QWidget::resizeEvent(e);

		// Inform about resize
		int width = size().width();
		int height = size().height();
		SEND_EVENT(&Event_WindowResize(width, height));
		sendPositionEvent();
	}

	void moveEvent(QMoveEvent* e)
	{
		QWidget::moveEvent(e);

		sendPositionEvent();
	}

	void keyPressEvent(QKeyEvent *e)
	{
		QCoreApplication::sendEvent(parentWidget(), e);
	}
	void keyReleaseEventEvent(QKeyEvent *e)
	{
		QCoreApplication::sendEvent(parentWidget(), e);
	}
	void wheelEvent(QWheelEvent *e)
	{
		QCoreApplication::sendEvent(parentWidget(), e);
	}
	// Behavior on mouse press
	void mousePressEvent(QMouseEvent *e)
	{
		//// lock / release mouse5
		//if(hasMouseLock && e->button() == Qt::RightButton)
		//	event_setMouseLock(false);

		if(!hasMouseLock && e->button() == Qt::LeftButton)
		{
			event_setMouseLock(true);
		}
		else
		{
			// Inform about key press
			int keyEnum = e->button();
			SEND_EVENT(&Event_MousePress(keyEnum, true));
		}
	}
	void mouseReleaseEvent(QMouseEvent *e)
	{
		// Inform about key release
		int keyEnum = e->button();
		SEND_EVENT(&Event_MousePress(keyEnum, false));
	}
	void event_setMouseLock(bool mouseLook)
	{
		// locking / releasing mouse cursor to widget
		this->hasMouseLock = mouseLook;
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
	// Sends mouse movement as an event.
	void mouseMoveEvent( QMouseEvent* e )
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

private:
	void computeFPS()
	{
		static int num_frames = 0;
		static float timeElapsed = 0.0f;
		num_frames++;

		// Compute averages FPS and MS over one second period.
		if((gameTimer.getTotalTime()-timeElapsed) >= 1.0f)
		{
			// calculate statistics
			float fps = (float)num_frames; // fps = frameCnt / 1
			float msPerFrame = 1000.0f/fps;

			// convert statistics into QString
			QString stats;
			stats = "FPS:  %1  Frame Time:  %2 (ms)";
			stats = stats.arg(fps).arg((int)msPerFrame);

			// send signal
			emit signal_fpsChanged(stats);
			static int debugId = StopWatch::getUniqueId();
			SEND_EVENT(&Event_PostDebugMessage(debugId, "Total: " + Converter::IntToStr(fps) + " (fps)"));

		

			// reset stats for next average.
			num_frames = 0;
			timeElapsed += 1.0f;
		}
	}

	void event_getWindowResolution(Event_GetWindowResolution* e)
	{
		e->width = size().width();
		e->height = size().height();
	}

signals:
	void signal_fpsChanged(QString value);
};

#endif