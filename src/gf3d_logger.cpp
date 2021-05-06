#include "gf3d_logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Logger::logger;

void Logger::init()
{
	auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	logger = std::make_shared<spdlog::logger>("gf3d_logger", sink);
	spdlog::register_logger(logger);
	logger->set_level(spdlog::level::trace);
	logger->set_pattern("%^[%s:%#] %v%$");
}