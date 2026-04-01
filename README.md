# YouTube Shorts Extractor

![YouTube Shorts Extractor](https://mikethewolf83.github.io/assets/img/shorts_extractor.jpg)

A command-line tool to extract short-form vertical videos (Shorts/Reels/TikToks) from longer video files. Automatically crops to 9:16 aspect ratio and applies fade-in/out effects.

## Features

- Extract multiple clips from a single video
- Automatic vertical crop (1080x1920)
- Audio fade-in/out (0.5s)
- Support for HH:MM:SS or seconds format
- Optimized H.264 encoding for social media

## Requirements

- **Compiler:** C++17 compatible (MSVC 2019+, GCC 7+, Clang 5+)
- **CMake:** 3.15 or higher
- **FFmpeg:** Must be in project directory or system PATH

## Building

### Windows

```bash
# Using Visual Studio 2022
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release

# Or use the build script
.\scripts\build.bat
```

### Linux/macOS

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Or use the build script
./scripts/build.sh
```

## Installation

1. Copy `ffmpeg.exe` (Windows) or `ffmpeg` (Linux/macOS) to the build directory
2. Or ensure FFmpeg is in your system PATH

## Usage

```bash
shorts_extractor -i <input> -o <output_dir> --clip <start> <end> [--clip <start> <end> ...]
```

### Arguments

| Argument | Description |
|----------|-------------|
| `-i, --input FILE` | Input video file |
| `-o, --output DIR` | Output directory for extracted shorts |
| `--clip START END` | Clip range (multiple allowed) |
| `-h, --help` | Show help message |

### Time Format

- **Seconds:** `125.5` (125.5 seconds)
- **HH:MM:SS:** `00:01:30` (1 minute 30 seconds)
- **With decimals:** `01:30:45.5` (1 hour, 30 minutes, 45.5 seconds)

### Examples

**Windows:**
```bash
.\shorts_extractor.exe -i "my_video.mp4" -o shorts --clip 00:01:00 00:01:30
```

**Linux/macOS:**
```bash
./shorts_extractor -i video.mp4 -o output --clip 60 90 --clip 120 150
```

**Multiple clips:**
```bash
shorts_extractor -i podcast.mp4 -o clips \
  --clip 00:05:00 00:05:45 "highlight_1" \
  --clip 00:12:30 00:13:15 "highlight_2"
```

## Output

The tool generates MP4 files with:
- Video: H.264, CRF 18, fast preset
- Audio: AAC, 192kbps
- Resolution: 1080x1920 (vertical)
- Fade effects: 0.5s fade-in/out

Output filename format: `<input_name>_<clip_name>.mp4`

## Running Tests

```bash
cd build
ctest --output-on-failure
```

## Project Structure

```
shorts_extractor/
├── src/           # Source code
├── include/       # Header files
├── tests/         # Unit tests
├── scripts/       # Build scripts
├── CMakeLists.txt # Build configuration
└── README.md      # This file
```

## License

MIT License - see [LICENSE](LICENSE) for details.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Run tests
5. Submit a pull request
