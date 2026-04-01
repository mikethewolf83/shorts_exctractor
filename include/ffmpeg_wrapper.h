#ifndef FFMPEG_WRAPPER_H
#define FFMPEG_WRAPPER_H

#include <string>
#include <vector>
#include <filesystem>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace fs = std::filesystem;

// Execute command safely on Windows using CreateProcess
inline bool executeCommand(const std::string& cmd) {
#ifdef _WIN32
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // CreateProcess requires mutable string
    std::vector<char> cmdMutable(cmd.begin(), cmd.end());
    cmdMutable.push_back('\0');

    if (!CreateProcessA(
        nullptr,
        cmdMutable.data(),
        nullptr,
        nullptr,
        FALSE,
        CREATE_NO_WINDOW,
        nullptr,
        nullptr,
        &si,
        &pi
    )) {
        return false;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exitCode = 0;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return exitCode == 0;
#else
    return std::system(cmd.c_str()) == 0;
#endif
}

// Find ffmpeg executable (same dir as app or PATH)
inline std::string findFfmpeg() {
    std::vector<std::string> candidates = {
        (fs::path(fs::current_path()) / "ffmpeg.exe").string(),
        "ffmpeg.exe"
    };

    for (const auto& cand : candidates) {
        if (fs::exists(cand)) {
            return cand;
        }
    }
    return "";
}

#endif // FFMPEG_WRAPPER_H
