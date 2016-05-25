#ifndef _MAP_h
#define _MAP_h
#include <vector>
class Game;
//class vector;
class Vec;
class Node
{
public:
    double cost;
    bool visited;
    bool peeked;
    int parent[2];
    char c;
    bool obstacle;
    int x;
    int z;
    Node();
};
class Map
{
private: 
    const static int size = 200;
    Node* squares[size][size];
    Node current;
public:
//    vector<Node> solution;
    Map();
    Map(Game*);
    void displayMap();
    bool inBounds(Vec);
    void getLowestCost();
    Vec* aStar(Vec start, Vec end);
    ~Map();
};




#endif
