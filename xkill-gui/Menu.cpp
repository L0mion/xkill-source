#include "Menu.h"

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>

void Menu::onUpdate( float delta )
{
	if(show)
	{
		std::vector<PlayerAttribute>* allPlayers;		GET_ATTRIBUTES(allPlayers, PlayerAttribute, ATTRIBUTE_PLAYER);
		std::vector<HealthAttribute>* allHealth;		GET_ATTRIBUTES(allHealth, HealthAttribute, ATTRIBUTE_HEALTH);
		std::vector<RenderAttribute>* allRender;		GET_ATTRIBUTES(allRender, RenderAttribute, ATTRIBUTE_RENDER);
		std::vector<SpatialAttribute>* allSpatial;		GET_ATTRIBUTES(allSpatial, SpatialAttribute, ATTRIBUTE_SPATIAL);
		std::vector<PositionAttribute>* positionAttributes_;	GET_ATTRIBUTES(positionAttributes_, PositionAttribute, ATTRIBUTE_POSITION);

		// test
		std::vector<int>* playerAttributesOwners;		GET_ATTRIBUTE_OWNERS(playerAttributesOwners, ATTRIBUTE_PLAYER);
		model->clear();
		for(unsigned i=0; i<allPlayers->size(); i++)
		{
			if(playerAttributesOwners->at(i)!=0)
			{
				// Extract attributes from a playerAttribute
				PlayerAttribute* player		=	&allPlayers->at(i);

				HealthAttribute* health		=	&allHealth->at(player->healthAttribute.index);
				//RenderAttribute* render		=	&allRender->at(player->renderAttribute.index);			
				//SpatialAttribute* spatial	=	&allSpatial->at(render->spatialAttribute.index);
				//PositionAttribute* position	=	&positionAttributes_->at(spatial->positionAttribute.index);

				QList<QStandardItem*> items;
				QString s;

				std::string testar = "Unkown";
				items.push_back(new QStandardItem(testar.c_str()));

				s = QString();
				s.append(QString("%1").arg(health->health));
				items.push_back(new QStandardItem(s));

				s = QString();
				s.append(QString("%1").arg(player->priority));
				items.push_back(new QStandardItem(s));

				s = QString();
				s.append(QString("%1").arg(player->totalExecutionTime));
				items.push_back(new QStandardItem(s));

				s = QString();
				s.append(QString("%1").arg(player->cycleSteals));
				items.push_back(new QStandardItem(s));

				// append
				model->appendRow(items);
			}
		}
		model->setHeaderData(0, Qt::Horizontal, QObject::tr("Process"));
		model->setHeaderData(1, Qt::Horizontal, QObject::tr("Health"));
		model->setHeaderData(2, Qt::Horizontal, QObject::tr("Priority"));
		model->setHeaderData(3, Qt::Horizontal, QObject::tr("Cycles"));
		model->setHeaderData(4, Qt::Horizontal, QObject::tr("Steals"));
		view->setFocusPolicy(Qt::NoFocus);
	}
}

void Menu::onEvent( Event* e )
{
	EventType type = e->getType();
	static int test = 20;
	switch (type) 
	{
	case EVENT_UPDATE:
		test--;
		if(test<0)
		{
			onUpdate(1.0f);
			test = 20;
		}
		break;
	default:
		break;
	}
}

Menu::Menu( QWidget* parent /*= 0*/, Qt::WFlags flags /*= 0*/ ) : QDialog(parent, flags)
{
	QWidget::setAttribute(Qt::WA_ShowWithoutActivating);
	QWidget::setWindowFlags(Qt::ToolTip);
	//QWidget::raise();
	resize(600, 200);
	QVBoxLayout* layout = new QVBoxLayout(this);

	// Create Table
	model = new QStandardItemModel(this);
	view = new QTableView;
	view->setFocusPolicy(Qt::NoFocus);
	view->setModel(model);
	layout->addWidget(view);

	// Create buttons
	for (int i=0; i<0; i++)
	{
		QPushButton* buttons = new QPushButton(tr("Button %1").arg(i + 1));
		buttons->setFocusPolicy(Qt::NoFocus);
		layout->addWidget(buttons);
	}

	// Show menu
	show = true;
	toggleMenu();

	SUBSCRIBE_TO_EVENT(this, EVENT_UPDATE);
}

void Menu::keyPressEvent( QKeyEvent* e )
{
	QCoreApplication::sendEvent(parentWidget(), e);
}
