#ifndef _NICKG_H_
#define _NICKG_H_

#include <ctime>
#include <GL/glx.h>
#include "vec.h"
#include <stdlib.h>
#include "charlesE.h"

#define RAND ((float)rand()/RAND_MAX)

class Seconds
{
    struct timespec startTime;
    struct timespec curTime;

    public:
    Seconds();
    void Start();
    double Get();

};


#endif
