//COMPILE WITH: mkoctfile -lGL -lglut --link-stand-alone IKtest.cpp -o IKtest && ./IKtest
#include <GL/glut.h>

#include <octave/oct.h>
#include "nodeFuncs.h"

#include <iostream>
#include <cmath>

bool* keyStates = new bool[256]; 

bool* keySpecialStates = new bool[256];


bool doJac = false;
bool doLimit = false;

float atX = 0, atZ = 0; float atY = 2;
float eyeX = -4; float eyeY = 2.1f; float eyeZ = 7;


GLfloat whiteSpecularMaterial[] = { 1.0, 1.0, 1.0};
GLfloat whiteSpecularLight[] = { 1.0, 1.0, 1.0};
GLfloat blackAmbientLight[] = { 0.0, 0.0, 0.0};
GLfloat whiteDiffuseLight[] = { 1.0, 1.0, 1.0};
GLfloat blankMaterial[] = { 0.0, 0.0, 0.0};
GLfloat mShininess[] = {128};


int noofnodes = 0;
NODE *nodeList[10];
int nooftests = 0;
NODE *testList[10];

float IKPosX = 3;
float IKPosY = 3;
float IKPosZ = 0;

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
	GLfloat LightPosition[] = {-2.0f, 2.0f, 10.0f, 0.0f};
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

void setupChain(int);

void keyPressed (unsigned char key, int x, int y) { 
	keyStates[key] = true;
	switch (key)
	{
		case 'Q': case 0x1b: //0x1B = esc
			std::cout << "Quitting" << std::endl;
			exit(0);
			break;
		case ' ':
			doJac ^= 1;		 	break;
		case 'l':
			doLimit ^= 1;		break;
		case '1':
			setupChain(1);		break;
		case '2':
			setupChain(2);		break;
		case '3':
			setupChain(3);		break;
		case '4':
			setupChain(4);		break;
		case '5':
			setupChain(5);		break;
		case '6':
			setupChain(6);		break;
	}
	float d = 0.1f;

	if (keyStates['a'])		{ IKPosX -= d; }
	if (keyStates['d'])		{ IKPosX += d; }
	if (keyStates['s'])		{ IKPosY -= d; }
	if (keyStates['w'])		{ IKPosY += d; }
	if (keyStates['q'])		{ IKPosZ -= d; }
	if (keyStates['e'])		{ IKPosZ += d; }
}
void keyUp (unsigned char key, int x, int y) { keyStates[key] = false; }
void keySpecialPressed (int key, int x, int y) { keySpecialStates[key] = true; 

	float d = 0.3f;
	if (keySpecialStates[GLUT_KEY_LEFT])  { eyeX -= d; }
	if (keySpecialStates[GLUT_KEY_RIGHT]) { eyeX += d; }
	if (keySpecialStates[GLUT_KEY_DOWN])  { eyeZ += d; }
	if (keySpecialStates[GLUT_KEY_UP])    { eyeZ -= d; }
}
void keySpecialUp (int key, int x, int y) { keySpecialStates[key] = false; }

void mouseFunc (int button, int state, int x, int y)
{

}


void evaluateChain(NODE* seg)
{
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

	if (seg->child) { evaluateChain(seg->child); }
	glPopMatrix();
}

void normaliseWeights(NODE*);

void setupChain(int j)
{
	std::cout << "setting up chain with " << j << " links. Current length is " << noofnodes << std::endl;

	if (j>(noofnodes))
	{
		while (j>noofnodes)
		{
			NODE *n = new NODE();
			n->name = "addedNode";
			n->parent = nodeList[noofnodes-1];
		 	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
			n->weight = 1;
		 	nodeList[noofnodes-1]->child = n;
		 	nodeList[noofnodes++] = n;
		}
	}
	else if ( j<noofnodes ) 
	{
		while ( j< noofnodes)
		{
			nodeList[noofnodes-2]->child = NULL;
			delete nodeList[noofnodes-1];
			noofnodes--;
		}
		
	}
	normaliseWeights(nodeList[0]);
	//while (int i = 0; i<j; i++)
	//{
		//NODE *n = new NODE;
	//}
}
void normaliseWeights(NODE *start)
{
	NODE *node = start;
	int count = 0;
	float weights = 0;
	while (node)
	{
		weights += node->weight;
		count ++;
		node = node->child;
	}
//	float fac = weights/count;
	float fac = count/weights;

	node = start;
	while (node)
	{
		node->weight = node->weight*fac;
		node = node->child;
	}
	
}

void setupChain()
{
	NODE *n1 = new NODE;
	NODE *n2 = new NODE;
	NODE *n3 = new NODE;
	NODE *n4 = new NODE;

	n1->name = "root"; n1->child = n2; n1->parent = NULL;
	n2->name = "lower"; n2->child = n3; n2->parent = n1;
	n3->name = "upper"; n3->child = n4; n3->parent = n2;
	n4->name = "hand";  n4->child = NULL; n4->parent = n3;

	n1->length[0] = 0; n1->length[1] = 1; n1->length[2] = 0; n1->weight = 0.3;
	n2->length[0] = 0; n2->length[1] = 1; n2->length[2] = 0; n2->weight = 1;
	n3->length[0] = 0; n3->length[1] = 1; n3->length[2] = 0; n3->weight = 1;
	n4->length[0] = 0; n4->length[1] = 1; n4->length[2] = 0; n4->weight = 1;
	//std::cout << "n1 Setup" << std::endl;
	
	/*
	n1->euler[0] = 45 ;	n1->euler[1] = 0;	n1->euler[2] = 0;
	n2->euler[0] = 20 ;	n2->euler[1] = 0;	n2->euler[2] = 0;
	n3->euler[0] = -90 ;	n3->euler[1] = 0;	n3->euler[2] = 0;
	n4->euler[0] = 25 ;	n4->euler[1] = 0;	n4->euler[2] = 0;
	*/
	n1->euler[0] = 45;	n1->euler[1] = 0;	n1->euler[2] = 45;
	n2->euler[0] = -90;	n2->euler[1] = 0;	n2->euler[2] = 0;
	n3->euler[0] = 0;	n3->euler[1] = 0;	n3->euler[2] = -90;
	n4->euler[0] = 0;	n4->euler[1] = 0;	n4->euler[2] = 25;
	
	nodeList[noofnodes++] = n1; //noofnodes++;
	nodeList[noofnodes++] = n2; //noofnodes++;
	nodeList[noofnodes++] = n3; //noofnodes++;
	nodeList[noofnodes++] = n4; //noofnodes++;
	//std::cout << "LEaving Exit" << std::endl;
	//
	NODE *na = new NODE;
	na->name = "test1";
	na->length[0] = 0; na->length[1] = 1; na->length[2] = 0; na->weight = 1;
	na->euler[0] = 0;	na->euler[1] = 0;	na->euler[2] = 0;
	na->child = NULL; na->parent = NULL;

	NODE *nb = new NODE;
	nb->name = "test2";
	nb->length[0] = 0; nb->length[1] = 2; nb->length[2] = 0; nb->weight = 1;
	nb->euler[0] = 45;	nb->euler[1] = 0;	nb->euler[2] = 0;
	nb->child = NULL; nb->parent = NULL;
	
	NODE *ne = new NODE;
	ne->name = "test5";
	ne->length[0] = 0; ne->length[1] = 1; ne->length[2] = 0; ne->weight = 1;
	ne->euler[0] = 45;	ne->euler[1] = 0;	ne->euler[2] = -80;
	ne->child = NULL; ne->parent = NULL;

	NODE *ng = new NODE;
	ng->name = "test5";
	ng->length[0] = 0; ng->length[1] = 1; ng->length[2] = 0; ng->weight = 1;
	ng->euler[0] = 45;	ng->euler[1] = 30;	ng->euler[2] = -80;
	ng->child = NULL; ng->parent = NULL;
	


	
	testList[nooftests++] = na;
	testList[nooftests++] = nb;
	testList[nooftests++] = ne;
	testList[nooftests++] = ng;
	


	NODE *nc = new NODE;
	NODE *nd = new NODE;
	NODE *nf = new NODE;
	nc->name = "test-3";
	nc->length[0] = 0; nc->length[1] = 1; nc->length[2] = 0; nc->weight = 1;
	nc->child = nd; nc->parent = NULL;

	nd->name = "test-3a";
	nd->length[0] = 0; nd->length[1] = 1; nd->length[2] = 0; nd->weight = 1;
	nd->child = nf; nd->parent = nc;

	nf->name = "test-3b";
	nf->length[0] = 0; nf->length[1] = 1; nf->length[2] = 0; nf->weight = 1;
	nf->child = NULL; nf->parent = nd;

	nc->euler[0] = 30;	nc->euler[1] = 0;	nc->euler[2] = 0;
	nd->euler[0] = -60;	nd->euler[1] = 0;	nd->euler[2] = 0;
	nf->euler[0] = 0;	nf->euler[1] = 0;	nf->euler[2] = 60;

	
	testList[nooftests++] = nc;
	testList[nooftests++] = nd;
	testList[nooftests++] = nf;
	

	NODE *nh = new NODE;
	NODE *ni = new NODE;
	nh->name = "test-2";
	nh->length[0] = 0; nh->length[1] = 1; nh->length[2] = 0; nh->weight = 1;
	nh->child = ni; nh->parent = NULL;
	testList[nooftests++] = nh;

	ni->name = "test-2a";
	ni->length[0] = 0; ni->length[1] = 1; ni->length[2] = 0; ni->weight = 1;
	ni->child = NULL; ni->parent = nh;
	testList[nooftests++] = ni;

	nh->euler[0] = -80;	nh->euler[1] = 0;	nh->euler[2] = -45;
	ni->euler[0] = 30;	ni->euler[1] = 20;	ni->euler[2] = 0;




	normaliseWeights(n1);
	return;
}


float distToTarget(NODE *node)
{
	float a,b,c,d;
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(node, pos);
	b = pos[0]-IKPosX;
	c = pos[1]-IKPosY;
	d = pos[2]-IKPosZ;

	a = sqrt(b*b + c*c + d*d);
	return a;
}

void transpose(float*, int, int, float*);
bool mult(float* A, int m1, int n1, float* B, int m2, int n2, float* res);



bool jacobian(NODE *node)
{
	float errorTolerance = 10; 
	float closeTol = 0.05;
	Matrix TH = Matrix(3,noofnodes);
	//Matrix W = Matrix(noofnodes, noofnodes).fill(0);
	//ColumnVector W = ColumnVector(noofnodes);
	Matrix S = Matrix(3, noofnodes);
	Matrix V = Matrix(3, noofnodes);
	Matrix J = Matrix(3, noofnodes);
	Matrix dX = Matrix(3, noofnodes);
	NODE *startNode= node;

	NODE *end = getEndEffector(node);
	int m = 0;
	float axis[3*noofnodes];
	while (node)
	{
		TH(0,m) = node->euler[0];
		TH(1,m) = node->euler[1];
		TH(2,m) = node->euler[2];
		//W(m,m) = node->weight;
		//W(m) = node->weight;

		float epos[3]; epos[0] = 0; epos[1] = 0; epos[2] = 0;
		float ppos[3]; ppos[0] = 0; ppos[1] = 0; ppos[2] = 0;
		float endpos[3]; endpos[0] = 0; endpos[1] = 0; endpos[2] = 0;
		calcEndPos(node, epos);
		if (node->parent) { calcEndPos(node->parent, ppos); }
		calcEndPos(end, endpos);

		//dX = distance from target to end effector
		dX(0, m) = IKPosX - endpos[0];   //Minimise dX
		dX(1, m) = IKPosY - endpos[1];
		dX(2, m) = IKPosZ - endpos[2];
		
		float ej[3]; float tj[3];
		ej[0] = endpos[0] - ppos[0];
		ej[1] = endpos[1] - ppos[1];
		ej[2] = endpos[2] - ppos[2];

		tj[0] = IKPosX - ppos[0];
		tj[1] = IKPosY - ppos[1];
		tj[2] = IKPosZ - ppos[2];


		axis[(m*3)+0] = (ej[1]*tj[2]) - (ej[2]*tj[1]);
		axis[(m*3)+1] = (ej[2]*tj[0]) - (ej[0]*tj[2]);
		axis[(m*3)+2] = (ej[0]*tj[1]) - (ej[1]*tj[0]);

		float lenAxis = sqrt(axis[(m*3)+0]*axis[(m*3)+0] + axis[(m*3)+1]*axis[(m*3)+1] + axis[(m*3)+2]*axis[(m*3)+2]);

		axis[(m*3)+0] /= lenAxis;
		axis[(m*3)+1] /= lenAxis;
		axis[(m*3)+2] /= lenAxis;

		/*
		glPushMatrix();
			glTranslatef(ppos[0], ppos[1], ppos[2]);
			glBegin(GL_LINE);
				glVertex3f(-axis[m*3+0]/4, -axis[m*3+1]/4, -axis[m*3+2]/4);
				glVertex3f( axis[m*3+0]/4,  axis[m*3+1]/4,  axis[m*3+2]/4);
			glEnd();
		glPopMatrix();
		*/

		//Fake Cross product to fill the Jacobian
		J(0, m) = (ej[1]*axis[2]) - (ej[2]*axis[1]);
		J(1, m) = (ej[2]*axis[0]) - (ej[0]*axis[2]);
		J(2, m) = (ej[0]*axis[1]) - (ej[1]*axis[0]);

		//std::cout << "V" << V << std::endl; 
		//std::cout << "J" << J << std::endl; 

		m++;
		if (node->child) { node = node->child; }
		else { node = NULL; } 
	}

	//Calculate Distance of end effector to Target. If close, Stop.
	std::cout << "dist = " << distToTarget(end) << std::endl;
	if (distToTarget(end) < closeTol) { return true; }
	
	
	
	/*
	//If error is small, stop iterating
	//If large, halve dX
	Matrix JJ = J*J.pseudo_inverse();
	Matrix error = Matrix(noofnodes, noofnodes);
	error = (identity_matrix(JJ.rows(),JJ.cols()) - (JJ)) * dX;
	while ( sqrt(error.sumsq().sum(1)(0, 0)) > errorTolerance)
	{
		error = (identity_matrix(JJ.rows(),JJ.cols()) - (JJ)) * dX;
		dX = quotient(dX, Matrix(dX.rows(),dX.cols()).fill(2.0));
	}
*/
	ColumnVector NewTH = ColumnVector(noofnodes);
	ColumnVector W = ColumnVector(noofnodes).fill(1);
	std::cout << J.pseudo_inverse()*dX << std::endl;
	NewTH = (J.pseudo_inverse()*dX*W);
	std::cout << TH << " + " << NewTH << std::endl;
	

	//Update node angles.
	node = startNode;
	for (int i =0; i<NewTH.length(); i++)	
	{
		float taxis[3];
		taxis[0] = axis[(i*3)+0]; 
		taxis[1] = axis[(i*3)+1]; 
		taxis[2] = axis[(i*3)+2];
		float eul[3]; eul[0] = 0; eul[1] = 0; eul[2] = 0;

		radAngleAxisRot(degRad(NewTH(i)), taxis, eul);
		if (node != NULL)
		{
			node->euler[0] += radDeg(eul[0]);
			node->euler[1] += radDeg(eul[1]);
			node->euler[2] += radDeg(eul[2]);
			while (node->euler[0] > 360) { node->euler[0] -= 360; }
			while (node->euler[0] < -360) { node->euler[0] += 360; }
			while (node->euler[1] > 360) { node->euler[1] -= 360; }
			while (node->euler[1] > 360) { node->euler[1] -= 360; }
			while (node->euler[2] < -360) { node->euler[2] += 360; }
			while (node->euler[2] < -360) { node->euler[2] += 360; }
			node = node->child;
		} else { std::cout << "Error: Array too long for chain" << std::endl; }
	}
	return false;
}


void CCD(NODE *cur)
{
	if (!cur) { return; }
	std::cout << cur->name << std::endl;
	NODE *end = getEndEffector(nodeList[0]);
	NODE *oEnd = end;
	float epos[3]; epos[0] = 0; epos[1] = 0; epos[2] = 0;
	float jpos[3]; jpos[0] = 0; jpos[1] = 0; jpos[2] = 0;
	calcEndPos(end, epos);
	//std::cout << "e: " << end->name << " Pos = " << epos[0] << ", " << epos[1] << std::endl;
	
	//jpos = point of the joint's beginning
	if (cur->parent) 
	{
	   	calcEndPos(cur->parent, jpos);
	}
	end = oEnd;
	//std::cout << "t: " << "Target Pos = " << IKPosX << ", " << IKPosY << std::endl;
	//glPushMatrix();
	//	glColor3f(0, 0, 1);
	//	glTranslatef(jpos[0], jpos[1], jpos[2]);
	//	glutSolidSphere(0.1, 5, 5);
	//	glColor3f(1, 0, 0);
	//glPopMatrix();


	//ej = vector from joint's rotation point to end effectors end pos
	float ej[3]; 
	ej[0] = epos[0] - jpos[0];
   	ej[1] = epos[1] - jpos[1];
   	ej[2] = epos[2] - jpos[2];

	//tj = vector from joint's rotation point to target
	float tj[3];
   	tj[0] = IKPosX - jpos[0];
   	tj[1] = IKPosY - jpos[1];
   	tj[2] = IKPosZ - jpos[2];

	//std::cout << "ej: = " << ej[0] << ", " << ej[1] << std::endl;
	//std::cout << "tj: = " << tj[0] << ", " << tj[1] << std::endl;

	float ejdottj = ej[0]*tj[0] + ej[1]*tj[1] + ej[2]*tj[2];
	float minejdottj = - ej[0]*tj[0] - ej[1]*tj[1] - ej[2]*tj[2];
	float ejSqr = sqrt(ej[0]*ej[0] + ej[1]*ej[1] + ej[2]*ej[2]);
	float tjSqr = sqrt(tj[0]*tj[0] + tj[1]*tj[1] + tj[2]*tj[2]);

	if (tjSqr < 0.05) { return; }
	float cosA = ejdottj/(ejSqr*tjSqr);

	//Axis = axis to rotate around. CrossProduct of ej+tj
	float axis[3];
	axis[0] = (ej[1]*tj[2])-(ej[2]*tj[1]);
	axis[1] = (ej[2]*tj[0])-(ej[0]*tj[2]);
	axis[2] = (ej[0]*tj[1])-(ej[1]*tj[0]);

	//Normalise Rotation axis
	float lenAxis = sqrt((axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2]));
	float sinA = (axis[0]+axis[1]+axis[2])/(ejSqr*tjSqr);
	std::cout << "Sins " <<  sinA << std::endl;

	axis[0] /= lenAxis;
	axis[1] /= lenAxis;
	axis[2] /= lenAxis;

	glPushMatrix();
		glTranslatef(jpos[0], jpos[1], jpos[2]);
		glBegin(GL_LINE);
			glVertex3f(-axis[0]/4, -axis[1]/4, -axis[2]/4);
			glVertex3f( axis[0]/4,  axis[1]/4,  axis[2]/4);
		glEnd();
	glPopMatrix();

	//Limit cosA - eliminates rounding errors.
	cosA = cosA>-1?cosA:-1;
	cosA = cosA<1?cosA:1;
	float rotAng = acos(cosA);
	if (rotAng < 0.01 ) { return; }
	if (sinA < 0) { rotAng = -rotAng; }
	//if (axis[2] < 0) { rotAng = -rotAng; }
	std::cout << "RotAng: " << radDeg(rotAng) << std::endl;
	//rotAng *= cur->weight;

	float eulers[3];
	radAngleAxisRot(rotAng, axis, eulers);

	cur->euler[0] += radDeg(eulers[0]);
	cur->euler[1] += radDeg(eulers[1]);
	cur->euler[2] += radDeg(eulers[2]);

	if (doLimit)
	{
		float lim = 180;
		while (cur->euler[0] < -lim ) { cur->euler[0] = -lim; }
		while (cur->euler[0] >  lim ) { cur->euler[0] =  lim; }

		while (cur->euler[1] < -lim ) { cur->euler[1] = -lim; }
		while (cur->euler[1] >  lim ) { cur->euler[1] =  lim; }

		while (cur->euler[2] < -lim ) { cur->euler[2] = -lim; }
		while (cur->euler[2] >  lim ) { cur->euler[2] =  lim; }
	} else {
		//Normalise to between +- 360
		if (cur->euler[0] > 360) { cur->euler[0] -= 360; } else if (cur->euler[0] < -360) { cur->euler[0] += 360; }
		if (cur->euler[1] > 360) { cur->euler[1] -= 360; } else if (cur->euler[1] < -360) { cur->euler[1] += 360; }
		if (cur->euler[2] > 360) { cur->euler[2] -= 360; } else if (cur->euler[2] < -360) { cur->euler[2] += 360; }
	}

	std::cout << "Eulers: " << cur->euler[0] << " " << cur->euler[1] << " " <<cur->euler[2] << std::endl; 

}

void display(void)
{
	glEnable(GL_DEPTH_TEST);
	
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();
	light();

	gluLookAt(eyeX, eyeY, eyeZ, atX, atY, atZ, 0, 1, 0);

	//Centre Sphere
	glutSolidSphere(0.1, 13, 13);

	glPushMatrix();
		glColor3f(0, 1, 0);
		glTranslatef(IKPosX, IKPosY, IKPosZ);
		glutSolidSphere(0.3, 5, 5);
	glPopMatrix();
	glLineWidth(2);



	//std::cout << "EVALUATEING CHAIN" << std::endl;

	/*
	for (int i=0; i<nooftests; i++)
	{

		std::cout << testList[i]->name << std::endl;
		float pos[3];
		pos[0] = 0; pos[1] = 0; pos[2] = 0;
		calcEndPos(testList[i], pos);
		glColor3f(0.5, 0, 0);
		glPushMatrix();
		 glTranslatef(pos[0], pos[1], pos[2]);
		 glutSolidSphere(0.1, 5, 5);
		glPopMatrix();

		if (testList[i]->parent == NULL)
		{
			evaluateChain(testList[i]);
		}
	}
	*/
	
	evaluateChain(nodeList[0]);

	
	if (doJac)
	{
		bool c = false;
		int count = 0;
		while ((count < 30) && (c == false))
		{
			count++;
			c = jacobian(nodeList[0]);
		}
	} else {
		NODE *node = getEndEffector(nodeList[0]);
		NODE *enode = node;
		while ((node) && (distToTarget(enode) > 0.05))
		{
			CCD(node);
			node = node->parent;
		}

	}

	float fs = 4;
	glEnable(GL_BLEND);
	glColor4f(0.2f, 0.8f, 0.8f, 0.3f);
	glBegin(GL_QUADS);
		glVertex3f(-fs, 0, -fs);
		glVertex3f(fs, 0, -fs);
		glVertex3f(fs, 0, 0);
		glVertex3f(-fs, 0, 0);
	glEnd();

	glPushMatrix();
		glColor4f(0.9f, 0.8f, 0.8f, 0.05f);
		//glTranslatef(0, 0, -0.05f);
		glScalef(1, 1, 0.001f);
		glutSolidSphere(noofnodes, 17, 17);
	glPopMatrix();


	glColor4f(0.2f, 0.8f, 0.8f, 0.3f);
	glBegin(GL_QUADS);
		glVertex3f(-fs, 0, 0);
		glVertex3f(fs, 0, 0);
		glVertex3f(fs, 0, fs);
		glVertex3f(-fs, 0, fs);
	glEnd();
	glColor3f(1, 0, 0);
	glDisable(GL_BLEND);

	glutSwapBuffers();
	sleep(0.5);
}


#ifndef MAIN
int main (int argc, char **argv) {
	for (int i=0; i<256; i++) { keyStates[i] = false; keySpecialStates[i] = false; }
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("IK Tests 3D");
	//glLineWidth(6);
	
	setupChain();

	init();

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);

	glutMouseFunc(mouseFunc);
	glutSpecialFunc(keySpecialPressed);
	glutSpecialUpFunc(keySpecialUp);

	glutMainLoop();
}
#endif
