#ifndef __SGIMAGEPROC_H__
#define __SGIMAGEPROC_H__

#include "SGCore.h"
#include <OpenImageIO/imageio.h>

using namespace OIIO;

namespace SGFramework
{
    #ifndef SGPixel
    #define SGPixel vector<float>
    #endif
    
    struct SGImage
    {
        public:
        SGImage();
        SGImage(const SGImage& v);
        SGImage& operator=(const SGImage& v);
        void Clear();
        ~SGImage();
        char* filename;
        SG_Uint imageResX, imageResY, nChannels;
        float* pixelData;
    };

    class SGImageProc
    {
        public :
        SGImageProc();
        SGImageProc(const char* filename);
        SGImageProc(const SGImageProc& v);
        SGImageProc& operator=(const SGImageProc& v);
        ~SGImageProc();


        void LoadPixelData(const char* filename);
        void WritePixelData(const char* filename);
        void Draw(const bool& state);
        bool GetRenderState() const {return renderEnabled;}
        void PaintPixels();
        void GetPixelValue(const int& i,const int& j, std::vector<float>& outPixel);
        void SetPixelValue( int i, int j, std::vector<float>& inPixel);
        void Clear();
        void Clear(int nx,int ny, int nc);
        int IndexAt(int i, int j, int channel) const {return channel + image.nChannels * (i+ j*image.imageResX);}
        
        std::string nameID;

        SGImage image;

        private:
        bool renderObjExist;
        bool renderEnabled;

    };
};

#endif