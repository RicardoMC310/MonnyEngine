//
// Created by ricardo on 17/07/25.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Monny {

class Logger {
    std::shared_ptr<spdlog::logger> logger;

public:
    Logger(const std::string& name) {
        logger = spdlog::stdout_color_mt(name);
        logger->set_level(spdlog::level::trace);
        logger->set_pattern("[%d/%m/%Y %H:%M:%S] [%^%l%$] [\033[1;34m%n\033[00;00m] %v");
    }

    ~Logger() = default;

    template <typename ...Args>
    void info(const std::string& msg, Args&&... args) {
        logger->info(fmt::runtime(msg), std::forward<Args>(args)...);
    }

    template <typename ...Args>
    void warn(const std::string& msg, Args&&... args) {
        logger->warn(fmt::runtime(msg), std::forward<Args>(args)...);
    }

    template <typename ...Args>
    void error(const std::string& msg, Args&&... args) {
        logger->error(fmt::runtime(msg), std::forward<Args>(args)...);
    }

    template <typename ...Args>
    void critical(const std::string& msg, Args&&... args) {
        logger->critical(fmt::runtime(msg), std::forward<Args>(args)...);
        throw std::runtime_error(fmt::format(fmt::runtime(msg), std::forward<Args>(args)...));
    }

    template <typename ...Args>
    void debug(const std::string& msg, Args&&... args) {
        logger->debug(fmt::runtime(msg), std::forward<Args>(args)...);
    }

    template <typename ...Args>
    void trace(const std::string& msg, Args&&... args) {
        logger->trace(fmt::runtime(msg), std::forward<Args>(args)...);
    }

};

} // Monny

#endif //LOGGER_H
