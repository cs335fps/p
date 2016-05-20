#ifndef _MAP_h
#define _MAP_h
class Game;
class Node
{
public:
    double cost;
    bool visited;
    bool peeked;
    int parent[2];
    char c;
    bool obstacle;

    Node();
};
class Map
{
private:
    Node* squares[100][100];
    Node current;
public:
    Map();
    Map(Game*);
    void displayMap();
    bool inBounds(Vec);
    void getLowestCost();
};




#endif
