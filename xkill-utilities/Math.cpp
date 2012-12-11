#include "Math.h"


BoolField::BoolField()
{
	for(unsigned int i = 0; i < NUM_INTS; i++)
	{
		values[i] = 0;
	}
}
void BoolField::clear()
{
	for(unsigned int i = 0; i < NUM_INTS; i++)
	{
		values[i] = 0;
	}
}
bool BoolField::getBool(int index)
{
	values[index/NUM_INTS_PER_BOOL] = values[index/NUM_INTS_PER_BOOL] & (1 << (index%NUM_INTS_PER_BOOL));
	if(index)
		return true;
	else
		return false;
}
void BoolField::setBool(int index, bool value)
{
	int a = index/NUM_INTS_PER_BOOL;
	int b = index%NUM_INTS_PER_BOOL;


	int i = (1 << (index % NUM_INTS_PER_BOOL));
	if(value)
		values[index/NUM_INTS_PER_BOOL] = values[index/NUM_INTS_PER_BOOL] | i;
	else if(getBool(index))
		values[index/NUM_INTS_PER_BOOL] = values[index/NUM_INTS_PER_BOOL] ^ i;
			
}

Float2::Float2()
{
	x = 0.0f;
	y = 0.0f;
}
Float2::Float2(float x, float y)
{
	this->x = x;
	this->y = y;
}
void Float2::copy(const float* float2)
{
	//memcpy(this, float2, sizeof(Float2));
	x = float2[0];
	y = float2[1];
}

Float3::Float3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}
Float3::Float3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
void Float3::copy(const float* float3)
{
	//memcpy(this, float3, sizeof(Float3));
	x = float3[0];
	y = float3[1];
	z = float3[2];
}

Float4::Float4()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
};
Float4::Float4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
};
void Float4::copy(const float* float4)
{
	//memcpy(this, float4, sizeof(Float4));
	x = float4[0];
	y = float4[1];
	z = float4[2];
	w = float4[3];
}

Float4x4::Float4x4()
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
}
void Float4x4::copy(float* float4x4)
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

bool Float4x4::getInverse(Float4x4& inverseOut)
{
//	bool gluInvertMatrix(const double m[16], double invOut[16])
    float inv[16], det;
	int i;
	float* m = this->m16;
	
    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
		inverseOut.m16[i] = inv[i] * det;

    return true;
}

Float4 Float4x4::multiply(const Float4& vector)
{
	Float4 result;
	for(unsigned int i=0; i<4; i++)
	{
		for(unsigned int j=0; j<4; j++)
		{
			result.m[i] += m[i][j]*vector.m[j];
		}
	}
	return result;
}

Float4x4 Float4x4::multiply(const Float4x4& matrix)
{
	Float4x4 result;
	for(unsigned int i=0; i<4; i++)
	{
		for(unsigned int j=0; j<4; j++)
		{
			for(unsigned int k=0; k<4; k++)
			{
				result.m[i][j] += m[i][k]*matrix.m[k][j];
			}
		}
	}
	return result;
}

void Float4x4::setIdentity()
{
		
	m[0][0] = 1.0f;
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = 1.0f;
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = 1.0f;
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}