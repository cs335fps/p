#ifndef LOADBMP_H
#define LOADBMP_H
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

class loadBMP
{
    public:
	GLuint getBMP(const char *path);
	GLuint getBMP(char *path);
};

#endif
