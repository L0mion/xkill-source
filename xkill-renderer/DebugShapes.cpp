#include <cmath>

#include <xkill-utilities/Math.h>

#include "DebugShapes.h"

DebugShapes::DebugShapes()
{
}
DebugShapes::~DebugShapes()
{
}

std::vector<VertexPosColor> DebugShapes::getSphere(float radius)
{
	std::vector<VertexPosColor> verticesSphere((SPHERE_RES + 1) * 3);

	Float3 red		= Float3(1.0f, 0.0f, 0.0f);
	Float3 green	= Float3(0.0f, 1.0f, 0.0f);
	Float3 blue		= Float3(0.0f, 0.0f, 1.0f);

	unsigned int index = 0;

	float twoPi	= (float)TwoPI;
	float step	= twoPi / (float)SPHERE_RES;

	Float3 xyVec;
	for(float a = 0.0f; a <= twoPi; a+= step)
	{
		xyVec = Float3(
			(float)cos(a) * radius,
			(float)sin(a) * radius,
			0.0f);
		verticesSphere[index++] = VertexPosColor(xyVec, red);
	}

	Float3 xzVec;
	for(float a = 0.0f; a <= twoPi; a += step)
	{
		xzVec = Float3(
			(float)cos(a) * radius,
			0.0f,
			(float)sin(a) * radius);
		verticesSphere[index++] = VertexPosColor(xzVec, green);
	}

	Float3 yzVec;
	for(float a = 0.0f; a <= twoPi; a += step)
	{
		yzVec = Float3(
			0.0f,
			(float)cos(a) * radius,
			(float)sin(a) * radius);
		verticesSphere[index++] = VertexPosColor(yzVec, blue);
	}

	return verticesSphere;
}