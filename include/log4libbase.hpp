// MIT License
//
// Copyright (c) 2026 Luc Paul Labonté
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// MIT License
//
// Copyright (c) 2026 Luc Paul Labonté
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef LOG4LIB_LOG_4_LIB_BASE_HPP
#define LOG4LIB_LOG_4_LIB_BASE_HPP

#include <source_location>
#include <string_view>

/**
 * @file log4libbase.hpp
 * @brief Base class used by your library client to inherit from. Also Provides the log levels supported by the library.
 */

namespace lfl {
/**
 * @enum LogLevel
 * @brief The log levels supported by the library
 */
enum class LogLevel : unsigned int {
  DEBUG,    /// A verbose log level normally used for debugging
  INFO,     /// A log level used for informational messages
  WARNING,  /// A log level used for warning messages, the system might recover or be unstable
  ERROR,    /// A log level used for errors
  CRITICAL, /// A log level used for critical errors, the system might be unstable
  OFF       /// Disabled logging
};

/**
 * @class Log4LibBase
 * @brief The base class used by your library client to inherit from
 * and provide the implementation of the logImpl method
 * to log messages part of your library.
 * The default log level is OFF
 */
class Log4LibBase {
public:
  /**
   * @brief Default constructor
   */
  Log4LibBase() = default;

  /**
   * @brief Default destructor
   */
  virtual ~Log4LibBase() = default;

  /**
   * @brief Public API to call when your library needs to log a message.
   * Does not validate whether the log message should be logged.
   * @param level The log level
   * @param message The message to log
   * @param location The line number and file that generated the log call
   */
  void log(LogLevel level, const std::string_view& message,
           std::source_location location = std::source_location::current());

  /**
   * @brief Returns the minimal log level that is currently set.
   * @return The minimal log level
   */
  [[nodiscard]] inline LogLevel getMinimalLogLevel() const { return minimalLogLevel_; }

  /**
   * @brief Sets the minimal log level that is currently set.
   * @param minimalLogLevel The minimal log level
   */
  void setMinimalLogLevel(LogLevel minimalLogLevel);

  /**
   * @brief Returns whether the given log level should be logged.
   * @param level The log level
   * @return True if the log level should be logged, false otherwise
   */
  [[nodiscard]] inline bool shouldLog(const LogLevel level) const {
    return level >= minimalLogLevel_ && minimalLogLevel_ != LogLevel::OFF && level != LogLevel::OFF;
  }

protected:
  /**
   * @brief Implementation of the log function.
   * @param level The log level
   * @param message The message to log
   * @param location The line number and file that generated the log call
   */
  virtual void logImpl(LogLevel level, const std::string_view& message, std::source_location location) = 0;

private:
  LogLevel minimalLogLevel_{LogLevel::OFF}; /// The minimal log level that is currently set.
};
} // namespace lfl

#endif  // LOG4LIB_LOG_4_LIB_BASE_HPP
