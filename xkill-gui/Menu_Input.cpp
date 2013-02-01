#include "Menu_Input.h"

#include <xkill-input/InputDevice.h>
#include <xkill-input/InputObject.h>
#include <xkill-input/InputObjectArray.h>


#include <QtGui/QStandardItemModel>

#include <xkill-utilities/Util.h>
#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/AttributeManager.h>

ATTRIBUTES_DECLARE_ALL

Menu_Input::Menu_Input(Ui::MainMenu* ui, QMainWindow* window)
{
	ATTRIBUTES_INIT_ALL

	this->ui = ui;
	
	window->connect(ui->comboBox_Input,				SIGNAL(currentIndexChanged(int)),	window,		SLOT(slot_loadInputList(int)));
	window->connect(ui->tableView_Input,			SIGNAL(clicked(QModelIndex)),		window,		SLOT(slot_loadInputSettings(QModelIndex)));
	window->connect(ui->tableView_Input,			SIGNAL(clicked(QModelIndex)),		window,		SLOT(slot_setInputObject(QModelIndex)));
	window->connect(ui->horizontalSlider_Input,		SIGNAL(sliderMoved(int)),			window,		SLOT(slot_inputSettingsChanged()));
	window->connect(ui->checkBox_Input_Inverted,	SIGNAL(clicked()),					window,		SLOT(slot_inputSettingsChanged()));
	window->connect(ui->pushButton_Input,			SIGNAL(clicked()),					window,		SLOT(slot_inputSettingsChanged()));
	window->connect(ui->checkBox_Input_Rumble,		SIGNAL(clicked()),					window,		SLOT(slot_inputSettingsChanged()));
}

Menu_Input::~Menu_Input()
{

}

void Menu_Input::Init(QStandardItemModel* inputList, QStandardItemModel* deviceList)
{
	inputListModel = inputList;
	deviceListModel = deviceList;

	currentObject = nullptr;
	currentDevice = nullptr;

	loadDeviceList();
	loadInputList(0);
}

void Menu_Input::loadDeviceList()
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

	ui->comboBox_Input->setModel(deviceListModel);
}

void Menu_Input::loadInputList(int deviceId)
{
	inputListModel->clear();

	QStringList qStringList;

	qStringList.push_back("Action");
	qStringList.push_back("Key");

	inputListModel->setHorizontalHeaderLabels(qStringList);

	if(deviceId >= itrInputDevice.size())
		return;

	Attribute_InputDevice* attr_device = itrInputDevice.at(deviceId);
	currentDevice = attr_device->device;

	InputAction inputAction;

	ui->tableView_Input->setSortingEnabled(false);

	for(int i = 0; i < InputAction::ACTION_LAST; i++)
	{
		QList<QStandardItem*> rowList;
		QString qStr = inputAction.InputActionStrings[i].c_str();
		QStandardItem* actionItem = new QStandardItem(qStr);
		actionItem->setEditable(false);
		rowList.push_back(actionItem);

		std::vector<int> objectIndex = currentDevice->getMappedArray(i);
		InputObjectArray* inputObjectsArray = currentDevice->getInputObjectArray();

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

	ui->tableView_Input->setModel(inputListModel);
	ui->tableView_Input->setColumnWidth(0,180);
	ui->tableView_Input->setColumnWidth(1,180);
}

void Menu_Input::loadInputSettings(int objectId)
{
	if(currentObject != nullptr)
	{
		ui->horizontalSlider_Input->setValue(static_cast<int>(currentObject->getSensitivity()*5000.0f + 0.5f)); //Must fix better translation
		ui->checkBox_Input_Inverted->setChecked(currentObject->isInverted());
	}

	if(currentDevice != nullptr)
	{
		ui->checkBox_Input_Rumble->setChecked(currentDevice->isForceFeedbackEnabled());
	}
}

void Menu_Input::setInputObject(QModelIndex index)
{
	Attribute_InputDevice* attr_device = itrInputDevice.at(ui->comboBox_Input->currentIndex());
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

void Menu_Input::updateMenu()
{
	if(currentObject != nullptr)
	{
		currentObject->setInverted(ui->checkBox_Input_Inverted->isChecked());
		currentObject->setSensitivity(static_cast<float>(ui->horizontalSlider_Input->value())/5000.0f);
	}

	if(currentDevice != nullptr)
	{
		currentDevice->setForceFeedbackEnabled(ui->checkBox_Input_Rumble->isChecked());	
	}
}
