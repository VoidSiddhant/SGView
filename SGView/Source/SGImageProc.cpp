#include "SGImageProc.h"

namespace SGFramework
{
    SGImage::SGImage()
    {
        pixelData = nullptr;
        filename = nullptr;
        imageResX = imageResY = nChannels = 0;
    }
    SGImage::SGImage(const SGImage &v)
    {
        filename = v.filename;
        this->imageResX = v.imageResX;
        imageResY = v.imageResY;
        this->nChannels = v.nChannels;
        long size = v.imageResX * v.imageResY * v.nChannels;
        this->pixelData = new float[size];
        for (int i = 0; i < size; i++)
        {
            pixelData[i] = v.pixelData[i];
        }
    }

    SGImage &SGImage::operator=(const SGImage &v)
    {
        filename = v.filename;
        this->imageResX = v.imageResX;
        imageResY = v.imageResY;
        this->nChannels = v.nChannels;
        long size = v.imageResX * v.imageResY * v.nChannels;
        this->pixelData = new float[size];
        for (int i = 0; i < size; i++)
        {
            pixelData[i] = v.pixelData[i];
        }
        return *this;
    }

    SGImage::~SGImage()
    {
        delete[] pixelData;
    }

    void SGImage::Clear()
    {
        delete[] pixelData;
        pixelData = nullptr;
        filename = nullptr;
        imageResX = imageResY = nChannels = 0;
    }

    SGImageProc::SGImageProc()
    {
        nameID = "SGImageProc";
        image.imageResX = image.imageResY = image.nChannels = 0;
        image.pixelData = nullptr;
        renderObjExist = renderEnabled = false;
    }

    SGImageProc::SGImageProc(const char *filename)
    {
        nameID = "SGImageProc";
        this->LoadPixelData(filename);
        renderObjExist = renderEnabled = false;
    }

    SGImageProc::SGImageProc(const SGImageProc &v)
    {
        nameID = "SGImageProc";
        image = v.image;
        renderObjExist = renderEnabled = false;
    }

    SGImageProc &SGImageProc::operator=(const SGImageProc &v)
    {
        nameID = "SGImageProc";
        image = v.image;
        return *this;
    }

    void SGImageProc::LoadPixelData(const char *filename)
    {
        cout << "Loading Pixel Data ........" << filename << endl;
        auto inp = ImageInput::open(filename);
        if (!inp)
        {
            std::cerr << "Failed to open image." << std::endl;
            exit(1);
        }

        const ImageSpec &spec = inp->spec();
        this->image.imageResX = spec.width;
        this->image.imageResY = spec.height;
        this->image.nChannels = spec.nchannels;

        // pixelData = std::unique_ptr<unsigned char[]>(new unsigned char[xres * yres * nchannels]);
        this->image.pixelData = new float[spec.image_pixels() * this->image.nChannels];
        inp->read_image(0, 0, 0, this->image.nChannels, TypeDesc::FLOAT, &this->image.pixelData[0]); // This needs to change to float input. for value to be 0-1.0
        inp->close();
        cout << "Loading Pixel Data ........" << filename << " Complete" << endl;
    }

    void SGImageProc::WritePixelData(const char *filename)
    {
        cout << "Writing pixel data to file " << filename << endl;
        std::unique_ptr<ImageOutput> out = ImageOutput::create(filename);
        if (!out)
            return;
        ImageSpec spec(image.imageResX, image.imageResY, image.nChannels, TypeDesc::FLOAT);
        out->open(filename, spec);
        out->write_image(TypeDesc::FLOAT, &image.pixelData[0]);
        out->close();
        cout << "Writing Pixel Data ........" << filename << " Complete" << endl;
    }

    void SGImageProc::Draw(const bool &state)
    {
        if (!renderObjExist)
        {
            SGCore::Instance().AddRenderObject(nameID.c_str(), this);
            renderObjExist = true;
        }
        renderEnabled = state;
    }

    void SGImageProc::PaintPixels()
    {
        glRasterPos2f(-1, 1);
        glPixelZoom(1, -1);
        if (image.nChannels <= 3)
            glDrawPixels(image.imageResX, image.imageResY, GL_RGB, GL_FLOAT, image.pixelData);
        else
            glDrawPixels(image.imageResX, image.imageResY, GL_RGBA, GL_FLOAT, image.pixelData);
    }

    void SGImageProc::GetPixelValue(const int &i, const int &j, vector<float> &outPixel)
    {
        if (image.pixelData == nullptr)
            return;
        if (i < 0 || i >= image.imageResX)
            return;
        if (j < 0 || j >= image.imageResY)
            return;

        outPixel.resize(image.nChannels);
        for (int index = 0; index < image.nChannels; index++)
        {
            outPixel[index] = image.pixelData[index + image.nChannels * (i + j * image.imageResX)];
        }
        //
    }

    void SGImageProc::SetPixelValue(int i, int j, vector<float> &inPixel)
    {
        if (image.pixelData == nullptr)
            return;
        if (i < 0 || i >= image.imageResX)
            return;
        if (j < 0 || j >= image.imageResY)
            return;
        for (int index = 0; index < image.nChannels; index++)
        {
            image.pixelData[IndexAt(i, j, index)] = inPixel[index];
        }
    }
    void SGImageProc::Clear()
    {
        image.Clear();
    }

    void SGImageProc::Clear(int nx, int ny, int nc)
    {
        image.imageResX = nx;
        image.imageResY = ny;
        image.nChannels = nc;
        size_t size = nx * ny * nc;
        image.pixelData = new float[size];
        for (int i = 0; i < size; i++)
        {
            image.pixelData[i] = 0;
        }
    }

    SGImageProc::~SGImageProc()
    {
    }

};