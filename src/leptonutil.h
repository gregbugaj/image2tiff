#ifndef IMAGE2TIFF_LEPTONUTIL_H
#define IMAGE2TIFF_LEPTONUTIL_H

#include <vector>
#include <string>
#include <leptonica/allheaders.h>

/**
 * Convert to correct format, this may involve up/down conversion.
 *
 * @param pix the pix to convert
 * @param blank should blank page be generated for failed binarization
 * @param dpi the desired DPI
 * @return converted pix
 */

PIX* binarize(PIX *pix, bool blank, int dpi);


/**
 * Scale image to desired DPI
 *
 * @param pix the pix to scale
 * @param dpi the desired DPI
 * @return
 */
PIX* scale(PIX *pix, int dpi);

/**
 * Process input files
 * @param inputFiles the input files to convert
 * @param output     the output file
 * @param bitonal    convert to bitonal
 * @param dpi        target DPI
 * @param blank      should blank pages be created
 */
void processInputFiles(const std::vector<std::string>& inputFiles, const std::string& output, bool bitonal, int dpi, bool blank);

/**
 * Process input directory
 * @param inputDir   the input directory to convert
 * @param output     the output file
 * @param bitonal    convert to bitonal
 * @param dpi        target DPI
 * @param blank      should blank pages be created
 */
void processInputDirectory(const std::string& inputDir, const std::string& output, bool bitonal, int dpi, bool blank);

#endif //IMAGE2TIFF_LEPTONUTIL_H
