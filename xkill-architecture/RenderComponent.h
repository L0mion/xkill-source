#pragma once

#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventManager.h>
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
		std::vector<int>* renderOwners;					GET_ATTRIBUTE_OWNERS(renderOwners, ATTRIBUTE_RENDER);
		std::vector<RenderAttribute>* allRender;		GET_ATTRIBUTES(allRender, RenderAttribute, ATTRIBUTE_RENDER);
		std::vector<SpatialAttribute>* allSpatial;		GET_ATTRIBUTES(allSpatial, SpatialAttribute, ATTRIBUTE_SPATIAL);
		std::vector<PositionAttribute>* allPosition;	GET_ATTRIBUTES(allPosition, PositionAttribute, ATTRIBUTE_POSITION);

		// Write test
		std::cout << "RENDERCOMPONENT: Write test" << std::endl;
		for(unsigned i=0; i<allRender->size(); i++)
		{
			if(renderOwners->at(i)!=0)
			{
				// Fetch attributes
				RenderAttribute* render		=	&allRender		->	at(i);
				SpatialAttribute* spatial	=	&allSpatial		->	at(render->spatialAttribute.index);
				PositionAttribute* position	=	&allPosition	->	at(spatial->positionAttribute.index);

				position->position[0] = i
					+ render->transparent 
					+ render->tessellation 
					+ position->position;
			}
		}

		// Read test
		std::cout << "RENDERCOMPONENT: Read test" << std::endl; 
		std::cout << "Owner\tTransp\tTess\tMesh\tTex\tPos\tRot\tScale" << std::endl;
		for(unsigned i=0; i<allRender->size(); i++)
		{
			if(renderOwners->at(i)!=0)
			{
				// Fetch attributes
				RenderAttribute* render		=	&allRender		->	at(i);
				SpatialAttribute* spatial	=	&allSpatial		->	at(render->spatialAttribute.index);
				PositionAttribute* position	=	&allPosition	->	at(spatial->positionAttribute.index);

				std::cout
					<< renderOwners->at(i)		<< "\t"
					<< render->transparent		<< "\t"
					<< render->tessellation		<< "\t"
					<< render->meshIndex			<< "\t"
					<< render->textureID		<< "\t"
					<< position->position[0]	<< "\t"
					<< spatial->rotation[0]		<< "\t"
					<< spatial->scale[0]
					<< std::endl;
			}
			else
			{
				std::cout << i << "\t" << "DELETED" << std::endl;
			}
		}
	}
};