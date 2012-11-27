#ifndef XKILL_RENDERER_MESHDEPENDENCIES_H
#define XKILL_RENDERER_MESHDEPENDENCIES_H

#include <Windows.h>

class MeshDependencies
{
public:
	MeshDependencies(
		const LPCWSTR mdTexAlbedo,
		const LPCWSTR mdTexNormal);
	~MeshDependencies();

	const LPCWSTR getMDTexAlbedo();
	const LPCWSTR getMSDTexNormal();
protected:
private:
	LPCWSTR mdTexAlbedo_;
	LPCWSTR mdTexNormal_;
};

/*
Possible upcoming dependencies:
 * Effect-type?
*/

#endif //XKILL_RENDERER_MESHDEPENDENCIES_H