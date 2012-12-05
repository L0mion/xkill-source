#ifndef XKILL_RENDERER_OBJFACE_H
#define XKILL_RENDERER_OBJFACE_H

//! Describes a face read from .obj.
/*!
Used to filter duplicate faces, and create lists of indices.
\ingroup xkill-mesh-io-obj
*/
class ObjFace
{
public:
	//!Does nothing.
	ObjFace();
	//! Passes required face-values to object.
	/*!
	\param v Vertex-index.
	\param t Texture-coordinates-index.
	\param n Normal-index.
	\param index Index to face.
	*/
	ObjFace(
		unsigned int v,
		unsigned int t,
		unsigned int n,
		unsigned int index);
	//!Does nothing.
	~ObjFace();

	friend bool operator== (ObjFace &mf1, ObjFace &mf2)
	{
		return (
			mf1.v_ == mf2.v_ &&
			mf1.t_ == mf2.t_ &&
			mf1.n_ == mf2.n_);
	}
	friend bool operator!= (ObjFace &mf1, ObjFace &mf2)
	{
		return !(mf1 == mf2);
	}

	unsigned int getV();	
	unsigned int getT();	
	unsigned int getN();
	unsigned int getIndex();

	void setIndex(unsigned int index);
protected:
private:
	unsigned int v_;		//!< Vertex-index.
	unsigned int t_;		//!< Texture-coordinates-index.
	unsigned int n_;		//!< Normal-index.

	unsigned int index_;	//!< Index to face.
};

#endif //XKILL_RENDERER_OBJFACE_H