#pragma once

#include "IComponent.h"

class RenderComponent : public IComponent
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
		vector<RenderAttribute>* attributes = AttributeManager::getInstance()->renderAttributes.getAllAttributes();


		//subcomponent.onUpdate(delta);

		vector<int>* owners = AttributeManager::getInstance()->renderAttributes.getAllOwners();

		// Write test
		cout << "RENDERCOMPONENT: Write test" << endl;
		for(int i=0; i<(int)(*attributes).size(); i++)
		{
			if(owners->at(i)!=0)
			{
				
				RenderAttribute* r = &attributes->at(i);
				//SpatialAttribute* s = &((std::vector<SpatialAttribute>*)r->spatialAttribute.host)->at(r->spatialAttribute.index);
				SpatialAttribute* s = ATTRIBUTE_CAST(SpatialAttribute,spatialAttribute,r);
				PositionAttribute* p = ATTRIBUTE_CAST(PositionAttribute,positionAttribute,s);
				//SpatialAttribute* s = r->spatialAttribute.getAttribute();

				/*struct MyStruct
				{
				transparent* r;
				SpatialAttribute* s;
				PositionAttribute* p;
				};
				SubRenderComponent;*/

				p->position = i
					+ r->transparent 
					+ r->tessellation 
					//+ r->meshid 
					//+ r->textureid
					+ p->position
					+ s->rotation
					+ s->scale;
			}
		}

		//// Read test
		//cout << "RENDERCOMPONENT: Read test" << endl; 
		//cout << "Owner\tTransp\tTess\tMesh\tTex\tPos\tRot\tScale" << endl;
		//for(int i=0; i<(int)(*attributes).size(); i++)
		//{
		//	if(owners->at(i)!=0)
		//	{
		//		RenderAttribute* r = &attributes->at(i);
		//		SpatialAttribute* s = r->spatialAttribute.getAttribute();
		//		PositionAttribute* p = s->positionAttribute.getAttribute();

		//		cout
		//			<< owners->at(i)		<< "\t"
		//			<< r->transparent		<< "\t"
		//			<< r->tessellation		<< "\t"
		//			<< r->meshID			<< "\t"
		//			<< r->textureID			<< "\t"
		//			<< p->position			<< "\t"
		//			<< s->rotation			<< "\t"
		//			<< s->scale 
		//			<< endl;
		//	}
		//}
	}
};