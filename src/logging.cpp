//
// Created by Martin Lejko on 23/04/2024.
//
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "logging.h"
std::shared_ptr<spdlog::logger> logger;

void setupLogger() {
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("events.log", true);
    file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%t] [%^%l%$] [%s:%#] %v");
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%t] [%^%l%$] [%s:%#] %v");
    logger = std::make_shared<spdlog::logger>("renderer_logger", spdlog::sinks_init_list{file_sink, console_sink});
    logger->set_level(spdlog::level::debug);
    logger->debug("Logger setup completed.");
}


