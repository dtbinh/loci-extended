//COMPILE WITH: mkoctfile -lGL -lglut --link-stand-alone IKtest.cpp -o IKtest && ./IKtest
#include <GL/glew.h>
#include <GL/glut.h>

#include <octave/oct.h>
#include <octave/octave.h>

#include <iostream>
#include <cmath>

bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[256];

bool doJac = false;

const float PI = 3.14159265;

float atX = 0, atZ = 0; float atY = 2;
float eyeX = 0; float eyeY = 2.1; float eyeZ = 7;


GLfloat whiteSpecularMaterial[] = { 1.0, 1.0, 1.0};
GLfloat whiteSpecularLight[] = { 1.0, 1.0, 1.0};
GLfloat blackAmbientLight[] = { 0.0, 0.0, 0.0};
GLfloat whiteDiffuseLight[] = { 1.0, 1.0, 1.0};
GLfloat blankMaterial[] = { 0.0, 0.0, 0.0};
GLfloat mShininess[] = {128};

struct NODE 
{
	const char *name;
	float length[2];
	double euler;
	NODE *parent;
	NODE *child;
};

int noofnodes = 0;
NODE *nodeList[10];

float IKPosX = 3;
float IKPosY = 3;

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

void setupChain(int);

/*
void keyOperations (void)
{
	if (keyStates['q']) {	std::cout << "HELO" << std::endl; }
	int mod = glutGetModifiers();

	if ((keyStates['q']) and (mod == GLUT_ACTIVE_ALT)) {	std::cout << "HELO Alt" << std::endl; }
	if ((keyStates['q']) and not (mod == GLUT_ACTIVE_ALT)) {	std::cout << "HELO" << std::endl; }

	
	if (keySpecialStates[GLUT_KEY_LEFT]) { std::cout << "LEFT" << std::endl;}
	if (keySpecialStates[GLUT_KEY_RIGHT]) { std::cout << "RIGHT" << std::endl;}
	if (keySpecialStates[GLUT_KEY_UP]) { std::cout << "UP" << std::endl;}
	if (keySpecialStates[GLUT_KEY_DOWN]) { std::cout << "DOWN" << std::endl;}
}
*/

void keyPressed (unsigned char key, int x, int y) { 
	switch (key)
	{
		case 'Q':
			std::cout << "Quitting" << std::endl;
			exit(0);
			break;
		case ' ':
			doJac ^= 1;
			break;
		case '1':
			setupChain(1);
			break;
		case '2':
			setupChain(2);
			break;
		case '3':
			setupChain(3);
			break;
		case '4':
			setupChain(4);
			break;
		case '5':
			setupChain(5);
			break;
		case '6':
			setupChain(6);
			break;
	}
}
void keyUp (unsigned char key, int x, int y) { keyStates[key] = false; }
void keySpecialPressed (int key, int x, int y) { keySpecialStates[key] = true; 

	if (keySpecialStates[GLUT_KEY_LEFT])  { IKPosX -= 0.1; }
	if (keySpecialStates[GLUT_KEY_RIGHT]) { IKPosX += 0.1; }
	if (keySpecialStates[GLUT_KEY_DOWN])  { IKPosY -= 0.1; }
	if (keySpecialStates[GLUT_KEY_UP])    { IKPosY += 0.1; }
}
void keySpecialUp (int key, int x, int y) { keySpecialStates[key] = false; }

void mouseFunc (int button, int state, int x, int y)
{

}
float radDeg (float rad) { return rad*180/PI; }
float degRad (float rad) { return rad*PI/180; }


void evaluateChain(NODE* seg)
{
	//std::cout << seg->name << std::endl;
	glPushMatrix();
		glColor3f(0.4, 0, 0);
		glRotatef(seg->euler, 0, 0, 1);
		glBegin(GL_LINE);
			glVertex3f(0, 0, 0);
			glVertex3f(seg->length[0], seg->length[1], 0);
		glEnd();
		glTranslatef(seg->length[0], seg->length[1], 0);

	if (seg->child) { evaluateChain(seg->child); }
	glPopMatrix();
}


void setupChain(int j)
{
	std::cout << "setting up chain with " << j << " links. Current length is " << noofnodes << std::endl;

	if (j>(noofnodes))
	{
		while (j>noofnodes)
		{
			std::cout << "adding node" << std::endl;
			NODE *n = new NODE();
			n->name = "addedNode";
			n->parent = nodeList[noofnodes-1];
			n->length[0] = 0; n->length[1] = 1;
			nodeList[noofnodes-1]->child = n;
			nodeList[noofnodes++] = n;
		}
	}
	else if ( j<noofnodes ) 
	{
		while ( j< noofnodes)
		{
			std::cout << "removing node" << std::endl;
			nodeList[noofnodes-2]->child = NULL;
			delete nodeList[noofnodes-1];
			noofnodes--;
		}
		
	}
	//while (int i = 0; i<j; i++)
	//{
		//NODE *n = new NODE;
	//}
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

	n1->length[0] = 0; n1->length[1] = 1;  
	n2->length[0] = 0; n2->length[1] = 1;  
	n3->length[0] = 0; n3->length[1] = 1;  
	n4->length[0] = 0; n4->length[1] = 1;  
	//std::cout << "n1 Setup" << std::endl;
	//
	
	n1->euler = 45;
	n2->euler = 30;
	n3->euler = -30;
	n4->euler = 30;
	
	
	nodeList[noofnodes++] = n1; //noofnodes++;
	nodeList[noofnodes++] = n2; //noofnodes++;
	nodeList[noofnodes++] = n3; //noofnodes++;
	nodeList[noofnodes++] = n4; //noofnodes++;
	//std::cout << "LEaving Exit" << std::endl;

	return;
}

NODE* getEndEffector(NODE *seg)
{
	if (seg->child)
	{
		getEndEffector(seg->child);
	}
	else
	{
		return seg;
	}
}

void calcEndPos(NODE *end, double *pos)
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

void transpose(float*, int, int, float*);
bool mult(float* A, int m1, int n1, float* B, int m2, int n2, float* res);
void jacobian(NODE *node)
{
	float errorTolerance = 30; 
	float closeTol = 0.05;
	ColumnVector TH = ColumnVector(noofnodes);
	Matrix S = Matrix(2, noofnodes);
	Matrix J = Matrix(2, noofnodes);
	Matrix dX = Matrix(2, noofnodes);
	NODE *startNode= node;

	NODE *end = getEndEffector(node);
	int m = 0;
	while (node)
	{
		TH(m) = node->euler;

		double epos[2]; epos[0] = 0; epos[1] = 0;
		double endpos[2]; endpos[0] = 0; endpos[1] = 0;
		calcEndPos(node, epos);
		calcEndPos(end, endpos);

		dX(0, m) = IKPosX - endpos[0];   //Minimise dX
		dX(1, m) = IKPosY - endpos[1];
		if (sqrt((dX(0,m)*dX(0,m)) + (dX(1,m)*dX(1,m))) < closeTol) { return; }

		S(0, m) = epos[0];
		S(1, m) = epos[1];

		J(0, m) = (IKPosX - S(0, m));
		J(1, m) = (IKPosY - S(1, m));
	 

		m++;
		if (node->child) { node = node->child; }
		else { node = NULL; } 
	}
	//Matrix Ja = Matrix(2, noofnodes);

	//Matrix JP = Matrix(noofnodes, 2);
	Matrix error = Matrix(noofnodes, noofnodes);
	//JP = (J.transpose()*J).inverse()*J.transpose(); 
	//std::cout << J*JP << std::endl;

	//std::cout << "TH: " << TH << std::endl;
	//std::cout << "S :\n" << S << std::endl;
	//std::cout << "J :\n" << J << std::endl;
	//std::cout << "dX :\n" << dX << std::endl;

	//std::cout << "error : \n" << error << std::endl;
	//std::cout << "|error| : \n" << sqrt(error.sumsq().sum(1)(0, 0)) << std::endl;
	
	while ( sqrt(error.sumsq().sum(1)(0, 0)) > errorTolerance)
	{
		error = (identity_matrix(2,2) - (J*J.pseudo_inverse())) * dX;
		dX = quotient(dX, Matrix(dX.rows(),dX.cols()).fill(2));
	}
	//std::cout << "|error| : \n" << sqrt(error.sumsq().sum(1)(0, 0)) << std::endl;

	//std::cout << JP*dX<< std::endl;
	//std::cout << (((JP*dX).column(0)).transpose()) + TH << std::endl;
	//std::cout << J.pseudo_inverse()*dX << std::endl;
	ColumnVector NewTH = ColumnVector(noofnodes);
	NewTH = TH + ((J.pseudo_inverse()*dX).column(0));
	//std::cout << NewTH << std::endl;

	node = startNode;
	for (int i =0; i<NewTH.length(); i++)	
	{
		if (node != NULL)
		{
			while (NewTH(i) > 360) { NewTH(i) -= 360; }
			while (NewTH(i) < -360) { NewTH(i) += 360; }
			node->euler = NewTH(i);
			node = node->child;
		} else { std::cout << "Error: Array too long for chain" << std::endl; }
	}
	return;
}

void CCD(NODE *cur)
{
	if (!cur) { return; }
	NODE *end = getEndEffector(nodeList[0]);
	NODE *oEnd = end;
	double epos[2]; epos[0] = 0; epos[1] = 0;
	double jpos[2]; jpos[0] = 0; jpos[1] = 0;
	calcEndPos(end, epos);
	//std::cout << "e: " << end->name << " Pos = " << epos[0] << ", " << epos[1] << std::endl;
	
	if (cur->parent) 
	{
	   	calcEndPos(cur->parent, jpos);
   
		//std::cout << "j: " << cur->parent->name << " Pos = " << jpos[0] << ", " << jpos[1] << std::endl;
	}
	end = oEnd;
	//std::cout << "t: " << "Target Pos = " << IKPosX << ", " << IKPosY << std::endl;
	//glPushMatrix();
	//	glColor3f(1, 0, 0);
	//	glTranslatef(epos[0], epos[1], 0);
	//	glutSolidSphere(0.1, 5, 5);
	//glPopMatrix();


	double ej[2]; ej[0] = epos[0] - jpos[0]; ej[1]= epos[1] - jpos[1];
	double tj[2]; tj[0] = IKPosX - jpos[0];  tj[1] = IKPosY - jpos[1];

	//std::cout << "ej: = " << ej[0] << ", " << ej[1] << std::endl;
	//std::cout << "tj: = " << tj[0] << ", " << tj[1] << std::endl;

	double ejdottj = ej[0]*tj[0] + ej[1]*tj[1];
	double minejdottj = ej[0]*tj[1] - ej[1]*tj[0];
	double ejSqr = sqrt(ej[0]*ej[0] + ej[1]*ej[1]);
	double tjSqr = sqrt(tj[0]*tj[0] + tj[1]*tj[1]);

	if (tjSqr < 0.05) { return; }
	double cosA = ejdottj/(ejSqr*tjSqr);
	double sinA = minejdottj/(ejSqr*tjSqr);

	//Limit cosA - eliminates rounding errors.
	cosA = cosA>-1?cosA:-1;
	cosA = cosA<1?cosA:1;
	double rotAng = acos(cosA);
	if (rotAng < 0.01 ) { return; }
	if (rotAng > 10) { rotAng = 10; }
	if (sinA < 0) { rotAng = -rotAng; }
	cur->euler += radDeg(rotAng);

	//Normalise to between +- 360
	if (cur->euler > 2*PI) { cur->euler -= 2*PI; } else if (cur->euler < -2*PI) { cur->euler += 2*PI; }

}

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
	glutSolidTeapot(0.5);

	glPushMatrix();
		glColor3f(0, 1, 0);
		glTranslatef(IKPosX, IKPosY, 0);
		glutSolidSphere(0.3, 5, 5);
	glPopMatrix();
	glLineWidth(2);



	//std::cout << "EVALUATEING CHAIN" << std::endl;
	evaluateChain(nodeList[0]);

	if (doJac)
	{
		jacobian(nodeList[0]);
	} else {
		NODE *node = getEndEffector(nodeList[0]);
		while (node)
		{
			CCD(node);
			node = node->parent;
		}

	}


	glEnable(GL_BLEND);
	glPushMatrix();
		glColor4f(0.9, 0.8, 0.8, 0.05);
		glTranslatef(0, 0, -0.05);
		glScalef(1, 1, 0.001);
		glutSolidSphere(noofnodes, 17, 17);
	glPopMatrix();
	
	float fs = 4;
	glColor4f(0.2, 0.8, 0.8, 0.3);
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


int main (int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("First OpenGL Window");
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
