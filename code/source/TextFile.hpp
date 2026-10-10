// textfile.h
// 单文件实现：读取文件 -> 自动判断编码 -> UTF-8 -> 按行访问
#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <climits>
#include <utility>

#if defined(_WIN32)
#  include <windows.h>
#else
#  include <iconv.h>
#  include <langinfo.h>
#  include <cerrno>
#  include <cstring>
#endif

namespace enc {

// ==================== 编码枚举 ====================
enum class TextEncoding {
    Unknown = 0,
    UTF8,        // 无 BOM 的 UTF-8
    UTF8_BOM,    // 带 BOM 的 UTF-8
    UTF16_LE,
    UTF16_BE,
    UTF32_LE,
    UTF32_BE,
    ANSI         // 本地编码（中文 Windows 上通常是 GBK/936）
};

inline const char* encodingName(TextEncoding e) noexcept {
    switch (e) {
        case TextEncoding::UTF8:     return "UTF-8";
        case TextEncoding::UTF8_BOM: return "UTF-8 (BOM)";
        case TextEncoding::UTF16_LE: return "UTF-16LE";
        case TextEncoding::UTF16_BE: return "UTF-16BE";
        case TextEncoding::UTF32_LE: return "UTF-32LE";
        case TextEncoding::UTF32_BE: return "UTF-32BE";
        case TextEncoding::ANSI:     return "ANSI/本地编码";
        default:                     return "Unknown";
    }
}

// ==================== 内部实现细节 ====================
namespace detail {

inline void appendUTF8(std::string& out, uint32_t cp) {
    if (cp <= 0x7F) {
        out.push_back(static_cast<char>(cp));
    } else if (cp <= 0x7FF) {
        out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else if (cp <= 0xFFFF) {
        out.push_back(static_cast<char>(0xE0 | (cp >> 12)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else {
        out.push_back(static_cast<char>(0xF0 | (cp >> 18)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    }
}

inline bool isValidUTF8(const uint8_t* p, size_t n) {
    size_t i = 0;
    while (i < n) {
        uint8_t c = p[i];
        if (c < 0x80) { ++i; continue; }

        int      len;
        uint32_t cp;
        if ((c & 0xE0) == 0xC0)      { len = 2; cp = c & 0x1F; }
        else if ((c & 0xF0) == 0xE0) { len = 3; cp = c & 0x0F; }
        else if ((c & 0xF8) == 0xF0) { len = 4; cp = c & 0x07; }
        else return false;

        if (i + static_cast<size_t>(len) > n) return false;

        for (int k = 1; k < len; ++k) {
            uint8_t cc = p[i + k];
            if ((cc & 0xC0) != 0x80) return false;
            cp = (cp << 6) | (cc & 0x3F);
        }
        if (len == 2 && cp < 0x80)              return false;
        if (len == 3 && cp < 0x800)             return false;
        if (len == 4 && cp < 0x10000)           return false;
        if (cp > 0x10FFFF)                      return false;
        if (cp >= 0xD800 && cp <= 0xDFFF)       return false;

        i += len;
    }
    return true;
}

inline std::string utf16ToUTF8(const uint8_t* p, size_t nbytes, bool bigEndian) {
    std::string out;
    out.reserve(nbytes);
    size_t i = 0;

    auto readUnit = [&](uint16_t& u) -> bool {
        if (i + 2 > nbytes) return false;
        if (bigEndian) u = static_cast<uint16_t>((p[i] << 8) | p[i + 1]);
        else           u = static_cast<uint16_t>(p[i] | (p[i + 1] << 8));
        i += 2;
        return true;
    };

    uint16_t u = 0;
    while (readUnit(u)) {
        uint32_t cp;
        if (u >= 0xD800 && u <= 0xDBFF) {
            uint16_t lo = 0;
            if (readUnit(lo) && lo >= 0xDC00 && lo <= 0xDFFF) {
                cp = 0x10000u + ((static_cast<uint32_t>(u - 0xD800) << 10) |
                                 static_cast<uint32_t>(lo - 0xDC00));
            } else {
                cp = 0xFFFD;
            }
        } else if (u >= 0xDC00 && u <= 0xDFFF) {
            cp = 0xFFFD;
        } else {
            cp = u;
        }
        appendUTF8(out, cp);
    }
    return out;
}

inline std::string utf32ToUTF8(const uint8_t* p, size_t nbytes, bool bigEndian) {
    std::string out;
    out.reserve(nbytes);
    size_t n = nbytes / 4;
    for (size_t i = 0; i < n; ++i) {
        const uint8_t* q = p + i * 4;
        uint32_t cp;
        if (bigEndian)
            cp = (uint32_t(q[0]) << 24) | (uint32_t(q[1]) << 16) |
                 (uint32_t(q[2]) << 8)  |  uint32_t(q[3]);
        else
            cp = (uint32_t(q[3]) << 24) | (uint32_t(q[2]) << 16) |
                 (uint32_t(q[1]) << 8)  |  uint32_t(q[0]);
        if (cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF)) cp = 0xFFFD;
        appendUTF8(out, cp);
    }
    return out;
}

inline std::string latin1ToUTF8(const uint8_t* p, size_t n) {
    std::string out;
    out.reserve(n * 2);
    for (size_t i = 0; i < n; ++i) appendUTF8(out, p[i]);
    return out;
}

inline int guessUTF16WithoutBOM(const uint8_t* p, size_t n) {
    if (n < 4 || (n % 2) != 0) return 0;

    size_t sample = std::min<size_t>(n, 8192);
    size_t evenZero = 0, oddZero = 0;
    for (size_t i = 0; i < sample; ++i) {
        if (p[i] == 0x00) {
            if (i % 2 == 0) ++evenZero;
            else            ++oddZero;
        }
    }
    size_t pairs = sample / 2;
    if (pairs == 0) return 0;
    if (oddZero  > pairs * 9 / 10 && evenZero < pairs / 10) return 1; // LE
    if (evenZero > pairs * 9 / 10 && oddZero  < pairs / 10) return 2; // BE
    return 0;
}

#if defined(_WIN32)
inline std::string localToUTF8(const uint8_t* p, size_t n) {
    if (n == 0) return {};
    if (n > static_cast<size_t>(INT_MAX)) n = static_cast<size_t>(INT_MAX);

    int wlen = ::MultiByteToWideChar(CP_ACP, 0,
                                     reinterpret_cast<LPCCH>(p), static_cast<int>(n),
                                     nullptr, 0);
    if (wlen > 0) {
        std::wstring w(static_cast<size_t>(wlen), L'\0');
        ::MultiByteToWideChar(CP_ACP, 0,
                              reinterpret_cast<LPCCH>(p), static_cast<int>(n),
                              &w[0], wlen);
        int u8len = ::WideCharToMultiByte(CP_UTF8, 0, w.data(), wlen,
                                          nullptr, 0, nullptr, nullptr);
        if (u8len > 0) {
            std::string out(static_cast<size_t>(u8len), '\0');
            ::WideCharToMultiByte(CP_UTF8, 0, w.data(), wlen,
                                  &out[0], u8len, nullptr, nullptr);
            return out;
        }
    }
    return latin1ToUTF8(p, n);
}
#else
inline std::string iconvToUTF8(const char* fromcode, const uint8_t* data, size_t n) {
    if (n == 0) return {};
    iconv_t cd = ::iconv_open("UTF-8", fromcode);
    if (cd == reinterpret_cast<iconv_t>(-1)) return {};

    std::string out;
    out.reserve(n * 2 + 16);

    char*  inPtr   = const_cast<char*>(reinterpret_cast<const char*>(data));
    size_t inLeft  = n;
    char   buf[8192];

    while (inLeft > 0) {
        char*  outPtr  = buf;
        size_t outLeft = sizeof(buf);
        size_t rc = ::iconv(cd, &inPtr, &inLeft, &outPtr, &outLeft);
        out.append(buf, sizeof(buf) - outLeft);
        if (rc == static_cast<size_t>(-1)) {
            if (errno == E2BIG) continue;
            if (errno == EILSEQ || errno == EINVAL) {
                out += "\xEF\xBF\xBD";
                ++inPtr; --inLeft;
                ::iconv(cd, nullptr, nullptr, nullptr, nullptr);
            } else {
                break;
            }
        }
    }
    ::iconv_close(cd);
    return out;
}

inline std::string localToUTF8(const uint8_t* p, size_t n) {
    const char* cs = ::nl_langinfo(CODESET);
    if (cs && *cs) {
        std::string s = iconvToUTF8(cs, p, n);
        if (!s.empty()) return s;
    }
    const char* fallbacks[] = { "GB18030", "GBK", "BIG5", "ISO-8859-1" };
    for (const char* f : fallbacks) {
        std::string s = iconvToUTF8(f, p, n);
        if (!s.empty()) return s;
    }
    return latin1ToUTF8(p, n);
}
#endif

// 核心：字节流 -> UTF-8
inline std::pair<std::string, TextEncoding>
bytesToUTF8(const uint8_t* p, size_t n) {
    std::pair<std::string, TextEncoding> r;
    r.second = TextEncoding::Unknown;
    if (n == 0) { r.second = TextEncoding::UTF8; return r; }

    // 1. BOM 检测（先长后短）
    if (n >= 3 && p[0] == 0xEF && p[1] == 0xBB && p[2] == 0xBF) {
        r.first.assign(reinterpret_cast<const char*>(p) + 3, n - 3);
        r.second = TextEncoding::UTF8_BOM;
        return r;
    }
    if (n >= 4 && p[0] == 0xFF && p[1] == 0xFE && p[2] == 0x00 && p[3] == 0x00) {
        r.first = utf32ToUTF8(p + 4, n - 4, false);
        r.second = TextEncoding::UTF32_LE;
        return r;
    }
    if (n >= 4 && p[0] == 0x00 && p[1] == 0x00 && p[2] == 0xFE && p[3] == 0xFF) {
        r.first = utf32ToUTF8(p + 4, n - 4, true);
        r.second = TextEncoding::UTF32_BE;
        return r;
    }
    if (n >= 2 && p[0] == 0xFF && p[1] == 0xFE) {
        r.first = utf16ToUTF8(p + 2, n - 2, false);
        r.second = TextEncoding::UTF16_LE;
        return r;
    }
    if (n >= 2 && p[0] == 0xFE && p[1] == 0xFF) {
        r.first = utf16ToUTF8(p + 2, n - 2, true);
        r.second = TextEncoding::UTF16_BE;
        return r;
    }

    // 2. 无 BOM 的 UTF-16 启发式
    int g = guessUTF16WithoutBOM(p, n);
    if (g == 1) {
        r.first = utf16ToUTF8(p, n, false);
        r.second = TextEncoding::UTF16_LE;
        return r;
    }
    if (g == 2) {
        r.first = utf16ToUTF8(p, n, true);
        r.second = TextEncoding::UTF16_BE;
        return r;
    }

    // 3. 合法 UTF-8
    if (isValidUTF8(p, n)) {
        r.first.assign(reinterpret_cast<const char*>(p), n);
        r.second = TextEncoding::UTF8;
        return r;
    }

    // 4. 本地编码
    r.first = localToUTF8(p, n);
    r.second = TextEncoding::ANSI;
    return r;
}

inline std::vector<uint8_t> readFileBytes(const std::string& path) {
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) throw std::runtime_error("无法打开文件: " + path);

    ifs.seekg(0, std::ios::end);
    std::streamoff size = ifs.tellg();
    if (size < 0) throw std::runtime_error("无法获取文件大小: " + path);
    ifs.seekg(0, std::ios::beg);

    std::vector<uint8_t> buf(static_cast<size_t>(size));
    if (size > 0) {
        ifs.read(reinterpret_cast<char*>(buf.data()), size);
        if (!ifs) throw std::runtime_error("读取文件失败: " + path);
    }
    return buf;
}

// 按 \n、\r\n、\r 切分成行；末尾空行不保留
inline std::vector<std::string> splitLines(const std::string& s) {
    std::vector<std::string> out;
    size_t i = 0, start = 0;
    while (i < s.size()) {
        char c = s[i];
        if (c == '\n') {
            out.emplace_back(s, start, i - start);
            ++i;
            start = i;
        } else if (c == '\r') {
            out.emplace_back(s, start, i - start);
            ++i;
            if (i < s.size() && s[i] == '\n') ++i;
            start = i;
        } else {
            ++i;
        }
    }
    if (start < s.size()) out.emplace_back(s, start, s.size() - start);
    return out;
}

} // namespace detail

// ==================== TextFile 类 ====================
/**
 * 用法示例：
 *   enc::TextFile f("a.txt");
 *   std::cout << f.encodingName() << "\n";     // 检测到的原始编码
 *   std::cout << f.text() << "\n";             // 完整 UTF-8 文本
 *   for (const auto& line : f.lines()) {       // 按行遍历
 *       std::cout << line << "\n";
 *   }
 */
class TextFile {
public:
    TextFile() = default;

    // 构造时即加载；失败抛 std::runtime_error
    explicit TextFile(const std::string& path) { load(path); }

    // ---- 加载接口 ----

    // 从文件加载；失败抛 std::runtime_error
    void load(const std::string& path) {
        std::vector<uint8_t> bytes = detail::readFileBytes(path);
        assignFromBytes(bytes.data(), bytes.size());
    }

    // 从文件加载；失败返回 false（不抛异常）
    bool tryLoad(const std::string& path) noexcept {
        try {
            load(path);
            return true;
        } catch (...) {
            reset();
            return false;
        }
    }

    // 从内存字节加载
    void loadFromMemory(const void* data, size_t size) {
        assignFromBytes(static_cast<const uint8_t*>(data), size);
    }

    void clear() noexcept { reset(); }

    // ---- 状态查询 ----
    bool          isLoaded()     const noexcept { return loaded_; }
    TextEncoding  encoding()     const noexcept { return encoding_; }
    const char*   encodingName() const noexcept { return enc::encodingName(encoding_); }

    // ---- 内容访问 ----

    // 完整 UTF-8 文本
    const std::string& text() const noexcept { return text_; }
    const std::string& utf8() const noexcept { return text_; }

    // 按行返回（惰性切分并缓存）
    const std::vector<std::string>& lines() const {
        if (!linesParsed_) {
            lines_ = detail::splitLines(text_);
            linesParsed_ = true;
        }
        return lines_;
    }

    // 行数
    size_t lineCount() const { return lines().size(); }

    // 取某一行（越界返回空字符串）
    const std::string& line(size_t index) const {
        const auto& v = lines();
        static const std::string empty;
        return index < v.size() ? v[index] : empty;
    }

    // 逐行回调，避免一次性构造 vector（适用于超大文件）
    template <typename Fn>
    void forEachLine(Fn&& fn) const {
        const std::string& s = text_;
        size_t i = 0, start = 0;
        while (i < s.size()) {
            char c = s[i];
            if (c == '\n' || c == '\r') {
                fn(std::string(s, start, i - start));
                if (c == '\r' && i + 1 < s.size() && s[i + 1] == '\n') ++i;
                ++i;
                start = i;
            } else {
                ++i;
            }
        }
        if (start < s.size()) fn(std::string(s, start, s.size() - start));
    }

private:
    void assignFromBytes(const uint8_t* p, size_t n) {
        auto r = detail::bytesToUTF8(p, n);
        text_       = std::move(r.first);
        encoding_   = r.second;
        loaded_     = true;
        lines_.clear();
        linesParsed_ = false;
    }

    void reset() noexcept {
        text_.clear();
        lines_.clear();
        encoding_    = TextEncoding::Unknown;
        loaded_      = false;
        linesParsed_ = false;
    }

    std::string   text_;
    TextEncoding  encoding_    = TextEncoding::Unknown;
    bool          loaded_      = false;

    mutable std::vector<std::string> lines_;
    mutable bool                     linesParsed_ = false;
};

} // namespace enc