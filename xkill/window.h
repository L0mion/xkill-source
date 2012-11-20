#ifndef XKILL_WINDOW_H
#define XKILL_WINDOW_H

#include <Windows.h>
#include <windowsx.h>

#include <vector>

struct InputContainer
{
	std::vector<bool>* keys;
	int mouseDeltaX;
	int mouseDeltaY;

	InputContainer(std::vector<bool>* keys, int mouseDeltaX, int mouseDeltaY)
	{
		this->keys = keys;
		this->mouseDeltaX = mouseDeltaX;
		this->mouseDeltaY = mouseDeltaY;
	}
};

class Window
{
public:
	Window(HINSTANCE hInstance, int cmdShow, int screenWidth, int screenHeight);
	~Window();

	void init();

	void checkMessages();
	InputContainer getInput() const;
	HWND getWindowHandle() const;
	bool getIsActive() const;
private:
	void createWindow();
	void initCursor();
	static void mouseDeltaMove(LPARAM lParam);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static std::vector<bool>* keys;	///< This attribute has caused us trouble.
	static int mouseDeltaX;
	static int mouseDeltaY;	

	static int screenWidth;
	static int screenHeight;

	static HWND hWnd;
	HINSTANCE hInstance;
	int cmdShow;
	bool isActive;
};


#endif //XKILL_WINDOW_H