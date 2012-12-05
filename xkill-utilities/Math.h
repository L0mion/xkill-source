#pragma once
#include <cstring>

#include "dllUtilities.h"

struct DLL_U Float2
{
	float x;
    float y;

	Float2(){};
	Float2(float x, float y){ this->x = x; this->y = y; };
	void copy(float* float2)
	{
		memcpy(this, float2, sizeof(Float2));
	}
};

struct DLL_U Float3
{
	float x;
    float y;
    float z;

	Float3(){};
	Float3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	void copy(float* float3)
	{
		memcpy(this, float3, sizeof(Float3));
	}
};

struct DLL_U Float4
{
	float x;
	float y;
	float z;
	float w;

	Float4(){};
	void copy(float* float4)
	{
		memcpy(this, float4, sizeof(Float4));
	}
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
    };

	Float4x4(){};
	void copy(float* float4x4)
	{
		memcpy(this, float4x4, sizeof(Float4x4));
	}
};