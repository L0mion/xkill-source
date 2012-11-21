#pragma once

//
// Enums
//

enum EventType
{
	EVENT_A,
	EVENT_B,
	EVENT_C,
	EVENT_D,
	EVENT_E,
	EVENT_F,

	EVENT_RENDER_SUB,

	EVENT_LAST
};


//
// Event types
//

// Base class

class Event
{
private:
	EventType type;

public:
	Event(EventType type) 
	{ 
		this->type = type; 
	}

	EventType getType() 
	{ 
		return type; 
	}
};


// Child classes

class Event_A : public Event
{
public:
	Event_A() : Event(EVENT_A){}

	int a;
};

class Event_B : public Event
{
public:
	Event_B() : Event(EVENT_B){}

	int b;
};