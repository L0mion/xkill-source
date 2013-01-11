#include "MenuManager.h"

#include <xkill-utilities/EventManager.h>

ATTRIBUTES_DECLARE_ALL;

HUDWindow::HUDWindow(QWidget* parent, int id) : QMainWindow(parent)
{
	this->id = id;

	//QWidget::setAttribute(Qt::WA_TranslucentBackground, true);
	QWidget::setWindowFlags(Qt::SplashScreen);
	setWindowFlags(Qt::WindowStaysOnBottomHint);
	QWidget::setStyleSheet("QWidget{color: #fff; background-color: #000;}");
	show();

	//
	// build menu
	//

	horizontalLayout = new QHBoxLayout(this);

	// health
	label_health = new QLabel(this);
	label_health->setText("<html><head/><body><p><span style=\" font-size:6pt; font-weight:600;\">Health 50</span></p></body></html>");
	horizontalLayout->addWidget(label_health);

	// ammo
	label_ammo = new QLabel(this);
	label_ammo->setText("<html><head/><body><p><span style=\" font-size:6pt; font-weight:600;\">Ammo 50</span></p></body></html>");
	horizontalLayout->addWidget(label_ammo);

	// ammo type
	label_ammoType = new QLabel(this);
	label_ammoType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/a_explosive2.png")));
	label_ammoType->setMaximumSize(QSize(15, 15));
	label_ammoType->setScaledContents(true);
	horizontalLayout->addWidget(label_ammoType);

	// weapon type
	label_weaponType = new QLabel(this);
	label_weaponType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/w_auto.png")));
	label_weaponType->setMaximumSize(QSize(15, 15));
	label_weaponType->setScaledContents(true);
	horizontalLayout->addWidget(label_weaponType);

	QWidget* mainWidget = new QWidget(this);
	mainWidget->setLayout(horizontalLayout);
	setCentralWidget(mainWidget);

	resize(horizontalLayout->minimumSize());
}

void HUDWindow::parentMoveEvent()
{
	QWidget* parent = this->parentWidget();
	int x = 20 + id*(width()+10);
	int y = parent->height() - this->height() - 20;
	move(x, y);
}

void HUDWindow::update(Attribute_Player* player)
{
	Attribute_Health*		health		=	itrHealth		.at(player->ptr_health);
	Attribute_WeaponStats*	weaponStats	=	itrWeaponStats	.at(player->ptr_weaponStats);

	// health & ammo bars
	QString str_health = QString::number(health->health);
	label_health->setText("<html><head/><body><p><span style=\" font-size:6pt; font-weight:600;\">Health "+str_health+"</span></p></body></html>");
	QString str_ammo = QString::number(weaponStats->nrOfShotsLeftInClip);
	label_ammo->setText("<html><head/><body><p><span style=\" font-size:6pt; font-weight:600;\">Ammo "+str_ammo+"</span></p></body></html>");

	// ammo icon
	if(ammo != weaponStats->ammunitionType)
	{
		ammo = weaponStats->ammunitionType;

		if(ammo == Attribute_WeaponStats::BULLET)
		{
			label_ammoType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/a_bullet.png")));
		}
		if(ammo == Attribute_WeaponStats::SCATTER)
		{
			label_ammoType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/a_scatter.png")));
		}
		if(ammo == Attribute_WeaponStats::EXPLOSIVE)
		{
			label_ammoType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/a_explosive.png")));
		}
	}

	// weapon icon
	if(weapon != weaponStats->firingMode)
	{
		weapon = weaponStats->firingMode;

		if(weapon == Attribute_WeaponStats::SINGLE)
		{
			label_weaponType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/w_single.png")));
		}
		if(weapon == Attribute_WeaponStats::SEMI)
		{
			label_weaponType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/w_semi.png")));
		}
		if(weapon == Attribute_WeaponStats::AUTO)
		{
			label_weaponType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/w_auto.png")));
		}
	}
}

void HUDManager::update()
{
	// Balance attributes / vs huds
	int numPlayers = itrPlayer.size();
	while(numPlayers>huds.size())
	{
		huds.push_back(new HUDWindow(parent, huds.size()));
	}
	while(numPlayers<huds.size())
	{
	
		delete huds.back();
		huds.pop_back();
	}
	parentMoveEvent();

	// Update huds
	int index = 0;
	while(itrPlayer.hasNext())
	{
		Attribute_Player* player = itrPlayer.getNext();
		huds[index]->update(player);
		index++;
	}
}

HUDManager::HUDManager(QWidget* parent)
{
	ATTRIBUTES_INIT_ALL;

	this->parent = parent;
	createHUD();
}


MenuManager::MenuManager( QWidget* parent )
{
	scoreBoard = new Menu_ScoreBoard(parent);
	inGameMenu = new Menu_InGame(parent);
	mainMenu = new Menu_Main(parent);
	hudManager = HUDManager(parent);
	
	mainMenu->toggleMenu(true);

	SUBSCRIBE_TO_EVENT(this, EVENT_UPDATE);
	SUBSCRIBE_TO_EVENT(this, EVENT_END_DEATHMATCH);
}

void MenuManager::keyPressEvent( QKeyEvent* e )
{
	if(GET_STATE() == STATE_DEATHMATCH)
	{
		switch (e->key()) 
		{
		case Qt::Key_Escape:
			inGameMenu->toggleMenu();
			break;
		case Qt::Key_Tab:
			scoreBoard->toggleMenu(true);
			break;
		default:
			break;
		}
	}
}

void MenuManager::keyReleaseEvent( QKeyEvent* e )
{
	switch (e->key()) 
	{

	case Qt::Key_Tab:
		scoreBoard->toggleMenu(false);
		break;
	default:
		break;
	}
}

