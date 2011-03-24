//COMPILE WITH: mkoctfile -lGL -lglut --link-stand-alone IKtest.cpp -o IKtest && ./IKtest
#include <GL/glut.h>

#include <octave/oct.h>

#include <iostream>
#include <cmath>

bool* keyStates = new bool[256]; 

bool* keySpecialStates = new bool[256];


bool doJac = false;
bool doLimit = true;

const float PI = 3.14159265f;

float atX = 0, atZ = 0; float atY = 2;
float eyeX = -4; float eyeY = 2.1f; float eyeZ = 7;


GLfloat whiteSpecularMaterial[] = { 1.0, 1.0, 1.0};
GLfloat whiteSpecularLight[] = { 1.0, 1.0, 1.0};
GLfloat blackAmbientLight[] = { 0.0, 0.0, 0.0};
GLfloat whiteDiffuseLight[] = { 1.0, 1.0, 1.0};
GLfloat blankMaterial[] = { 0.0, 0.0, 0.0};
GLfloat mShininess[] = {128};

struct NODE 
{
	const char *name;
	float length[3];
	float euler[3];
	float weight;
	NODE *parent;
	NODE *child;
};

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
float radDeg (float rad) { return rad*180/PI; }
float degRad (float rad) { return rad*PI/180; }


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
	testList[nooftests++] = na;

	NODE *nb = new NODE;
	nb->name = "test2";
	nb->length[0] = 0; nb->length[1] = 2; nb->length[2] = 0; nb->weight = 1;
	nb->euler[0] = 45;	nb->euler[1] = 0;	nb->euler[2] = 0;
	nb->child = NULL; nb->parent = NULL;
	testList[nooftests++] = nb;

	NODE *ne = new NODE;
	ne->name = "test5";
	ne->length[0] = 0; ne->length[1] = 1; ne->length[2] = 0; ne->weight = 1;
	ne->euler[0] = 45;	ne->euler[1] = 0;	ne->euler[2] = -80;
	ne->child = NULL; ne->parent = NULL;
	testList[nooftests++] = ne;

	NODE *ng = new NODE;
	ng->name = "test5";
	ng->length[0] = 0; ng->length[1] = 1; ng->length[2] = 0; ng->weight = 1;
	ng->euler[0] = 45;	ng->euler[1] = 30;	ng->euler[2] = -80;
	ng->child = NULL; ng->parent = NULL;
	testList[nooftests++] = ng;



	NODE *nc = new NODE;
	NODE *nd = new NODE;
	NODE *nf = new NODE;
	nc->name = "test-3";
	nc->length[0] = 0; nc->length[1] = 1; nc->length[2] = 0; nc->weight = 1;
	nc->child = nd; nc->parent = NULL;
	testList[nooftests++] = nc;

	nd->name = "test-3a";
	nd->length[0] = 0; nd->length[1] = 1; nd->length[2] = 0; nd->weight = 1;
	nd->child = nf; nd->parent = nc;

	testList[nooftests++] = nd;
	nf->name = "test-3b";
	nf->length[0] = 0; nf->length[1] = 1; nf->length[2] = 0; nf->weight = 1;
	nf->child = NULL; nf->parent = nd;
	testList[nooftests++] = nf;

	nc->euler[0] = 30;	nc->euler[1] = 0;	nc->euler[2] = 0;
	nd->euler[0] = -60;	nd->euler[1] = 0;	nd->euler[2] = 0;
	nf->euler[0] = 0;	nf->euler[1] = 0;	nf->euler[2] = 60;


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

	nh->euler[0] = 0;	nh->euler[1] = 0;	nh->euler[2] = -40;
	ni->euler[0] = 0;	ni->euler[1] = 0;	ni->euler[2] = -40;




	normaliseWeights(n1);
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

Matrix fillRotMat(float a, float b, float g)
{
	//XYZ Matrix - Wikipedia?
	Matrix rotMat = Matrix(3,3); rotMat.fill(0);
	rotMat(0,0) = cos(b)*cos(g);
	rotMat(0,1) = -cos(a)*sin(g) + sin(a)*sin(b)*cos(g);;
	rotMat(0,2) = sin(a)*sin(g) + cos(a)*sin(b)*cos(g);

	rotMat(1,0) = cos(b)*cos(g);
	rotMat(1,1) = cos(a)*cos(g) + sin(a)*sin(b)*sin(g);
	rotMat(1,2) = -sin(a)*cos(g) + cos(a)*sin(b)*sin(g);

	rotMat(2,0) = -sin(b);
	rotMat(2,1) = sin(a)*cos(b);
	rotMat(2,2) = cos(a)*cos(b);

	return rotMat;

	/* // ZYX -- presentation order
	rotMat(0,0) = cos(a)*cos(b)*cos(g)-sin(a)*sin(g);
	rotMat(0,1) = -cos(a)*cos(b)*sin(g)-sin(a)*cos(g);
	rotMat(0,2) = cos(a)*sin(b);

	rotMat(1,0) = sin(a)*cos(b)*cos(g)+cos(a)*sin(g);
	rotMat(1,1) = -sin(a)*cos(b)*sin(g)+cos(a)*cos(g);
	rotMat(1,2) = sin(a)*sin(b);

	rotMat(2,0) = -sin(b)*cos(g);
	rotMat(2,1) = sin(b)*sin(g);
	rotMat(2,2) = -cos(b);
	*/
}

/*
 * calcEndPos
 * take a NODE and a float array of length 3
 * fill the array with x,y,z coords of the nodes's end position
 */
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
		pos[0] = 0; pos[1] = 0; pos[2] = 0;
	}
	//Iteratively get the orientation given the parent nodes
	/*
	float pThetaX, pThetaY, pThetaZ;
	pThetaX = 0;
	pThetaY = 0;
	pThetaZ = 0;
	*/
	Matrix pRotMatF = Matrix(3,3).fill(0);
	Matrix pRotMat = Matrix(3,3).fill(0);
	while ((end->parent != NULL)) { 
		end = end->parent;
	}

	while (end != thisEnd)
	{

		Matrix pRotMatT = Matrix(3,3).fill(0);
		pRotMatT = fillRotMat(degRad(end->euler[0]), degRad(end->euler[1]), degRad(end->euler[2]));
		if ((mx_el_eq(pRotMat, 0) == boolMatrix(3,3,1))){
		   	pRotMatF = pRotMatT; 
		} else {
			pRotMatF = pRotMat *pRotMatT;
		}
		pRotMat = pRotMatF;
		std::cout << "pRotMat" << pRotMatF << std::endl;

	//	pThetaX += end->parent->euler[0]; 
	//	pThetaY += end->parent->euler[1]; 
	//	pThetaZ += end->parent->euler[2]; 
		end = end->child;	
	}
	end = thisEnd;
	

	//std::cout << pRotMatF << std::endl;
	/*
	std::cout << pThetaX << " ";
	std::cout << pThetaY << " ";
	std::cout << pThetaZ << " " << std::endl;
	*/

	//length of node and angle from vertical.
	float a, b, g;
	a = degRad(end->euler[0]);
	b = degRad(end->euler[1]);
	g = degRad(end->euler[2]);

	Matrix rotMat = Matrix(3,3);
	Matrix tRotMat = Matrix(3,3);
	rotMat = fillRotMat(a,b,g);
	ColumnVector lenVect = ColumnVector(3);
	ColumnVector posVect = ColumnVector(3);

	lenVect(0) = end->length[0];
	lenVect(1) = end->length[1];
	lenVect(2) = end->length[2];

	std::cout << (bool)(mx_el_eq(pRotMat, 0) == boolMatrix(3,3,1)) << std::endl;
	if (mx_el_eq(pRotMat, 0) == boolMatrix(3,3,1)) {
		tRotMat = rotMat;
	} else {
		tRotMat = rotMat*pRotMat;
	}
	std::cout << tRotMat << std::endl;

	posVect = tRotMat*lenVect;

	//std::cout << radDeg(a) << " " << radDeg(b) << " " << radDeg(g)  << std::endl;
	//std::cout << rotMat << std::endl;
	//std::cout << posVect << std::endl;


	float h = sqrt(end->length[0]*end->length[0] + end->length[1]*end->length[1] + end->length[2]*end->length[2]);
	float h2 = sqrt(posVect(0)*posVect(0) + posVect(1)*posVect(1) + posVect(2)*posVect(2));

	//assert(h == h2);

	//float theta = atan2(end->length[0], end->length[1]);

	//std::cout << end->name << " h = " << h << ", theta = " << radDeg(theta) << std::endl;

	pos[0] += posVect(0);
	pos[1] += posVect(1);
	pos[2] += posVect(2);

	//Simple Trig to get the end vector position of this node.
	//pos[0] += h*-sin(theta + degRad(end->euler[0]+pThetaX));
	//pos[1] += h*cos(theta + degRad(end->euler[0]+pThetaX));
	//pos[2] += 0; 

	//std::cout << end->name << " End Pos = " << pos[0] << ", " << pos[1] << std::endl;

	return;
}

void transpose(float*, int, int, float*);
bool mult(float* A, int m1, int n1, float* B, int m2, int n2, float* res);
/*
void jacobian(NODE *node)
{
	float errorTolerance = 30; 
	float closeTol = 0.05;
	ColumnVector TH = ColumnVector(noofnodes);
	//Matrix W = Matrix(noofnodes, noofnodes).fill(0);
	ColumnVector W = ColumnVector(noofnodes);
	Matrix S = Matrix(3, noofnodes);
	Matrix V = Matrix(3, noofnodes);
	Matrix J = Matrix(3, noofnodes);
	Matrix dX = Matrix(3, noofnodes);
	NODE *startNode= node;

	NODE *end = getEndEffector(node);
	int m = 0;
	while (node)
	{
		TH(m) = node->euler;
		//W(m,m) = node->weight;
		W(m) = node->weight;

		double epos[2]; epos[0] = 0; epos[1] = 0;
		double ppos[2]; ppos[0] = 0; ppos[1] = 0;
		double endpos[2]; endpos[0] = 0; endpos[1] = 0;
		calcEndPos(node, epos);
		if (node->parent) { calcEndPos(node->parent, ppos); }
		calcEndPos(end, endpos);

		//dX = distance from target to end effector
		dX(0, m) = IKPosX - endpos[0];   //Minimise dX
		dX(1, m) = IKPosY - endpos[1];
		dX(2, m) = 0;
		//if (sqrt((dX(0,m)*dX(0,m)) + (dX(1,m)*dX(1,m))) < closeTol) { return; }

		//S = endPosition of current node
		S(0, m) = epos[0];
		S(1, m) = epos[1];
		S(2, m) = 0;

		//Fake Cross product to fill the Jacobian
		J(0,m) = (IKPosY - S(1, m)) ;
		J(1,m) = -(IKPosX - S(0, m)) ;
		J(2, m) = 0;

		//std::cout << "V" << V << std::endl; 
		//std::cout << "J" << J << std::endl; 

		m++;
		if (node->child) { node = node->child; }
		else { node = NULL; } 
	}

	//Calculate Distance of end effector to Target. If close, Stop.
	std::cout << "dist = " << sqrt((dX(0, 0)*dX(0,0)) + (dX(1,0)*dX(1,0))) << std::endl;
	if (sqrt((dX(0,0)*dX(0,0)) + (dX(1,0)*dX(1,0))) < closeTol) { return; }
	
	
	
	//If error is small, stop iterating
	//If large, halve dX
	Matrix error = Matrix(noofnodes, noofnodes);
	while ( sqrt(error.sumsq().sum(1)(0, 0)) > errorTolerance)
	{
		error = (identity_matrix(2,2) - (J*J.pseudo_inverse())) * dX;
		dX = quotient(dX, Matrix(dX.rows(),dX.cols()).fill(2.0));
	}

	ColumnVector NewTH = ColumnVector(noofnodes);
	NewTH = TH + ((J.pseudo_inverse()*dX).column(0));
	

	//Update node angles.
	node = startNode;
	for (int i =0; i<NewTH.length(); i++)	
	{
		if (node != NULL)
		{
			node->euler = NewTH(i);
			while (node->euler > 360) { node->euler -= 360; }
			while (node->euler < -360) { node->euler += 360; }
			node = node->child;
		} else { std::cout << "Error: Array too long for chain" << std::endl; }
	}
	return;
}
*/

void CCD(NODE *cur)
{
	if (!cur) { return; }
	NODE *end = getEndEffector(nodeList[0]);
	NODE *oEnd = end;
	float epos[3]; epos[0] = 0; epos[1] = 0; epos[2] = 0;
	float jpos[3]; jpos[0] = 0; jpos[1] = 0; jpos[2] = 0;
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


	float ej[2]; ej[0] = epos[0] - jpos[0]; ej[1]= epos[1] - jpos[1];
	float tj[2]; tj[0] = IKPosX - jpos[0];  tj[1] = IKPosY - jpos[1];

	//std::cout << "ej: = " << ej[0] << ", " << ej[1] << std::endl;
	//std::cout << "tj: = " << tj[0] << ", " << tj[1] << std::endl;

	float ejdottj = ej[0]*tj[0] + ej[1]*tj[1];
	float minejdottj = ej[0]*tj[1] - ej[1]*tj[0];
	float ejSqr = sqrt(ej[0]*ej[0] + ej[1]*ej[1]);
	float tjSqr = sqrt(tj[0]*tj[0] + tj[1]*tj[1]);

	if (tjSqr < 0.05) { return; }
	float cosA = ejdottj/(ejSqr*tjSqr);
	float sinA = minejdottj/(ejSqr*tjSqr);

	//Limit cosA - eliminates rounding errors.
	cosA = cosA>-1?cosA:-1;
	cosA = cosA<1?cosA:1;
	float rotAng = acos(cosA);
	if (rotAng < 0.01 ) { return; }
	if (rotAng > 10) { rotAng = 10; }
	if (sinA < 0) { rotAng = -rotAng; }
	rotAng *= cur->weight;
	cur->euler[0] += radDeg(rotAng);

	if (doLimit)
	{
		while (cur->euler[0] < -80 ) { cur->euler[0] = -80; }
		while (cur->euler[0] >  80 ) { cur->euler[0] =  80; }
	} else {
		//Normalise to between +- 360
		if (cur->euler[0] > 2*PI) { cur->euler[0] -= 2*PI; } else if (cur->euler[0] < -2*PI) { cur->euler[0] += 2*PI; }
	}

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

	for (int i=0; i<nooftests; i++)
	{
		std::cout << testList[i]->name << std::endl;
		if (testList[i]->parent == NULL)
		{
			evaluateChain(testList[i]);
		}
		float pos[3];
		pos[0] = 0; pos[1] = 0; pos[2] = 0;
		calcEndPos(testList[i], pos);
		glPushMatrix();
		 glTranslatef(pos[0], pos[1], pos[2]);
		 glutSolidSphere(0.1, 5, 5);
		glPopMatrix();
	}
	
	//evaluateChain(nodeList[0]);

	if (doJac)
	{
		//jacobian(nodeList[0]);
	} else if (false) {
		NODE *node = getEndEffector(nodeList[0]);
		float pos[3];
		while (node)
		{
			pos[0] = 0; pos[1] = 0; pos[2] = 0;
			calcEndPos(node, pos);
			glPushMatrix();
			 glTranslatef(pos[0], pos[1], pos[2]);
			 glutSolidSphere(0.1, 5, 5);
			glPopMatrix();
			//CCD(node);
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
}


int main (int argc, char **argv) {
	for (int i=0; i<256; i++) { keyStates[i] = false; keySpecialStates[i] = false; }
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
