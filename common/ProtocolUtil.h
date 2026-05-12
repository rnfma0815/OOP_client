#pragma once

#include <string>
#include <vector>

namespace ProtocolUtil {
    std::string encode(const std::string& text);
    std::string decode(const std::string& text);
    std::vector<std::string> split(const std::string& text, char delimiter);
}
