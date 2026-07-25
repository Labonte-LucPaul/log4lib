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

#include <syslog.h>
#include <iostream>
#include "log4lib/log4libbase.hpp"
#include "libtest/worker_in_lib.hpp"

namespace myclient {
class Wrapper final : public lfl::Log4LibBase {
public:
  Wrapper() = default;
  explicit Wrapper(lfl::LogLevel level) { setMinimalLogLevel(level); }

private:
  void logImpl(lfl::LogLevel level, const std::string_view& message, std::source_location location) override {
    std::clog << "Logging: " << message << " at level " << static_cast<int>(level) << " from " << location.file_name()
      << ":" << location.line() << std::endl;
  }
};

class SyslogWrapper final : public lfl::Log4LibBase {
public:
  SyslogWrapper() {
    openlog("myclient", LOG_PID, LOG_USER);
  }

  explicit SyslogWrapper(lfl::LogLevel level) {
    setMinimalLogLevel(level);
    openlog("myclient", LOG_PID, LOG_USER);
  }

  ~SyslogWrapper() override {
    closelog();
  }

private:
  void logImpl(lfl::LogLevel level, const std::string_view& message, std::source_location location) override {
    if (level == lfl::LogLevel::DEBUG) {
      syslog(LOG_DEBUG, "%s", message.data());
    } else if (level == lfl::LogLevel::INFO) {
      syslog(LOG_INFO, "%s", message.data());
    } else if (level == lfl::LogLevel::WARNING) {
      syslog(LOG_WARNING, "%s", message.data());
    } else if (level == lfl::LogLevel::ERROR) {
      syslog(LOG_ERR, "%s", message.data());
    } else if (level == lfl::LogLevel::CRITICAL) {
      syslog(LOG_CRIT, "%s", message.data());
    } else {
    }
  }
};
} // namespace myclient

int main() {
  mylib::WorkerInLib worker(std::make_unique<myclient::Wrapper>(lfl::LogLevel::DEBUG));
  worker.doWork();
  mylib::WorkerInLib worker2(std::make_unique<myclient::SyslogWrapper>(lfl::LogLevel::DEBUG));
  worker2.doWork();

  return 0;
}