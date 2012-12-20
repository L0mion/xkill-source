#ifndef XKILL_UTILITIES_DEBUGSHAPE_H
#define XKILL_UTILITIES_DEBUGSHAPE_H

enum DebugShapeType { DEBUG_SHAPE_SPHERE, DEBUG_SHAPE_BB, DEBUG_SHAPE_FRUSTUM, DEBUG_SHAPE_NA };

//! Describes a basic debug shape, specifying with an enum what type of a debug-shape is supposed to be initializes later in renderer.
struct DebugShape
{
	DebugShape()							{ shapeType_ = DEBUG_SHAPE_NA;	}
	DebugShape(DebugShapeType shapeType)	{ shapeType_ = shapeType;		}
	virtual ~DebugShape()	{}
	
	DebugShapeType shapeType_;
};

//! Describes a sphere-type debug shape.
struct DebugShapeSphere : public DebugShape
{
	DebugShapeSphere(float radius) : DebugShape(DEBUG_SHAPE_SPHERE) { radius_ = radius; }
	~DebugShapeSphere()	{}

	float radius_;
};

//! Describes a bounding box-type debug shape.
struct DebugShapeBB : public DebugShape
{
	DebugShapeBB(Float3 bbMin, Float3 bbMax) : DebugShape(DEBUG_SHAPE_BB) { bbMin_ = bbMin; bbMax_ = bbMax; } 
	~DebugShapeBB()	{}

	Float3 bbMin_;
	Float3 bbMax_;
};

//! Describes a frustum-type debug shape, that is initializes with the eight points that make out the box.
struct DebugShapeFrustum : public DebugShape
{
	DebugShapeFrustum(
		Float3 p1,
		Float3 p2,
		Float3 p3,
		Float3 p4,
		Float3 p5,
		Float3 p6,
		Float3 p7,
		Float3 p8) : DebugShape(DEBUG_SHAPE_FRUSTUM)
	{
		p[0] = p1; p[1] = p2;
		p[2] = p3; p[3] = p4;
		p[4] = p5; p[5] = p6;
		p[6] = p7; p[7] = p8;
	}
	~DebugShapeFrustum() {}

	Float3 p[8];
};

#endif //XKILL_UTILITIES_DEBUGSHAPE_H