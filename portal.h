#ifndef PORTAL_H
#define PORTAL_H
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include "solidSphere.h"
#include "loadBMP.h"

// ////////////////////////////////////////////////////////////////////////////
class portal
{
        private:
                solidSphere portA, portB,
                            innerA, innerB;
                loadBMP lb;
                float portLocA[3],
                      portLocB[3],
                      enterPoint[3];
                int entered;
                unsigned int ptexa, ptexb;
        public:
                portal();
                portal(float *a, float *b, float rad);
                portal(float a, float b, float c, 
                       float x, float y, float z,
                       float rad);
                void draw();
                void loc(float x, float y, float z);
                void reLocateOBJ(float *point, float *newLoc);
                void reLocateOBJ(float x, float y, float z,
                                 float &nx, float &ny, float &nz);
                void assignTexA(unsigned int txt);
                void assignTexB(unsigned int txt);

};
#endif
