#include "AttributeType.h"
#include "AttributeManager.h"
#include <DirectXMath.h>
#include "DebugShape.h"
#include "XKILL_Enums.h"
#include "DataItem.h"

IAttribute::IAttribute()
{
}
IAttribute::~IAttribute()
{
}

DataItemList* IAttribute::getDataList()
{
	DataItemList* list = new DataItemList();
	list->add_NotSupported("NOT_IMPLEMENTED");
	return list;
}

void IAttribute::saveTo( DataItemList* list )
{
	// DO NOTHING
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

DataItemList* Attribute_Position::getDataList()
{
	DataItemList* list = new DataItemList();
	list->add(position, "position");
	return list;
}

void Attribute_Position::saveTo( DataItemList* list )
{
	list->get(&position);
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

DataItemList* Attribute_Spatial::getDataList()
{
	DataItemList* list = new DataItemList();
	list->add(&ptr_position,		"ptr_position");
	list->add(rotation,				"rotation");
	list->add(scale,				"scale");
	return list;
}

void Attribute_Spatial::saveTo( DataItemList* list )
{
	list->get(&ptr_position);
	list->get(&rotation);
	list->get(&scale);
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

DataItemList* Attribute_Render::getDataList()
{
	DataItemList* list = new DataItemList();
	list->add(&ptr_spatial,			"ptr_spatial");
	list->add(&ptr_bounding,		"ptr_bounding");
	list->add(meshID,				"meshID");
	list->add(textureID,			"textureID");
	list->add(transparent,			"transparent");
	list->add(tessellation,			"tessellation");
	list->add(culling.values[0],	"culling1");
	list->add(culling.values[1],	"culling2");
	return list;
}

void Attribute_Render::saveTo( DataItemList* list )
{
	list->get(&ptr_spatial);
	list->get(&ptr_bounding);
	list->get(&meshID);
	list->get(&textureID);
	list->get(&transparent);
	list->get(&tessellation);
	list->get(&culling.values[0]);
	list->get(&culling.values[1]);
}

Attribute_Physics::Attribute_Physics()
{
	collisionResponse = true;
	reloadDataIntoBulletPhysics = true;
	mass = 1.0f;
	
	meshID = -1;

	collisionFilterGroup = XKILL_Enums::PhysicsAttributeType::NOTHING;
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

DataItemList* Attribute_Physics::getDataList()
{
	DataItemList* list = new DataItemList();
	list->add(&ptr_spatial,					"ptr_spatial");
	list->add(&ptr_render,					"ptr_render");
	list->add(linearVelocity,				"linearVelocity");
	list->add(angularVelocity,				"angularVelocity");
	list->add(gravity,						"gravity");
	list->add(mass,							"mass");
	list->add(meshID,						"meshID");
	list->add(collisionFilterMask,			"collisionFilterMask");
	list->add(collisionResponse,			"collisionResponse");
	list->add(reloadDataIntoBulletPhysics,	"reloadDataIntoBulletPhysics");

	return list;
}

void Attribute_Physics::saveTo( DataItemList* list )
{
	list->get(&ptr_spatial);
	list->get(&ptr_render);
	list->get(&linearVelocity);
	list->get(&angularVelocity);
	list->get(&gravity);
	list->get(&mass);
	list->get(&meshID);
	list->get(&collisionFilterMask);
	list->get(&collisionResponse);
	list->get(&reloadDataIntoBulletPhysics);
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

DataItemList* Attribute_Projectile::getDataList()
{
	DataItemList* list = new DataItemList();
	list->add(&ptr_physics,	"ptr_physics");
	list->add(entityIdOfCreator,					"entityIdOfCreator");
	list->add(currentLifeTimeLeft,					"currentLifeTimeLeft");

	return list;
}

void Attribute_Projectile::saveTo( DataItemList* list )
{
	list->get(&ptr_physics);
	list->get(&entityIdOfCreator);
	list->get(&currentLifeTimeLeft);
}

Attribute_Light_Dir::Attribute_Light_Dir()
{

}
Attribute_Light_Dir::~Attribute_Light_Dir()
{
	//Do nothing.
}

DataItemList* Attribute_Light_Dir::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add(lightDir.ambient,		"ambient");
	list->add(lightDir.diffuse,		"diffuse");
	list->add(lightDir.specular,	"specular");
	list->add(lightDir.direction,	"direction");

	return list;
}

void Attribute_Light_Dir::saveTo( DataItemList* list )
{
	list->get(&lightDir.ambient);
	list->get(&lightDir.diffuse);
	list->get(&lightDir.specular);
	list->get(&lightDir.direction);
}

Attribute_Light_Point::Attribute_Light_Point()
{

}
Attribute_Light_Point::~Attribute_Light_Point()
{
	//Do nothing.
}

DataItemList* Attribute_Light_Point::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add(&ptr_position, 			"ptr_position");
	list->add(lightPoint.ambient,		"ambient");
	list->add(lightPoint.diffuse,		"diffuse");
	list->add(lightPoint.specular,		"specular");
	//list->add(lightPoint.pos,			"pos");
	list->add(lightPoint.range,			"range");
	list->add(lightPoint.attenuation,	"attenuation");

	return list;
}

void Attribute_Light_Point::saveTo( DataItemList* list )
{
	list->get(&ptr_position);
	list->get(&lightPoint.ambient);
	list->get(&lightPoint.diffuse);
	list->get(&lightPoint.specular);
	//list->get(&lightPoint.pos);
	list->get(&lightPoint.range);
	list->get(&lightPoint.attenuation);
}

Attribute_Light_Spot::Attribute_Light_Spot()
{

}
Attribute_Light_Spot::~Attribute_Light_Spot()
{
	//Do nothing.
}

DataItemList* Attribute_Light_Spot::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add(&ptr_position, 			"ptr_position");
	list->add(lightSpot.ambient,		"ambient");
	list->add(lightSpot.diffuse,		"diffuse");
	list->add(lightSpot.specular,		"specular");
	//list->add(lightSpot.pos,			"pos");
	list->add(lightSpot.range,			"range");
	list->add(lightSpot.direction,		"direction");
	list->add(lightSpot.spotPow,		"spotPow");
	list->add(lightSpot.attenuation,	"attenuation");

	return list;
}

void Attribute_Light_Spot::saveTo( DataItemList* list )
{
	list->get(&ptr_position);
	list->get(&lightSpot.ambient);
	list->get(&lightSpot.diffuse);
	list->get(&lightSpot.specular);
	//list->get(&lightSpot.pos);
	list->get(&lightSpot.range);
	list->get(&lightSpot.direction);
	list->get(&lightSpot.spotPow);
	list->get(&lightSpot.attenuation);
}

Attribute_Sound::Attribute_Sound()
{
}
Attribute_Sound::~Attribute_Sound()
{
}

DataItemList* Attribute_Sound::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add(&ptr_position, "ptr_position");

	return list;
}

void Attribute_Sound::saveTo( DataItemList* list )
{
	list->get(&ptr_position);
}

Attribute_SoundSettings::Attribute_SoundSettings()
{
	this->soundMuted = false;
	this->soundVolume = 1.0f;
}
Attribute_SoundSettings::~Attribute_SoundSettings()
{
}

DataItemList* Attribute_SoundSettings::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add(soundMuted, "Sound Muted");
	list->add(soundVolume, "Sound Volume");

	return list;
}

void Attribute_SoundSettings::saveTo( DataItemList* list )
{
	list->get(&soundMuted);
	list->get(&soundVolume);
}

Attribute_Camera::Attribute_Camera()
{
	mat_projection = Float4x4(0.0f);
	mat_view = Float4x4(0.0f);

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

DataItemList* Attribute_Camera::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add(&ptr_spatial,		"ptr_spatial");
	list->add(mat_view,			"mat_view");
	list->add(mat_projection,	"mat_projection");
	list->add(fieldOfView,		"fieldOfView");
	list->add(aspectRatio,		"aspectRatio");
	list->add(zNear,			"zNear");
	list->add(zFar,				"zFar");
	list->add(up,				"up");
	list->add(right,			"right");
	list->add(look,				"look");

	return list;
}

void Attribute_Camera::saveTo( DataItemList* list )
{
	list->get(&ptr_spatial);
	list->get(&mat_view);
	list->get(&mat_projection);
	list->get(&fieldOfView);
	list->get(&aspectRatio);
	list->get(&zNear);
	list->get(&zFar);
	list->get(&up);
	list->get(&right);
	list->get(&look);
}

void Attribute_Camera::syncSpatialWithAim()
{
	// Update rotation quaternion so others can read from it
	DirectX::XMMATRIX xm_view = DirectX::XMLoadFloat4x4((DirectX::XMFLOAT4X4*)&mat_view);
	xm_view = DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(xm_view), xm_view);
	DirectX::XMVECTOR xv_rot = DirectX::XMQuaternionRotationMatrix(xm_view);
	xv_rot = DirectX::XMQuaternionNormalize(xv_rot);
	DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)&ptr_spatial->rotation, xv_rot);
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

void Attribute_SplitScreen::saveTo( DataItemList* list )
{
	list->get(&ptr_camera);
	list->get(&ptr_player);
	list->get(&ssTopLeftX);
	list->get(&ssTopLeftY);
	list->get(&ssWidth);
	list->get(&ssHeight);
}

DataItemList* Attribute_SplitScreen::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add(&ptr_camera,	"ptr_camera");
	list->add(&ptr_player,	"ptr_player");
	list->add(ssTopLeftX,	"ssTopLeftX");
	list->add(ssTopLeftY,	"ssTopLeftY");
	list->add(ssWidth,		"ssWidth");
	list->add(ssHeight,		"ssHeight");

	return list;
}

float Attribute_SplitScreen::getAspectRatio()
{
	return (float)ssWidth/(float)ssHeight;
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
	reload = false;

	position = Float2(0.0f, 0.0f);
	rotation = Float2(0.0f, 0.0f);

	DirectX::XMFLOAT3 test;
}
Attribute_Input::~Attribute_Input()
{
}

DataItemList* Attribute_Input::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add(&ptr_physics,			"ptr_physics");
	list->add(position,				"position");
	list->add(rotation,				"rotation");
	list->add(fire,					"fire");
	list->add(changeAmmunitionType,	"changeAmmunitionType");
	list->add(changeFiringMode,		"changeFiringMode");

	return list;
}

void Attribute_Input::saveTo( DataItemList* list )
{
	list->get(&ptr_physics);
	list->get(&position);
	list->get(&rotation);
	list->get(&fire);
	list->get(&changeAmmunitionType);
	list->get(&changeFiringMode);
}

Attribute_InputDevice::Attribute_InputDevice()
{
	device = nullptr;
}
Attribute_InputDevice::~Attribute_InputDevice()
{
}

DataItemList* Attribute_InputDevice::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add_NotSupported("device");

	return list;
}

void Attribute_InputDevice::saveTo( DataItemList* list )
{
	list->get_NotSupported();
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
	jetpack = false;
	detectedAsDead = true;
	meshID_whenAlive = 0;
	meshID_whenDead = 0;
	currentSprintTime = 0;
	sprintTime = 2.0f;
	canSprint = true;
	sprintRechargeRate = 0.2f;
	executing = false;

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

void Attribute_Player::saveTo( DataItemList* list )
{
	list->get(&ptr_render);
	list->get(&ptr_input);
	list->get(&ptr_camera);
	list->get(&ptr_health);
	list->get(&ptr_weaponStats);
	list->get(&id);
	list->get(&priority);
	list->get(&cycleSteals);
	list->get(&totalExecutionTime);
	list->get(&currentSpeed);
	list->get(&walkSpeed);
	list->get(&sprintSpeed);
}

DataItemList* Attribute_Player::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add(&ptr_render,			"ptr_render");
	list->add(&ptr_input,			"ptr_input");
	list->add(&ptr_camera,			"ptr_camera");
	list->add(&ptr_health,			"ptr_health");
	list->add(&ptr_weaponStats,		"ptr_weaponStats");
	list->add(id,					"id");
	list->add(priority,				"priority");
	list->add(cycleSteals,			"cycleSteals");
	list->add(totalExecutionTime,	"totalExecutionTime");
	list->add(currentSpeed,			"priority");
	list->add(walkSpeed,			"walkSpeed");
	list->add(sprintSpeed,			"sprintSpeed");

	return list;
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

void Attribute_Mesh::saveTo( DataItemList* list )
{
	list->get(&meshID);
	list->get_NotSupported();
	list->get(&dynamic);
}

DataItemList* Attribute_Mesh::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add(meshID,		"meshID");
	list->add_NotSupported(	"mesh");
	list->add(dynamic,		"dynamic");

	return list;
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

void Attribute_Health::saveTo( DataItemList* list )
{
	list->get(&maxHealth);
	list->get(&health);
}

DataItemList* Attribute_Health::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add(maxHealth,	"maxHealth");
	list->add(health,		"health");

	return list;
}

Attribute_Damage::Attribute_Damage()
{
	damage = 1.0f;
}
Attribute_Damage::~Attribute_Damage()
{
}

void Attribute_Damage::saveTo( DataItemList* list )
{
	list->get(&damage);
	list->get(&owner_entityID);
}

DataItemList* Attribute_Damage::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add(damage,			"damage");
	list->add(owner_entityID,	"owner_entityID");

	return list;
}

Attribute_PlayerSpawnPoint::Attribute_PlayerSpawnPoint()
{
	secondsSinceLastSpawn = 0.0f;
	spawnArea = 0.0f;
}
Attribute_PlayerSpawnPoint::~Attribute_PlayerSpawnPoint()
{
}

void Attribute_PlayerSpawnPoint::saveTo( DataItemList* list )
{
	list->get(&ptr_position);
	list->get(&secondsSinceLastSpawn);
	list->get(&spawnArea);
}

DataItemList* Attribute_PlayerSpawnPoint::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add(&ptr_position, "ptr_position");
	list->add(secondsSinceLastSpawn,	"secondsSinceLastSpawn");
	list->add(spawnArea,			"spawnArea");

	return list;
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

void Attribute_PickupablesSpawnPoint::saveTo( DataItemList* list )
{
	list->get(&ptr_position);
	//list->get(&spawnPickupableType);
	list->get(&spawnDelayInSeconds);
	list->get(&secondsSinceLastSpawn);
	list->get(&secondsSinceLastPickup);
	list->get(&maxNrOfExistingSpawnedPickupables);
	list->get(&currentNrOfExistingSpawnedPickupables);
}

DataItemList* Attribute_PickupablesSpawnPoint::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add(&ptr_position, "ptr_position");
	//list->add(spawnPickupableType, "spawnPickupableType");
	list->add(spawnDelayInSeconds, "spawnDelayInSeconds");
	list->add(secondsSinceLastSpawn, "secondsSinceLastSpawn");
	list->add(secondsSinceLastPickup, "secondsSinceLastPickup");
	list->add(maxNrOfExistingSpawnedPickupables, "maxNrOfExistingSpawnedPickupables");
	list->add(currentNrOfExistingSpawnedPickupables, "currentNrOfExistingSpawnedPickupables");


	return list;
}

Attribute_Pickupable::Attribute_Pickupable()
{
	pickupableType = XKILL_Enums::PickupableType::MEDKIT;
}
Attribute_Pickupable::~Attribute_Pickupable()
{
}

void Attribute_Pickupable::saveTo( DataItemList* list )
{
	list->get(&ptr_position);
	list->get(&ptr_physics);
	list->get(&ptr_pickupablesSpawnPoint_creator);
	//list->get_AttributePointer(&pickupableType);
	list->get(&amount);
}

DataItemList* Attribute_Pickupable::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add(&ptr_position, 						"ptr_position");
	list->add(&ptr_physics, 							"ptr_physics");
	list->add(&ptr_pickupablesSpawnPoint_creator, 	"ptr_creatorPickupablesSpawnPoint");
	//list->add_AttributePointer(pickupableType, "pickupableType");
	list->add(amount, "amount");

	return list;
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

	list->add_Enum(currentAmmunitionType,											"ammunitionType");
	list->add_Enum(currentFiringModeType,											"firingMode");

	list->add(ammunition[currentAmmunitionType].currentTotalNrOfShots,				"currentTotalNrOfShots");
	list->add(firingMode[currentFiringModeType].clipSize,							"clipSize");
	//list->add(firingMode[currentFiringModeType].nrOfShotsLeftInClip,				"nrOfShotsLeftInClip");

	list->add(firingMode[currentFiringModeType].reloadTime,							"reloadTime");
	list->add(firingMode[currentFiringModeType].reloadTimeLeft,						"reloadTimeLeft");
	list->add(firingMode[currentFiringModeType].cooldownBetweenShots,				"cooldownBetweenShots");
	list->add(firingMode[currentFiringModeType].cooldownLeft,						"cooldownLeft");

	list->add(ammunition[currentAmmunitionType].speed,								"speed");
	list->add(ammunition[currentAmmunitionType].nrOfProjectilesPerSalvo,			"nrOfProjectilesPerSalvo");
	list->add(ammunition[currentAmmunitionType].damage,								"damage");

	list->add(ammunition[currentAmmunitionType].displacementSphereRadius,			"displacementSphereRadius");
	list->add(ammunition[currentAmmunitionType].spreadConeRadius,					"spreadConeRadius");

	list->add(ammunition[currentAmmunitionType].explosive,							"explosive");
	list->add(ammunition[currentAmmunitionType].explosionSphereInitialRadius,		"explosionSphereInitialRadius");
	list->add(ammunition[currentAmmunitionType].explosionSphereFinalRadius,			"explosionSphereFinalRadius");
	list->add(ammunition[currentAmmunitionType].explosionSphereExplosionDuration,	"explosionSphereExplosionDuration");
	
	return list;
}
void Attribute_WeaponStats::saveTo(DataItemList* list)
{
	currentAmmunitionType	= (XKILL_Enums::AmmunitionType)	list->get_Enum();
	currentFiringModeType	= (XKILL_Enums::FiringModeType)	list->get_Enum();

	list->get(&ammunition[currentAmmunitionType].currentTotalNrOfShots);
	list->get(&firingMode[currentFiringModeType].clipSize);
	//list->get(&firingMode[currentFiringModeType].nrOfShotsLeftInClip);

	list->get(&firingMode[currentFiringModeType].reloadTime);
	list->get(&firingMode[currentFiringModeType].reloadTimeLeft);
	list->get(&firingMode[currentFiringModeType].cooldownBetweenShots);
	list->get(&firingMode[currentFiringModeType].cooldownLeft);
	
	list->get(&ammunition[currentAmmunitionType].speed);
	list->get(&ammunition[currentAmmunitionType].nrOfProjectilesPerSalvo);
	list->get(&ammunition[currentAmmunitionType].damage);
	
	list->get(&ammunition[currentAmmunitionType].displacementSphereRadius);
	list->get(&ammunition[currentAmmunitionType].spreadConeRadius);
		
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

DataItemList* Attribute_ExplosionSphere::getDataList()
{
	DataItemList* list = new DataItemList();
	list->add(&ptr_physics, "ptr_physics");;
	list->add(currentLifeTimeLeft,	"currentLifeTimeLeft");
	return list;
}

void Attribute_ExplosionSphere::saveTo( DataItemList* list )
{
	list->get(&ptr_physics);
	list->get(&currentLifeTimeLeft);
}

Attribute_Ray::Attribute_Ray()
{
	from = Float3(0.0f, 0.0f, 0.0f);
	to = Float3(0.0f, 0.0f, 0.0f);
}
Attribute_Ray::~Attribute_Ray()
{
}

DataItemList* Attribute_Ray::getDataList()
{
	DataItemList* list = new DataItemList();
	list->add(from, "from");;
	list->add(to,	"to");
	return list;
}

void Attribute_Ray::saveTo( DataItemList* list )
{
	list->get(&from);
	list->get(&to);
}

void Behavior_Offset::updateOffset()
{
	using namespace DirectX;

	if(!ptr_parent_spatial_rotation.isEmpty())
	{
		// Fetch attributes
		Float4 parent_rot = ptr_parent_spatial_rotation->rotation;
		Float4 own_rot = ptr_spatial->rotation;
		XMVECTOR xv_own_rot =  XMLoadFloat4(( XMFLOAT4*)&own_rot);
		//
		// Add rotation offset relative to parent
		//

		XMVECTOR xv_rot_offset =  XMLoadFloat4(( XMFLOAT4*)&offset_rotation);
		XMVECTOR parent_xv_rot =  XMLoadFloat4(( XMFLOAT4*)&parent_rot);
		xv_rot_offset =  XMQuaternionMultiply(xv_rot_offset, parent_xv_rot);



		// Slerp interpolate to smooth out moment
		float MAGIC_SLERP_NUMBER = 1.0f;
		xv_rot_offset = XMQuaternionSlerp(xv_rot_offset, xv_own_rot, MAGIC_SLERP_NUMBER*ATTRIBUTE_MANAGER->settings->trueDeltaTime);

		Float4 rot_offset;  XMStoreFloat4(( XMFLOAT4*)&rot_offset, xv_rot_offset);
		ptr_spatial->rotation = rot_offset;
	}

	// Make sure we have a parent
	if(!ptr_parent_spatial_position.isEmpty())
	{
		// Fetch attributes from parent
		Float4 parent_rot = ptr_parent_spatial_position->rotation;
		Float3 parent_pos = ptr_parent_spatial_position->ptr_position->position;


		//
		// Add translation offset relative to parent
		//

		XMVECTOR xv_pos = XMLoadFloat3(( XMFLOAT3*)&offset_position);
		XMVECTOR parent_xv_rot = XMLoadFloat4(( XMFLOAT4*)&parent_rot);
		XMVECTOR xv_pos_offset = XMVector3Rotate(xv_pos, parent_xv_rot);
		Float3 pos_offset;  XMStoreFloat3(( XMFLOAT3*)&pos_offset, xv_pos_offset);


		//
		// Add position translation relative to parent
		//

		pos_offset = parent_pos + pos_offset;
		ptr_spatial->ptr_position->position = pos_offset;
	}

	
}

DataItemList* Behavior_Offset::getDataList()
{
	DataItemList* list = new DataItemList();
	list->add(&ptr_parent_spatial_position,			"ptr_parent_spatial_position");
	list->add(&ptr_parent_spatial_rotation,			"ptr_parent_spatial_rotation");
	list->add(offset_position,						"offset_position");
	list->add(offset_rotation,						"offset_rotation");
	return list;
}
void Behavior_Offset::saveTo( DataItemList* list )
{
	list->get(&ptr_parent_spatial_position);
	list->get(&ptr_parent_spatial_rotation);
	list->get(&offset_position);
	list->get(&offset_rotation);
}

DataItemList* Attribute_Bounding::getDataList()
{
	DataItemList* list = new DataItemList();
	list->add_NotSupported("boxPoints");
	list->add_NotSupported("bonvexPoints");
	return list;
}
