#pragma once

#ifdef USE_LIBFMT
#include <fmt/core.h>
namespace fmt_compat = fmt;
#else
#include <format>
namespace fmt_compat = std;
#endif
