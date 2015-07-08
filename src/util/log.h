#ifndef LOG_H_INCLUDE
#define LOG_H_INCLUDE

#include <string>
#include <sstream>
#include <fstream>

typedef int LogLevel;
static const LogLevel LOG_LEVEL_DEBUG = 3;
static const LogLevel LOG_LEVEL_INFO = 2;
static const LogLevel LOG_LEVEL_WARN = 1;
static const LogLevel LOG_LEVEL_ERROR = 0;

class Log
{
public:
    static bool initialize(std::string logFile);
    static void destroy();

    static void message(std::string message);
    static void debug(std::string debug);
    static void info(std::string info);
    static void warn(std::string warning);
    static void error(std::string error);

    static void setLogLevel(const LogLevel& level) { mLogLevel = level; }
    static LogLevel getLogLevel() { return mLogLevel; }

private:
    //Log() {}
    static std::ofstream mFileStream;
    static LogLevel mLogLevel;
};

#endif // LOG_H_INCLUDE
