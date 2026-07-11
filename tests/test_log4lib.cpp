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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <source_location>
#include "../include/log4lib.hpp"
#include "../include/log4libbase.hpp"

class Log4LibMock final : public lfl::Log4LibBase
{
public:
    ~Log4LibMock() override = default;

    MOCK_METHOD(void, logImpl, (lfl::LogLevel level, const std::string_view& message, std::source_location location),
        (override)
    );
};

TEST(Log4LibTest, validateLogDebugExpectOnlyDebugLevelToLog)
{
    auto logMock = std::make_shared<Log4LibMock>();
    logMock->setMinimalLogLevel(lfl::LogLevel::DEBUG);

    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::DEBUG, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(1);
    lfl::log::debug(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::INFO);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::DEBUG, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(0);
    lfl::log::debug(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::WARNING);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::DEBUG, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(0);
    lfl::log::debug(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::ERROR);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::DEBUG, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(0);
    lfl::log::debug(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::CRITICAL);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::DEBUG, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(0);
    lfl::log::debug(logMock, "Hello world", std::source_location::current());
}

TEST(Log4LibTest, validateLogInfoExpectOnlyInfoLevelAndAbovePriorityToLog)
{
    auto logMock = std::make_shared<Log4LibMock>();

    logMock->setMinimalLogLevel(lfl::LogLevel::DEBUG);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::INFO, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(1);
    lfl::log::info(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::INFO);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::INFO, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(1);
    lfl::log::info(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::WARNING);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::INFO, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(0);
    lfl::log::info(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::ERROR);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::INFO, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(0);
    lfl::log::info(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::CRITICAL);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::INFO, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(0);
    lfl::log::info(logMock, "Hello world", std::source_location::current());
}

TEST(Log4LibTest, validateLogWarningExpectOnlyWarningLevelAndAbovePriorityToLog)
{
    auto logMock = std::make_shared<Log4LibMock>();
    logMock->setMinimalLogLevel(lfl::LogLevel::DEBUG);

    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::WARNING, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(1);
    lfl::log::warning(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::INFO);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::WARNING, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(1);
    lfl::log::warning(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::WARNING);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::WARNING, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(1);
    lfl::log::warning(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::ERROR);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::WARNING, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(0);
    lfl::log::warning(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::CRITICAL);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::WARNING, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(0);
    lfl::log::warning(logMock, "Hello world", std::source_location::current());
}

TEST(Log4LibTest, validateLogErrorExpectOnlyErrorLevelAndAbovePriorityToLog)
{
    auto logMock = std::make_shared<Log4LibMock>();

    logMock->setMinimalLogLevel(lfl::LogLevel::DEBUG);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::ERROR, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(1);
    lfl::log::error(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::INFO);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::ERROR, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(1);
    lfl::log::error(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::WARNING);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::ERROR, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(1);
    lfl::log::error(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::ERROR);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::ERROR, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(1);
    lfl::log::error(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::CRITICAL);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::ERROR, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(0);
    lfl::log::error(logMock, "Hello world", std::source_location::current());
}

TEST(Log4LibTest, validateLogCriticalExpectLogOnAllLevel)
{
    auto logMock = std::make_shared<Log4LibMock>();

    logMock->setMinimalLogLevel(lfl::LogLevel::DEBUG);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::CRITICAL, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(1);
    lfl::log::critical(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::INFO);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::CRITICAL, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(1);
    lfl::log::critical(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::WARNING);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::CRITICAL, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(1);
    lfl::log::critical(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::ERROR);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::CRITICAL, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(1);
    lfl::log::critical(logMock, "Hello world", std::source_location::current());

    logMock->setMinimalLogLevel(lfl::LogLevel::CRITICAL);
    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::CRITICAL, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(1);
    lfl::log::critical(logMock, "Hello world", std::source_location::current());
}

TEST(Log4LibTest, validateLogLevelOffExpectNoLog)
{
    auto logMock = std::make_shared<Log4LibMock>();
    logMock->setMinimalLogLevel(lfl::LogLevel::OFF);

    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::DEBUG, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(0);
    lfl::log::debug(logMock, "Hello world", std::source_location::current());

    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::INFO, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(0);
    lfl::log::info(logMock, "Hello world", std::source_location::current());

    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::WARNING, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(0);
    lfl::log::warning(logMock, "Hello world", std::source_location::current());

    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::ERROR, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(0);
    lfl::log::error(logMock, "Hello world", std::source_location::current());

    EXPECT_CALL(*logMock, logImpl(lfl::LogLevel::CRITICAL, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(0);
    lfl::log::critical(logMock, "Hello world", std::source_location::current());
}
