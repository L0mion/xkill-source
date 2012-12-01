#pragma once

#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventManager.h>
#include "AttributeStorage.h"

/// Singleton responsible for providing access to each Attribute.
/** 
\ingroup ARCHITECTURE
*/

class AttributeManager: public IObserver
{
private:
	AttributeManager()
	{
		SUBSCRIBE_TO_EVENT(this, EVENT_GET_ATTRIBUTE);
	}

public:
	AttributeStorage<PositionAttribute> positionAttributes;
	AttributeStorage<SpatialAttribute>  spatialAttributes;
	AttributeStorage<RenderAttribute>	renderAttributes;
	AttributeStorage<PhysicsAttribute>	physicsAttributes;
	AttributeStorage<CameraAttribute>	cameraAttributes;
	AttributeStorage<InputAttribute>	inputAttributes;
	AttributeStorage<PlayerAttribute>	playerAttributes;
	AttributeStorage<BoundingAttribute> boundingAttributes;

	static AttributeManager* getInstance()
	{
		static AttributeManager instance;
		return &instance;
	}

	/**
	Handles Events for AttributeManager.
	*/
	void onEvent(Event* e)
	{
		EventType type = e->getType();
		switch (type) 
		{
		case EVENT_GET_ATTRIBUTE:
			event_getAttributeVector((Event_getAttribute*)e);
			break;
		default:
			break;
		}
	}

	/**
	Provides access to AttributeManager's \ref ATTRIBUTES through 
	the use of Event_getAttributeVector. The data inside
	vent_getAttributeVector can then be accessed by the sender.
	*/
	void event_getAttributeVector(Event_getAttribute* e)
	{

		AttributeType type = (AttributeType)e->attributeEnum;

		switch(type) 
		{
			// Macro to facilitate adding and refactoring attributes
#define PUT_ATTRIBUTES_INSIDE_EVENT(Enum, AttributeStorage, EventObject)	\
		case Enum:															\
			EventObject->hostVector = AttributeStorage.getAllAttributes();	\
			EventObject->owners = AttributeStorage.getAllOwners();			\
			break;

			// If Enum matches, fetch information from the correspoinding
			// AttributeStorage and store that information inside 
			// Event_getAttribute
			PUT_ATTRIBUTES_INSIDE_EVENT(ATT_POSITION,	positionAttributes,		e);
			PUT_ATTRIBUTES_INSIDE_EVENT(ATT_SPATIAL,	spatialAttributes,		e);
			PUT_ATTRIBUTES_INSIDE_EVENT(ATT_RENDER,		renderAttributes,		e);
			PUT_ATTRIBUTES_INSIDE_EVENT(ATT_PHYSICS,	physicsAttributes,		e);
			PUT_ATTRIBUTES_INSIDE_EVENT(ATT_CAMERA,		cameraAttributes,		e);
			PUT_ATTRIBUTES_INSIDE_EVENT(ATT_INPUT,		inputAttributes,		e);
			PUT_ATTRIBUTES_INSIDE_EVENT(ATT_PLAYER,		playerAttributes,		e);
			PUT_ATTRIBUTES_INSIDE_EVENT(ATT_BOUNDING,	boundingAttributes,		e);

			// undefine dirty macro
#undef PUT_ATTRIBUTES_INSIDE_EVENT
			
		default:
			break;
		}
	}
};