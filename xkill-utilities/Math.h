#pragma once
#include <cstring>

#include "dllUtilities.h"

#define PI		3.141592654
#define TwoPI	PI * 2

class DLL_U BoolField
{
public:
	static const int NUM_INTS = 4;
	static const int NUM_INTS_PER_BOOL = 32;
	BoolField();
	void clear();
	bool getBool(int index);
	void setBool(int index, bool value);
private:
	int values[NUM_INTS];
};

struct DLL_U Float2
{
	float x;
    float y;

	Float2();
	Float2(float x, float y);
	void copy(const float* float2);
};

struct DLL_U Float3
{
	float x;
    float y;
    float z;

	Float3();
	Float3(float x, float y, float z);
	void copy(const float* float3);
};

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

	Float4();
	Float4(float x, float y, float z, float w);
	void copy(const float* float4);
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
	void setIdentity();
	bool getInverse(Float4x4& inverseOut);
	Float4 multiply(const Float4& vector);
	Float4x4 multiply(const Float4x4& matrix);
	void copy(float* float4x4);
};