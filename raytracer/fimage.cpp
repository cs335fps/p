#include "fimage.h"
#include <sstream>
using namespace std;

fimage::fimage(){};

int fimage::LoadFile(const char * name) {
    width = 0;
    height = 0;
    strcpy(fileName,name);
    ifstream file(name, ios_base::binary);
    if (file.is_open()){
        string s;
        file >> s;
        printf("%s\n",s.c_str());
        if (s != "P6") {
            return 2;
        }
        while (width == 0) {
            getline(file,s);
            if (s[0] == '#')
                continue;
            istringstream c(s);
            c >> width >> height;
        }
        file >> depth;
        printf("%i %i %f\n",width,height,depth);
        count = width * height * 3;
        int c;
        c = file.get();
        data = new float[count];
        
        for (int i = 0; i < count; i++) {
            c = file.get();
            data[i] = (float)c / depth;
        }
        depth++;
        return 0;
    } else {
        return 1;
    }
}

fimage::fimage(const char * name) {
    LoadFile(name);
}
fimage::fimage(int w, int h, int d) {
    width = w;
    height = h;
    count = w * h * 3;
    depth = (float)d;
    data = new float[count];
}
fimage::fimage(fimage *im) {
    width = im->width;
    height = im->height;
    count = im->count;
    depth = im->depth;
    data = new float[count];
    
    
    for (int i = 0; i < count; i++) {
        data[i] = im->data[i];
    }
}

void fimage::setRes(int w, int h, int d) {
    width = w;
    height = h;
    count = w * h * 3;
    depth = (float)d;
    if (data == 0)
        delete[] data;
    
    data = new float[count];
}

void fimage::setPixel(int x, int y,float r,float g,float b) {
    int spot = (y * width + x) * 3;
    if (spot >= count)
        return;
    data[spot + 0] = r;
    data[spot + 1] = g;
    data[spot + 2] = b;
}

Vec fimage::getPixel(float x, float y) {
    int spot = ((int)y * width + (int)x) * 3;
    if (spot >= count)
        return Vec(1.0,0.5,1.0);
    return Vec(data[spot + 0],data[spot + 1],data[spot + 2]);
}

void fimage::setPixel(int x, int y, Vec c) {
    int spot = (y * width + x) * 3;
    if (spot >= count)
        return;
    data[spot + 0] = c[0];
    data[spot + 1] = c[1];
    data[spot + 2] = c[2];
}

bool fimage::validCoord(int x, int y) {
    return (x >=0 && x < width && y >= 0 && y < height); 
}

void fimage::save() {
    unsigned char saveData[count];
    ofstream file;
    file.open(fileName);
    charFromFloat(this, saveData, depth);
    file << "P6\n" << width << " " << height << "\n" << (int)floor(depth) - 1 << "\n";
    for (int i = 0; i < count; i++) {
        file << saveData[i];
    }
    file.close();
}
void fimage::testImage() {
    float r,g,b;
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            r = (float)x / width;
            g = sin(sqrt((x*x+y*y)) / (width/4)) / 2.0 + 0.5;
            b = (float)y / height;
            setPixel(x,y,r,g,b);
        }
    }
}

void fimage::charFromFloat(fimage *im, unsigned char* pim, float depth) {    
    int w = im->width;
    int h = im->height;
    fimage fim(im);
    int spot;
    int dith[4][3] = { {1,0,7},{-1,1,3},{0,1,5},{1,1,1} };

    
    float before;
    float after;
    float error;
    
    int tx, ty;
    
    for (int i = 0; i < fim.count; i++){
        fim.data[i] *= depth ;
    }
    
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            for (int z = 0; z < 3; z++) {
                spot = (y * w + x) * 3 + z;
                if (fim.data[spot] > depth - 1.0)
                    fim.data[spot] = depth - 1.0;
                if (fim.data[spot] < 0.0)
                    fim.data[spot] = 0.0; 
                before = fim.data[spot];
                after = floor(fim.data[spot]);
                error = before - after;
                *(pim + spot) = (char)after;
                
                for (int i = 0; i < 4; i++) {
                    tx = x + dith[i][0];
                    ty = y + dith[i][1];
                    if (fim.validCoord(tx,ty)) {
                        spot = (ty * w + tx) * 3 + z;
                        fim.data[spot] += error * (float)dith[i][2] / 16.0;
                    }
                }
            }
        }    
    }
}