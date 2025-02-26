#include "args.hpp"

#include <iostream>
#include <boost/program_options.hpp>

#include "version.hpp"

namespace po = boost::program_options;

std::optional<cli::Input> cli::parse_args(const int argc, char *argv[]) noexcept {
    Input input{};
    Args &args = input.args;

    po::options_description desc(
            "cheeseburger " PROJECT_VERSION "\n"
            "\n"
            "Cheeseburger is a rewrite, in C++, of the popular program lolcat.\n"
            "It is aimed to provide higher speed, and thus be more usable in\n"
            "init scripts, and such\n"
            "\n"
            "Options");

    try {
        desc.add_options()
                ("help,h", "Show help")
                ("version,v", "Show version")
                ("m_spread,p", po::value<float>(&args.spread)->default_value(3.0),
                 "Rainbow m_spread")
                ("m_freq,F", po::value<float>(&args.freq)->default_value(0.1),
                 "Rainbow frequency")
                ("seed,S", po::value<int>(&args.seed)->default_value(0),
                 "Rainbow seed (0=random)")
                ("m_animate,a", po::bool_switch(&args.animate),
                 "Enable animation")
                ("duration,d", po::value<int>(&args.duration)->default_value(12),
                 "Animation duration")
                ("m_speed,s", po::value<float>(&args.speed)->default_value(20.0f),
                 "Animation m_speed")
                ("m_invert,i", po::bool_switch(&args.invert),
                 "Invert colors")
                ("truecolor,t", po::bool_switch(&args.truecolor),
                 "Force truecolor mode")
                ("force,f", po::bool_switch(&args.force),
                 "Force color output")
                ("files", po::value<std::vector<std::string> >(),
                 "Input files (also positional)");

        po::positional_options_description p;
        p.add("files", -1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv)
                          .options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.contains("help")) {
            std::cout << desc << "\n";
            return std::nullopt;
        }

        if (vm.contains("version")) {
            std::cout << "cheeseburger version " << PROJECT_VERSION << '\n';
            return std::nullopt;
        }
        input.files = vm.contains("files")
                      ? vm["files"].as<std::vector<std::string> >()
                      : std::vector<std::string>{"-"};
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return std::nullopt;
    }

    return {input};
}
