#pragma once

#include "dllSound.h"
#include <xkill-utilities/IObserver.h>

class FMODEventSystem;

/// Component responsible for playing Sound based on which Event is sent
/** 
If 3D positioning of sound is needed, creating an Entity 
with a SoundAttribute and process each Attribute here is 
probably the best approach.

\ingroup COMPONENTS
*/

class DLL_S SoundComponent : public IObserver
{
private:
	FMODEventSystem* mFMODEventSystem;

public:
	SoundComponent();
	~SoundComponent();
	bool init();

	void onEvent(Event* e);
	void onUpdate(float delta);
};