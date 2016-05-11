// Charles Enright
// CS 335 Software Engineering
// File includes AI and elements of game mechanics for first person shooter.

#include "solidSphere.h"
#include "charlesE.h"


Mob::Mob(){
    body.redraw(2.0, 10, 10);
    this->spawn();
}

void Mob::spawn(){
    location.z = 2;
    location.x = 2;
    location.y = 2; // y is up and down.
    body.draw(location.x, location.y, location.z);
    //velocity.z = -0.05;
    //velocity.x = .15;
    //velocity.y = 0.05;
}

void Mob::death(){
    delete this;	

}
void Mob::damage(int health){

    this->hp -= health;
    if(this->hp < 0)
	this->death();
}

void Mob::move(){

}

void Mob::move(Game g){
//Put AI logic here.
//
//1) Collision detection: gather local objects
//2) Collision detection: for each local object, is touching object?
//3) collision detection: if touching object, bounce
    location.z += velocity.z;
    location.y += velocity.y;
    location.x += velocity.x;
}

void Mob::render(){
    this->move();
    body.draw(location.x, location.y, location.z);

}

void Enemy::move(){


}
cWall::cWall()
{

}
cWall::cWall(Vec a, Vec b, float w, float h)
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
	float zScale = dif.z / length; // Z is the second flat axis; y is the vertical axis.
	
	for (int i = 0; i < 4; i++)
    	{
            this->corners[i] = Vec(
		endpoints[i / 2].x - w * (offsets[i][0] * xScale + offsets[i][1] * zScale),
		0,
		endpoints[i / 2].z - w * (offsets[i][2] * zScale + offsets[i][3] * xScale)
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
{ // TODO: Ask nick to comment his variables.
    Vec position = *pos;
    position.y = 0.0; // this is our vertical element
    Vec AP = position - this->endpoints[0];
    Vec AB = this->endpoints[1] - this->endpoints[0];
    float t = Dot(AB, AP) / Dot(AB, AB);
    
    if(t < 0 || t > 1)
	return 0;

    Vec closestPoint = this->endpoints[0] + AB * t;

    if ( ( position - closestPoint).Magnitude() < 1.0 + this->width / 2.0){
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





