#ifndef _FIMAGE_H_
#define _FIMAGE_H_

#include <fstream>
#include <cmath>
#include <cstring>
#include "vector.h"

class fimage {
    public:
    float *data;
    int width;
    int height;
    int count;
    float depth;
    char fileName[128];
    fimage(const char * name);
    fimage(int w, int h, int d);
    fimage(fimage *im);
    fimage();
    void setRes(int w, int h, int d);
    void setPixel(int x, int y,float r,float g,float b);
    void setPixel(int x, int y, Vec c);
    Vec getPixel(float x, float y);
    bool validCoord(int x, int y);
    void save();
    void testImage();
    void charFromFloat(fimage *im, unsigned char* pim, float depth);
    
    int LoadFile(const char * c);
};

#endif