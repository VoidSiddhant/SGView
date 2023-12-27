#include "SGCore.h"

namespace SGFramework
{
    SGCore *SGCore::instance = nullptr;

    void GlutDisplay()
    {
        SGCore::Instance().Render();
    }
    void GlutReshape(int w, int h)
    {
        glViewport(0,0,w,h);
        glutPostRedisplay();
    }
    SGCore::SGCore()
    {
    }

    SGCore &SGCore::Instance()
    {
        if (instance == nullptr)
            instance = new SGCore();
        return *instance;
    }

    SGCore::~SGCore()
    {
        auto iter = renderObjects.begin();
        while (iter != renderObjects.end())
        {
            if (iter->first == "SGImageProc")
            {
                // cout<<"Rendering : "<<iter->first<<endl;
                auto iter2 = iter->second.begin();
                while (iter2 != iter->second.end())
                {
                    SGImageProc* p = static_cast<SGImageProc*>(*iter2);
                    iter2++;
                    delete p;
                }
            }
            iter++;
        }
        delete instance;
    }

    int SGCore::InitEngine(int argc, char **argv, const char *windowTitle)
    {
        try{
            glutInit(&argc, argv);
            glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
            glutInitWindowSize(SCREEN_W, SCREEN_H);
            glutCreateWindow(windowTitle);
            glClearColor(0.5, 0.5, 0.6, 0.0);
            glutIdleFunc(&GlutIdle);
            glutReshapeFunc(GlutReshape);
            glutDisplayFunc(GlutDisplay);
        }
        catch(...)
        {
            return 0;
        }

        return 1;
    }

    void SGCore::AttachCallback_Keyboard(void (*callback)(SG_UChar, int, int))
    {
        glutKeyboardFunc(callback);
    }

    void SGCore::AttachCallback_Mouse(void (*callback)())
    {
    }

    void SGCore::AddRenderObject(const char *objectname, void *obj)
    {
        renderObjects[objectname].push_back(obj);
    }

    int SGCore::Run()
    {
        glutMainLoop();
        return 0;
    }

    void SGCore::Render()
    {
        glClear(GL_COLOR_BUFFER_BIT);

        auto iter = renderObjects.begin();
        while (iter != renderObjects.end())
        {
            if (iter->first == "SGImageProc")
            {
                // cout<<"Rendering : "<<iter->first<<endl;
                auto iter2 = iter->second.begin();
                while (iter2 != iter->second.end())
                {
                    SGImageProc *p = static_cast<SGImageProc *>(*iter2);
                    iter2++;
                    if(p->GetRenderState() == false) continue;
                    p->PaintPixels();
                }
            }
            iter++;
        }
        glutSwapBuffers();
        glutPostRedisplay();
    }

};