#pragma once


#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h" // (output stream operator for spdlog) for logging custom types such as our Events

namespace strvy {

	class STRVY_API Log
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_clientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_clientLogger;
		static std::shared_ptr<spdlog::logger> s_coreLogger;
	};
}


// Core log macros
#define SV_CORE_TRACE(...)   ::strvy::Log::getCoreLogger()->trace(__VA_ARGS__)
#define SV_CORE_INFO(...)    ::strvy::Log::getCoreLogger()->info(__VA_ARGS__)
#define SV_CORE_WARN(...)    ::strvy::Log::getCoreLogger()->warn(__VA_ARGS__)
#define SV_CORE_ERROR(...)   ::strvy::Log::getCoreLogger()->error(__VA_ARGS__)
#define SV_CORE_FATAL(...)   ::strvy::Log::getCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define SV_TRACE(...)   ::strvy::Log::getClientLogger()->trace(__VA_ARGS__)
#define SV_INFO(...)    ::strvy::Log::getClientLogger()->info(__VA_ARGS__)
#define SV_WARN(...)    ::strvy::Log::getClientLogger()->warn(__VA_ARGS__)
#define SV_ERROR(...)   ::strvy::Log::getClientLogger()->error(__VA_ARGS__)
#define SV_FATAL(...)   ::strvy::Log::getClientLogger()->fatal(__VA_ARGS__)