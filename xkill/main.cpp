#include <Windows.h>
#include <windowsx.h>

#include <xkill-renderer\renderingComponent.h>
#include <xkill-renderer\window.h>

#if defined( DEBUG ) || defined( _DEBUG )
#include <vld.h>
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
	UINT screenWidth = 800;
	UINT screenHeight = 800;

	Window* window = new Window(hInstance, cmdShow, screenWidth, screenHeight);
	window->init();

	RenderingComponent* renderingComponent = new RenderingComponent(
		window->getWindowHandle(), 
		screenWidth, 
		screenHeight, 
		1);
	renderingComponent->init();

	while(window->getIsActive())
	{
		window->checkMessages();
		renderingComponent->render();
	}

	delete renderingComponent;
	delete window;


	return 0;
}