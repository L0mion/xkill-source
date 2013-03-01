#ifndef XKILL_PHYSICS_PHYSICSUTILITIES
#define XKILL_PHYSICS_PHYSICSUTILITIES


//static const float WorldScaling = 100.0f;

class btVector3;
class btQuaternion;
class btMatrix3x3;
struct Float4;
struct Float4x4;
struct Float3;


btQuaternion convert(Float4 float4);

btVector3 convert(Float3 float3);
Float3 convert(const btVector3* float3);

btMatrix3x3 convert(Float4x4 float4x4);
float yawFromQuaternion(const btQuaternion& q);

#endif //XKILL_PHYSICS_PHYSICSUTILITIES