#include "scene.h"



Scene::Scene(){}
void Scene::setRes(int w, int h, int d) {
    image.setRes(w, h, d);
    windowScale = 1.0 / image.width / scale;
    windowVect();
    softShadows = 1;
    occlusion = 1;
}

Light* Scene::newLight() {
    Light *l = new Light;
    Lights.push_back(l);
    return l; 
}

fimage* Scene::AddTexture(const char *c) {
    for (unsigned int i = 0; i < Textures.size(); i++) {
        if (strcmp(c,Textures[i]->fileName) == 0) {
            return Textures[i];
        }
    }
    fimage *fi = new fimage(c);
    Textures.push_back(fi);
    return fi;
}

void Scene::setCamera(Vec origin, Vec focus, float s) {
    scale = s;
    for (int i = 0; i < 3; i++) {
        pov[i] = origin[i];
        win[i] = focus[i];
    }        
    win.normalize();
    windowScale = 1.0 / image.width / scale;
    windowVect();
}

Scene::Scene(int w, int h, int d) {
    image.setRes(w, h, d);

    windowScale = 1.0 / image.width / scale;
    windowVect();
    output[0] = '\0';
}

void Scene::windowVect() {

    for (int i = 0; i < 3; i++)
        p[i] = pov[i] + win[i];
    u[0] = -1.0 * win[1];
    u[1] = win[0];
    u[2] = 0.0;
    u.normalize();
    v = win.cross(u);
    v.normalize();
}

Vec Scene::getPixelCoordinate(float x, float y) {
    Vec A;
    float cx = x - ((float)image.width / 2);
    float cy = y - ((float)image.height / 2);
    for (int i = 0; i < 3; i++)
        A[i] = p[i] - cx * windowScale * u[i] - cy * windowScale * v[i];
    return A;
}

void Scene::getPixelVector(float x, float y, Vec *Rp, Vec *RA) {
    Vec w;
    w = getPixelCoordinate(x,y);
    *Rp = pov;
    *RA = w - pov;
}



void Scene::drawScene() {
    Vec o;
    Vec d;
    if (superSample < 1.0)
        superSample = 1.0;

    clock_t t, t2;
    t = clock();
    t2 = t;
    ofstream outfile;
    if (output[0] != '\0') {
        outfile.open(output, ios_base::app);
        outfile << "Number of primatives: " << Objects.size() << endl;
    }
    float avg = 0.0;
    float lastY = 0.0;
    float smooth = .9;
    printf("Time remaining:");
    for (float y = 0.0; y < image.height; y++) {
        for (float x = 0.0; x < image.width; x++) {
            Vec co;
            if (sampleMethod == 0 || superSample == 1.0){
                for (float m1 = 0; m1 < superSample; m1++) {
                    for (float m2 = 0; m2 < superSample; m2++) {
                        float mx = (float)x + m1 / superSample;
                        float my = (float)y + m2 / superSample;
                        getPixelVector(mx,my,&o,&d);
                        co = co + Cast(o, d, recursion);
                    }
                }
            }else if (sampleMethod == 1) {
                int ss = (int)(superSample*superSample);
                for (int i = 0; i < ss; i++) {
                    float mx = (float)x + RAND;
                    float my = (float)y + RAND;
                    getPixelVector(mx,my,&o,&d);
                    co = co + Cast(o, d, recursion);
                }
            }
            co = co * (1.0/superSample/superSample);
            image.setPixel(x,y,co);
        }
        float s = ((float)(clock()-t2))/CLOCKS_PER_SEC;
        if(s > 5.0) {
            float lastSpeed = s / (y-lastY);

            if (avg == 0.0) avg = lastSpeed;
            float avg = smooth * lastSpeed + (1.0-smooth) * avg;

            float remTime = avg * ((float)image.height - y);
            printf("\rTime remaining: %.0f seconds. %.0f percent complete.         ",remTime, y / image.height * 100.0);
            fflush(stdout);
            t2 = clock();
            lastY = y;
        }
    }
    t = clock() - t;
    float s = ((float)t)/CLOCKS_PER_SEC;
    float sps = (float)(image.width * image.height * superSample * superSample) / s;
    printf("\rFinished in %.1f seconds. %.0fk samples per second.\n",s,sps/1000.0);
    outfile << "Render time: " << s << " seconds." << endl;
} 

bool report() {
    static int r = 0;
    if (++r == 10000) {
        r = 0;
        return 1;
    }
    return 0;
}

float Scene::SampleLight(hit *h, Vec *d, Light *l, int rnd) {
    Vec lightDir;
    if (rnd)
        lightDir = l->RandomSpot() - h->location;
    else
        lightDir = l->origin - h->location;

    float b = h->normal.GetNormalized().dot(lightDir.GetNormalized());
    float c = h->normal.dot(*d);
    if ((b < 0.0) == (c < 0.0)) //If the light is on the opposite side we are testing
        return 0.0;
    float lightDist = lightDir.magnitude();
    hit obstacle = Closest(h->location, lightDir);
    if (obstacle.contact && obstacle.distance < lightDist)
        return 0.0;
    return fabs(b);
}

Vec Scene::TraceLight(hit h, Vec d) {
    Vec brightness = ambient;
    int init;
    if (!softShadows)
        init = 1;
    else
        init = 15;
    for (uint i = 0; i < Lights.size(); i++) {
        float brightness2 = 0.0;
        float min = 9e9;
        float max = 0;
        float cur;
        for (int j = 0; j < init; j++){
            cur = SampleLight(&h, &d, Lights[i],softShadows);
            brightness2 = brightness2 + cur;
            if (cur < min)
                min = cur;
            if (cur > max)
                max = cur;
        }
        int addSamples = 0;
        if (softShadows) {
            float diff = max - min;
            if (diff != 0.0)
                addSamples = (int)(1000.0 * pow(diff,4) / diff);
        }
        for (int j = 0; j < addSamples; j++){
            brightness2 = brightness2 + SampleLight(&h, &d, Lights[i],1);
        }

        brightness = brightness + ((Lights[i]->texture.color * Lights[i]->intensity) * (brightness2 / (float)(init + addSamples)));
    }
    return brightness;
}

float Scene::Occlusion(hit h, Vec d) {
    int occ = 25;
    float dist = 0.0;
    float th = 1.0;
    for (int i = 0; i < occ; i++) {
        Vec dir(RAND - 0.5, RAND - 0.5, RAND - 0.5); //Uneven distribution, fix this
        dir.normalize();
        if ((h.normal.dot(dir) < 0.0) == (h.normal.dot(d) < 0.0))
            dir = dir * -1.0;
        hit h2 = Closest(h.location, dir);
        if (h2.contact) {
            float dis = h2.distance;
            dis = dis * (1.0-h.normal.dot(h2.normal));
            if (dis > th)
                dis = th;
            dist += dis;
        } else {
            dist += th;
        }

    }
    dist /= occ;
    if (dist > th)
        dist = th;
    return dist / th;
}

hit Scene::Closest(Vec o, Vec d){
    float closest = INF;
    hit closestHit(0);
    Vec newO = o + (d.GetNormalized() * 0.0001);//step forward a little
    for (uint i = 0; i < Objects.size(); i++) {

        hit h = Objects[i]->Trace(newO, d);
        if (h.contact && h.distance < closest) {
            closest = h.distance;
            closestHit = h;
        }
    }
    return closestHit;
}

Vec Scene::Cast(Vec o, Vec d, int depth) {
    Vec PixelColor;
    Object *ob;
    hit closestHit = Closest(o,d);
    if (closestHit.contact == 0) {
        PixelColor[0] = PixelColor[1] = 1.0 - d[2] * 1.0;
        PixelColor[2] = 1.0;
        return PixelColor;
    } else {
        ob = closestHit.object;
        Vec c = ob->Color(closestHit);
        Vec l;
        if (ob->texture.intensity == 1.0)
            l = Vec(1.0,1.0,1.0);
        else
            l = TraceLight(closestHit,d);
        if (ob->texture.reflection < 1.0) {//Dont bother with color if it is 100% reflective
            PixelColor[0] = c[0] * l[0];
            PixelColor[1] = c[1] * l[1];
            PixelColor[2] = c[2] * l[2];
        }
    }

    if (ob->texture.reflection > 0.0 && depth > 0) {
        Vec reflect = d - (closestHit.normal * 2.0 * d.dot(closestHit.normal));
        reflect = Cast(closestHit.location,reflect,depth-1);
        PixelColor = PixelColor  + (reflect * ob->texture.reflection * 0.75);
    }
    if (occlusion)
        PixelColor = PixelColor * (0.5+(Occlusion(closestHit,d)/2.0));
    


    return PixelColor;
}
