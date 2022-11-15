#pragma once

#ifdef SM_PLATFORM_WINDOWS
	#ifdef SM_BUILD_DLL
		#define SMONGINE_API __declspec(dllexport)
	#else
		#define SMONGINE_API __declspec(dllimport)
	#endif
#else
	#error Smongine only supports Windows.
#endif
