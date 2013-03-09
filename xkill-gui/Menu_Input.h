#pragma once

#include "ui_MainMenu.h"

class QStandardItemModel;
class InputObject;
class InputDevice;

class Menu_Input : QObject
{
	Q_OBJECT

public:
	Menu_Input(Ui::MainWindow* ui, QMainWindow* window);
	~Menu_Input();

	void Init(QStandardItemModel* inputList, QStandardItemModel* deviceList);

	void loadDeviceList();
	

public slots:
	void setInputObject(QModelIndex index);
	void loadInputList(int deviceId);
	void loadInputSettings(int objectId);
	//! Updates the in game data based on the qt menu
	void settingsMenuUpdated();

private:
	InputObject* currentObject;
	InputDevice* currentDevice;

	QStandardItemModel* inputListModel;
	QStandardItemModel* deviceListModel;

	Ui::MainWindow* ui;
};