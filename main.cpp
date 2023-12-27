#include "SGCore.h"
#include "SGImageProc.h"
#include "SGImageEffect.h"

using namespace SGFramework;

SGImageProc *imageProcActive;

std::string ext;
SGStencil stencil(5);

float g = 1.0f;
SG_Uint fileCounter = 0;

// JULIA PARAMETERS.............................................................................
int iteration = 100;
SGPoint center(0.03811f, 0.01329f);
double range = 1.0f;
// ................................................................................................

void Input(unsigned char key, int x, int y)
{
    if (key == 'J')
    {
        // std::string ch = "myFile" + std::to_string(fileCounter) + ext;
        // imageProcActive->WritePixelData(ch.c_str());
        // fileCounter++;
         SGImageEffect::JuliaSet(center, range, iteration, *imageProcActive);
    }
    if (key == 'j')
    {
        // std::string ch = "julia_"+std::to_string(range) +std::to_string(iteration)+ ext;
        std::string ch = "saveFile_" + std::to_string(fileCounter) + ".jpeg";
        imageProcActive->WritePixelData(ch.c_str());
        fileCounter++;
    }
    if(key == 'o' || key == 'O')
    {
        std::string ch = "saveFile_" + std::to_string(fileCounter) + ".exr";
        imageProcActive->WritePixelData(ch.c_str());
        fileCounter++;
    }
    if (key == 'C')
    {
        SGImageEffect::ContrastUnits(*imageProcActive);
    }
    if (key == 'H')
    {
        SGImageEffect::HistogramEQ(*imageProcActive);
    }
    if (key == 'g')
    {
        g = 0.9f;
        SGImageEffect::SetGamma(*imageProcActive, g);
    }
    if (key == 'G')
    {
        g = 1.111111f;
        SGImageEffect::SetGamma(*imageProcActive, g);
    }
    if (key == 's')
    {
        cout << "Generating new Stencil....." << endl;
        stencil.GenerateStencil();
        stencil.DebugOutput();
        // return;
        SGImageProc tmpProc;
        SGImageEffect::BoundedLinearConvolution(stencil, *imageProcActive, tmpProc);
        *imageProcActive = tmpProc; // Deep copy
    }
}

void SGFramework::GlutIdle()
{
}

int main(int argc, char **argv)
{
    std::string filename;
    if (argc <= 1)
    {
        cout << "Image file not provided : Please use format -image <filename>\n";
        return 0;
    }
    else if (argc >= 3)
    {
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "-image") == 0)
            {
                if (i + 1 >= argc)
                {
                    cout << "Please use correct format : -image <filename>\n";
                    return 0;
                }
                filename = argv[i + 1];
                // Get the extension as well
                auto index = filename.rfind('.');
                ext = filename.substr(index);
                cout << "File extension : " << ext << endl;
            }
        }
    }
    else
    {
        cout << "Please use correct format : -image <filename>\n";
        return 0;
    }
    cout << "Opening Image file : " << filename << endl;
    if (!SGCore::Instance().InitEngine(argc, argv, "Assignemnt#2"))
    {
        cout << "init engine failed.....exiting\n";
        return 0;
    }
    SGCore::Instance().AttachCallback_Keyboard(Input);
    imageProcActive = new SGImageProc(filename.c_str());
    imageProcActive->Draw(true);
    glutReshapeWindow(imageProcActive->image.imageResX, imageProcActive->image.imageResY);
    return SGCore::Instance().Run();
}
