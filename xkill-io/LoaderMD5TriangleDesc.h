#ifndef LOADER_MD5_TRIANGLE_DESC_H
#define LOADER_MD5_TRIANGLE_DESC_H

static const unsigned int MD5_TRIANGLE_SIZE = 3;

struct LoaderMD5TriangleDesc
{
	int indices_[MD5_TRIANGLE_SIZE];

	LoaderMD5TriangleDesc()
	{
		for(unsigned int i=0; i<MD5_TRIANGLE_SIZE; i++)
			indices_[i] = 0;
	}
	LoaderMD5TriangleDesc(int indices[])
	{
		for(unsigned int i=0; i<MD5_TRIANGLE_SIZE; i++)
			indices_[i] = indices[i];
	}
	LoaderMD5TriangleDesc(int index0, int index1, int index2)
	{
		indices_[0] = index0;
		indices_[1] = index1;
		indices_[2] = index2;
	}
};

#endif //LOADER_MD_TRIANGLE_DESC_H