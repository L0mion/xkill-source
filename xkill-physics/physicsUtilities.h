#ifndef XKILL_PHYSICS_PHYSICSUTILITIES
#define XKILL_PHYSICS_PHYSICSUTILITIES


static const float WorldScaling = 100.0f;

class btVector3;
class btQuaternion;
struct Float4;
struct Float3;

btQuaternion convert(Float4 float4);
btVector3 convert(Float3 float3);

#endif //XKILL_PHYSICS_PHYSICSUTILITIES