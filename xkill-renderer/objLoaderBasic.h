#ifndef XKILL_RENDERER_OBJLOADERBASIC_H
#define XKILL_RENDERER_OBJLOADERBASIC_H

#include <vector>
#include <string>
#include <fstream>

#include "mathBasic.h"
#include "vertices.h"

class ObjLoaderBasic
{
public:
	ObjLoaderBasic();
	virtual ~ObjLoaderBasic();
	void parseObjectFile(std::string filename, std::vector<VertexPosNormTex>* vertices);

private:
	
	std::vector<VecF3> positions_;
	std::vector<VecF3> normals_;
	std::vector<VecF2> texcoords_;

	void parsePosition(std::fstream& infile);
	void parseNormal(std::fstream& infile);
	void parseTexcoord(std::fstream& infile);
	void parseFace(std::fstream& infile, std::vector<VertexPosNormTex>* vertices);
	void parseMaterial(std::fstream& infile);
};

#endif //XKILL_RENDERER_OBJLOADERBASIC_H