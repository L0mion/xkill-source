#pragma once

#include <vector>
#include <xkill-utilities/Math.h>
#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/IObserver.h>

//! Component to process Culling of world
/** 
\ingroup components
*/
class CullingComponent : public IObserver
{
	Int2 yminmax;
	Int2 min_,max_;
	std::vector<std::vector<AttributePtr<Attribute_Render>>> nodes_;
	void cameraVsNode(Int2 min, Int2 max, AttributePtr<Attribute_Camera> camera);
public:
	//! Initializes Culling Component to its default state.
	CullingComponent();
	//! Releases all allocated memory.
	virtual ~CullingComponent();
	//! Initializes culling. Returns true.
	bool init();
	//! Methods that will be called on events. 
	void onEvent(Event* e);
	
	//! Update method that will be called every frame.
	void onUpdate(float delta);
};