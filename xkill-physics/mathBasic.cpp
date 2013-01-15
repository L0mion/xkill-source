#include "MathBasic.h"

VecF3	VecF3::cross(const VecF3& vec) const
{
	const float _x = y * vec.z - z * vec.y;
	const float _y = z * vec.x - x * vec.z;
	const float _z = x * vec.y - y * vec.x;

	return VecF3(_x, _y, _z);
}
VecF3&	VecF3::normalize()
{
	const float length = sqrtf(x * x + y * y + z * z);
	x /= length;
	y /= length;
	z /= length;
	return *this;
}
void	VecF3::rotate(float angle, const VecF3& axel)
{
	const float sinHalfAngle = sinf((float)RADIAN(angle/2));
	const float cosHalfAngle = cosf((float)RADIAN(angle/2));

	const float rX = axel.x * sinHalfAngle;
	const float rY = axel.y * sinHalfAngle;
	const float rZ = axel.z * sinHalfAngle;
	const float rW = cosHalfAngle;
	Quaternion rotationQ(rX, rY, rZ, rW);

	Quaternion conjugateQ = rotationQ.conjugate();
	Quaternion w = rotationQ * (*this) * conjugateQ;

	x = w.x;
	y = w.y;
	z = w.z;
}
float	VecF3::dot(VecF3 vec)
{
	float result =
		x * vec.x +
		y * vec.y +
		z * vec.z;
	return result;
}

void MatF4::scaling(float x, float y, float z)
{
	m[0][0] = x;	m[0][1] = 0.0f;	m[0][2] = 0.0f;	m[0][3] = 0.0f;
	m[1][0] = 0.0f;	m[1][1] = y;	m[1][2] = 0.0f;	m[1][3] = 0.0f;
	m[2][0] = 0.0f;	m[2][1] = 0.0f;	m[2][2] = z;	m[2][3] = 0.0f;
	m[3][0] = 0.0f;	m[3][1] = 0.0f;	m[3][2] = 0.0f;	m[3][3] = 1.0f;
}
void MatF4::rotation(float x, float y, float z)
{
	MatF4 rx, ry, rz;

	const float radianX = (float)RADIAN(x);
	const float radianY = (float)RADIAN(y);
	const float radianZ = (float)RADIAN(z);

	rx.m[0][0] = 1.0f; rx.m[0][1] = 0.0f;	rx.m[0][2] = 0.0f;	rx.m[0][3] = 0.0f;
	rx.m[1][0] = 0.0f; rx.m[1][1] = cosf(radianX);	rx.m[1][2] = -sinf(radianX);	rx.m[1][3] = 0.0f;
	rx.m[2][0] = 0.0f; rx.m[2][1] = sinf(radianX);	rx.m[2][2] = cosf(radianX);	rx.m[2][3] = 0.0f;
	rx.m[3][0] = 0.0f; rx.m[3][1] = 0.0f;	rx.m[3][2] = 0.0f;	rx.m[3][3] = 1.0f;

	ry.m[0][0] = cosf(radianY);	ry.m[0][1] = 0.0f; ry.m[0][2] = -sinf(radianY);	ry.m[0][3] = 0.0f;
	ry.m[1][0] = 0.0f;	ry.m[1][1] = 1.0f; ry.m[1][2] = 0.0f;	ry.m[1][3] = 0.0f;
	ry.m[2][0] = sinf(radianY);	ry.m[2][1] = 0.0f; ry.m[2][2] = cosf(radianY);	ry.m[2][3] = 0.0f;
	ry.m[3][0] = 0.0f;	ry.m[3][1] = 0.0f; ry.m[3][2] = 0.0f;	ry.m[3][3] = 1.0f;

	rz.m[0][0] = cosf(radianZ);	rz.m[0][1] = -sinf(radianZ);	rz.m[0][2] = 0.0f; rz.m[0][3] = 0.0f;
	rz.m[1][0] = sinf(radianZ);	rz.m[1][1] = cosf(radianZ);	rz.m[1][2] = 0.0f; rz.m[1][3] = 0.0f;
	rz.m[2][0] = 0.0f;	rz.m[2][1] = 0.0f;	rz.m[2][2] = 1.0f; rz.m[2][3] = 0.0f;
	rz.m[3][0] = 0.0f;	rz.m[3][1] = 0.0f;	rz.m[3][2] = 0.0f; rz.m[3][3] = 1.0f;

	*this = rz * ry * rx;
}
void MatF4::translation(float x, float y, float z)
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void MatF4::transpose()
{
	float temp[4][4];
	temp[0][0] = m[0][0];
	temp[1][0] = m[0][1];
	temp[2][0] = m[0][2];
	temp[3][0] = m[0][3];

	temp[0][1] = m[1][0];
	temp[1][1] = m[1][1];
	temp[2][1] = m[1][2];
	temp[3][1] = m[1][3];

	temp[0][2] = m[2][0];
	temp[1][2] = m[2][1];
	temp[2][2] = m[2][2];
	temp[3][2] = m[2][3];

	temp[0][3] = m[3][0];
	temp[1][3] = m[3][1];
	temp[2][3] = m[3][2];
	temp[3][3] = m[3][3];

	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			m[i][j] = temp[i][j];
		}
	}
}

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

void Quaternion::normalize()
{
	float length = sqrtf(x * x + y * y + z * z + w * w);
	x /= length;
	y /= length;
	z /= length;
	w /= length;
}
Quaternion	Quaternion::conjugate()
{
	Quaternion ret(-x, -y, -z, w);
	return ret;
}

Quaternion operator*(const Quaternion& left, const Quaternion& right)
{
	const float w = (left.w * right.w) - (left.x * right.x) - (left.y * right.y) - (left.z * right.z);
	const float x = (left.x * right.w) + (left.w * right.x) + (left.y * right.z) - (left.z * right.y);
	const float y = (left.y * right.w) + (left.w * right.y) + (left.z * right.x) - (left.x * right.z);
	const float z = (left.z * right.w) + (left.w * right.z) + (left.x * right.y) - (left.y * right.x);

	Quaternion ret(x, y, z, w);
	return ret;
}
Quaternion operator*(const Quaternion& quaternion, const VecF3& vec)
{
	const float w = - (quaternion.x * vec.x) - (quaternion.y * vec.y) - (quaternion.z * vec.z);
	const float x = (quaternion.w * vec.x) + (quaternion.y * vec.z) - (quaternion.z * vec.y);
	const float y = (quaternion.w * vec.y) + (quaternion.z * vec.x) - (quaternion.x * vec.z);
	const float z = (quaternion.w * vec.z) + (quaternion.x * vec.y) - (quaternion.y * vec.x);

	Quaternion ret(x, y, z, w);
	return ret;
}