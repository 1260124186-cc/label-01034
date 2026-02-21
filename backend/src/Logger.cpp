#include "Logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

Logger* Logger::instance = nullptr;

Logger::Logger() : minLevel(LogLevel::INFO), consoleOutput(true) {}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger* Logger::getInstance() {
    if (instance == nullptr) {
        instance = new Logger();
    }
    return instance;
}

void Logger::destroyInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

void Logger::init(const std::string& logFilePath, LogLevel level, bool console) {
    minLevel = level;
    consoleOutput = console;
    if (logFile.is_open()) logFile.close();
    logFile.open(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "无法打开日志文件: " << logFilePath << std::endl;
    }
    info("日志系统初始化完成", "Logger");
}

std::string Logger::levelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO ";
        case LogLevel::WARN:  return "WARN ";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKN ";
    }
}

std::string Logger::currentTimestamp() const {
    auto now = std::time(nullptr);
    auto* tm = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void Logger::log(LogLevel level, const std::string& message, const std::string& source) {
    if (level < minLevel) return;

    std::lock_guard<std::mutex> lock(mtx);

    std::string timestamp = currentTimestamp();
    std::string lvl = levelToString(level);
    std::string src = source.empty() ? "" : "[" + source + "] ";

    std::string line = "[" + timestamp + "] [" + lvl + "] " + src + message;

    if (logFile.is_open()) {
        logFile << line << std::endl;
        logFile.flush();
    }

    if (consoleOutput && level >= LogLevel::WARN) {
        std::cerr << line << std::endl;
    }
}

void Logger::debug(const std::string& msg, const std::string& src) { log(LogLevel::DEBUG, msg, src); }
void Logger::info(const std::string& msg, const std::string& src)  { log(LogLevel::INFO, msg, src); }
void Logger::warn(const std::string& msg, const std::string& src)  { log(LogLevel::WARN, msg, src); }
void Logger::error(const std::string& msg, const std::string& src) { log(LogLevel::ERROR, msg, src); }
void Logger::fatal(const std::string& msg, const std::string& src) { log(LogLevel::FATAL, msg, src); }
