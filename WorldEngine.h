/*
this iteration requieres that the obj file is using 
triangles not squares
1) no textures
2) no normals
3) no edges

*/
#ifndef WORLDENGINE_H
#define WORLDENGINE_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <cmath>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <iostream>

using namespace std;

class worldEngine
{
	private:
		struct vec{
			GLfloat v1[3],v2[3],v3[3];
			vec(GLfloat a,GLfloat b,GLfloat c,
			    GLfloat d,GLfloat e,GLfloat f,
			    GLfloat g,GLfloat h,GLfloat i)
			{
				v1[0] = a; v1[1]= b; v1[2] = c;
				v2[0] = d; v2[1]= e; v2[2] = f;
				v3[0] = g; v3[1]= h; v3[2] = i;
			}
			vec(){}
		};
		struct bitree
		{
			bitree *left, *right;
			bitree *yroot;
			GLfloat xory;
			vec *fce;

			bitree(GLfloat x)
			{
				left = right = NULL;
				yroot = NULL;
				xory = x;
			}
		};
		float norm[3];
		GLfloat x, y, z, rx, ry, rz;
		vector<GLfloat> vertex;
		vector<GLfloat> normals;
		vector<GLfloat> Faces_Triangles;
		vector<GLfloat> vertexBuffer;
		vector<GLfloat> UVs;
		long TotalConnectedPoints;
		long TotalConnectedTriangles;
		bool solid;
		bitree *root;

		// //////////////////////////////////////////////////////////////////////////||////////////////////
		// creates new nodes for the binary tree
		bitree *newNode(GLfloat coord)
		{
			return new bitree(coord);
		}
		// adds faces to binary tree based on the v1 vertex
		void bitreeAdd(vec *fce)
		{
			bitree *node, *trav, *prev;

			trav = root;
			prev = trav;
			
			// create new node and add it to the tree
			node = newNode(fce->v1[0]);
			node->fce = fce;

			if (trav == root and trav == NULL){
				root = node;
				return;
			}

			// find the end of the binary tree
			while (trav != NULL){
				prev = trav;
				if (trav->xory > fce->v1[0])
					trav = trav->right;
				else
					trav = trav->left;
			}

			if (prev == NULL){
				prev = node;
			}else{
				if (prev->xory > fce->v1[0])
					prev->right = node;
				else{
					if (prev->xory == fce->v1[0])
						bitreeAddY(prev->yroot, node, fce);
					else
						prev->left = node;
				}
			}
		}
		// //////////////////////////////////////////////////////////////////////////||////////////////////
		void bitreeAddY(bitree *Yroot, bitree *node, vec *fce)
		{
			bitree *trav, *prev;

			trav = Yroot;
			prev = trav;
			// find the end of the binary tree
			while (trav != NULL){
				prev = trav;
				if (trav->xory > fce->v1[1])
					trav = trav->right;
				else
					trav = trav->left;
			}

			// assighn current new node to the new y coordinate
			// then add this node to the x nodes y binary tree
			node->xory = fce->v1[1];
			node->fce = fce;
			if (prev == NULL){
				prev = node;
			}else{
				// adding will create a binary tree
				if (prev->xory > fce->v1[1])
					prev->right = node;
				// if they are equal this will produce a
				// column of z coordinats to check through
				else
					prev->left = node;
			}
		}
		// //////////////////////////////////////////////////////////////////////////||////////////////////
		vector<vec*> search(GLfloat point[])
		{
			bitree *trav, *prev;
			GLfloat ycord;
			vector<vec*> ret;

			trav = root;
			prev = trav;

			// find closest x coordinate
			cout << "Checking X" << endl;
			while (trav != NULL){
				prev = trav;
				if (trav->xory > point[0])
					trav = trav->right;
				else
					trav = trav->left;
			}
			cout << "Checking Y" << endl;
			if (prev != NULL){
				// find closest y coordinate
				trav = prev->yroot;
				while (trav != NULL){
					prev = trav;
					cout << prev << endl;
					if (trav->xory > point[1])
						trav = trav->right;
					else
						trav = trav->left;
				}

				if (prev->yroot == NULL){
					ret.push_back(prev->fce);
					return ret;
				}else{
					// get list of faces that are closest to the 
					// found y coorodinate. continue to check untill
					// y coordinate is not the same
					ycord = prev->xory;
					while (prev->xory == ycord and prev != NULL){
						ret.push_back(prev->fce);
						prev = prev->left;
					}
				}
			}

			return ret;
		}
		// //////////////////////////////////////////////////////////////////////////||////////////////////
		void destroyTree(bitree *node)
		{
			if (root != NULL){
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

	public:
		worldEngine(const char filename[200])
		{
			char line[200];
			GLfloat X,Y,Z;
			GLfloat u,v,s;
			GLfloat tvec[3][3];
			int vertNum[3];
			int triangle_index = 0;		// Set triangle index to zero

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
									// Read floats from the line: v X Y Z
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
							for (int i = 0; i < 3; i++) {
								u = (vertex[(3*vertNum[i]) % vertex.size()]);
								v = (vertex[(3*vertNum[i]+1) % vertex.size()]);
								s = (vertex[(3*vertNum[i]+2) % vertex.size()]);

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
								calculateNormal( coord1, coord2, coord3 );

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

				// coppy verticies and sort
				//sortvert = vertex;
				//sort(sortvert);
			}
		}
		// //////////////////////////////////////////////////////////////////////////||////////////////////
		void calculateNormal(float* coord1,float* coord2,float* coord3 )
		{
			float va[3], vb[3], vr[3], val;
			va[0] = coord1[0] - coord2[0];
			va[1] = coord1[1] - coord2[1];
			va[2] = coord1[2] - coord2[2];

			vb[0] = coord1[0] - coord3[0];
			vb[1] = coord1[1] - coord3[1];
			vb[2] = coord1[2] - coord3[2];

			// cross product 
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
		void draw()
		{
			glPushMatrix();

			glEnableClientState(GL_VERTEX_ARRAY);	// Enable vertex arrays
			glEnableClientState(GL_NORMAL_ARRAY);	// Enable normal arrays

			glTranslatef(x,y,z);

			glRotatef(rx, 1.0f,.0f,.0f);		// Rotate On The X axis 
			glRotatef(ry, .0f,1.0f,.0f);		// Rotate On The Y axis 
			glRotatef(rz, .0f,.0f,1.0f);		// Rotate On The Z axis 

			// Vertex Pointer to triangle array
			glVertexPointer(3 ,GL_FLOAT, 0, Faces_Triangles.data());
			// Normal pointer to normal array
			glNormalPointer(GL_FLOAT, 0, normals.data());
			// Draw the triangles
			glDrawArrays(GL_TRIANGLES, 0, TotalConnectedTriangles);
			glDisableClientState(GL_VERTEX_ARRAY);	// Disable vertex arrays
			glDisableClientState(GL_NORMAL_ARRAY);	// Disable normal arrays

			glPopMatrix();
		}
		// //////////////////////////////////////////////////////////////////////////||////////////////////
		bool isTouching(float x, float y, float z)
		{
			GLfloat cen[] = {x,y,z};
			return isTouching(cen);
		}
		// //////////////////////////////////////////////////////////////////////////||////////////////////
		bool isTouching(float center[])
		{
			vector<vec*> faces;
			
			if (solid == true){
				faces = search(center);
			}
			/*
			for (int i=0; i<faces.size(); i++){
				cout <<faces[i]->v1[0] << " " << faces[i]->v1[1] << " " << faces[i]->v1[2] << endl;
				cout <<faces[i]->v2[0] << " " << faces[i]->v2[1] << " " << faces[i]->v2[2] << endl;
				cout <<faces[i]->v3[0] << " " << faces[i]->v3[1] << " " << faces[i]->v3[2] << endl << endl;
			}
			*/
			return false;
		}
		// //////////////////////////////////////////////////////////////////////////||////////////////////
		void loc(float inX, float inY, float inZ)
		{
			x = inX;
			y = inY;
			z = inZ;
		}
		// //////////////////////////////////////////////////////////////////////////||////////////////////
		void rot(float inX, float inY, float inZ)
		{
			rx = inX;
			ry = inY;
			rz = inZ;
		}
		// //////////////////////////////////////////////////////////////////////////||////////////////////
		~worldEngine()
		{
			destroyTree(root);
		}
};
#endif
