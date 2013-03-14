#pragma once

#include <xkill-utilities/Util.h>
#include <xkill-utilities/Converter.h>
#include <QtGui/QLabel>

#include "ui_Menu_HUD.h"

class HudScheduling_Item
{
public:
	QLabel* label;
	AttributePtr<Attribute_Player> ptr_player;
	int priority;
	Float2 position;

	void update();
};

class HudScheduling
{
private:
	QWidget* parent;
	QWidget* window;
	QWidget* subWindow;
	QProgressBar* progressbar;
	QLabel* advantageLabel;
	AttributePtr<Attribute_Player> ptr_player;

	std::vector<HudScheduling_Item> items;
	bool isScheduling;
public:
	HudScheduling()
	{
		isScheduling = false;
	}

	void init(Ui::Menu_HUD* ui, AttributePtr<Attribute_Player> ptr_player);

	void hide();
	int findHighestPriority()
	{
		int highest = 0;
		for(int i=0; i<items.size(); i++)
		{
			if(items[i].priority > highest)
				highest = items[i].priority;
		}

		return highest;
	}
	void refresh();
};