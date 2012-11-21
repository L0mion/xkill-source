#ifndef XKILL_RENDERER_RENDERINGUTILITIES_H
#define XKILL_RENDERER_RENDERINGUTILITIES_H

#define SAFE_RELEASE(com)	if(com != nullptr) { com->Release();	com = nullptr; }
#define SAFE_DELETE(obj)	if(obj != nullptr) { delete obj;		obj = nullptr; }

#include <Windows.h>

//! Prints an error-msg depending on current configuration.
	/*!
	Will output a MessageBox if DEBUG- or _DEBUG-flags are set. Will otherwize simply return without printing.
	\param clue Used to, in some way, signify where the error occured. A good practice is to input location of the function call in question, e.g. ERROR_MSG("BrokenClass::brokenFunc brokenCall"); 
	\sa MessageBox */
static void ERROR_MSG(LPCWSTR clue)
{
		//todoist: retrieve error-msg
		//_com_error error(hr);
		//LPCTSTR errorText = error.ErrorMessage();

#if defined( DEBUG ) || defined( _DEBUG )
		MessageBox(
			NULL, 
			clue,
			L"HRESULT failed!",
			MB_OK | MB_ICONEXCLAMATION);
#else
	//Print message to error log.
#endif //DEBUG || _DEBUG
}

#endif //XKILL_RENDERER_RENDERINGUTILITIES_H