#ifndef __SGIMAGE_EFFECTS__
#define __SGIMAGE_EFFECTS__

#include "SGUtil.h"
#include "SGStencil.h"
#include "SGImageProc.h"
#include <complex.h>

namespace SGFramework
{
    class SGPoint
    {
    public:
        SGPoint();
        SGPoint(float);
        SGPoint(float, float);
        SGPoint(const SGPoint &invalue);
        SGPoint &operator*(const float &inpoint);
        SGPoint &operator*(const SGPoint &inpoint);
        SGPoint &operator=(const SGPoint &inpoint);
        SGPoint &operator+(const SGPoint &inpoint);
        SGPoint &operator+(const float &inpoint);
        SGPoint &operator-(const SGPoint &inpoint);
        SGPoint &operator-(const float &inpoint);
        SGPoint &operator/(const SGPoint &inpoint);
        SGPoint &operator/(const float &inpoint);

        ~SGPoint();

        float x, y;
    };

    class SGColor
    {
    public:
        SGColor();
        ~SGColor();
        SGColor(float r, float g, float b);
        SGColor(const SGColor &color);
        SGColor &operator*(const float &value);
        SGColor &operator=(const SGColor &color);
        SGColor &operator+(const SGColor &color);
        vector<float> Convert2Pixel();
        static void Pixel2RGB(SGPixel& pixel,SGColor& outColor);
        float r, g, b;
    };

    class SGImageEffect
    {
    public:
        // Adjust gamma value : 1.0 - is default
        static void SetGamma(SGImageProc &imageproc, const float &value);
        static void BoundedLinearConvolution(const SGStencil &stencil, SGImageProc &in, SGImageProc &out);
        static void JuliaSet(const SGPoint &center, const float &radius, const int &interations, SGImageProc &outImage);
        static void JuliaSet(SGImageProc &outProc);
        static void ContrastUnits(SGImageProc& outProc);
        static void HistogramEQ(SGImageProc& outProc);
    private:
        static SGPoint warp(const SGPoint &point, const int &iterations);
        static vector<float> lut(const float &deviation);
    };
};

#endif