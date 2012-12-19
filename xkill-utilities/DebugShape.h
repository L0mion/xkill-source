#ifndef XKILL_UTILITIES_DEBUGSHAPE_H
#define XKILL_UTILITIES_DEBUGSHAPE_H

enum DebugShapeType { DEBUG_SHAPE_SPHERE, DEBUG_SHAPE_BB, DEBUG_SHAPE_NA };

struct DebugShape
{
	DebugShape()							{ shapeType_ = DEBUG_SHAPE_NA;	}
	DebugShape(DebugShapeType shapeType)	{ shapeType_ = shapeType;		}
	virtual ~DebugShape()	{}
	
	DebugShapeType shapeType_;
};

struct DebugShapeSphere : public DebugShape
{
	//DebugShapeSphere() : DebugShape(DEBUG_SHAPE_SPHERE)	{ radius_ = 0.0f; }
	DebugShapeSphere(float radius) : DebugShape(DEBUG_SHAPE_SPHERE) { radius_ = radius; }
	~DebugShapeSphere()	{}

	float radius_;
};

struct DebugShapeBB : public DebugShape
{
	//DebugShapeBB() : DebugShape(DEBUG_SHAPE_BB)	{ bbMin_ = bbMax_ = 0.0f; }
	DebugShapeBB(Float3 bbMin, Float3 bbMax) : DebugShape(DEBUG_SHAPE_BB) { bbMin_ = bbMin; bbMax_ = bbMax; } 
	~DebugShapeBB()	{}

	Float3 bbMin_;
	Float3 bbMax_;
};

#endif //XKILL_UTILITIES_DEBUGSHAPE_H