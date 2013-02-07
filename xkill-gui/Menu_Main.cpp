#include "Menu_Main.h"
#include <QtGui/QKeyEvent>
#include <xkill-utilities/Util.h>
#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/AttributeManager.h>

#include <xkill-input/InputDevice.h>
#include <xkill-input/InputObjectArray.h>
#include <xkill-input/InputObject.h>

#include <QtXml/QtXml>
#include <QtGui/QStandardItemModel>
#include <QtGui/QMessageBox>

#include "Menu_Input.h"
#include "Menu_Ammo.h"
#include "Menu_FiringMode.h"
#include "Menu_Sound.h"

#include "Menu_Editor.h"

ATTRIBUTES_DECLARE_ALL

Menu_Main::Menu_Main( QWidget* parent ) : QMainWindow(parent), ToggleHelper(this)
{
	ATTRIBUTES_INIT_ALL

	ui.setupUi(this);
	QWidget::setAttribute(Qt::WA_TranslucentBackground, true);
	QWidget::setWindowFlags(Qt::SplashScreen);
	//setAttribute(Qt::WA_TransparentForMouseEvents);
	//setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::WindowStaysOnTopHint);

	ui.verticalLayout_2->setSpacing(0);
	ui.verticalLayout_2->setMargin(0);
	ui.verticalLayout->setSpacing(0);
	ui.verticalLayout->setMargin(0);

	connect(ui.pushButton_exit,			SIGNAL(clicked()),					this,	SLOT(slot_quitToDesktop()));
	connect(ui.pushButton_exit_2,		SIGNAL(clicked()),					this,	SLOT(slot_quitToDesktop()));
	connect(ui.comboBox_LevelSelect,	SIGNAL(currentIndexChanged(int)),	this,	SLOT(slot_selectLevel(int)));
	connect(ui.pushButton_AddLevel,		SIGNAL(clicked()),					this,	SLOT(slot_addLevel()));
	connect(ui.pushButton_SaveLevel,	SIGNAL(clicked()),					this,	SLOT(slot_saveLevel()));
	connect(ui.pushButton_RemoveLevel,	SIGNAL(clicked()),					this,	SLOT(slot_removeLevel()));
	connect(ui.pushButton_startGame,	SIGNAL(clicked()),					this,	SLOT(slot_startGame()));

	/*
	
	connect(ui.radioButton_Ammo_Bullet,							SIGNAL(clicked()),					this,	SLOT(slot_updateAmmoMenu()));
	connect(ui.radioButton_Ammo_Scatter,						SIGNAL(clicked()),					this,	SLOT(slot_updateAmmoMenu()));
	connect(ui.radioButton_Ammo_Explosive,						SIGNAL(clicked()),					this,	SLOT(slot_updateAmmoMenu()));

	connect(ui.horizontalSlider_Ammo_Damage,					SIGNAL(sliderMoved(int)),			this,	SLOT(slot_ammoMenuUpdated()));
	connect(ui.horizontalSlider_Ammo_ExplosionInitialRadius,			SIGNAL(sliderMoved(int)),			this,	SLOT(slot_ammoMenuUpdated()));
	connect(ui.horizontalSlider_Ammo_NrOfProjectiles,			SIGNAL(sliderMoved(int)),			this,	SLOT(slot_ammoMenuUpdated()));
	connect(ui.horizontalSlider_Ammo_Speed,						SIGNAL(sliderMoved(int)),			this,	SLOT(slot_ammoMenuUpdated()));
	connect(ui.horizontalSlider_Ammo_Spread,					SIGNAL(sliderMoved(int)),			this,	SLOT(slot_ammoMenuUpdated()));
	connect(ui.horizontalSlider_Ammo_VelocitVariation,			SIGNAL(sliderMoved(int)),			this,	SLOT(slot_ammoMenuUpdated()));
	connect(ui.groupBox_Ammo_Explosive,							SIGNAL(clicked()),					this,	SLOT(slot_ammoMenuUpdated()));

	connect(ui.radioButton_Weapon_Single,						SIGNAL(clicked()),					this,	SLOT(slot_updateFiringModeMenu()));
	connect(ui.radioButton_Weapon_Semi,							SIGNAL(clicked()),					this,	SLOT(slot_updateFiringModeMenu()));
	connect(ui.radioButton_Weapon_Auto,							SIGNAL(clicked()),					this,	SLOT(slot_updateFiringModeMenu()));

	connect(ui.checkBox_Weapon_Bullet,							SIGNAL(clicked()),					this,	SLOT(slot_firingModeUpdated()));
	connect(ui.checkBox_Weapon_Scatter,							SIGNAL(clicked()),					this,	SLOT(slot_firingModeUpdated()));
	connect(ui.checkBox_Weapon_Explosive,						SIGNAL(clicked()),					this,	SLOT(slot_firingModeUpdated()));
	connect(ui.horizontalSlider_Weapon_ClipSize,				SIGNAL(sliderMoved(int)),			this,	SLOT(slot_firingModeUpdated()));
	connect(ui.horizontalSlider_Weapon_DamageModifier,			SIGNAL(sliderMoved(int)),			this,	SLOT(slot_firingModeUpdated()));
	connect(ui.horizontalSlider_Weapon_ExplosionSphereModifier,	SIGNAL(sliderMoved(int)),			this,	SLOT(slot_firingModeUpdated()));
	connect(ui.horizontalSlider_Weapon_RateOfFire,				SIGNAL(sliderMoved(int)),			this,	SLOT(slot_firingModeUpdated()));
	connect(ui.horizontalSlider_Weapon_ReloadTime,				SIGNAL(sliderMoved(int)),			this,	SLOT(slot_firingModeUpdated()));

	connect(ui.tabWidget_2,										SIGNAL(currentChanged(int)),		this,	SLOT(slot_updateAmmoMenu()));
	connect(ui.tabWidget_2,										SIGNAL(currentChanged(int)),		this,	SLOT(slot_updateFiringModeMenu()));
	
	
	*/
	
	// Set num players to 2
	ui.horizontalSlider_numPlayers->setValue(2);

	filePath = QString("../../xkill-resources/xkill-scripts/levels.xml");
	levelListModel = new QStandardItemModel(0, 1, this);

	//editorModel->setHorizontalHeaderItem(1, new QStandardItem("ID"));

	QStringList columnNames;

	loadXML();

	input_Menu = new Menu_Input(&ui, this);
	input_Menu->Init(new QStandardItemModel(0, 2, this), new QStandardItemModel(0, 1, this));
	ammo_Menu = new Menu_Ammo(&ui, this);
	firingMode_Menu = new Menu_FiringMode(&ui, this);
	sound_Menu = new Menu_Sound(&ui, this);
}

Menu_Main::~Menu_Main()
{
	delete input_Menu;
	delete ammo_Menu;
	delete firingMode_Menu;
	delete sound_Menu;
}

void Menu_Main::parentMoveEvent()
{
	QPoint pos = parentWidget()->pos();
	pos = QPoint();
	int x = pos.x() + parentWidget()->width()/2 - this->width()/2;
	int y = pos.y() + parentWidget()->height()/2 - this->height()/2;
	move(x, y);
}

void Menu_Main::loadXML()
{
	levelListModel->clear();

	// load XML file
	QDomDocument document;
	QFile* file = new QFile(filePath);
	if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(0, "Error", "Failed to find XML file"); return;
	}
	if(!document.setContent(file))
	{
		QMessageBox::information(0, "Error", "Failed to parse XML file to open XML file");  return;
	}
	file->close();
	delete file;

	// read all levels to ItemModel
	QStandardItem* root = new QStandardItem("Levels");
	levelListModel->appendRow(root);
	levelListModel->item(0);
	QDomElement xmlRoot = document.firstChildElement();
	QDomNodeList allLevel = xmlRoot.elementsByTagName("Level");
	for(int i=0; i<allLevel.count(); i++)
	{
		// parse level
		QDomElement level = allLevel.at(i).toElement();
		QStandardItem* name = new QStandardItem(level.attribute("Name"));
		QStandardItem* description = new QStandardItem(level.attribute("Description"));
		root->appendRow(name);
		name->appendRow(description);
	}

	// parse model and build stuff from it
	ui.comboBox_LevelSelect->clear();
	QStandardItem* allLevelItm = levelListModel->item(0,0);
	for(int i=0; i<allLevelItm->rowCount(); i++)
	{
		QStandardItem* child = allLevelItm->child(i,0);
		ui.comboBox_LevelSelect->addItem(child->text());
	}
	ui.treeView->setModel(levelListModel);

	ui.treeView->setExpanded(allLevelItm->index(), true);
	
}

void Menu_Main::slot_selectLevel( int levelId )
{
	QStandardItem* levels = levelListModel->item(0,0);
	if(levels->rowCount()>levelId && levelId>=0)
	{
		QStandardItem* name = levels->child(levelId,0);
		QStandardItem* desc = name->child(0,0);
		ui.textBrowser_LevelInfo->setText(desc->text());
	}
}

void Menu_Main::slot_addLevel()
{
	QStandardItem* desc = new QStandardItem("Description");
	QStandardItem* name = new QStandardItem("Name");
	name->appendRow(desc);
	QStandardItem* levels = levelListModel->item(0,0);
	levels->appendRow(name);
}

void Menu_Main::slot_removeLevel()
{
	QStandardItem* levels = levelListModel->item(0,0);
	QModelIndex index = ui.treeView->currentIndex();
	levels->removeRow(index.row());
}

void Menu_Main::slot_saveLevel()
{
	// make root node
	QDomDocument document;
	QDomElement xmlroot = document.createElement("xkill");
	document.appendChild(xmlroot);

	// parse model into level xml
	QStandardItem* levels = levelListModel->item(0,0);
	for(int i=0; i<levels->rowCount(); i++)
	{
		QDomElement xmlbook = document.createElement("Level");
		QStandardItem* name = levels->child(i,0);
		QStandardItem* desc = name->child(0,0);
		xmlbook.setAttribute("Name",name->text());
		xmlbook.setAttribute("Description", desc->text());
		xmlroot.appendChild(xmlbook);
	}

	// save to disk
	QFile file(filePath);
	if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::information(0, "Error", "Failed to write file");  return;
	}
	QTextStream stream(&file);
	stream << document.toString();
	file.close();

	loadXML();
}

void Menu_Main::keyPressEvent( QKeyEvent* e )
{
	QCoreApplication::sendEvent(parentWidget(), e);
}

void Menu_Main::mousePressEvent( QMouseEvent* e )
{
	e->accept(); // avoids propagating event to parent
}

void Menu_Main::slot_startGame()
{
	// Hide mouse
	SEND_EVENT(&Event_SetMouseLock(true));

	SEND_EVENT(&Event_EndDeathmatch());	//Temporary to remove the "empty" game that is used to paint the menu background black.
	int num_players = ui.horizontalSlider_numPlayers->value();
	SEND_EVENT(&Event_StartDeathmatch(num_players));

	ToggleHelper::toggleMenu(false);
}

void Menu_Main::slot_loadInputList(int deviceId)
{
	input_Menu->loadInputList(deviceId);
}

void Menu_Main::slot_loadInputSettings(QModelIndex index)
{
	input_Menu->loadInputSettings(index.row());
}

void Menu_Main::slot_inputSettingsChanged()
{
	input_Menu->updateMenu();
}

void Menu_Main::slot_setInputObject(QModelIndex index)
{
	input_Menu->setInputObject(index);
}

void Menu_Main::slot_updateAmmoMenu()
{
	ammo_Menu->setSettingsMenu();
}

void Menu_Main::slot_ammoMenuUpdated()
{
	ammo_Menu->settingsMenuUpdated();
}

void Menu_Main::slot_updateFiringModeMenu()
{
	firingMode_Menu->setSettingsMenu();
}

void Menu_Main::slot_firingModeUpdated()
{
	firingMode_Menu->settingsMenuUpdated();
}

void Menu_Main::slot_soundMenuUpdated()
{
	sound_Menu->updateMenu();
}

void Menu_Main::slot_quitToDesktop()
{
	SEND_EVENT(&Event(EVENT_QUIT_TO_DESKTOP));
}
