/*
 * Author: Roy Banuelos
 * About: Declaration for WorldEngine.h
 * constructor will parce obj files.
 * this will also render objects in 
 * 3D space program also used a double
 * binary tree to sort through all faces
 * for collision detection
 */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <cmath>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdio.h>
#include "WorldEngine.h"
#include "loadBMP.h"
	
using namespace std;

// //////////////////////////////////////////////////////////////////////////||////////////////////
// creates new nodes for the binary tree
bitree *worldEngine::newNode(GLfloat coord)
{
    return new bitree(coord);
}
// adds faces to binary tree based on the v1 vertex
void worldEngine::bitreeAdd(vec *fce)
{
    bitree *node, *trav, *prev;

    trav = root;
    prev = trav;

    // create new node and add it to the tree
    node = newNode(fce->v1[0]);
    node->fce = fce;

    if (trav == NULL){
	root = node;
	return;
    }
    else{
	// find the end of the binary tree
	while (trav != NULL){
	    prev = trav;
	    if (fce->v1[0] > trav->xory)
		trav = trav->right;
	    else
		trav = trav->left;
	}

	if (fce->v1[0] > prev->xory){

	    prev->right = node;
	}else{
	    if (fce->v1[0] == prev->xory )
		//bitreeAddY(prev->yroot, node, fce);
		prev->yroot = node;
	    else
		prev->left = node;
	}
    }
}
// //////////////////////////////////////////////////////////////////////////||////////////////////
void worldEngine::bitreeAddY(bitree *Yroot, bitree *node, vec *fce)
{
    bitree *trav, *prev;

    trav = Yroot;
    prev = trav;

    if (trav == NULL){
	Yroot = node;
	return;
    }
    else{
	// find the end of the binary tree
	while (trav != NULL){
	    prev = trav;
	    if (fce->v1[0] > trav->xory)
		trav = trav->right;
	    else
		trav = trav->left;
	}

	if (fce->v1[0] > prev->xory){
	    prev->right = node;
	}else
	    prev->left = node;
    }
}
// //////////////////////////////////////////////////////////////////////////||////////////////////
vector<vec*> worldEngine::search(GLfloat point[])
{
    bitree *trav, *prev, *yparent;
    GLfloat ycord;
    vector<vec*> ret;

    trav = root;
    prev = trav;

    // find closest x coordinate
    while (trav != NULL){
	prev = trav;

	if (point[0] > trav->xory){
	    trav = trav->right;
	}else
	    trav = trav->left;
    }
    if (prev != NULL){
	// find closest y coordinate
	yparent = prev;                    
	if (prev->yroot == NULL){
	    ret.push_back(prev->fce);
	    return ret;
	}else{
	    trav = prev->yroot;
	    while (trav != NULL){
		prev = trav;
		if (point[2] > trav->xory)
		    trav = trav->right;
		else
		    trav = trav->left;
	    }

	    // get list of faces that are closest to the 
	    // found y coorodinate. continue to check untill
	    // y coordinate is not the same
	    ycord = prev->xory;
	    ret.push_back(yparent->fce);

	    while (prev != NULL){
		if (prev->xory == ycord){
		    ret.push_back(prev->fce);
		    prev = prev->left;
		}
	    }
	}
    }
    return ret;
}
// //////////////////////////////////////////////////////////////////////////||////////////////////
void worldEngine::destroyTree(bitree *node)
{
    if (node != NULL){
	// for the x coordinates
	// goto the end of the binary tree
	destroyTree(node->left);
	destroyTree(node->right);

	// do the same for the y coordinats
	destroyTree(node->yroot);

	// delete the end node
	delete node;
    }
}
// /////////////////////////////////////////////////////////////////////////////////////////////////
bool worldEngine::rayplane(float nv[3], float sv[3], float dv[3], vec fce)
{
    float a, t,x,y,z;

    a = dv[0]*nv[0] + dv[1]*nv[1] + dv[2]*nv[2];

    if (a == 0){
	return false;
    }
    t = (fce.v1[0]*nv[1] + fce.v1[1]*nv[1] + 
	    fce.v1[2]*nv[2] - nv[0]*sv[0] - 
	    nv[1]*sv[1] - nv[2]*sv[2]) / a;
    if (t < 0)
	return false;
    x = sv[0]+t*dv[0];
    y = sv[1]+t*dv[1];
    z = sv[2]+t*dv[2];

    float pt[3] = {x,y,z};

    x = baricen(fce.v1, fce.v2, fce.v3);
    y = baricen(fce.v1, fce.v2, pt);
    z = baricen(fce.v1, pt, fce.v3);
    t = baricen(fce.v2, fce.v3, pt);

    if(abs(x -y -z -t) < 0.00001)
	return true;
    else
	return false;
}
// ////////////////////////////////////////////////////////////////////////////////////////////////////
double worldEngine::baricen(float tp1[3], float tp2[3], float tp3[3])
{
    crossProd(tp1,tp2,tp3);
    return .5 * sqrt(vr[0]*vr[0] + vr[1]*vr[1] + vr[2]*vr[2]);
}
// ////////////////////////////////////////////////////////////////////////////////////////////////////
void worldEngine::crossProd(float* coord1,float* coord2,float* coord3 )
{
    float va[3], vb[3];
    va[0] = coord1[0] - coord2[0];
    va[1] = coord1[1] - coord2[1];
    va[2] = coord1[2] - coord2[2];

    vb[0] = coord1[0] - coord3[0];
    vb[1] = coord1[1] - coord3[1];
    vb[2] = coord1[2] - coord3[2];

    // set cross product vector
    vr[0] = va[1] * vb[2] - vb[1] * va[2];
    vr[1] = vb[0] * va[2] - va[0] * vb[2];
    vr[2] = va[0] * vb[1] - vb[0] * va[1];
}
worldEngine::worldEngine()
{
}
worldEngine::worldEngine(const char filename[200])
{
    load(filename);
}
void worldEngine::load(const char filename[200])
{
    char line[200];
    GLfloat X,Y,Z;
    GLfloat u,v,s;
    GLfloat tvec[3][3];
    int vertNum[3];
    int triangle_index = 0,
	vertlim;

    solid = true;
    TotalConnectedTriangles = 0;
    TotalConnectedPoints = 0;

    root = NULL;
    x = y= z =0;
    rx = ry =rz =0;

    vertex.resize(0);
    normals.resize(0);
    Faces_Triangles.resize(0);
    vertexBuffer.resize(0);
    UVs.resize(0); 

    ifstream objFile (filename);

    // If obj file is open, continue
    if (objFile)
    {
	while(!objFile.eof())
	{
	    objFile.getline(line,199);//getline(objFile, line);

	    switch (line[0]) {
		case 'v':
		    line[0] = ' ';
		    switch (line[1]) {
			case ' ':
			    line[1] = ' ';
			    sscanf(line,"%g %g %g ",&X,&Y,&Z);
			    vertex.push_back(X);
			    vertex.push_back(Y);
			    vertex.push_back(Z);
			    TotalConnectedPoints += 3;
			    break;
			case 't':
			    line[1] = ' ';
			    sscanf(line,"%g %g", &u,&v);
			    UVs.push_back(u);
			    UVs.push_back(v);
			    break;
			case 'n':
			    line[1] = ' ';
			    sscanf(line,"%g %g %g", &u,&v,&s);
			    normals.push_back(u);
			    normals.push_back(v);
			    normals.push_back(s);
			    break;
		    }
		    break;

		case 'f':
		    line[0] = ' ';
		    sscanf(line,"%i %i %i",
			    &vertNum[0],
			    &vertNum[1],
			    &vertNum[2]);
		    vertNum[0] -= 1;
		    vertNum[1] -= 1;
		    vertNum[2] -= 1;
		    vertlim = vertex.size();
		    for (int i = 0; i < 3; i++) {
			u = (vertex[(3*vertNum[i]) % vertlim]);
			v = (vertex[(3*vertNum[i]+1) %vertlim]);
			s = (vertex[(3*vertNum[i]+2) %vertlim]);

			tvec[i][0] = u;
			tvec[i][1] = v;
			tvec[i][2] = s;

			Faces_Triangles.push_back(u);
			Faces_Triangles.push_back(v);
			Faces_Triangles.push_back(s);
		    }
		    bitreeAdd(new vec(
				tvec[0][0], tvec[0][1], tvec[0][2],
				tvec[1][0], tvec[1][1], tvec[1][2],
				tvec[2][0], tvec[2][1], tvec[2][2]));

		    if(normals.size() >= 0) {
			float coord1[3] = { 
			    Faces_Triangles[triangle_index], 
			    Faces_Triangles[triangle_index+1],
			    Faces_Triangles[triangle_index+2]};
			float coord2[3] = {
			    Faces_Triangles[triangle_index+3],
			    Faces_Triangles[triangle_index+4],
			    Faces_Triangles[triangle_index+5]};
			float coord3[3] = {
			    Faces_Triangles[triangle_index+6],
			    Faces_Triangles[triangle_index+7],
			    Faces_Triangles[triangle_index+8]};
			calculateNormal(coord1, coord2, coord3);

			for (int i = 0; i < 3; i++){
			    normals.push_back( norm[0]);
			    normals.push_back( norm[1]);
			    normals.push_back( norm[2]);
			}
		    }

		    triangle_index += 9;
		    TotalConnectedTriangles += 3;
		    break;
	    }
	}
	objFile.close();
    }
}
// //////////////////////////////////////////////////////////////////////////||////////////////////
void worldEngine::calculateNormal(float* coord1,float* coord2,float* coord3 )
{
    float va[3], vb[3], val;
    va[0] = coord1[0] - coord2[0];
    va[1] = coord1[1] - coord2[1];
    va[2] = coord1[2] - coord2[2];

    vb[0] = coord1[0] - coord3[0];
    vb[1] = coord1[1] - coord3[1];
    vb[2] = coord1[2] - coord3[2];

    // set cross product 
    vr[0] = va[1] * vb[2] - vb[1] * va[2];
    vr[1] = vb[0] * va[2] - va[0] * vb[2];
    vr[2] = va[0] * vb[1] - vb[0] * va[1];

    // normalization factor
    val = sqrt( vr[0]*vr[0] + vr[1]*vr[1] + vr[2]*vr[2] );

    norm[0] = vr[0]/val;
    norm[1] = vr[1]/val;
    norm[2] = vr[2]/val;
    ///////////////////////////////////////////////////////////////////////////////
    return;
}
// //////////////////////////////////////////////////////////////////////////||////////////////////
void worldEngine::draw()
{
    glPushMatrix();

    glEnableClientState(GL_VERTEX_ARRAY);   // Enable vertex arrays
    glEnableClientState(GL_NORMAL_ARRAY);   // Enable normal arrays

    glTranslatef(x,y,z);

    glRotatef(rx, 1.0f,.0f,.0f);       // Rotate On The X axis 
    glRotatef(ry, .0f,1.0f,.0f);       // Rotate On The Y axis 
    glRotatef(rz, .0f,.0f,1.0f);       // Rotate On The Z axis 

    // Vertex Pointer to triangle array
    glVertexPointer(3 ,GL_FLOAT, 0, Faces_Triangles.data());
    // Normal pointer to normal array
    glNormalPointer(GL_FLOAT, 0, normals.data());
    // Draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, TotalConnectedTriangles);
    glDisableClientState(GL_VERTEX_ARRAY);  // Disable vertex arrays
    glDisableClientState(GL_NORMAL_ARRAY);  // Disable normal arrays

    glPopMatrix();
}
// //////////////////////////////////////////////////////////////////////////||////////////////////
bool worldEngine::isTouching(float x, float y, float z, float r)
{
    GLfloat cen[] = {x,y,z};
    return isTouching(cen, r);
}
// //////////////////////////////////////////////////////////////////////////||////////////////////
bool worldEngine::isTouching(float center[], float r)
{
    vector<vec*> faces;
    //float ptNorm[3], ptdir[3];
    float dist1=0, dist2=0, dist3=0,dist4=0;
    float npt1[3];

    if (solid == true){
	// find the closest triangle
	// calculate new mesh locations
	npt1[0] = center[0] - x;
	npt1[1] = center[1] - y;
	npt1[2] = center[2] - z;

	faces = search(npt1);

	//cout << faces.size() << endl;
	for (unsigned int i=0; i<faces.size(); i++){
	    dist1 = baricen(faces[i]->v1,faces[i]->v2,faces[i]->v3);
	    dist2 = baricen(faces[i]->v1,faces[i]->v2,npt1);
	    dist3 = baricen(faces[i]->v1,npt1,faces[i]->v3);
	    dist4 = baricen(npt1,          faces[i]->v2, faces[i]->v3);

	    if (abs(dist1-dist2-dist3-dist4) < 0.001+r)
		return true;
	}
    }
    return false;
}
// //////////////////////////////////////////////////////////////////////////||////////////////////
bool worldEngine::isTouchingRvec(float center[], float r, float *ret)
{
    vector<vec*> faces;
    float ptNorm[3], ptdir[3];
    float dist1=0, dist2=0;

    if (solid == true){
	// find the closest triangle
	faces = search(center);
	ptNorm[0] = (faces[0]->v2[1]*faces[0]->v3[2]) - 
	    (faces[0]->v2[2]*faces[0]->v3[1]);
	ptNorm[1] = (faces[0]->v2[2]*faces[0]->v3[0]) -
	    (faces[0]->v2[0]*faces[0]->v3[2]);
	ptNorm[2] = (faces[0]->v1[0]*faces[0]->v2[1]) -
	    (faces[0]->v1[1]*faces[0]->v2[0]);

	ptdir[0] = -1*ptNorm[0];
	ptdir[1] = -1*ptNorm[1];
	ptdir[2] = -1*ptNorm[2];

	if (rayplane(ptNorm, center, ptdir, *faces[0]) ||
		rayplane(ptdir, center, ptNorm, *faces[0])){
	    if (dist1 > r || dist2 > r)
		return false;
	    else{
		if (dist1>0){
		    ret[0] =  center[0] - ptNorm[0]*(r-dist1);
		    ret[1] =  center[1] - ptNorm[1]*(r-dist1);
		    ret[2] =  center[2] - ptNorm[2]*(r-dist1);
		}else{
		    ret[0] =  center[0] + ptNorm[0]*(r-dist2);
		    ret[1] =  center[1] + ptNorm[1]*(r-dist2);
		    ret[2] =  center[2] + ptNorm[2]*(r-dist2);
		}
		return true;
	    }
	}

    }
    return false;
}
// //////////////////////////////////////////////////////////////////////////||////////////////////
void worldEngine::loc(float inX, float inY, float inZ)
{
    x = inX;
    y = inY;
    z = inZ;
}
// //////////////////////////////////////////////////////////////////////////||////////////////////
void worldEngine::rot(float inX, float inY, float inZ)
{
    rx = inX;
    ry = inY;
    rz = inZ;
}
// //////////////////////////////////////////////////////////////////////////||////////////////////
worldEngine::~worldEngine()
{
    destroyTree(root);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//         ////////         //////////          //////////          ///////////            ///////
/////////////////////////////////////////////////////////////////////////////////////////////////
// texture uplading variables for pahts
GLuint loadBMP::getBMP(char *path)
{
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize, wAlpha;
	int pixel;

	FILE * file = fopen(path,"rb");
	if (!file)
	{
		printf("UNABLE TO OPEN %s\n", path);
		return 0;
	}

	if (fread(header,1,54,file) != 54)
	{
		printf("NOT A BMP FILE\n");
		return 0;
	}
	if (header[0] != 'B' || header[1] != 'M')
	{
		printf("NOT A BMP FILE\n");
		return 0;
	}
	// read image header imformation
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// if the image header is corupted somehow
	if (imageSize == 0)
	{
		imageSize = width * height * 3;
	}
	if(dataPos == 0)
		dataPos = 54;

	// get image data
	pixel = width * height;
	wAlpha = width * height * 4;
	unsigned char *data = new unsigned char[imageSize];
	unsigned char *data_A = new unsigned char[wAlpha];
	fread(data, 1, imageSize, file);
	fclose(file);

	// apply apha channel
	for (int i=0; i< pixel; i++)
	{
		data_A[i*4] = data[i*3];
		data_A[i*4+1] = data[i*3+1];
		data_A[i*4+2] = data[i*3+2];
		if(data[i*3]== 0 and data[i*3+1]==0 and data[i*3+2]==0){
			data_A[i*4+3] = 0;
		}else
			data_A[i*4+3] = 250;
	}

	// format image to gl format
	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data_A);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	delete data;
	delete data_A;
	return textureID;
}
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////
// texture uploader for direct file uploading
GLuint loadBMP::getBMP(const char *path)
{
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize, wAlpha;
	int pixel;

	FILE * file = fopen(path,"rb");
	if (!file)
	{
		printf("UNABLE TO OPEN %s\n", path);
		return 0;
	}

	if (fread(header,1,54,file) != 54)
	{
		printf("NOT A BMP FILE\n");
		return 0;
	}
	if (header[0] != 'B' || header[1] != 'M')
	{
		printf("NOT A BMP FILE\n");
		return 0;
	}
	// read image header imformation
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// if the image header is corupted somehow
	if (imageSize == 0) 
	{
		imageSize = width * height * 3;
	}
	if (dataPos == 0)
		dataPos = 54;

	// get image data
	pixel = width * height;
	wAlpha = width * height * 4;
	unsigned char *data = new unsigned char[imageSize];
	unsigned char *data_A = new unsigned char[wAlpha];
	fread(data, 1, imageSize, file);
	fclose(file);

	// apply apha channel
	for (int i=0; i< pixel; i++) 
	{
		data_A[i*4] = data[i*3];
		data_A[i*4+1] = data[i*3+1];
		data_A[i*4+2] = data[i*3+2];
		if(data[i*3]== 0 and data[i*3+1]==0 and data[i*3+2]==0){
			data_A[i*4+3] = 0;
		}else
			data_A[i*4+3] = 250;
	}

	// format image to gl format
	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data_A);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	delete data;
	delete data_A;
	return textureID;
}

