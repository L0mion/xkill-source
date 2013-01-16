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
		entityId = -1;
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
	SUBSCRIBE_TO_EVENT(this, EVENT_UPDATE);
	ATTRIBUTES_INIT_ALL;
	this->hide();

	// Init Entity Browser
	model_entityBrowser = new QStandardItemModel(0, 1, this);
	model_entityBrowser->setHorizontalHeaderItem(0, new QStandardItem("Entity ID"));
	ui.treeView_entityBrowser->setModel(model_entityBrowser);
	ui.treeView_entityBrowser->setEditTriggers(QAbstractItemView::NoEditTriggers);

	// Init Entity Inspector
	model_entityInspector = new QStandardItemModel(0, 2, this);
	model_entityInspector->setHorizontalHeaderItem(0, new QStandardItem("Attributes"));
	model_entityInspector->setHorizontalHeaderItem(1, new QStandardItem("ID"));
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
	connect(ui.horizontalSlider_simulationSpeed, SIGNAL(valueChanged(int)), this, SLOT(slot_changed_simulationSpeed(int)));
	connect(ui.checkBox_autoRefresh, SIGNAL(clicked()),		this,	SLOT(slot_attributeInspector_refresh()));
	connect(ui.dockWidget, SIGNAL(visibilityChanged(bool)), this,	SLOT(slot_editorRefresh()));
	 
	ui.dockWidget->hide();
}

void Menu_Editor::slot_editorRefresh()
{
	if(ui.dockWidget->isVisible())
	{
		// Reset column counting
		num_rows = 0;

		// Fill columns
		std::vector<int>* allPlayerOwner = 	GET_ATTRIBUTE_OWNERS(player);
		entityBrowser_add("Players", allPlayerOwner);
		std::vector<int>* allPositionOwner = GET_ATTRIBUTE_OWNERS(position);
		entityBrowser_add("Positions", allPositionOwner);
		std::vector<int>* allSpawnOwner = GET_ATTRIBUTE_OWNERS(spawnPoint);
		entityBrowser_add("SpawnPoints", allSpawnOwner);
		std::vector<int>* allRenderOwner = GET_ATTRIBUTE_OWNERS(render);
		entityBrowser_add("Render", allRenderOwner);
		std::vector<int>* allMeshOwner = GET_ATTRIBUTE_OWNERS(mesh);
		entityBrowser_add("Meshes", allMeshOwner);
		std::vector<int>* allPhysicsOwner = GET_ATTRIBUTE_OWNERS(physics);
		entityBrowser_add("PhysicsObjects", allPhysicsOwner);
		std::vector<int>* allInputDeviceOwner = GET_ATTRIBUTE_OWNERS(inputDevice);
		entityBrowser_add("InputDevices", allInputDeviceOwner);
		std::vector<int>* allProjectileOwner = GET_ATTRIBUTE_OWNERS(projectile);
		entityBrowser_add("Projectiles", allProjectileOwner);
		
	}
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

void Menu_Editor::entityInspector_add(int index, QString name)
{
	model_entityInspector->setItem(index, new QStandardItem(name));
}

void Menu_Editor::onEvent(Event* e)
{
	EventType type = e->getType();
	// HACK don't update to often
	static int updateLimit = 5;
	static int updateCounter = updateLimit;
	switch(type) 
	{
	case EVENT_UPDATE:
		if(ui.checkBox_autoRefresh->isChecked())
		{
			updateCounter--;
			if(updateCounter<0)
			{
				updateCounter = updateLimit;
				slot_attributeInspector_refresh();
			}
		}
	default:
		break;
	}
}

void Menu_Editor::slot_clicked_entityBrowser( QModelIndex indexClicked )
{
	QVariant data = model_entityBrowser->data(indexClicked);
	if(data.type() == QVariant::Int)
	{
		//model_entityInspector->setHorizontalHeaderItem(1, new QStandardItem("ID"));
		int entityId = data.toInt();
		std::vector<Entity>* allEntity; GET_ENTITIES(allEntity);
		Entity* entity = &allEntity->at(entityId);
		selected_attribute.entityId = entityId;

		// Get all attributes as enums and build menu from it
		std::vector<AttributeController>* attributes = entity->getAttributeControllers();
		int num_items = 0;
		for(int i=0; i<attributes->size(); i++)
		{
			int attributeType = attributes->at(i).type;
			switch (attributeType) 
			{
			case ATTRIBUTE_POSITION:
				entityInspector_add(num_items, "Position");                                   
				break;
			case ATTRIBUTE_SPATIAL:
				entityInspector_add(num_items, "Spatial");
				break;
			case ATTRIBUTE_RENDER:
				entityInspector_add(num_items, "Render");
				break;
			case ATTRIBUTE_DEBUGSHAPE:
				entityInspector_add(num_items, "DebugShape");
				break;
			case ATTRIBUTE_PHYSICS:
				entityInspector_add(num_items, "Physics");
				break;
			case ATTRIBUTE_CAMERA:
				entityInspector_add(num_items, "Camera");
				break;
			case ATTRIBUTE_INPUT:
				entityInspector_add(num_items, "Input");
				break;
			case ATTRIBUTE_PLAYER:
				entityInspector_add(num_items, "Player");
				break;
			case ATTRIBUTE_BOUNDING:
				entityInspector_add(num_items, "Bounding");
				break;
			case ATTRIBUTE_PROJECTILE:
				entityInspector_add(num_items, "Projectile");
				break;
			case ATTRIBUTE_LIGHT_DIRECTIONAL:
				entityInspector_add(num_items, "LightDir");
				break;
			case ATTRIBUTE_LIGHT_POINT:
				entityInspector_add(num_items, "LightPoint");
				break;
			case ATTRIBUTE_LIGHT_SPOT:
				entityInspector_add(num_items, "LightSpot");
				break;
			case ATTRIBUTE_MESH:
				entityInspector_add(num_items, "Mesh");
				break;
			case ATTRIBUTE_HEALTH:
				entityInspector_add(num_items, "Health");
				break;
			case ATTRIBUTE_DAMAGE:
				entityInspector_add(num_items, "Damage");
				break;
			case ATTRIBUTE_SPAWNPOINT:
				entityInspector_add(num_items, "SpawnPoint");
				break;
			case ATTRIBUTE_WEAPONSTATS:
				entityInspector_add(num_items, "WeaponStats");
				break;
			case ATTRIBUTE_EXPLOSIONSPHERE:
				entityInspector_add(num_items, "ExplosionSphere");
				break;
			case ATTRIBUTE_INPUTDEVICE:
				entityInspector_add(num_items, "InputDevice");
				break;
			default:
				entityInspector_add(num_items, "UNKOWN");
				break;
			}
			QStandardItem* item_id = new QStandardItem();
			model_entityInspector->setItem(num_items, 1, item_id);
			model_entityInspector->setData(item_id->index(), QVariant(attributes->at(i).index));
			num_items++;

			
			
		}
		//model_entityInspector->setItem(1, new QStandardItem("Blaj"));

			// Remove unused rows
		int excessRows = model_entityInspector->rowCount() - num_items;
		if(excessRows>0)
			model_entityInspector->removeRows(num_items, excessRows);
	}
}

void Menu_Editor::slot_clicked_entityInspector( QModelIndex indexClicked )
{
	int index = indexClicked.row();
	selected_attribute.index = index;

	slot_attributeInspector_refresh();
}

void Menu_Editor::slot_changed_simulationSpeed(int speed)
{
	// Set simulation speed and update label
	float simulationSpeed = (float)speed/100;
	ui.label_simulationSpeed->setText(QString::number(simulationSpeed));
	settings->timeScale = simulationSpeed;
}

void Menu_Editor::slot_attributeInspector_refresh()
{

	if(ui.checkBox_autoRefresh->isChecked())
	{
		ui.treeView_attributeInspector->setEditTriggers(QAbstractItemView::NoEditTriggers);
		ui.treeView_attributeInspector->setSelectionMode(QAbstractItemView::NoSelection);
	}
	else
	{
		ui.treeView_attributeInspector->setEditTriggers(QAbstractItemView::DoubleClicked);
		ui.treeView_attributeInspector->setSelectionMode(QAbstractItemView::SingleSelection);
	}

	/*if(ui.checkBox_autoRefresh->isChecked())
	{
		ui.treeView_attributeInspector->setEnabled(false);
	}
	else
	{
		ui.treeView_attributeInspector->setEnabled(true);
	}
*/

	//
	// Fetch entity we will inspect attributes from
	//

	// abort if Entity do not exists
	if(selected_attribute.entityId == -1)
		return;

	Entity* entity = itr_entity->at(selected_attribute.entityId);

	// fetch DataList from the selected Entity
	int index = selected_attribute.index;
	DataItemList* list = entity->getDataListFromAttribute(index);

	// abort if attribute does not exist
	if(list == NULL)
		return;

	// save data (we need it to save data into attributes later on)
	delete selected_attribute.data; // delete previous data
	selected_attribute.data = list;



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
		else
		{
			// Clean reused attribute
			if(item_property->hasChildren())
			{
				item_property->removeRows(0, item_property->rowCount());
			}
			model_attributeInspector->setData(item_value->index(), QVariant(""));
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

		if(type == DataItem::_FLOAT2)
		{
			Float2 value = *data.value._float2;
			double x = value.x;
			{
				item_property->setChild(0, 0, new QStandardItem("X"));
				QStandardItem* child = new QStandardItem();
				item_property->setChild(0, 1, child);
				model_attributeInspector->setData(child->index(), QVariant(x));
				//model_attributeInspector->setData(model_attributeInspector->index(0), Qt::blue, Qt::BackgroundRole);
			}
			double y = value.y;
			{
				item_property->setChild(1, 0, new QStandardItem("Y"));
				QStandardItem* child = new QStandardItem();
				item_property->setChild(1, 1,child);
				model_attributeInspector->setData(child->index(), QVariant(y));
			}
		}

		if(type == DataItem::_FLOAT3)
		{
			Float3 value = *data.value._float3;
			double x = value.x;
			{
				item_property->setChild(0, 0, new QStandardItem("X"));
				QStandardItem* child = new QStandardItem();
				item_property->setChild(0, 1, child);
				model_attributeInspector->setData(child->index(), QVariant(x));
				//model_attributeInspector->setData(model_attributeInspector->index(0), Qt::blue, Qt::BackgroundRole);
			}
			double y = value.y;
			{
				item_property->setChild(1, 0, new QStandardItem("Y"));
				QStandardItem* child = new QStandardItem();
				item_property->setChild(1, 1,child);
				model_attributeInspector->setData(child->index(), QVariant(y));
			}
			double z = value.z;
			{
				item_property->setChild(2, 0, new QStandardItem("Z"));
				QStandardItem* child = new QStandardItem();
				item_property->setChild(2, 1, child);
				model_attributeInspector->setData(child->index(), QVariant(z));
			}
		}

		if(type == DataItem::_FLOAT4)
		{
			Float4 value = *data.value._float4;
			double x = value.x;
			{
				item_property->setChild(0, 0, new QStandardItem("X"));
				QStandardItem* child = new QStandardItem();
				item_property->setChild(0, 1, child);
				model_attributeInspector->setData(child->index(), QVariant(x));
				//model_attributeInspector->setData(model_attributeInspector->index(0), Qt::blue, Qt::BackgroundRole);
			}
			double y = value.y;
			{
				item_property->setChild(1, 0, new QStandardItem("Y"));
				QStandardItem* child = new QStandardItem();
				item_property->setChild(1, 1,child);
				model_attributeInspector->setData(child->index(), QVariant(y));
			}
			double z = value.z;
			{
				item_property->setChild(2, 0, new QStandardItem("Z"));
				QStandardItem* child = new QStandardItem();
				item_property->setChild(2, 1, child);
				model_attributeInspector->setData(child->index(), QVariant(z));
			}
			double w = value.w;
			{
				item_property->setChild(3, 0, new QStandardItem("W"));
				QStandardItem* child = new QStandardItem();
				item_property->setChild(3, 1, child);
				model_attributeInspector->setData(child->index(), QVariant(w));
			}
		}

		if(type == DataItem::_FLOAT4X4)
		{
			Float4x4 value = *data.value._float4x4;
			{
				item_property->setChild(0, 0, new QStandardItem("MATRIX"));
			}
		}

		if(type == DataItem::_STRING)
		{
			std::string value = *data.value._string;
			model_attributeInspector->setData(item_value->index(), QVariant(value.c_str()));
		}


		if(type == DataItem::_ATTRIBUTE_POINTER)
		{
			int value = *data.value._int;
			model_attributeInspector->setData(item_value->index(), QVariant(value));
		}

		if(type == DataItem::_INVALID)
		{
			model_attributeInspector->setData(item_value->index(), QVariant(""));
		}
	}

	// Remove unused rows
	int excessRows = model_attributeInspector->rowCount() - num_items;
	if(excessRows>0)
		model_attributeInspector->removeRows(num_items, excessRows);
	ui.treeView_attributeInspector->expandAll();
}


