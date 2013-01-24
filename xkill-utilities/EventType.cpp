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

Event_PlaySound::Event_PlaySound(int soundId, bool muteSound) : Event(EVENT_PLAYSOUND)
{
	this->soundId = soundId;
	this->muteSound = muteSound;
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

Event_CreateProjectile::Event_CreateProjectile(Float3 position, Float3 velocity, Float4 rotation, float damage, int entityIdOfCreator, bool explodeOnImpact) : Event(EVENT_CREATE_PROJECTILE)
{
	this->position = position;
	this->velocity = velocity;
	this->rotation = rotation;
	this->damage = damage;
	this->entityIdOfCreator = entityIdOfCreator;
	this->explodeOnImpact = explodeOnImpact;
}

Event_CreateMesh::Event_CreateMesh(unsigned int id, MeshModel* mesh, bool dynamic) : Event(EVENT_CREATE_MESH)
{
	this->id		= id;
	this->mesh		= mesh;
	this->dynamic	= dynamic;
}

Event_PhysicsAttributesColliding::Event_PhysicsAttributesColliding(int attribute1_index, int attribute2_index) : Event(EVENT_PHYSICS_ATTRIBUTES_COLLIDING)
{
	this->attribute1_index = attribute1_index;
	this->attribute2_index = attribute2_index;
}

Event_RemoveEntity::Event_RemoveEntity(int entityId) : Event(EVENT_REMOVE_ENTITY)
{
	this->entityId = entityId;
}

Event_KeyPress::Event_KeyPress(int keyEnum, bool isPressed) : Event(EVENT_KEY_PRESS)
{
	this->keyEnum = keyEnum;
	this->isPressed = isPressed;
}

Event_MousePress::Event_MousePress(int keyEnum, bool isPressed) : Event(EVENT_MOUSE_PRESS)
{
	this->keyEnum = keyEnum;
	this->isPressed = isPressed;
}

Event_CreateSpawnPoint::Event_CreateSpawnPoint(Float3 spawnPointPosition, float spawnAreaRadius) : Event(EVENT_CREATE_SPAWNPOINT)
{
	this->spawnPointPosition = spawnPointPosition;
	this->spawnAreaRadius = spawnAreaRadius;
}

Event_StartDeathmatch::Event_StartDeathmatch(int num_players) : Event(EVENT_START_DEATHMATCH)
{
	this->num_players = num_players;
}

Event_EndDeathmatch::Event_EndDeathmatch() : Event(EVENT_END_DEATHMATCH)
{
}

Event_StateChanged::Event_StateChanged(StateType newState, FiniteStateMachine* sender) : Event(EVENT_STATE_CHANGED)
{
	this->sender = sender;
	this->newState = newState;
}

Event_SyncStateCommand::Event_SyncStateCommand( 
	FiniteStateMachine* sender, 
	StateType fromState, 
	StateType toState, 
	bool isReplacementState )
	: Event(EVENT_SYNCSTATECOMMAND)
{
	this->sender = sender;
	this->fromState = fromState;
	this->toState = toState;
	this->isReplacementState = isReplacementState;
}

Event_CreateExplosionSphere::Event_CreateExplosionSphere(Float3 position, float radius, float damage, int entityIdOfCreator) : Event(EVENT_CREATE_EXPLOSIONSPHERE)
{
	this->position = position;
	this->radius = radius;
	this->damage = damage;
	this->entityIdOfCreator = entityIdOfCreator;
}

Event_CreateWorld::Event_CreateWorld(Float3 position, Float4 rotation, unsigned int meshID) : Event(EVENT_CREATE_WORLD)
{
	this->position = position;
	this->rotation = rotation;
	this->meshID = meshID;
}

Event_CreateAmmo::Event_CreateAmmo(Float3 position, unsigned int type) : Event(EVENT_CREATE_AMMO)
{
	this->position = position;
	this->position = position;
}

Event_CreateHack::Event_CreateHack(Float3 position, unsigned int type) : Event(EVENT_CREATE_HACK)
{
	this->type = type;
	this->type = type;
}

Event_CreateLight::Event_CreateLight(Float3 position, Float3 direction, Float3 ambient, Float3 diffuse, Float3 specular,
				  Float3 attenuation, float range, float spotPow, unsigned int type) : Event(EVENT_CREATE_LIGHT)
{
	this->position = position;
	this->direction = direction;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->attenuation = attenuation;
	this->range = range;
	this->spotPow = spotPow;
	this->type = type;
}

Event_TransferEventsToGame::Event_TransferEventsToGame(std::vector<Event*> events) : Event(EVENT_TRANSFEREVENTSTOGAME)
{
	this->events = events;
}

Event_CreateEntity::Event_CreateEntity( EntityType entityType ) : Event(EVENT_CREATE_ENTITY)
{
	this->entityType = entityType;
}

Event_CreateInputDevice::Event_CreateInputDevice(InputDevice* inputDevice, InputObjectArray* inputObjectArray) : Event(EVENT_CREATE_INPUTDEVICE)
{
	device = inputDevice;
	objectArray = inputObjectArray;
}

Event_DrawBulletPhysicsDebugLines::Event_DrawBulletPhysicsDebugLines(std::vector<VertexPosColor>* debugLineVertices) : Event (EVENT_DRAW_BULLET_PHYSICS_DEBUG_LINES)
{
	this->debugLineVertices = debugLineVertices;
}