#include "Menu_Main.h"
#include <QtGui/QKeyEvent>
#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>


#include <QtXml/QtXml>
#include <QtGui/QStandardItemModel>
#include <QtGui/QMessageBox>

#include "Menu_Editor.h"

Menu_Main::Menu_Main( QWidget* parent ) : QMainWindow(parent), ToggleHelper(this)
{
	ui.setupUi(this);
	QWidget::setAttribute(Qt::WA_TranslucentBackground, true);
	QWidget::setWindowFlags(Qt::SplashScreen);
	//setAttribute(Qt::WA_TransparentForMouseEvents);
	//setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::WindowStaysOnBottomHint);

	connect(ui.pushButton_exit, SIGNAL(clicked()), parentWidget(), SLOT(close()));
	connect(ui.pushButton_exit_2, SIGNAL(clicked()), parentWidget(), SLOT(close()));
	connect(ui.comboBox_LevelSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_selectLevel(int)));
	connect(ui.pushButton_AddLevel, SIGNAL(clicked()), this, SLOT(slot_addLevel()));
	connect(ui.pushButton_SaveLevel, SIGNAL(clicked()), this, SLOT(slot_saveLevel()));
	connect(ui.pushButton_RemoveLevel, SIGNAL(clicked()), this, SLOT(slot_removeLevel()));
	connect(ui.pushButton_startGame, SIGNAL(clicked()), this, SLOT(slot_startGame()));

	filePath = QString("../../xkill-resources/xkill-scripts/levels.xml");
	model = new QStandardItemModel(0, 1, this);
	
	//editorModel->setHorizontalHeaderItem(1, new QStandardItem("ID"));

	QStringList columnNames;

	loadXML();
}

void Menu_Main::parentMoveEvent()
{
	QPoint pos = parentWidget()->pos();
	pos = QPoint();
	int x = pos.x() + parentWidget()->width()/2 - this->width()/2;
	int y = pos.y() + parentWidget()->height()/2 - this->height()/2;
	move(x, y);
}

void Menu_Main::loadXML()
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
	model->item(0);
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

	ui.treeView->setExpanded(allLevelItm->index(), true);
	
}

void Menu_Main::slot_selectLevel( int levelId )
{
	QStandardItem* levels = model->item(0,0);
	if(levels->rowCount()>levelId && levelId>=0)
	{
		QStandardItem* name = levels->child(levelId,0);
		QStandardItem* desc = name->child(0,0);
		ui.textBrowser_LevelInfo->setText(desc->text());
	}
}

void Menu_Main::slot_addLevel()
{
	QStandardItem* desc = new QStandardItem("Description");
	QStandardItem* name = new QStandardItem("Name");
	name->appendRow(desc);
	QStandardItem* levels = model->item(0,0);
	levels->appendRow(name);
}

void Menu_Main::slot_removeLevel()
{
	QStandardItem* levels = model->item(0,0);
	QModelIndex index = ui.treeView->currentIndex();
	levels->removeRow(index.row());
}

void Menu_Main::slot_saveLevel()
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

void Menu_Main::keyPressEvent( QKeyEvent* e )
{
	QCoreApplication::sendEvent(parentWidget(), e);
}

void Menu_Main::mousePressEvent( QMouseEvent* e )
{
	e->accept(); // avoids propagating event to parent
}

void Menu_Main::slot_startGame()
{
	int num_players = ui.horizontalSlider_numPlayers->value();
	SEND_EVENT(&Event_StartDeathmatch(num_players));

	ToggleHelper::toggleMenu(false);
}


