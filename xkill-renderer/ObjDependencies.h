#ifndef XKILL_RENDERER_OBJDEPENDENCIES_H
#define XKILL_RENDERER_OBJDEPENDENCIES_H

#include <vector>

class ObjDependencies
{
public:
	ObjDependencies();
	~ObjDependencies();

	void pushDependencyMTL(const std::string mtlFile);

	std::vector<std::string> getMTLFiles();
protected:
private:
	std::vector<std::string> mtlFiles_;
};

#endif //XKILL_RENDERER_OBJDEPENDENCIES_H