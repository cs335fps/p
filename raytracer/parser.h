#ifndef _PARSER_H_
#define _PARSER_H_

#include "scene.h"
#include "object.h"
#include "vector.h"
#include <sstream>

class Parser {
    
public:
    
    void parse(Scene *sc, char* fileName);
    void parseOBJ(Scene *sc, const char* fileName);
    
};

#endif