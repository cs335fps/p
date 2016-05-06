#include "parser.h"


void Parser::parseOBJ(Scene *sc, const char* fileName) {
    ifstream file(fileName, ifstream::in);
    string lines;
    vector<Vec> Vecs;
    Object *o = NULL;
    
    while(getline(file,lines)) {
        istringstream line(lines);
        string type;
        line >> type;
        if (type == "v") {
            float x, y, z;
            line >> x >> y >> z;
            Vecs.push_back(Vec(x,y,z * -1.0));
        } else if (type == "f") {
            int x, y, z;
            line >> x >> y >> z;
            x--;
            y--;
            z--;
            o = sc->newObject<Triangle>();
            o->SetVert(Vecs[x],Vecs[y],Vecs[z]);
            //o->SetVert(Vecs[x],Vecs[z] * -1.0,Vecs[y]);
            o->texture.color = Vec(1.0,1.0,1.0);
            //o->texture.reflection = 0.0;
        }
    }
}

void Parser::parse(Scene *sc, char* fileName) {
    ifstream file(fileName, ifstream::in);
    string lines;
    
    Vec cameraOrigin;
    Vec cameraFocus;
    float cameraScale = 1.0;
    int AnyLightRadius = 0;
    int LastObjectALight = 0;    
    Object *o = NULL;
    
    int ignore = 0;
    while (getline(file,lines)) {
        
        if (lines == "/*")
            ignore = 1;
        if (lines == "*/")
            ignore = 0;
        if (ignore)
            continue;
        istringstream line(lines);
        string type;
        line >> type;
        if (type == "resolution:") {
            float w, h, d;
            line >> w >> h >> d;
            sc->image.setRes(w,h,d);
            
        } else if (type == "output:" ) {
            string s;
            line >> s;
            strcpy(sc->output, s.c_str());
        } else if (type == "camera-origin:") {
            float x, y, z;
            line >> x >> y >> z;
            cameraOrigin.set(x,y,z);
            
        } else if (type == "camera-focus:") {
            float x, y, z;
            line >> x >> y >> z;
            cameraFocus.set(x,y,z);
            
        } else if (type == "file-name:") {
            line >> sc->image.fileName;
            
        } else if (type == "camera-zoom:") {
            line >> cameraScale;
            
        } else if (type == "supersample:") {
            line >> sc->superSample;
            
        } else if (type == "recursion:") {
            line >> sc->recursion;
            
        } else if (type == "shadow-sample:") {
            line >> sc->shadowSample;
           
        } else if (type == "ambient:") {
            float x, y, z;
            line >> x >> y >> z;
            sc->ambient.set(x,y,z);

        } else if (type == "soft-shadows:") {
            int x;
            line >> x;
            sc->softShadows = x;
            
        } else if (type == "occlusion:") {
            int x;
            line >> x;
            sc->occlusion = x;
          
        } else if (type == "sample-method:") {
            string sm;
            line >> sm;
            if (sm == "uniform")
                sc->sampleMethod = 0;
            else if (sm == "random")
                sc->sampleMethod = 1;
                    
        } else if (type == "object:") {
            LastObjectALight = 0;
            string ob;
            line >> ob;
            if (ob == "plane") {
                o = sc->newObject<Plane>();
            } else if (ob == "sphere") {
                o = sc->newObject<Sphere>();
            } else if (ob == "triangle") {
                o = sc->newObject<Triangle>();
            } else if (ob == "light") {
                o = sc->newLight();
                LastObjectALight = 1;
            }
            o->texture.pattern = Texture::NONE;
        } else if (type == "vert:" && o != NULL) {
            float x, y, z;
            line >> x >> y >> z;
            o->AddVert(x,y,z);
            
        } else if (type == "vert1:" && o != NULL) {
            float x, y, z;
            line >> x >> y >> z;
            o->SetVert(0,x,y,z);
            
        } else if (type == "vert2:" && o != NULL) {
            float x, y, z;
            line >> x >> y >> z;
            o->SetVert(1,x,y,z);
        
        } else if (type == "vert3:" && o != NULL) {
            float x, y, z;
            line >> x >> y >> z;
            o->SetVert(2,x,y,z);
        
        } else if (type == "image-vert1:" && o != NULL) {
            float x, y;
            line >> x >> y;
            o->texture.imageVert[0][0] = x;
            o->texture.imageVert[0][1] = y;
            
        } else if (type == "image-vert2:" && o != NULL) {
            float x, y;
            line >> x >> y;
            o->texture.imageVert[1][0] = x;
            o->texture.imageVert[1][1] = y;
        
        } else if (type == "image-vert3:" && o != NULL) {
            float x, y;
            line >> x >> y;
            o->texture.imageVert[2][0] = x;
            o->texture.imageVert[2][1] = y;
        
        } else if (type == "origin:" && o != NULL) {
            float x, y, z;
            line >> x >> y >> z;
            o->origin.set(x,y,z);
        } else if (type == "normal:" && o != NULL) {
            float x, y, z;
            line >> x >> y >> z;
            o->SetNormal(x,y,z);
        } else if (type == "texture:" && o != NULL) {
            string s;
            line >> s;
            if (s == "checker")
                o->texture.pattern = Texture::CHECKER;
            else if (s == "image")
                o->texture.pattern = Texture::IMAGE;
        } else if (type == "texture-scale:" && o != NULL) {
            float s;
            line >> s;
            o->texture.scale = s;
        } else if (type == "color:" && o != NULL) {
            float r, g, b;
            line >> r >> g >> b;
            o->texture.color.set(r,g,b);
            
        } else if (type == "color2:" && o != NULL) {
            float r, g, b;
            line >> r >> g >> b;
            o->texture.color2.set(r,g,b);
            
        } else if (type == "color3:" && o != NULL) {
            float r, g, b;
            line >> r >> g >> b;
            o->texture.color3.set(r,g,b);
            
        } else if (type == "texture-scale:" && o != NULL) {
            float s;
            line >> s;
            o->texture.scale = s;
            
        } else if (type == "radius:" && o != NULL) {
            float r;
            line >> r;
            o->SetRadius(r);
            if(r != 0.0 && LastObjectALight)
                AnyLightRadius = 1;
            
        } else if (type == "reflection:" && o != NULL) {
            float r;
            line >> r;
            o->texture.reflection = r;
            
        } else if (type == "intensity:" && o != NULL) {
            float i;
            line >> i;
            o->SetIntensity(i);
            
        } else if (type == "image:" && o != NULL) {
            string s;
            line >> s;
            o->texture.image = sc->AddTexture(s.c_str());
            
        } else if (type == "obj:") {
            string s;
            line >> s;
            parseOBJ(sc, s.c_str());
            
        }
    }
    
    file.close();
    if(AnyLightRadius == 0)
        sc->softShadows = 0;
    sc->setCamera(cameraOrigin, cameraFocus, cameraScale);
}
