//g++ IKtestMulti.cpp -o IKtestMulti -lGL -lGLU -lglut && ./IKtestMulti

//#include <GL/glew.h>
#include <GL/glut.h>

//#include <octave/oct.h>
#include "nodeFuncsMulti.h"
#include <iostream>
#include <cmath>

bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[256];

float atX = 0, atZ = 0; float atY = 2;
float eyeX = 0; float eyeY = 2.1f; float eyeZ = 7;


GLfloat whiteSpecularMaterial[] = { 1.0, 1.0, 1.0};
GLfloat whiteSpecularLight[] = { 1.0, 1.0, 1.0};
GLfloat blackAmbientLight[] = { 0.0, 0.0, 0.0};
GLfloat whiteDiffuseLight[] = { 1.0, 1.0, 1.0};
GLfloat blankMaterial[] = { 0.0, 0.0, 0.0};
GLfloat mShininess[] = {128};

int noofnodes = 0;
//int nooftargets = 0;
NODE *nodeList[10];
TARGET *targetList[10];

void init (void)
{
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_COLOR_MATERIAL);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
}

void light (void)
{
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);
	GLfloat LightPosition[] = {2.0f, 2.0f, 5.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
}

void reshape (int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)width/(GLfloat)height, 0.01, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyPressed (unsigned char key, int x, int y) { 
	keyStates[key] = true; 
	//int mod = glutGetModifiers();
	//if ((keyStates['Q']) and (mod == GLUT_ACTIVE_SHIFT)) {	std::cout << "Quitting?" << std::endl; exit(0); }
	//else if ((keyStates['q']) and not (mod)) {	std::cout << "q" << std::endl; }
	float d = 0.1f;
	if (keyStates['a'])  	{ targetList[1]->pos[0] -= d; }
	if (keyStates['d']) 	{ targetList[1]->pos[0] += d; }
	if (keyStates['s'])  	{ targetList[1]->pos[1] -= d; }
	if (keyStates['w'])   	{ targetList[1]->pos[1] += d; }
	if (keyStates['q'])  	{ targetList[1]->pos[2] -= d; }
	if (keyStates['e'])   	{ targetList[1]->pos[2] += d; }

	if (keyStates['j'])  	{ targetList[0]->pos[0] -= d; }
	if (keyStates['l']) 	{ targetList[0]->pos[0] += d; }
	if (keyStates['k'])  	{ targetList[0]->pos[1] -= d; }
	if (keyStates['i'])   	{ targetList[0]->pos[1] += d; }
	if (keyStates['u'])  	{ targetList[0]->pos[2] -= d; }
	if (keyStates['o'])   	{ targetList[0]->pos[2] += d; }

}
void keyUp (unsigned char key, int x, int y) { keyStates[key] = false; }

void mouseFunc (int button, int state, int x, int y)
{

}

void evaluateChain(NODE* seg)
{
	if (seg == NULL ) { return; }
	//std::cout << seg->name << std::endl;
	glPushMatrix();
		glColor3f(0.4f, 0, 0);
		glRotatef(seg->euler[2], 0, 0, 1);
		glRotatef(seg->euler[1], 0, 1, 0);
		glRotatef(seg->euler[0], 1, 0, 0);
		glBegin(GL_LINES);
			glVertex3f(0, 0, 0);
			glVertex3f(seg->length[0], seg->length[1], seg->length[2]);
		glEnd();
		glTranslatef(seg->length[0], seg->length[1], seg->length[2]);

	NODE *oseg = seg;
	for (int i=0; i< seg->noofchildren; i++)
	{
		seg = oseg;
		if (seg->child[i] != NULL) 
		{
		   	evaluateChain(seg->child[i]); 
		}
	}
	glPopMatrix();
}

void increaseChildren(NODE* seg, int noofchildren)
{
	seg->child = (NODE**) malloc(sizeof(NODE*) * noofchildren);
}

void setupChain()
{
std::cout << "Setting up Chain" << std::endl;
	NODE *n1 = new NODE;
	//NODE *n2 = new NODE;
	//NODE *n3 = new NODE;
	NODE *n4 = new NODE;
	//NODE *n3b = new NODE;
	NODE *n4b = new NODE;

	n1->noofchildren = 2; n1->target = NULL;
	//n2->noofchildren = 2; n2->target = NULL;
	//n3->noofchildren = 1; n3->target = NULL;
	//n3b->noofchildren = 1; n3b->target = NULL;
	n4->noofchildren = 0; n4->target = NULL;
	n4b->noofchildren = 0; n4b->target = NULL;

	std::cout << "Setting up links" << std::endl;

	increaseChildren(n1, n1->noofchildren);
	//increaseChildren(n2, n2->noofchildren);
	//increaseChildren(n3, n3->noofchildren);
	//increaseChildren(n3b, n3b->noofchildren);
	increaseChildren(n4, n4->noofchildren);
	increaseChildren(n4b, n4b->noofchildren);

	n1->name = "root"; n1->child[0] = n4; n1->child[1] = n4b; n1->parent = NULL;
	//n3->name = "upper"; n3->child[0] = n4; n3->parent = n1;
	//n3b->name = "LUPPER"; n3b->child[0] = n4b; n3b->parent = n2;
	n4->name = "hand";  n4->child[0] = NULL; n4->parent = n1; n4->target = targetList[0];
	n4b->name = "LHAND";  n4b->child[0] = NULL; n4b->parent = n1; n4b->target = targetList[1];


	std::cout << "Setting chain Lens" << std::endl;

	n1->length[0] = 0; n1->length[1] = 1;  
	//n2->length[0] = 0; n2->length[1] = 1;  
	//n3->length[0] = 0; n3->length[1] = 1;  
	//n3b->length[0] = 0; n3b->length[1] = 1;  
	n4->length[0] = 0; n4->length[1] = 1;  
	n4b->length[0] = 0; n4b->length[1] = 1;  
	//std::cout << "n1 Setup" << std::endl;
	//
	/*
	n1->euler = 45;
	n2->euler = 30;
	n3->euler = -30;
	n4->euler = 30;
	*/

	n1->euler[0] = 0;	n1->euler[1] = 0;	n1->euler[2] = 0; 
	//n2->euler[0] = 0;	n2->euler[1] = 0;	n2->euler[2] = 0; 
	//n3->euler[0] = 0;	n3->euler[1] = 0;	n3->euler[2] = 0; 
	//n3b->euler[0] = 0;	n3b->euler[1] = 0;	n3b->euler[2] = 0; 
	n4->euler[0] = 0;	n4->euler[1] = 0;	n4->euler[2] = 0; 
	n4b->euler[0] = 0;	n4b->euler[1] = 0;	n4b->euler[2] = 0; 
	
	nodeList[noofnodes++] = n1; 
	//nodeList[noofnodes++] = n2; 
	//nodeList[noofnodes++] = n3; 
	//nodeList[noofnodes++] = n3b;
	nodeList[noofnodes++] = n4; 
	nodeList[noofnodes++] = n4b;
	//std::cout << "LEaving Exit" << std::endl;

	return;

}

void setupChainLonger()
{
	std::cout << "Setting up Chain" << std::endl;
	NODE *n1 = new NODE;
	NODE *n2 = new NODE;
	NODE *n3 = new NODE;
	NODE *n4 = new NODE;
	NODE *n3b = new NODE;
	NODE *n4b = new NODE;

	n1->noofchildren = 1; n1->target = NULL;
	n2->noofchildren = 2; n2->target = NULL;
	n3->noofchildren = 1; n3->target = NULL;
	n3b->noofchildren = 1; n3b->target = NULL;
	n4->noofchildren = 0; n4->target = NULL;
	n4b->noofchildren = 0; n4b->target = NULL;

	std::cout << "Setting up links" << std::endl;

	increaseChildren(n1, n1->noofchildren);
	increaseChildren(n2, n2->noofchildren);
	increaseChildren(n3, n3->noofchildren);
	increaseChildren(n3b, n3b->noofchildren);
	increaseChildren(n4, n4->noofchildren);
	increaseChildren(n4b, n4b->noofchildren);

	n1->name = "root"; n1->child[0] = n2; n1->parent = NULL;
	n2->name = "lower"; n2->child[0] = n3; n2->child[1] = n3b; n2->parent = n1;
	n3->name = "upper"; n3->child[0] = n4; n3->parent = n2;
	n3b->name = "LUPPER"; n3b->child[0] = n4b; n3b->parent = n2;
	n4->name = "hand";  n4->child[0] = NULL; n4->parent = n3; n4->target = targetList[0];
	n4b->name = "LHAND";  n4b->child[0] = NULL; n4b->parent = n3b; n4b->target = targetList[1];


	std::cout << "Setting chain Lens" << std::endl;

	n1->length[0] = 0; n1->length[1] = 1;  
	n2->length[0] = 0; n2->length[1] = 1;  
	n3->length[0] = 0; n3->length[1] = 1;  
	n3b->length[0] = 0; n3b->length[1] = 1;  
	n4->length[0] = 0; n4->length[1] = 1;  
	n4b->length[0] = 0; n4b->length[1] = 1;  
	//std::cout << "n1 Setup" << std::endl;
	//
	/*
	n1->euler = 45;
	n2->euler = 30;
	n3->euler = -30;
	n4->euler = 30;
	*/

	n1->euler[0] = 0;	n1->euler[1] = 0;	n1->euler[2] = 0; 
	n2->euler[0] = 0;	n2->euler[1] = 0;	n2->euler[2] = 0; 
	n3->euler[0] = 0;	n3->euler[1] = 0;	n3->euler[2] = 0; 
	n3b->euler[0] = 0;	n3b->euler[1] = 0;	n3b->euler[2] = 0; 
	n4->euler[0] = 0;	n4->euler[1] = 0;	n4->euler[2] = 0; 
	n4b->euler[0] = 0;	n4b->euler[1] = 0;	n4b->euler[2] = 0; 
	
	nodeList[noofnodes++] = n1; 
	nodeList[noofnodes++] = n2; 
	nodeList[noofnodes++] = n3; 
	nodeList[noofnodes++] = n3b;
	nodeList[noofnodes++] = n4; 
	nodeList[noofnodes++] = n4b;
	//std::cout << "LEaving Exit" << std::endl;

	return;
}
/*

void getEndEffector(NODE *cur, int *noofends, NODE **retList)
{
	if (cur->noofchildren == 0)
	{
		//std::cout << "Found end effector: " << cur->name << std::endl;
		retList[*noofends] = cur;
		*noofends += 1;
		return;
	}
	else
	{
		for (int i=0; i<cur->noofchildren; i++)
		{
			getEndEffector(cur->child[i], noofends, retList);
		}
	}
}
	
NODE* getFirstEndEffector(NODE *seg)
{
	//std::cout << "Getting end Effector for: " << seg->name << std::endl;
	if (seg->child[0])
	{
		return getFirstEndEffector(seg->child[0]);
	}
	else
	{
		return seg;
	}
}
*/


/*
void calcEndPos(NODE *end, float *pos)
{
	NODE *thisEnd = end;
	//Recursively calculate the parent nodes
	if (end->parent != NULL)
	{
		calcEndPos(end->parent, pos);
	} 
	else
	{
		pos[0] = 0; pos[1] = 0;
	}
	//Iteratively get the orientation given the parent nodes
	float pTheta = 0;
	while (end->parent) { pTheta += end->parent->euler; end = end->parent;	}
	end = thisEnd;

	//length of node and angle from vertical.
	float h = sqrt(end->length[0]*end->length[0] + end->length[1]*end->length[1]);
	float theta = atan2(end->length[0], end->length[1]);

	//std::cout << end->name << " h = " << h << ", theta = " << radDeg(theta) << std::endl;

	//Simple Trig to get the end vector position of this node.
	pos[0] += h*-sin(theta + degRad(end->euler+pTheta));
	pos[1] += h*cos(theta + degRad(end->euler+pTheta));

	//std::cout << end->name << " End Pos = " << pos[0] << ", " << pos[1] << std::endl;

	return;
}
*/

/*
void getTarget(NODE *cur, int *nooftargets, TARGET **retList)
{
	//std::cout << "G-t for " << cur->name << " - " << std::flush;
	//std::cout << "nooftargets:" << *nooftargets << " " << std::flush;
	if (cur->target != NULL) {
		//std::cout << "target found " << cur->target->name << std::endl;
		//std::cout << "nooft set " << retList[0] << " " << cur->target << std::endl;
	   	retList[*nooftargets] = cur->target;
	   	*nooftargets += 1;
	   	return;
   	}
	for (int i = 0; i< cur->noofchildren; i++)
	{
		//int nooft = 0;
	   	//TARGET **rL; 
		//rL = (TARGET**) malloc(sizeof(TARGET*)*3);
		getTarget(cur->child[i], nooftargets, retList);
		
		//for (int i =0; i< nooft; i++)
		//{
		//	retList[*nooftargets+i] = retL[i];
		//}
		//*nooftargets += nooft;

		//free(rL);
	}
	//else { return NULL;	}
}
*/

/*
void CCD(NODE *cur)
{
	std::cout << cur->name << std::endl;
	if (!cur) { return; }
	NODE **eList; int noofends= 0;
	eList = (NODE**) malloc(sizeof(NODE*) * nooftargets);
	//NODE *oEnd = end;
	getEndEffector(cur, &noofends, eList);
	float epos[3]; epos[0] = 0; epos[1] = 0; epos[2] = 0;
	float jpos[3]; jpos[0] = 0; jpos[1] = 0; jpos[2] = 0;
	float eposS[3];
	for (int i=0; i<noofends; i++)
	{
		eposS[0] = 0; eposS[1] = 0; eposS[2] = 0;
		calcEndPos(eList[i], eposS);
		epos[0] += eposS[0];
		epos[1] += eposS[1];
		epos[2] += eposS[2];
	}
	free(eList);
	epos[0] /= noofends;
	epos[1] /= noofends;
	epos[2] /= noofends;
	//std::cout << "e: " << cur->name << " Pos = " << epos[0] << ", " << epos[1] << std::endl;

	//Get rotation point of current joint - it's parent's end - it's start
	if (cur->parent)
	{
		calcEndPos(cur->parent, jpos);
	}
	
	float ej[3]; 
	ej[0] = epos[0] - jpos[0];
   	ej[1] = epos[1] - jpos[1];
   	ej[2] = epos[2] - jpos[2];

	float tj[3]; tj[0] = 0; tj[1] = 0; tj[2] = 0;
	TARGET **tList; int nooftar = 0;
	tList = (TARGET**) malloc(sizeof(TARGET*) * nooftargets);
	getTarget(cur, &nooftar, tList);
	if (nooftar>0) 
	{
		//std::cout << nooftar<< " targets for " << cur->name << std::flush;
		for (int i=0; i< nooftar; i++)
		{
			tj[0] += tList[i]->pos[0] ;
			tj[1] += tList[i]->pos[1] ;
			tj[2] += tList[i]->pos[2] ;
		}

		tj[0] /= nooftar; 
		tj[1] /= nooftar; 
		tj[2] /= nooftar; 

		glColor3f(1.0f/nooftar, 0, 0 );
		glPushMatrix();
			glTranslatef(tj[0], tj[1], tj[2]);
			glutWireSphere(0.1f, 5, 5);
		glPopMatrix();
		tj[0] -= jpos[0]; 
		tj[1] -= jpos[1];
		tj[2] -= jpos[2];

	} else {
		free(tList);
	   	return;
   	}
	free(tList);


	//std::cout << cur->name << " ";
	//std::cout << "ej: = " << ej[0] << ", " << ej[1] << std::endl;
	//std::cout << "tj: = " << tj[0] << ", " << tj[1] << std::endl;

	float ejdottj = ej[0]*tj[0] + ej[1]*tj[1]; + ej[2]*tj[2];
	float minejdottj = ej[0]*tj[1] - ej[1]*tj[0] - ej[2]*tj[2];
	float ejSqr = sqrt(ej[0]*ej[0] + ej[1]*ej[1] + ej[2]*ej[2]);
	float tjSqr = sqrt(tj[0]*tj[0] + tj[1]*tj[1] + tj[2]*tj[2]);

	if (tjSqr < 0.05) { return; }
	float cosA = ejdottj/(ejSqr*tjSqr);
	float sinA = minejdottj/(ejSqr*tjSqr);

	//Axis = axis to rotate around. CrossProduct of ej+tj
	float axis[3];
	axis[0] = (ej[1]*tj[2])-(ej[2]*tj[1]);
	axis[1] = (ej[2]*tj[0])-(ej[0]*tj[2]);
	axis[2] = (ej[0]*tj[1])-(ej[1]*tj[0]);

	//Normalise Rotation axis
	float lenAxis = sqrt((axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2]));
	axis[0] /= lenAxis;
	axis[1] /= lenAxis;
	axis[2] /= lenAxis;

	//Limit cosA - eliminates rounding errors.
	cosA = cosA>-1?cosA:-1;
	cosA = cosA<1?cosA:1;
	float rotAng = acos(cosA);
	if (rotAng < 0.01 ) { return; }
	if (sinA < 0) { rotAng = -rotAng; }

	//Rotate by acos(cosA) around axis
	//Convert to Euler Rotations angles
	//
	Matrix axisMat = Matrix(3,3);
	axisMat(0,0) = 1+ (1-cos(rotAng))*(axis[0]*axis[0]-1);
	axisMat(0,1) = -axis[2]*sin(rotAng)+(1-cos(rotAng))*axis[0]*axis[1];
	axisMat(0,2) = axis[1]*sin(rotAng)+(1-cos(rotAng))*axis[0]*axis[2];

	axisMat(1,0) = axis[2]*sin(rotAng)+(1-cos(rotAng))*axis[0]*axis[1];
	axisMat(1,1) = 1 + (1-cos(rotAng))*(axis[1]*axis[1]-1);
	axisMat(1,2) = -axis[0]*sin(rotAng)+(1-cos(rotAng))*axis[1]*axis[2];
	
	axisMat(2,0) = -axis[1]*sin(rotAng)+(1-cos(rotAng))*axis[0]*axis[2];
	axisMat(2,1) = axis[0]*sin(rotAng)+(1-cos(rotAng))*axis[1]*axis[2];
	axisMat(2,2) = 1 + (1-cos(rotAng))*(axis[2]*axis[2]-1);

	float x, y, z;
	if (axisMat(1,0) > 0.998)
	{
		std::cout << "sing NORTH" << std::endl;
		 //Singluarity at north Pole
		 y = atan2(axisMat(0,2), axisMat(2,2));
		 x = PI/2;
		 z = 0;
	} else if (axisMat(1,0) < -0.998)
	{
		std::cout << "sing SOUTH" << std::endl;
		//Singularit at south pole
		y = atan2(axisMat(0,2), axisMat(2,2));
		x = -PI/2;
		z = 0;
	} else {
		y = atan2(-axisMat(2,0), axisMat(0,0));
		x = atan2(-axisMat(1,2), axisMat(1,1));
		z = asin(-axisMat(1,0));
	}


	cur->euler[0] += radDeg(x);
	cur->euler[1] += radDeg(y);
	cur->euler[2] += radDeg(z);

	//Normalise to between +- 360
	if (cur->euler[0] > 360) { cur->euler[0] -= 360; } else if (cur->euler[0] < -360) { cur->euler[0] += 360; }
	if (cur->euler[1] > 360) { cur->euler[1] -= 360; } else if (cur->euler[1] < -360) { cur->euler[1] += 360; }
	if (cur->euler[2] > 360) { cur->euler[2] -= 360; } else if (cur->euler[2] < -360) { cur->euler[2] += 360; }

	if (cur->euler[0] > 80) { cur->euler[0] = 80; }
	if (cur->euler[0] < -80) { cur->euler[0] = -80; }
	if (cur->euler[1] > 80) { cur->euler[1] = 80; }
	if (cur->euler[1] > 80) { cur->euler[1] = 80; }
	if (cur->euler[2] < -80) { cur->euler[2] = -80; }
	if (cur->euler[2] < -80) { cur->euler[2] = -80; }

}
*/
void display(void)
{
	//keyOperations();
	glEnable(GL_DEPTH_TEST);
	
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();
	light();

	gluLookAt(eyeX, eyeY, eyeZ, atX, atY, atZ, 0, 1, 0);


	//Centre Sphere
	glutSolidSphere(0.1, 17, 17);

	for (int i=0; i < nooftargets; i++)
	{
		glPushMatrix();
			glColor3f(0, 1, 0);
			glTranslatef(targetList[i]->pos[0], targetList[i]->pos[1], targetList[i]->pos[2]);
			glutWireSphere(0.2, 5, 5);
		glPopMatrix();
	}

	glLineWidth(2);



	NODE **nList;
	NODE **nListO;
	int *nListI;
	nList = (NODE**) malloc(sizeof(NODE*) * noofnodes);
	nListO = (NODE**) malloc(sizeof(NODE*) * noofnodes);
	nListI = (int*) malloc(sizeof(int) * noofnodes);
	int noofe = 0;
	int noofeO = 0;
	getEndEffector(nodeList[0], &noofe, nList);

	/*
	for (int i=0; i<noofe; i++)
	{
		NODE *node = nList[i];
		while (node != NULL)
		{

			bool found = false;
			for (int j=0; j<noofeO; j++)
			{
				if (node == nListO[j])
				{
					found = true;
				}
			}
			if (found == false)
			{
				nListO[noofeO] = node;	
				nListI[noofeO] = i;
				noofeO ++;
			}
			node = node->parent;
		}
	}

	for (int i = 0; i<noofeO; i++)
	{
		std::cout << "CCD: " << nListO[i]->name << " " << std::flush;
		CCD(nListO[i], nList[nListI[i]]);
	}
	*/
	//exit(0);
	//CCD(nodeList[5]);
	//CCD(nodeList[4]);
	//CCD(nodeList[3]);
	//CCD(nodeList[2]);
	//CCD(nodeList[1]); 
	CCD(nodeList[0]);
	std::cout << std::endl;


	//std::cout << "EVALUATEING CHAIN" << std::endl;
	evaluateChain(nodeList[0]);


	glEnable(GL_BLEND);
	glPushMatrix();
		glColor4f(0.9f, 0.8f, 0.8f, 0.05f);
		glTranslatef(0, 0, -0.05f);
		glScalef(1, 1, 0.001f);
		//glutSolidSphere(4, 17, 17);
	glPopMatrix();
	
	float fs = 4;
	glColor4f(0.2f, 0.8f, 0.8f, 0.1f);
	glBegin(GL_QUADS);
		glVertex3f(-fs, 0, -fs);
		glVertex3f(fs, 0, -fs);
		glVertex3f(fs, 0, fs);
		glVertex3f(-fs, 0, fs);
	glEnd();
	glColor3f(1, 0, 0);

	glDisable(GL_BLEND);
	glutSwapBuffers();
}

void setupTargets()
{
	TARGET *t1 = new TARGET;
	TARGET *t2 = new TARGET;

	t1->name = "RHandTarget";
	t1->pos[0] = 3; t1->pos[1] = 3;

	t2->name = "LHandTarget";
	t2->pos[0] = -3; t2->pos[1] = 3;

	targetList[nooftargets++] = t1;
	targetList[nooftargets++] = t2;
}

int main (int argc, char **argv) {
	for (int i=0; i<256; i++) { keyStates[i] = false; keySpecialStates[i] = false; }
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("First OpenGL Window");
	//glLineWidth(6);
	
	setupTargets();
	setupChain();
	std::cout << "Ended setup Chain" << std::endl;

	init();

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);

	glutMouseFunc(mouseFunc);
	//glutSpecialFunc(keySpecialPressed);
	//glutSpecialUpFunc(keySpecialUp);

	glutMainLoop();
}
