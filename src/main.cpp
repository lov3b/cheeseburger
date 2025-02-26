#include "args.hpp"
#include "cheese.hpp"

#include <iostream>
#include <fstream>

int main(const int argc, char *argv[]) {
    const auto opt_args = cli::parse_args(argc, argv);
    if (!opt_args) return EXIT_SUCCESS;

    Cheese cheese(opt_args->args);
    try {
        for (const auto &file: opt_args->files) {
            if (file == "-") {
                cheese.process(std::cin);
            } else {
                std::ifstream fin(file);
                if (!fin) throw std::runtime_error("Can't open file: " + file);
                cheese.process(fin);
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
