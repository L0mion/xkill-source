#include "Winfo.h"

Winfo::Winfo()
{
	screenWidth_	= 0;
	screenHeight_	= 0;
	numViewports_	= 0;
	csDispathX_		= 0;
	csDispathY_		= 0;
	numViewportsX_	= 0;
	numViewportsY_	= 0;
}
Winfo::~Winfo()
{
	//Do nothing.
}

void Winfo::init(
	unsigned int screenWidth,
	unsigned int screenHeight,
	unsigned int numViewports,
	unsigned int csDispathX,
	unsigned int csDispathY,
	unsigned int numViewportsX,
	unsigned int numViewportsY)
{
	screenWidth_	= screenWidth;
	screenHeight_	= screenHeight;
	numViewports_	= numViewports;
	csDispathX_		= csDispathX;
	csDispathY_		= csDispathY;
	numViewportsX_	= numViewportsX;
	numViewportsY_	= numViewportsY;
}

unsigned int	Winfo::getScreenWidth()		const
{
	return screenWidth_;
}
unsigned int	Winfo::getScreenHeight()	const
{
	return screenHeight_;
}
unsigned int	Winfo::getNumViewports()	const
{
	return numViewports_;
}
unsigned int	Winfo::getCSDispathX()		const
{
	return csDispathX_;
}
unsigned int	Winfo::getCSDispathY()		const
{
	return csDispathY_;
}
unsigned int Winfo::getNumViewportsX()		const
{
	return numViewportsX_;
}
unsigned int Winfo::getNumViewportsY()		const
{
	return numViewportsY_;
}