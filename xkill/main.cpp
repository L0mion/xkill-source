#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <xkill-renderer/renderingComponent.h>
#include "cameraBasic.h"
#include <xkill-architecture/architecture.h>
#include "window.h"

#if defined( DEBUG ) || defined( _DEBUG )
#include <vld.h>
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow);
void run(RenderingComponent* renderingComponent, Window* window, CameraBasic* camera, CameraBasic* camera2);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
	Architecture architecture;
	unsigned int test = architecture.getTest();

	UINT screenWidth = 800;
	UINT screenHeight = 800;

	UINT viewportWidth = screenWidth;
	UINT viewportHeight = screenHeight/2;
	UINT numViewports = 2;

	Window* window = new Window(hInstance, cmdShow, screenWidth, screenHeight);
	window->init();

	RenderingComponent* renderingComponent = new RenderingComponent(
		window->getWindowHandle(), 
		screenWidth, 
		screenHeight,
		viewportWidth,
		viewportHeight,
		numViewports);
	HRESULT hr = renderingComponent->init();

	float aspect = (float)viewportWidth/(float)viewportHeight;
	CameraBasic* camera = new CameraBasic(aspect, 0.78f, 1000.0f, 0.1f, screenWidth, screenHeight);
	camera->updateProj();
	CameraBasic* camera2 = new CameraBasic(aspect, 0.78f, 1000.0f, 0.1f, screenWidth, screenHeight);
	camera2->updateProj();
	camera2->updateView();

	if(SUCCEEDED(hr))
		run(renderingComponent, window, camera, camera2);

	delete camera;
	delete camera2;
	delete renderingComponent;
	delete window;

	return 0;
}

void run(RenderingComponent* renderingComponent, Window* window, CameraBasic* camera, CameraBasic* camera2)
{
	while(window->getIsActive())
	{
		window->checkMessages();
		InputContainer input = window->getInput();
		camera->mouse(input.mouseDeltaX_, input.mouseDeltaY_);
		camera->keyboard(input.keys_);
		camera->updateView();
		
		renderingComponent->clearGBuffers();

		renderingComponent->setViewport(0);
		renderingComponent->renderToGBuffer(camera->getView(), camera->getProjection());
		
		renderingComponent->setViewport(1);
		renderingComponent->renderToGBuffer(camera2->getView(), camera2->getProjection());

		renderingComponent->renderToBackBuffer();
	}
}