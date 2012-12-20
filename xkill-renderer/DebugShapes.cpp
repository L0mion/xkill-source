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

	return getBoxLines(
		p1, 
		p2,
		p3,
		p4,
		p5,
		p6,
		p7,
		p8,
		Float3(0.0f, 1.0f, 0.0f)); //BBs are blue color.
}

std::vector<VertexPosColor> DebugShapes::getFrustum(
		Float3 p1, Float3 p2,
		Float3 p3, Float3 p4,
		Float3 p5, Float3 p6,
		Float3 p7, Float3 p8)
{
	return getBoxLines(
		p1, 
		p2,
		p3,
		p4,
		p5,
		p6,
		p7,
		p8,
		Float3(1.0f, 0.0f, 0.0f)); //Frustum is red color.
}

std::vector<VertexPosColor> DebugShapes::getBoxLines(
	Float3 p1, Float3 p2,
	Float3 p3, Float3 p4,
	Float3 p5, Float3 p6,
	Float3 p7, Float3 p8,
	Float3 color)
{
	//Connect vertices in a line list.
	std::vector<VertexPosColor> vertices;
	VertexPosColor v;
	v.color_ = color;

#define CONNECT_LINE(p1, p2)					\
	v.position_ = p1; vertices.push_back(v);	\
	v.position_ = p2; vertices.push_back(v);	\

	//Connect first quad
	CONNECT_LINE(p1, p2);
	CONNECT_LINE(p1, p3);
	CONNECT_LINE(p4, p2);
	CONNECT_LINE(p4, p3);

	//Connect second quad
	CONNECT_LINE(p5, p6);
	CONNECT_LINE(p5, p7);
	CONNECT_LINE(p8, p7);
	CONNECT_LINE(p8, p6);

	//Connect quads
	CONNECT_LINE(p1, p5);
	CONNECT_LINE(p2, p6);
	CONNECT_LINE(p3, p7);
	CONNECT_LINE(p4, p8);

#undef CONNECT_LINE

	return vertices;
}