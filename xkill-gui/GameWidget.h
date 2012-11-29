#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget.h>
#include <QResizeEvent>
#include <QMouseEvent>

#include <QTime>
#include <QTimer> // needed to implement framerate

#include "GameTimer.h"


class GameWidget : public QWidget
{
	Q_OBJECT

private:
	GameTimer gameTimer;
	QTimer* updateTimer;
	bool hasMouseLock;

public:
	GameWidget(QWidget* parent = 0, Qt::WFlags flags = 0) : QWidget(parent, flags)
	{
		// Make widget non-transparent & draw directly onto screen
		setAttribute(Qt::WA_OpaquePaintEvent);
		setAttribute(Qt::WA_PaintOnScreen);

		// init updateTimer
		updateTimer = new QTimer(this);
		connect(updateTimer, SIGNAL(timeout()), this, SLOT(onUpdate()));

		// init gameTimer
		gameTimer.reset();
	};
	~GameWidget()
	{
	};

	// Runs every time gameTimer times out
	void onUpdate()
	{
		gameTimer.tick();
		float delta = gameTimer.getDeltaTime();
		calcFPS();
	};
	
	QPaintEngine* paintEngine() const {return 0;}; // overrides Qt:s own paint engine; prevents flicker

protected:
	void paintEvent(QPaintEvent* e){}; // should not be implemented
	void resizeEvent(QResizeEvent* e)
	{
		QWidget::resizeEvent(e);
		int width = size().width();
		int height = size().height();
		//renderer->onResize(width, height);
	};

private:
	void calcFPS()
	{
		//stringstream ss()
	}
};

#endif