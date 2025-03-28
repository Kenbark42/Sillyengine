#include "Engine/Core/Logger.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>

namespace Engine
{

    // Initialize static members
    LogLevel Logger::minLevel = LogLevel::Info;
    std::vector<std::unique_ptr<LogSink>> Logger::sinks;
    std::mutex Logger::mutex;
    bool Logger::initialized = false;

    // Console sink implementation
    ConsoleSink::ConsoleSink() {}

    void ConsoleSink::write(LogLevel level, const std::string &message, const std::string &timestamp)
    {
        // ANSI color codes for different log levels
        const char *color = "";
        const char *resetColor = "\033[0m";

        // Select color based on log level
        switch (level)
        {
        case LogLevel::Trace:
            color = "\033[90m";
            break; // Gray
        case LogLevel::Debug:
            color = "\033[37m";
            break; // White
        case LogLevel::Info:
            color = "\033[32m";
            break; // Green
        case LogLevel::Warning:
            color = "\033[33m";
            break; // Yellow
        case LogLevel::Error:
            color = "\033[31m";
            break; // Red
        case LogLevel::Fatal:
            color = "\033[35m";
            break; // Magenta
        default:
            color = "\033[0m";
            break; // Reset
        }

        // Output format: [TIMESTAMP] [LEVEL] MESSAGE
        std::cout << color << "[" << timestamp << "] [" << Logger::levelToString(level) << "] "
                  << message << resetColor << std::endl;
    }

    // File sink implementation
    FileSink::FileSink(const std::string &filePath)
    {
        file.open(filePath, std::ios::out | std::ios::app);

        if (!file.is_open())
        {
            std::cerr << "Failed to open log file: " << filePath << std::endl;
        }
    }

    FileSink::~FileSink()
    {
        if (file.is_open())
        {
            file.close();
        }
    }

    void FileSink::write(LogLevel level, const std::string &message, const std::string &timestamp)
    {
        if (file.is_open())
        {
            // Output format: [TIMESTAMP] [LEVEL] MESSAGE
            file << "[" << timestamp << "] [" << Logger::levelToString(level) << "] "
                 << message << std::endl;
        }
    }

    // Logger implementation
    void Logger::init(LogLevel level)
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (initialized)
        {
            return;
        }

        minLevel = level;

        // Add console sink by default
        sinks.push_back(std::make_unique<ConsoleSink>());

        initialized = true;
    }

    void Logger::init(const LoggerConfig &config)
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (initialized)
        {
            return;
        }

        minLevel = config.level;

        // Add sinks based on configuration
        if (config.logToConsole)
        {
            sinks.push_back(std::make_unique<ConsoleSink>());
        }

        if (config.logToFile)
        {
            sinks.push_back(std::make_unique<FileSink>(config.logFilePath));
        }

        initialized = true;
    }

    void Logger::shutdown()
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (!initialized)
        {
            return;
        }

        // Clear all sinks
        sinks.clear();

        initialized = false;
    }

    void Logger::setLevel(LogLevel level)
    {
        std::lock_guard<std::mutex> lock(mutex);
        minLevel = level;
    }

    LogLevel Logger::getLevel()
    {
        return minLevel;
    }

    void Logger::addSink(std::unique_ptr<LogSink> sink)
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (!initialized)
        {
            init();
        }

        sinks.push_back(std::move(sink));
    }

    void Logger::trace(const std::string &message)
    {
        log(LogLevel::Trace, message);
    }

    void Logger::debug(const std::string &message)
    {
        log(LogLevel::Debug, message);
    }

    void Logger::info(const std::string &message)
    {
        log(LogLevel::Info, message);
    }

    void Logger::warning(const std::string &message)
    {
        log(LogLevel::Warning, message);
    }

    void Logger::error(const std::string &message)
    {
        log(LogLevel::Error, message);
    }

    void Logger::fatal(const std::string &message)
    {
        log(LogLevel::Fatal, message);
    }

    void Logger::log(LogLevel level, const std::string &message)
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (!initialized)
        {
            init();
        }

        if (level < minLevel)
        {
            return;
        }

        // Get timestamp
        std::string timestamp = getTimestamp();

        // Write to all sinks
        for (const auto &sink : sinks)
        {
            sink->write(level, message, timestamp);
        }
    }

    std::string Logger::levelToString(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Trace:
            return "TRACE";
        case LogLevel::Debug:
            return "DEBUG";
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Warning:
            return "WARNING";
        case LogLevel::Error:
            return "ERROR";
        case LogLevel::Fatal:
            return "FATAL";
        default:
            return "UNKNOWN";
        }
    }

    std::string Logger::getTimestamp()
    {
        // Get current time
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      now.time_since_epoch()) %
                  1000;

        // Format: YYYY-MM-DD HH:MM:SS.mmm
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();

        return ss.str();
    }

} // namespace Engine