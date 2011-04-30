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
	if (keyStates[0x1b])	{ std::cout << "Quitting" << std::endl; exit(0); }
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

void keySpecialPressed (int key, int x, int y) 
{
	keySpecialStates[key] = true; 
	float d = 0.3f;
	if (keySpecialStates[GLUT_KEY_LEFT])  { eyeX -= d; }
	if (keySpecialStates[GLUT_KEY_RIGHT]) { eyeX += d; }
	if (keySpecialStates[GLUT_KEY_DOWN])  { eyeZ += d; }
	if (keySpecialStates[GLUT_KEY_UP])    { eyeZ -= d; }
}

void keyUp (unsigned char key, int x, int y) { keyStates[key] = false; }
void keySpecialUp (int key, int x, int y) { keySpecialStates[key] = false; }

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

	increaseChildren(n1,  n1->noofchildren);
	increaseChildren(n2,  n2->noofchildren);
	increaseChildren(n3,  n3->noofchildren);
	increaseChildren(n3b, n3b->noofchildren);
	increaseChildren(n4,  n4->noofchildren);
	increaseChildren(n4b, n4b->noofchildren);

	n1->name = "root"; n1->parent = NULL;
	//n1->target = targetList[0];
	n1->child[0] = n2; //n1->child[1] = n3b; 

	n2->name = "lower"; n2->parent = n1;
	n2->child[0] = n3; n2->child[1] = n3b;  

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

	n1->euler[0] = 45;	n1->euler[1] = 0;	n1->euler[2] = 45; 
	n2->euler[0] = 0;	n2->euler[1] = 0;	n2->euler[2] = 0; 
	n3->euler[0] = 0;	n3->euler[1] = 0;	n3->euler[2] = 0; 
	n3b->euler[0] = 0;	n3b->euler[1] = 0;	n3b->euler[2] = 0; 
	n4->euler[0] = 0;	n4->euler[1] = 0;	n4->euler[2] = 0; 
	n4b->euler[0] = 0;	n4b->euler[1] = 0;	n4b->euler[2] = 0; 

	n1->offset[0] = 0;	n1->offset[1] = 0;	n1->offset[2] = 0; 
	n2->offset[0] = 0;	n2->offset[1] = 0;	n2->offset[2] = 0; 
	n3->offset[0] = 0;	n3->offset[1] = 0;	n3->offset[2] = 0; 
	n3b->offset[0] = 0;	n3b->offset[1] = 0;	n3b->offset[2] = 0; 
	n4->offset[0] = 0;	n4->offset[1] = 0;	n4->offset[2] = 0; 
	n4b->offset[0] = 0;	n4b->offset[1] = 0;	n4b->offset[2] = 0; 
	
	
	nodeList[noofnodes++] = n1; 
	nodeList[noofnodes++] = n2; 
	nodeList[noofnodes++] = n3; 
	nodeList[noofnodes++] = n3b;
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
float distToTarget(NODE *node)
{
	if (node == NULL) { return 0; }
	float a = 0;
	TARGET **tList; int nooftar = 0;
	tList = (TARGET**) malloc(sizeof(TARGET*) * nooftargets);
	getTarget (node, &nooftar, tList);
	for (int i = 0; i< nooftar; i++)
	{
		float b,c,d;
		float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
		calcEndPos(node, pos);
		b = pos[0] - tList[i]->pos[0];
		c = pos[1] - tList[i]->pos[1];
		d = pos[2] - tList[i]->pos[2];
		//std::cout << b << " " << c << " " << d << " ";
		a += sqrt(b*b + c*c + d*d);
	}
	free(tList);
	return a;
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

	int count = 0;
	float d = 0;
	for (int i=0; i<noofe; i++)
	{
		d += distToTarget(nList[i]);
	}

	while ((count < 30) && d > 0.06)
	{
		CCD(nodeList[5]);
		CCD(nodeList[3]);
		CCD(nodeList[4]);
		CCD(nodeList[2]);
		CCD(nodeList[1]);
		CCD(nodeList[0]);
		count ++;

		d = 0;
		for (int i=0; i<noofe; i++)
		{
			d += distToTarget(nList[i]);
		}
	}
	std::cout << "Count " << count << " dist: " << d << std::endl;
	
	//exit(0);
	
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
	t1->pos[0] = 0.8; t1->pos[1] = 2.2; t1->pos[2] = 1;

	t2->name = "LHandTarget";
	t2->pos[0] = -0.8; t2->pos[1] = 2.2; t2->pos[2] = 0;

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
	
	glutSpecialFunc(keySpecialPressed);
	glutSpecialUpFunc(keySpecialUp);

	glutMainLoop();
}
