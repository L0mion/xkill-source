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
	AttributeStorage<PositionAttribute>			positionAttributes_;
	AttributeStorage<SpatialAttribute>			spatialAttributes_;
	AttributeStorage<RenderAttribute>			renderAttributes_;
	AttributeStorage<PhysicsAttribute>			physicsAttributes_;
	AttributeStorage<CameraAttribute>			cameraAttributes_;
	AttributeStorage<InputAttribute>			inputAttributes_;
	AttributeStorage<PlayerAttribute>			playerAttributes_;
	AttributeStorage<BoundingAttribute>			boundingAttributes_;

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

			// If Enum matches, fetch information from the corresponding
			// AttributeStorage and store that information inside 
			// Event_getAttribute

			PUT_ATTRIBUTES_INSIDE_EVENT(ATTRIBUTE_POSITION,	positionAttributes_,	e);
			PUT_ATTRIBUTES_INSIDE_EVENT(ATTRIBUTE_SPATIAL,	spatialAttributes_,		e);
			PUT_ATTRIBUTES_INSIDE_EVENT(ATTRIBUTE_RENDER,	renderAttributes_,		e);
			PUT_ATTRIBUTES_INSIDE_EVENT(ATTRIBUTE_PHYSICS,	physicsAttributes_,		e);
			PUT_ATTRIBUTES_INSIDE_EVENT(ATTRIBUTE_CAMERA,	cameraAttributes_,		e);
			PUT_ATTRIBUTES_INSIDE_EVENT(ATTRIBUTE_INPUT,	inputAttributes_,		e);
			PUT_ATTRIBUTES_INSIDE_EVENT(ATTRIBUTE_PLAYER,	playerAttributes_,		e);
			PUT_ATTRIBUTES_INSIDE_EVENT(ATTRIBUTE_BOUNDING,	boundingAttributes_,	e);

			// undefine dirty macro
#undef PUT_ATTRIBUTES_INSIDE_EVENT
			
		default:
			break;
		}
	}
};