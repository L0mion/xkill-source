#pragma once

#include "Menu_ScoreBoard.h"

ATTRIBUTES_DECLARE_ALL


Menu_ScoreBoard::Menu_ScoreBoard(QWidget* parent, Qt::WFlags flags) : QMainWindow(parent), ToggleHelper(this)
{
	ATTRIBUTES_INIT_ALL

	ui.setupUi(this);
	QWidget::setAttribute(Qt::WA_TranslucentBackground, true);
	QWidget::setWindowFlags(Qt::SplashScreen);
	QWidget::setAttribute(Qt::WA_ShowWithoutActivating);
	setWindowFlags(Qt::WindowStaysOnBottomHint);
	QWidget::setStyleSheet("QWidget{color: #fff; background-color: #000;}");
	QWidget::setEnabled(false);
}


// Not working! What's wrong with Qt?
void Menu_ScoreBoard::mapListToAttributes(int numAttributes)
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


void Menu_ScoreBoard::onUpdate(float delta)
{
	// Iterate list and update score
	if(showMenu)
	{
		// Update tree widget
		ui.treeWidget->clear();
		while(itrPlayer.hasNext())
		{
			// Extract attributes from a playerAttribute
			Attribute_Player* player		=	itrPlayer.getNext();
			Attribute_Health* health		=	itrHealth.at(player->ptr_health.index);
			Attribute_Render* render		=	itrRender.at(player->ptr_render.index);			
			Attribute_Spatial* spatial		=	itrSpatial.at(render->ptr_spatial.index);
			Attribute_Position* position	=	itrPosition.at(spatial->ptr_position.index);

			// Add row
			QTreeWidgetItem* itm = new QTreeWidgetItem();
			itm->setText(0, "Process "+QString::number(itrPlayer.index()));
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

		// Sort by score
		ui.treeWidget->sortItems(2, Qt::DescendingOrder);
	}
}

void Menu_ScoreBoard::parentMoveEvent()
{
	QPoint pos = parentWidget()->pos();
	pos = QPoint();
	int x = pos.x() + parentWidget()->width()/2 - this->width()/2;
	int y = pos.y() + parentWidget()->height()/2 - this->height()/2;
	move(x, y);
}


void Menu_ScoreBoard::keyPressEvent(QKeyEvent* e)
{
	QCoreApplication::sendEvent(parentWidget(), e);
}
