#ifndef XKILL_RENDERER_MATHBASIC_H
#define XKILL_RENDERER_MATHBASIC_H

#include <math.h>

#include "dllRenderer.h"

#define PI 3.141592654

#define RADIAN(x) ((x) * PI / 180.0f)
#define DEGREE(x) ((x) * 180.0f / PI)

static float lerp(float x, float y, float f)
{
	float result = x * (1.0f - f) + y * f;
	return result;
}

static int mod(int x, int m) 
{
	int r = x%m;
	return r<0 ? r+m : r;
}

inline void clamp(int *x, int a, int b)
{
	x[0] = x[0] < a ? a : (x[0] > b ? b : x[0]);
}

inline int round(float value)
{
	float roundDirection = 0.5f;
	if(value<0)
		roundDirection = -0.5f;
	return (int)(value+roundDirection);
}

struct VecI2
{
	int x;
	int y;

	VecI2()
	{
		x=0;
		y=0;
	}

	void normalize()
	{
		if(x==0 && y==0)
			return;
		if(abs(x)>abs(y))
		{
			x/=abs(x);
			y=0;
		}
		else
		{
			y/=abs(y);
			x=0;
		}
	}

	VecI2(int x, int y)
	{
		this->x = x;
		this->y=y;
	}

	VecI2 operator-()
	{
		VecI2 v;
		v.x = -x;
		v.y = -y;
		return v;
	}
};

inline bool operator==(const VecI2& left, const VecI2& right)
{
	return (left.x == right.x &&
		left.y == right.y);
}

struct VecF2
{
	float x;
	float y;

	VecF2()
	{
		x = 0.0f;
		y = 0.0f;
	}
	VecF2(float _x, float _y)
	{
		x = _x;
		y = _y;
	}
};

struct VecF3
{
	float x;
	float y;
	float z;

	VecF3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	VecF3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	VecF3& operator+=(const VecF3& r)
	{
		x += r.x;
		y += r.y;
		z += r.z;

		return *this;
	}
	VecF3& operator-=(const VecF3& right)
	{
		x -= right.x;
		y -= right.y;
		z -= right.z;

		return *this;
	}
	VecF3& operator*=(float f)
	{
		x *= f;
		y *= f;
		z *= f;

		return *this;
	}

	VecF3	cross(const VecF3& vec) const;
	VecF3&	normalize();
	void	rotate(float angle, const VecF3& axis);
	float	dot(VecF3 vec);
	void	lerp(VecF3 *v, float factor)
	{
		x = x * (1.0f - factor) + v->x * factor;
		y = y * (1.0f - factor) + v->y * factor;
		z = z * (1.0f - factor) + v->z * factor;
	}

	float length()
	{
		return sqrt(x * x + y * y + z * z);
	}

	float distanceTo(VecF3 v)
	{
		VecF3 v3(x-v.x,y-v.y,v.z-z);
		return v3.length();
	}
};
inline VecF3 operator+(const VecF3& left, const VecF3& right)
{
	VecF3 ret(
		left.x + right.x,
		left.y + right.y,
		left.z + right.z);
	return ret;
}
inline VecF3 operator-(const VecF3& left, const VecF3& right)
{
	VecF3 ret(
		left.x - right.x,
		left.y - right.y,
		left.z - right.z);
	return ret;
}
inline VecF3 operator*(const VecF3& left, float scalar)
{
	VecF3 ret(left.x * scalar,
		left.y * scalar,
		left.z * scalar);
	return ret;
}

struct VecF4
{
	float x;
	float y;
	float z;
	float w;

	VecF4()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	}

	VecF4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
};

class MatF4
{
public:
	float m[4][4];

	MatF4()
	{
		identity();
	}

	inline void identity()
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}

	inline MatF4 operator*(const MatF4& right) const
	{
		MatF4 ret;
		for (unsigned int i = 0 ; i < 4 ; i++) {
			for (unsigned int j = 0 ; j < 4 ; j++) {
				ret.m[i][j] =
					m[i][0] * right.m[0][j] +
					m[i][1] * right.m[1][j] +
					m[i][2] * right.m[2][j] +
					m[i][3] * right.m[3][j];
			}
		}
		return ret;
	}

	void lookAtLH(VecF3 eye, VecF3 at, VecF3 up)
	{
		VecF3 zaxis = (at - eye).normalize();
		VecF3 xaxis = (up.cross(zaxis)).normalize();
		VecF3 yaxis = zaxis.cross(xaxis);

		m[0][0] = xaxis.x;	m[0][1] = yaxis.x;	m[0][2] = zaxis.x;	m[0][3] = 0.0f;
		m[1][0] = xaxis.y;	m[1][1] = yaxis.y;	m[1][2] = zaxis.y;	m[1][3] = 0.0f;
		m[2][0] = xaxis.z;	m[2][1] = yaxis.z;	m[2][2] = zaxis.z;	m[2][3] = 0.0f;
		m[3][0] = -xaxis.dot(eye);	m[3][1] = -yaxis.dot(eye);	m[3][3] = -zaxis.dot(eye);	m[3][3] = 1.0f;
	}

	void scaling(float x, float y, float z);
	void rotation(float x, float y, float z);
	void translation(float x, float y, float z);
	void transpose();
};

struct Quaternion
{
	float x, y, z, w;

	Quaternion()
	{
		identity();
	}

	Quaternion(float _x, float _y, float _z, float _w);
	Quaternion(MatF4* pm)
	{
		int i, maxi;
		float maxdiag, S, trace;

		trace = pm->m[0][0] + pm->m[1][1] + pm->m[2][2] + 1.0f;
		if (trace > 0.0f)
		{
			x = (pm->m[1][2] - pm->m[2][1]) / (2.0f * sqrt(trace));
			y = (pm->m[2][0] - pm->m[0][2]) / (2.0f * sqrt(trace));
			z = (pm->m[0][1] - pm->m[1][0]) / (2.0f * sqrt(trace));
			w = sqrt(trace) / 2.0f;
			return;
		}
		maxi = 0;
		maxdiag = pm->m[0][0];
		for (i=1; i<3; i++)
		{
			if ( pm->m[i][i] > maxdiag )
			{
				maxi = i;
				maxdiag = pm->m[i][i];
			}
		}
		switch( maxi )
		{
		case 0:
			S = 2.0f * sqrt(1.0f + pm->m[0][0] - pm->m[1][1] - pm->m[2][2]);
			x = 0.25f * S;
			y = ( pm->m[0][1] + pm->m[1][0] ) / S;
			z = ( pm->m[0][2] + pm->m[2][0] ) / S;
			w = ( pm->m[1][2] - pm->m[2][1] ) / S;
			break;
		case 1:
			S = 2.0f * sqrt(1.0f + pm->m[1][1] - pm->m[0][0] - pm->m[2][2]);
			x = ( pm->m[0][1] + pm->m[1][0] ) / S;
			y = 0.25f * S;
			z = ( pm->m[1][2] + pm->m[2][1] ) / S;
			w = ( pm->m[2][0] - pm->m[0][2] ) / S;
			break;
		case 2:
			S = 2.0f * sqrt(1.0f + pm->m[2][2] - pm->m[0][0] - pm->m[1][1]);
			x = ( pm->m[0][2] + pm->m[2][0] ) / S;
			y = ( pm->m[1][2] + pm->m[2][1] ) / S;
			z = 0.25f * S;
			w = ( pm->m[0][1] - pm->m[1][0] ) / S;
			break;
		}
	};

	void identity()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f;
	};
	void normalize();
	static float quaternionDot(Quaternion *q1, Quaternion *q2)
	{
		return (q1->x) * (q2->x) + (q1->y) * (q2->y) + (q1->z) * (q2->z) + (q1->w) * (q2->w);
	}
	static Quaternion quaternionSlerp(Quaternion *q1, Quaternion *q2, float t)
	{
		float dot, epsilon;
		epsilon = 1.0f;
		dot = quaternionDot(q1, q2);
		if (dot < 0.0f)
			epsilon = -1.0f;

		Quaternion ret;
		ret.x = (1.0f - t) * q1->x + epsilon * t * q2->x;
		ret.y = (1.0f - t) * q1->y + epsilon * t * q2->y;
		ret.z = (1.0f - t) * q1->z + epsilon * t * q2->z;
		ret.w = (1.0f - t) * q1->w + epsilon * t * q2->w;
		ret.normalize();
		return ret;
	};
	Quaternion conjugate();

	MatF4 toMatrix()
	{
		MatF4 m;
		m.m[0][0] = 1.0f - 2.0f*y*y - 2.0f*z*z;
		m.m[0][1] = 2.0f*x*y - 2.0f*z*w;
		m.m[0][2] = 2.0f*x*z + 2.0f*y*w;
		m.m[0][3] = 0.0f;

		m.m[1][0] = 2.0f*x*y + 2.0f*z*w;
		m.m[1][1] = 1.0f - 2.0f*x*x - 2.0f*z*z;
		m.m[1][2] = 2.0f*y*z - 2.0f*x*w;
		m.m[1][3] = 0.0f;

		m.m[2][0] = 2.0f*x*z - 2.0f*y*w;
		m.m[2][1] = 2.0f*y*z + 2.0f*x*w;
		m.m[2][2] = 1.0f - 2.0f*x*x - 2.0f*y*y;
		m.m[2][3] = 0.0f;

		m.m[3][0] = 0.0f;
		m.m[3][1] = 0.0f;
		m.m[3][2] = 0.0f;
		m.m[3][3] = 1.0f;
		return m;
	}
};
Quaternion operator*(const Quaternion& left, const Quaternion& right);
Quaternion operator*(const Quaternion& quaternion, const VecF3& vec);


#endif //XKILL_RENDERER_MATHBASIC_H