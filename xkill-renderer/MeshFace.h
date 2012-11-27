#ifndef XKILL_RENDERER_MESHFACE_H
#define XKILL_RENDERER_MESHFACE_H

class MeshFace
{
public:
	MeshFace()
	{
		//Do nothing.
	}
	MeshFace(
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
	~MeshFace()
	{
		//Do nothing.
	}

	friend bool operator== (MeshFace &mf1, MeshFace &mf2)
	{
		return (
			mf1.v_ == mf2.v_ &&
			mf1.t_ == mf2.t_ &&
			mf1.n_ == mf2.n_);
	}
	friend bool operator!= (MeshFace &mf1, MeshFace &mf2)
	{
		return !(mf1 == mf2);
	}

	unsigned int getV()		{ return v_;		}
	unsigned int getT()		{ return t_;		}
	unsigned int getN()		{ return n_;		}
	unsigned int getIndex()	{ return index_;	}

	void setIndex(unsigned int index) {this->index_ = index; }
protected:
private:
	unsigned int v_;
	unsigned int t_;
	unsigned int n_;

	unsigned int index_;
};

#endif //XKILL_RENDERER_MESHFACE_H