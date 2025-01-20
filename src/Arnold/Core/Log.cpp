#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace AE::Core {
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::init() {
        try {
            // Create loggers first
            s_CoreLogger = spdlog::stdout_color_mt("ARNOLD_ENGINE");
            s_ClientLogger = spdlog::stdout_color_mt("APP");

            // Then configure CoreLogger
            s_CoreLogger->set_pattern("%^[%T] %n: %v%$");
            s_CoreLogger->set_level(spdlog::level::trace);

            // Then configure ClientLogger
            s_ClientLogger->set_pattern("%^[%T] %n: %v%$");
            s_ClientLogger->set_level(spdlog::level::trace);

#if defined(__APPLE__)
            // Apply colors only after loggers are fully initialized
            if (auto core_sink = dynamic_cast<spdlog::sinks::stdout_color_sink_mt*>(s_CoreLogger->sinks()[0].get())) {
                core_sink->set_color(spdlog::level::trace, core_sink->blue);
                core_sink->set_color(spdlog::level::debug, core_sink->blue);
                core_sink->set_color(spdlog::level::info, core_sink->blue);
                core_sink->set_color(spdlog::level::warn, core_sink->blue);
                core_sink->set_color(spdlog::level::err, core_sink->blue);
                core_sink->set_color(spdlog::level::critical, core_sink->blue);
            }

            if (auto client_sink = dynamic_cast<spdlog::sinks::stdout_color_sink_mt*>(s_ClientLogger->sinks()[0].get())) {
                client_sink->set_color(spdlog::level::trace, client_sink->green);
                client_sink->set_color(spdlog::level::debug, client_sink->green);
                client_sink->set_color(spdlog::level::info, client_sink->green);
                client_sink->set_color(spdlog::level::warn, client_sink->green);
                client_sink->set_color(spdlog::level::err, client_sink->green);
                client_sink->set_color(spdlog::level::critical, client_sink->green);
            }
#endif
        }
        catch (const spdlog::spdlog_ex& ex) {
            std::cerr << "Log initialization failed: " << ex.what() << std::endl;
        }
    }
}
