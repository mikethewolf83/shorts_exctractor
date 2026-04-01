#include <gtest/gtest.h>
#include "time_parser.h"

TEST(TimeParserTest, ParseSeconds) {
    EXPECT_DOUBLE_EQ(parseTime("60"), 60.0);
    EXPECT_DOUBLE_EQ(parseTime("125.5"), 125.5);
}

TEST(TimeParserTest, ParseHMS) {
    EXPECT_DOUBLE_EQ(parseTime("00:01:00"), 60.0);
    EXPECT_DOUBLE_EQ(parseTime("00:01:30"), 90.0);
    EXPECT_DOUBLE_EQ(parseTime("01:00:00"), 3600.0);
    EXPECT_DOUBLE_EQ(parseTime("01:30:45.5"), 5445.5);
}

TEST(TimeParserTest, FormatTime) {
    EXPECT_EQ(formatTime(60.0), "00:01:00.000");
    EXPECT_EQ(formatTime(90.0), "00:01:30.000");
    EXPECT_EQ(formatTime(3600.0), "01:00:00.000");
    EXPECT_EQ(formatTime(5445.5), "01:30:45.500");
}

TEST(TimeParserTest, RoundTrip) {
    double original = 5445.5;
    std::string formatted = formatTime(original);
    double parsed = parseTime(formatted);
    EXPECT_DOUBLE_EQ(original, parsed);
}

TEST(TimeParserTest, SanitizeName) {
    EXPECT_EQ(sanitizeName("my video"), "my_video");
    EXPECT_EQ(sanitizeName("test-file.mp4"), "test-filemp4");
    EXPECT_EQ(sanitizeName("Hello World!"), "Hello_World");
}
