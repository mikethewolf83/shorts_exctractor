#ifndef TIME_PARSER_H
#define TIME_PARSER_H

#include <string>
#include <regex>
#include <iomanip>
#include <sstream>

// Convert time string (HH:MM:SS or seconds) to seconds
inline double parseTime(const std::string& timeStr) {
    std::regex timePattern(R"(^(\d+):(\d{2}):(\d{2}(?:\.\d+)?)$)");
    std::smatch matches;
    if (std::regex_match(timeStr, matches, timePattern)) {
        double hours = std::stod(matches[1]);
        double minutes = std::stod(matches[2]);
        double seconds = std::stod(matches[3]);
        return hours * 3600 + minutes * 60 + seconds;
    }
    return std::stod(timeStr);
}

// Format seconds to HH:MM:SS.mmm for FFmpeg
inline std::string formatTime(double seconds) {
    int hours = static_cast<int>(seconds) / 3600;
    int minutes = (static_cast<int>(seconds) % 3600) / 60;
    double secs = seconds - hours * 3600 - minutes * 60;

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hours << ":"
        << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::fixed << std::setprecision(3) << secs;
    return oss.str();
}

#endif // TIME_PARSER_H
