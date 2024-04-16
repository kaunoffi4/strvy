#pragma once


#ifdef SV_PLATFORM_WINDOWS
	#ifdef SV_BUILD_DLL
		#define STRVY_API __declspec(dllexport)
	#else
		#define STRVY_API __declspec(dllimport)
	#endif
#else
	#error strvy only supports Windows!
#endif