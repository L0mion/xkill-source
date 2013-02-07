#include "AttributeType.h"
#include <DirectXMath.h>
#include <windows.h>

#include "DebugShape.h"
#include "XKILL_Enums.h"

IAttribute::IAttribute()
{
}
IAttribute::~IAttribute()
{
}

Attribute_Position::Attribute_Position()
{
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;
}
Attribute_Position::~Attribute_Position()
{
}

Attribute_Spatial::Attribute_Spatial()
{
	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation.z = 0.0f;
	rotation.w = 1.0f;

	scale.x = 1.0f;
	scale.y = 1.0f;
	scale.z = 1.0f;
}
Attribute_Spatial::~Attribute_Spatial()
{
}

Attribute_Render::Attribute_Render()
{
	transparent			= false;
	tessellation		= false;

	meshID			= 0;
	textureID		= 0;
}
Attribute_Render::~Attribute_Render()
{
}

Attribute_Physics::Attribute_Physics()
{
	collisionResponse = true;
	reloadDataIntoBulletPhysics = true;
	mass = 1.0f;
	
	meshID = -1;

	collisionFilterGroup = Attribute_Physics::NOTHING;
	collisionFilterMask = 0;

	gravity = Float3(0.0f, -10.0f, 0.0f);

	angularVelocity.x = 0; 
	angularVelocity.y = 0;
	angularVelocity.z = 0;
	linearVelocity.x = 0;
	linearVelocity.y = 0;
	linearVelocity.z = 0;
}
Attribute_Physics::~Attribute_Physics()
{
}

Attribute_Projectile::Attribute_Projectile()
{
	entityIdOfCreator = -1;
	totalLifeTime = 10.0f;
	currentLifeTimeLeft = totalLifeTime;
	ammunitionType = XKILL_Enums::AmmunitionType::BULLET;
	firingModeType = XKILL_Enums::FiringModeType::SEMI;
	scatterDropped = false;
}
Attribute_Projectile::~Attribute_Projectile()
{
}

Attribute_Light_Dir::Attribute_Light_Dir()
{

}
Attribute_Light_Dir::~Attribute_Light_Dir()
{
	//Do nothing.
}
Attribute_Light_Point::Attribute_Light_Point()
{

}
Attribute_Light_Point::~Attribute_Light_Point()
{
	//Do nothing.
}
Attribute_Light_Spot::Attribute_Light_Spot()
{

}
Attribute_Light_Spot::~Attribute_Light_Spot()
{
	//Do nothing.
}

Attribute_Sound::Attribute_Sound()
{
}
Attribute_Sound::~Attribute_Sound()
{
}

Attribute_SoundSettings::Attribute_SoundSettings()
{
	this->soundMuted = false;
	this->soundVolume = 1.0f;
}
Attribute_SoundSettings::~Attribute_SoundSettings()
{
}

Attribute_Camera::Attribute_Camera()
{
	ZeroMemory(&mat_projection, sizeof(mat_projection));
	ZeroMemory(&mat_view, sizeof(mat_view));

	aspectRatio = 0.785f;
	fieldOfView = 0.785f; 
	zFar = 40.0f;
	zNear = 0.01f;

	up		= Float3(0.0f, 1.0f, 0.0f);
	right	= Float3(1.0f, 0.0f, 0.0f);
	look	= Float3(0.0f, 0.0f, 1.0f);
}
Attribute_Camera::~Attribute_Camera()
{
}

Attribute_SplitScreen::Attribute_SplitScreen()
{
	ssTopLeftX = 0;
	ssTopLeftY = 0;

	ssWidth		= 0;
	ssHeight	= 0;
}
Attribute_SplitScreen::~Attribute_SplitScreen()
{
	//Do nothing.
}

Attribute_Input::Attribute_Input()
{
	jump = false;
	jetpack = false;
	sprint = false;
	killPlayer = false;
	fire = false;
	firePressed = false;
	changeAmmunitionType = false;
	changeFiringMode = false;
	lowSensitivity = false;
	ZeroMemory(&position,sizeof(position));
	ZeroMemory(&rotation,sizeof(rotation));
	DirectX::XMFLOAT3 test;
}
Attribute_Input::~Attribute_Input()
{
}

Attribute_InputDevice::Attribute_InputDevice()
{
	device = nullptr;
}
Attribute_InputDevice::~Attribute_InputDevice()
{
}

int Attribute_Player::nextId = 0;
Attribute_Player::Attribute_Player()
{
	id = nextId++;
	priority = 0;
	cycleSteals = 0;
	totalExecutionTime = 0;
	respawnDelay = 5.0f;
	currentRespawnDelay = 0.0f;
	delayInSecondsBetweenEachJump = 1.0f;
	timeSinceLastJump = delayInSecondsBetweenEachJump+1.0f;
	collidingWithWorld = false;
	timeSinceLastDamageTaken = 100.0f;
	jetpackTimer = 0.0f;
	detectedAsDead = true;
	meshID_whenAlive = 0;
	meshID_whenDead = 0;
	currentSprintTime = 0;
	sprintTime = 2.0f;
	canSprint = true;
	sprintRechargeRate = 0.2f;

	walkSpeed = 5.0f;
	sprintSpeed = walkSpeed*2;
	currentSpeed = walkSpeed;
}
Attribute_Player::~Attribute_Player()
{
}
void Attribute_Player::clean()
{
	nextId = 0;
}

Attribute_Mesh::Attribute_Mesh()
{
	this->meshID		= 0;
	this->dynamic		= false;
	this->fileName		= "unknown";
	this->vertexType	= VERTEX_INVALID;
}
Attribute_Mesh::Attribute_Mesh(
	unsigned int	id,
	MeshDesc		mesh,
	bool			dynamic,
	std::string		fileName,
	VertexType		vertexType)
{
	this->meshID		= id;
	this->mesh			= mesh;
	this->dynamic		= dynamic;
	this->fileName		= fileName;
	this->vertexType	= vertexType;
}
Attribute_Mesh::~Attribute_Mesh()
{
}
void Attribute_Mesh::clean()
{
	//if(mesh)
	//{
	//	delete mesh;
	//	mesh = nullptr;
	//}
}

Attribute_Health::Attribute_Health()
{
	maxHealth = 100.0f;
	health = 0.0f;
	healthFromLastFrame = 0.0f;
}
Attribute_Health::~Attribute_Health()
{
}

Attribute_Damage::Attribute_Damage()
{
	damage = 1.0f;
}
Attribute_Damage::~Attribute_Damage()
{
}

Attribute_PlayerSpawnPoint::Attribute_PlayerSpawnPoint()
{
	secondsSinceLastSpawn = 0.0f;
	spawnArea = 0.0f;
}
Attribute_PlayerSpawnPoint::~Attribute_PlayerSpawnPoint()
{
}

Attribute_PickupablesSpawnPoint::Attribute_PickupablesSpawnPoint()
{
	spawnPickupableType = XKILL_Enums::PickupableType::MEDKIT;
	spawnDelayInSeconds = 0.0f;
	secondsSinceLastSpawn = 0.0f;
	secondsSinceLastPickup = 0.0f;
	maxNrOfExistingSpawnedPickupables = 1;
	currentNrOfExistingSpawnedPickupables = 0;
}
Attribute_PickupablesSpawnPoint::~Attribute_PickupablesSpawnPoint()
{
}

Attribute_Pickupable::Attribute_Pickupable()
{
	pickupableType = XKILL_Enums::PickupableType::MEDKIT;
}
Attribute_Pickupable::~Attribute_Pickupable()
{
}

#include "WeaponStructs.h"
#include "MutatorSettings.h"
Attribute_WeaponStats::Attribute_WeaponStats()
{
	MutatorSettings ms;

	for(int i = 0; i < XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES; i++)
	{
		for(int j = 0; j < XKILL_Enums::FiringModeType::NROFFIRINGMODETYPES; j++)
		{
			ms.setupAttribute(this, static_cast<XKILL_Enums::AmmunitionType>(i), static_cast<XKILL_Enums::FiringModeType>(j));
		}
	}
}
Attribute_WeaponStats::~Attribute_WeaponStats()
{

}
void Attribute_WeaponStats::setWeaponStats(XKILL_Enums::AmmunitionType ammunitionType, XKILL_Enums::FiringModeType firingModeType)
{
	currentAmmunitionType = ammunitionType;
	currentFiringModeType = firingModeType;

	MutatorSettings ms;
	ms.setupAttribute(this);

	//totalNrOfShots = 1000;
	//clipSize = 10;
	//reloadTime = 0.0f;
	//nrOfProjectilesForEachShot = 1;
	//displacementSphereRadius = 0.0f;
	//spreadConeRadius = 0.0f;
	//isExplosive = false;
	//velocityDifference = 0.0f;
	//
	//switch(ammunitionType)
	//{
	//case BULLET: //One powerful accurate bullet.
	//	velocityOfEachProjectile = 40.0f;
	//	damgeOfEachProjectile = 5;
	//	break;
	//case SCATTER: //Many weak and less accurate bullets.
	//	velocityOfEachProjectile = 10.0f;
	//	nrOfProjectilesForEachShot = 10;
	//	damgeOfEachProjectile = 2;
	//	displacementSphereRadius = 0.02f;
	//	spreadConeRadius = 0.2f;
	//	velocityDifference = 0.5f;
	//	break;
	//case EXPLOSIVE: //One powerful accurate exploding bullet.
	//	velocityOfEachProjectile = 5.0f;
	//	damgeOfEachProjectile = 10;
	//	explosionSphereRadius = 1.0f;
	//	isExplosive = true;
	//	break;
	//}
	//
	//switch(firingMode)
	//{
	//case SINGLE: //Reload after each shot. Fast reload time.
	//	cooldownBetweenShots = 0.0f;
	//	reloadTime = 1.0f;
	//	clipSize = 1;
	//	break;
	//case SEMI: //Reload after a number of shots. Medium load time.
	//	cooldownBetweenShots = 0.5f;
	//	reloadTime = 2.0f;
	//	explosionSphereRadius *= 0.5f;
	//	clipSize = 10;
	//	break;
	//case AUTO: //Reload after a large number of shots. Long reload time.
	//	cooldownBetweenShots = 0.1f;
	//	reloadTime = 3.0f;
	//	explosionSphereRadius *= 0.15f;
	//	clipSize = 50;
	//	break;
	//}
	//
	//cooldownLeft = cooldownBetweenShots;
	//reloadTimeLeft = reloadTime;
	//
	//nrOfShotsLeftInClip = clipSize;
}
std::string Attribute_WeaponStats::getAmmunitionTypeAsString()
{
	std::string ammunitionTypeAsString = "Error in std::string Attribute_WeaponStats::getAmmunitionTypeAsString()";
	switch(currentAmmunitionType)
	{
		case XKILL_Enums::AmmunitionType::BULLET:
			ammunitionTypeAsString = "Bullet";
			break;
		case XKILL_Enums::AmmunitionType::SCATTER:
			ammunitionTypeAsString = "Scatter";
			break;
		case XKILL_Enums::AmmunitionType::EXPLOSIVE:
			ammunitionTypeAsString = "Explosive";
			break;
	}
	return ammunitionTypeAsString;
}
std::string Attribute_WeaponStats::getFiringModeAsString()
{
	std::string firingModeAsString = "Error in std::string Attribute_WeaponStats::getFiringModesString()";
	switch(currentFiringModeType)
	{
		case XKILL_Enums::FiringModeType::SINGLE:
			firingModeAsString = "Single";
			break;
		case XKILL_Enums::FiringModeType::SEMI:
			firingModeAsString = "Semi";
			break;
		case XKILL_Enums::FiringModeType::AUTO:
			firingModeAsString = "Auto";
			break;
	}
	return firingModeAsString;
}
DataItemList* Attribute_WeaponStats::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add_Enum(currentAmmunitionType,								"ammunitionType");
	list->add_Enum(currentFiringModeType,								"firingMode");

	list->add(ammunition[currentAmmunitionType].totalNrOfShots,			"totalNrOfShots");
	list->add(firingMode[currentFiringModeType].clipSize,				"clipSize");
	list->add(firingMode[currentFiringModeType].nrOfShotsLeftInClip,	"nrOfShotsLeftInClip");

	list->add(firingMode[currentFiringModeType].reloadTime,				"reloadTime");
	list->add(firingMode[currentFiringModeType].reloadTimeLeft,			"reloadTimeLeft");
	list->add(firingMode[currentFiringModeType].cooldownBetweenShots,	"cooldownBetweenShots");
	list->add(firingMode[currentFiringModeType].cooldownLeft,			"cooldownLeft");

	list->add(ammunition[currentAmmunitionType].speed,					"velocityOfEachProjectile");
	list->add(ammunition[currentAmmunitionType].nrOfProjectiles,		"nrOfProjectilesForEachShot");
	list->add(ammunition[currentAmmunitionType].damage,					"damageOfEachProjectile");

	list->add(ammunition[currentAmmunitionType].spawnVariation,			"displacementSphereRadius");
	list->add(ammunition[currentAmmunitionType].spread,					"spreadConeRadius");

	list->add(ammunition[currentAmmunitionType].explosive,								"isExplosive");
	list->add(ammunition[currentAmmunitionType].explosionSphereInitialRadius,			"explosionSphereInitialRadius");
	list->add(ammunition[currentAmmunitionType].explosionSphereFinalRadius,				"explosionSphereFinalRadius");
	list->add(ammunition[currentAmmunitionType].explosionSphereExplosionDuration,		"explosionSphereExplosionDuration");
	
	return list;
}
void Attribute_WeaponStats::saveTo(DataItemList* list)
{
	currentAmmunitionType	= (XKILL_Enums::AmmunitionType)	list->get_Enum();
	currentFiringModeType	= (XKILL_Enums::FiringModeType)	list->get_Enum();

	list->get(&ammunition[currentAmmunitionType].totalNrOfShots);
	list->get(&firingMode[currentFiringModeType].clipSize);
	list->get(&firingMode[currentFiringModeType].nrOfShotsLeftInClip);

	list->get(&firingMode[currentFiringModeType].reloadTime);
	list->get(&firingMode[currentFiringModeType].reloadTimeLeft);
	list->get(&firingMode[currentFiringModeType].cooldownBetweenShots);
	list->get(&firingMode[currentFiringModeType].cooldownLeft);
	
	list->get(&ammunition[currentAmmunitionType].speed);
	list->get(&ammunition[currentAmmunitionType].nrOfProjectiles);
	list->get(&ammunition[currentAmmunitionType].damage);
	
	list->get(&ammunition[currentAmmunitionType].spawnVariation);
	list->get(&ammunition[currentAmmunitionType].spread);
		
	list->get(&ammunition[currentAmmunitionType].explosive);
	list->get(&ammunition[currentAmmunitionType].explosionSphereInitialRadius);
	list->get(&ammunition[currentAmmunitionType].explosionSphereFinalRadius);
	list->get(&ammunition[currentAmmunitionType].explosionSphereExplosionDuration);
}

Attribute_DebugShape::Attribute_DebugShape()
{
	shape	= nullptr;
	render	= false;
}
Attribute_DebugShape::~Attribute_DebugShape()
{
	//Do nothing.
}
void Attribute_DebugShape::clean()
{
	if(shape)
	{
		delete shape;
		shape = nullptr;
	}
	render = false;
}

DataItemList* Attribute_DebugShape::getDataList()
	{
		DataItemList* list = new DataItemList();

		list->add(&ptr_spatial, "ptr_spatial");;
		list->add(meshID,					"meshID");
		list->add_Enum(shape->shapeType_,	"shape->shapeType");
		list->add(render,					"render");

		return list;
	}

void Attribute_DebugShape::saveTo( DataItemList* list )
{
	list->get(&ptr_spatial);
	list->get(&meshID);
	shape->shapeType_ = (DebugShapeType)list->get_Enum();
	list->get(&render);
}

Attribute_ExplosionSphere::Attribute_ExplosionSphere()
{
	currentLifeTimeLeft = 1.0f;
	currentRadius = 0.0f;
	ammunitionType = XKILL_Enums::AmmunitionType::EXPLOSIVE;
	firingModeType = XKILL_Enums::FiringModeType::SINGLE;
}
Attribute_ExplosionSphere::~Attribute_ExplosionSphere()
{
}

Attribute_Ray::Attribute_Ray()
{
	from = Float3(0.0f, 0.0f, 0.0f);
	to = Float3(0.0f, 0.0f, 0.0f);
}
Attribute_Ray::~Attribute_Ray()
{
}

void Behavior_Offset::updateOffset()
{
	// Make sure we have a parent
	if(ptr_parent_spatial_position.isNotEmpty())
	{
		// Fetch attributes from parent
		Float4 parent_rot = ptr_parent_spatial_position->rotation;
		Float3 parent_pos = ptr_parent_spatial_position->ptr_position->position;


		//
		// Add translation offset relative to parent
		//

		DirectX::XMVECTOR xv_pos = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&offset_position);
		DirectX::XMVECTOR parent_xv_rot = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&parent_rot);
		DirectX::XMVECTOR xv_pos_offset = DirectX::XMVector3Rotate(xv_pos, parent_xv_rot);
		Float3 pos_offset; DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)&pos_offset, xv_pos_offset);


		//
		// Add position translation relative to parent
		//

		pos_offset = parent_pos + pos_offset;
		ptr_spatial->ptr_position->position = pos_offset;

	}

	if(ptr_parent_spatial_rotation.isNotEmpty())
	{
		// Fetch attributes from parent
		Float4 parent_rot = ptr_parent_spatial_rotation->rotation;

		//
		// Add rotation offset relative to parent
		//

		DirectX::XMVECTOR xv_rot_offset = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&offset_rotation);
		DirectX::XMVECTOR parent_xv_rot = DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)&parent_rot);
		xv_rot_offset = DirectX::XMQuaternionMultiply(xv_rot_offset, parent_xv_rot);
		Float4 rot_offset; DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)&rot_offset, xv_rot_offset);

		ptr_spatial->rotation = rot_offset;
	}
}
