#include "AttributeType.h"
#include <DirectXMath.h>
#include <windows.h>

IAttribute::IAttribute()
{
}
IAttribute::~IAttribute()
{
}

PositionAttribute::PositionAttribute()
{
	position[0] = 0.0f;
	position[1] = 0.0f;
	position[2] = 0.0f;
}
PositionAttribute::~PositionAttribute()
{
}

SpatialAttribute::SpatialAttribute()
{
	rotation[0] = 0.0f;
	rotation[1] = 0.0f;
	rotation[2] = 0.0f;
	rotation[3] = 1.0f;

	scale[0] = 1.0f;
	scale[1] = 1.0f;
	scale[2] = 1.0f;
}
SpatialAttribute::~SpatialAttribute()
{
}

RenderAttribute::RenderAttribute()
{
	transparent		= false;
	tessellation	= false;
	meshID			= 0;
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
	rotationVelocity[0] = 0; 
	rotationVelocity[1] = 0;
	rotationVelocity[2] = 0;
	rotationVelocity[3] = 1;
	velocity[0] = 0;
	velocity[1] = 0;
	velocity[2] = 0;
}
PhysicsAttribute::~PhysicsAttribute()
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
	ZeroMemory(mat_projection, sizeof(mat_projection));
	ZeroMemory(mat_view, sizeof(mat_view));
}
CameraAttribute::~CameraAttribute()
{
}

struct float3
{
	float f1, f2, f3;
	float3(float f1, float f2, float f3)
	{
		this->f1 = f1; this->f2 = f2; this->f3 = f3;
	}
	~float3();
};
InputAttribute::InputAttribute()
{
	fire = false;
	ZeroMemory(position,(2*sizeof(float)));
	ZeroMemory(rotation,(2*sizeof(float)));
	DirectX::XMFLOAT3 test;
	test.x = 1.0f;
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