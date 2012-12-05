#pragma once
#include <cstring>

struct Float2
{
	float x;
    float y;

	Float2(){};
	void copy(float* float2)
	{
		memcpy(this, float2, sizeof(Float2));
	}
};

struct Float3
{
	float x;
    float y;
    float z;

	Float3(){};
	void copy(const float* float3)
	{
		memcpy(this, float3, sizeof(Float3));
	}
};

struct Float4
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

struct Float4x4
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