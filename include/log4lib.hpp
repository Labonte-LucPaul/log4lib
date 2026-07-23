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

#ifndef LOG4LIB_LOG_4_LIB_INTERNAL_HPP
#define LOG4LIB_LOG_4_LIB_INTERNAL_HPP
#include <format>
#include <memory>
#include <source_location>

#include "log4libbase.hpp"

/**
 * @file log4lib.hpp
 * @brief Wrapper header file for the Log4Lib library that provides specialized functions for logging with log levels.
 * @{
 */

namespace lfl {
/**
 * @struct log
 * @brief Wrapper class for the Log4Lib library that provides specialized functions for logging with log levels.
 */
struct log final{
  log() = delete;
  ~log() = default;
  log(log&) = delete;
  log(log&&) = delete;
  /**
   * @brief Implementation of the specialized debug log function. It will not log if the logger is nullptr or the minimal
   * log level is not DEBUG.
   * @param logger The logger to use
   * @param format The format string
   * @param args The arguments to format
   * @param location The line number and file that generated the log call
   */
  template <typename... Args>
  static constexpr void debug(const std::shared_ptr<Log4LibBase>& logger, std::format_string<Args...> format,
                              Args... args,
                              const std::source_location location = std::source_location::current()) {
    if (logger != nullptr && logger->shouldLog(LogLevel::DEBUG)) {
      logger->log(LogLevel::DEBUG, std::format(format, args...), location);
    }
  }

  /**
   * @brief Implementation of the specialized info log function. It will not log if the logger is nullptr or the minimal
   * log level is not at least INFO.
   * @param logger The logger to use
   * @param format The format string
   * @param args The arguments to format
   * @param location The line number and file that generated the log call
   */
  template <typename... Args>
  static constexpr void info(const std::shared_ptr<Log4LibBase>& logger, std::format_string<Args...> format,
                             Args... args,
                             const std::source_location location = std::source_location::current()) {
    if (logger != nullptr && logger->shouldLog(LogLevel::INFO)) {
      logger->log(LogLevel::INFO, std::format(format, args...), location);
    }
  }

  /**
   * @brief Implementation of the specialized warning log function. It will not log if the logger is nullptr or the
   * minimal log level is not at least WARNING.
   * @param logger The logger to use
   * @param format The format string
   * @param args The arguments to format
   * @param location The line number and file that generated the log call
   */
  template <typename... Args>
  static constexpr void warning(const std::shared_ptr<Log4LibBase>& logger, std::format_string<Args...> format,
                                Args... args,
                                const std::source_location location = std::source_location::current()) {
    if (logger != nullptr && logger->shouldLog(LogLevel::WARNING)) {
      logger->log(LogLevel::WARNING, std::format(format, args...), location);
    }
  }

  /**
   * @brief Implementation of the specialized error log function.
   * It will not log if the logger is nullptr or the minimal log level is not at least ERROR.
   * @param logger The logger to use
   * @param format The format string
   * @param args The arguments to format
   * @param location The line number and file that generated the log call
   */
  template <typename... Args>
  static constexpr void error(const std::shared_ptr<Log4LibBase>& logger, std::format_string<Args...> format,
                              Args... args,
                              const std::source_location location = std::source_location::current()) {
    if (logger != nullptr && logger->shouldLog(LogLevel::ERROR)) {
      logger->log(LogLevel::ERROR, std::format(format, args...), location);
    }
  }

  /**
   * @brief Implementation of the specialized critical log function. It will not log if the logger is nullptr or the
   * minimal log level is not at least CRITICAL.
   * @param logger The logger to use
   * @param format The format string
   * @param args The arguments to format
   * @param location The line number and file that generated the log call
   */
  template <typename... Args>
  static constexpr void critical(const std::shared_ptr<Log4LibBase>& logger, std::format_string<Args...> format,
                                 Args... args,
                                 const std::source_location location = std::source_location::current()) {
    if (logger != nullptr && logger->shouldLog(LogLevel::CRITICAL)) {
      logger->log(LogLevel::CRITICAL, std::format(format, args...), location);
    }
  }
};
} // namespace lfl::log

/// @}

#endif  // LOG4LIB_LOG_4_LIB_INTERNAL_HPP
