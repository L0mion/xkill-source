#include <Windows.h>

#include "Winfo.h"

Winfo::Winfo()
{
	screenWidth_	= 0;
	screenHeight_	= 0;
	numViewports_	= 0;
	csDispathX_		= 0;
	csDispathY_		= 0;
}
Winfo::~Winfo()
{
	//Do nothing.
}

void Winfo::init(
	unsigned int	screenWidth,
	unsigned int	screenHeight,
	unsigned int	numViewports,
	unsigned int	csDispathX,
	unsigned int	csDispathY)
{
	screenWidth_	= screenWidth;
	screenHeight_	= screenHeight;
	numViewports_	= numViewports;
	csDispathX_		= csDispathX;
	csDispathY_		= csDispathY;


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