#pragma once

#include <memory>


#ifdef SV_PLATFORM_WINDOWS
	#ifdef SV_BUILD_DLL
		#define STRVY_API //__declspec(dllexport)
	#else
		#define STRVY_API //__declspec(dllimport)
	#endif
#else
	#error strvy only supports Windows!
#endif

#ifdef SV_DEBUG
	#define SV_ENABLE_ASSERTS
#endif

#ifdef SV_ENABLE_ASSERTS
	#define SV_ASSERT(x, ...) { if(!(x)) { SV_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define SV_CORE_ASSERT(x, ...) { if(!(x)) { SV_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define SV_ASSERT(x, ...) x
	#define SV_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define SV_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace strvy {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> createScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> createRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}