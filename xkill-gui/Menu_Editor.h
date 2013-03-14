#pragma once

#include <QtGui/QMainWindow>
#include <xkill-utilities/IObserver.h>
#include "ui_MainWindow.h"

class QStandardItemModel;

class Menu_Editor : public QWidget, public IObserver
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
	void entityInspector_add(int index, QString name);
	void onEvent(Event* e);

public slots:
	void slot_editorRefresh();
	void slot_clicked_entityBrowser(QModelIndex index);
	void slot_clicked_entityInspector(QModelIndex index);
	void slot_changed_simulationSpeed(int speed);
	void slot_attributeInspector_refresh();
	void slot_attributeInspector_itemChanged();
};