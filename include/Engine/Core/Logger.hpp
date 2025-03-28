#pragma once

#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <fstream>
#include <mutex>
#include <functional>

namespace Engine
{

    /**
     * @brief Log level enumeration
     */
    enum class LogLevel
    {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };

    /**
     * @brief Logger configuration
     */
    struct LoggerConfig
    {
        LogLevel level;
        bool logToConsole;
        bool logToFile;
        std::string logFilePath;
    };

    /**
     * @brief Logger sink interface
     *
     * A sink is an output destination for log messages.
     */
    class LogSink
    {
    public:
        /**
         * @brief Virtual destructor
         */
        virtual ~LogSink() = default;

        /**
         * @brief Writes a log message to the sink
         * @param level Log level
         * @param message Log message
         * @param timestamp Timestamp
         */
        virtual void write(LogLevel level, const std::string &message, const std::string &timestamp) = 0;
    };

    /**
     * @brief Console log sink
     */
    class ConsoleSink : public LogSink
    {
    public:
        /**
         * @brief Constructor
         */
        ConsoleSink();

        /**
         * @brief Writes a log message to the console
         * @param level Log level
         * @param message Log message
         * @param timestamp Timestamp
         */
        void write(LogLevel level, const std::string &message, const std::string &timestamp) override;
    };

    /**
     * @brief File log sink
     */
    class FileSink : public LogSink
    {
    public:
        /**
         * @brief Constructor
         * @param filePath Path to the log file
         */
        explicit FileSink(const std::string &filePath);

        /**
         * @brief Destructor
         */
        ~FileSink() override;

        /**
         * @brief Writes a log message to the file
         * @param level Log level
         * @param message Log message
         * @param timestamp Timestamp
         */
        void write(LogLevel level, const std::string &message, const std::string &timestamp) override;

    private:
        /**
         * @brief Log file
         */
        std::ofstream file;
    };

    /**
     * @brief Static logger class
     *
     * The logger class provides static methods for logging messages at different levels.
     */
    class Logger
    {
    public:
        /**
         * @brief Initializes the logger
         * @param level Minimum log level
         */
        static void init(LogLevel level = LogLevel::Info);

        /**
         * @brief Initializes the logger with custom configuration
         * @param config Logger configuration
         */
        static void init(const LoggerConfig &config);

        /**
         * @brief Shuts down the logger
         */
        static void shutdown();

        /**
         * @brief Sets the minimum log level
         * @param level Minimum log level
         */
        static void setLevel(LogLevel level);

        /**
         * @brief Gets the minimum log level
         * @return Minimum log level
         */
        static LogLevel getLevel();

        /**
         * @brief Adds a sink to the logger
         * @param sink Sink to add
         */
        static void addSink(std::unique_ptr<LogSink> sink);

        /**
         * @brief Logs a trace message
         * @param message Message to log
         */
        static void trace(const std::string &message);

        /**
         * @brief Logs a debug message
         * @param message Message to log
         */
        static void debug(const std::string &message);

        /**
         * @brief Logs an info message
         * @param message Message to log
         */
        static void info(const std::string &message);

        /**
         * @brief Logs a warning message
         * @param message Message to log
         */
        static void warning(const std::string &message);

        /**
         * @brief Logs an error message
         * @param message Message to log
         */
        static void error(const std::string &message);

        /**
         * @brief Logs a fatal message
         * @param message Message to log
         */
        static void fatal(const std::string &message);

        /**
         * @brief Logs a message with a specific level
         * @param level Log level
         * @param message Message to log
         */
        static void log(LogLevel level, const std::string &message);

    private:
        /**
         * @brief Gets the string representation of a log level
         * @param level Log level
         * @return String representation
         */
        static std::string levelToString(LogLevel level);

        /**
         * @brief Gets the current timestamp
         * @return Current timestamp
         */
        static std::string getTimestamp();

        /**
         * @brief Minimum log level
         */
        static LogLevel minLevel;

        /**
         * @brief List of sinks
         */
        static std::vector<std::unique_ptr<LogSink>> sinks;

        /**
         * @brief Mutex for thread-safe logging
         */
        static std::mutex mutex;

        /**
         * @brief Flag indicating if the logger is initialized
         */
        static bool initialized;
    };

} // namespace Engine