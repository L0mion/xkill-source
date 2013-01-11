#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QtGui/QWidget.h>
#include <QtGui/QResizeEvent>
#include <QtGui/QMouseEvent>
#include <QtCore/QTime>
#include <QtCore/QTimer> // needed to implement frame rate
#include <xkill-architecture/GameManager.h>
#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventManager.h>

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

public:
	GameWidget(QWidget* parent) : QWidget(parent)
	{
		// make widget non-transparent & draw directly onto screen
		QWidget::setAttribute(Qt::WA_OpaquePaintEvent);
		QWidget::setAttribute(Qt::WA_PaintOnScreen);
		resize(800, 800);
		
		// subscribe to events
		SUBSCRIBE_TO_EVENT(this, EVENT_GET_WINDOW_RESOLUTION);

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
		default:
			break;
		}
	}

public slots:
	// Runs every time gameTimer times out
	void slot_onUpdate()
	{
		gameTimer.tick();
		float delta = gameTimer.getDeltaTime();
		// add time manipultion
		delta *= ATTRIBUTE_MANAGER->settings->timeScale;
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
	void paintEvent(QPaintEvent* e){}; // should not be implemented
	void resizeEvent(QResizeEvent* e)
	{
		QWidget::resizeEvent(e);

		// Inform about resize
		int width = size().width();
		int height = size().height();
		Event_WindowResize event_windowResize(width, height);
		SEND_EVENT(&event_windowResize);
	};

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