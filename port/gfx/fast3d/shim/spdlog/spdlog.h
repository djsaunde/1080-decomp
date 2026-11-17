#pragma once
/* shim (port/gfx/fast3d): spdlog's logging macros, printed to stderr with "{}"
 * placeholders filled in order. SPDLOG_DEBUG / TRACE are dropped. */
#include <cstdio>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>

namespace lus_shim {
inline void fmt_rest(std::ostringstream& o, std::string_view f) { o << f; }
/* "{}" and "{:spec}" placeholders; a spec ending in x / X prints hex */
template <class T, class... R> void fmt_rest(std::ostringstream& o, std::string_view f, const T& a, const R&... r) {
    const size_t p = f.find('{');
    const size_t q = p == std::string_view::npos ? p : f.find('}', p);
    if (q == std::string_view::npos) {
        o << f;
        return;
    }
    o << f.substr(0, p);
    const std::string_view spec = f.substr(p + 1, q - p - 1);
    const bool hex = !spec.empty() && (spec.back() == 'x' || spec.back() == 'X');
    if constexpr (std::is_integral_v<T>) {
        if (hex) {
            o << std::hex << std::uppercase << (unsigned long long)a << std::dec;
        } else if constexpr (sizeof(T) == 1) {
            o << (int)a;
        } else {
            o << a;
        }
    } else {
        o << a;
    }
    fmt_rest(o, f.substr(q + 1), r...);
}
template <class... A> void log(const char* level, std::string_view f, const A&... a) {
    std::ostringstream o;
    fmt_rest(o, f, a...);
    std::fprintf(stderr, "[fast3d %s] %s\n", level, o.str().c_str());
}
} // namespace lus_shim

#define SPDLOG_CRITICAL(...) ::lus_shim::log("critical", __VA_ARGS__)
#define SPDLOG_ERROR(...) ::lus_shim::log("error", __VA_ARGS__)
#define SPDLOG_WARN(...) ::lus_shim::log("warn", __VA_ARGS__)
#define SPDLOG_INFO(...) ::lus_shim::log("info", __VA_ARGS__)
#define SPDLOG_DEBUG(...) ((void)0)
#define SPDLOG_TRACE(...) ((void)0)
