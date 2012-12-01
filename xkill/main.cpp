#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <xkill-renderer/renderingComponent.h>
#include "cameraBasic.h"
#include <xkill-architecture/architecture.h>
#include "window.h"

#if defined( DEBUG ) || defined( _DEBUG )
//#include <vld.h>
#endif

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#ifdef _DEBUG   
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )      
#define new DBG_NEW   
#endif
//#endif  // _DEBUG

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow);
void run(RenderingComponent* renderingComponent, Window* window, CameraBasic* camera);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
	// Detect memory leaks
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	Architecture architecture;
	unsigned int test = architecture.getTest();

	UINT screenWidth = 800;
	UINT screenHeight = 800;

	Window* window = new Window(hInstance, cmdShow, screenWidth, screenHeight);
	window->init();

	RenderingComponent* renderingComponent = new RenderingComponent(
		window->getWindowHandle(), 
		screenWidth, 
		screenHeight);
	HRESULT hr = renderingComponent->init();

	float aspect = (float)screenWidth/(float)screenHeight;
	CameraBasic* camera = new CameraBasic(aspect, 0.78f, 1000.0f, 0.1f, screenWidth, screenHeight);
	camera->updateProj();

	if(SUCCEEDED(hr))
		run(renderingComponent, window, camera);

	delete camera;
	delete renderingComponent;
	delete window;

	//_CrtDumpMemoryLeaks();

	return 0;
}

void run(RenderingComponent* renderingComponent, Window* window, CameraBasic* camera)
{
	while(window->getIsActive())
	{
		window->checkMessages();
		InputContainer input = window->getInput();
		camera->mouse(input.mouseDeltaX_, input.mouseDeltaY_);
		camera->keyboard(input.keys_);
		camera->updateView();
		renderingComponent->render(camera->getView(), camera->getProjection());
	}
}