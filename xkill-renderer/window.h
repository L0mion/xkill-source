/**
 * \class Window
 * \ingroup xkill-renderer
 * \brief Temporary Window using WINAPI.
 * \note It shall be noted that this class is temporary.
 * \author (last to touch it) $Author: David Grelsson, Eric Nilsson $
 */

#ifndef XKILL_RENDERER_WINDOW_H
#define XKILL_RENDERER_WINDOW_H

#include <Windows.h>
#include <windowsx.h>

#include <vector>

#include "dllRenderer.h"

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

class DLL Window
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

	/** \brief Messaging callback loop
      * \param hWnd Derp
	  * \param message Derp
	  * \param wParam Derp
	  * \param lParam Derp
	  * \return derp
      * 
      * Callback function to recieve messages from Windows.
      */
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


#endif //XKILL_RENDERER_WINDOW_H