#include <window.h>

std::vector<bool>* Window::keys;
int Window::mouseDeltaX;
int Window::mouseDeltaY;

int Window::screenWidth;
int Window::screenHeight;

HWND Window::hWnd;

Window::Window(HINSTANCE hInstance, int cmdShow, int screenWidth, int screenHeight)
{
	this->hInstance = hInstance;
	this->cmdShow = cmdShow;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	keys = new std::vector<bool>();
	keys->resize(256);

	isActive = true;
}

Window::~Window()
{
	//DANGER!!!!!!!!
	//deleting static member variable in class instance
	delete keys;
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
			isActive = false;
	}
}

InputContainer Window::getInput() const
{
	return InputContainer(keys, mouseDeltaX, mouseDeltaY);
}

HWND Window::getWindowHandle() const
{
	return hWnd;
}

bool Window::getIsActive() const
{
	return isActive;
}

void Window::createWindow()
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL,
						  L"WindowClass",
						  L"Temp",
						  WS_OVERLAPPEDWINDOW,
						  100,
						  100,
						  screenWidth,
						  screenHeight,
						  NULL,
						  NULL,
						  hInstance,
						  NULL);

	ShowWindow(hWnd, cmdShow);
}

void Window::initCursor()
{
	SetCursorPos(screenWidth/2, screenHeight/2);
	
	//Capture the cursor to this window
	SetCapture(hWnd);

	//Hide the cursor from the user
	ShowCursor(false);
}

void Window::mouseDeltaMove(LPARAM lParam)
{
	//Find the upper left corner of the window's client area in screen coordinates
	POINT point;
	point.x = 0;
	point.y = 0;
	MapWindowPoints(hWnd, NULL, &point, 1);
	
	//Get current mouse position
	int mouseX = GET_X_LPARAM(lParam)+point.x;
	int mouseY = GET_Y_LPARAM(lParam)+point.y;

	//Calculate relative mouse movement
	mouseDeltaX = mouseX - screenWidth/2;
	mouseDeltaY = mouseY - screenHeight/2;

	//Return cursor to screen center
	SetCursorPos(screenWidth/2, screenHeight/2);
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
		
		keys->at(wParam) = true;

		return 0;
	}break;

	case WM_KEYUP:
	{
		keys->at(wParam) = false;

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