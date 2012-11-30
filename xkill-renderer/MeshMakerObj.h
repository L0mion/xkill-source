#ifndef XKILL_RENDERER_MESHMAKER_H
#define XKILL_RENDERER_MESHMAKER_H

class LoaderObj;
class LoaderMTL;

class MeshMakerObj
{
public:
	MeshMakerObj();
	~MeshMakerObj();

	void init();
protected:
private:
	LoaderObj* loaderObj_;
	LoaderMTL* loaderMtl_;
};

#endif //XKILL_RENDERER_MESHMAKER_H