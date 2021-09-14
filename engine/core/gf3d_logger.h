#pragma once
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>
#include "NonCopyable.h"

class Logger : NonCopyable
{
public:
	static void init(const char* filename = "gf3d.log");
	static void shutdown();
	static std::shared_ptr<spdlog::logger>& getLogger() { return logger; }
private:
	Logger();
	static std::shared_ptr<spdlog::logger> logger;
};

#define LOGGER_TRACE(...)  SPDLOG_LOGGER_TRACE(Logger::getLogger(), __VA_ARGS__);
#define LOGGER_DEBUG(...)  SPDLOG_LOGGER_DEBUG(Logger::getLogger(), __VA_ARGS__);
#define LOGGER_INFO(...)  SPDLOG_LOGGER_INFO(Logger::getLogger(), __VA_ARGS__);
#define LOGGER_WARN(...)  SPDLOG_LOGGER_WARN(Logger::getLogger(), __VA_ARGS__);
#define LOGGER_ERROR(...)  SPDLOG_LOGGER_ERROR(Logger::getLogger(), __VA_ARGS__);
#define LOGGER_CRITICAL(...)  SPDLOG_LOGGER_CRITICAL(Logger::getLogger(), __VA_ARGS__);