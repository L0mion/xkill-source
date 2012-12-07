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
	meshIndex		= 0;
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
	collisionShapeIndex = 0;
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
	owner = -1;
	totalLifeTime = 3.0f;
	currentLifeTimeLeft = totalLifeTime;
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
	name = "Player";
	health = 1;
	priority = 0;
	cycleSteals = 0;
	totalExecutionTime = 0;
}
PlayerAttribute::~PlayerAttribute()
{
}

MeshAttribute::~MeshAttribute()
{
	//if(mesh)
	//	delete mesh;
}

void MeshAttribute::clean()
{
}
