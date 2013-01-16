#pragma once

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/EventType.h>
#include <xkill-utilities/AttributeType.h>
#include <QtGui/QDialog>
#include <QtGui/QKeyEvent>
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
		setWindowFlags(Qt::WindowStaysOnBottomHint);
		QWidget::setStyleSheet("QWidget{color: #fff; background-color: #000;}");
		QWidget::setEnabled(false);
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
		// Iterate list and update score
		if(showMenu)
		{
			std::vector<Attribute_Player>* allPlayers		=	GET_ATTRIBUTES(player);
			std::vector<Attribute_Health>* allHealth		=	GET_ATTRIBUTES(health);
			std::vector<Attribute_Render>* allRender		=	GET_ATTRIBUTES(render);
			std::vector<Attribute_Spatial>* allSpatial		=	GET_ATTRIBUTES(spatial);
			std::vector<Attribute_Position>* allPositions	=	GET_ATTRIBUTES(position);

			// Update tree widget
			ui.treeWidget->clear();
			std::vector<int>* allPlayerOwner				= GET_ATTRIBUTE_OWNERS(player);
			for(unsigned i=0; i<allPlayerOwner->size(); i++)
			{
				if(allPlayerOwner->at(i)!=0)
				{
					// Extract attributes from a playerAttribute
					Attribute_Player* player			=	&allPlayers->at(i);
					Attribute_Health* health			=	&allHealth->at(player->ptr_health.index);
					Attribute_Render* render			=	&allRender->at(player->ptr_render.index);			
					Attribute_Spatial* spatial		=	&allSpatial->at(render->ptr_spatial.index);
					Attribute_Position* position		=	&allPositions->at(spatial->ptr_position.index);

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
		pos = QPoint();
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