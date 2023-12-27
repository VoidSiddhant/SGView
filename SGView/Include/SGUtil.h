#ifndef SG_UTIL_H
#define SG_UTIL_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <iostream>
#include <sstream>
#include <cmath>
#include <cstring>
#include <vector>
#include<memory>
#include<random>
#include<map>
#include <unordered_map>

namespace SGFramework
{
#define SG_Uint unsigned int
#define SG_Int GL_INT
#define SG_UChar unsigned char
#define SG_UUID	long int

#ifndef SCREEN_W
#define SCREEN_W 1280
#endif

#ifndef SCREEN_H
#define SCREEN_H 720
#endif

};

#endif