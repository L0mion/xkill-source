#ifndef XKILL_RENDERER_RENDERINGUTILITIES_H
#define XKILL_RENDERER_RENDERINGUTILITIES_H

#define SAFE_RELEASE(com)	if(com != nullptr) { com->Release();	com = nullptr;	}
#define SAFE_DELETE(obj)	if(obj != nullptr) { delete obj;		obj = nullptr;	}
#define SAFE_RESET(obj)		if(obj != nullptr) { obj->reset();						}

#pragma comment( lib, "dxguid.lib")

#include <Windows.h>
#include <d3d11.h>
#include <string>
#include <D3DCommon.h>

#include <dbghelp.h>
#include <shellapi.h>
#include <shlobj.h>

//! Prints an error-msg depending on current configuration.
	/*!
	Will output a MessageBox if DEBUG- or _DEBUG-flags are set. Will otherwise simply return without printing.
	\param clue Used to, in some way, signify where the error occured. A good practice is to input location of the function call in question, e.g. ERROR_MSG("BrokenClass::brokenFunc brokenCall"); 
	\sa MessageBox */
static void ERROR_MSG(LPCWSTR clue)
{
		//todoist: retrieve error-msg
		//_com_error error(hr);
		//LPCTSTR errorText = error.ErrorMessage();

//#if defined( DEBUG ) || defined( _DEBUG )
		MessageBox(
			NULL, 
			clue,
			L"HRESULT failed!",
			MB_OK | MB_ICONEXCLAMATION);
//#else
	//Print message to error log.
//#endif //DEBUG || _DEBUG
}

static void ERROR_MSG(std::string clue)
{
//#if defined( DEBUG ) || defined( _DEBUG )
	MessageBoxA(
		NULL, 
		clue.c_str(),
		"Something failed!",
		MB_OK | MB_ICONEXCLAMATION);
//#else
	//Print message to error log.
//#endif //DEBUG || _DEBUG
}

template<UINT TNameLength>
inline void SET_D3D_OBJECT_NAME(
	_In_	ID3D11DeviceChild* resource,
	_In_z_	const char (&name)[TNameLength])
{
#if defined(D3D_PROFILE)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
#endif //D3D_PROFILE
}

#endif //XKILL_RENDERER_RENDERINGUTILITIES_H