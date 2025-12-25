/*
 * Copyright 2025 Love Billenius

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 *  1. Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.

 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.

 *  3. Neither the name of the copyright holder nor the names of its contributors
 *   may be used to endorse or promote products derived from this software without
 *   specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cheese.hpp"
#include "terminal.hpp"

#include <cstring>
#include <iostream>
#include <numbers>
#include <random>
#include <string>
#include <string_view>
#include <thread>
#include "format.hpp"

constexpr int rgb_to_256(const uint8_t r, const uint8_t g, const uint8_t b) {
    return 16 + (36 * (r / 51) + 6 * (g / 51) + (b / 51));
}

static bool is_truecolor() {
    const char *colorterm = std::getenv("COLORTERM");
    return colorterm && (std::strstr(colorterm, "truecolor") || std::strstr(colorterm, "24bit"));
}

Cheese::Cheese(const cli::Args &args) :
    m_spread(args.spread), m_speed(args.speed), m_freq(args.freq), m_duration(args.duration),
    m_color_offset(args.seed ? args.seed : static_cast<int>(std::random_device{}() % 256)), m_invert(args.invert),
    m_animate(args.animate), m_truecolor_mode(args.truecolor || is_truecolor()), m_force_term(args.force) {}

void Cheese::process(std::istream &in) {
    if (m_animate && is_tty())
        std::cout << term::HIDE_CURSOR;

    std::string line;
    while (std::getline(in, line)) {
        if (m_animate) {
            animate_line(line);
        } else {
            print_line(line);
            m_line_count++;
        }
    }

    if (is_tty())
        std::cout << term::SHOW_CURSOR << term::RESET;
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
        return fmt_compat::format("\033[{};2;{};{};{}m", bg_fg, r, g, b);
    return fmt_compat::format("\033[{};5;{}m", bg_fg, rgb_to_256(r, g, b));
}

unsigned unicode_length(unsigned char c) {
    if ((c & 0x80) == 0)
        return 1; // ASCII
    else if ((c & 0xE0) == 0xC0)
        return 2; // 2-byte
    else if ((c & 0xF0) == 0xE0)
        return 3; // 3-byte
    else if ((c & 0xF8) == 0xF0)
        return 4; // 4-byte
    return 1;
}

void Cheese::print_line(const std::string_view line) const {
    const size_t len = line.length();
    int char_index = 0;

    for (size_t i = 0; i < len; ++i) {
        if (line[i] == '\033') {
            std::cout << line[i]; // Print ESC

            if (i + 1 < len && line[i + 1] == '[') {
                std::cout << line[++i]; // Print '['

                while (i + 1 < len) {
                    const char next = line[i + 1];
                    if (next >= 0x40 && next <= 0x7E) {
                        std::cout << next;
                        i++;
                        break;
                    }
                    if (next < 0x20 || next > 0x7E) {
                        break;
                    }
                    std::cout << next;
                    i++;
                }
            }
            continue;
        }

        // calculate color
        const float pos = (static_cast<float>(m_color_offset + m_line_count + char_index)) / m_spread;
        std::cout << rainbow(m_freq, pos);

        unsigned char c = static_cast<unsigned char>(line[i]);
        int char_len = unicode_length(c);

        // print the unicode sequence
        for (int byte = 0; byte < char_len && i < len; byte++) {
            std::cout << line[i];
            if (byte < char_len - 1)
                i++;
        }

        std::cout << (m_invert ? term::RESET_BACKGROUND : term::RESET_FOREGROUND);
        char_index++;
    }

    std::cout << '\n';
}
void Cheese::animate_line(const std::string &line) {
    std::cout << term::SAVE_POS;
    const int original_color_offset = m_color_offset;
    for (int i = 0; i < m_duration; i++) {
        std::cout << term::RESTORE_POS;
        m_color_offset += static_cast<int>(m_spread);
        print_line(line);
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000 / m_speed)));
    }
    m_color_offset = original_color_offset;
    m_line_count++;
}

bool Cheese::is_tty() const { return m_force_term || term::is_tty(fileno(stdout)); }
