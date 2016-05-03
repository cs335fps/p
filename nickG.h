#ifndef _NICKG_H_
#define _NICKG_H_

#include <ctime>
#include "vec.h"

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
