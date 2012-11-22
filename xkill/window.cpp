#include "window.h"

std::vector<bool>* Window::keys_;
int Window::mouseDeltaX_;
int Window::mouseDeltaY_;

int Window::screenWidth_;
int Window::screenHeight_;

HWND Window::hWnd_;

Window::Window(HINSTANCE hInstance, int cmdShow, int screenWidth, int screenHeight)
{
	this->hInstance_ = hInstance;
	this->cmdShow_ = cmdShow;
	this->screenWidth_ = screenWidth;
	this->screenHeight_ = screenHeight;

	keys_ = new std::vector<bool>();
	keys_->resize(256);

	isActive_ = true;
}

Window::~Window()
{
	//DANGER!!!!!!!!
	//deleting static member variable in class instance
	delete keys_;
}

void Window::init()
{
	createWindow();
	initCursor();
}

void Window::checkMessages()
{
	MSG msg = {0};

	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if(msg.message == WM_QUIT)
			isActive_ = false;
	}
}

InputContainer Window::getInput() const
{
	return InputContainer(keys_, mouseDeltaX_, mouseDeltaY_);
}

HWND Window::getWindowHandle() const
{
	return hWnd_;
}

bool Window::getIsActive() const
{
	return isActive_;
}

void Window::createWindow()
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance_;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	hWnd_ = CreateWindowEx(NULL,
						  L"WindowClass",
						  L"Temp",
						  WS_OVERLAPPEDWINDOW,
						  100,
						  100,
						  screenWidth_,
						  screenHeight_,
						  NULL,
						  NULL,
						  hInstance_,
						  NULL);

	ShowWindow(hWnd_, cmdShow_);
}

void Window::initCursor()
{
	SetCursorPos(screenWidth_/2, screenHeight_/2);
	
	//Capture the cursor to this window
	SetCapture(hWnd_);

	//Hide the cursor from the user
	ShowCursor(false);
}

void Window::mouseDeltaMove(LPARAM lParam)
{
	//Find the upper left corner of the window's client area in screen coordinates
	POINT point;
	point.x = 0;
	point.y = 0;
	MapWindowPoints(hWnd_, NULL, &point, 1);
	
	//Get current mouse position
	int mouseX = GET_X_LPARAM(lParam)+point.x;
	int mouseY = GET_Y_LPARAM(lParam)+point.y;

	//Calculate relative mouse movement
	mouseDeltaX_ = mouseX - screenWidth_/2;
	mouseDeltaY_ = mouseY - screenHeight_/2;

	//Return cursor to screen center
	SetCursorPos(screenWidth_/2, screenHeight_/2);
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}break;

	case WM_KEYDOWN:
	{
		if(wParam == VK_ESCAPE)
		{
			DestroyWindow(hWnd);
		}
		
		keys_->at(wParam) = true;

		return 0;
	}break;

	case WM_KEYUP:
	{
		keys_->at(wParam) = false;

		return 0;
	}break;

	case WM_MOUSEMOVE:
	{
		mouseDeltaMove(lParam);
		return 0;
	}break;
	
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}