#pragma once

#include <QtGui/QMainWindow>
#include "ToggleHelper.h"
#include "ui_MainMenu.h"

class Menu_Editor;
class QStandardItemModel;
class QStandardItemModel;
class InputObject;

class Menu_Main : public QMainWindow, public ToggleHelper
{
	Q_OBJECT

private:
	Ui::MainMenu ui;

public:
	Menu_Main(QWidget* parent);

	void parentMoveEvent();

	QStandardItemModel* levelListModel;
	QStandardItemModel* inputListModel;
	QStandardItemModel* deviceListModel;
	InputObject* currentObject;
	QString filePath;

	void loadXML();

public slots:
		void slot_quitToDesktop();
		void slot_startGame();
		void slot_selectLevel(int levelId);
		void slot_addLevel();
		void slot_removeLevel();
		void slot_saveLevel();
		void slot_loadInputList(int deviceId);
		void slot_loadInputSettings(QModelIndex index);
		void slot_inputSettingsChanged();
		void slot_setInputObject(QModelIndex index);

protected:
	void keyPressEvent(QKeyEvent* e);
	void mousePressEvent(QMouseEvent* e);

	void loadDeviceList();
	void loadInputList(int deviceId);
	void loadInputSettings(int objectId);
};