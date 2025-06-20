#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <string>
#include <memory>

namespace Tasq
{

    class Logger
    {
        std::shared_ptr<spdlog::logger> logger;

    public:
        Logger(const std::string &name);
        ~Logger() = default;

        template <typename... Args>
        void info(const std::string &fmt, Args &&...args)
        {
            std::string formatted = fmt::format(fmt, std::forward<Args>(args)...);
            logger->info(formatted);
        }

        template <typename... Args>
        void warn(const std::string &fmt, Args &&...args)
        {
            std::string formatted = fmt::format(fmt, std::forward<Args>(args)...);
            logger->warn(formatted);
        }

        template <typename... Args>
        void error(const std::string &fmt, Args &&...args)
        {
            std::string formatted = fmt::format(fmt, std::forward<Args>(args)...);
            logger->error(formatted);
        }

        template <typename... Args>
        void debug(const std::string &fmt, Args &&...args)
        {
            std::string formatted = fmt::format(fmt, std::forward<Args>(args)...);
            logger->debug(formatted);
        }

        template <typename... Args>
        void critical(const std::string &fmt, Args &&...args)
        {
            std::string formatted = fmt::format(fmt, std::forward<Args>(args)...);
            logger->critical(formatted);
            throw std::runtime_error(formatted);
        }

    };

}