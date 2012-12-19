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
		// subscribe to events
		SUBSCRIBE_TO_EVENT(this, EVENT_GET_ATTRIBUTE);

		positionAttributes_		.init(ATTRIBUTE_POSITION);
		spatialAttributes_		.init(ATTRIBUTE_SPATIAL);
		renderAttributes_		.init(ATTRIBUTE_RENDER);
		debugShapeAttributes_	.init(ATTRIBUTE_DEBUGSHAPE);
		physicsAttributes_		.init(ATTRIBUTE_PHYSICS);
		cameraAttributes_		.init(ATTRIBUTE_CAMERA);
		inputAttributes_		.init(ATTRIBUTE_INPUT);
		playerAttributes_		.init(ATTRIBUTE_PLAYER);
		boundingAttributes_		.init(ATTRIBUTE_BOUNDING);
		meshAttributes_			.init(ATTRIBUTE_MESH);
		projectileAttributes_	.init(ATTRIBUTE_PROJECTILE);

		healthAttributes_		.init(ATTRIBUTE_HEALTH);
		damageAttributes_		.init(ATTRIBUTE_DAMAGE);
		spawnPointAttributes_	.init(ATTRIBUTE_SPAWNPOINT);
		weaponStatsAttributes_	.init(ATTRIBUTE_WEAPONSTATS);
	}

public:
	AttributeStorage<PositionAttribute>			positionAttributes_;
	AttributeStorage<SpatialAttribute>			spatialAttributes_;
	AttributeStorage<RenderAttribute>			renderAttributes_;
	AttributeStorage<DebugShapeAttribute>		debugShapeAttributes_;
	AttributeStorage<PhysicsAttribute>			physicsAttributes_;
	AttributeStorage<CameraAttribute>			cameraAttributes_;
	AttributeStorage<InputAttribute>			inputAttributes_;
	AttributeStorage<PlayerAttribute>			playerAttributes_;
	AttributeStorage<BoundingAttribute>			boundingAttributes_;
	AttributeStorage<ProjectileAttribute>		projectileAttributes_;
	AttributeStorage<MeshAttribute>				meshAttributes_;
	AttributeStorage<HealthAttribute>			healthAttributes_;
	AttributeStorage<DamageAttribute>			damageAttributes_;
	AttributeStorage<SpawnPointAttribute>		spawnPointAttributes_;
	AttributeStorage<WeaponStatsAttribute>		weaponStatsAttributes_;

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
			event_getAttributeVector((Event_GetAttribute*)e);
			break;
		default:
			break;
		}
	}

	/**
	Provides access to AttributeManager's \ref ATTRIBUTES through 
	the use of Event_getAttributeVector. The data inside
	event_getAttributeVector can then be accessed by the sender.
	*/
	void event_getAttributeVector(Event_GetAttribute* e)
	{

		AttributeType type = (AttributeType)e->attributeEnum;

		switch(type) 
		{
			// Macro to facilitate adding and refactoring attributes
#define PUT_ATTRIBUTES_INSIDE_EVENT(AttributeStorage)	\
			e->hostVector = AttributeStorage.getAllAttributes();	\
			e->owners = AttributeStorage.getAllOwners();			\
			break;

			// If Enum matches, fetch information from the corresponding
			// AttributeStorage and store that information inside 
			// Event_getAttribute
		case ATTRIBUTE_POSITION:	PUT_ATTRIBUTES_INSIDE_EVENT(positionAttributes_		);
		case ATTRIBUTE_SPATIAL:		PUT_ATTRIBUTES_INSIDE_EVENT(spatialAttributes_		);
		case ATTRIBUTE_RENDER:		PUT_ATTRIBUTES_INSIDE_EVENT(renderAttributes_		);
		case ATTRIBUTE_DEBUGSHAPE:	PUT_ATTRIBUTES_INSIDE_EVENT(debugShapeAttributes_	);
		case ATTRIBUTE_PHYSICS:		PUT_ATTRIBUTES_INSIDE_EVENT(physicsAttributes_		);
		case ATTRIBUTE_CAMERA:		PUT_ATTRIBUTES_INSIDE_EVENT(cameraAttributes_		);
		case ATTRIBUTE_INPUT:		PUT_ATTRIBUTES_INSIDE_EVENT(inputAttributes_		);
		case ATTRIBUTE_PLAYER:		PUT_ATTRIBUTES_INSIDE_EVENT(playerAttributes_		);
		case ATTRIBUTE_BOUNDING:	PUT_ATTRIBUTES_INSIDE_EVENT(boundingAttributes_		);
		case ATTRIBUTE_MESH:		PUT_ATTRIBUTES_INSIDE_EVENT(meshAttributes_			);
		case ATTRIBUTE_PROJECTILE:	PUT_ATTRIBUTES_INSIDE_EVENT(projectileAttributes_	);
		case ATTRIBUTE_HEALTH:		PUT_ATTRIBUTES_INSIDE_EVENT(healthAttributes_		);
		case ATTRIBUTE_DAMAGE:		PUT_ATTRIBUTES_INSIDE_EVENT(damageAttributes_		);
		case ATTRIBUTE_SPAWNPOINT:	PUT_ATTRIBUTES_INSIDE_EVENT(spawnPointAttributes_	);
		case ATTRIBUTE_WEAPONSTATS:	PUT_ATTRIBUTES_INSIDE_EVENT(weaponStatsAttributes_	);
			

			// undefine dirty macro
#undef PUT_ATTRIBUTES_INSIDE_EVENT
			
		default:
			break;
		}
	}
};