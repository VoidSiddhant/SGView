#ifndef __SGCORE_H__
#define __SGCORE_H__

#include "SGUtil.h"
#include<map>
#include "SGImageProc.h"


using namespace std;


namespace SGFramework
{
    void GlutDisplay();
    void GlutReshape(int w, int h);
    void GlutIdle();
    class SGImageProc;

    class SGCore
    {
        public:
        //Singleton Instance
            static SGCore &Instance();

            static int InitEngine(int argc, char** argv, const char*);
            int Run();
            void AttachCallback_Keyboard(void (*callback)(SG_UChar, int , int));
            void AttachCallback_Mouse(void (*callback)());
            void AddRenderObject(const char* ,void*);
            void Render();

            ~SGCore();
        private:
        SGCore();
        static SGCore* instance;
         std::unordered_map<std::string,vector<void*>> renderObjects;
    };
};

#endif
