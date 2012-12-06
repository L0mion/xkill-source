
#include "MainWindow.h"
#include <QtGui/QApplication>

#include <vld.h>

/// If you can't tell what the main() is for you should have your programming license revoked
/** 
*/

int main(int argc, char *argv[])
{
	// Detect memory leaks
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Create MainWindow
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	// Run Qt
	return a.exec();
}
