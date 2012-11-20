struct vec3
{
	float x,y,z;
};

struct vec4 : public vec3
{
	float w;
};

struct PositionAttribute
{
	vec4 position;
};

struct SpatialAttribute
{
	PositionAttribute* pa;
	vec4 rotation;
	vec4 scale;
};

struct PhysicsAttribute
{
	SpatialAttribute* sa;
	vec4 velocity;
	vec4 rotationVelocity;
	bool added;
	bool alive;
};

