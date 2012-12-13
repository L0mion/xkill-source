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
	added = false;
	alive = true;
	mass = 1.0f;
	
	meshID = 0;

	gravity = Float3(0.0f, -10.0f, 0.0f);
	
	isProjectile = false;

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
