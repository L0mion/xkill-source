#include "MenuManager.h"

#include <xkill-utilities/Util.h>


ATTRIBUTES_DECLARE_ALL;

HUDWindow2::HUDWindow2(QWidget* parent, int id) : QMainWindow(parent)
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

	horizontalLayout = new QHBoxLayout();
	horizontalLayout->setSpacing(0);
	horizontalLayout->setMargin(0);
	verticalLayout	= new QVBoxLayout();
	verticalLayout->setSpacing(0);
	horizontalLayout->setMargin(0);
	horizontalLayout->addLayout(verticalLayout);

	// health
	progressBar_health = new QProgressBar();
	verticalLayout->addWidget(progressBar_health);
	progressBar_health->setValue(50);
	progressBar_health->setMaximumSize(QSize(50, 10));
	progressBar_health->setTextVisible(false);
	progressBar_health->setStyleSheet(QString::fromUtf8("QProgressBar::chunk \n"
"{\n"
"	background-color: rgb(0, 170, 0);\n"
"	border: 1px solid black;\n"
"}\n"
"\n"
"QProgressBar\n"
"{\n"
"	border: 0px solid black;\n"
"	background: rgb(0, 0, 0);\n"
"}"));
	

	// ammo
	progressBar_ammo = new QProgressBar();
	verticalLayout->addWidget(progressBar_ammo);
	progressBar_ammo->setValue(50);
    progressBar_ammo->setMaximumSize(QSize(50, 10));
	progressBar_ammo->setTextVisible(false);
    progressBar_ammo->setStyleSheet(QString::fromUtf8("QProgressBar::chunk \n"
"{\n"
"	background-color: rgb(200, 170, 0);\n"
"	border: 1px solid black;\n"
"}\n"
"\n"
"QProgressBar\n"
"{\n"
"	border: 0px solid black;\n"
"	background: rgb(0, 0, 0);\n"
"}"));
	

	// ammo type
	label_ammoType = new QLabel();
	label_ammoType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/a_explosive2.png")));
	
	label_ammoType->setScaledContents(true);
	horizontalLayout->addWidget(label_ammoType);
	
	// weapon type
	label_weaponType = new QLabel();
	label_weaponType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/w_auto.png")));
	
	label_weaponType->setScaledContents(true);
	horizontalLayout->addWidget(label_weaponType);


	// total ammo
	label_totalAmmoLeft = new QLabel("Test");
	horizontalLayout->addWidget(label_totalAmmoLeft);



	QWidget* mainWidget = new QWidget();
	mainWidget->setLayout(horizontalLayout);
	setCentralWidget(mainWidget);

	resize(horizontalLayout->minimumSize());
}

void HUDWindow2::parentMoveEvent(AttributePtr<Attribute_SplitScreen> splitScreen)
{
	float sizeScale = (float) splitScreen->ssHeight / 1000;
	sizeScale = 0.75f*sizeScale + 0.25f;

	int x = 20*sizeScale + splitScreen->ssTopLeftX;
	int y = splitScreen->ssTopLeftY + splitScreen->ssHeight - this->height() - 20*sizeScale;
	move(x, y);
}

void HUDWindow2::update(AttributePtr<Attribute_SplitScreen> splitScreen)
{
	AttributePtr<Attribute_Player>		player		=	splitScreen->ptr_player;
	AttributePtr<Attribute_Health>		health		=	player->ptr_health;
	AttributePtr<Attribute_WeaponStats>	weaponStats	=	player->ptr_weaponStats;

	Ammunition* ammunition = &weaponStats->ammunition[weaponStats->currentAmmunitionType];
	FiringMode* firingMode = &weaponStats->firingMode[weaponStats->currentFiringModeType];

	float sizeScale = (float) splitScreen->ssHeight / 1000;
	sizeScale = 0.50f*sizeScale + 0.50f;
	QSize iconSize((int)32*sizeScale, (int)32*sizeScale);
	label_weaponType->setMaximumSize(iconSize);
	label_ammoType->setMaximumSize(iconSize);
	progressBar_health->setMaximumSize(QSize(200*sizeScale, 16*sizeScale));
	progressBar_ammo->setMaximumSize(QSize(200*sizeScale, 16*sizeScale));
	progressBar_health->setMinimumSize(QSize(200*sizeScale, 16*sizeScale));
	progressBar_ammo->setMinimumSize(QSize(200*sizeScale, 16*sizeScale));
	
	// health & ammo bars
	int healthRatio = (int)((health->health / health->maxHealth) * 100);
	int ammoIndex = ammunition->type;
	int ammoRatio = (int)(((float)firingMode->nrOfShotsLeftInClip[ammoIndex] / firingMode->clipSize) * 100);
	progressBar_health->setValue(healthRatio);
	progressBar_ammo->setValue(ammoRatio);
	progressBar_health->update();
	progressBar_ammo->update();

	// ammo icon
	if(ammo != weaponStats->currentAmmunitionType)
	{
		ammo = weaponStats->currentAmmunitionType;

		if(ammo == XKILL_Enums::AmmunitionType::BULLET)
		{
			label_ammoType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/a_bullet.png")));
		}
		if(ammo == XKILL_Enums::AmmunitionType::SCATTER)
		{
			label_ammoType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/a_scatter.png")));
		}
		if(ammo == XKILL_Enums::AmmunitionType::EXPLOSIVE)
		{
			label_ammoType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/a_explosive.png")));
		}
	}

	// weapon icon
	if(weapon != weaponStats->currentFiringModeType)
	{
		weapon = weaponStats->currentFiringModeType;

		if(weapon == XKILL_Enums::FiringModeType::SINGLE)
		{
			label_weaponType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/w_single.png")));
		}
		if(weapon == XKILL_Enums::FiringModeType::SEMI)
		{
			label_weaponType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/w_semi.png")));
		}
		if(weapon == XKILL_Enums::FiringModeType::AUTO)
		{
			label_weaponType->setPixmap(QPixmap(QString::fromUtf8(":/xkill/images/w_auto.png")));
		}
	}

	// total ammo left	
	QString num_totalShots = QString::number(weaponStats->ammunition[ammoIndex].currentTotalNrOfShots);
	label_totalAmmoLeft->setText(num_totalShots);

	resize(horizontalLayout->minimumSize());
}

HUDManager::HUDManager()
{
	//Intentionally left blank
}

HUDManager::HUDManager(QWidget* parent)
{
	ATTRIBUTES_INIT_ALL;

	this->parent = parent;
}

void HUDManager::update()
{
	// Balance attributes / vs huds
	int num_splitScreen = itrSplitScreen.count();
	while(num_splitScreen>huds.size())
	{
		huds.push_back(new HUDWindow2(parent, huds.size()));
	}
	while(num_splitScreen<huds.size())
	{
		delete huds.back();
		huds.pop_back();
	}

	// Update huds
	int index = 0;
	while(itrSplitScreen.hasNext())
	{
		AttributePtr<Attribute_SplitScreen> ptr_splitScreen = itrSplitScreen.getNext();
		huds[index]->update(ptr_splitScreen);
		huds[index]->parentMoveEvent(ptr_splitScreen);
		index++;
	}
}

void HUDManager::createHUD()
{
	for(int i=0; i<5; i++)
	{
		huds.push_back(new HUDWindow2(parent, i));
	}
}

void HUDManager::parentMoveEvent()
{
	update();
}


MenuManager::MenuManager( QWidget* parent )
{
	scoreBoard = new Menu_ScoreBoard(parent);
	inGameMenu = new Menu_InGame(parent);
	mainMenu = new Menu_Main(parent);
	hudManager = HUDManager(parent);
	
	mainMenu->toggleMenu(true);


}

void MenuManager::keyPressEvent( QKeyEvent* e )
{
}

void MenuManager::keyReleaseEvent( QKeyEvent* e )
{

}

void MenuManager::onEvent( Event* e )
{
}

void MenuManager::onUpdate( float delta )
{
	//Intentionally left blank
}

void MenuManager::moveEvent()
{
	mainMenu->parentMoveEvent();
	scoreBoard->parentMoveEvent();
	inGameMenu->parentMoveEvent();
	hudManager.parentMoveEvent();
}

