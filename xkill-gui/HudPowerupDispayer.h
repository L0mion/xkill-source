#pragma once

#include <xkill-utilities/Util.h>
#include <xkill-utilities/Converter.h>
#include <QtGui/QLabel>

#include "ui_Menu_HUD.h"


class HudPowerupDispayer
{
	class HudItem
	{
	private:
		HudPowerupDispayer* manager;

	public:
		QLabel* icon;
		QLabel* info;
		QProgressBar* bar;

		Float2 position_icon;
		Float2 position_info;

		bool hasInfo;
		bool _isActive;
		bool prev_almostEmpty;

		int index;

		HudItem()
		{
		}

		void init(HudPowerupDispayer* manager, std::string str_icon, std::string str_info);

		void addIcon()
		{

		}

		void addInfo()
		{

		}

		QLabel* label;
		AttributePtr<Attribute_Player> ptr_player;
		Float2 position;
		Float2 targetPosition;

		void setPosition(Float2 position)
		{
			this->position = position;
			label->move(position.x + 0.5f, position.y + 0.5f);
		}

		bool updateActive(bool isActive);

		bool isActive()
		{
			return _isActive;
		}

		void setProgress(float ratio)
		{
			int value = (int)(ratio * 100);

			if(value != bar->value())
			{
				bar->setValue(value);


				// Make bar red when close to running out
				bool almostEmpty = value < 50;
				if(almostEmpty != prev_almostEmpty)
				{
					prev_almostEmpty = almostEmpty;

					if(almostEmpty)
						bar->setStyleSheet("QProgressBar{ background-color: rgba(0, 0, 0, 0); padding: 0px;} QProgressBar::chunk{ background-color: rgba(255, 0, 0, 200);}");
					else
						bar->setStyleSheet("QProgressBar{ background-color: rgba(0, 0, 0, 0); padding: 0px;} QProgressBar::chunk{ background-color: rgba(255, 255, 255, 200);}");
				}

				// Make bar update
				bar->update();
			}
		}

		void update(int index, int infoIndex);
	};

private:
	QWidget* parent;
	Float2 position;
	AttributePtr<Attribute_Player> ptr_player;
	
	int numIndex;
	int numActriveItems;

	HudItem pow_running;
	HudItem pow_executing;
	HudItem pow_speed;
	HudItem pow_jet;
	HudItem pow_cycleSteal;
	HudItem pow_power;

	std::vector<HudItem*> items;

public:
	HudPowerupDispayer()
	{
		numIndex = 0;
	}

	QWidget* getParent()
	{
		return parent;
	}

	Float2 getPosition()
	{
		return position;
	}

	void registerItem(HudItem* item)
	{
		items.push_back(item);
	}

	int createIndex()
	{
		int index = numIndex;
		numIndex++;

		return index;
	}

	void init(QWidget* bottomFrame, AttributePtr<Attribute_Player> ptr_player);

	void update();
};