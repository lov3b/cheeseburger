#pragma once

#include <string>

namespace term {

constexpr std::string_view reset = "\033[m";
constexpr std::string_view hide_cursor = "\033[?25l";
constexpr std::string_view show_cursor = "\033[?25h";
constexpr std::string_view save_pos = "\0337";
constexpr std::string_view restore_pos = "\0338";
constexpr std::string_view RESET_BACKGROUND = "\033[49m";
constexpr std::string_view RESET_FOREGROUND = "\033[39m";

bool is_tty(int fd);

} // namespace term
