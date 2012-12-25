#pragma once

#include <QMainWindow>
#include "ui_MainMenu.h"

class QStandardItemModel;

class Menu_Editor : public QWidget
{
	Q_OBJECT

private:
	int num_rows;
	Ui::MainMenu& ui;
	QStandardItemModel* model_entityBrowser;
	QStandardItemModel* model_entityInspector;
	QStandardItemModel* model_attributeInspector;

public:
	Menu_Editor(Ui::MainMenu& ui, QWidget* parent);
	void entityBrowser_add(QString name, std::vector<int>* owners);
	void entityInspector_add(QString name);
public slots:
	void slot_editorRefresh();
	void slot_clicked_entityBrowser(QModelIndex index);
	void slot_tab_changed(int index)
	{
		if(index == 2)
			slot_editorRefresh();
	}
};