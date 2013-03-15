#pragma once

#include <xkill-utilities/Util.h>
#include <xkill-utilities/Converter.h>
#include <QtGui/QLabel>

#include "ui_Menu_HUD.h"

class HudPowerup
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

class HudPowerupDispayer
{
private:
	QWidget* parent;
	AttributePtr<Attribute_Player> ptr_player;

public:

	void init(Ui::Menu_HUD* ui, AttributePtr<Attribute_Player> ptr_player)
	{
		//ui->
	}

	void refresh()
	{

	}
};