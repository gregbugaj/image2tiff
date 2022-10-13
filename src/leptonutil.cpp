#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <chrono>

#include "leptonutil.h"
#include "types.h"

using namespace std::chrono;
namespace  fs = std::experimental::filesystem;

PIX* scale(PIX *pixs, int dpi)
{
    if(pixs == nullptr)
        return nullptr;

    PIX* pix = pixCopy(NULL, pixs);

    if(pix->xres == dpi && pix->yres == dpi)
        return pix;

    pix       = pixScale(pix, dpi / pix->xres, dpi / pix->yres);
    pix->xres = dpi;
    pix->yres = dpi;

    return pix;
}


/**
 * Load PIX data from given path
 * @param path
 * @return loaded PIX data
 */
PIX* load(const string_t& path)
{
    PIX *data;
    // We are checking the image based on the extension
    auto found = path.find_last_of('.');
    string_t ext;

    if(found != std::string::npos){
        ext = path.substr(found, path.length()-1);
    }

    if (ext == ".png")
    {
        FILE *pfile = fopenReadStream(path.c_str());
        if (pfile != nullptr)
        {
            data = pixReadStreamPng(pfile);
            lept_fclose(pfile);
        }
    }
    else if (ext == ".tif" || ext == ".tiff")
    {
        data = pixReadTiff(path.c_str(), 0);
    }
    else
    {
        // everything else
        data = pixRead(path.c_str());
    }

    return data;
}

PIX* binarize(PIX *pix, bool blank, int dpi)
{
    if(pix == nullptr)
        return nullptr;

    if(pix->d == 1)
        return pixClone(pix);

    // Grayscale 8bpp/ Color 16/32 bpp
    PIX* pix8 = pixConvertTo8(pix, 0);

    // possible for the PNG not to have resolution present so we default to passed in DPI
    if(pix8->xres == 0 || pix8->yres == 0)
    {
        pix8->xres = dpi;
        pix8->yres = dpi;
    }

    auto  xres = pix8->xres;
    auto  yres = pix8->yres;

    // 75, 75, 150, 150,
    // 16, 16, 64, 64, 0.0
    PIX *binary = nullptr;

    if(pixSauvolaBinarizeTiled(pix8, 32, 0.12, 4, 4, NULL, &binary) != 0)
    {
        binary = pixThresholdToBinary(pix8, 128);
        // if we failed do we need to create a blank
        if(blank && binary == nullptr)
            binary = pixCreate(pix->w, pix->h, 1);
    }

    pixSetResolution(binary, xres, yres);
    pixDestroy(&pix8);

    return binary;
}


/**
 * Convert raw PIX
 * 1) Binarize
 * 2) Scale
 * @param pix
 * @param bitonal
 * @param dpi
 * @param blank
 * @return
 */
PIX* convert(PIX *pixs, bool bitonal, int dpi, bool blank)
{
    if(pixs == nullptr)
        return nullptr;

    PIX* pix = pixClone(pixs);
    PIX* bin = pixClone(pixs);

    if(bitonal)
        bin = binarize(pix, blank, dpi);
    else
        bin = pixClone(pixs);

    PIX* scaled = scale(bin, dpi);

    pixDestroy(&pix);
    pixDestroy(&bin);

    return scaled;
}

void validateFileExists(const std::string &filename)
{
    std::ifstream f(filename);

    if (!f.good())
    {
        std::stringstream ss;
        ss << "File not found : "<< filename;

        throw std::runtime_error(ss.str());
    }
}

void processInputFiles(const std::vector<std::string>& inputFiles, const std::string& output, bool bitonal, int dpi, bool blank)
{
    std::cout << "Files" << std::endl;
    const char* poutput = output.c_str();
    auto first = true;

    for (const auto& f : inputFiles)
    {
        std::cout << f << std::endl;
        validateFileExists(f);
        PIX* pix = load(f);

        if(pix == nullptr)
            continue;

        PIX* converted = convert(pix, bitonal, dpi, blank);

        if(converted)
        {
            // write out converted doc
            auto mode = first ? "w" : "a";
            pixWriteTiff(poutput, converted, IFF_TIFF_G4, mode);
            first = false;
            pixDestroy(&converted);
        }

        pixDestroy(&pix);
    }
}


void processInputDirectory(const std::string& inputDir, const std::string& output, bool bitonal, int dpi, bool blank)
{
    // Natural ordering will be used for file generation
    auto path = fs::path(inputDir);
    std::vector <std::string> set;

    for(auto& file : fs::directory_iterator(path))
    {
        set.push_back(file.path());
    }

    processInputFiles(set, output, bitonal, dpi, blank);
}