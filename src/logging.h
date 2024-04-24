//
// Created by Martin Lejko on 24/04/2024.
//

#ifndef SIMPLE_SOFTWARE_RENDERER_LOGGING_H
#define SIMPLE_SOFTWARE_RENDERER_LOGGING_H
#include "spdlog/spdlog.h"
// Set up logger

extern std::shared_ptr<spdlog::logger> logger;
void setupLogger();
#endif //SIMPLE_SOFTWARE_RENDERER_LOGGING_H
