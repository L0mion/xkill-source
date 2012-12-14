#pragma once

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/EventType.h>
#include <QDialog>
#include <QKeyEvent>
#include "ToggleHelper.h"
#include "ui_ScoreMenu.h" 

class Menu_ScoreBoard : public QMainWindow, public ToggleHelper
{
private:
	Ui::ScoreMenu ui;

public:
	Menu_ScoreBoard(QWidget* parent = 0, Qt::WFlags flags = 0) : QMainWindow(parent), ToggleHelper(this)
	{
		ui.setupUi(this);
		QWidget::setAttribute(Qt::WA_TranslucentBackground, true);
		QWidget::setWindowFlags(Qt::SplashScreen);
		QWidget::setAttribute(Qt::WA_ShowWithoutActivating);
		QWidget::setStyleSheet("QWidget{color: #fff; background-color: #000;}");
	}

	// Not working! What's wrong with Qt?
	void mapListToAttributes(int numAttributes)
	{
		//// Remove excess ListItems
		//QTreeWidgetItemIterator it(ui.treeWidget);
		//int numListItems = 0;
		//while(*it) 
		//{	
		//	QString text = (*it)->text(0);
		//	if(numListItems>numAttributes)
		//	{
		//		delete (*it);
		//	}
		//	numListItems++;
		//}

		////	Add ListItems if needed
		//int difference = numAttributes - numListItems;
		//while(difference > 0)
		//{
		//	QTreeWidgetItem* itm = new QTreeWidgetItem();
		//	itm->setText(0, "Test");

		//	ui.treeWidget->addTopLevelItem(itm);
		//	difference--;
		//}
	}

	void onUpdate(float delta)
	{
		// Iternate list and update score
		if(showMenu)
		{
			std::vector<PlayerAttribute>* allPlayers;		GET_ATTRIBUTES(allPlayers, PlayerAttribute, ATTRIBUTE_PLAYER);
			std::vector<HealthAttribute>* allHealth;		GET_ATTRIBUTES(allHealth, HealthAttribute, ATTRIBUTE_HEALTH);
			std::vector<RenderAttribute>* allRender;		GET_ATTRIBUTES(allRender, RenderAttribute, ATTRIBUTE_RENDER);
			std::vector<SpatialAttribute>* allSpatial;		GET_ATTRIBUTES(allSpatial, SpatialAttribute, ATTRIBUTE_SPATIAL);
			std::vector<PositionAttribute>* allPositions;	GET_ATTRIBUTES(allPositions, PositionAttribute, ATTRIBUTE_POSITION);

			// Update tree widget
			ui.treeWidget->clear();
			std::vector<int>* allPlayerOwner;				GET_ATTRIBUTE_OWNERS(allPlayerOwner, ATTRIBUTE_PLAYER);
			for(unsigned i=0; i<allPlayerOwner->size(); i++)
			{
				if(allPlayerOwner->at(i)!=0)
				{
					// Extract attributes from a playerAttribute
					PlayerAttribute* player			=	&allPlayers->at(i);
					HealthAttribute* health			=	&allHealth->at(player->healthAttribute.index);
					RenderAttribute* render		=	&allRender->at(player->renderAttribute.index);			
					SpatialAttribute* spatial		=	&allSpatial->at(render->spatialAttribute.index);
					PositionAttribute* position	=	&allPositions->at(spatial->positionAttribute.index);

					// Add row
					QTreeWidgetItem* itm = new QTreeWidgetItem();
					itm->setText(0, "Process "+QString::number(i));
					itm->setText(1, QString::number((int)player->totalExecutionTime));
					itm->setText(2, QString::number(player->priority));
					itm->setText(3, QString::number(player->cycleSteals));
					itm->setText(4, QString::number(health->health));
					int pos_x = position->position.x;
					int pos_y = position->position.y;
					int pos_z = position->position.z;
					QString str_position = QString::number(pos_x) + ", "  + QString::number(pos_y) + ", "  + QString::number(pos_z);
					itm->setText(5, str_position);
					ui.treeWidget->addTopLevelItem(itm);
				}
			}

			// Sort by score
			ui.treeWidget->sortItems(2, Qt::DescendingOrder);
		}
		
	}

	void parentMoveEvent()
	{
		QPoint pos = parentWidget()->pos();
		int x = pos.x() + parentWidget()->width()/2 - this->width()/2;
		int y = pos.y() + parentWidget()->height()/2 - this->height()/2;
		move(x, y);
	}

protected:
	void keyPressEvent(QKeyEvent* e)
	{
		QCoreApplication::sendEvent(parentWidget(), e);
	}
};