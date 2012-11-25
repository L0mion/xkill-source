#ifndef XKILL_WINDOW_H
#define XKILL_WINDOW_H

#include <Windows.h>
#include <windowsx.h>

#include <vector>

/*! \defgroup xkill XKILL
   Main xkill package.
*/


//! Used to send input from the window message loop.
/*!
\ingroup xkill
*/
struct InputContainer
{
	std::vector<bool>* keys; //!< Each index maps to a key and is true or false depending on whether or not the key is currently pressed. 
	int mouseDeltaX;		//!< Delta movement of the mouse along the x-axis.
	int mouseDeltaY;		//!< Delta movement of the mouse along the y axis.

	//! All member variables must be set in the constructor. 
	InputContainer(std::vector<bool>* keys, int mouseDeltaX, int mouseDeltaY)
	{
		this->keys = keys;
		this->mouseDeltaX = mouseDeltaX;
		this->mouseDeltaY = mouseDeltaY;
	}
};


//! Creates a WINAPI window that will used to present the images produced by RenderingComponent.
/*!
\sa RenderingComponent
\ingroup xkill
*/
class Window
{
public:
	//! Initializes member variables to default values
	/*!
	\param hInstance Variable provided by WINAPI that is used to create the window.
	\param cmdShow Variable provided by WINAPI that is used to create the window.
	\param screenWidth The width of the window.
	\param screenHeight The height of the window.
	*/
	Window(HINSTANCE hInstance, int cmdShow, int screenWidth, int screenHeight);
	//! Clears all memory allocated by the window.
	/*!
	Warning! Deletes a static member variable!
	*/
	~Window();

	//! Initializes the window.
	/*!
	\sa createWindow
	\sa initCursor
	*/
	void init();

	//! Retrieves messages from the Operating System. 
	void checkMessages();

	InputContainer getInput() const;
	HWND getWindowHandle() const;
	bool getIsActive() const;
private:
	//! Creates a window using WINAPI functions.
	void createWindow();	
	//! Locks the cursor to this window, centers the cursor to the center of the window and hides the cursor from the user.
	void initCursor();

	//! Detects mouse movements and warps the cursor back to the center of the window.
	static void mouseDeltaMove(LPARAM lParam);
	//! Windows callback function.
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static std::vector<bool>* keys;		//!< Each index maps to a key and is true or false depending on whether or not the key is currently pressed. 
	static int mouseDeltaX;				//!< Delta movement of the mouse along the x-axis.
	static int mouseDeltaY;				//!< Delta movement of the mouse along the y axis.

	static int screenWidth;		//!< Width of the window
	static int screenHeight;	//!< Height of the window

	static HWND hWnd;		//!< Handle to the window
	HINSTANCE hInstance;	//!< Provided by the operating system, used to create the window
	int cmdShow;			//!< Provided by the operating system, used to create the window
	bool isActive;			//!< Changes to false when the window is shutting down.
};


#endif //XKILL_WINDOW_H