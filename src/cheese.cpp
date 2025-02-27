#include "cheese.hpp"
#include "terminal.hpp"

#include <iostream>
#include <cstring>
#include <string>
#include <thread>
#include <numbers>
#include <regex>
#include "format.hpp"

constexpr int rgb_to_256(const uint8_t r, const uint8_t g, const uint8_t b) {
    return 16 + (36 * (r / 51) + 6 * (g / 51) + (b / 51));
}

static bool is_truecolor() {
    const char *colorterm = std::getenv("COLORTERM");
    return colorterm &&
           (std::strstr(colorterm, "truecolor") || std::strstr(colorterm, "24bit"));
}

Cheese::Cheese(const cli::Args &args) : m_truecolor_mode(args.truecolor || is_truecolor()), m_invert(args.invert),
                                        m_spread(args.spread), m_speed(args.speed), m_duration(args.duration),
                                        m_freq(args.freq), m_animate(args.animate), m_force_term(args.force) {
    is_truecolor();
    m_color_offset = args.seed ? args.seed : static_cast<int>(std::random_device{}() % 256);
}

void Cheese::process(std::istream &in) {
    if (m_animate && is_tty())
        std::cout << term::hide_cursor;

    std::string line;
    while (std::getline(in, line)) {
        if (m_animate) {
            animate_line(line);
        } else {
            print_line(line);
            ++m_line_count;
        }
    }

    if (is_tty())
        std::cout << term::show_cursor << term::reset;
}

std::string Cheese::rainbow(const float freq, const float pos) const {
    constexpr float pi = std::numbers::pi_v<float>;
    const auto r = static_cast<uint8_t>(std::sin(freq * pos) * 127 + 128);
    const auto g = static_cast<uint8_t>(std::sin(freq * pos + 2 * pi / 3) * 127 + 128);
    const auto b = static_cast<uint8_t>(std::sin(freq * pos + 4 * pi / 3) * 127 + 128);
    return format_color(r, g, b);
}

std::string Cheese::format_color(const uint8_t r, const uint8_t g, const uint8_t b) const {
    const auto bg_fg = m_invert ? 48 : 38;
    if (m_truecolor_mode)
        return std::format("\033[{};2;{};{};{}m", bg_fg, r, g, b);
    return std::format("\033[{}];5;{}m", bg_fg, rgb_to_256(r, g, b));
}


void Cheese::print_line(const std::string &line, bool /*animate*/) const {
    static const std::regex pattern(R"((\x1B\[[0-?]*[ -/]*[@-~])|([^\x1B]))");
    std::sregex_iterator it(line.begin(), line.end(), pattern);
    int char_index = 0;
    for (const std::sregex_iterator end; it != end; ++it) {
        const auto &match = *it;
        if (match[1].matched) {
            std::cout << match[1].str();
        } else if (match[2].matched) {
            const float pos = (static_cast<float>(m_color_offset + m_line_count + char_index)) / m_spread;
            std::cout << rainbow(m_freq, pos)
                      << match[2].str()
                      << (m_invert ? term::RESET_BACKGROUND : term::RESET_FOREGROUND);
            ++char_index;
        }
    }
    std::cout << '\n';
}

void Cheese::animate_line(const std::string &line) {
    std::cout << term::save_pos;
    const int original_os = m_color_offset;
    for (int i = 0; i < m_duration; ++i) {
        std::cout << term::restore_pos;
        m_color_offset += static_cast<int>(m_spread);
        print_line(line, true);
        std::this_thread::sleep_for(
                std::chrono::milliseconds(static_cast<int>(1000 / m_speed))
        );
    }
    m_color_offset = original_os;
    ++m_line_count;
}

bool Cheese::is_tty() const { return m_force_term || term::is_tty(fileno(stdout)); }
