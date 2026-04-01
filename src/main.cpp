#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include <algorithm>
#include <sstream>

#include "clip.h"
#include "time_parser.h"
#include "ffmpeg_wrapper.h"

namespace fs = std::filesystem;

void showHelp(const char* progName) {
    std::cout << "YouTube Shorts Extractor\n"
              << "Usage: " << progName << " -i <input> -o <output_dir> --clip <start> <end> [...]\n\n"
              << "Arguments:\n"
              << "  -i, --input FILE      Input video file\n"
              << "  -o, --output DIR      Output directory for shorts\n"
              << "  --clip START END      Clip range (multiple allowed)\n"
              << "                        Time format: seconds or HH:MM:SS\n"
              << "  -h, --help            Show this help\n\n"
              << "Example:\n"
              << "  " << progName << " -i \"my video.mp4\" -o shorts \\\n"
              << "    --clip 00:01:23 00:01:45 --clip 125.5 132.0\n";
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        showHelp(argv[0]);
        return 1;
    }

    std::string input, outputDir;
    std::vector<Clip> clips;
    bool showHelpFlag = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            showHelpFlag = true;
        } else if (arg == "-i" || arg == "--input") {
            if (++i < argc) input = argv[i];
        } else if (arg == "-o" || arg == "--output") {
            if (++i < argc) outputDir = argv[i];
        } else if (arg == "--clip") {
            if (i + 2 < argc) {
                try {
                    double start = parseTime(argv[++i]);
                    double end = parseTime(argv[++i]);
                    std::string clipName = "";

                    // Check if next arg exists and is NOT another flag
                    if (i + 1 < argc && argv[i + 1][0] != '-') {
                        clipName = argv[++i];
                    }

                    if (end > start) {
                        clips.push_back({start, end, clipName});
                    } else {
                        std::cerr << "Error: End time must be greater than start time\n";
                        return 1;
                    }
                } catch (...) {
                    std::cerr << "Invalid time format at position " << i << "\n";
                    return 1;
                }
            } else {
                std::cerr << "Error: --clip requires two time arguments\n";
                return 1;
            }
        }
    }

    if (showHelpFlag) {
        showHelp(argv[0]);
        return 0;
    }

    if (input.empty() || outputDir.empty() || clips.empty()) {
        std::cerr << "Error: Missing required arguments\n";
        showHelp(argv[0]);
        return 1;
    }

    if (!fs::exists(input)) {
        std::cerr << "Error: Input file not found: " << input << "\n";
        return 1;
    }

    fs::create_directories(outputDir);
    std::string ffmpeg = findFfmpeg();
    if (ffmpeg.empty()) {
        std::cerr << "Error: ffmpeg.exe not found in application directory\n";
        return 1;
    }

    std::string inputStem = sanitizeName(fs::path(input).stem().string());

    std::cout << "Processing " << clips.size() << " short(s)...\n";
    int clipNum = 1;
    for (auto& clip : clips) {
        double start = clip.start;
        double end = clip.end;
        double duration = end - start;
        if (duration < 1.5) { // Need at least 1s + 0.5s fade margins
            std::cerr << "Warning: Clip " << clipNum << " too short for 0.5s fades (needs >1.0s), skipping\n";
            continue;
        }

        std::string shortName = clip.name.empty() ? ("short_" + std::to_string(clipNum)) : sanitizeName(clip.name);
        std::string outFileName = inputStem + "_" + shortName + ".mp4";
        std::string outFile = (fs::path(outputDir) / outFileName).string();

        // Build FFmpeg command with proper Windows path escaping
        std::ostringstream cmd;
        cmd << "\"" << ffmpeg << "\" -y -ss " << formatTime(start)
            << " -i \"" << input << "\""
            << " -t " << duration
            << " -vf \"scale=1080:1920:force_original_aspect_ratio=increase,crop=1080:1920\""
            << " -af \"afade=t=in:st=0:d=0.5,afade=t=out:st="
            << (duration - 0.5) << ":d=0.5\""
            << " -c:v libx264 -crf 18 -preset fast -movflags +faststart"
            << " -c:a aac -b:a 192k"
            << " \"" << outFile << "\"";

        std::cout << "\n[" << clipNum << "/" << clips.size() << "] "
                  << formatTime(start) << " -> " << formatTime(end)
                  << " (" << duration << "s)\n";

        if (!executeCommand(cmd.str())) {
            std::cerr << "Error: FFmpeg failed to process clip " << clipNum << "\n";
            std::cerr << "Command: " << cmd.str() << "\n";
            continue;
        }
        std::cout << "Created: " << outFile << "\n";
        clipNum++;
    }

    std::cout << "\nDone! " << (clipNum - 1) << " short(s) created in \"" << outputDir << "\"\n";
    return 0;
}
