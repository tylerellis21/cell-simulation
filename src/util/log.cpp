#include "log.h"
#include "timehelper.h"

#include <cassert>
#include <iostream>

//Static decleration.
std::ofstream Log::mFileStream;
LogLevel Log::mLogLevel = LOG_LEVEL_DEBUG;

bool Log::initialize(std::string logFilePath)
{
    mFileStream.open(logFilePath.c_str(), std::ios::out | std::ios::app);
    if (!mFileStream.is_open())
    {
        std::cout << "failed to open logger: " << logFilePath << std::endl;
        return false;
    }

    return true;
}

void Log::destroy()
{
    if (mFileStream.is_open())
    {
        mFileStream.close();
    }
}

void Log::message(std::string message)
{
    std::cout << "[" << currentDateTime() << "] "  << message << std::endl;
    if (mFileStream.is_open())
    {
        mFileStream << "[" << currentDateTime() << "] "  << message << std::endl;
    }
}

void Log::debug(std::string debug)
{
    if (mLogLevel >= LOG_LEVEL_DEBUG)
    {
        message(std::string(" [DEBUG] ").append(debug));
    }
}

void Log::info(std::string info)
{
    if (mLogLevel >= LOG_LEVEL_INFO)
    {
        message(std::string(" [INFO] ").append(info));
    }
}

void Log::warn(std::string warning)
{
    if (mLogLevel >= LOG_LEVEL_WARN)
    {
        message(std::string(" [WARN] ").append(warning));
    }
}

void Log::error(std::string error)
{
    if (mLogLevel >= LOG_LEVEL_ERROR)
    {
        message(std::string(" [ERROR] ").append(error));
    }
}
