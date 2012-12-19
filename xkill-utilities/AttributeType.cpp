#include "AttributeType.h"
#include <DirectXMath.h>
#include <windows.h>

#include "MeshModel.h"

IAttribute::IAttribute()
{
}
IAttribute::~IAttribute()
{
}

PositionAttribute::PositionAttribute()
{
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;
}
PositionAttribute::~PositionAttribute()
{
}

SpatialAttribute::SpatialAttribute()
{
	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation.z = 0.0f;
	rotation.w = 1.0f;

	scale.x = 1.0f;
	scale.y = 1.0f;
	scale.z = 1.0f;
}
SpatialAttribute::~SpatialAttribute()
{
}

RenderAttribute::RenderAttribute()
{
	transparent		= false;
	tessellation	= false;
	meshID		= 0;
	textureID		= 0;
}
RenderAttribute::~RenderAttribute()
{
}

PhysicsAttribute::PhysicsAttribute()
{
	collisionResponse = true;
	added = false;
	alive = true;
	mass = 1.0f;
	
	meshID = 0;

	gravity = Float3(0.0f, -10.0f, 0.0f);
	
	isProjectile = false;
	isExplosionSphere = false;

	angularVelocity.x = 0; 
	angularVelocity.y = 0;
	angularVelocity.z = 0;
	linearVelocity.x = 0;
	linearVelocity.y = 0;
	linearVelocity.z = 0;
}
PhysicsAttribute::~PhysicsAttribute()
{
}

ProjectileAttribute::ProjectileAttribute()
{
	entityIdOfCreator = -1;
	currentLifeTimeLeft = 10.0f;
	explodeOnImnpact = false;
	explosionSphereRadius = 1.0f;
}
ProjectileAttribute::~ProjectileAttribute()
{
}

SoundAttribute::SoundAttribute()
{
}
SoundAttribute::~SoundAttribute()
{
}

CameraAttribute::CameraAttribute()
{
	ZeroMemory(&mat_projection, sizeof(mat_projection));
	ZeroMemory(&mat_view, sizeof(mat_view));
	aspect = 0;
	fov = 0.785f; 
	zFar = 40.0f;
	zNear = 0.01f;
}

CameraAttribute::~CameraAttribute()
{
}

InputAttribute::InputAttribute()
{
	fire = false;
	changeWeapon = false;
	ZeroMemory(&position,sizeof(position));
	ZeroMemory(&rotation,sizeof(rotation));
	DirectX::XMFLOAT3 test;
}
InputAttribute::~InputAttribute()
{
}
PlayerAttribute::PlayerAttribute()
{
	id = -1;
	priority = 0;
	cycleSteals = 0;
	totalExecutionTime = 0;
}
PlayerAttribute::~PlayerAttribute()
{
}

MeshAttribute::MeshAttribute()
{
	this->meshID		= 0;
	this->mesh			= nullptr;
	this->dynamic		= false;
}
MeshAttribute::MeshAttribute(
	unsigned int	id,
	MeshModel*		mesh,
	bool			dynamic)
{
	this->meshID		= id;
	this->mesh			= mesh;
	this->dynamic		= dynamic;
}
MeshAttribute::~MeshAttribute()
{
}
void MeshAttribute::clean()
{
}

HealthAttribute::HealthAttribute()
{
	startHealth = 10;
	health = 0;
}
HealthAttribute::~HealthAttribute()
{
}

DamageAttribute::DamageAttribute()
{
	damage = 1;
}
DamageAttribute::~DamageAttribute()
{
}

SpawnPointAttribute::SpawnPointAttribute()
{
	timeSinceLastSpawn = 0.0f;
	spawnArea = 0.0f;
}
SpawnPointAttribute::~SpawnPointAttribute()
{
}

WeaponStatsAttribute::WeaponStatsAttribute()
{
	setWeaponStats(BULLET, SINGLE);
}

void WeaponStatsAttribute::setWeaponStats(AmmunitionType ammunitionType, FiringMode firingMode)
{
	this->ammunitionType = ammunitionType;
	this->firingMode = firingMode;

	totalNrOfShots = 100;
	clipSize = 10;
	reloadTime = 0.0f;
	nrOfProjectilesForEachShot = 1;
	displacementSphereRadius = 0.0f;
	spreadConeRadius = 0.0f;
	isExplosive = false;

	switch(ammunitionType)
	{
	case BULLET: //One powerful accurate bullet.
		velocityOfEachProjectile = 2500.0f;
		damgeOfEachProjectile = 5;
		break;
	case SCATTER: //Many weak and less accurate bullets.
		velocityOfEachProjectile = 1000.0f;
		nrOfProjectilesForEachShot = 10;
		damgeOfEachProjectile = 2;
		displacementSphereRadius = 0.02f;
		spreadConeRadius = 0.2f;
		break;
	case EXPLOSIVE: //One powerful accurate exploding bullet.
		velocityOfEachProjectile = 500.0f;
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

void WeaponStatsAttribute::setWeaponToDebugMachineGun()
{
	totalNrOfShots = -1;
	nrOfShotsLeftInClip = clipSize;
	clipSize = 0.0f;
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

WeaponStatsAttribute::~WeaponStatsAttribute()
{
}