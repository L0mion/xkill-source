#include <cmath>

#include <xkill-utilities/Math.h>

#include "DebugShapes.h"

DebugShapes::DebugShapes()
{
	//Do nothing.
}
DebugShapes::~DebugShapes()
{
	//Do nothing.
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
std::vector<VertexPosColor> DebugShapes::getBB(Float3 bbMin, Float3 bbMax)
{	
	Float3 p1 = bbMin;
	Float3 p2 = Float3(bbMax.x, bbMin.y, bbMin.z);
	Float3 p3 = Float3(bbMin.x, bbMax.y, bbMin.z);
	Float3 p4 = Float3(bbMax.x, bbMax.y, bbMin.z);

	Float3 p5 = Float3(bbMin.x, bbMin.y, bbMax.z);
	Float3 p6 = Float3(bbMax.x, bbMin.y, bbMax.z);
	Float3 p7 = Float3(bbMin.x, bbMax.y, bbMax.z);
	Float3 p8 = bbMax;
		
	VertexPosColor v;
	v.color_ = Float3(1.0f, 0.0f, 0.0f);

	//Connect vertices in a line list.
	std::vector<VertexPosColor> vertices;

	//Connect first quad
	v.position_ = p1; vertices.push_back(v);
	v.position_ = p2; vertices.push_back(v);
	v.position_ = p1; vertices.push_back(v);
	v.position_ = p3; vertices.push_back(v);
	v.position_ = p4; vertices.push_back(v);
	v.position_ = p2; vertices.push_back(v);
	v.position_ = p4; vertices.push_back(v);
	v.position_ = p3; vertices.push_back(v);

	//Connect second quad
	v.position_ = p5; vertices.push_back(v);
	v.position_ = p6; vertices.push_back(v);
	v.position_ = p5; vertices.push_back(v);
	v.position_ = p7; vertices.push_back(v);
	v.position_ = p8; vertices.push_back(v);
	v.position_ = p7; vertices.push_back(v);
	v.position_ = p8; vertices.push_back(v);
	v.position_ = p6; vertices.push_back(v);

	//Connect quads
	v.position_ = p1; vertices.push_back(v);
	v.position_ = p5; vertices.push_back(v);
	v.position_ = p2; vertices.push_back(v);
	v.position_ = p6; vertices.push_back(v);
	v.position_ = p3; vertices.push_back(v);
	v.position_ = p7; vertices.push_back(v);
	v.position_ = p4; vertices.push_back(v);
	v.position_ = p8; vertices.push_back(v);

	return vertices;
}

/*
v.position_ = p1; vertices.push_back(v);
v.position_ = p2; vertices.push_back(v);
v.position_ = p3; vertices.push_back(v);
v.position_ = p4; vertices.push_back(v);
v.position_ = p5; vertices.push_back(v);
v.position_ = p6; vertices.push_back(v);
v.position_ = p7; vertices.push_back(v);
v.position_ = p8; vertices.push_back(v);
*/