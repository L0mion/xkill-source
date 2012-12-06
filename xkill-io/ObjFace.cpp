#include "ObjFace.h"

ObjFace::ObjFace()
{
	//Do nothing.
}
ObjFace::ObjFace(
	unsigned int v,
	unsigned int t,
	unsigned int n,
	unsigned int index)
{
	this->v_		= v;
	this->t_		= t;
	this->n_		= n;
	this->index_	= index;
}
ObjFace::~ObjFace()
{
	//Do nothing.
}

void ObjFace::setIndex(unsigned int index)
{
	this->index_ = index;
}

unsigned int ObjFace::getV()		{ return v_;		}
unsigned int ObjFace::getT()		{ return t_;		}
unsigned int ObjFace::getN()		{ return n_;		}
unsigned int ObjFace::getIndex()	{ return index_;	}