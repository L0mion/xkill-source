#include <DirectXMath.h>
#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <xkill-renderer/renderingComponent.h>
//#include "cameraBasic.h"
#include <xkill-architecture/GameManager.h>
#include "window.h"


#if defined( DEBUG ) || defined( _DEBUG )

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//#include <vld.h>
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow);
void run(RenderingComponent* renderingComponent, Window* window/*, CameraBasic* camera*/);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
		// Detect memory leaks
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	UINT screenWidth = 800;
	UINT screenHeight = 800;


	Window* window = new Window(hInstance, cmdShow, screenWidth, screenHeight);
	window->init();

	AllocConsole();
	SetStdHandle(STD_INPUT_HANDLE |STD_OUTPUT_HANDLE, window->getWindowHandle());

	GameManager gm;
	gm.init(window->getWindowHandle(),screenWidth,screenHeight);
	
	while(window->getIsActive())
	{
		window->checkMessages();
		gm.run();
	}

	//RenderingComponent* renderingComponent = new RenderingComponent(
	//	window->getWindowHandle(), 
	//	screenWidth, 
	//	screenHeight,
	//	numPlayers);
	//HRESULT hr = renderingComponent->init();

	/*RenderingComponent* renderingComponent = new RenderingComponent(
		window->getWindowHandle(), 
		screenWidth, 
		screenHeight);
	HRESULT hr = renderingComponent->init();*/


	float aspect = (float)screenWidth/(float)screenHeight;
	//CameraBasic* camera = new CameraBasic(aspect, 0.78f, 1000.0f, 0.1f, screenWidth, screenHeight);
	//camera->updateProj();

	/*if(SUCCEEDED(hr))
		run(renderingComponent, window, camera);*/

	//delete camera;
	/*delete renderingComponent;*/
	if(window)
		delete window;

	return 0;
}

void run(RenderingComponent* renderingComponent, Window* window/*, CameraBasic* camera*/)
{
	while(window->getIsActive())
	{
		window->checkMessages();
		InputContainer input = window->getInput();
		//camera->mouse(input.mouseDeltaX_, input.mouseDeltaY_);
		//camera->keyboard(input.keys_);
		//camera->updateView();
		//renderingComponent->render(&camera->getView(), &camera->getProjection());
		renderingComponent->onUpdate(1.0f);
	}
}












////////////////////////////
// Main page
////////////////////////////

/** 
\mainpage Index
\htmlonly
<h1>XKILL</h1>
<h2>Heartaches by the Cycle</h2>
<p style="width: 500px;">
In a forgotten store-room rests an ancient single-core processor. In this harsh land-
scape, before multicore-technology, fierce gladiator tournaments are fough by processes
struggling to be granted execution time by The Scheduler. These tournaments consist of
deadly combat as The Processes battle to destroy one another.
</p>

<img src="http://fc07.deviantart.net/fs70/i/2012/275/6/7/concepting_by_wolfgan-d5gjy81.jpg" style="width: 20%;"/>

<iframe width="46" height="23" style="position: relative; display: block; width: 46px; height: 23px;" src="http://bandcamp.com/EmbeddedPlayer/v=2/track=1053150290/size=short/bgcol=FFFFFF/linkcol=4285BB/" allowtransparency="true" frameborder="0"><a href="http://sioum.bandcamp.com/track/chambers">Chambers by Sioum</a></iframe>

<h3>Developer Team</h3>
<ul>
<li>Mattias Andersson	</li>
<li>Nils Forsman		</li>
<li>David Grelsson		</li>
<li>Henrik Nell		 	</li>
<li>Eric Nilsson		</li>
<li>Viktor Sid&eacute;n	</li>
<li>Jon Wid&eacute;n	</li>
</ul>

<h3>Graphics Team</h3>
<ul>
<li>Oliver Blomgren		</li>
<li>Jimmy Johnsson		</li>
<li>Daniel Oldberg		</li>
<li>Adam Aberg			</li>
</ul>

<h3>Audio Team</h3>
<ul>
<li>Audio Team			</li>
<li>Johan Mauritzson	</li>
<li>Jesper Palm			</li>
</ul>

<h3>Level Design</h3>
<ul>
<li>Olle Karlsson		</li>
</ul>

\endhtmlonly



*/
