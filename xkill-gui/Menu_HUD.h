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
	float lifetime;
	QLabel* message;

public:
	HudMessage(Event_PostHudMessage* e, QWidget* parent)
	{
		message = new QLabel(e->message.c_str());

		message->setParent(parent);
		lifetime = 2.0f;
		message->show();
	}
	~HudMessage()
	{
		delete message;
	}
	void move(Float2 position)
	{
		// Offset relative to center of label
		position.x -= message->width() * 0.5f;
		position.y -= message->height() * 0.5f;

		message->move(position.x, position.y);
	}
	bool isExpired()
	{
		// Decrement timer
		if(lifetime > 0.0f)
			lifetime -= SETTINGS->trueDeltaTime;

		// Check expiration condition
		if(lifetime <= 0.0f)
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
			if(stack.at(i)->isExpired())
				removeTopMessage();
		}
	}

	void addMessage(Event_PostHudMessage* e)
	{
		// Ignore messages aimed at other players
		if(splitScreen->ptr_player != e->ptr_subject_player)
			return;

		// Limit simultaneous show messages 
		// to 5 to not overwhelm the player
		if(stack.count() + 1 > 5)
			removeTopMessage();

		// Add message to stack
		stack.push(new HudMessage(e, parent));

		// Show new messages above old messages
		int numStacks = stack.count();
		for(int i=0; i<numStacks; i++)
		{
			int spacing = 20;

			Float2 newPos;
			newPos.x = position.x;
			newPos.y = position.y + (numStacks - i)*spacing + spacing*3.8f;

			stack.at(i)->move(newPos);
		}
	}

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
	AttributePtr<Attribute_SplitScreen> splitScreen;
	Ui::Menu_HUD ui;
	HudMessage_Manager hudMessage_manager;
	float healthFade;
	float ammoFade;

public:
	Menu_HUD(AttributePtr<Attribute_SplitScreen> splitScreen, QWidget* parent);
	~Menu_HUD();

	void mapToSplitscreen();
	void refresh();
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