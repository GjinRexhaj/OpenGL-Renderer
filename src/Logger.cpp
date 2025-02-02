/*
 * Logger.cpp
 *
 *  Created on: Dec 29, 2024
 *      Author: gjin
 *
 *      Logging for this program is done via
 *      this custom logging class, this is the implementation file.
 *
 *      Logger logger("logfile.txt"); // Create logger instance
 *
 *      logger.log(INFO, "Program started.");
 *      logger.log(DEBUG, "Debugging information.");
 *      logger.log(ERROR, "An error occurred.");
 */


#include "Logger.h"
#include "framework.h"
#include <ctime>
#include <sstream>
#include <iostream>

// declare global logger
// writes to "logfile.txt"
Logger Global::logger("logfile.txt");
// declare global ImGui text buffer that's linked to the logger
ImGuiTextBuffer Global::GLlogBuffer;




using namespace std;



//ofstream Logger::logFile;


Logger::Logger(const string& filename) {
    logFile.open(filename, ios::app);
    if (!logFile.is_open()) {
        cerr << "Error opening to log file" << "\n";
    }
}

Logger::~Logger() {
    logFile.close();
}

void Logger::log(logLevel level, const string& message) {
    time_t now = time(0);
    tm* timeinfo = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp),
            "%Y-%m-%d %H:%M:%S", timeinfo);

    ostringstream logEntry;
    logEntry << "[" << timestamp << "] "
             << levelToString(level) << ": " << message
             << "\n";

    cout << logEntry.str();

    std::string print = levelToString(level) + ": " + message + "\n";
    Global::GLlogBuffer.appendf(print.c_str());
    //Global::GLlogBuffer.appendf(logEntry.str().c_str());
    //Global::GLlogBuffer.appendf(message.c_str());
    //Global::GLlogBuffer.appendf("\n");


    if (logFile.is_open()) {
        logFile << logEntry.str();
        logFile.flush();
    }
}

string Logger::levelToString(logLevel level){
    switch (level) {
    case DEBUG:
        return "DEBUG";
    case INFO:
        return "INFO";
    case WARNING:
        return "WARNING";
    case ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";

    }
}
