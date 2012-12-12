#include "EventType.h"

Event::Event(EventType type) 
{ 
	this->type = type; 
}

EventType Event::getType() 
{ 
	return type; 
}

Event_MouseMove::Event_MouseMove(int dx, int dy) : Event(EVENT_MOUSE_MOVE)
{
	this->dx = dx;
	this->dy = dy;
}

Event_PlaySound::Event_PlaySound(int soundId) : Event(EVENT_PLAYSOUND)
{
	this->soundId = soundId;
}

Event_Rumble::Event_Rumble(unsigned int deviceNr,
						   bool runRumble,
						   float duration,
						   float leftScale,
						   float rightScale) : 	Event(EVENT_RUMBLE)
{
	this->deviceNr = deviceNr;
	this->runRumble = runRumble;
	this->duration = duration;
	this->leftScale = leftScale;
	this->rightScale = rightScale;
}

Event_GetAttribute::Event_GetAttribute(int attributeEnum) : Event(EVENT_GET_ATTRIBUTE)
{
	this->attributeEnum = attributeEnum;
		
	hostVector = 0;
	owners = 0;
}

Event_GetEntities::Event_GetEntities() : Event(EVENT_GET_ENTITIES)
{
}

Event_GetWindowResolution::Event_GetWindowResolution() : Event(EVENT_GET_WINDOW_RESOLUTION)
{
	width = 320;
	height = 240;
}

float Event_GetWindowResolution::getAspectRatio()
{
	return (float)width/(float)height;
}

Event_WindowResize::Event_WindowResize(int width, int height) : Event(EVENT_WINDOW_RESIZE)
{
	this->width = width;
	this->height = height;
}

float Event_WindowResize::getAspectRatio()
{
	return (float)width/(float)height;
}

Event_ShowMessageBox::Event_ShowMessageBox(std::string message) : Event(EVENT_SHOW_MESSAGEBOX)
{
	this->message = message;
}

Event_CreateProjectile::Event_CreateProjectile(Float3 position, Float3 velocity, Float4 rotation, Float3 gravity, int entityIdOfCreator) : Event(EVENT_CREATE_PROJECTILE)
{
	this->position = position;
	this->velocity = velocity;
	this->rotation = rotation;
	this->gravity = gravity;
	this->entityIdOfCreator = entityIdOfCreator;
}

Event_CreateMesh::Event_CreateMesh(MeshModel* mesh, bool dynamic) : Event(EVENT_CREATE_MESH)
{
	this->mesh		= mesh;
	this->dynamic	= dynamic;
}

Event_PhysicsAttributesColliding::Event_PhysicsAttributesColliding(int attribute1_index, int attribute2_index) : Event(EVENT_ENTITIES_COLLIDING)
{
	this->attribute1_index = attribute1_index;
	this->attribute2_index = attribute2_index;
}

Event_RemoveEntity::Event_RemoveEntity(int entityId) : Event(EVENT_REMOVE_ENTITY)
{
	this->entityId = entityId;
}

Event_KeyPress::Event_KeyPress(int keyEnum) : Event(EVENT_KEY_PRESS)
{
	this->keyEnum = keyEnum;
}

Event_KeyRelease::Event_KeyRelease(int keyEnum) : Event(EVENT_KEY_RELEASE)
{
	this->keyEnum = keyEnum;
}

Event_DoCulling::Event_DoCulling() : Event(EVENT_DO_CULLING)
{
}