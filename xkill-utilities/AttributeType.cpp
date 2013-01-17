#include "AttributeType.h"
#include <DirectXMath.h>
#include <windows.h>

#include "MeshModel.h"
#include "DebugShape.h"

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
	alive = true;
	mass = 1.0f;
	
	meshID = 0;

	collisionFilterGroup = Attribute_Physics::DEFAULT_ERROR;
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
	currentLifeTimeLeft = 10.0f;
	explodeOnImnpact = false;
	explosionSphereRadius = 1.0f;
}
Attribute_Projectile::~Attribute_Projectile()
{
}

//Attribute_Light::Attribute_Light()
//{
//	lightType = LIGHTTYPE_NA;
//
//	direction	= Float3(0.0f, 0.0f, 0.0f);
//	attenuation	= Float3(0.0f, 0.0f, 0.0f);
//
//	ambient		= Float4(0.0f, 0.0f, 0.0f, 0.0f);
//	diffuse		= Float4(0.0f, 0.0f, 0.0f, 0.0f);
//	specular	= Float4(0.0f, 0.0f, 0.0f, 0.0f);
//
//	range		= 0.0f;
//	spotPower	= 0.0f;
//}
//Attribute_Light::~Attribute_Light()
//{
//	//Do nothing.
//}

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

Attribute_Camera::Attribute_Camera()
{
	ZeroMemory(&mat_projection, sizeof(mat_projection));
	ZeroMemory(&mat_view, sizeof(mat_view));
	aspect = 0;
	fov = 0.785f; 
	zFar = 40.0f;
	zNear = 0.01f;
	reset = false;
}
Attribute_Camera::~Attribute_Camera()
{
}

Attribute_Input::Attribute_Input()
{
	jump = false;
	sprint = false;
	killPlayer = false;
	fire = false;
	changeAmmunitionType = false;
	changeAmmunitionType = false;
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
	this->mesh			= nullptr;
	this->dynamic		= false;
}
Attribute_Mesh::Attribute_Mesh(
	unsigned int	id,
	MeshModel*		mesh,
	bool			dynamic)
{
	this->meshID		= id;
	this->mesh			= mesh;
	this->dynamic		= dynamic;
}
Attribute_Mesh::~Attribute_Mesh()
{
}
void Attribute_Mesh::clean()
{
	if(mesh)
	{
		delete mesh;
		mesh = nullptr;
	}
}

Attribute_Health::Attribute_Health()
{
	startHealth = 10.0f;
	health = 0.0f;
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

Attribute_SpawnPoint::Attribute_SpawnPoint()
{
	timeSinceLastSpawn = 0.0f;
	spawnArea = 0.0f;
}
Attribute_SpawnPoint::~Attribute_SpawnPoint()
{
}

Attribute_WeaponStats::Attribute_WeaponStats()
{
	setWeaponStats(EXPLOSIVE, AUTO);
}

void Attribute_WeaponStats::setWeaponStats(AmmunitionType ammunitionType, FiringMode firingMode)
{
	this->ammunitionType = ammunitionType;
	this->firingMode = firingMode;

	totalNrOfShots = 1000;
	clipSize = 10;
	reloadTime = 0.0f;
	nrOfProjectilesForEachShot = 1;
	displacementSphereRadius = 0.0f;
	spreadConeRadius = 0.0f;
	isExplosive = false;
	velocityDifference = 0.0f;

	switch(ammunitionType)
	{
	case BULLET: //One powerful accurate bullet.
		velocityOfEachProjectile = 40.0f;
		damgeOfEachProjectile = 5;
		break;
	case SCATTER: //Many weak and less accurate bullets.
		velocityOfEachProjectile = 10.0f;
		nrOfProjectilesForEachShot = 10;
		damgeOfEachProjectile = 2;
		displacementSphereRadius = 0.02f;
		spreadConeRadius = 0.2f;
		velocityDifference = 0.5f;
		break;
	case EXPLOSIVE: //One powerful accurate exploding bullet.
		velocityOfEachProjectile = 5.0f;
		damgeOfEachProjectile = 10;
		explosionSphereRadius = 1.0f;
		isExplosive = true;
		break;
	}

	switch(firingMode)
	{
	case SINGLE: //Reload after each shot. Fast reload time.
		cooldownBetweenShots = 0.0f;
		reloadTime = 1.0f;
		clipSize = 1;
		break;
	case SEMI: //Reload after a number of shots. Medium load time.
		cooldownBetweenShots = 0.5f;
		reloadTime = 2.0f;
		explosionSphereRadius *= 0.5f;
		clipSize = 10;
		break;
	case AUTO: //Reload after a large number of shots. Long reload time.
		cooldownBetweenShots = 0.1f;
		reloadTime = 3.0f;
		explosionSphereRadius *= 0.15f;
		clipSize = 50;
		break;
	}

	cooldownLeft = cooldownBetweenShots;
	reloadTimeLeft = reloadTime;

	nrOfShotsLeftInClip = clipSize;
}
void Attribute_WeaponStats::setWeaponToDebugMachineGun()
{
	totalNrOfShots = -1;
	nrOfShotsLeftInClip = clipSize;
	clipSize = 0;
	cooldownLeft = 0.0f;
	reloadTime = 0.0f;

	nrOfProjectilesForEachShot = 1;
	displacementSphereRadius = 0.0f;
	spreadConeRadius = 0.0f;

	velocityOfEachProjectile = 2500.0f;
	damgeOfEachProjectile = 1;
	explosionSphereRadius = 0.0f;
	cooldownBetweenShots = 0.0f;
}

std::string Attribute_WeaponStats::getAmmunitionTypeAsString()
{
	std::string ammunitionTypeAsString = "Error in std::string Attribute_WeaponStats::getAmmunitionTypeAsString()";
	switch(this->ammunitionType)
	{
		case BULLET:
			ammunitionTypeAsString = "Bullet";
			break;
		case SCATTER:
			ammunitionTypeAsString = "Scatter";
			break;
		case EXPLOSIVE:
			ammunitionTypeAsString = "Explosive";
			break;
	}
	return ammunitionTypeAsString;
}
std::string Attribute_WeaponStats::getFiringModeAsString()
{
	std::string firingModeAsString = "Error in std::string Attribute_WeaponStats::getFiringModesString()";
	switch(this->firingMode)
	{
		case SINGLE:
			firingModeAsString = "Single";
			break;
		case SEMI:
			firingModeAsString = "Semi";
			break;
		case AUTO:
			firingModeAsString = "Auto";
			break;
	}
	return firingModeAsString;
}

Attribute_WeaponStats::~Attribute_WeaponStats()
{
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
		list->add_AttributePointer(ptr_spatial.index, "ptr_Spatial");;
		list->add((int)meshID,			"MeshID");
		list->add(shape->shapeType_,	"Shape->ShapeType");
		list->add(render,				"Render");
		return list;
	}

Attribute_ExplosionSphere::Attribute_ExplosionSphere()
{
	currentLifeTimeLeft = 1.0f;
}
Attribute_ExplosionSphere::~Attribute_ExplosionSphere()
{
}