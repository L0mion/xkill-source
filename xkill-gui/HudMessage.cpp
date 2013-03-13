#include "HudMessage.h"

HudMessage::HudMessage( Event_PostHudMessage* e, QWidget* parent )
{
	// Create label
	_label = new QLabel(e->message.c_str());
	_label->setAlignment(Qt::AlignHCenter);
	_label->setParent(parent);

	// Apply stylesheet
	_label->setStyleSheet(e->styleSheet.c_str());

	// Display for a certain time
	_lifetime = 3.0f;

	// Show label
	_label->show();
}


HudMessage::~HudMessage()
{
	delete _label;
}

int HudMessage::getHeight()
{
	return _label->height();
}

void HudMessage::setPosition( Float2 position )
{
	setTargetPosition(position);

	position = _targetPosition;

	// An additional offset,
	// creates a cool interpolation
	// when a new label is created
	position = position + Float2(-_label->height() * 4.0f, -_label->height() * 0.0f);

	// Move
	_label->move(position.x, position.y);
}

void HudMessage::setTargetPosition( Float2 position )
{
	// Offset relative to center of label
	position.x -= _label->width() * 0.5f;
	position.y -= _label->height() * 0.5f;

	_targetPosition = position;
}

void HudMessage::updatePosition()
{
	// Interpolate position
	Float2 currentPos;
	currentPos.x = _label->pos().x();
	currentPos.y = _label->pos().y();

	float factor = 5.0f * SETTINGS->trueDeltaTime;
	if(factor > 1.0f)
		factor = 1.0f;
	Float2 newPos = Float2::lerp(&currentPos, &_targetPosition, factor);

	_label->move(newPos.x, newPos.y);
}

void HudMessage::update()
{
	updatePosition();

	// Decrement timer
	if(_lifetime > 0.0f)
		_lifetime -= SETTINGS->trueDeltaTime;
}

bool HudMessage::isExpired()
{
	// Check expiration condition
	if(_lifetime <= 0.0f)
		return true;

	return false;
}

HudMessage_Manager::~HudMessage_Manager()
{
	UNSUBSCRIBE_TO_EVENTS(this);

	for(int i=0; i<stack.count(); i++)
		delete stack.at(i);
}

HudMessage_Manager::HudMessage_Manager()
{
	SUBSCRIBE_TO_EVENT(this, EVENT_POST_HUD_MESSAGE);
}

void HudMessage_Manager::init( QWidget* parent, AttributePtr<Attribute_SplitScreen> splitScreen )
{
	this->parent = parent;
	this->splitScreen = splitScreen;
}

void HudMessage_Manager::move( Float2 position )
{
	this->position = position;
}

void HudMessage_Manager::update()
{
	for(int i=0; i<stack.count(); i++)
	{
		stack.at(i)->update();
		if(stack.at(i)->isExpired())
			removeTopMessage();
	}
}

void HudMessage_Manager::addMessage( Event_PostHudMessage* e )
{
	Event_PostHudMessage::Receiver re= e->receiver;

	// Ignore messages aimed at other players
	if(e->receiver == Event_PostHudMessage::RECEIVER_ONLY_SUBJECT)
	{
		if(splitScreen->ptr_player != e->ptr_subject_player)
			return;
	}
	if(e->receiver == Event_PostHudMessage::RECEIVER_ALL_BUT_SUBJECT)
	{
		if(splitScreen->ptr_player == e->ptr_subject_player)
			return;
	}

	// Limit simultaneous show messages 
	// to not overwhelm the player
	const int kMaxMessages = 8;
	if(stack.count() + 1 > kMaxMessages)
		removeTopMessage();

	// Add message to stack
	HudMessage* m = new HudMessage(e, parent);
	stack.push(m);

	// Show new messages above old messages
	int numStacks = stack.count();
	int offset = 20*3.8f;
	Float2 newPos;
	newPos.x = position.x;
	newPos.y = position.y + offset;
	m->setPosition(newPos);
	for(int i=numStacks-1; i>=0; i--)
	{
		int height = stack.at(i)->getHeight();
		offset += height;

		Float2 newPos;
		newPos.x = position.x;
		newPos.y = position.y + offset - height * 0.5f;

		stack.at(i)->setTargetPosition(newPos);
	}
}

void HudMessage_Manager::removeTopMessage()
{
	delete stack.pop();
}

void HudMessage_Manager::onEvent( Event* e )
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

void HudMessage_Manager::silenceAllMessages()
{
	for(int i=0; i<stack.count(); i++)
		removeTopMessage();
}
