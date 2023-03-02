#pragma once

#define KU_ENABLE_ASSERTS // TODO: add to premake for debug configurations

#ifdef KU_PLATFORM_WINDOWS
	#define DEBUG_BREAK __debugbreak()
#else
	#define DEBUG_BREAK raise(SIGTRAP)
#endif

#ifdef KU_ENABLE_ASSERTS
	#define KU_ASSERT(x, ...) if(!(x)) { KU_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK; }
	#define KU_CORE_ASSERT(x, ...) if(!(x)) { KU_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK; }
#else
	#define KU_ASSERT(x, ...) 
	#define KU_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
