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
	//QWidget::setStyleSheet("QWidget{color: #fff; background-color: #000;}");
	QWidget::setEnabled(false);

	ui.horizontalLayout->setMargin(0);
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
			AttributePtr<Attribute_Player> player		=	itrPlayer.getNext();
			AttributePtr<Attribute_Health> health		=	player->ptr_health;
			AttributePtr<Attribute_Render> render		=	player->ptr_render;			
			AttributePtr<Attribute_Spatial> spatial		=	render->ptr_spatial;
			AttributePtr<Attribute_Position> position	=	spatial->ptr_position;

			// Add row
			QTreeWidgetItem* itm = new QTreeWidgetItem();
			itm->setText(0, "Process " + QString::number(itrPlayer.storageIndex()));
			itm->setText(1, QString::number((int)player->cycles));
			itm->setText(2, QString::number(player->priority));
			itm->setText(3, QString::number(player->cycleSteals));
			itm->setText(4, QString::number(health->health));
			
			//Mark executing player as red in the scoreboard
			if(player->executing)
			{
				 itm->setForeground(0 , QBrush(Qt::red));
			}

			int pos_x = position->position.x;
			int pos_y = position->position.y;
			int pos_z = position->position.z;
			QString str_position = QString::number(pos_x) + ", "  + QString::number(pos_y) + ", "  + QString::number(pos_z);
			itm->setText(5, str_position);
			ui.treeWidget->addTopLevelItem(itm);
		}

		// Sort by score
		ui.treeWidget->sortItems(1, Qt::DescendingOrder);
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
