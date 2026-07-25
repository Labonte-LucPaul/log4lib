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
#include "log4lib/log4libbase.hpp"

class Log4LibMock final : public lfl::Log4LibBase
{
public:
    ~Log4LibMock() override = default;

    MOCK_METHOD(void, logImpl, (lfl::LogLevel level, const std::string_view& message, std::source_location location),
        (override)
    );
};

TEST(Log4LibTest, validateCanLogMessageExpectLogImplToBeCalled)
{
    Log4LibMock logMock;
    EXPECT_CALL(logMock, logImpl(lfl::LogLevel::DEBUG, ::testing::HasSubstr("Hello world"), ::testing::_)).Times(1);

    logMock.log(lfl::LogLevel::DEBUG, "Hello world", std::source_location::current());
}

TEST(Log4LibTest, validateCanChangeMinimalLogLevelExpectNewMinimalLogLevel)
{
    Log4LibMock logMock;
    logMock.setMinimalLogLevel(lfl::LogLevel::DEBUG);
    EXPECT_EQ(logMock.getMinimalLogLevel(), lfl::LogLevel::DEBUG);

    logMock.setMinimalLogLevel(lfl::LogLevel::INFO);
    EXPECT_EQ(logMock.getMinimalLogLevel(), lfl::LogLevel::INFO);

    logMock.setMinimalLogLevel(lfl::LogLevel::WARNING);
    EXPECT_EQ(logMock.getMinimalLogLevel(), lfl::LogLevel::WARNING);

    logMock.setMinimalLogLevel(lfl::LogLevel::ERROR);
    EXPECT_EQ(logMock.getMinimalLogLevel(), lfl::LogLevel::ERROR);

    logMock.setMinimalLogLevel(lfl::LogLevel::CRITICAL);
    EXPECT_EQ(logMock.getMinimalLogLevel(), lfl::LogLevel::CRITICAL);

    logMock.setMinimalLogLevel(lfl::LogLevel::OFF);
    EXPECT_EQ(logMock.getMinimalLogLevel(), lfl::LogLevel::OFF);
}

TEST(Log4LibTest, validateShouldLogOnAllLevelExpectTrueIfThresholdFilterIsGreaterOrEqualThanMinimalLevel)
{
    Log4LibMock logMock;

    logMock.setMinimalLogLevel(lfl::LogLevel::DEBUG);
    EXPECT_TRUE(logMock.shouldLog(lfl::LogLevel::DEBUG));
    EXPECT_TRUE(logMock.shouldLog(lfl::LogLevel::INFO));
    EXPECT_TRUE(logMock.shouldLog(lfl::LogLevel::WARNING));
    EXPECT_TRUE(logMock.shouldLog(lfl::LogLevel::ERROR));
    EXPECT_TRUE(logMock.shouldLog(lfl::LogLevel::CRITICAL));
    EXPECT_FALSE(logMock.shouldLog(lfl::LogLevel::OFF));

    logMock.setMinimalLogLevel(lfl::LogLevel::INFO);
    EXPECT_FALSE(logMock.shouldLog(lfl::LogLevel::DEBUG));
    EXPECT_TRUE(logMock.shouldLog(lfl::LogLevel::INFO));
    EXPECT_TRUE(logMock.shouldLog(lfl::LogLevel::WARNING));
    EXPECT_TRUE(logMock.shouldLog(lfl::LogLevel::ERROR));
    EXPECT_TRUE(logMock.shouldLog(lfl::LogLevel::CRITICAL));
    EXPECT_FALSE(logMock.shouldLog(lfl::LogLevel::OFF));

    logMock.setMinimalLogLevel(lfl::LogLevel::WARNING);
    EXPECT_FALSE(logMock.shouldLog(lfl::LogLevel::DEBUG));
    EXPECT_FALSE(logMock.shouldLog(lfl::LogLevel::INFO));
    EXPECT_TRUE(logMock.shouldLog(lfl::LogLevel::WARNING));
    EXPECT_TRUE(logMock.shouldLog(lfl::LogLevel::ERROR));
    EXPECT_TRUE(logMock.shouldLog(lfl::LogLevel::CRITICAL));
    EXPECT_FALSE(logMock.shouldLog(lfl::LogLevel::OFF));

    logMock.setMinimalLogLevel(lfl::LogLevel::ERROR);
    EXPECT_FALSE(logMock.shouldLog(lfl::LogLevel::DEBUG));
    EXPECT_FALSE(logMock.shouldLog(lfl::LogLevel::INFO));
    EXPECT_FALSE(logMock.shouldLog(lfl::LogLevel::WARNING));
    EXPECT_TRUE(logMock.shouldLog(lfl::LogLevel::ERROR));
    EXPECT_TRUE(logMock.shouldLog(lfl::LogLevel::CRITICAL));
    EXPECT_FALSE(logMock.shouldLog(lfl::LogLevel::OFF));

    logMock.setMinimalLogLevel(lfl::LogLevel::OFF);
    EXPECT_FALSE(logMock.shouldLog(lfl::LogLevel::DEBUG));
    EXPECT_FALSE(logMock.shouldLog(lfl::LogLevel::INFO));
    EXPECT_FALSE(logMock.shouldLog(lfl::LogLevel::WARNING));
    EXPECT_FALSE(logMock.shouldLog(lfl::LogLevel::ERROR));
    EXPECT_FALSE(logMock.shouldLog(lfl::LogLevel::CRITICAL));
    EXPECT_FALSE(logMock.shouldLog(lfl::LogLevel::OFF));
}
