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

Event_PlaySound::Event_PlaySound(int soundId, Float3 position, bool use3DAudio) : Event(EVENT_PLAYSOUND)
{
	this->soundId = soundId;
	this->position = position;
	this->use3DAudio = use3DAudio;
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


Event_AttributeUpdated::Event_AttributeUpdated(int index, int attributeEnum) : Event(EVENT_ATTRIBUTE_UPDATED)
{
	this->index = index;
	this->attributeEnum = attributeEnum;
	isCreated = false;
	isDeleted = false;
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

Event_CreateProjectile::Event_CreateProjectile(Float3 position, Float3 velocity, Float4 rotation, int entityIdOfCreator, XKILL_Enums::AmmunitionType ammunitionType, XKILL_Enums::FiringModeType firingMode, float damage) : Event(EVENT_CREATE_PROJECTILE)
{
	this->position = position;
	this->velocity = velocity;
	this->rotation = rotation;
	this->entityIdOfCreator = entityIdOfCreator;
	this->ammunitionType = ammunitionType;
	this->firingMode = firingMode;
	this->damage = damage;
}

Event_CreateMesh::Event_CreateMesh(
	unsigned int	id, 
	MeshDesc		mesh, 
	bool			dynamic, 
	std::string		fileName,
	VertexType		vertexType) : Event(EVENT_CREATE_MESH)
{
	this->id		= id;
	this->mesh		= mesh;
	this->dynamic	= dynamic;
	this->fileName	= fileName;
	this->vertexType = vertexType;
}

Event_LoadTextures::Event_LoadTextures(TexDesc* texDesc) : Event(EVENT_LOAD_TEXTURES)
{
	texDesc_ = texDesc;
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

Event_PlayerDeath::Event_PlayerDeath(int playerAttributeIndex) : Event(EVENT_PLAYERDEATH)
{
	this->playerAttributeIndex = playerAttributeIndex;
}

Event_CreatePlayerSpawnPoint::Event_CreatePlayerSpawnPoint(Float3 spawnPointPosition, float spawnAreaRadius) : Event(EVENT_CREATE_PLAYER_SPAWNPOINT)
{
	this->spawnPointPosition = spawnPointPosition;
	this->spawnAreaRadius = spawnAreaRadius;
}

Event_CreatePickupablesSpawnPoint::Event_CreatePickupablesSpawnPoint(Float3 spawnPointPosition, XKILL_Enums::PickupableType pickupableType) : Event(EVENT_CREATE_PICKUPABLES_SPAWNPOINT)
{
	this->spawnPointPosition = spawnPointPosition;
	this->pickupableType = pickupableType;
}

Event_CreatePickupable::Event_CreatePickupable(Float3 position, XKILL_Enums::PickupableType pickupableType, AttributePtr<Attribute_PickupablesSpawnPoint>  creatorPickupablesSpawnPoint, int amount)  : Event(EVENT_CREATE_PICKUPABLE)
{
	this->position = position;
	this->pickupableType = pickupableType;
	this->creatorPickupablesSpawnPoint = creatorPickupablesSpawnPoint;
	this->amount = amount;
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
	: Event(EVENT_SYNC_STATE_COMMAND)
{
	this->sender = sender;
	this->fromState = fromState;
	this->toState = toState;
	this->isReplacementState = isReplacementState;
}

Event_CreateExplosionSphere::Event_CreateExplosionSphere(Float3 position, float damage, int entityIdOfCreator, XKILL_Enums::AmmunitionType ammunitionType, XKILL_Enums::FiringModeType firingModeType) : Event(EVENT_CREATE_EXPLOSIONSPHERE)
{
	this->position = position;
	this->damage = damage;
	this->entityIdOfCreator = entityIdOfCreator;
	this->ammunitionType = ammunitionType;
	this->firingModeType = firingModeType;
}

Event_CreateWorld::Event_CreateWorld(Float3 position, Float4 rotation, unsigned int meshID) : Event(EVENT_CREATE_WORLD)
{
	this->position = position;
	this->rotation = rotation;
	this->meshID = meshID;
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

Event_CreateCorpse::Event_CreateCorpse(AttributePtr<Attribute_Player> ptr_player) : Event(EVENT_CREATE_CORPSE)
{
	this->ptr_player = ptr_player;
}

Event_TransferEventsToGame::Event_TransferEventsToGame(std::vector<Event*> events) : Event(EVENT_TRANSFER_EVENTS_TO_GAME)
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

Event_DrawBulletPhysicsDebugLines::Event_DrawBulletPhysicsDebugLines(std::vector<VertexPosColor>* debugLineVertices) : Event(EVENT_DRAW_BULLET_PHYSICS_DEBUG_LINES)
{
	this->debugLineVertices = debugLineVertices;
}

Event_ModifyPhysicsObject::Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData modifyWhatDataInPhysicsObjectData, void* data, AttributePtr<Attribute_Physics> ptr_physics) : Event(EVENT_MODIFY_PHYSICS_OBJECT)
{
	this->modifyWhatDataInPhysicsObjectData = modifyWhatDataInPhysicsObjectData;
	this->data = data;
	this->ptr_physics = ptr_physics;
}

Event_SetMouseLock::Event_SetMouseLock( bool isLock ) : Event(EVENT_SET_MOUSELOCK)
{
	this->isLock = isLock;
}
Event_HackActivated::Event_HackActivated(float time, XKILL_Enums::HackType hackType, AttributePtr<Attribute_Player> player) : 
	Event(EVENT_HACK_ACTIVATED)
{
	this->time = time;
	this->hackType = hackType;
	this->player = player;
}

Event_WindowMove::Event_WindowMove( Int2 pos, Int2 oldPos ) : Event(EVENT_WINDOW_MOVE)
{
	this->pos = pos;
	this->oldPos = oldPos;
}

Event_ClosestHitRayCast::Event_ClosestHitRayCast(Float3 from, Float3 to, short collisionFilterMask) : Event(EVENT_CLOSEST_HIT_RAY_CAST)
{
	this->from = from;
	this->to = to;
	this->collisionFilterMask = collisionFilterMask;
}

Event_AllHitsRayCast::Event_AllHitsRayCast(Float3 from, Float3 to, short collisionFilterMask) : Event(EVENT_ALL_HITS_RAY_CAST)
{
	this->from = from;
	this->to = to;
	this->collisionFilterMask = collisionFilterMask;
}

Event_PlayerExecuting::Event_PlayerExecuting(int executingPlayerIndex) : Event(EVENT_PLAYER_EXECUTING)
{
	this->executingPlayerIndex = executingPlayerIndex;
}

Event_GetFileList::Event_GetFileList(std::string filepath, std::string extension) : Event(EVENT_GET_FILE_LIST)
{
	this->filepath = filepath;
	this->extension = extension;
}

Event_LoadLevel::Event_LoadLevel(std::string levelName) : Event(EVENT_LOAD_LEVEL)
{
	this->levelName = levelName;
}

Event_SetFullscreen::Event_SetFullscreen( bool on ) : Event(EVENT_SHOW_FULLSCREEN)
{
	this->on = on;
}

Event_PostHudMessage::Event_PostHudMessage( std::string message, AttributePtr<Attribute_Player> ptr_subject_player /*= AttributePtr<Attribute_Player>() */ ) : Event(EVENT_POST_HUD_MESSAGE)
{
	this->message = message;
	this->ptr_subject_player = ptr_subject_player;
	this->receiver = RECEIVER_ONLY_SUBJECT;

	setStyle(STYLE_NORMAL);
}

void Event_PostHudMessage::setStyle( Style style )
{
	styleSheet = "color: rgba(255, 255, 255, 220);";


	if(style == STYLE_NORMAL){}
	if(style == STYLE_SUBTILE)
		styleSheet = "color: rgba(255, 255, 255, 100);";
	if(style == STYLE_WARNING)
		styleSheet = "color: rgba(255, 0, 0, 255); font-size:15pt;";


	// Add an additional black background
	styleSheet += "background-color: rgba(0, 0, 0, 50); padding: 2px 10px; font-size: 15px;";
	//styleSheet += "font-size: 30px; font-family: pxll; font-weight: normal;";
}

void Event_PostHudMessage::setHtmlMessage( std::string prefex, std::string subject, std::string suffix /*= ""*/, std::string description /*= ""*/ )
{
	std::string text_normal = "<span style='color: rgba(255, 255, 255, 240);'>";
	std::string text_subtile = "<span style='color: rgba(255, 255, 255, 100);'>";
	std::string text_end = "</span>";

	prefex = text_subtile + prefex + text_end;
	subject = text_normal + subject + text_end;
	suffix = text_subtile + suffix + text_end;
	if(description != "")
		description = text_subtile + "(" + text_subtile + description + text_end + text_subtile + ")" + text_end;

	message = prefex + " " + subject + " " + suffix + " " + description ;
}
