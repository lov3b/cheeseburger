#include "terminal.hpp"

#ifdef _WIN32

#include <io.h>

#else

#include <unistd.h>

#endif

namespace term {
    bool is_tty(const int fd) {
#ifdef _WIN32
        return _isatty(fd) != 0;
#else
        return isatty(fd) != 0;
#endif
    }
}
