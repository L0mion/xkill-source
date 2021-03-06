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
	Float2 position;
	Float2 targetPosition;


	void setPosition(Float2 position)
	{
		this->position = position;
		label->move(position.x + 0.5f, position.y + 0.5f);
	}

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
	int ownerIndex;

	std::vector<HudScheduling_Item> items;
	float standardMargin;
	float itemWidth;
	float itemHeight;
	bool isScheduling;
public:
	HudScheduling()
	{
		isScheduling = false;
	}

	void init(Ui::Menu_HUD* ui, AttributePtr<Attribute_Player> ptr_player);

	void hide();
	void show();

	int findHighestPriority();
	int findHighestCycles()
	{
		int highest = 0;
		for(int i=0; i<items.size(); i++)
		{
			if(items[i].ptr_player->cycles > highest)
				highest = items[i].ptr_player->cycles;
		}

		return highest;
	}
	int findLowestCycles()
	{
		int min = INT_MAX;
		for(int i=0; i<items.size(); i++)
		{
			if(items[i].ptr_player->cycles < min)
				min = items[i].ptr_player->cycles;
		}

		return min;
	}
	
	struct CountAndIndex
	{
		int count;
		int index;
	};

	/**
	Finds the count of items with same priority
	and our relative index among them
	*/
	CountAndIndex findRelativeIndex(int priority, int ourIndex)
	{
		int count = 0;
		int relativeIndex = 0;

		for(int i=0; i<items.size(); i++)
		{
			if(items[i].ptr_player->priority == priority)
			{
				count++;

				if(i < ourIndex)
					relativeIndex++;
			}
		}

		CountAndIndex value;
		value.count = count;
		value.index = relativeIndex;

		return value;
	}
	void refresh();
};