#include <iomanip>
#include <iostream>

#include <core/phyr.h>

#include <core/color/spectrum.h>
#include <core/material/texture.h>

#include <modules/textures/consttex.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

int main(int argc, const char* argv[]) {
    using namespace phyr;

    // Initiate Spectrum data
    Spectrum::init();

    // Create a red Spectrum
    Real redCol[3] = { 1, 0, 0 };
    Spectrum red = Spectrum::getFromRGB(redCol, SpectrumType::Reflectance);

    Real greenCol[3] = { 0, 1, 0 };
    Spectrum green = Spectrum::getFromRGB(greenCol, SpectrumType::Reflectance);

    Real resCol[3];
    Spectrum res = red * green;
    res.toRGBConstants(resCol);

    std::cout << "R: " << resCol[0] << ", G: " << resCol[1] << ", B: " << resCol[2] <<  std::endl;
    return 0;
}

#pragma GCC diagnostic pop
