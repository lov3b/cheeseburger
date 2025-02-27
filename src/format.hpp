#pragma once

#ifndef __cpp_lib_format

#include <fmt/core.h>

namespace std {
    using fmt::format;
}

#else
#include <format>
#endif