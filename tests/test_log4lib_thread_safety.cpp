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

#include <array>
#include <atomic>
#include <cstddef>
#include <memory>
#include <mutex>
#include <source_location>
#include <string_view>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include "log4lib/log4lib.hpp"

namespace {
class ThreadSafeCountingLogger final : public lfl::Log4LibBase
{
public:
    [[nodiscard]] std::size_t count(const lfl::LogLevel level) const
    {
        const std::lock_guard lock(mutex_);
        return counts_[toIndex(level)];
    }

    [[nodiscard]] std::size_t totalCount() const
    {
        const std::lock_guard lock(mutex_);
        std::size_t total = 0U;
        for (const auto& count : counts_) {
            total += count;
        }
        return total;
    }

private:
    static constexpr std::size_t toIndex(const lfl::LogLevel level)
    {
        return static_cast<std::size_t>(level);
    }

    void logImpl(lfl::LogLevel level, const std::string_view& message, std::source_location location) override
    {
        (void)message;
        (void)location;

        const std::lock_guard lock(mutex_);
        ++counts_[toIndex(level)];
    }

    mutable std::mutex mutex_;
    std::array<std::size_t, static_cast<std::size_t>(lfl::LogLevel::OFF) + 1U> counts_{};
};

void waitForStart(const std::atomic<bool>& start)
{
    while (!start.load(std::memory_order_acquire)) {
        std::this_thread::yield();
    }
}
} // namespace

TEST(Log4LibThreadSafetyTest, concurrentLoggingExpectEveryEnabledMessageToBeWritten)
{
    constexpr std::size_t threadCount = 8U;
    constexpr std::size_t messagesPerLevelPerThread = 500U;
    constexpr std::size_t expectedPerLevel = threadCount * messagesPerLevelPerThread;

    auto logger = std::make_shared<ThreadSafeCountingLogger>();
    logger->setMinimalLogLevel(lfl::LogLevel::DEBUG);

    std::atomic_bool start{false};
    std::vector<std::thread> threads;
    threads.reserve(threadCount);

    for (std::size_t threadIndex = 0U; threadIndex < threadCount; ++threadIndex) {
        threads.emplace_back([logger, &start]() {
            waitForStart(start);

            for (std::size_t messageIndex = 0U; messageIndex < messagesPerLevelPerThread; ++messageIndex) {
                lfl::log::debug(logger, "debug message");
                lfl::log::info(logger, "info message");
                lfl::log::warning(logger, "warning message");
                lfl::log::error(logger, "error message");
                lfl::log::critical(logger, "critical message");
            }
        });
    }

    start.store(true, std::memory_order_release);
    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(logger->count(lfl::LogLevel::DEBUG), expectedPerLevel);
    EXPECT_EQ(logger->count(lfl::LogLevel::INFO), expectedPerLevel);
    EXPECT_EQ(logger->count(lfl::LogLevel::WARNING), expectedPerLevel);
    EXPECT_EQ(logger->count(lfl::LogLevel::ERROR), expectedPerLevel);
    EXPECT_EQ(logger->count(lfl::LogLevel::CRITICAL), expectedPerLevel);
    EXPECT_EQ(logger->count(lfl::LogLevel::OFF), 0);
    EXPECT_EQ(logger->totalCount(), expectedPerLevel * 5);
}

TEST(Log4LibThreadSafetyTest, concurrentLogLevelReadsAndWritesExpectConsistentObservableState)
{
    constexpr std::size_t writerThreadCount = 4U;
    constexpr std::size_t readerThreadCount = 4U;
    constexpr std::size_t iterations = 2000U;
    constexpr std::array levels{
        lfl::LogLevel::DEBUG,
        lfl::LogLevel::INFO,
        lfl::LogLevel::WARNING,
        lfl::LogLevel::ERROR,
        lfl::LogLevel::CRITICAL,
        lfl::LogLevel::OFF,
    };

    ThreadSafeCountingLogger logger;
    std::atomic_bool start{false};
    std::atomic<std::size_t> enabledDecisions{0U};
    std::vector<std::thread> threads;
    threads.reserve(writerThreadCount + readerThreadCount);

    for (std::size_t threadIndex = 0U; threadIndex < writerThreadCount; ++threadIndex) {
        threads.emplace_back([threadIndex, &logger, &levels, &start]() {
            waitForStart(start);

            for (std::size_t iteration = 0U; iteration < iterations; ++iteration) {
                logger.setMinimalLogLevel(levels[(threadIndex + iteration) % levels.size()]);
            }
        });
    }

    for (std::size_t threadIndex = 0U; threadIndex < readerThreadCount; ++threadIndex) {
        threads.emplace_back([&logger, &levels, &start, &enabledDecisions]() {
            waitForStart(start);

            for (std::size_t iteration = 0U; iteration < iterations; ++iteration) {
                for (const auto level : levels) {
                    if (logger.shouldLog(level)) {
                        enabledDecisions.fetch_add(1U, std::memory_order_relaxed);
                    }

                    const auto minimalLevel = logger.getMinimalLogLevel();
                    EXPECT_GE(static_cast<unsigned int>(minimalLevel), static_cast<unsigned int>(lfl::LogLevel::DEBUG));
                    EXPECT_LE(static_cast<unsigned int>(minimalLevel), static_cast<unsigned int>(lfl::LogLevel::OFF));
                }
            }
        });
    }

    start.store(true, std::memory_order_release);
    for (auto& thread : threads) {
        thread.join();
    }

    logger.setMinimalLogLevel(lfl::LogLevel::ERROR);
    EXPECT_EQ(logger.getMinimalLogLevel(), lfl::LogLevel::ERROR);
    EXPECT_TRUE(logger.shouldLog(lfl::LogLevel::CRITICAL));
    EXPECT_FALSE(logger.shouldLog(lfl::LogLevel::WARNING));
    EXPECT_LE(enabledDecisions.load(std::memory_order_relaxed), readerThreadCount * iterations * levels.size());
}
