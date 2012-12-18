#include "Menu_Editor.h"

#include <QStandardItemModel>
#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>

Menu_Editor::Menu_Editor( Ui::MainMenu& ui, QWidget* parent ) : QWidget(parent), ui(ui)
{
	this->hide();

	model_entityBrowser = new QStandardItemModel(0, 1, this);
	model_entityBrowser->setHorizontalHeaderItem(0, new QStandardItem("Entity ID"));
	ui.treeView_EntityBrowser->setModel(model_entityBrowser);
	ui.treeView_EntityBrowser->setEditTriggers(QAbstractItemView::NoEditTriggers);


	connect(ui.pushButton_editorRefresh, SIGNAL(clicked()), this, SLOT(slot_editorRefresh()));
	connect(ui.treeView_EntityBrowser, SIGNAL(clicked(QModelIndex)), this, SLOT(slot_clicked_entityBrowser(QModelIndex)));
}

void Menu_Editor::slot_editorRefresh()
{
	// Reset column counting
	num_rows = 0;

	// Fill columns
	std::vector<int>* allPlayerOwner;				GET_ATTRIBUTE_OWNERS(allPlayerOwner, ATTRIBUTE_PLAYER);
	entityBrowser_add("Players", allPlayerOwner);
	std::vector<int>* allSpawnOwner;				GET_ATTRIBUTE_OWNERS(allSpawnOwner, ATTRIBUTE_SPAWNPOINT);
	entityBrowser_add("Spawn points", allPlayerOwner);
}

void Menu_Editor::entityBrowser_add(QString name, std::vector<int>* owners)
{
	// Create / reuse row
	QStandardItem* item = model_entityBrowser->item(num_rows);
	if(item == 0)
	{
		item = new QStandardItem(name);
		model_entityBrowser->setItem(num_rows, item);
	}
	else
	{
		item->setText(name);
	}
	num_rows++;
	
	// Fill row with data
	for(unsigned i=0; i<owners->size(); i++)
	{
		if(owners->at(i)!=0)
		{
			item->setChild(i, 0, new QStandardItem(QString::number(owners->at(i))));
			item->setChild(i, 1, new QStandardItem(QString::number(owners->at(i))));
		}
	}
}

void Menu_Editor::slot_clicked_entityBrowser( QModelIndex indexClicked )
{
	//indexClicked->ro;
	//model_entityBrowser->g
}

