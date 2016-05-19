// Charles Enright
// CS 335 Software Engineering
// File includes AI and elements of game mechanics for first person shooter.

#include "solidSphere.h"
#include "charlesE.h"
#define vmi vector<Mob*>::iterator
#define vwi vector<Wall>::iterator
class wall;
Mob::~Mob()
{
	
}
Mob::Mob()
{

}
Mob::Mob(int mobID, Vec* spawnpoint)
{
    this->hp = 30; 
    this->id = mobID;
    body.redraw(1.0, 10, 10);
    this->spawn(spawnpoint);
}

void Mob::spawn(Vec* spawnpoint)
{

    location.z = spawnpoint->z;
    location.x = spawnpoint->x;
    location.y = 2; // y is up and down.
    body.draw(location.x, location.y, location.z);
    //velocity.z = -0.05;
    //velocity.x = .15;
    //velocity.y = 0.05;
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
//Put AI logic here.
//
//1) Collision detection: gather local objects

//2) Collision detection: for each local object, is touching object?

//3) collision detection: if touching object, bounce
    static Vec* tmp = new Vec;
    *tmp = location+velocity;
    //if(this->Collide(tmp))
//	velocity = velocity * -1;
    location.z += velocity.z;
    location.y += velocity.y;
    location.x += velocity.x;
    
    if(velocity.z > 0.075)
       velocity.z = 0.075;	
    else if(velocity.z < -0.075)
	velocity.z = -0.075;
    if(velocity.z > 0.001)
    	velocity.z -= .0005;
    else if(velocity.z < -0.001)
	velocity.z += 0.0005;
    else
	velocity.z = 0;
    if(velocity.y > 0.075)
	velocity.y = 0.075;
    else if(velocity.y < -0.075)
	velocity.y = -0.075;
    if(velocity.y > 0.001)
    	velocity.y -= 0.0005;
    else if (velocity.y < -0.001)
	velocity.y += 0.0005;
    else
	velocity.y = 0;
    if(location.y < 2)
	velocity.y = 0;
    if(velocity.x > 0.075)
	velocity.x = 0.075;
    else if (velocity.x < -0.075)
	velocity.x = -0.075;
    if(velocity.x > 0.001)
    	velocity.x -= 0.0005;
    else if (velocity.x < -0.001)
	velocity.x += 0.0005;
    else
	velocity.x = 0;

}

void Mob::render()
{
    this->move();
    glBindTexture(GL_TEXTURE_2D, texture);
    body.draw(location.x, location.y, location.z);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int Mob::Collide(Vec* p)
{
    if(body.isTouching(p->x, p->y, p->z)){
        this->velocity = (this->location - *p);
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
void Enemy::move()
{


}

cWall::cWall(int mobID, Vec* spawnpoint):Mob(mobID, spawnpoint)
{

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
void chadKey(Game* g)
{

    static int toggle = 0;
    if(toggle == 0) {
        toggle = 1;
        //Set all mobs to color red and float straight up.
        for (
		vmi m = g->mobs.begin(); 
		m != g->mobs.end(); 
		m++
	){
                (*m)->setVelY(0.055);
    		//cout <<"now in chadkey";
        }
        for(
            vwi w = g->walls.begin(); 
            w != g->walls.end(); 
            w++ 
	){
            w->SetHeight(0.5);
        }
    }
    else {
        toggle = 0;
        for(vmi m = g->mobs.begin(); m != g->mobs.end(); m++){
            (*m)->setVelY(-0.055);
	    (*m)->setLocY((*m)->getLoc()->y - 0.05);
        }
        for(
            vwi w = g->walls.begin();
	    w != g->walls.end();
            w++
        ){
            w->SetHeight(40);
        }
        
    }
}
void respawn_mobs(Game* g, int num = 10)
{
    int max = g->mobs.size()+num;
    for(int i = g->mobs.size(); i < max; i++)
	g->mobs.push_back(new Mob(i, new Vec(r(50, -50), 2.5, r(50, -50))));
}
Game::~Game()
{
    for(Mob* i = this->mobs.back(); !this->mobs.empty(); i = this->mobs.back()){
        cout << "Killing mob " << endl;
	i->death(this);
    }
    int i = 0; 
    for(Wall w = this->walls.back(); !this->walls.empty(); w = this->walls.back()){
	i++;
	cout << "Razing wall " << i << endl;
	if(i > 999) break;
	this->walls.pop_back();
    }
    while(!this->bullets.empty()){
	cout << "Killing Bullet: " << endl;
	this->bullets.pop_back();
    }
    while(!this->bulletHoles.empty()){
        cout << "Cleaning up bullet holes.";
	this->bulletHoles.pop_back();
    }

}
View::~View(){
    delete this->game;
    delete this->dpy;

}
#include <iostream>
#include <stdio.h>
double celsiusToFahrenheit()
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
     std::cout << "Enter Celcius temp: " << endl;
     std::cin >> f;
     f = (9.0/5.0) * f + 32;
     printf("Fahrenheit: %lf\n", f);
     return f; 
}

