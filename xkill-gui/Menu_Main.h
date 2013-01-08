#pragma once

#include <QtGui/QMainWindow>
#include "ToggleHelper.h"
#include "ui_MainMenu.h"

class Menu_Editor;
class QStandardItemModel;

class Menu_Main : public QMainWindow, public ToggleHelper
{
	Q_OBJECT

private:
	Ui::MainMenu ui;
	Menu_Editor* editor;

public:
	Menu_Main(QWidget* parent);

	void parentMoveEvent();

	QStandardItemModel* model;
	QString filePath;

	void loadXML();

public slots:
		void slot_startGame();
		void slot_selectLevel(int levelId);
		void slot_addLevel();
		void slot_removeLevel();
		void slot_saveLevel();

protected:
	void keyPressEvent(QKeyEvent* e);
	void mousePressEvent(QMouseEvent* e);
};