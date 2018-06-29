#include <iostream>

#include <set>
#include "cxxopts.hpp"
#include "types.h"
#include "leptonutil.h"


/**
 * Application entry point
 * @param argc
 * @param args
 * @return
 */
int main(int argc, char* argv[])
{
    bool debug = false;
    cxxopts::Options options("image2tiff", " - Image to Tiff conversion program");

    options.add_options()
            ("debug", "Enable debugging", cxxopts::value<bool>(debug))
            ("input-files", "Files to combine", cxxopts::value<std::vector<std::string>>(), "FILE")
            ("input-dir", "Input directory of images to combine",   cxxopts::value<std::string>(), "DIR")
            ("output", "Output file", cxxopts::value<std::string>()->default_value("generated_ts.tiff"), "FILE")
            ("help", "Print help")
            ;

    options.add_options("Output")
            ("bitonal", "Convert to bitonal", cxxopts::value<bool>()->default_value("true"))
            ("dpi", "Output resolution (600, 300, 150, 96, 72)",  cxxopts::value<int>()->default_value("300"), "N")
            ("blank-page", "Create blank-pages when conversion fails for input image", cxxopts::value<bool>()->default_value("true"))
            ;

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help({"", "Output"}) << std::endl;
        exit(1);
    }

    // check for input
    if (result.count("input-files") == 0  && result.count("input-dir") == 0)
    {
        std::cout << "input-files or input-dir is required" << std::endl;
        std::cout << options.help({"", "Output"}) << std::endl;
        exit(1);
    }

    auto output = result["output"].as<std::string>();
    auto bitonal = result["bitonal"].as<bool>();
    auto blank = result["blank-page"].as<bool>();
    auto dpi = result["dpi"].as<int>();

    if (result.count("input-files"))
    {
        auto& ff = result["input-files"].as<std::vector<std::string>>();
        processInputFiles(ff, output, bitonal, dpi, blank);
    }

    if(result.count("input-dir"))
    {
        auto& dir = result["input-dir"].as<std::string>();
        processInputDirectory(dir, output, bitonal, dpi, blank);
    }

    return 0;
}
