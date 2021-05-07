#include "gf3d_logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <vector>

std::shared_ptr<spdlog::logger> Logger::logger;

void Logger::init()
{
	std::vector<spdlog::sink_ptr> sinks;
	auto colorSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	colorSink->set_pattern("%^[%s:%#] %v%$");

	auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("gf3d.log", true);
	fileSink->set_pattern("[%T] [%l] [%s:%#] %v");

	sinks.push_back(colorSink);
	sinks.push_back(fileSink);

	logger = std::make_shared<spdlog::logger>("gf3d_logger", sinks.begin(), sinks.end());
	spdlog::register_logger(logger);
	logger->set_level(spdlog::level::trace);
	logger->flush_on(spdlog::level::trace);

	LOGGER_INFO("====Logger initialized====");
}

void Logger::shutdown()
{
	logger->flush();
	LOGGER_INFO("====Program End====");
}
