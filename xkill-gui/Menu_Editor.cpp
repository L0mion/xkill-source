#include "Menu_Editor.h"

#include <QtGui/QStandardItemModel>

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/Entity.h>


Menu_Editor::Menu_Editor( Ui::MainMenu& ui, QWidget* parent ) : QWidget(parent), ui(ui)
{
	this->hide();

	// Init Entity Browser
	model_entityBrowser = new QStandardItemModel(0, 1, this);
	model_entityBrowser->setHorizontalHeaderItem(0, new QStandardItem("Entity ID"));
	ui.treeView_entityBrowser->setModel(model_entityBrowser);
	//ui.treeView_entityBrowser->setEditTriggers(QAbstractItemView::NoEditTriggers);

	// Init Entity Inspector
	model_entityInspector = new QStandardItemModel(0, 1, this);
	model_entityInspector->setHorizontalHeaderItem(0, new QStandardItem("Attributes"));
	ui.treeView_entityInspector->setModel(model_entityInspector);
	ui.treeView_entityInspector->setEditTriggers(QAbstractItemView::NoEditTriggers);

	// Init Attribute Inspector
	model_attributeInspector = new QStandardItemModel(0, 2, this);
	model_attributeInspector->setHorizontalHeaderItem(0, new QStandardItem("Property"));
	model_attributeInspector->setHorizontalHeaderItem(1, new QStandardItem("Value"));
	ui.treeView_attributeInspector->setModel(model_attributeInspector);
	ui.treeView_attributeInspector->setEditTriggers(QAbstractItemView::NoEditTriggers);

	connect(ui.pushButton_editorRefresh, SIGNAL(clicked()), this, SLOT(slot_editorRefresh()));
	connect(ui.treeView_entityBrowser, SIGNAL(clicked(QModelIndex)), this, SLOT(slot_clicked_entityBrowser(QModelIndex)));
	connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slot_tab_changed(int)));
	//ui.groupBox_AttributeInspector->hide();
}

void Menu_Editor::slot_editorRefresh()
{
	// Reset column counting
	num_rows = 0;

	// Fill columns
	std::vector<int>* allPlayerOwner = 	GET_ATTRIBUTE_OWNERS(player);
	entityBrowser_add("Players", allPlayerOwner);
	std::vector<int>* allSpawnOwner = GET_ATTRIBUTE_OWNERS(spawnPoint);
	entityBrowser_add("Spawn points", allSpawnOwner);
	std::vector<int>* allMeshOwner = GET_ATTRIBUTE_OWNERS(mesh);
	entityBrowser_add("Meshes", allMeshOwner);
	std::vector<int>* allPhysicsOwner = GET_ATTRIBUTE_OWNERS(physics);
	entityBrowser_add("Physics objects", allPhysicsOwner);
	std::vector<int>* allProjectileOwner = GET_ATTRIBUTE_OWNERS(projectile);
	entityBrowser_add("Projectiles", allProjectileOwner);
}

void Menu_Editor::entityBrowser_add(QString name, std::vector<int>* owners)
{
	std::vector<Attribute_Player>* allPlayers		=	GET_ATTRIBUTES(player);

	std::vector<int>* allSpawnOwner = GET_ATTRIBUTE_OWNERS(spawnPoint);
	allPlayers		=	GET_ATTRIBUTES(player);
	// Create / reuse row
	QStandardItem* item = model_entityBrowser->item(num_rows);
	// TRUE: Item doesn't exist, create new Item
	if(item == 0)
	{
		item = new QStandardItem();
		model_entityBrowser->setItem(num_rows, item);
	}
	num_rows++;
	
	// Fill row with data
	int num_entities = 0;
	for(unsigned i=0; i<owners->size(); i++)
	{
		if(owners->at(i)!=0)
		{
			// Create data item
			QStandardItem* data = new QStandardItem();
			item->setChild(num_entities, 0, data);
			model_entityBrowser->setData(data->index(), QVariant(owners->at(i)));

			// Increment index
			num_entities++;
		}
	}

	// Remove unused rows
	int excessRows = item->rowCount() - num_entities;
	if(excessRows>0)
		item->removeRows(num_entities, excessRows);

	// Set name
	item->setText("[" + QString::number(num_entities) + "] " + name);
}

void Menu_Editor::slot_clicked_entityBrowser( QModelIndex indexClicked )
{
	QVariant data = model_entityBrowser->data(indexClicked);
	if(data.type() == QVariant::Int)
	{
		model_entityInspector->clear();
		int entityId = data.toInt();
		std::vector<Entity>* allEntity; GET_ENTITIES(allEntity);
		Entity* entity = &allEntity->at(entityId);

		if(entity->hasAttribute(ATTRIBUTE_POSITION))
			entityInspector_add("Position");
		if(entity->hasAttribute(ATTRIBUTE_SPATIAL))
			entityInspector_add("Spatial");
		if(entity->hasAttribute(ATTRIBUTE_RENDER))
			entityInspector_add("Render");
		if(entity->hasAttribute(ATTRIBUTE_DEBUGSHAPE))
			entityInspector_add("Debug Shape");
		if(entity->hasAttribute(ATTRIBUTE_PHYSICS))
			entityInspector_add("Physics");
		if(entity->hasAttribute(ATTRIBUTE_CAMERA))
			entityInspector_add("Camera");
		if(entity->hasAttribute(ATTRIBUTE_INPUT))
			entityInspector_add("Input");
		if(entity->hasAttribute(ATTRIBUTE_INPUTDEVICESETTINGS))
			entityInspector_add("Input Device Settings");
		if(entity->hasAttribute(ATTRIBUTE_PLAYER))
			entityInspector_add("Player");
		if(entity->hasAttribute(ATTRIBUTE_BOUNDING))
			entityInspector_add("Bounding Shape");
		if(entity->hasAttribute(ATTRIBUTE_MESH))
			entityInspector_add("Mesh");
		if(entity->hasAttribute(ATTRIBUTE_PROJECTILE))
			entityInspector_add("Projectile");
		if(entity->hasAttribute(ATTRIBUTE_HEALTH))
			entityInspector_add("Health");
		if(entity->hasAttribute(ATTRIBUTE_DAMAGE))
			entityInspector_add("Damage");
		if(entity->hasAttribute(ATTRIBUTE_SPAWNPOINT))
			entityInspector_add("Spawn Point");
		if(entity->hasAttribute(ATTRIBUTE_WEAPONSTATS))
			entityInspector_add("Weapon Stats");
		if(entity->hasAttribute(ATTRIBUTE_EXPLOSIONSPHERE))
			entityInspector_add("Explosion Sphere");
	}
}

void Menu_Editor::entityInspector_add(QString name)
{
	model_entityInspector->appendRow(new QStandardItem(name));
}

void Menu_Editor::slot_tab_changed( int index )
{
	if(index == 2)
		slot_editorRefresh();
}

