#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

// 日志级别
enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3,
    FATAL = 4
};

// 单例模式 - 日志记录器
class Logger {
private:
    static Logger* instance;
    std::ofstream logFile;
    LogLevel minLevel;
    bool consoleOutput;
    std::mutex mtx;

    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string levelToString(LogLevel level) const;
    std::string currentTimestamp() const;

public:
    static Logger* getInstance();
    static void destroyInstance();

    void init(const std::string& logFilePath, LogLevel level = LogLevel::INFO, bool console = true);
    void setLevel(LogLevel level) { minLevel = level; }

    void log(LogLevel level, const std::string& message, const std::string& source = "");
    void debug(const std::string& msg, const std::string& src = "");
    void info(const std::string& msg, const std::string& src = "");
    void warn(const std::string& msg, const std::string& src = "");
    void error(const std::string& msg, const std::string& src = "");
    void fatal(const std::string& msg, const std::string& src = "");
};

#endif // LOGGER_H
