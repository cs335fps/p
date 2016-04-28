/*
this iteration requieres that the obj file is using 
triangles not squares
1) no textures
2) no normals
3) no edges

*/
#ifndef WORLDENGINE_H
#define WORLDENGINE_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <cmath>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdio.h>

using namespace std;

struct vec{
               GLfloat v1[3],v2[3],v3[3];
               vec(GLfloat a,GLfloat b,GLfloat c,
                   GLfloat d,GLfloat e,GLfloat f,
                   GLfloat g,GLfloat h,GLfloat i)
               {
                    v1[0] = a; v1[1]= b; v1[2] = c;
                    v2[0] = d; v2[1]= e; v2[2] = f;
                    v3[0] = g; v3[1]= h; v3[2] = i;
               }
               vec(){}
          };
          struct bitree
          {
               bitree *left, *right;
               bitree *yroot;
               GLfloat xory;
               vec *fce;

               bitree(GLfloat x)
               {
                    left = right = NULL;
                    yroot = NULL;
                    xory = x;
               }
          };

class worldEngine
{
     private:
          float norm[3], vr[3];
          GLfloat x, y, z, rx, ry, rz;
          vector<GLfloat> vertex;
          vector<GLfloat> normals;
          vector<GLfloat> Faces_Triangles;
          vector<GLfloat> vertexBuffer;
          vector<GLfloat> UVs;
          long TotalConnectedPoints;
          long TotalConnectedTriangles;
          bool solid;
          bitree *root;

          bitree *newNode(GLfloat coord);
          void bitreeAdd(vec *fce);
          void bitreeAddY(bitree *Yroot, bitree *node, vec *fce);
          vector<vec*> search(GLfloat point[]);
          void destroyTree(bitree *node);
          bool rayplane(float nv[3], float sv[3], float dv[3], vec fce);
          double baricen(float tp1[3], float tp2[3], float tp3[3]);
          void crossProd(float* coord1,float* coord2,float* coord3 );
     public:
          worldEngine();
          worldEngine(const char filename[200]);
          void load(const char filename[200]);
          void calculateNormal(float* coord1,float* coord2,float* coord3 );
          void draw();
          bool isTouching(float x, float y, float z, float r);
          bool isTouching(float center[], float r);
          bool isTouchingRvec(float center[], float r, float *ret);
          void loc(float inX, float inY, float inZ);
          void rot(float inX, float inY, float inZ);
          ~worldEngine();
};
#endif
