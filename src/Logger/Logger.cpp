#include "../../include/Logger/Logger.hpp"

Tasq::Logger::Logger(const std::string& name)
{
    spdlog::set_pattern("[ \033[01;95m%Y-%m-%d %H:%M:%S\033[00;00m ] [ \033[01;94m%n\033[00;00m ] [ %^%l%$ ] %v");
    logger = spdlog::stdout_color_mt(name);
    logger->set_level(spdlog::level::debug);
}