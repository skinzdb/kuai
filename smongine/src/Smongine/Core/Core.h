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

#define SM_ENABLE_ASSERTS // TODO: add to premake for debug configurations

#ifdef SM_ENABLE_ASSERTS
	#define SM_ASSERT(x, ...) if(!(x)) { SM_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }
	#define SM_CORE_ASSERT(x, ...) if(!(x)) { SM_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }
#else
	#define SM_ASSERT(x, ...) 
	#define SM_CORE_ASSERT(x, ...)
#endif