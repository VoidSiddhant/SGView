#include "SGStencil.h"

namespace SGFramework
{
    SGStencil::SGStencil(const int &in_width)
    {
        width = 2 * in_width + 1;
        stencil.resize(width, std::vector<double>(width, 0.0));
        GenerateStencil();
    }

    void SGStencil::GenerateStencil()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> distribution(-0.1, 0.1);

        // Fill the stencil with random values
        double centerValue = 0.0;
        int centerIndex = width / 2;
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (j == centerIndex && i == centerIndex)
                    continue;                                   // We calculate the center value seperately
                stencil[i][j] = distribution(gen);
                centerValue += stencil[i][j];
            }
        }
//        cout<<"Center index : "<<centerIndex<<endl;
//        cout<<"Total Value : "<<centerValue<<endl;
        stencil[centerIndex][centerIndex] = 1.0 - centerValue; /// Center value should be such that the total sum with all values = 1.0
//        cout<<"Center Value : "<<stencil[centerIndex][centerIndex]<<endl;
    }

    void SGStencil::DebugOutput()
    {
        for (int y = 0; y < getWidth(); y++)
        {
            for (int x = 0; x < getWidth(); x++)
            {
                cout << getValue(x, y) << " | ";
            }
            cout << endl;
        }
    }
}