#pragma once

#include "ui_MainMenu2.h"

class QStandardItemModel;
class InputObject;
class InputDevice;

class Menu_Input
{
public:
	Menu_Input(Ui::MainWindow* ui, QMainWindow* window);
	~Menu_Input();

	void Init(QStandardItemModel* inputList, QStandardItemModel* deviceList);

	void loadDeviceList();
	void loadInputList(int deviceId);
	void loadInputSettings(int objectId);

	void setInputObject(QModelIndex index);
	void updateMenu();

private:
	InputObject* currentObject;
	InputDevice* currentDevice;

	QStandardItemModel* inputListModel;
	QStandardItemModel* deviceListModel;

	Ui::MainWindow* ui;
};