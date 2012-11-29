#pragma once

#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventType.h>
#include <xkill-utilities/AttributeType.h>
#include <vector>
#include <iostream>

#include "AttributeManager.h"

/// Component to process RenderAttribute
/** 
Some other attributes associated with rendering, such
as CameraAttribute should probably be processed 
here as well.

\ingroup COMPONENTS
*/

class RenderComponent : public IObserver
{
private:

public:
	RenderComponent()
	{
	}

	void onEvent(Event* e)
	{
		EventType type = e->getType();
		switch (type) 
		{
		case EVENT_A:
			break;
		default:
			break;
		}
	};

	void onUpdate(float delta)
	{
		// Setup
		std::vector<RenderAttribute>* attributes = AttributeManager::getInstance()->renderAttributes.getAllAttributes();
		std::vector<int>* owners = AttributeManager::getInstance()->renderAttributes.getAllOwners();

		// Write test
		std::cout << "RENDERCOMPONENT: Write test" << std::endl;
		for(int i=0; i<(int)(*attributes).size(); i++)
		{
			if(owners->at(i)!=0)
			{
				
				RenderAttribute* r = &attributes->at(i);
				SpatialAttribute* s = ATTRIBUTE_CAST(SpatialAttribute,spatialAttribute,r);
				PositionAttribute* p = ATTRIBUTE_CAST(PositionAttribute,positionAttribute,s);

				//p->position = i
				//	+ r->transparent 
				//	+ r->tessellation 
				//	//+ r->meshid 
				//	//+ r->textureid
				//	+ p->position
				//	+ s->rotation
				//	+ s->scale;
			}
		}

		// Read test
		std::cout << "RENDERCOMPONENT: Read test" << std::endl; 
		std::cout << "Owner\tTransp\tTess\tMesh\tTex\tPos\tRot\tScale" << std::endl;
		for(int i=0; i<(int)(*attributes).size(); i++)
		{
			if(owners->at(i)!=0)
			{
				RenderAttribute* r = &attributes->at(i);
				SpatialAttribute* s = ATTRIBUTE_CAST(SpatialAttribute,spatialAttribute,r);
				PositionAttribute* p = ATTRIBUTE_CAST(PositionAttribute,positionAttribute,s);

				std::cout
					<< owners->at(i)		<< "\t"
					<< r->transparent		<< "\t"
					<< r->tessellation		<< "\t"
					<< r->meshID			<< "\t"
					<< r->textureID			<< "\t"
					<< p->position			<< "\t"
					<< s->rotation			<< "\t"
					<< s->scale 
					<< std::endl;
			}
			else
			{
				std::cout
					<< i		<< "\t"
					<< "DELETED"
					<< std::endl;
			}
		}
	}
};