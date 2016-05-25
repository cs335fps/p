// Charles Enright
// CS 335 Software Engineering
// File includes AI and elements of game mechanics for first person shooter.

#include "solidSphere.h"
#include "charlesE.h"
#define vmi vector<Mob*>::iterator
#define vwi vector<Wall>::iterator
class Wall;
class Map;
Mob::~Mob()
{
    //delete this->map2d;
}
Mob::Mob()
{
    this->hp = 0;
}
Mob::Mob(int mobID, Vec* spawnpoint)
{
    this->hp = 30; 
    this->id = mobID;
    body.redraw(1.0, 10, 10);
    this->spawn(spawnpoint);
    this->texture = 0;
    this->moved = (int)(r(0,120));
    // Must wait until we have walls to draw map.
    // Walls are in the game object.
    this->hasMap = false;
}

void Mob::spawn(Vec* spawnpoint)
{

    location.z = spawnpoint->z;
    location.x = spawnpoint->x;
    location.y = 2; // y is up and down.

    //body.draw(location.x, location.y, location.z);
    body.drawObj(location.x, location.y, location.z);
    velocity.z = r(-0.05, 0.05);
    velocity.x = r(-0.05, 0.05);
    velocity.y = r(-0.05, 0.05);
}

void Mob::death(Game* g)
{
    int i = 0;
    for(vmi m = g->mobs.begin(); m != g->mobs.end(); i++, m++){
       if((**m) == this->id){
            //causes an undefined behavior warning.
            Mob* temp = *m; 
            g->mobs.erase(m);
            delete temp;
            return; 
        }
        i++;
    }

    //throw -1;
}
void Mob::damage(int health, Game* g)
{
    this->hp -= health;
    if(this->hp <= 0)
        this->death(g); 
}

void Mob::move()
{
    return;
    this->move(NULL);
}

bool Mob::operator==(int b)
{
    return this->id == b;
}

bool Mob::operator==(Mob* b)
{
    return this->id == b->id;
}

void Mob::move(Game* g)
{
    location.z += (float) velocity.z / 240.0;
    location.y += velocity.y / 240.0;
    location.x += velocity.x / 240.0;


    if (this->moved > 120) { 
        this->moved = 0;
        /*if(!this->hasMap){
          this->map2d = new Map(g);
          this->hasMap = true;
          }*/
    }
    else{
        this->moved++;
        return;
    }
    //Put AI logic here.
    //
    //1) Collision detection: gather local objects
    // This is done in Map::Map(); it prevents overhead from gathering
    // new local objects each time.
    //2) Collision detection: for each local object, is touching object?
    // Done in Game::move where each mob iterates over each wall.
    //3) collision detection: if touching object, bounce
    // Also done in Wall:Collision.
    static Vec* tmp = new Vec(0,0,0);
    //*tmp = location+velocity;
    //if(this->Collide(tmp) == 1){
    //}
    //4) Map new route to player and start traversing route.
    if(hasMap)
        tmp = this->map2d->aStar(this->location, g->position);
    //check if no solution; if so, jump and teleport.
    if(!hasMap || (tmp->x == 0 && tmp->y == 0 && tmp->z == 0)){
        tmp->x = g->position.x - this->location.x;
        this->location.y = 2.0;
        this->velocity.y = 0.5;
        tmp->z =  g->position.x - this->location.z;
        this->location.x = this->location.x + (tmp->x / 3.0);
        this->location.z = this->location.z + (tmp->z / 3.0);
    }
    /*else{
        this->velocity.x = 3 * tmp->x;//should be about 3-5.
        this->velocity.z = 3 * tmp->z;
    }*/
    if(velocity.z > 24.075)
        velocity.z = 24.075;	
    else if(velocity.z < -24.075)
        velocity.z = -24.075;
    if(velocity.z > 0.1305)
        velocity.z -= .0112;
    else if(velocity.z < -0.1305)
        velocity.z += 0.1102;
    else
        velocity.z = 0;
    if(velocity.y > 26.075)
        velocity.y = 26.075;
    else if(velocity.y < -26.075)
        velocity.y = -26.075;
    if(location.y > 2) // gravity.
        velocity.y -= 0.6102;
    else if (location.y < 2){
        velocity.y = 0.0;
        location.y = 2;
    }
    else
        velocity.y = 0;
    if(location.y < 2)
        velocity.y = 0;
    if(velocity.x > 24.075)
        velocity.x = 24.075;
    else if (velocity.x < -24.075)
        velocity.x = -24.075;
    if(velocity.x > 0.061)
        velocity.x -= 0.0035;
    else if (velocity.x < -0.061)
        velocity.x += .035;
    else
        velocity.x = 0;
    if(velocity.x == 0 && velocity.y == 0 && velocity.z == 0){
        velocity.x = r(-13.05, 13.05);
        velocity.z = r(-13.05, 13.05);
    }

}

void Mob::render()
{
    //cout << "Now rendering mob " << this->id;
    //this->move(g);
    glBindTexture(GL_TEXTURE_2D, texture);
    body.draw(location.x, location.y, location.z);
    glBindTexture(GL_TEXTURE_2D, 0);
}
/*
   void Enemy::render()
   {
   this->move();
   glBindTexture(GL_TEXTURE_2D, texture);
   body.drawObj(location.x, location.y, location.z);
   glBindTexture(GL_TEXTURE_2D, 0);
   }
   */
int Mob::Collide(Vec* p)
{
    if(body.isTouching(p->x, p->y, p->z)){
        Vec distance;
        distance.x = location.x - p->x;
        distance.y = location.y - p->y;
        distance.z = location.z - p->z;
        location.z -= distance.z;
        location.y -= distance.y;
        location.x -= distance.x;
        velocity.z *= -1*distance.z;
        velocity.y *= -1*distance.y;
        velocity.x *= -1*distance.x;
        return 1;
        //cout << "Object touching" << " x " << this->velocity.x << " y " 
        //<< this->velocity.y 
        //    << " z " << this->velocity.z<<endl;
    }
    return 0;
}

float r(float min, float max)
{
    static float r_m = static_cast<float> (RAND_MAX);
    return (
            static_cast<float> (rand()) / (r_m / (max-min)) + min
           );
}
/*
   void Enemy::move()
   {
//Never got implemented!
}
*/
cWall::cWall(int mobID, Vec* spawnpoint):Mob(mobID, spawnpoint)
{
    //We're not using cWalls because...we're just not using them.
}

cWall::cWall(Vec a, Vec b, float w, float h):Mob()
{
    Set(a, b, w, h);
}

void cWall::Set(Vec a, Vec b, float w = 2, float h = 2)
{
    float offsets[][4] = {
        {1, -1, 1, 1},
        {1, 1, 1, -1},
        {-1, 1, -1, -1},
        {-1, -1, -1, 1}
    };
    this->endpoints[0] = a;
    this->endpoints[1] = b;
    this->height = h;
    this->width = w;
    this->color.x = 0.1;
    this->color.y = 0.1;
    this->color.z = 0.2;	
    float length = (a - b).Magnitude();
    w = w / 2.0;
    Vec dif = (b - a);
    float xScale = dif.x / length;
    float zScale = dif.z / length; 
    // Z is the second flat axis; y is the vertical axis.

    for (int i = 0; i < 4; i++)
    {
        this->corners[i] = Vec(
                endpoints[i / 2].x - w * (
                    offsets[i][0] * xScale + offsets[i][1] * zScale
                    ),
                0,
                endpoints[i / 2].z - w * (
                    offsets[i][2] * zScale + offsets[i][3] * xScale
                    )
                );
        corners[i+4] = corners[i] + Vec(0, height, 0);	    
    }
}

void cWall::Draw()
{
    int sides[][4] = {
        {0, 1, 2, 3},
        {5, 4, 7, 6},
        {5, 1, 0, 4},
        {4, 0, 3, 7},
        {7, 3, 2, 6},
        {1, 5, 6, 2}
    };

    glColor3f(color.x, color.y, color.z);
    for(int i = 0; i < 6; i++){
        glBegin(GL_POLYGON);
        glNormal3fv(&Normal(
                    this->corners[ sides[i][2] ], 
                    this->corners[ sides[i][1] ], 
                    this->corners[ sides[i][1] ])[0] 
                );
        for(int j = 0; j < 4; j++){
            glVertex3fv(&corners[sides[i][j]][0]);
        }
        glEnd();
    }
}

int cWall::Collide(Vec * pos, float diameter = 0)
{ 
    Vec position = *pos;
    position.y = 0.0; // this is our vertical element
    Vec AP = position - this->endpoints[0];
    Vec AB = this->endpoints[1] - this->endpoints[0];
    float t = Dot(AB, AP) / Dot(AB, AB);

    if(t < 0 || t > 1)
        return 0;

    Vec closestPoint = this->endpoints[0] + AB * t;

    if ( ( position - closestPoint).Magnitude() < 1.0 + this->width / 2.0) {
        float side = -Cross(AP, AB).y;
        if (side < 0.0) 
            side = -1.0;
        else
            side = 1;
        Vec perp(AB.z, 0, -AB.x);
        perp.Normalize();
        position = closestPoint + perp * (1 + width / 2.0) * side;
        pos->x = position.x;
        pos->z = position.z;
        return 1;
    }
    return 0;
}

void Mob::setLocY(float y) 
{
    this->location.y = y;
}

void Mob::setVelY(float y) 
{
    this->velocity.y = y;
} 

double celsiusToFahrenheit(double celsius)
{
    static double f;
    //#define _L8T_
#ifdef _L8T_
    static double test[] = {
        180, 356, 
        100, 212,
        40,  104,
        37,   98,
        30,   86,
        21,   70,
        10,   50,
        0,    32,
        -18,   0,
        -40, -40
    };
    printf("%lf\n", test[2*4]);
    printf("%lf\n", test[2*2+1]);
    printf("%lf\n", test[1*2+1]);
#endif
    /*std::cout << "Enter Celcius temp: " << endl;
      std::cin >> f;*/

    f = (9.0/5.0) * celsius + 32;
    printf("Fahrenheit: %lf\n", f);
    return f; 
}
double fahrToCels(double fahr){
    static double c;
    c = (5.0/9.0) * (fahr - 32);
    printf("Centigrade: %lf\n", c);
    return c;
}
void startAstar(Game* g)
{
    static int toggle = 0;
    //static float wallHeight = 0.5;
    if(toggle == 0) {
        toggle = 1;
        //respawn_mobs(g, 10);
        //Set all mobs to color red and float straight up.
        for (
                vmi m = g->mobs.begin(); 
                m != g->mobs.end(); 
                m++
            ){
            (*m)->setVelY(4.055);
            if(!(*m)->hasMap){	
                //(*m)->map2d = new Map(g);
                //(*m)->hasMap = true;
            }
            //cout <<"now in chadkey";

        }
        g->mobs[0]->map2d->displayMap();
        g->mobs[0]->map2d->aStar(
                *(g->mobs[0]->getLoc()), g->position
                );
    }
    else {
        toggle = 0;
        for(vmi m = g->mobs.begin(); m != g->mobs.end(); m++){
            (*m)->setVelY(-4.055);
            (*m)->setLocY((*m)->getLoc()->y - 0.05);
            (*m)->hasMap = false;
        }
    }
}
void chadKey(Game* g, View* v)
{
 static int toggle = 0;
    //static float wallHeight = 0.5;
    if(toggle == 0) {
        toggle = 1;
        respawn_mobs(g, 10);
        //Set all mobs to color red and float straight up.
        for (
                vmi m = g->mobs.begin(); 
                m != g->mobs.end(); 
                m++
            ){
            (*m)->setVelY(4.055);
            if(!(*m)->hasMap){	
                //(*m)->map2d = new Map(g);
                //(*m)->hasMap = true;
            }
            //cout <<"now in chadkey";
        }
        for(
                vwi w = g->walls.begin(); 
                w != g->walls.end(); 
                w++ 
           ){
            w->SetHeight(0.5);
        }
        g->temperature = celsiusToFahrenheit(g->temperature);
    }
    else {
        toggle = 0;
        for(vmi m = g->mobs.begin(); m != g->mobs.end(); m++){
            (*m)->setVelY(-4.055);
            (*m)->setLocY((*m)->getLoc()->y - 0.05);
         //   (*m)->hasMap = false;
        }
        for(
                vwi w = g->walls.begin();
                w != g->walls.end();
                w++
           ){
            w->SetHeight(3);
        }
        g->temperature = fahrToCels(g->temperature);	
    }


   }

void respawn_mobs(Game* g, int num = 10)
{
    int s = g->mobs.size();
    for(int i = 0; i < num; i++)
        g->mobs.push_back(new Mob(s+i, new Vec(r(-20, 20), 2, r(-20, 20))));
}

Game::~Game()
{
    /*for(Mob* i = this->mobs.back(); !this->mobs.empty(); i = this->mobs.back()){
        // cout << "Killing mob " << endl;
        i->death(this);
    }*/
    int m = this->mobs.size();
    for(int i = 0; i < m; i++){
        this->mobs[i]->death(this);
    }
    for(int i = 0; !this->walls.empty();this->walls.pop_back()){
        i++;
        // cout << "Razing wall " << i << endl;
        if(i > 999) break;

        this->walls.pop_back();
    }
    /*while(!this->bullets.empty()){
        cout << "Killing Bullet: " << endl;
	delete &this->bullets.back();
        this->bullets.pop_back();
    }
    while(!this->bulletHoles.empty()){
        cout << "Cleaning up bullet holes.";
        this->bulletHoles.pop_back();
    }*/
    // tmpPos seems to be automatic allocation/garbage collection.
    //delete[] tmpPos;
}

View::~View()
{
    delete this->game; // pointer to game object.
    // delete this->dpy; Says it won't delete. How do we free this memory?

}
#include <iostream>
#include <stdio.h>
Node::Node()
{
    this->visited = false;
    this->cost = 2047;
    this->peeked = false;
    parent[0] = 0;
    parent[1] = 0;
    c = '.';
    obstacle = false;
}

Map::Map(Game* g)
{
    for(int i = 0; i < 200; i++)
        for(int j = 0; j < 200; j++){
            squares[i][j] = new Node();
            squares[i][j]->x = i;
            squares[i][j]->z = j;
        }
    static vector<Vec> vv;
    for(vwi w = g->walls.begin(); w != g->walls.end(); w++){
        vv = w->GetPoints(); 
        for(
                vector<Vec>::iterator vvi = vv.begin();
                vvi != vv.end(); 
                vvi++
           ){
            squares[(int) vvi->x + 100][(int) vvi->z + 100]->obstacle = true;
            //cout << "Obstacle: " 
            //     << (int) vvi->x << " " << (int) vvi->z << endl;
        }
    }
}

bool Map::inBounds(Vec v)
{
    return(v.x >= -75 && v.x <= 75 && v.z <= 75 && v.z >= -75);
}

void Map::getLowestCost(){
    double lowCost = 9e6;
    for (int i = 45; i < 155; i++){
        for(int j = 45; j < 155; j++){
            if(//!this->squares[i][j]->obstacle&&
                    !this->squares[i][j]->visited && 
                    this->squares[i][j]->cost < lowCost
              ) {
                lowCost = squares[i][j]->cost;
                this->current.x = i;
                this->current.z = j;
            }
        }
    }
}

Vec* Map::aStar(Vec start, Vec end)
{
    // Dijkstra's derived from my CS 312 Lab 6. -- Charles Enright
    // May contain code derived from Gordon.
    double root2 = sqrt(2);
    Vec nextPath;
    squares[int(start.x + 100)][(int)(start.z + 100)]->cost = 0.0;
    start.x = (int) start.x;
    start.z = (int) start.z;
    end.x = (int) start.x;
    end.z = (int) start.z;
    int offset[8][2] = {
        {-1, 0}, {1, 0},
        {0, -1}, {0, 1},
        {-1, -1}, {-1, 1},
        {1, -1}, {1, 1}
    };
    int sentinal = 200;
    int iter = 0;
    do {
        if(iter > sentinal){
            cout << "Hit sentinel. Aborting aStar.";
            break;
        }
        else{
            iter++;

        }
        int x, y;
        getLowestCost();      
        for (int i = 0; i < 8; i++) {	
            x = this->current.x;
            y = this->current.z;

            if (inBounds(Vec(x, 0, y)) && 
                    !squares[x][y]->visited
               ) {
                double cost;
                squares[x][y]->peeked = true;

                //if diagonal, cost is root 2; else cost = 1;
                if(offset[i][0] == offset[i][1]
                        || offset[i][0] == -1 * offset[i][1]
                  ){
                    cost = squares[x][y]->cost + root2;
                } 
                else {
                    cost = squares[x][y]->cost + 1.0;
                }
                double d0, d1, dist;
                d0 = (double) x - end.x;
                d1 = (double) y - end.z;
                dist = d0*d0+d1*d1;
                cost += dist;
                if (squares[x][y]->cost > cost) {
                    squares[x][y]->cost = cost;
                    squares[x][y]->parent[0] = x;
                    squares[x][y]->parent[1] = y;
                    squares[x][y]->c = '*';
                } 
            }
        }
        squares[x][y]->visited = true;
        usleep(10);
    } while (this->current.x != end.x && this->current.z != end.z);
    //find next node.
    while(this->current.parent[0] != 0 && this->current.parent[1] != 0) {
        if(current.parent[0] == start.x && current.parent[1] == start.z)
            return new Vec(start.x - current.x, 0, start.z - current.z);
        current = *squares[current.parent[0]][current.parent[1]];
    }
    //this->displayMap();
    return new Vec(0, 0, 0);
}
void Map::displayMap()
{
    for(int i = 35; i < 155; i++){
        for(int j = 45; j < 165; j++){
            if(this->squares[i][j]->obstacle){
                if(this->squares[i][j]->peeked)
                    cout << "\033[1;34m#\033[0m";
                else
                    cout << "\033[1;33m#\033[0m";
            }
            else if (this->squares[i][j]->visited) {
                cout << "\033[1;";
                if(this->squares[i][j]->c != '\0')
                    cout << "31m" << this->squares[i][j]->c;
                else
                    cout << "o";
                cout << "\033[0m";
            }
            else if(this->squares[i][j]->peeked)
                cout << "\033[1;34mo";
            else
                cout << ".";
            cout << "\033[0m";
        }
        cout << endl;
    }
}
Map::~Map()
{
    //automatic allocate/deallocate
    //delete[] squares;
}
Wall::~Wall()
{
//    delete[8] c;
//    delete[2] v;
}


