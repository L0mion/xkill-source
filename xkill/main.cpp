#include <Windows.h>
#include <windowsx.h>

#include <xkill-renderer\renderingComponent.h>
#include <xkill-renderer\window.h>

#if defined( DEBUG ) || defined( _DEBUG )
#include <vld.h>
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
	RenderingComponent renderingComponent;
	renderingComponent.youJustKissedABoomBoomBaby();

	Window window(hInstance, cmdShow, 800, 800);
	window.init();
	
	while(window.getIsActive())
	{
		window.checkMessages();
	}


	return 0;
}