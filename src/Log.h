#pragma once

#include "Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace AE
{
    class ARNOLD_API Log
    {
    public:
        static void init();
        inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

// Core log macros
#define AE_CORE_TRACE(...) AE::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AE_CORE_INFO(...)  AE::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AE_CORE_WARN(...)  AE::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AE_CORE_ERROR(...) AE::Log::GetCoreLogger()->error(__VA_ARGS__)
#define AE_CORE_FATAL(...) AE::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define AE_TRACE(...)  AE::Log::GetClientLogger()->trace(__VA_ARGS__)
#define AE_INFO(...)  AE::Log::GetClientLogger()->info(__VA_ARGS__)
#define AE_WARN(...)  AE::Log::GetClientLogger()->warn(__VA_ARGS__)
#define AE_ERROR(...) AE::Log::GetClientLogger()->error(__VA_ARGS__)
#define AE_FATAL(...) AE::Log::GetClientLogger()->fatal(__VA_ARGS__)