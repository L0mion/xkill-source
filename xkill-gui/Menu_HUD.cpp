#include "Menu_HUD.h"

ATTRIBUTES_DECLARE_ALL;

Menu_HUDManager::Menu_HUDManager( QWidget* parent ) : QObject(parent)
{
	ATTRIBUTES_INIT_ALL;

	this->parent = parent;
	isEnabled = false;

	// Events
	SUBSCRIBE_TO_EVENT(this, EVENT_UPDATE);
	SUBSCRIBE_TO_EVENT(this, EVENT_ENABLE_HUD);
	SUBSCRIBE_TO_EVENT(this, EVENT_SPLITSCREEN_CHANGED);
}

void Menu_HUDManager::onEvent( Event* e )
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_ENABLE_HUD:
		
		if(((Event_EnableHud*)e)->enableHud)
		{
			isEnabled = true;

			for(int i=0; i<huds.size(); i++)
				huds[i]->show();
		}
		else
		{
			isEnabled = false;

			for(int i=0; i<huds.size(); i++)
				huds[i]->hide();
		}
	
		break;
	case EVENT_SPLITSCREEN_CHANGED:
		mapHudsToSplitscreen();
		break;
	default:
		break;
	}
}

void Menu_HUDManager::mapHudsToSplitscreen()
{
	// Delete previous huds
	for(int i=0; i<huds.size(); i++)
	{
		delete huds[i];
	}
	huds.clear();

	// Map new HUDs to split screens
	while(itrSplitScreen.hasNext())
	{
		AttributePtr<Attribute_SplitScreen> ptr_splitScreen = itrSplitScreen.getNext();
		huds.push_back(new Menu_HUD(ptr_splitScreen, parent));
		if(isEnabled)
			huds.back()->show();
	}
}

void Menu_HUDManager::updateHuds()
{
	// Update huds
	int index = 0;
	while(itrSplitScreen.hasNext())
	{
		AttributePtr<Attribute_SplitScreen> ptr_splitScreen = itrSplitScreen.getNext();
		//huds[index]->refresh(ptr_splitScreen);
		index++;
	}
}

void Menu_HUD::refresh()
{
	{
		//ui.frame_center->move(0, 0);

		// Center of split-screen
		Float2 center;
		center.x = splitScreen->ssTopLeftX + splitScreen->ssWidth/2 ;
		center.y = splitScreen->ssTopLeftY + splitScreen->ssHeight/2;
		
		Float2 split;
		split.x = splitScreen->ssWidth ;
		split.y = splitScreen->ssHeight;

		QPoint p;

		QPoint p1;
		{
			QWidget* w_target = this;
			QWidget* w = ui.label_aim;
			p = ui.label_aim->pos();
			do 
			{
				p = w->mapTo(w->parentWidget(), p);
				w = w->parentWidget();

			} while(w != w_target);
		}




		//// Center of cross-hair relative to frame
		//QPoint g_aimPos = ui.label_aim->mapToGlobal(QPoint(0,0));
		//QPoint g_parent = ui.frame_center->mapToGlobal(QPoint(0,0));
		//QPoint q_aimPos = g_aimPos - g_parent;
		//Float2 aimPos;
		//aimPos.x =  q_aimPos.x();
		//aimPos.y =  q_aimPos.y();



		//
		// Translate cross-hair to center
		//

		/*Float2 d;
		d =  center - aimPos;

		Float2 framePos;
		framePos.x = ui.frame_center->pos().x();
		framePos.y = ui.frame_center->pos().y();

		Float2 newPos = framePos + d;
		*/

		ui.frame_center->move(p.x(), p.y());
	}
}

Menu_HUD::Menu_HUD( AttributePtr<Attribute_SplitScreen> splitScreen, QWidget* parent ) : QWidget(parent)
{
	ui.setupUi(this);
	this->splitScreen = splitScreen;
	resize(splitScreen->ssWidth, splitScreen->ssHeight);
	hide();

	QWidget::setAttribute(Qt::WA_ShowWithoutActivating);
	QWidget::setAttribute(Qt::WA_TransparentForMouseEvents);
	
	refresh();
}
