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

#include <iostream>
#include <log4lib/log4lib.hpp>
#include <log4lib/log4libbase.hpp>

#include <memory>

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

class WorkerInLib {
public:
  explicit WorkerInLib(std::unique_ptr<lfl::Log4LibBase> logger) : logger(std::move(logger)) {
  }

  void doWork() const {
    lfl::log::debug(logger, "Logging as DEBUG");
    lfl::log::info(logger, "Logging as INFO");
    lfl::log::warning(logger, "Logging as WARNING");
    lfl::log::error(logger, "Logging as ERROR");
    lfl::log::critical(logger, "Logging as CRITICAL");
  }

private:
  const std::shared_ptr<lfl::Log4LibBase> logger{nullptr};
};

int main() {
  WorkerInLib worker(std::make_unique<Wrapper>(lfl::LogLevel::DEBUG));
  worker.doWork();
  return 0;
}