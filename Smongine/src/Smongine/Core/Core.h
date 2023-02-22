#pragma once

#define SM_ENABLE_ASSERTS // TODO: add to premake for debug configurations

#ifdef SM_PLATFORM_WINDOWS
	#define DEBUG_BREAK __debugbreak()
#else
	#define DEBUG_BREAK raise(SIGTRAP)
#endif

#ifdef SM_ENABLE_ASSERTS
	#define SM_ASSERT(x, ...) if(!(x)) { SM_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK; }
	#define SM_CORE_ASSERT(x, ...) if(!(x)) { SM_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK; }
#else
	#define SM_ASSERT(x, ...) 
	#define SM_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
