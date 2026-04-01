#ifndef CLIP_H
#define CLIP_H

#include <string>
#include <algorithm>
#include <cctype>

struct Clip {
    double start;
    double end;
    std::string name;
};

// Replace spaces with underscores and remove characters that are invalid in filenames
inline std::string sanitizeName(std::string name) {
    std::replace(name.begin(), name.end(), ' ', '_');
    // Remove other problematic characters
    name.erase(std::remove_if(name.begin(), name.end(), [](char c) {
        return !std::isalnum(static_cast<unsigned char>(c)) && c != '_' && c != '-';
    }), name.end());
    return name;
}

#endif // CLIP_H
