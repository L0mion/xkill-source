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
	void parseObjectFile(std::string filename, std::vector<Vertex>* vertices);

private:
	
	std::vector<VecF3> positions;
	std::vector<VecF3> normals;
	std::vector<VecF2> texcoords;

	void parsePosition(std::fstream& infile);
	void parseNormal(std::fstream& infile);
	void parseTexcoord(std::fstream& infile);
	void parseFace(std::fstream& infile, std::vector<Vertex>* vertices);
	void parseMaterial(std::fstream& infile);
};

#endif //XKILL_RENDERER_OBJLOADERBASIC_H