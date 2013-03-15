#include "CullingComponent.h"

#include <xkill-utilities/Util.h>


ATTRIBUTES_DECLARE_ALL;


CullingComponent::CullingComponent()
{
	// subscribe to events
	SUBSCRIBE_TO_EVENT(this, EVENT_START_DEATHMATCH);
	ATTRIBUTES_INIT_ALL;
}

CullingComponent::~CullingComponent()
{
	UNSUBSCRIBE_TO_EVENTS(this);
}

bool CullingComponent::init()
{

	return true;
}

void CullingComponent::onEvent(Event* e)
{
	
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_START_DEATHMATCH:
		nodes_.clear();
		yminmax=min_=max_=Int2(0,0);
		while(itrRender.hasNext())
		{
			AttributePtr<Attribute_Render> ptr_render = itrRender.getNext();
			if(ptr_render->meshID == 200)
			{
				Float3 position = ptr_render->ptr_spatial->ptr_position->position;
				if(min_.x > ((int)position.x))
					min_.x = ((int)position.x);
				if(min_.y > ((int)position.z))
					min_.y = ((int)position.z);
				if(max_.x < ((int)position.x))
					max_.x = ((int)position.x);
				if(max_.y < ((int)position.z))
					max_.y = ((int)position.z);

				if(yminmax.x > ((int)position.y));
					yminmax.x = position.y;
				if(yminmax.y < ((int)position.y));
					yminmax.y = position.y;
			}
		}
		max_.x++;
		max_.y++;
		int width = max_.x-min_.x;
		int height = max_.y-min_.y;

		nodes_ = std::vector<std::vector<AttributePtr<Attribute_Render>>>((width)*(height));
		while(itrRender.hasNext())
		{
			AttributePtr<Attribute_Render> ptr_render = itrRender.getNext();
			if(ptr_render->meshID == 200)
			{
				Int2 pos = Int2((int)ptr_render->ptr_spatial->ptr_position->position.x-min_.x,(int)ptr_render->ptr_spatial->ptr_position->position.z-min_.y);
				nodes_.at((pos.x) + (pos.y) * width).push_back(ptr_render);
			}
		}
		break;
	}
}

void CullingComponent::cameraVsNode(Int2 min, Int2 max, AttributePtr<Attribute_Camera> camera)
{
	if((max.x-min.x > 0) && (max.y-min.y > 0))
	{
		float fovx = atan(camera->aspectRatio*tan(camera->fieldOfView));
		float length = sqrt(camera->look.x*camera->look.x+camera->look.z*camera->look.z);
		Float2 pos(camera->ptr_spatial->ptr_position->position.x,camera->ptr_spatial->ptr_position->position.z);
		Float2 look(camera->look.x/length,camera->look.z/length);

		float l = camera->look.y; 
		float f = camera->fieldOfView;
		float viewbehind = (camera->fieldOfView/2.0f + PI/4 + abs(asin(camera->look.y))) - PI/2; 
		if(viewbehind > 0)
		{
			viewbehind = tan(viewbehind)*max(abs(camera->ptr_spatial->ptr_position->position.y - yminmax.x),abs(camera->ptr_spatial->ptr_position->position.y - yminmax.y));
			Float2 lb(look.x*viewbehind,look.y*viewbehind);
			pos = pos - lb;
		}

		
		
		Float2 p[] = {Float2(min.x-0.5f-pos.x,min.y-0.5f-pos.y),
					  Float2(min.x-0.5f-pos.x,max.y+0.5f-pos.y),
					  Float2(max.x+0.5f-pos.x,min.y-0.5f-pos.y),
					  Float2(max.x+0.5f-pos.x,max.y+0.5f-pos.y)};
		bool hit = false;
		for(unsigned int i = 0; i < 4; i++)
		{
			float angle = acos((look.x*p[i].x+look.y*p[i].y) / (look.length()*p[i].length()));
			///SMOETHING HERE
			if(angle < fovx)
			{
				hit = true;
				break;
			}
		}
		if(!hit)
		{
			if((min.x < pos.x && pos.x < max.x) && (min.y < pos.y && pos.y < max.y))
			{
				hit = true;
			}
		}

		if(hit)
		{
			if( (max.x-min.x)*(max.y-min.y) <= 1) // if less than 4 grids left then uncull
			{
				int width = max_.x-min_.x;
				for(int i = min.y; i < max.y; i++)
				{
					for(int j = min.x; j < max.x; j++)
					{
						int index = (j-min_.x) + (i-min_.y)*width;
						unsigned int size = nodes_[index].size();
						for(unsigned int k = 0; k < size; k++)
						{
							nodes_[index].at(k)->culling.setBool(camera.index(),true);
						}
					}
				}
			}
			else // otherwise subdivide
			{
				int width = (max.x - min.x)/2;
				int height = (max.y - min.y)/2;
				cameraVsNode(min,Int2(min.x+width,min.y+height),camera);
				cameraVsNode(Int2(min.x+width,min.y),Int2(max.x,min.y+height),camera);
				cameraVsNode(Int2(min.x,min.y+height),Int2(min.x+width,max.y),camera);
				cameraVsNode(Int2(min.x+width,min.y+height),max,camera);
			}
		}
	}
}

void CullingComponent::onUpdate(float delta)
{
	while(itrRender.hasNext())
	{
		AttributePtr<Attribute_Render> ptr_render = itrRender.getNext();
		if(ptr_render->meshID != XKILL_Enums::ModelId::LASER && ptr_render->meshID != 201 && !(ptr_render->meshID > 99 && ptr_render->meshID<200))
		{
			ptr_render->culling.clear();
		}
	}
	while(itrCamera.hasNext())
	{
		AttributePtr<Attribute_Camera> ptr_camera = itrCamera.getNext();
		//quadtreecull
		cameraVsNode(min_,max_,ptr_camera);
		//simple cull
		while(itrRender.hasNext())
		{
			AttributePtr<Attribute_Render> ptr_render = itrRender.getNext();
			if(ptr_render->meshID != XKILL_Enums::ModelId::LASER && ptr_render->meshID != 200 && ptr_render->meshID != 201 && !(ptr_render->meshID > 99 && ptr_render->meshID<200))
			{
				Float3 p0 = ptr_render->ptr_spatial->ptr_position->position;
				Float3 p1 = ptr_camera->ptr_spatial->ptr_position->position;
				Float3 p2 = ptr_camera->ptr_spatial->ptr_position->position + ptr_camera->look;
		
				float l = (p2-p1).length();
				float t = (p1-p0).dot(p2-p1)/(l*l);
				if(t < 0)
				{
					ptr_render->culling.setBool(ptr_camera.index(),true);
				}
			}
		}
	}

}