#pragma once

#include <xkill-utilities/Util.h>
#include <QtGui/QLabel>


class HudMessage
{
private:
	float _lifetime;
	QLabel* _label;
	Float2 _targetPosition;

public:
	HudMessage(Event_PostHudMessage* e, QWidget* parent);
	~HudMessage();

	void update();
	int getHeight();
	void setPosition(Float2 position);
	void setTargetPosition(Float2 position);
	void updatePosition();
	bool isExpired();
};

class HudMessage_Manager : IObserver
{
private:
	SimpleQueue<HudMessage*> stack;
	QWidget* parent;
	Float2 position;
	AttributePtr<Attribute_SplitScreen> splitScreen;

public:
	HudMessage_Manager();
	~HudMessage_Manager();
	void init(QWidget* parent, AttributePtr<Attribute_SplitScreen> splitScreen);
	void onEvent(Event* e);

	void update();
	void move(Float2 position);
	void addMessage(Event_PostHudMessage* e);
	void removeTopMessage();
};