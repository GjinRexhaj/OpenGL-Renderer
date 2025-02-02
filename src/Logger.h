/*
 * Logger.h
 *
 *  Created on: Dec 29, 2024
 *      Author: gjin
 *
 *      Logging for this program is done via
 *      this custom logging class, this is the header file.
 *
 *      Logger logger("logfile.txt"); // Create logger instance
 *
 *      logger.log(INFO, "Program started.");
 *      logger.log(DEBUG, "Debugging information.");
 *      logger.log(ERROR, "An error occurred.");
 */

#pragma once

#include "framework.h"
#include <fstream>
#include <string>


enum logLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:

    Logger(const std::string& filename);
    ~Logger();
    void log(logLevel level, const std::string& message);

private:
    std::ofstream logFile;
    std::string levelToString(logLevel level);



};


// CREATE GLOBAL INSTANCE OF LOGGER
// INSTANTIATE WITH "logfile.txt"
class Global {
public:
    static Logger logger;
    static ImGuiTextBuffer GLlogBuffer;
};








