#include "Menu_Editor.h"

#include <QtGui/QStandardItemModel>

#include <xkill-utilities/Util.h>

ATTRIBUTES_DECLARE_ALL;

class SelectedAttribute
{
public:
	SelectedAttribute()
	{
		data = NULL;
		entityId = -1;
		ignoreRefresh = false;
	}
	~SelectedAttribute()
	{
		delete data;
	}

	DataItemList*	data;
	int				index;
	int				entityId;
	bool			ignoreRefresh; // Used to prevent wasteful updating
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
	ui.treeView_attributeInspector->setColumnWidth(0,180);
	


	//ui.treeView_attributeInspector->setEditTriggers(QAbstractItemView::NoEditTriggers);

	connect(ui.pushButton_editorRefresh,			SIGNAL(clicked()),						this,	SLOT(slot_editorRefresh()));
	connect(ui.treeView_entityBrowser,				SIGNAL(clicked(QModelIndex)),			this,	SLOT(slot_clicked_entityBrowser(QModelIndex)));
	connect(ui.treeView_entityInspector,			SIGNAL(clicked(QModelIndex)),			this,	SLOT(slot_clicked_entityInspector(QModelIndex)));
	connect(ui.horizontalSlider_simulationSpeed,	SIGNAL(valueChanged(int)),				this,	SLOT(slot_changed_simulationSpeed(int)));
	connect(ui.checkBox_showDebugPhysics,			SIGNAL(stateChanged(int)),				this,	SLOT(slot_showDebugPhysics()));
	connect(ui.checkBox_autoRefresh,				SIGNAL(stateChanged(int)),				this,	SLOT(slot_attributeInspector_refresh()));
	connect(ui.dockWidget,							SIGNAL(visibilityChanged(bool)),		this,	SLOT(slot_editorRefresh()));
	connect(model_attributeInspector,				SIGNAL(itemChanged(QStandardItem*)),	this,	SLOT(slot_attributeInspector_itemChanged()));
	 
	ui.dockWidget->hide();

	// Show debuglines in debug
	#ifdef XKILL_DEBUG
	{
		ui.checkBox_showDebugPhysics->setChecked(true);
	}
#endif
}

void Menu_Editor::slot_editorRefresh()
{
	if(ui.dockWidget->isVisible())
	{
		// Reset column counting
		num_rows = 0;

		// Fill columns
		entityBrowser_add("Players",				&itrPlayer.getAllOwnerId());
		entityBrowser_add("Positions",				&itrPosition.getAllOwnerId());
		entityBrowser_add("PlayerSpawnPoints",		&itrPlayerSpawnPoint.getAllOwnerId());
		entityBrowser_add("Render",					&itrRender.getAllOwnerId());
		entityBrowser_add("Meshes",					&itrMesh.getAllOwnerId());
		entityBrowser_add("PhysicsObjects",			&itrPhysics.getAllOwnerId());
		entityBrowser_add("InputDevices",			&itrInputDevice.getAllOwnerId());
		entityBrowser_add("Projectiles",			&itrProjectile.getAllOwnerId());
		entityBrowser_add("Pickupables",			&itrPickupable.getAllOwnerId());
		entityBrowser_add("PickupablesSpawnPoints",	&itrPickupablesSpawnPoint.getAllOwnerId());
		entityBrowser_add("Lights",					&itrLightPoint.getAllOwnerId());
	}
}

void Menu_Editor::entityBrowser_add(QString name, std::vector<int>* owners)
{
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
		break;
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
			// Name
			std::string attributeName = attributes->at(i).getAttribute()->getName();
			entityInspector_add(num_items, attributeName.c_str());                                   
		
			// ID
			QStandardItem* item_id = new QStandardItem();
			model_entityInspector->setItem(num_items, 1, item_id);
			model_entityInspector->setData(item_id->index(), QVariant(attributes->at(i).getIndex()));
			
			num_items++;
		}

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
	// Disable attributes from refreshing
	// which prevents unnecessary updating
	// attributes while creating menu

	selected_attribute.ignoreRefresh = true;

	if(ui.checkBox_autoRefresh->isChecked())
	{
		/*ui.treeView_attributeInspector->setEditTriggers(QAbstractItemView::NoEditTriggers);
		ui.treeView_attributeInspector->setSelectionMode(QAbstractItemView::NoSelection);*/
		ui.treeView_attributeInspector->setEnabled(false);
	}
	else
	{
		/*ui.treeView_attributeInspector->setEditTriggers(QAbstractItemView::DoubleClicked);
		ui.treeView_attributeInspector->setSelectionMode(QAbstractItemView::SingleSelection);*/
		ui.treeView_attributeInspector->setEnabled(true);
	}

	//
	// Fetch entity we will inspect attributes from
	//

	// abort if Entity do not exists
	if(selected_attribute.entityId == -1)
		return;

	Entity* entity = itr_entity->at(selected_attribute.entityId);

	// fetch DataList from the selected  attribute 
	// of the selected Entity

	int index = selected_attribute.index;
	IAttribute* attribute = entity->getAttributeInterface(index);

	// abort if attribute does not exist
	if(attribute == NULL)
		return;

	DataItemList* list = attribute->getDataList();

	// save data (we need it to save data into attributes later on)
	delete selected_attribute.data; // delete previous data
	selected_attribute.data = list;



	//////////////////////////////////////////////////////////////////////////
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


		//////////////////////////////////////////////////////////////////////////
		// Fill item with data
		//

		DataItem* data = list->getNext();

		// set label
		item_property->setText(data->label.c_str());
		item_property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
		

		//
		// create menu for the "specific" type
		//

		DataItem::DataType type = data->type;
		switch(type) 
		{
		case DataItem::_BOOL:
			{
				bool value = *data->value._bool;
				model_attributeInspector->setData(item_value->index(), QVariant(value));

				// Fetch new value from menu
				QVariant test = model_attributeInspector->data(item_value->index());
				QVariant::Type test2 = test.type();
				test2 = test.type();
			}
			break;
		case DataItem::_INT:
			{
				int value = *data->value._int;
				model_attributeInspector->setData(item_value->index(), QVariant(value));
			}
			break;
		case DataItem::_FLOAT:
			{
				double value = (double)*data->value._float;
				model_attributeInspector->setData(item_value->index(), QVariant(value));
			}
			break;
		case DataItem::_FLOAT2:
			{
				Float2 value = *data->value._float2;
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
			break;
		case DataItem::_FLOAT3:
			{
				Float3 value = *data->value._float3;
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
			break;
		case DataItem::_FLOAT4:
			{
				Float4 value = *data->value._float4;
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
			break;
		case DataItem::_FLOAT4X4:
			{
				Float4x4 value = *data->value._float4x4;
				{
					item_property->setChild(0, 0, new QStandardItem("MATRIX"));
				}
			}
			break;
		case DataItem::_STRING:
			{
				std::string value = *data->value._string;
				model_attributeInspector->setData(item_value->index(), QVariant(value.c_str()));
			}
			break;
		case DataItem::_ATTRIBUTE_POINTER:
			{
				int value = *data->value._int;
				model_attributeInspector->setData(item_value->index(), QVariant(value));
			}
			break;
		case DataItem::_INVALID:
			{
				model_attributeInspector->setData(item_value->index(), QVariant(""));
			}
			break;
		default:
			{

			}
			break;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	// Clean up
	//

	// Remove unused rows
	int excessRows = model_attributeInspector->rowCount() - num_items;
	if(excessRows>0)
		model_attributeInspector->removeRows(num_items, excessRows);
	ui.treeView_attributeInspector->expandAll();

	// Enable itemChanged to trigger again
	// see above
	selected_attribute.ignoreRefresh = false;
}

void Menu_Editor::slot_attributeInspector_itemChanged()
{
	// Only activate if update is caused by "player"
	// not by update is caused by code
	if(!selected_attribute.ignoreRefresh)
	{
		// abort if data do not exist
		DataItemList* list = selected_attribute.data;
		if(!list)
			return;
		// abort if Entity do not exists
		if(selected_attribute.entityId == -1)
			return;
		Entity* entity = itr_entity->at(selected_attribute.entityId);
		// abort if attribute does not exist
		IAttribute* attribute = entity->getAttributeInterface(selected_attribute.index);
		if(!attribute)
			return;

		// parse menu and save changes into our DataList
		list->reset();
		int num_items = 0;
		while(list->hasNext())
		{
			// Fetch data
			DataItem* data = list->getNext();

			// Determine type
			DataItem::DataType type = data->type;
			switch(type) 
			{
			case DataItem::_BOOL:
				{
					// Access value
					bool* value = data->value._bool;
					
					// Fetch new value from menu
					*value = model_attributeInspector->data(model_attributeInspector->item(num_items, 1)->index()).toBool();
				}
				break;
			case DataItem::_INT:
				{
					// Access value
					int* value = data->value._int;

					// Fetch new value from menu
					*value = model_attributeInspector->data(model_attributeInspector->item(num_items, 1)->index()).toInt();
				}
				break;
			case DataItem::_FLOAT:
				{
					// Access value
					float* value = data->value._float;

					// Fetch new value from menu
					*value = model_attributeInspector->data(model_attributeInspector->item(num_items, 1)->index()).toFloat();
				}
				break;
			case DataItem::_FLOAT2:
				{
					// Access value
					Float2* value = data->value._float2;

					// Fetch new value from menu
					QStandardItem* item_property = model_attributeInspector->item(num_items, 0);

					value->x = (float)model_attributeInspector->data(item_property->child(0, 1)->index()).toDouble();
					value->y = (float)model_attributeInspector->data(item_property->child(1, 1)->index()).toDouble();
				}
				break;
			case DataItem::_FLOAT3:
				{
					// Access value
					Float3* value = data->value._float3;

					// Fetch new value from menu
					QStandardItem* item_property = model_attributeInspector->item(num_items, 0);
					
					value->x = (float)model_attributeInspector->data(item_property->child(0, 1)->index()).toDouble();
					value->y = (float)model_attributeInspector->data(item_property->child(1, 1)->index()).toDouble();
					value->z = (float)model_attributeInspector->data(item_property->child(2, 1)->index()).toDouble();
				}
				break;
			case DataItem::_FLOAT4:
				{
					// Access value
					Float4* value = data->value._float4;

					// Fetch new value from menu
					QStandardItem* item_property = model_attributeInspector->item(num_items, 0);

					value->x = (float)model_attributeInspector->data(item_property->child(0, 1)->index()).toDouble();
					value->y = (float)model_attributeInspector->data(item_property->child(1, 1)->index()).toDouble();
					value->z = (float)model_attributeInspector->data(item_property->child(2, 1)->index()).toDouble();
					value->w = (float)model_attributeInspector->data(item_property->child(3, 1)->index()).toDouble();
				}
				break;
			case DataItem::_STRING:
				{
					// Access value
					std::string* value = data->value._string;

					// Fetch new value from menu
					*value = model_attributeInspector->data(model_attributeInspector->item(num_items, 1)->index()).toString().toStdString();
				}
				break;
			case DataItem::_ATTRIBUTE_POINTER:
				{
					// Access value
					int* value = data->value._int;

					// Fetch new value from menu
					*value = model_attributeInspector->data(model_attributeInspector->item(num_items, 1)->index()).toInt();
				}
				break;
			}

			// Switch to next row
			num_items++;
		}

		// Save to attribute
		attribute->saveTo(list);

		// Refresh all attributes connected to entity
		// this is to ensure attributes with pointers 
		// (such as physics) gets updated as well as 
		// the attribute they are pointing to

		std::vector<AttributeController>* attributes = entity->getAttributeControllers();
		for(int i=0; i<attributes->size(); i++)
		{
			int attributeEnum = attributes->at(i).getAttribute()->getType();
			int index = attributes->at(i).getIndex();

			// Send event
			EventManager::getInstance()->sendEvent(&Event_AttributeUpdated(index, attributeEnum));
		}
	}
}

void Menu_Editor::slot_showDebugPhysics()
{
	settings->showDebugPhysics = ui.checkBox_showDebugPhysics->isChecked();
}


