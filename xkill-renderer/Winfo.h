#ifndef XKILL_RENDERER_WINFO_H
#define XKILL_RENDERER_WINFO_H

#define CS_TILE_SIZE 16

typedef struct HWND__* HWND;

static const unsigned int MULTISAMPLES_GBUFFERS		= 1;
static const unsigned int MULTISAMPLES_BACKBUFFER	= 1;
static const unsigned int MULTISAMPLES_DEPTHBUFFER	= 1;

class Winfo
{
public:
	Winfo();
	~Winfo();

	void init(
		unsigned int screenWidth,
		unsigned int screenHeight,
		unsigned int numViewports,
		unsigned int csDispathX_,
		unsigned int csDispathY_,
		unsigned int numViewportsX,
		unsigned int numViewportsY);

	unsigned int getScreenWidth()	const;
	unsigned int getScreenHeight()	const;
	unsigned int getNumViewports()	const;
	unsigned int getCSDispathX()	const;
	unsigned int getCSDispathY()	const;
	unsigned int getNumViewportsX() const;
	unsigned int getNumViewportsY() const;
protected:
private:
	unsigned int screenWidth_;	//!< Width of screen.
	unsigned int screenHeight_;	//!< Height of screen.
	unsigned int numViewports_;	//!< Number of viewports that will be used.
	unsigned int csDispathX_;	//!< Number of threads along the x-axis that will be dispatched by the compute shader.
	unsigned int csDispathY_;	//!< Number of threads along the y-axis that will be dispatched by the compute shader.
	unsigned int numViewportsX_;
	unsigned int numViewportsY_;
};

#endif //XKILL_RENDERER_WINFO_H