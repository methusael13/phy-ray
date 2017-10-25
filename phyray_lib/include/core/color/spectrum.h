#ifndef PHYRAY_CORE_SPECTRUM_H
#define PHYRAY_CORE_SPECTRUM_H

#include <core/phyr.h>

namespace phyr {

// Extern definitions for SampledSpectrum
extern bool isSampleSpaceSorted(const Real* lambda, int n);
extern void sortSampleSpace(Real* lambda, Real* v, int n);

/**
 * Calculate average of the sample space over the range {lambda0}
 * to {lambda1}. Sample space data is give by {lambda} and {v} and size {n}
 */
extern Real averageSampleRange(const Real* lambda, const Real* v, int n,
                               Real lambda0, Real lambda1);

// Conversion routines
inline void convertXYZToRGB(const Real xyz[3], Real rgb[3]) {
    rgb[0] =  3.240479f * xyz[0] - 1.537150f * xyz[1] - 0.498535f * xyz[2];
    rgb[1] = -0.969256f * xyz[0] + 1.875991f * xyz[1] + 0.041556f * xyz[2];
    rgb[2] =  0.055648f * xyz[0] - 0.204043f * xyz[1] + 1.057311f * xyz[2];
}

inline void convertRGBToXYZ(const Real rgb[3], Real xyz[3]) {
    xyz[0] = 0.412453f * rgb[0] + 0.357580f * rgb[1] + 0.180423f * rgb[2];
    xyz[1] = 0.212671f * rgb[0] + 0.715160f * rgb[1] + 0.072169f * rgb[2];
    xyz[2] = 0.019334f * rgb[0] + 0.119193f * rgb[1] + 0.950227f * rgb[2];
}


// Spectral data declarations by CIE
static const int nCIESamples = 471;  // 360 to 830 nm
extern const Real CIE_X[nCIESamples];
extern const Real CIE_Y[nCIESamples];
extern const Real CIE_Z[nCIESamples];
extern const Real CIE_LAMBDA[nCIESamples];

// Spectral data for RGB to Sampled conversion
static const int nRGB2SpectSamples = 32;
extern const Real RGB2SpectLambda[nRGB2SpectSamples];
extern const Real RGBRefl2SpectWhite[nRGB2SpectSamples];
extern const Real RGBRefl2SpectCyan[nRGB2SpectSamples];
extern const Real RGBRefl2SpectMagenta[nRGB2SpectSamples];
extern const Real RGBRefl2SpectYellow[nRGB2SpectSamples];
extern const Real RGBRefl2SpectRed[nRGB2SpectSamples];
extern const Real RGBRefl2SpectGreen[nRGB2SpectSamples];
extern const Real RGBRefl2SpectBlue[nRGB2SpectSamples];
extern const Real RGBIllum2SpectWhite[nRGB2SpectSamples];
extern const Real RGBIllum2SpectCyan[nRGB2SpectSamples];
extern const Real RGBIllum2SpectMagenta[nRGB2SpectSamples];
extern const Real RGBIllum2SpectYellow[nRGB2SpectSamples];
extern const Real RGBIllum2SpectRed[nRGB2SpectSamples];
extern const Real RGBIllum2SpectGreen[nRGB2SpectSamples];
extern const Real RGBIllum2SpectBlue[nRGB2SpectSamples];

static const Real CIE_Y_LAMBDA = 106.856895;

enum class SpectrumType { Reflectance, Illuminant };

// CoefficientSpectrum declarations
template <int sampleSize>
class CoefficientSpectrum {
  public:
    CoefficientSpectrum(Real v = 0) {
        for (int i = 0; i < sampleSize; i++)
            samples[i] = v;
    }

    bool hasNaNs() const {
        for (int i = 0; i < sampleSize; i++)
            if (isNaN(samples[i])) return true;
        return false;
    }

    // Arithmetic operators
    CoefficientSpectrum operator+(const CoefficientSpectrum& c1) const {
        ASSERT(!c1.hasNaNs()); CoefficientSpectrum cs;
        for (int i = 0; i < sampleSize; i++) cs.samples[i] = samples[i] + c1.samples[i];
        return cs;
    }
    CoefficientSpectrum& operator+=(const CoefficientSpectrum& c1) {
        ASSERT(!c1.hasNaNs());
        for (int i = 0; i < sampleSize; i++) samples[i] += c1.samples[i];
        return *this;
    }

    CoefficientSpectrum operator-(const CoefficientSpectrum& c1) const {
        ASSERT(!c1.hasNaNs()); CoefficientSpectrum cs;
        for (int i = 0; i < sampleSize; i++) cs.samples[i] = samples[i] - c1.samples[i];
        return cs;
    }
    CoefficientSpectrum& operator-=(const CoefficientSpectrum& c1) {
        ASSERT(!c1.hasNaNs());
        for (int i = 0; i < sampleSize; i++) samples[i] -= c1.samples[i];
        return *this;
    }

    CoefficientSpectrum operator*(const CoefficientSpectrum& c1) const {
        ASSERT(!c1.hasNaNs()); CoefficientSpectrum cs;
        for (int i = 0; i < sampleSize; i++) cs.samples[i] = samples[i] * c1.samples[i];
        return cs;
    }
    CoefficientSpectrum& operator*=(const CoefficientSpectrum& c1) {
        ASSERT(!c1.hasNaNs());
        for (int i = 0; i < sampleSize; i++) samples[i] *= c1.samples[i];
        return *this;
    }
    CoefficientSpectrum operator*(Real v) const {
        ASSERT(!isNaN(v)); CoefficientSpectrum cs;
        for (int i = 0; i < sampleSize; i++) cs.samples[i] = samples[i] * v;
        return cs;
    }
    CoefficientSpectrum& operator*=(Real v) {
        ASSERT(!isNaN(v));
        for (int i = 0; i < sampleSize; i++) samples[i] *= v;
        return *this;
    }

    friend inline CoefficientSpectrum operator*(Real v, const CoefficientSpectrum& cs) {
        return cs * v;
    }

    CoefficientSpectrum operator/(const CoefficientSpectrum& c1) const {
        ASSERT(!c1.hasNaNs()); CoefficientSpectrum cs;
        for (int i = 0; i < sampleSize; i++) {
            ASSERT(c1.samples[i] != 0);
            cs.samples[i] = samples[i] / c1.samples[i];
        }
        return cs;
    }
    CoefficientSpectrum& operator/=(const CoefficientSpectrum& c1) {
        ASSERT(!c1.hasNaNs());
        for (int i = 0; i < sampleSize; i++) {
            ASSERT(c1.samples[i] != 0);
            samples[i] /= c1.samples[i];
        }
        return *this;
    }
    CoefficientSpectrum operator/(Real v) const {
        ASSERT(!isNaN(v) && v != 0); CoefficientSpectrum cs;
        for (int i = 0; i < sampleSize; i++) cs.samples[i] = samples[i] / v;
        return cs;
    }
    CoefficientSpectrum& operator/=(Real v) {
        ASSERT(!isNaN(v) && v != 0);
        for (int i = 0; i < sampleSize; i++) samples[i] /= v;
        return *this;
    }

    inline bool operator==(const CoefficientSpectrum& c1) const {
        for (int i = 0; i < sampleSize; i++)
            if (samples[i] != c1.samples[i]) return false;
        return true;
    }
    inline bool operator!=(const CoefficientSpectrum& c1) const {
        for (int i = 0; i < sampleSize; i++)
            if (samples[i] == c1.samples[i]) return false;
        return true;
    }

    inline CoefficientSpectrum operator-() const {
        CoefficientSpectrum cs;
        for (int i = 0; i < sampleSize; i++) cs[i] = -samples[i];
        return cs;
    }

    bool isBlack() const {
        for (int i = 0; i < sampleSize; i++)
            if (samples[i] != 0) return false;
        return true;
    }

    CoefficientSpectrum sqrt(const CoefficientSpectrum& c1) const {
        CoefficientSpectrum cs;
        for (int i = 0; i < sampleSize; i++)
            cs.samples[i] = std::sqrt(c1.samples[i]);
        ASSERT(!cs.hasNaNs());
        return cs;
    }
    CoefficientSpectrum exp(const CoefficientSpectrum& c1) const {
        CoefficientSpectrum cs;
        for (int i = 0; i < sampleSize; i++)
            cs.samples[i] = std::exp(c1.samples[i]);
        return cs;
    }

    CoefficientSpectrum clamp(Real low = 0, Real high = Infinity) const {
        CoefficientSpectrum cs;
        for (int i = 0; i < sampleSize; i++)
            cs.samples[i] = phyr::clamp(samples[i], low, high);
        return cs;
    }

    Real maxComponentValue() const {
        Real maxv = samples[0];
        for (int i = 1; i < sampleSize; i++) maxv = std::max(samples[i], maxv);
        return maxv;
    }

    template <int N>
    friend inline CoefficientSpectrum<N> pow(const CoefficientSpectrum<N>& c1, Real e);

    inline Real operator[](int i) const { return samples[i]; }
    inline Real& operator[](int i) { return samples[i]; }

    static const int nSampleSize = sampleSize;

  protected:
    Real samples[sampleSize];
};


// SampledSpectrum definitions

// Wavelength defined in nanometres
static const int spectrumWavelengthStart = 400;
static const int spectrumWavelengthEnd = 700;
static const int spectrumSampleSize = 60;

class SampledSpectrum : public CoefficientSpectrum<spectrumSampleSize> {
  public:
    SampledSpectrum(Real v = 0) : CoefficientSpectrum(v) {}
    SampledSpectrum(const CoefficientSpectrum<spectrumSampleSize>& cs) :
        CoefficientSpectrum<spectrumSampleSize>(cs) {}

    /**
     * Create a SampledSpectrum from given lambda ranges and their values.
     * Original values passed are not modified.
     */
    static SampledSpectrum getFromSample(const Real* lambda, const Real* v, int n);

    /**
     * Initializes the X, Y, and Z sampled spectrum from given CIE spectrum
     */
    static void init() {
        // Initiate X, Y, and Z spectrums
        for (int i = 0; i < nSampleSize; i++) {
            Real l0 = lerp(Real(i) / nSampleSize,
                           spectrumWavelengthStart, spectrumWavelengthEnd);
            Real l1 = lerp(Real(i + 1) / nSampleSize,
                           spectrumWavelengthStart, spectrumWavelengthEnd);
            X.samples[i] = averageSampleRange(CIE_LAMBDA, CIE_X, nCIESamples, l0, l1);
            Y.samples[i] = averageSampleRange(CIE_LAMBDA, CIE_Y, nCIESamples, l0, l1);
            Z.samples[i] = averageSampleRange(CIE_LAMBDA, CIE_Z, nCIESamples, l0, l1);
        }

        // Initiate spectrums for RGB to XYZ conversion
        for (int i = 0; i < nSampleSize; ++i) {
            Real l0 = lerp(Real(i) / nSampleSize,
                           spectrumWavelengthStart, spectrumWavelengthEnd);
            Real l1 = lerp(Real(i + 1) / nSampleSize,
                           spectrumWavelengthStart, spectrumWavelengthEnd);
            rgbRefl2SpectWhite.samples[i] =
                    averageSampleRange(RGB2SpectLambda, RGBRefl2SpectWhite,
                                       nRGB2SpectSamples, l0, l1);
            rgbRefl2SpectCyan.samples[i] =
                    averageSampleRange(RGB2SpectLambda, RGBRefl2SpectCyan,
                                       nRGB2SpectSamples, l0, l1);
            rgbRefl2SpectMagenta.samples[i] =
                    averageSampleRange(RGB2SpectLambda, RGBRefl2SpectMagenta,
                                       nRGB2SpectSamples, l0, l1);
            rgbRefl2SpectYellow.samples[i] =
                    averageSampleRange(RGB2SpectLambda, RGBRefl2SpectYellow,
                                       nRGB2SpectSamples, l0, l1);
            rgbRefl2SpectRed.samples[i] =
                    averageSampleRange(RGB2SpectLambda, RGBRefl2SpectRed,
                                       nRGB2SpectSamples, l0, l1);
            rgbRefl2SpectGreen.samples[i] =
                    averageSampleRange(RGB2SpectLambda, RGBRefl2SpectGreen,
                                       nRGB2SpectSamples, l0, l1);
            rgbRefl2SpectBlue.samples[i] =
                    averageSampleRange(RGB2SpectLambda, RGBRefl2SpectBlue,
                                       nRGB2SpectSamples, l0, l1);
            rgbIllum2SpectWhite.samples[i] =
                    averageSampleRange(RGB2SpectLambda, RGBIllum2SpectWhite,
                                       nRGB2SpectSamples, l0, l1);
            rgbIllum2SpectCyan.samples[i] =
                    averageSampleRange(RGB2SpectLambda, RGBIllum2SpectCyan,
                                       nRGB2SpectSamples, l0, l1);
            rgbIllum2SpectMagenta.samples[i] =
                    averageSampleRange(RGB2SpectLambda, RGBIllum2SpectMagenta,
                                       nRGB2SpectSamples, l0, l1);
            rgbIllum2SpectYellow.samples[i] =
                    averageSampleRange(RGB2SpectLambda, RGBIllum2SpectYellow,
                                       nRGB2SpectSamples, l0, l1);
            rgbIllum2SpectRed.samples[i] =
                    averageSampleRange(RGB2SpectLambda, RGBIllum2SpectRed,
                                       nRGB2SpectSamples, l0, l1);
            rgbIllum2SpectGreen.samples[i] =
                    averageSampleRange(RGB2SpectLambda, RGBIllum2SpectGreen,
                                       nRGB2SpectSamples, l0, l1);
            rgbIllum2SpectBlue.samples[i] =
                    averageSampleRange(RGB2SpectLambda, RGBIllum2SpectBlue,
                                       nRGB2SpectSamples, l0, l1);
        }
    }

    static SampledSpectrum getFromRGB(const Real rgb[3],
                                      SpectrumType type = SpectrumType::Illuminant);
    static SampledSpectrum getFromXYZ(const Real xyz[3],
                                      SpectrumType type = SpectrumType::Reflectance) {
        Real rgb[3]; convertXYZToRGB(xyz, rgb);
        return getFromRGB(rgb, type);
    }

    /**
     * Calculates the x, y, and z constants from the XYZ spectrum data
     * using the Riemann method for integral approximation.
     */
    void toXYZConstants(Real xyz[3]) const {
        xyz[0] = xyz[1] = xyz[2] = 0;
        for (int i = 0; i < nSampleSize; i++) {
            xyz[0] += X.samples[i] * samples[i];
            xyz[1] += Y.samples[i] * samples[i];
            xyz[2] += Z.samples[i] * samples[i];
        }

        Real scale = (spectrumWavelengthEnd - spectrumWavelengthStart) /
                     (CIE_Y_LAMBDA * nCIESamples);
        xyz[0] *= scale; xyz[1] *= scale; xyz[2] *= scale;
    }

    void toRGBConstants(Real rgb[3]) const {
        // Calculate xyz constants and use that to convert to rgb
        Real xyz[3]; toXYZConstants(xyz);
        convertXYZToRGB(xyz, rgb);
    }

    /**
     * Calculates and returns the y constant. It is closely related to luminance
     * and is later used by some light transport algorithms for measuring the
     * relative importance of light-carrying paths through the scene.
     */
    Real getYConstant() const {
        Real yval = 0;
        for (int i = 0; i < nSampleSize; i++)
            yval += Y.samples[i] * samples[i];
        return yval * (spectrumWavelengthEnd - spectrumWavelengthStart) /
                      (CIE_Y_LAMBDA * nCIESamples);
    }

  private:
    /**
     * Create a SampledSpectrum from given lambda ranges and their values.
     * Assumes given data is already sorted by lambda.
     */
    static SampledSpectrum _getFromSample(const Real* lambda, const Real* v, int n);

    static SampledSpectrum X, Y, Z;
    // Reflectance spectrum
    static SampledSpectrum rgbRefl2SpectWhite, rgbRefl2SpectCyan;
    static SampledSpectrum rgbRefl2SpectMagenta, rgbRefl2SpectYellow;
    static SampledSpectrum rgbRefl2SpectRed, rgbRefl2SpectGreen;
    static SampledSpectrum rgbRefl2SpectBlue;
    // Illuminant spectrum
    static SampledSpectrum rgbIllum2SpectWhite, rgbIllum2SpectCyan;
    static SampledSpectrum rgbIllum2SpectMagenta, rgbIllum2SpectYellow;
    static SampledSpectrum rgbIllum2SpectRed, rgbIllum2SpectGreen;
    static SampledSpectrum rgbIllum2SpectBlue;
};


// Spectrum inlines
template <int N>
inline CoefficientSpectrum<N> pow(const CoefficientSpectrum<N>& c1, Real e) {
    CoefficientSpectrum<N> cs;
    for (int i = 0; i < N; i++) cs.samples[i] = std::pow(c1.samples[i], e);
    return cs;
}

}  // namespace phyrs

#endif
