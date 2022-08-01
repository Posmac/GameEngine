#pragma once

#ifdef RF_PLATFORM_WINDOWS
	#if RF_DYNAMIC_LINK
		#ifdef RF_BUILD_DLL
			#define RF_API __declspec(dllexport)
		#else
			#define RF_API __declspec(dllimport)
		#endif
	#else 
		#define RF_API
	#endif
#else
	#error Engine supports only Windows platform!
#endif

#ifdef RF_DEBUG
	#define RF_ENABLE_ASSERTS
#endif

#ifdef RF_ENABLE_ASSERTS
	#define RF_CLIENT_ASSERT(x, ...) { if(!(x)) { RF_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define RF_CORE_ASSERT(x, ...) { if(!(x)) { RF_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define RF_CLIENT_ASSERT(x, ...)
	#define RF_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1<<x)

#define RF_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)