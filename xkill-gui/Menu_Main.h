#pragma once

#include <QMainWindow>
#include <QKeyEvent>
#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>
#include "ToggleHelper.h"
#include "ui_MainMenu2.h" 
#include <QtXml>
#include <QStandardItemModel>
#include <QMessageBox>

class Menu_Main : public QMainWindow, public ToggleHelper
{
	Q_OBJECT

private:
	Ui::MainMenu ui;

public:
	Menu_Main(QWidget* parent) : QMainWindow(parent), ToggleHelper(this)
	{
		ui.setupUi(this);
		QWidget::setAttribute(Qt::WA_TranslucentBackground, true);
		QWidget::setWindowFlags(Qt::SplashScreen);
		//setAttribute(Qt::WA_TransparentForMouseEvents);
		//setAttribute(Qt::WA_TranslucentBackground);
		setWindowFlags(Qt::WindowStaysOnBottomHint);
		
		connect(ui.pushButton_Exit, SIGNAL(clicked()), parentWidget(), SLOT(close()));
		connect(ui.pushButton_Exit_2, SIGNAL(clicked()), parentWidget(), SLOT(close()));
		connect(ui.comboBox_LevelSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_selectLevel(int)));
		connect(ui.textBrowser_LevelInfo, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_selectLevel(int)));
		connect(ui.pushButton_AddLevel, SIGNAL(clicked()), this, SLOT(slot_addLevel()));
		connect(ui.pushButton_SaveLevel, SIGNAL(clicked()), this, SLOT(slot_saveLevel()));
		connect(ui.pushButton_RemoveLevel, SIGNAL(clicked()), this, SLOT(slot_removeLevel()));
		connect(ui.pushButton_editorRefresh, SIGNAL(clicked()), this, SLOT(slot_EditorRefresh()));
		
		filePath = QString("../../xkill-resources/xkill-scripts/levels.xml");
		model = new QStandardItemModel(0, 1, this);
		editorModel = new QStandardItemModel(0, 2, this);

		ui.treeView_EntityBrowser->setModel(editorModel);

		loadXML();
	}

	void parentMoveEvent()
	{
		QPoint pos = parentWidget()->pos();
		pos = QPoint();
		int x = pos.x() + parentWidget()->width()/2 - this->width()/2;
		int y = pos.y() + parentWidget()->height()/2 - this->height()/2;
		move(x, y);
	}

	QStandardItemModel* editorModel;

	

protected:
	QStandardItemModel* model;
	QString filePath;


	void loadXML()
	{
		model->clear();

		// load XML file
		QDomDocument document;
		QFile* file = new QFile(filePath);
		if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::information(0, "Error", "Failed to find XML file"); return;
		}
		if(!document.setContent(file))
		{
			QMessageBox::information(0, "Error", "Failed to parse XML file to open XML file");  return;
		}
		file->close();
		delete file;

		// read all levels to ItemModel
		QStandardItem* root = new QStandardItem("Levels");
		model->appendRow(root);
		QDomElement xmlRoot = document.firstChildElement();
		QDomNodeList allLevel = xmlRoot.elementsByTagName("Level");
		for(int i=0; i<allLevel.count(); i++)
		{
			// parse level
			QDomElement level = allLevel.at(i).toElement();
			QStandardItem* name = new QStandardItem(level.attribute("Name"));
			QStandardItem* description = new QStandardItem(level.attribute("Description"));
			root->appendRow(name);
			name->appendRow(description);
		}

		// parse model and build stuff from it
		ui.comboBox_LevelSelect->clear();
		QStandardItem* allLevelItm = model->item(0,0);
		for(int i=0; i<allLevelItm->rowCount(); i++)
		{
			QStandardItem* child = allLevelItm->child(i,0);
			ui.comboBox_LevelSelect->addItem(child->text());
		}
		ui.treeView->setModel(model);
	}

public slots:
	void slot_selectLevel(int levelId)
	{
		QStandardItem* levels = model->item(0,0);
		if(levels->rowCount()>levelId && levelId>=0)
		{
			QStandardItem* name = levels->child(levelId,0);
			QStandardItem* desc = name->child(0,0);
			ui.textBrowser_LevelInfo->setText(desc->text());
		}
	}

	void slot_EditorRefresh()
	{
		// Iterate list and update score
		std::vector<PlayerAttribute>* allPlayers;		GET_ATTRIBUTES(allPlayers, PlayerAttribute, ATTRIBUTE_PLAYER);
		std::vector<HealthAttribute>* allHealth;		GET_ATTRIBUTES(allHealth, HealthAttribute, ATTRIBUTE_HEALTH);
		std::vector<RenderAttribute>* allRender;		GET_ATTRIBUTES(allRender, RenderAttribute, ATTRIBUTE_RENDER);
		std::vector<SpatialAttribute>* allSpatial;		GET_ATTRIBUTES(allSpatial, SpatialAttribute, ATTRIBUTE_SPATIAL);
		std::vector<PositionAttribute>* allPositions;	GET_ATTRIBUTES(allPositions, PositionAttribute, ATTRIBUTE_POSITION);

		// Update tree widget
		ui.treeWidget->clear();
		//editorModel->clear();
		std::vector<int>* allPlayerOwner;				GET_ATTRIBUTE_OWNERS(allPlayerOwner, ATTRIBUTE_PLAYER);
		QStandardItem* playersItm = new QStandardItem("Players");
		editorModel->appendRow(playersItm);


		editorModel->setItem(0,1, new QStandardItem("Row"));

		for(unsigned i=0; i<allPlayerOwner->size(); i++)
		{
			if(allPlayerOwner->at(i)!=0)
			{
				// Extract attributes from a playerAttribute
				PlayerAttribute* player			=	&allPlayers->at(i);
				HealthAttribute* health			=	&allHealth->at(player->healthAttribute.index);
				RenderAttribute* render			=	&allRender->at(player->renderAttribute.index);			
				SpatialAttribute* spatial		=	&allSpatial->at(render->spatialAttribute.index);
				PositionAttribute* position		=	&allPositions->at(spatial->positionAttribute.index);

				// Player
				QStandardItem* playerItm = new QStandardItem(QString::number(i));
				playersItm->appendRow(playerItm);

				// Position
				int pos_x = position->position.x;
				int pos_y = position->position.y;
				int pos_z = position->position.z;
				QString str_position = QString::number(pos_x) + ", "  + QString::number(pos_y) + ", "  + QString::number(pos_z);
				
				// Render
				QStandardItem* renderItm = new QStandardItem("Render");
				playerItm->appendRow(renderItm);

				// MeshID
				QStandardItem* meshIdItm = new QStandardItem("MeshID");
				playerItm->appendRow(meshIdItm);

				meshIdItm->appendRow(new QStandardItem(QString::number(render->meshID)));

				// Tex
				QStandardItem* textureIdItm = new QStandardItem("TextureID");
				playerItm->appendRow(textureIdItm);

				QList<QStandardItem*> list;
				list.append(new QStandardItem(QString::number(render->textureID)));
				textureIdItm->appendColumn(list);

				playerItm->appendRow( new QStandardItem("Position"));
				playerItm->appendRow( new QStandardItem(str_position));
				playerItm->appendRow( new QStandardItem("Health"));
			}
		}
	}

	void slot_addLevel()
	{
		QStandardItem* desc = new QStandardItem("Description");
		QStandardItem* name = new QStandardItem("Name");
		name->appendRow(desc);
		QStandardItem* levels = model->item(0,0);
		levels->appendRow(name);
	}

	void slot_removeLevel()
	{
		QStandardItem* levels = model->item(0,0);
		QModelIndex index = ui.treeView->currentIndex();
		levels->removeRow(index.row());
	}

	void slot_saveLevel()
	{
		// make root node
		QDomDocument document;
		QDomElement xmlroot = document.createElement("xkill");
		document.appendChild(xmlroot);

		// parse model into level xml
		QStandardItem* levels = model->item(0,0);
		for(int i=0; i<levels->rowCount(); i++)
		{
			QDomElement xmlbook = document.createElement("Level");
			QStandardItem* name = levels->child(i,0);
			QStandardItem* desc = name->child(0,0);
			xmlbook.setAttribute("Name",name->text());
			xmlbook.setAttribute("Description", desc->text());
			xmlroot.appendChild(xmlbook);
		}

		// save to disk
		QFile file(filePath);
		if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QMessageBox::information(0, "Error", "Failed to write file");  return;
		}
		QTextStream stream(&file);
		stream << document.toString();
		file.close();

		loadXML();
	}

protected:
	void keyPressEvent(QKeyEvent* e)
	{
		QCoreApplication::sendEvent(parentWidget(), e);
	}
	void mousePressEvent(QMouseEvent* e)
	{
		e->accept(); // avoids propagating event to parent
	}
};