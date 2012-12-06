#pragma once
#include <cstring>

#include "dllUtilities.h"

struct DLL_U Float2
{
	float x;
    float y;

	Float2()
	{
		x = 0.0f;
		y = 0.0f;
	};
	Float2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	void copy(float* float2)
	{
		//memcpy(this, float2, sizeof(Float2));
		x = float2[0];
		y = float2[1];
	}
};

struct DLL_U Float3
{
	float x;
    float y;
    float z;

	Float3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	};
	Float3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	};
	void copy(float* float3)
	{
		//memcpy(this, float3, sizeof(Float3));
		x = float3[0];
		y = float3[1];
		z = float3[2];
	}
};

struct DLL_U Float4
{
	float x;
	float y;
	float z;
	float w;

	Float4()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f;
	};
	Float4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	};
	void copy(float* float4)
	{
		//memcpy(this, float4, sizeof(Float4));
		x = float4[0];
		y = float4[1];
		z = float4[2];
		w = float4[3];
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

	Float4x4()
	{
		m[0][0] = 0.0f;
		m[0][1] = 0.0f;
		m[0][2] = 0.0f;
		m[0][3] = 0.0f;

		m[1][0] = 0.0f;
		m[1][1] = 0.0f;
		m[1][2] = 0.0f;
		m[1][3] = 0.0f;

		m[2][0] = 0.0f;
		m[2][1] = 0.0f;
		m[2][2] = 0.0f;
		m[2][3] = 0.0f;

		m[3][0] = 0.0f;
		m[3][1] = 0.0f;
		m[3][2] = 0.0f;
		m[3][3] = 0.0f;
	};
	void copy(float* float4x4)
	{
		//memcpy(this, float4x4, sizeof(Float4x4));
		m[0][0] = float4x4[0];
		m[0][1] = float4x4[1];
		m[0][2] = float4x4[2];
		m[0][3] = float4x4[3];

		m[1][0] = float4x4[4];
		m[1][1] = float4x4[5];
		m[1][2] = float4x4[6];
		m[1][3] = float4x4[7];

		m[2][0] = float4x4[8];
		m[2][1] = float4x4[9];
		m[2][2] = float4x4[10];
		m[2][3] = float4x4[11];

		m[3][0] = float4x4[12];
		m[3][1] = float4x4[13];
		m[3][2] = float4x4[14];
		m[3][3] = float4x4[15];
	}
};