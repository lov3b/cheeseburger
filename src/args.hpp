#pragma once

#include <string>
#include <optional>
#include <vector>


namespace cli {
    struct Args {
        float spread;
        float freq;
        float speed;
        int seed;
        int duration;
        bool animate;
        bool invert;
        bool truecolor;
        bool force;
    };

    struct Input {
        Args args;
        std::vector<std::string> files;
    };

    std::optional<Input> parse_args(int argc, char *argv[]) noexcept;
};
