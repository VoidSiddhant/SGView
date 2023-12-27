#ifndef __SGSTENCIL__
#define __SGSTENCIL__
#include"SGUtil.h"

using namespace std;

namespace SGFramework
{
    class SGStencil
    {
        public:
        SGStencil(const int& width);
        int getWidth() const {return width;}
        double getValue(int x , int y) const {return stencil[x][y];}
        void GenerateStencil();
        void DebugOutput();

        private:
        int width;
        std::vector<std::vector<double>> stencil;
    };
};

#endif