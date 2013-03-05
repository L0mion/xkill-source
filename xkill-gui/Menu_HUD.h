#pragma once

#include <QtGui/QMainWindow>
#include <xkill-utilities/Util.h>

#include <QtGui/QMainWindow>
#include <xkill-utilities/WeaponStructs.h>
#include <QtGui/QProgressBar>
#include <vector>

#include "ui_Menu_HUD.h"

class Attribute_SplitScreen;

class HudMessage
{
private:
	float _lifetime;
	QLabel* _label;
	Float2 _targetPosition;

public:
	HudMessage(Event_PostHudMessage* e, QWidget* parent);
	~HudMessage()
	{
		delete _label;
	}
	int getHeight()
	{
		return _label->height();
	}

	void setPosition(Float2 position);

	void setTargetPosition(Float2 position)
	{
		// Offset relative to center of label
		position.x -= _label->width() * 0.5f;
		position.y -= _label->height() * 0.5f;

		_targetPosition = position;
	}

	void updatePosition();
	
	void update()
	{
		updatePosition();
		
		// Decrement timer
		if(_lifetime > 0.0f)
			_lifetime -= SETTINGS->trueDeltaTime;
	}
	bool isExpired()
	{
		// Check expiration condition
		if(_lifetime <= 0.0f)
			return true;

		return false;
	}
};

class HudMessage_Manager : IObserver
{
private:
	SimpleQueue<HudMessage*> stack;
	QWidget* parent;
	Float2 position;
	AttributePtr<Attribute_SplitScreen> splitScreen;

public:
	HudMessage_Manager()
	{
		SUBSCRIBE_TO_EVENT(this, EVENT_POST_HUD_MESSAGE);
	}
	~HudMessage_Manager()
	{
		UNSUBSCRIBE_TO_EVENTS(this);

		for(int i=0; i<stack.count(); i++)
			delete stack.at(i);
	}
	void init(QWidget* parent, AttributePtr<Attribute_SplitScreen> splitScreen)
	{
		this->parent = parent;
		this->splitScreen = splitScreen;
	}

	void move(Float2 position)
	{
		this->position = position;
	}

	void update()
	{
		for(int i=0; i<stack.count(); i++)
		{
			stack.at(i)->update();
			if(stack.at(i)->isExpired())
				removeTopMessage();
		}
	}

	void addMessage(Event_PostHudMessage* e);

	void removeTopMessage()
	{
		delete stack.pop();
	}

	void onEvent(Event* e)
	{
		EventType type = e->getType();
		switch (type) 
		{
		case EVENT_POST_HUD_MESSAGE:
			addMessage((Event_PostHudMessage*)e);
			break;
		default:
			break;
		}
	}
};

class Menu_HUD : public QWidget, IObserver
{
private:
	AttributePtr<Attribute_SplitScreen> ptr_splitScreen;
	Ui::Menu_HUD ui;
	HudMessage_Manager hudMessage_manager;
	float healthFade;
	float ammoFade;
	float firingModeFade;
	int index_crosshair;
	int index_firingMode;
	int prev_ammoRatio;
	int prev_reloadRatio;

public:
	Menu_HUD(AttributePtr<Attribute_SplitScreen> splitScreen, QWidget* parent);
	~Menu_HUD();

	void mapToSplitscreen();
	void refresh();
	void refreshScoreboard();
	void onEvent(Event* e);
};

class Menu_HUDManager : public QObject , IObserver
{
	Q_OBJECT

private:
	std::vector<Menu_HUD*> huds;
	QWidget* parent;
	bool isEnabled;

public:
	Menu_HUDManager(QWidget* parent);
	void mapHudsToSplitscreen();
	void onEvent(Event* e);
};