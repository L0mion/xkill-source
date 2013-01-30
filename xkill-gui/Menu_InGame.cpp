#include "Menu_InGame.h"
#include <QtGui/QKeyEvent>

#include <xkill-utilities/Util.h>

Menu_InGame::Menu_InGame( QWidget* parent ) : QMainWindow(parent), ToggleHelper(this)
{
	ui.setupUi(this);
	QWidget::setAttribute(Qt::WA_TranslucentBackground, true);
	QWidget::setWindowFlags(Qt::SplashScreen);
	//setAttribute(Qt::WA_TransparentForMouseEvents);
	//setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::WindowStaysOnBottomHint);

	connect(ui.pushButton_return, SIGNAL(clicked()), this, SLOT(slot_return()));
	connect(ui.pushButton_quitToMenu, SIGNAL(clicked()), this, SLOT(slot_quitToMenu()));
	connect(ui.pushButton_quitToDesktop, SIGNAL(clicked()), this, SLOT(slot_quitToDesktop()));

	ui.verticalLayout_2->setSpacing(0);
	ui.verticalLayout_2->setMargin(0);
}

void Menu_InGame::parentMoveEvent()
{
	QPoint pos = parentWidget()->pos();
	pos = QPoint();
	int x = pos.x() + parentWidget()->width()/2 - this->width()/2;
	int y = pos.y() + parentWidget()->height()/2 - this->height()/2;
	move(x, y);
}

void Menu_InGame::slot_return()
{
	toggleMenu(false);
}

void Menu_InGame::slot_quitToMenu()
{
	GET_STATE() = STATE_MAINMENU;
	SEND_EVENT(&Event_EndDeathmatch());
	//SEND_EVENT(&Event_StartDeathmatch(0));
}

void Menu_InGame::slot_quitToDesktop()
{
	SEND_EVENT(&Event(EVENT_QUIT_TO_DESKTOP));
}
