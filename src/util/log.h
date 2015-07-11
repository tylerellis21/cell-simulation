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

/**
 * @brief This class is used to write time stamped log messages.
 */
class Log
{
public:

    /**
     * @brief Initialize the log file.
     * @param logFile = The file path to log to.
     * @return True if sucessful.
     */
    static bool initialize(std::string logFile);

    /**
     * @brief Destroy the log file closeing the open resources.
     */
    static void destroy();

    /**
     * @brief Write a message to the log.
     * @param message = The message to write.
     */
    static void message(std::string message);

    /**
     * @brief Write a debug message to the log.
     * @param debug = The debug message.
     */
    static void debug(std::string debug);

    /**
     * @brief Write an info message to the log.
     * @param info = The info message.
     */
    static void info(std::string info);

    /**
     * @brief Write a warning message to the log.
     * @param warning = The warning message.
     */
    static void warn(std::string warning);

    /**
     * @brief Write an error to the log.
     * @param error = The error message.
     */
    static void error(std::string error);

    /**
     * @brief Set the log level filter level.
     * @param level = The level of log messages to filter out.
     */
    static void setLogLevel(const LogLevel& level) { mLogLevel = level; }

    /**
     * @brief Get the current log filter level.
     * @return The current log filter level.
     */
    static LogLevel getLogLevel() { return mLogLevel; }

private:

    /**
     * @brief mFileStream
     */
    static std::ofstream mFileStream;

    /**
     * @brief mLogLevel
     */
    static LogLevel mLogLevel;
};

#endif // LOG_H_INCLUDE
