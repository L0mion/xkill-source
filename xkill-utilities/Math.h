#pragma once
#include <cstring>

#include "dllUtilities.h"

#define PI		3.141592654
#define TwoPI	PI * 2

class DLL_U Math
{
public:
	static int randomInt(int value);
	static int randomInt(int min, int max);
	static float randomFloat(float min, float max);
};

class DLL_U BoolField
{
public:
	static const int NUM_INTS = 4;
	static const int NUM_INTS_PER_BOOL = 32;
	BoolField();
	void clear();
	bool getBool(int index);
	bool getAnySet();
	void setBool(int index, bool value);

	int values[NUM_INTS];
	private:
};

struct DLL_U Int2
{
	int x;
	int y;

	Int2();
	Int2(int x, int y);
};

struct DLL_U Float2
{
	float x;
    float y;

	Float2();
	Float2(float x, float y);
	void copy(const float* float2);
	void lerp( Float2 *v, float factor );
	Float2 lerp( Float2 *v1, Float2 *v2, float factor );
	Float2& normalize();
	float length();
};

//
// Float3
//

struct DLL_U Float3
{
	float x;
    float y;
    float z;

	Float3();
	Float3(float x, float y, float z);
	void copy(const float* float3);
	
	float* asFloat();

	Float3&	normalize();
	Float3 cross(const Float3& vec) const;
	float dot(Float3 vec);
	void lerp(Float3 *v, float factor);
	float length();
	float distanceTo(Float3 v);

	Float3 operator/(float scalar);
};


// Vector addition
inline Float3 operator+(const Float3& left, const Float3& right)
{
	Float3 ret(
		left.x + right.x,
		left.y + right.y,
		left.z + right.z);
	return ret;
}

// Vector subtraction
inline Float3 operator-(const Float3& left, const Float3& right)
{
	Float3 ret(
		left.x - right.x,
		left.y - right.y,
		left.z - right.z);
	return ret;
}

// Vector scaling
inline Float3 operator*(const Float3& left, float scalar)
{
	Float3 ret(left.x * scalar,
		left.y * scalar,
		left.z * scalar);
	return ret;
}


//
// Float4
//

struct DLL_U Float4
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		float m[4];
	};

	Float3 quaternionToVector();
	Float4();
	Float4(float x, float y, float z, float w);
	Float4(Float3 float3, float w);
	void copy(const float* float4);
	Float4& normalize();
};

struct DLL_U Float4x4
{
	union
    {
        struct
        {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
		float m16[16];
    };

	Float4x4();
	Float4x4(float filler);
	void setIdentity();
	bool getInverse(Float4x4& inverseOut);
	Float4 multiply(const Float4& vector);
	Float4x4 multiply(const Float4x4& matrix);
	void copy(float* float4x4);

	Float3 getLookAt();

	Float4x4 getRotationOnly();
};

float DLL_U PlaneDotCoord(Float4 plane, Float3 coord);

float  DLL_U Determinant(float _11, float _21, float _12, float _22);

Float3 DLL_U PlaneIntersectPlane(Float4 plane1, Float4 plane2, Float4 plane3);

Float4 DLL_U PlaneNormalize(Float4 plane);
