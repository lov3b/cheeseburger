#pragma once

#ifdef USE_LIBFMT

#include <fmt/core.h>

namespace std {
using fmt::format;
}

#else
#include <format>
#endif

