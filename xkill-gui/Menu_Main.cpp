#include "Menu_Main.h"
#include <QtGui/QKeyEvent>
#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/AttributeManager.h>

#include <xkill-input/InputDevice.h>
#include <xkill-input/InputObjectArray.h>
#include <xkill-input/InputObject.h>

#include <QtXml/QtXml>
#include <QtGui/QStandardItemModel>
#include <QtGui/QMessageBox>

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
	setWindowFlags(Qt::WindowStaysOnBottomHint);

	connect(ui.pushButton_exit, SIGNAL(clicked()), parentWidget(), SLOT(close()));
	connect(ui.pushButton_exit_2, SIGNAL(clicked()), parentWidget(), SLOT(close()));
	connect(ui.comboBox_LevelSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_selectLevel(int)));
	connect(ui.pushButton_AddLevel, SIGNAL(clicked()), this, SLOT(slot_addLevel()));
	connect(ui.pushButton_SaveLevel, SIGNAL(clicked()), this, SLOT(slot_saveLevel()));
	connect(ui.pushButton_RemoveLevel, SIGNAL(clicked()), this, SLOT(slot_removeLevel()));
	connect(ui.pushButton_startGame, SIGNAL(clicked()), this, SLOT(slot_startGame()));

	connect(ui.comboBox_Input, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_loadInputList(int)));
	connect(ui.tableView_Input, SIGNAL(clicked(QModelIndex)), this, SLOT(slot_loadInputSettings(QModelIndex)));
	connect(ui.tableView_Input, SIGNAL(clicked(QModelIndex)), this, SLOT(slot_setInputObject(QModelIndex)));

	connect(ui.horizontalSlider_Input, SIGNAL(sliderMoved(int)), this, SLOT(slot_inputSettingsChanged()));
	connect(ui.checkBox_Input, SIGNAL(clicked()), this, SLOT(slot_inputSettingsChanged()));
	connect(ui.pushButton_Input, SIGNAL(clicked()), this, SLOT(slot_inputSettingsChanged()));


	filePath = QString("../../xkill-resources/xkill-scripts/levels.xml");
	levelListModel = new QStandardItemModel(0, 1, this);
	inputListModel = new QStandardItemModel(0, 2, this);
	deviceListModel = new QStandardItemModel(0, 1, this);

	currentObject = nullptr;

	loadDeviceList();
	loadInputList(0);

	//editorModel->setHorizontalHeaderItem(1, new QStandardItem("ID"));

	QStringList columnNames;

	loadXML();
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
	int num_players = ui.horizontalSlider_numPlayers->value();
	SEND_EVENT(&Event_StartDeathmatch(num_players));

	ToggleHelper::toggleMenu(false);
}

void Menu_Main::slot_loadInputList(int deviceId)
{
	loadInputList(deviceId);
}

void Menu_Main::slot_loadInputSettings(QModelIndex index)
{
	loadInputSettings(index.row());
}

void Menu_Main::slot_inputSettingsChanged()
{
	if(currentObject != nullptr)
	{
		currentObject->setInverted(ui.checkBox_Input->isChecked());
		currentObject->setSensitivity(static_cast<float>(ui.horizontalSlider_Input->value())/5000.0f);
	}
}

void Menu_Main::slot_setInputObject(QModelIndex index)
{
	Attribute_InputDevice* attr_device = itrInputDevice.at(ui.comboBox_Input->currentIndex());
	InputDevice* device = attr_device->device;

	std::vector<int> objectIndex = device->getMappedArray(index.row());
	if(objectIndex.size() > 0)
	{
		currentObject = device->getInputObjectArray()->inputObjects[objectIndex[0]];
		loadInputSettings(0);
	}
	else
	{
		currentObject = nullptr;
	}
}

void Menu_Main::loadDeviceList()
{
	deviceListModel->clear();

	InputDevice* device;
	Attribute_InputDevice* attr_inputDevice;

	QStandardItem* item;

	while(itrInputDevice.hasNext())
	{
		attr_inputDevice = itrInputDevice.getNext();
		device = attr_inputDevice->device;

		item = new QStandardItem(device->GetName().c_str());

		deviceListModel->appendRow(item);	
	}

	ui.comboBox_Input->setModel(deviceListModel);
}

void Menu_Main::loadInputList(int deviceId)
{
	inputListModel->clear();

	QStringList qStringList;

	qStringList.push_back("Action");
	qStringList.push_back("Key");

	inputListModel->setHorizontalHeaderLabels(qStringList);

	if(deviceId >= itrInputDevice.size())
		return;

	Attribute_InputDevice* attr_device = itrInputDevice.at(deviceId);
	InputDevice* device = attr_device->device;

	InputAction inputAction;

	ui.tableView_Input->setSortingEnabled(false);

	for(int i = 0; i < InputAction::ACTION_LAST; i++)
	{
		QList<QStandardItem*> rowList;
		QString qStr = inputAction.InputActionStrings[i].c_str();
		QStandardItem* actionItem = new QStandardItem(qStr);
		actionItem->setEditable(false);
		rowList.push_back(actionItem);

		std::vector<int> objectIndex = device->getMappedArray(i);
		InputObjectArray* inputObjectsArray = device->getInputObjectArray();

		QStandardItem* keyItem;

		qStr = "";

		std::string str = "";

		for(unsigned int j = 0; j < objectIndex.size(); j++)
		{
			if(i == 0 && j == 0)
			{
				currentObject = inputObjectsArray->inputObjects[objectIndex[j]];
				loadInputSettings(objectIndex[j]);
			}

			str += inputObjectsArray->inputObjects[objectIndex[j]]->getName();

			if(j != (objectIndex.size()-1))
				str += ", ";
		}

		qStr = str.c_str();

		keyItem = new QStandardItem(qStr);
		keyItem->setEditable(false);
		rowList.push_back(keyItem);

		inputListModel->appendRow(rowList);
	}

	ui.tableView_Input->setModel(inputListModel);
}

void Menu_Main::loadInputSettings(int objectId)
{
	if(currentObject != nullptr)
	{
		ui.horizontalSlider_Input->setValue(static_cast<int>(currentObject->getSensitivity()*5000.0f + 0.5f)); //Must fix better translation
		ui.checkBox_Input->setChecked(currentObject->isInverted());
	}
}