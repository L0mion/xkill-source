#ifndef XKILL_RENDERER_RENDERINGUTILITIES_H
#define XKILL_RENDERER_RENDERINGUTILITIES_H

#define SAFE_RELEASE(com)	if(com != nullptr) { com->Release();	com = nullptr; }
#define SAFE_DELETE(obj)	if(obj != nullptr) { delete obj;		obj = nullptr; }
#define SAFE_CLEAN(obj)		if(obj != nullptr) { obj->cleanUp(); }

#include <Windows.h>

static void ERROR_MSG(LPCWSTR errorMsg)
{
		//todoist: retrieve error-msg
		//_com_error error(hr);
		//LPCTSTR errorText = error.ErrorMessage();

#if defined( DEBUG ) || defined( _DEBUG )
		MessageBox(
			NULL, 
			errorMsg,
			L"HRESULT failed!",
			MB_OK | MB_ICONEXCLAMATION);
#else
	//Print message to error log.
#endif //DEBUG || _DEBUG
}

#endif //XKILL_RENDERER_RENDERINGUTILITIES_H