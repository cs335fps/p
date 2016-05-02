// Charles Enright
// CS 335 Software Engineering
// File includes AI and elements of game mechanics for first person shooter.

#include "solidSphere.h"
#include "charlesE.h"


Mob::Mob(){
    body.redraw(3.0, 10, 10);
}

void Mob::spawn(){
    location.z = 2;
    location.x = 5;
    location.y = 5;
    body.draw(location.x, location.y, location.z);

}

void Mob::death(){


}
void Mob::damage(int health){



}

void Mob::move(){
//Put AI logic here.



}

void Mob::render(){
    this->move();
    body.draw(location.x, location.y, location.z);

}

void Enemy::move(){


}
/*
typedef struct t_global { //Gordon's code for t_global struct
        int xres, yres;
        float aspectRatio;
        Vec cameraPosition;
        Vec armRot;
        Vec baseAngle;
        Vec armAngle;
        GLfloat lightPosition[4];
        bool quadMovement;
        bool baseRotate;
        bool shadows;
        //constructor
        t_global() {
                xres=640;
                yres=480;
                aspectRatio = (GLfloat)xres / (GLfloat)yres;
                MakeVector(0.0, 5.0, 8.0, cameraPosition);
                MakeVector(1.0, 0.0, 0.0, armRot);
                MakeVector(0.0, 90.0+45.0, 0.0, baseAngle);
                MakeVector(0.0, 0.0, 0.0, armAngle);
                //light is up high, right a little, toward a little
                MakeVector(100.0f, 240.0f, 40.0f, lightPosition);
                lightPosition[3] = 1.0f;
                quadMovement = false;
                baseRotate = false;
                shadows = false;
        }
} Global;
Global g;
Vec MakeVector(x, y, z){

return new Vec(x, y, z);
}
void drawFloor() // Gordon's code to draw floor.
{
	glPushMatrix();
	glColor3f(0.6f, 0.6f, 0.5f);
	float w=5.0*0.5;
	float d=5.0*0.5;
	float h=-1.0;
	glTranslatef(0.0f, 0.0f, 1.1f);
	glBegin(GL_QUADS);
		//top
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glVertex3f( w, h,-d);
		glVertex3f(-w, h,-d);
		glVertex3f(-w, h, d);
		glVertex3f( w, h, d);
	glEnd();
	glPopMatrix();
}

void drawLightedArm() //Gordon's code to draw an arm.
{
	glPushMatrix();
	//glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(g.baseAngle[1], 0.0f, 1.0f, 0.0f);
	glColor3f(0.2f, 0.7f, 1.0f);
	box(1.0, 2.0, 1.0);
	//
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(0.6f, 1.0f, 0.0f);
	//
	glRotatef(g.armAngle[0], 1.0f, 0.0f, 0.0f);
	box(0.2, 2.2, 0.6);
	glPopMatrix();
	//
	if (g.baseRotate)
		g.baseAngle[1] += 1;
	g.armAngle[0] += 1.5;
}
void box(float w1, float h1, float d1)
{
        float w=w1*0.5;
        float d=d1*0.5; 
        float h=h1*0.5;
        //notice the normals being set
        glBegin(GL_QUADS);
                //top
                glNormal3f( 0.0f, 1.0f, 0.0f);
                glVertex3f( w, h,-d);
                glVertex3f(-w, h,-d);
                glVertex3f(-w, h, d);
                glVertex3f( w, h, d);
                // bottom
                glNormal3f( 0.0f, -1.0f, 0.0f);
                glVertex3f( w,-h, d);
                glVertex3f(-w,-h, d);
                glVertex3f(-w,-h,-d);
                glVertex3f( w,-h,-d);
                // front
                glNormal3f( 0.0f, 0.0f, 1.0f);
                glVertex3f( w, h, d);
                glVertex3f(-w, h, d);
                glVertex3f(-w,-h, d);
                glVertex3f( w,-h, d);
                // back
                glNormal3f( 0.0f, 0.0f, -1.0f);
                glVertex3f( w,-h,-d);
                glVertex3f(-w,-h,-d);
                glVertex3f(-w, h,-d);
                glVertex3f( w, h,-d);
                // left side
                glNormal3f(-1.0f, 0.0f, 0.0f);
                glVertex3f(-w, h, d);
                glVertex3f(-w, h,-d);
                glVertex3f(-w,-h,-d);
                glVertex3f(-w,-h, d);
                // Right side
                glNormal3f( 1.0f, 0.0f, 0.0f);
                glVertex3f( w, h,-d);
                glVertex3f( w, h, d);
                glVertex3f( w,-h, d);
                glVertex3f( w,-h,-d);
                glEnd();
        glEnd();
}*/
