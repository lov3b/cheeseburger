#pragma once
#include <string_view>

namespace term {

// clang-format off
constexpr std::string_view RESET = "\x1b" "[m";
constexpr std::string_view HIDE_CURSOR = "\x1b" "[?25l";
constexpr std::string_view SHOW_CURSOR = "\x1b" "[?25h";
constexpr std::string_view SAVE_POS = "\x1b" "7";
constexpr std::string_view RESTORE_POS = "\x1b" "8";
constexpr std::string_view RESET_BACKGROUND = "\x1b" "[49m";
constexpr std::string_view RESET_FOREGROUND = "\x1b" "[39m";
// clang-format on

bool is_tty(int fd);

} // namespace term
