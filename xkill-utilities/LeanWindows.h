#ifndef LEANWINDOWS_H
#define LEANWINDOWS_H

//Substittute for <windows.h> when include is needed in .h-files.

#define WIN32_LEAN_AND_MEAN //Added 2012-12-14 14.21. Background: "window.h" include in .h-files due to failed HWND forward declarations.
	
//Copy-pasted from "windows.h". Solves the error: fatal error C1189: #error :  "No Target Architecture"
//Needed when including <windef.h>, which can be included instead of <windows.h> in .h files when needed.
#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && !defined(_ARM_) && defined(_M_IX86)
#define _X86_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && !defined(_ARM_) && defined(_M_AMD64)
#define _AMD64_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && !defined(_ARM_) && defined(_M_ARM)
#define _ARM_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && !defined(_ARM_) && defined(_M_M68K)
#define _68K_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && !defined(_ARM_) && defined(_M_MPPC)
#define _MPPC_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_M_IX86) && !defined(_AMD64_) && !defined(_ARM_) && defined(_M_IA64)
#if !defined(_IA64_)
#define _IA64_
#endif /* !_IA64_ */
#endif

#include <windef.h>

#endif