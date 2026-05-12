#include "ProtocolUtil.h"

#include <cctype>
#include <sstream>

namespace {
char toHex(int value) {
    return value < 10 ? static_cast<char>('0' + value) : static_cast<char>('A' + value - 10);
}

int fromHex(char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    return -1;
}

bool shouldEncode(unsigned char ch) {
    return ch == '%' || ch == '|' || ch == ',' || ch == ';' || ch == '\n' || ch == '\r';
}
}

namespace ProtocolUtil {
std::string encode(const std::string& text) {
    std::string result;

    for (unsigned char ch : text) {
        if (shouldEncode(ch)) {
            result.push_back('%');
            result.push_back(toHex(ch >> 4));
            result.push_back(toHex(ch & 0x0F));
        }
        else {
            result.push_back(static_cast<char>(ch));
        }
    }

    return result;
}

std::string decode(const std::string& text) {
    std::string result;

    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == '%' && i + 2 < text.size()) {
            int high = fromHex(text[i + 1]);
            int low = fromHex(text[i + 2]);
            if (high >= 0 && low >= 0) {
                result.push_back(static_cast<char>((high << 4) | low));
                i += 2;
                continue;
            }
        }

        result.push_back(text[i]);
    }

    return result;
}

std::vector<std::string> split(const std::string& text, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(text);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}
}
