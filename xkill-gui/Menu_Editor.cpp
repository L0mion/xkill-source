#include "Menu_Editor.h"

#include <QtGui/QStandardItemModel>

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/Entity.h>

ATTRIBUTES_DECLARE_ALL;

class SelectedAttribute
{
public:
	SelectedAttribute()
	{
		data = NULL;
	}
	~SelectedAttribute()
	{
		delete data;
	}

	DataItemList*	data;
	int				index;
	int				entityId;
};

static SelectedAttribute selected_attribute;


Menu_Editor::Menu_Editor( Ui::MainWindowClass& ui, QWidget* parent ) : QWidget(parent), ui(ui)
{
	ATTRIBUTES_INIT_ALL;
	this->hide();

	// Init Entity Browser
	model_entityBrowser = new QStandardItemModel(0, 1, this);
	model_entityBrowser->setHorizontalHeaderItem(0, new QStandardItem("Entity ID"));
	ui.treeView_entityBrowser->setModel(model_entityBrowser);
	ui.treeView_entityBrowser->setEditTriggers(QAbstractItemView::NoEditTriggers);

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
	//ui.treeView_attributeInspector->setEditTriggers(QAbstractItemView::NoEditTriggers);

	connect(ui.pushButton_editorRefresh, SIGNAL(clicked()), this, SLOT(slot_editorRefresh()));
	connect(ui.treeView_entityBrowser, SIGNAL(clicked(QModelIndex)), this, SLOT(slot_clicked_entityBrowser(QModelIndex)));
	connect(ui.treeView_entityInspector, SIGNAL(clicked(QModelIndex)), this, SLOT(slot_clicked_entityInspector(QModelIndex)));
	connect(ui.horizontalSlider_simulationSpeed, SIGNAL(valueChanged(int)), this, SLOT(slot_changed_simulationSpeed(int)));
	ui.dockWidget->hide();
}

void Menu_Editor::slot_editorRefresh()
{
	// Reset column counting
	num_rows = 0;

	// Fill columns
	std::vector<int>* allPlayerOwner = 	GET_ATTRIBUTE_OWNERS(player);
	entityBrowser_add("Players", allPlayerOwner);
	std::vector<int>* allSpawnOwner = GET_ATTRIBUTE_OWNERS(spawnPoint);
	entityBrowser_add("SpawnPoints", allSpawnOwner);
	std::vector<int>* allMeshOwner = GET_ATTRIBUTE_OWNERS(mesh);
	entityBrowser_add("Meshes", allMeshOwner);
	std::vector<int>* allPhysicsOwner = GET_ATTRIBUTE_OWNERS(physics);
	entityBrowser_add("PhysicsObjects", allPhysicsOwner);
	std::vector<int>* allProjectileOwner = GET_ATTRIBUTE_OWNERS(projectile);
	entityBrowser_add("Projectiles", allProjectileOwner);
}

void Menu_Editor::entityBrowser_add(QString name, std::vector<int>* owners)
{
	std::vector<Attribute_Player>* allPlayers		=	GET_ATTRIBUTES(player);

	std::vector<int>* allSpawnOwner = GET_ATTRIBUTE_OWNERS(spawnPoint);
	allPlayers						= GET_ATTRIBUTES(player);
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
		model_entityInspector->setHorizontalHeaderItem(0, new QStandardItem("Attributes"));
		int entityId = data.toInt();
		std::vector<Entity>* allEntity; GET_ENTITIES(allEntity);
		Entity* entity = &allEntity->at(entityId);
		selected_attribute.entityId = entityId;

		// Get all attributes as enums and build menu from it
		std::vector<int> enums = entity->getAttributesAsEnums();
		for(std::vector<int>::iterator it = enums.begin(); it != enums.end(); ++it)
		{
			int e = *it;
			if(e == ATTRIBUTE_POSITION)
				entityInspector_add("Position");
			if(e == ATTRIBUTE_SPATIAL)
				entityInspector_add("Spatial");
			if(e == ATTRIBUTE_RENDER)
				entityInspector_add("Render");
			if(e == ATTRIBUTE_DEBUGSHAPE)
				entityInspector_add("Debug Shape");
			if(e == ATTRIBUTE_PHYSICS)
				entityInspector_add("Physics");
			if(e == ATTRIBUTE_CAMERA)
				entityInspector_add("Camera");
			if(e == ATTRIBUTE_INPUT)
				entityInspector_add("Input");
			if(e == ATTRIBUTE_PLAYER)
				entityInspector_add("Player");
			if(e == ATTRIBUTE_BOUNDING)
				entityInspector_add("Bounding Shape");
			if(e == ATTRIBUTE_MESH)
				entityInspector_add("Mesh");
			if(e == ATTRIBUTE_PROJECTILE)
				entityInspector_add("Projectile");
			if(e == ATTRIBUTE_HEALTH)
				entityInspector_add("Health");
			if(e == ATTRIBUTE_DAMAGE)
				entityInspector_add("Damage");
			if(e == ATTRIBUTE_SPAWNPOINT)
				entityInspector_add("Spawn Point");
			if(e == ATTRIBUTE_WEAPONSTATS)
				entityInspector_add("Weapon Stats");
			if(e == ATTRIBUTE_EXPLOSIONSPHERE)
				entityInspector_add("Explosion Sphere");
		}
	}
}

void Menu_Editor::slot_clicked_entityInspector( QModelIndex indexClicked )
{
	int index = indexClicked.row();
	selected_attribute.index = index;

	// Fetch DataList from the selected Entity
	Entity* entity = itr_entity->at(selected_attribute.entityId);
	delete selected_attribute.data;
	selected_attribute.data = entity->getDataListFromAttribute(index);
	DataItemList* list = selected_attribute.data;


	//
	// Build menu from DataList
	//

	int num_items = 0;
	while(list->hasNext())
	{
		// TRUE: Item doesn't exist, create new Item
		QStandardItem* item_property = model_attributeInspector->item(num_items, 0);
		QStandardItem* item_value	 = model_attributeInspector->item(num_items, 1);
		if(!item_property)
		{
			item_property = new QStandardItem();
			item_value	  = new QStandardItem();
			model_attributeInspector->setItem(num_items, 0, item_property);
			model_attributeInspector->setItem(num_items, 1, item_value);
		}
		num_items++;


		//
		// Fill item with data
		//

		DataItem data = list->getNext();
		
		// set label
		item_property->setText(data.label.c_str());
		item_property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
		// set data
		DataItem::DataType type = data.type;
		if(type == DataItem::_BOOL)
		{
			bool value = *data.value._bool;
			model_attributeInspector->setData(item_value->index(), QVariant(value));
		}
		if(type == DataItem::_INT)
		{
			int value = *data.value._int;
			model_attributeInspector->setData(item_value->index(), QVariant(value));
		}
		if(type == DataItem::_FLOAT)
		{
			double value = (double)*data.value._float;
			model_attributeInspector->setData(item_value->index(), QVariant(value));
		}

		if(type == DataItem::_FLOAT3)
		{
			Float3 value = *data.value._float3;
			float x = value.x;
		
			QStandardItem* child = new QStandardItem("bUU");
			item_value->appendRow(child);
			//child->setData(QVariant(x));
		}

		if(type == DataItem::_FLOAT4)
		{
			Float4 value = *data.value._float4;
			double x = (double)value.x;
			model_attributeInspector->setData(item_value->index(), QVariant(x));
		}
	}

	// Remove unused rows
	int excessRows = model_attributeInspector->rowCount() - num_items;
	if(excessRows>0)
		model_attributeInspector->removeRows(num_items, excessRows);
}

void Menu_Editor::entityInspector_add(QString name)
{
	model_entityInspector->appendRow(new QStandardItem(name));
}

void Menu_Editor::slot_changed_simulationSpeed(int speed)
{
	// Set simulation speed and update label
	float simulationSpeed = (float)speed/100;
	ui.label_simulationSpeed->setText(QString::number(simulationSpeed));
	settings->timeScale = simulationSpeed;
}


