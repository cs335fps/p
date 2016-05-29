// Nicholas Gardner
// April 2016 - May 2016

#include "nickG.h"

// ################ General Functions ##############

Vec Reflect(Vec dir, Vec norm)
{
    return dir - (norm * 2.0 * dir.Dot(norm));

}

void PrintText(string s, float x, float y,
    float h, unsigned int img, int align)
{
    float w = h * 0.8;
    int len = s.length();
    float tWidth = w * (float)len * .9;
    float offs = 0.0;
    if (align == 1)
        offs = tWidth / 2.0;
    else if (align == 2)
        offs = tWidth;
    for (int i = 0; i < len; i++) {
        char c = s[i];
        if (c > 96 && c < 123)
            c -= 32;
        PrintChar(x + w * i * .9 - offs, y, w, h, c, img);
    }
}

void PrintChar(float x, float y,float w,float h,char c,unsigned int img)
{
    int i = c -  32;
    int sx = i % 8;
    int sy = 7 - (i / 8);
    glBindTexture(GL_TEXTURE_2D, img);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0 / 8.0 * (float)(sx + 1), 1.0 / 8.0 * (float)(sy + 1));
    glVertex2f(x+w, y+h);

    glTexCoord2f(1.0 / 8.0 * (float)(sx + 1), 1.0 / 8.0 * (float)(sy + 0));
    glVertex2f(x+w, y);

    glTexCoord2f(1.0 / 8.0 * (float)(sx + 0), 1.0 / 8.0 * (float)(sy + 0));
    glVertex2f(x, y);

    glTexCoord2f(1.0 / 8.0 * (float)(sx + 0), 1.0 / 8.0 * (float)(sy + 1));
    glVertex2f(x, y+h);

    glBindTexture(GL_TEXTURE_2D, 0);
    glEnd();
}

void DrawHealth(Game* game, int w, int h)
{
    float o = h / 140;
    float pct = (float)game->playerHP / (float)game->maxHP;
    pct *= w / 3;
    glColor4f(0,0,0,1);
    glBegin(GL_POLYGON);
    glVertex2f(w / 3, h - h / 20);
    glVertex2f(w / 3, h - h * 2 / 20);
    glVertex2f(w * 2 / 3, h - h * 2 / 20);
    glVertex2f(w * 2 / 3, h - h / 20);
    glEnd( );
    if(game->playerHP > 30)
        glColor4f(0.1,0.6,0.1,1);
    else if(game->playerHP <= 30 && game->playerHP  > 10)
        glColor4f(0.6,0.6,0.,1);
    else if(game->playerHP <= 10 && game->playerHP  > 0)
        glColor4f(0.6,0.0,0.0,1);
    else if(game->playerHP <= 0)
        return;
    glBegin(GL_POLYGON);
    glVertex2f(w / 3 + o, h - h / 20 - o);
    glVertex2f(w / 3 + o, h - h * 2 / 20 + o);
    glVertex2f(w / 3 - o + pct, h - h * 2 / 20 + o);
    glVertex2f(w / 3 - o + pct, h - h / 20 - o);
    glEnd( );
}

void AmmoBox(int x1,int y1,int x2,int y2, int o)
{
    glColor4f(.9,.9,.9,1);
    glBegin(GL_POLYGON);
    glVertex2f(x1 + o,y1 + o);
    glVertex2f(x1 + o,y2);
    glVertex2f(x2 - o,y2);
    glVertex2f(x2 - o,y1 + o);
    glEnd( );
}

void DrawAmmo(Game* game, int w, int h)
{
    int o = h / 140;
    glColor4f(0,0,0,1);
    glBegin(GL_POLYGON);
    glVertex2f(w * 38 / 40, h * 3 / 4 + o);
    glVertex2f(w * 38 / 40, h / 4);
    glVertex2f(w * 39 / 40, h / 4);
    glVertex2f(w * 39 / 40, h * 3 / 4 + o);
    glEnd( );
    if (game->setReloadDelay > 0)
        return;
    int x1 = w * 38 / 40;
    int x2 = w * 39 / 40;
    int y = h / 4;
    int yh = h / 2;
    int max = game->maxbullets;
    for (int i = 0; i < game->nbullets; i++) {
        AmmoBox(x1, y + yh * i / max,x2, y + yh * (i+1) / max, o);
    }
}

int hash(int *in, int n)
{
    int seed = 0;//533000389;
    for (int i = 0; i < n; i++) {
        seed ^= in[i];
        for (int j = 0; j < 8; j++) {
            if (seed & 1)
                seed = (seed >> 1) ^ 0xedb88320;
            else
                seed = seed >> 1;
        }
    }
    return seed;
}

int RaySphere(Vec rayOrigin, Vec rayDirection, 
        Vec sphereOrigin, float sphereRadius, float *closest)
{
    rayDirection.Normalize();
    float a = rayDirection.Dot(rayDirection);
    Vec oDiff = rayOrigin - sphereOrigin;
    float b = (rayDirection.Dot(oDiff));
    float c = oDiff.Dot(oDiff) - sphereRadius * sphereRadius;
    float s = b * b - a * c;
    if (s < 0.0)
        return 0;
    float d1 = (-1.0 * b + sqrt(s)) / a;
    float d2 = (-1.0 * b - sqrt(s)) / a;

    if (d2 < d1 && d2 > 0.0)
        d1 = d2;
    if (d1 < 0.0)
        return 0;

    if (d1 > *closest)
        return 0;

    *closest = d1;

    return 1;
}

void DrawCrosshairs(Game *game, int w, int h)
{
    int l = h / 1;

    int hitAnim = game->hitAnim;
    if (hitAnim > 0) {
        glPushMatrix();
        glTranslatef(w/2,h/2,0);
        glColor4f(1,0,0,((float)hitAnim / 20.0));
        glBegin(GL_LINE);
        GLUquadric* qobj = gluNewQuadric();
        gluQuadricOrientation(qobj,GLU_INSIDE);
        gluDisk(qobj, (float)l/20.0*.75, l/20, 32, 1);
        glEnd(); 
        glPopMatrix();
    }
    glLineWidth(1);
    glBegin(GL_LINES);
    glColor3f(1.0f,1.0f,1.0f);
    glVertex2d(w / 2 - l / 20, h / 2);
    glVertex2d(w / 2 + l / 20, h / 2);
    glEnd();
    glBegin(GL_LINES);
    glVertex2d(w / 2, h / 2 - l / 20);
    glVertex2d(w / 2, h / 2 + l / 20);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2d(w / 2 - l / 20, h / 2+1);
    glVertex2d(w / 2 + l / 20, h / 2+1);
    glEnd();
    glBegin(GL_LINES);
    glVertex2d(w / 2+1, h / 2 - l / 20);
    glVertex2d(w / 2+1, h / 2 + l / 20);
    glEnd();
}

void ParseLevel(const char* fileName, Game* game,  float height)
{
    vector<string> lines;
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "file not found.\n";
        return;
    }
    string line;
    string cmp;
    string spawnPt = "#FF00FF";
    while (getline(file, line)) {
        lines.push_back(line);
    }
    game->walls.clear();
    game->spawnPts.clear();
    int lCount = lines.size();
    for (int i = 0; i < lCount; i++) {
        line = lines[i];
        cmp = "<line";
        if (line.compare(0, cmp.length(), cmp) == 0) {
            vector<string> elems = Split(line, "\"");
            if (elems[9].compare(spawnPt) == 0) {
                //Spawn point
                game->spawnPts.push_back(Vec(
                            atof(elems[1].c_str()), 2, 
                            atof(elems[3].c_str())
                            ));
            } else {
                game->walls.push_back(Wall(Vec(
                                atof(elems[1].c_str()), 0, 
                                atof(elems[3].c_str())
                                ),Vec(
                                    atof(elems[5].c_str()), 0, 
                                    atof(elems[7].c_str())
                                    ),0.1, height, Vec(1,1,1)));
            }
        }
    }
}

vector<string> Split(string s, string del)
{
    vector<string> ret;
    size_t lastPos = 0;
    size_t nextPos = 0;

    nextPos = s.find(del, lastPos);

    while (nextPos != string::npos) {
        ret.push_back(s.substr(lastPos,nextPos - lastPos));
        lastPos = nextPos + del.length();
        nextPos = s.find(del, lastPos);
    }

    ret.push_back(s.substr(lastPos,s.length() - lastPos));

    return ret;
}

// ################ Bullet Class ###################
void Bullet::render()
{
    glEnable (GL_BLEND);
    glDisable(GL_LIGHTING);
    glLineWidth(5.0);
    glColor4f(1,1,1,((float)age / 10.0));
    glBegin(GL_LINES);
    glVertex3fv(&origin.x);
    glColor4f(1,1,1,0);
    glVertex3fv(&end.x);
    glEnd();
    glEnable(GL_LIGHTING);
}

BulletHole::BulletHole(Vec o, Vec n)
{
    origin = o;
    normal = n;
}

void BulletHole::render()
{
    glDisable(GL_LIGHTING);
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 0.0);
    float circMax = 8;
    Vec x;
    float rad = 0.05;
    float th;
    for (float i = 0.0; i < circMax; i+=1.0) {
        th = i / circMax * 6.2831;
        x = (origin + Vec(
                    -normal.z * cos(th) * rad,
                    sin(th) * rad,
                    normal.x * cos(th) * rad
                    ));
        glVertex3fv(&x.x);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void BulletHole::render2()
{
    float arr[11][2] = {
        {-4,4},
        {-3,3},
        {-3,1},
        {-1,2},
        {-1,0},
        {0,2},
        {1,0},
        {1,2},
        {3,1},
        {3,3},
        {4,4},
    };
    glDisable(GL_LIGHTING);
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 0.0);
    float circMax = 16;
    Vec x;
    float rad = 0.05;
    float th;
    Vec perp = Vec(-normal.z,normal.y,normal.x);
    for (float i = 0.0; i < circMax; i+=1.0) {
        th = i / circMax * 6.2831;
        x = (origin + Vec(
                    -normal.z * cos(th) * rad,
                    sin(th) * rad * 2.0,
                    normal.x * cos(th) * rad
                    ));
        x = x - perp * 0.1;
        glVertex3fv(&x.x);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (float i = 0.0; i < circMax; i+=1.0) {
        th = i / circMax * 6.2831;
        x = (origin + Vec(
                    -normal.z * cos(th) * rad,
                    sin(th) * rad * 2.0,
                    normal.x * cos(th) * rad
                    ));
        x = x + perp * 0.1;
        glVertex3fv(&x.x);
    }
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
    for (float i = 0.0; i < 11.0; i+=1.0) {
        x = origin + Vec(perp.x * 0.05 * arr[(int)i][0],
                arr[(int)i][1] * 0.05 - 0.3,
                perp.z * 0.05 * arr[(int)i][0]);
        glVertex3fv(&x.x);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

// ################ Wall class #####################
Wall::Wall()
{

}

Wall::Wall(Vec a, Vec b, float w, float h, Vec col)
{
    Set(a, b, w, h, col);
}

vector<Vec> Wall::GetPoints(double units)
{
    vector<Vec> points;
    int n = ceil(length / units);
    Vec dir = (v[1] - v[0]) / (float)n;
    for (float i = 0; i <= n; i+=1.0) {
        points.push_back(v[0] + dir * i);
    }
    return points;
}

int Wall::Ray(Vec origin, Vec direction, float * closest, Vec * normal)
{

    direction.Normalize();
    Vec n = Normal(v[0],v[0]+Vec(0,1,0),v[1]);
    float dirDotNormal = direction.Dot(n);
    if (dirDotNormal == 0.0)
        return 0; // Parallel
    float t = -1.0 * (origin - v[0]).Dot(n) / dirDotNormal;
    if (t < 0.0 || t > *closest) {
        return 0; // Behind us
    }

    Vec location = origin + (direction * t);
    if (location.y < 0 || location.y > height)
        return 0; // Over or under wall

    location.y = 0;
    if ((location - v[0]).Magnitude() > length ||
            (location - v[1]).Magnitude() > length)
        return 0; // Too far left or right


    *closest = t - (width/2.0) * (1.0/fabs(dirDotNormal));
    if (normal != NULL) {
        *normal = n;
    }
    return 1;
}

void Wall::Set(Vec a, Vec b, float w, float h, Vec col)
{

    int nums[] = {int(a.x * 10),int(a.z * 10),int(b.x * 10),int(b.z * 10)};
    int ha = hash(nums, 4);
    ha = ha >> 8 & 3;
    float colf = ((float)ha / 3 * 0.4) + .1;
    color = Vec(colf, colf, colf);
    //color = col;
    float o[][4] = { // offsets
        {1, -1, 1, 1},
        {1, 1, 1, -1},
        {-1, 1, -1, -1},
        {-1, -1, -1, 1} };
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

void Wall::SetHeight(float h)
{
    height = h;
    for (int i = 4; i < 8; i++) {
        c[i].y = h;
    }

}

void Wall::render(unsigned int wallTex)
{
    static float t[4][2] = {
        {1.0,1.0},
        {1.0,0.0},
        {0.0,0.0},
        {0.0,1.0}};
    static int s[][4] = { // sides
        {0,1,2,3},
        {5,4,7,6},
        {5,1,0,4},
        {7,3,2,6},
        {4,0,3,7},
        {6,2,1,5} };  
    static float co = 0.0;
    co += 0.001;
    if (co > 360.0)
        co -= 1.0;

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    for (int i = 0; i < 4; i++) {
        glBegin(GL_POLYGON);

        if (game->partyMode)
            glColor3f(sin(co)/2.0+0.5,
                    sin(co+120.0)/2.0+0.5,
                    sin(co+240.0)/2.0+0.5); // PARTY MODE
        else
            glColor3fv(&color[0]);

        glNormal3fv(&Normal(c[s[i][2]],c[s[i][1]],c[s[i][0]])[0]);
        for (int j = 0; j < 4; j++) {
            glVertex3fv(&c[s[i][j]][0]);
        }
        glEnd();
    }
   glBindTexture(GL_TEXTURE_2D, wallTex);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    for (int i = 4; i < 6; i++) {
        glBegin(GL_POLYGON);
        for (int j = 0; j < 4; j++) {
            glTexCoord2f(t[j][0] * length / 4.0,t[j][1] * height / 4.0);
            glVertex3fv(&c[s[i][j]][0]);
        }
        glEnd();
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_COLOR_MATERIAL);
}

int Wall::Collide(Vec* pos, float radius, Vec* normal = NULL)
{
    int col = 0;
    Vec p = *pos;
    p.y = 0.0;
    for (int i = 0; i < 4; i++) {

        // corner collide
        Vec pc = c[i] - p;
        if (pc.Magnitude() < radius / 2.0) {
            pc.Normalize();
            p = c[i] - pc * radius / 2.0;
            pos->x = p.x;
            pos->z = p.z;
            if (normal != NULL)
                *normal = pc;
            col = 1;
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
        if (dist < radius / 2.0) {
            // Figure out which side of the wall we are on.
            float side = -Cross(AP,AB).y;
            if (side < 0.0)
                side = -1.0;
            else
                side = 1.0;
            Vec perp(AB.z,0,-AB.x);
            perp.Normalize();
            p = closestPoint + perp * (radius / 2.0) * side;
            pos->x = p.x;
            pos->z = p.z;

            if (normal != NULL)
                *normal = perp;

            return 1;
        }
    }
    return col;
}

int Wall::Collide(Vec *pos)
{
    float pDia = 1.0; // player diameter
    return Collide(pos, pDia);
}

// ################ Seconds class ##################
Seconds::Seconds()
{
    Start();
}

void Seconds::Start()
{
    clock_gettime(CLOCK_REALTIME, &startTime);
}

// Variation on Gordons mytime from asteroids game.
double Seconds::Get()
{
    clock_gettime(CLOCK_REALTIME, &curTime);
    return (double)(curTime.tv_sec - startTime.tv_sec ) +
        (double)(curTime.tv_nsec - startTime.tv_nsec) / 1e9;
}

// ################# Vec class #####################
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




