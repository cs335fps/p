#ifndef SOLIDSPHERE_H
#define SOLIDSPHERE_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <cmath>
#include "WorldEngine.h"

using namespace std;

class solidSphere
{
     protected:
          float X,Y,Z, rad;
          float rotIndex, rotx, roty, rotz;
          vector<GLfloat> vert;
          vector<GLfloat> normal;
          vector<GLfloat> texcoords;
          vector<GLushort> indices;
          worldEngine obj;
     public:
          solidSphere()
          {
               redraw(1, 16, 32);
               obj.load("raptor.obj");
          }
          solidSphere(float radius, unsigned int rings, 
                    unsigned int sectors)
          {
               redraw(radius,rings,sectors);
               obj.load("raptor.obj");
          }
          void redraw(float radius, unsigned int rings, 
                    unsigned int sectors,bool normalFlip= false)
          {
               rad =  radius;
               int RINGS = rings, 
                   SECTORS = sectors;
               rotIndex = rotx = roty = rotz = 0;
               float const R = 1.0/(float)(rings-1);
               float const S = 1.0/(float)(sectors-1);

               vert.resize(rings * sectors * 3);
               normal.resize(rings * sectors * 3);
               texcoords.resize(rings * sectors * 2);

               vector<GLfloat>::iterator v = vert.begin();
               vector<GLfloat>::iterator n = normal.begin();
               vector<GLfloat>::iterator t = texcoords.begin();

               for (int i=0; i<RINGS; i++)
                    for (int u=0; u< SECTORS; u++){
                         float const y = sin(-M_PI_2 + M_PI 
                                   * i * R);
                         float const x = cos(2 * M_PI * u * S) 
                              * sin(M_PI * i * R);
                         float const z = sin(2 * M_PI * u * S) 
                              * sin(M_PI * i * R);
                         *t++ = u*S;
                         *t++ = i*R;

                         *v++ = x*radius;
                         *v++ = y*radius;
                         *v++ = z*radius;

                         if (normalFlip == true)
                         {
                              *n++ = -x;
                              *n++ = -y;
                              *n++ = -z;
                         }else{
                              *n++ = x;
                              *n++ = y;
                              *n++ = z;
                         }
                    }
               indices.resize(rings * sectors * 4);
               vector<GLushort>::iterator i = indices.begin();
               for (int r=0; r<RINGS-1; r++)
                    for (int s=0; s<SECTORS-1; s++){
                         *i++ = r * sectors + s;
                         *i++ = r * sectors + (s+1);
                         *i++ = (r+1) * sectors + (s+1);
                         *i++ = (r+1) * sectors + s;
                    }
          }
          void draw(GLfloat x, GLfloat y, GLfloat z)
          {
               glMatrixMode(GL_MODELVIEW);
               glPushMatrix();

               // change position
               glTranslatef(x,y,z);
               X=x;
               Y=y;
               Z=z;

               glRotatef(rotIndex, rotx, roty, rotz);
               glEnableClientState(GL_VERTEX_ARRAY);
               glEnableClientState(GL_NORMAL_ARRAY);
               glEnableClientState(GL_TEXTURE_COORD_ARRAY);

               glVertexPointer(3, GL_FLOAT, 0, &vert[0]);
               glNormalPointer(GL_FLOAT, 0, &normal[0]);
               glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
               glDrawElements(GL_QUADS, indices.size(), 
                         GL_UNSIGNED_SHORT, &indices[0]);
               glPopMatrix();
          }
///////////////////////////////////////////////////////////////////////////////
          bool isTouching(float p1, float p2, float p3)
          {
               float pt[] = {p1,p2,p3};
               return isTouching(pt);
          }
          bool isTouching(float point[])
          {
               // get the vector between the phear center 
               // and the given point
               float d1 = (point[0] - X) * (point[0] - X) +
                          (point[1] - Y) * (point[1] - Y) +
                          (point[2] - Z) * (point[2] - Z);
               float d2 = rad * rad;

               // check to see if the point is within or on the obj
               return (d1 <= d2);
          }
          void rot(float index,float x,float y,float z)
          {
               rotIndex = index;
               rotx = x;
               roty = y;
               rotz = z;
          }
          float getx()
          {
               return X;
          }
          float gety()
          {
               return Y;
          }
          float getz()
          {
               return Z;
          }
          float getRad()
          {
               return rad;
          }
          void resize(float r)
          {
               redraw(r,16,32);
          }
// ////////////////////////////////////////////////////////////////////////////
// //     //////      ///////       ///////       ///////       //////     ///
// //////////////////////////////////////////////////////////////////////////
         void drawObj(float p1, float p2, float p3)
         {
                 obj.draw();
                 obj.loc(p1,p2,p3);
                 obj.rot(rotx, roty, rotz);
         }
};

#endif
