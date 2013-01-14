#pragma once

#include <QtGui/QMainWindow>
#include "ui_MainWindow.h"

class QStandardItemModel;

class Menu_Editor : public QWidget
{
	Q_OBJECT

private:
	int num_rows;
	Ui::MainWindowClass& ui;
	QStandardItemModel* model_entityBrowser;
	QStandardItemModel* model_entityInspector;
	QStandardItemModel* model_attributeInspector;

public:
	Menu_Editor(Ui::MainWindowClass& ui, QWidget* parent);
	void entityBrowser_add(QString name, std::vector<int>* owners);
	void entityInspector_add(QString name);
public slots:
	void slot_editorRefresh();
	void slot_clicked_entityBrowser(QModelIndex index);
	void slot_clicked_entityInspector(QModelIndex index);
	void slot_changed_simulationSpeed(int speed);
};