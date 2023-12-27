#include "SGImageEffect.h"

namespace SGFramework
{

    SGPoint::SGPoint()
    {
        x = y = 0;
    }

    SGPoint::SGPoint(float value)
    {
        this->x = value;
        this->y = value;
    }

    SGPoint::SGPoint(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    SGPoint::SGPoint(const SGPoint &invalue)
    {
        this->x = invalue.x;
        this->y = invalue.y;
    }

    SGPoint &SGPoint::operator=(const SGPoint &invalue)
    {
        this->x = x;
        this->y = y;
        return *this;
    }

    SGPoint &SGPoint::operator*(const float &invalue)
    {
        this->x *= invalue;
        this->y *= invalue;
        return *this;
    }

    SGPoint &SGPoint::operator*(const SGPoint &invalue)
    {
        this->x *= invalue.x;
        this->y *= invalue.y;
        return *this;
    }

    SGPoint &SGPoint::operator+(const float &invalue)
    {
        this->x += invalue;
        this->y += invalue;
        return *this;
    }

    SGPoint &SGPoint::operator+(const SGPoint &invalue)
    {
        this->x += invalue.x;
        this->y += invalue.y;
        return *this;
    }

    SGPoint &SGPoint::operator-(const float &invalue)
    {
        this->x += invalue;
        this->y += invalue;
        return *this;
    }

    SGPoint &SGPoint::operator-(const SGPoint &invalue)
    {
        this->x += invalue.x;
        this->y += invalue.y;
        return *this;
    }

    SGPoint &SGPoint::operator/(const float &invalue)
    {
        this->x /= invalue;
        this->y /= invalue;
        return *this;
    }

    SGPoint &SGPoint::operator/(const SGPoint &invalue)
    {
        this->x /= invalue.x;
        this->y /= invalue.y;
        return *this;
    }

    SGPoint::~SGPoint()
    {
    }

    SGColor::SGColor()
    {
        r = g = b = 1.0f;
    }

    SGColor::SGColor(float r, float g, float b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    SGColor::SGColor(const SGColor &color)
    {
        this->r = color.r;
        this->g = color.g;
        this->b = color.b;
    }

    SGColor &SGColor::operator*(const float &value)
    {
        r *= value;
        g *= value;
        b *= value;
        return *this;
    }

    SGColor &SGColor::operator+(const SGColor &value)
    {
        r += value.r;
        g += value.g;
        b += value.b;
        return *this;
    }

    SGColor &SGColor::operator=(const SGColor &color)
    {
        this->r = color.r;
        this->g = color.g;
        this->b = color.b;
        return *this;
    }

    vector<float> SGColor::Convert2Pixel()
    {
        return {r, g, b};
    }

    void SGColor::Pixel2RGB(SGPixel &pixel, SGColor &outColor)
    {
        outColor.r = pixel[0];
        outColor.g = pixel[1];
        outColor.b = pixel[2];
    }

    SGColor::~SGColor()
    {
    }

    SGPoint SGImageEffect::warp(const SGPoint &point, const int &iterations)
    {
        std::complex<float> zn = std::complex<float>(point.x, point.y);
        std::complex<float> zc = std::complex<float>(0.8f * cos(254.3f * 3.14159265f / 180.0f), 0.8f * sin(254.3f * 3.14159265f / 180.0f));
        // zc.real(zc.real() / 1.0e-3);
        // zc.imag(zc.imag() / 1.0e-3);

        // std::complex<float> zc = std::complex<float>(-0.8f , 0.0f);
        //  cout << "zn : " << zn.real() << " , " << zn.imag() << endl;
        //  cout << "zc : " << zc.real() << " , " << zc.imag() << endl;
        int N = iterations;
        int cycles = 2;

        for (int i = 0; i < N; i++)
        {
            for (int c = 1; c < cycles; c++)
            {
                zn *= zn;
            }
            zn = zn + zc;
        }
        // cout << "zn : " << zn.real() << " , " << zn.imag() << endl;
        return SGPoint(zn.real(), zn.imag());
    }

    vector<float> SGImageEffect::lut(const float &deviation)
    {

        SGColor colorSamples[8] = {

            SGColor(0.25, 0.5, 0.75),
            SGColor(0.9, 0.2, 0.6),
            SGColor(0.1, 0.7, 0.3),
            SGColor(0.8, 0.4, 0.2),
            SGColor(0.65, 0.35, 0.9),
            SGColor(0.45, 0.75, 0.15),
            SGColor(0.3, 0.9, 0.5),
            SGColor(0.6, 0.1, 0.8),
        };
        if (deviation > 1.0f || std::isinf(deviation) || std::isnan(deviation))
        {
            // cout << "Infinity : " << deviation << endl;
            return std::vector<float>(3.0, 0.0);
        }
        else
        {

            // we sample using linear interpolation
            int M = 7;
            float x = (deviation) * (float)M;
            int m = int(x);
            float w = x - m;
            int mplus1 = m + 1;
            if (mplus1 >= M)
                mplus1 = M;
            SGColor outColor = colorSamples[m] * (1.0f - w) + colorSamples[mplus1] * w;
            vector<float> outPixel = {outColor.r, outColor.g, outColor.b};
            return outPixel;
        }
    }

    void SGImageEffect::JuliaSet(const SGPoint &center, const float &radius, const int &iterations, SGImageProc &outImage)
    {
        cout << "Computing Julia Set \n";
        int lastPro = 0;
        for (int y = 0; y < outImage.image.imageResY; y++)
        {
            for (int x = 0; x < outImage.image.imageResX; x++)
            {
                // cout << x << endl;
                //  Transform to [-1, 1] range
                SGPoint point;
                point.x = 2.0f * (x / (float)outImage.image.imageResX) - 1.0f;
                point.y = 2.0f * (y / (float)outImage.image.imageResY) - 1.0f;
                point.x *= radius;
                point.y *= radius;
                point.x += center.x * radius;
                point.y += center.y * radius;
                // cout << "Point : " << point.x << " , ," << point.y << endl;
                SGPoint PP = warp(point, iterations);
                float rate = std::sqrt(PP.x * PP.x + PP.y * PP.y) / 2.0f;
                // float rate = std::sqrt() / 1.0f;

                vector<float> pixel = lut(rate);
                outImage.SetPixelValue(x, y, pixel);
            }
            float outNy = outImage.image.imageResY;
            float progress = ((float)y / outNy);
            int curPro = progress * 100.0;
            if (curPro == lastPro)
                continue;
            lastPro = curPro;
            int barWidth = 35;

            std::cout << "[";
            int pos = barWidth * progress;
            for (int i = 0; i < barWidth; ++i)
            {
                if (i < pos)
                    std::cout << "=";
                else if (i == pos)
                    std::cout << ">";
                else
                    std::cout << " ";
            }
            std::cout << "] " << int(progress * 100.0) << " %\r";
            std::cout.flush();
            std::cout << std::endl;
        }
        cout << "Computing Julia Set Complete \n";
    }

    template <typename T>
    T Lerp(T v1, T v2, float dt)
    {
        return (v1 + (v2 - v1) * dt);
    }

    SGPoint lin2rgb(SGPoint lin)
    {
        lin.x = pow(lin.x, 1.0f / 2.2f);
        lin.y = pow(lin.y, 1.0f / 2.0f);
        return lin;
    }

    float lin2rgb(float lin)
    {
        lin = pow(lin, 1.0f / 2.2f);
        return lin;
    }

    void SGImageEffect::SetGamma(SGImageProc &imageproc, const float &value = 1.0)
    {
        cout << "Applying Gamma : " << value << endl;
        size_t size = imageproc.image.imageResX * imageproc.image.imageResY * imageproc.image.nChannels;
        for (size_t index = 0; index < size; index++)
        {
            imageproc.image.pixelData[index] = std::pow(imageproc.image.pixelData[index], value);
        }
    }
    void SGImageEffect::BoundedLinearConvolution(const SGStencil &stencil, SGImageProc &in, SGImageProc &out)
    {
        cout << "Applying Linear Convolution : " << endl;
        int halfWidth = stencil.getWidth() / 2.0;
        cout << "Half Width : " << halfWidth << endl;
        uint inNy = in.image.imageResY;
        uint inNx = in.image.imageResX;
        int inDepth = in.image.nChannels;

        out.Clear(inNx, inNy, inDepth);
        uint outNy = out.image.imageResY;
        uint outNx = out.image.imageResX;
        int outDepth = out.image.nChannels;
        cout << "Out info : " << outNx << endl;
        cout << "Out info : " << outDepth << endl;
        cout << "Out info : " << outNy << endl;

        // Pick a pixel starting from 0,0
        int lastPro = 0;
        for (int j = 0; j < outNy; j++)
        {
            // Expand the center of the stencil on both sides forming the upper and lower cieling of the box
            int jmin = j - halfWidth;
            int jmax = j + halfWidth;
            for (int i = 0; i < outNx; i++)
            {
                // Expand the center of the stencil on both sides forming the right and left cieling of the box
                int imin = i - halfWidth;
                int imax = i + halfWidth;
                std::vector<float> pixel(outDepth, 0.0);
                std::vector<float> sample(inDepth, 0.0);
                // Iterate each pixel point in the stencil/Image and sample.
                for (int jj = jmin; jj <= jmax; jj++)
                {
                    int stencilj = jj - jmin;
                    int jjj = jj;
                    if (jjj < 0)
                        continue; // jjj += outNy;   //Since pixel array is cleared to 0.0 black, we can skip sampling for index outside the boundary
                    if (jjj >= outNy)
                        continue; // jjj -= outNy;   //Since pixel array is cleared to 0.0 black, we can skip sampling for index outside the boundary

                    for (int ii = imin; ii <= imax; ii++)
                    {
                        int stencili = ii - imin;
                        int iii = ii;
                        if (iii < 0)
                            continue; // iii += outNx;   //Since pixel array is cleared to 0.0 black, we can skip sampling for index outside the boundary
                        if (iii >= outNx)
                            continue; // iii -= outNx   //Since pixel array is cleared to 0.0 black, we can skip sampling for index outside the boundary

                        const float &stencil_value = stencil.getValue(stencili, stencilj);
                        in.GetPixelValue(iii, jjj, sample);
                        for (size_t c = 0; c < sample.size(); c++)
                        {

                            pixel[c] += sample[c] * stencil_value;
                            // Clamp these values to 0 and 1 range.
                            // pixel[c] = clamp<float>(pixel[c], 0.0f, 1.0f);
                        }
                    }
                    out.SetPixelValue(i, j, pixel);
                }
            }
            float progress = (((float)j / (float)outNy));
            int curPro = progress * 100.0;
            if (curPro == lastPro)
                continue;
            lastPro = curPro;
            int barWidth = 35;

            std::cout << "[";
            int pos = barWidth * progress;
            for (int i = 0; i < barWidth; ++i)
            {
                if (i < pos)
                    std::cout << "=";
                else if (i == pos)
                    std::cout << ">";
                else
                    std::cout << " ";
            }
            std::cout << "] " << int(progress * 100.0) << " %\r";
            std::cout.flush();
            std::cout << std::endl;
        }
        cout << "Exited Convolution" << endl;
    }

    void SGImageEffect::ContrastUnits(SGImageProc &outProc)
    {
        // First we find the average color of each channel ~C

        float avgR = 0.0f, avgG = 0.0f, avgB = 0.0f;
        float rmsR, rmsG, rmsB;
        rmsR = rmsG = rmsB = 0.0f;
        float sizeX_f = outProc.image.imageResX;
        float sizeY_f = outProc.image.imageResY;

        for (int y = 0; y < outProc.image.imageResY; y++)
        {
            for (int x = 0; x < outProc.image.imageResX; x++)
            {
                SGPixel pixel;
                SGColor color;
                outProc.GetPixelValue(x, y, pixel);
                SGColor::Pixel2RGB(pixel, color);
                avgR += color.r;
                avgG += color.g;
                avgB += color.b;
            }
        }

        avgR /= (sizeX_f * sizeY_f);
        avgG /= (sizeX_f * sizeY_f);
        avgB /= (sizeX_f * sizeY_f);

        for (int y = 0; y < outProc.image.imageResY; y++)
        {
            for (int x = 0; x < outProc.image.imageResX; x++)
            {
                SGPixel pixel;
                SGColor color;
                outProc.GetPixelValue(x, y, pixel);
                SGColor::Pixel2RGB(pixel, color);
                rmsR += (color.r - avgR) * (color.r - avgR);
                rmsG += (color.g - avgG) * (color.g - avgG);
                rmsB += (color.b - avgB) * (color.b - avgB);
            }
        }

        rmsR /= (sizeX_f * sizeY_f);
        rmsG /= (sizeX_f * sizeY_f);
        rmsB /= (sizeX_f * sizeY_f);

        rmsR = sqrt(rmsR);
        rmsG = sqrt(rmsG);
        rmsB = sqrt(rmsB);
        int lastPro = 0;
        for (int y = 0; y < outProc.image.imageResY; y++)
        {
            for (int x = 0; x < outProc.image.imageResX; x++)
            {
                // Contrast Units
                SGPixel pixel;
                SGColor color;
                outProc.GetPixelValue(x, y, pixel);
                pixel[0] -= avgR;
                pixel[0] /= rmsR;

                pixel[1] -= avgG;
                pixel[1] /= rmsG;

                pixel[2] -= avgB;
                pixel[2] /= rmsB;
                outProc.SetPixelValue(x, y, pixel);
            }
            float progress = ((float)y / sizeY_f);
            int curPro = progress * 100.0;
            if(curPro == lastPro)
                continue;
            lastPro = curPro;
            int barWidth = 35;

            std::cout << "[";
            int pos = barWidth * progress;
            for (int i = 0; i < barWidth; ++i)
            {
                if (i < pos)
                    std::cout << "=";
                else if (i == pos)
                    std::cout << ">";
                else
                    std::cout << " ";
            }
            std::cout << "] " << int(progress * 100.0) << " %\r";
            std::cout.flush();
            std::cout << std::endl;
        }
    }

    void SGImageEffect::HistogramEQ(SGImageProc &outProc)
    {
        int binSize = 500;
        vector<int> histr(binSize, 0);
        vector<int> histg(binSize, 0);
        vector<int> histb(binSize, 0);
        float sizeX_f = outProc.image.imageResX;
        float sizeY_f = outProc.image.imageResY;
        // min-max values should be the size of the image data in each channel
        // we let random value data set to be our imagePixel data
        // 500 bins for each Histogram

        float maxR, maxG, maxB, minR, minG, minB, dR, dG, dB;
        maxR = maxG = maxB = 0.0f;
        minR = minG = minB = 1.0f;

        // lets find emin and emax : min-max values in each channel
        for (int y = 0; y < outProc.image.imageResY; y++)
        {
            for (int x = 0; x < outProc.image.imageResX; x++)
            {
    
                SGPixel pixel;
                SGColor color;
                outProc.GetPixelValue(x, y, pixel);
                SGColor::Pixel2RGB(pixel, color);
                maxR = (color.r > maxR) ? color.r : maxR;
                minR = (color.r < minR) ? color.r : minR;

                maxG = (color.g > maxG) ? color.g : maxG;
                minG = (color.g < minG) ? color.g : minG;

                maxB = (color.b > maxB) ? color.b : maxB;
                minB = (color.b < minB) ? color.b : minB;

                dR = (maxR - minR) / (float)binSize;
                dG = (maxG - minG) / (float)binSize;
                dB = (maxB - minB) / (float)binSize;
            }
        }

        // Construct histogram
        for (int y = 0; y < outProc.image.imageResY; y++)
        {
            for (int x = 0; x < outProc.image.imageResX; x++)
            {
           
                SGPixel pixel;
                SGColor color;
                outProc.GetPixelValue(x, y, pixel);
                SGColor::Pixel2RGB(pixel, color);
                int binIndexR = (color.r - minR) / dR;
                int binIndexG = (color.g - minG) / dG;
                int binIndexB = (color.b - minB) / dB;
                if(binIndexR >= 0 && binIndexR < binSize)
                    histr[binIndexR]++;
                if(binIndexG >= 0 && binIndexG < binSize)
                    histg[binIndexG]++;
                if(binIndexB >= 0 && binIndexB < binSize)
                    histb[binIndexB]++;
            }
        }

        // Calculate PDF
        vector<float> histrPdf(binSize, 0.0f);
        vector<float> histgPdf(binSize, 0.0f);
        vector<float> histbPdf(binSize, 0.0f);
        for (int i = 0; i < binSize; i++)
        {
            histrPdf[i] = histr[i] / (sizeX_f * sizeY_f);
            histgPdf[i] = histg[i] / (sizeX_f * sizeY_f);
            histbPdf[i] = histb[i] / (sizeX_f * sizeY_f);
        }

        // Check if PDF is correct : Sum(pdf) = 1
        float sumR, sumG, sumB;
        sumR = sumG = sumB = 0;
        for (int i = 0; i < binSize; i++)
        {
            sumR += histrPdf[i];
            sumG += histgPdf[i];
            sumB += histbPdf[i];
        }

        cout << "Histogram PDF -> R : G : B = " << sumR << " : " << sumG << " : " << sumB << endl;

        // Calculate CDF
        vector<float> histrCdf(binSize, 0.0f);
        vector<float> histgCdf(binSize, 0.0f);
        vector<float> histbCdf(binSize, 0.0f);
        histrCdf[binSize - 1] = 1.0;
        histgCdf[binSize - 1] = 1.0;
        histbCdf[binSize - 1] = 1.0;
        for (int i = 1; i < binSize - 1; i++)
        {
            histrCdf[i] = histrCdf[i - 1] + histrPdf[i];
            histgCdf[i] = histgCdf[i - 1] + histgPdf[i];
            histbCdf[i] = histbCdf[i - 1] + histbPdf[i];
        }

        // Now we equalize
        // Construct histogram
        int lastPro = 0;
        for (int y = 0; y < outProc.image.imageResY; y++)
        {
            for (int x = 0; x < outProc.image.imageResX; x++)
            {
                // Contrast Units
                SGPixel pixel;
                SGColor color;
                outProc.GetPixelValue(x, y, pixel);
                SGColor::Pixel2RGB(pixel, color);
                int bindIndexR = (color.r - minR) / dR;
                color.r = histrCdf[bindIndexR];
                int bindIndexG = (color.g - minG) / dG;
                color.g = histgCdf[bindIndexG];
                int bindIndexB = (color.b - minB) / dB;
                color.b = histbCdf[bindIndexB];
                pixel = color.Convert2Pixel();
                outProc.SetPixelValue(x,y,pixel);
            }
            float progress = ((float)y / sizeY_f);
            int curPro = progress * 100.0;
            if(curPro == lastPro)
                continue;
            lastPro = curPro;
            int barWidth = 35;

            std::cout << "[";
            int pos = barWidth * progress;
            for (int i = 0; i < barWidth; ++i)
            {
                if (i < pos)
                    std::cout << "=";
                else if (i == pos)
                    std::cout << ">";
                else
                    std::cout << " ";
            }
            std::cout << "] " << int(progress * 100.0) << " %\r";
            std::cout.flush();
            std::cout << std::endl;
        }
    }
}