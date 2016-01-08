/**
 * @defgroup logging Logging
 * @brief The YDLE logging system.
 *
 * @examplepara
 * ~~~~~~~~~~~~~~~~~~~~~
 * #include <LogDestination.h>
 *
 * // Call this once
 * ydleMaster::InitLogging(ydleMaster::YDLE_LOG_WARN, ydleMaster::YDLE_LOG_STDERR);
 *
 * YDLE_FATAL << "Null pointer!";
 * YDLE_WARN << "Could not connect to server: " << ip_address;
 * YDLE_INFO << "Reading configs from " << config_dir;
 * YDLE_DEBUG << "Counter was " << counter;
 * ~~~~~~~~~~~~~~~~~~~~~
 *
 * @addtogroup logging
 * @{
 *
 * @file Logging.h
 * @brief Header file for YDLE Logging
 */
 
 #ifndef YDLELOG_H
#define YDLELOG_H

#include <sstream>

/**
 * Provide a stream interface to log a message at the specified log level.
 * Rather than calling this directly use one of the YDLE_FATAL, YDLE_WARN,
 * YDLE_INFO or YDLE_DEBUG macros.
 * @param level the log_level to log at.
 */
#define YDLE_LOG(level) (level <= ydleMaster::LogLevel()) && \
                        ydleMaster::LogLine(__FILE__, __LINE__, level).stream()
/**
 * Provide a stream to log a fatal message. e.g.
 * @code
 *     YDLE_FATAL << "Null pointer!";
 * @endcode
 */
#define YDLE_FATAL YDLE_LOG(ydleMaster::YDLE_LOG_FATAL)

/**
 * Provide a stream to log a warning message.
 * @code
 *     YDLE_WARN << "Could not connect to server: " << ip_address;
 * @endcode
 */
#define YDLE_WARN YDLE_LOG(ydleMaster::YDLE_LOG_WARN)

/**
 * Provide a stream to log an infomational message.
 * @code
 *     YDLE_INFO << "Reading configs from " << config_dir;
 * @endcode
 */
#define YDLE_INFO YDLE_LOG(ydleMaster::YDLE_LOG_INFO)

/**
 * Provide a stream to log a debug message.
 * @code
 *     YDLE_DEBUG << "Counter was " << counter;
 * @endcode
 */
#define YDLE_DEBUG YDLE_LOG(ydleMaster::YDLE_LOG_DEBUG)

namespace ydleMaster {

using std::string;

/**
 * @brief The YDLE log levels.
 * This controls the verbosity of logging. Each level also includes those below
 * it.
 */
enum log_level {
  YDLE_LOG_NONE,   /**< No messages are logged. */
  YDLE_LOG_FATAL,  /**< Fatal messages are logged. */
  YDLE_LOG_WARN,   /**< Warnings messages are logged. */
  YDLE_LOG_INFO,   /**< Informational messages are logged. */
  YDLE_LOG_DEBUG,  /**< Debug messages are logged. */
  YDLE_LOG_MAX,
};

/**
 * @private
 * @brief Application global logging level
 */
extern log_level logging_level;

/**
 * @brief The destination to write log messages to
 */
typedef enum {
  YDLE_LOG_STDERR,  /**< Log to stderr. */
  YDLE_LOG_SYSLOG,  /**< Log to syslog. */
  YDLE_LOG_NULL,
} log_output;

/**
 * @cond HIDDEN_SYMBOLS
 * @class LogLine
 * @brief A LogLine, this represents a single log message.
 */
class LogLine {
 public:
    LogLine(const char *file, int line, log_level level);
    ~LogLine();
    void Write();

    std::ostream &stream() { return m_stream; }
 private:
    log_level m_level;
    std::ostringstream m_stream;
    unsigned int m_prefix_length;
};

/**
 * @brief Fetch the current level of logging.
 * @returns the current log_level.
 */
log_level LogLevel() ;

} ; // namespace ydleMaster

#endif // YDLELOG_H