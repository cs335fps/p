// Nicholas Gardner
// April 2016 - May 2016
//
// ### Seconds class ###
// Returns # of seconds from start call as double
//
// ### Vec class ###
// 3D vector class with as many operators as I can think of
//
// ### Wall class ###
// Draws and collides with walls
//
//
#include "nickG.h"

void Bullet::render()
{
    glEnable (GL_BLEND);
    glDisable(GL_LIGHTING);
    glLineWidth(5.0);
    glColor4f(1,1,1,((float)age / 10.0));
    glBegin(GL_LINES);
    glVertex3fv(&start.x);
    glColor4f(1,1,1,0);
    glVertex3fv(&end.x);
    glEnd();
    glEnable(GL_LIGHTING);
}


// ######################## Wall class #############################
// #################################################################
Wall::Wall()
{

}

Wall::Wall(Vec a, Vec b, float w, float h, Vec col)
{
    Set(a, b, w, h, col);
}

void Wall::Set(Vec a, Vec b, float w, float h, Vec col)
{
    color = col;
    //w = w + RAND * 0.001;
    float o[][4] = { // offsets
        {1, -1, 1, 1},
        {1, 1, 1, -1},
        {-1, 1, -1, -1},
        {-1, -1, -1, 1}
    };
    v[0] = a;
    v[1] = b;
    height = h;
    width = w;
    length = (a - b).Magnitude();
    w = w / 2.0;
    Vec dif = (b - a);
    float xScale = dif.x / length;
    float zScale = dif.z / length;

    for (int i = 0; i < 4; i++) {
        c[i] = Vec(
                v[i/2].x - w * (o[i][0] * xScale + o[i][1] * zScale * 1.01),
                0,
                v[i/2].z - w * (o[i][2] * zScale + o[i][3] * xScale * 1.01)
                );
        c[i+4] = c[i] + Vec(0,height,0);
    }
}
void Wall::render()
{

    static int s[][4] = { // sides
        {0,1,2,3},
        {5,4,7,6},
        {5,1,0,4},
        {4,0,3,7},
        {7,3,2,6},
        {1,5,6,2}
    };  

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    for (int i = 0; i < 6; i++) {
        glBegin(GL_POLYGON);
        glColor3fv(&color[0]);
        //glColor3f(RAND,RAND,RAND); // PARTY MODE
        glNormal3fv(&Normal(c[s[i][2]],c[s[i][1]],c[s[i][0]])[0]);
        for (int j = 0; j < 4; j++) {

            glVertex3fv(&c[s[i][j]][0]);
        }
        glEnd();
    }
}

int Wall::Collide(Vec *pos)
{
    float pDia = 1.0; // player diameter
    Vec p = *pos;
    p.y = 0.0;
    for (int i = 0; i < 4; i++) {
    
        // corner collide
        Vec pc = c[i] - p;
        if (pc.Magnitude() < pDia / 2.0) {
            pc.Normalize();
            p = c[i] - pc * pDia / 2.0;
            pos->x = p.x;
            pos->z = p.z;
        }
    
    
        // Wall collide
        Vec AP = p - c[i];
        Vec AB = c[(i+1)%4] - c[i];
        float t = Dot(AB, AP) / Dot(AB, AB);

        if (t < 0.0 || t > 1.0)
            continue;

        // Closest point on line to point p
        Vec closestPoint = c[i] + AB * t;  

        float dist = (p - closestPoint).Magnitude();
        if (dist < pDia / 2.0) {
            // Figure out which side of the wall we are on.
            float side = -Cross(AP,AB).y;
            if (side < 0.0)
                side = -1.0;
            else
                side = 1.0;
            Vec perp(AB.z,0,-AB.x);
            perp.Normalize();
            p = closestPoint + perp * (pDia / 2.0) * side;
            pos->x = p.x;
            pos->z = p.z;
            return 1;
        }
    }
    return 0;
}

// ######################## Seconds class ##########################
// #################################################################
Seconds::Seconds()
{
    Start();
}

// Set start reference time.
void Seconds::Start()
{
    clock_gettime(CLOCK_REALTIME, &startTime);
}

// Variation on Gordons mytime from asteroids game.
// Returns the number of seconds since the class was initialized
double Seconds::Get()
{
    clock_gettime(CLOCK_REALTIME, &curTime);
    return (double)(curTime.tv_sec - startTime.tv_sec ) +
        (double)(curTime.tv_nsec - startTime.tv_nsec) / 1e9;
}

// ######################### Vec class #############################
// #################################################################
Vec::Vec()
{
    x = y = z = 0.0f;
}

Vec::Vec(float a)
{
    Vec(a, 0.0f, 0.0f);
}

Vec::Vec(float a, float b)
{
    Vec(a, b, 0.0f);
}

Vec::Vec(float a, float b, float c)
{
    x = a;
    y = b;
    z = c;
}

Vec::Vec(const Vec &v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

float& Vec::operator[] (const int index)
{
    if (index == 0) {
        return x;

    } else if (index == 1) {
        return y;

    } else if (index == 2) {
        return z;

    } else {
        int i = index % 3;
        return (*this)[i];
    }
}

Vec Vec::operator+(const Vec &v)
{
    Vec c;
    c.x = x + v.x;
    c.y = y + v.y;
    c.z = z + v.z;
    return c;
}

Vec Vec::operator-(const Vec &v)
{
    Vec c;
    c.x = x - v.x;
    c.y = y - v.y;
    c.z = z - v.z;
    return c;
}

Vec Vec::operator-()
{
    Vec c;
    c.x = -x;
    c.y = -y;
    c.z = -z;
    return c;
}

Vec Vec::operator*(const float &f)
{
    Vec c;
    c.x = x * f;
    c.y = y * f;
    c.z = z * f;
    return c;
}

Vec Vec::operator/(const float &f)
{
    Vec c;
    c.x = x / f;
    c.y = y / f;
    c.z = z / f;
    return c;
}

Vec& Vec::operator=(const Vec &vSource)
{
    x = vSource.x;
    y = vSource.y;
    z = vSource.z;
    return *this;
}

Vec operator*(float f, Vec v)
{
    Vec c;
    c.x = v.x * f;
    c.y = v.y * f;
    c.z = v.z * f;
    return c;
}

Vec operator/(float f, Vec v)
{
    Vec c;
    c.x = f / v.x;
    c.y = f / v.y;
    c.z = f / v.z;
    return c;
}

float Vec::Magnitude()
{
    return sqrt(pow(x,2)+pow(y,2)+pow(z,2));
}

float Vec::Dot(Vec b)
{
    return x * b.x + y * b.y + z * b.z;
}

Vec Vec::Cross(Vec b)
{
    Vec c;
    c.x = y * b.z - z * b.y;
    c.y = z * b.x - x * b.z;
    c.z = x * b.y - y * b.x;
    return c;
}

Vec Vec::Norm()
{
    float m = Magnitude();
    Vec c(x/m, y/m, z/m);
    return c;
}

Vec Vec::Normalize()
{
    float m = Magnitude();
    x = x/m;
    y = y/m;
    z = z/m;
    return *this;
}

void Vec::Print()
{
    std::cout << x << ", " << y << ", " << z << std::endl;
}

Vec Normal(Vec a, Vec b)
{
    return a.Cross(b).Norm();
}

Vec Normal(Vec a, Vec b, Vec c)
{
    Vec u(a-b);
    Vec v(a-c);
    return Normal(u, v);
}

float Dot(Vec a, Vec b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec Cross(Vec a, Vec b)
{
    Vec c;
    c.x = a.y * b.z - a.z * b.y;
    c.y = a.z * b.x - a.x * b.z;
    c.z = a.x * b.y - a.y * b.x;
    return c;
}




