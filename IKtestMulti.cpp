//g++ IKtestMulti.cpp -o IKtestMulti -lGL -lGLU -lglut && ./IKtestMulti

//#include <GL/glew.h>
#include <GL/glut.h>

#include <octave/oct.h>
#include <iostream>
#include <cmath>

bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[256];

const float PI = 3.14159265f;

float atX = 0, atZ = 0; float atY = 2;
float eyeX = 0; float eyeY = 2.1f; float eyeZ = 7;

bool doLimits = true;
bool doJac = true;

GLfloat whiteSpecularMaterial[] = { 1.0, 1.0, 1.0};
GLfloat whiteSpecularLight[] = { 1.0, 1.0, 1.0};
GLfloat blackAmbientLight[] = { 0.0, 0.0, 0.0};
GLfloat whiteDiffuseLight[] = { 1.0, 1.0, 1.0};
GLfloat blankMaterial[] = { 0.0, 0.0, 0.0};
GLfloat mShininess[] = {128};

struct TARGET;

struct NODE 
{
	const char *name;
	float length[2];
	float euler;
	float weight;
	NODE *parent;
	int noofchildren;
	NODE **child;
	TARGET *target;
};

struct TARGET
{
	const char *name;
	float pos[2];
	NODE *end;
};

int noofnodes = 0;
int nooftargets = 0;
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
	if (keyStates[0x1b])  	{ std::cout << "Quitting" << std::endl; exit(0); }
	if (keyStates['a'])  	{ targetList[1]->pos[0] -= d; }
	if (keyStates['d']) 	{ targetList[1]->pos[0] += d; }
	if (keyStates['s'])  	{ targetList[1]->pos[1] -= d; }
	if (keyStates['w'])   	{ targetList[1]->pos[1] += d; }

	if (keyStates['j'])  	{ targetList[0]->pos[0] -= d; }
	if (keyStates['l']) 	{ targetList[0]->pos[0] += d; }
	if (keyStates['k'])  	{ targetList[0]->pos[1] -= d; }
	if (keyStates['i'])   	{ targetList[0]->pos[1] += d; }

	if (keyStates[' '])		{ doLimits ^= 1; }
	if (keyStates['t'])		{ doJac ^= 1; }

}
void keyUp (unsigned char key, int x, int y) { keyStates[key] = false; }

void mouseFunc (int button, int state, int x, int y)
{

}
float radDeg (float rad) { return rad*180/PI; }
float degRad (float rad) { return rad*PI/180; }


void evaluateChain(NODE* seg)
{
	if (seg == NULL ) { return; }
	//std::cout << seg->name << std::endl;
	glPushMatrix();
		glColor3f(0.4f, 0, 0);
		glRotatef(seg->euler, 0, 0, 1);
		glBegin(GL_LINES);
			glVertex3f(0, 0, 0);
			glVertex3f(seg->length[0], seg->length[1], 0);
		glEnd();
		glTranslatef(seg->length[0], seg->length[1], 0);

	//if (seg->child) { evaluateChain(seg->child); }
	NODE *oseg = seg;
	for (int i=0; i< seg->noofchildren; i++)
	{
		seg = oseg;
		if (seg->child[i] != NULL) 
		{
			//std::cout << "Evaluating "<< seg->name << " Child " << i << " = " << seg->child[i]->name << std::endl;
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
	NODE *n3b = new NODE;
	NODE *n4 = new NODE;
	NODE *n4b = new NODE;
	//NODE *n5b = new NODE;

	n1->noofchildren = 1; n1->target = NULL;
	n2->noofchildren = 2; n2->target = NULL;
	n3->noofchildren = 1; n3->target = NULL;
	n3b->noofchildren = 1; n3b->target = NULL;
	n4->noofchildren = 0; n4->target = NULL;
	n4b->noofchildren = 0; n4b->target = NULL;
	//n5b->noofchildren = 0; n5b->target = NULL;

	std::cout << "Setting up links" << std::endl;

	increaseChildren(n1, n1->noofchildren);
	increaseChildren(n2, n2->noofchildren);
	increaseChildren(n3, n3->noofchildren);
	increaseChildren(n3b, n3b->noofchildren);
	increaseChildren(n4, n4->noofchildren);
	increaseChildren(n4b, n4b->noofchildren);
	//increaseChildren(n5b, n5b->noofchildren);

	n1->name = "root"; n1->child[0] = n2; n1->parent = NULL;
	n2->name = "lower"; n2->child[0] = n3; n2->child[1] = n3b; n2->parent = n1;
	n3->name = "upper"; n3->child[0] = n4; n3->parent = n2;
	n3b->name = "LUPPER"; n3b->child[0] = n4b; n3b->parent = n2;
	n4->name = "hand";  n4->child[0] = NULL; n4->parent = n3; 
	n4->target = targetList[1];
	n4b->name = "LHAND";  n4b->child[0] = NULL; n4b->parent = n3b;
	n4b->target = targetList[0];


	std::cout << "Setting chain Lens" << std::endl;

	n1->length[0] = 0; n1->length[1] = 1;  
	n2->length[0] = 0; n2->length[1] = 1;  
	n3->length[0] = 0; n3->length[1] = 1;  
	n3b->length[0] = 0; n3b->length[1] = 1;  
	n4->length[0] = 0; n4->length[1] = 1;  
	n4b->length[0] = 0; n4b->length[1] = 1;  
	//n5b->length[0] = 0; n5b->length[1] = 1;  
	//std::cout << "n1 Setup" << std::endl;

	n1->euler = 0; n2->euler = 0;
	n3->euler = 0; n3b->euler = 0;
	n4->euler = 0; n4b->euler = 0;
	
	n1->weight = 1; n2->weight = 1;
	n3->weight = 1; n3b->weight = 1;
	n4->weight = 1; n4b->weight = 1;
	nodeList[noofnodes++] = n1; 
	nodeList[noofnodes++] = n2; 
	nodeList[noofnodes++] = n3; 
	nodeList[noofnodes++] = n3b;
	nodeList[noofnodes++] = n4; 
	nodeList[noofnodes++] = n4b;
}

void setupChainUnbalanced()
{
	std::cout << "Setting up Chain" << std::endl;
	NODE *n0 = new NODE;
	NODE *n1 = new NODE;
	NODE *n2 = new NODE;
	NODE *n3 = new NODE;
	NODE *n4 = new NODE;
	NODE *n4b = new NODE;
	NODE *n3b = new NODE;
	NODE *n5 = new NODE;
	//NODE *n5b = new NODE;

	n0->noofchildren = 1; n0->target = NULL;
	n1->noofchildren = 2; n1->target = NULL;
	n2->noofchildren = 1; n2->target = NULL;
	n3->noofchildren = 1; n3->target = NULL;
	n3b->noofchildren = 1; n3b->target = NULL;
	n4->noofchildren = 1; n4->target = NULL;
	n4b->noofchildren = 0; n4b->target = NULL;
	n5->noofchildren = 0; n5->target = NULL;
	//n5b->noofchildren = 0; n5b->target = NULL;

	std::cout << "Setting up links" << std::endl;

	increaseChildren(n0, n0->noofchildren);
	increaseChildren(n1, n1->noofchildren);
	increaseChildren(n2, n2->noofchildren);
	increaseChildren(n3, n3->noofchildren);
	increaseChildren(n3b, n3b->noofchildren);
	increaseChildren(n4, n4->noofchildren);
	increaseChildren(n4b, n4b->noofchildren);
	increaseChildren(n5, n5->noofchildren);
	//increaseChildren(n5b, n5b->noofchildren);

	n0->name = "preRoot"; n0->child[0] = n1; n1->parent = NULL;
	n1->name = "root"; n1->child[0] = n2; n1->child[1] = n3b; n1->parent = n0;
	n2->name = "lower"; n2->child[0] = n3; n2->parent = n1;
	n3->name = "upper"; n3->child[0] = n4; n3->parent = n2;
	n3b->name = "LUPPER"; n3b->child[0] = n4b; n3b->parent = n1;
	n4->name = "hand";  n4->child[0] = n5; n4->parent = n3; 
	n4b->name = "LHAND";  n4b->child[0] = NULL; n4b->parent = n3b;
	n4b->target = targetList[1];

	n5->name = "tip";  n5->child[0] = NULL; n5->parent = n4; n5->target = targetList[0];
	//n5b->name = "LTIP";  n5b->child[0] = NULL; n5b->parent = n4b; n5b->target = targetList[1];

	std::cout << "Setting chain Lens" << std::endl;

	n0->length[0] = 0; n0->length[1] = 1;  
	n1->length[0] = 0; n1->length[1] = 1;  
	n2->length[0] = 0; n2->length[1] = 1;  
	n3->length[0] = 0; n3->length[1] = 1;  
	n3b->length[0] = 0; n3b->length[1] = 1;  
	n4->length[0] = 0; n4->length[1] = 1;  
	n4b->length[0] = 0; n4b->length[1] = 1;  
	n5->length[0] = 0; n5->length[1] = 1;  
	//n5b->length[0] = 0; n5b->length[1] = 1;  
	//std::cout << "n1 Setup" << std::endl;
	//
	/*
	n1->euler = 45;
	n2->euler = 30;
	n3->euler = -30;
	n4->euler = 30;
	*/

	n0->euler = 0;
	n1->euler = 0; n2->euler = 0;
	n3->euler = 0; n3b->euler = 0;
	n4->euler = 0; n4b->euler = 0;
	n5->euler = 0; //n5b->euler = 0;

	n1->weight = 1;
	n1->weight = 1; n2->weight = 1;
	n3->weight = 1; n3b->weight = 1;
	n4->weight = 1; n4b->weight = 1;
	n5->weight = 1; //n5b->weight = 1;
	
	
	nodeList[noofnodes++] = n0; 
	nodeList[noofnodes++] = n1; 
	nodeList[noofnodes++] = n2; 
	nodeList[noofnodes++] = n3; 
	nodeList[noofnodes++] = n3b;
	nodeList[noofnodes++] = n4; 
	nodeList[noofnodes++] = n4b;
	nodeList[noofnodes++] = n5; 
	//nodeList[noofnodes++] = n5b;
	//std::cout << "LEaving Exit" << std::endl;

	return;
}

void getEndEffector(NODE *cur, int *noofends, NODE **retList)
{
	if (!cur) { std::cout << "getEndEffector null node" << std::endl; return; }
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
		getTarget(cur->child[i], nooftargets, retList);
	}
}

void CCD(NODE *cur)
{
	//std::cout << cur->name << std::endl;
	if (!cur) { return; }
	NODE **eList; int noofends= 0;
	eList = (NODE**) malloc(sizeof(NODE*) * nooftargets);
	//NODE *oEnd = end;
	getEndEffector(cur, &noofends, eList);
	float epos[2]; epos[0] = 0; epos[1] = 0;
	float jpos[2]; jpos[0] = 0; jpos[1] = 0;
	float eposS[2];
	for (int i=0; i<noofends; i++)
	{
		eposS[0] = 0; eposS[1] = 0;
		calcEndPos(eList[i], eposS);
		epos[0] += eposS[0];
		epos[1] += eposS[1];
	}
	free(eList);
	epos[0] /= noofends;
	epos[1] /= noofends;
	//std::cout << "e: " << cur->name << " Pos = " << epos[0] << ", " << epos[1] << std::endl;

	//Get rotation point of current joint - it's parent's end - it's start
	if (cur->parent)
	{
		calcEndPos(cur->parent, jpos);
	}
	
	float ej[2]; ej[0] = epos[0] - jpos[0]; ej[1]= epos[1] - jpos[1];
	float tj[2]; tj[0] = 0; tj[1] = 0;
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
		}

		tj[0] /= nooftar; 
		tj[1] /= nooftar; 

		/*
		glColor3f(1.0f/nooftar, 0, 0 );
		glPushMatrix();
			glTranslatef(tj[0], tj[1], 0);
			glutWireSphere(0.1f, 5, 5);
		glPopMatrix();
		*/
		tj[0] -= jpos[0]; 
		tj[1] -= jpos[1];

	} else {
		free(tList);
	   	return;
   	}
	free(tList);


	//std::cout << cur->name << " ";
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
	if (sinA < 0) { rotAng = -rotAng; }
	cur->euler += radDeg(rotAng);

	//Normalise to between +- 360

	if (doLimits)
	{
		if (cur->euler > 80) { cur->euler = 80; }
		if (cur->euler < -80) { cur->euler = -80; }
	} else {

		if (cur->euler > 360) { cur->euler -= 360; } else if (cur->euler < -360) { cur->euler += 360; }
	}

}

float distToTarget(NODE *node)
{
	float a, b, c;
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(node, pos);

	TARGET **tList; int nooftar= 0;
	tList = (TARGET**) malloc(sizeof(TARGET*) * nooftargets);
	getTarget(node, &nooftar, tList);
	for (int i=0; i<nooftar; i++)
	{
		std::cout << tList[i]->name << std::endl;
		b += (pos[0] - tList[i]->pos[0]);
		c += (pos[1] - tList[i]->pos[1]);
	}
	b/=nooftar;
	c/=nooftar;

	a = sqrt(b*b + c*c);
	return a;
}

void fillJ(ColumnVector *TH, ColumnVector *W, Matrix *dX, Matrix *J, NODE *node, int *m)
{
	if (!node) { return; }
	std::cout << "m " << *m << " node: "<< node->name << std::endl;
	(*TH)(*m) = node->euler;
	(*W)(*m) = node->weight;

	float epos[2]; epos[0] = 0; epos[1] = 0;
	float ppos[2]; ppos[0] = 0; ppos[1] = 0;
	calcEndPos(node, epos);
	if (node->parent) { calcEndPos(node->parent, ppos); }

	NODE **eList; int noofends = 0;
	eList = (NODE**) malloc(sizeof(NODE*) *nooftargets);
	getEndEffector(node, &noofends, eList);
	float endposS[2]; endposS[0] = 0; endposS[1] = 0;
	float tarposS[2]; tarposS[0] = 0; tarposS[1] = 0;
	for (int i=0; i<noofends; i++)
	{
		float endpos[2]; endpos[0] = 0; endpos[1] = 0;
		calcEndPos(eList[i], endpos);
		endposS[0] += endpos[0];
		endposS[1] += endpos[1];

		if (eList[i]->target)
		{
			tarposS[0] += eList[i]->target->pos[0];
			tarposS[1] += eList[i]->target->pos[1];
		} 
	}
	
	endposS[0] /= noofends;
	endposS[1] /= noofends;
						 
	tarposS[0] /= noofends;
	tarposS[1] /= noofends;
	

	glColor3f(0, 1.0/noofends, 0);
	glPushMatrix();
		glTranslatef(tarposS[0], tarposS[1], 0);
		glutSolidSphere(0.1, 7, 7);
	glPopMatrix();

	glColor3f(0, 0, 1.0/noofends);
	glPushMatrix();
		glTranslatef(endposS[0], endposS[1], 0);
		glutSolidSphere(0.1, 7, 7);
	glPopMatrix();


	//dX = distance from target to end effector
	(*dX)(0, *m) = tarposS[0] - endposS[0];   //Minimise dX
	(*dX)(1, *m) = tarposS[1] - endposS[1];
	//
	//Fake Cross product to fill the Jacobian
	(*J)(0, *m) = -(endposS[1] - ppos[1]) ;
	(*J)(1, *m) =  (endposS[0] - ppos[0]) ;


	(*m)++;
	for (int i=0; i<node->noofchildren; i++)
	{
		fillJ(TH, W, J, dX, node->child[i], m);
	}
	return;
}

void extractAngles(ColumnVector *NewTH, NODE *node, int *n)
{
	if (!node) { std::cout << "extractAngles null node" << std::endl; return; }
		//std::cout << "n " << *n << " node: " << node->name << std::endl;
		node->euler = (*NewTH)(*n);
		if (doLimits)
		{
			if (node->euler < -80 ) { node->euler = -80; }
			if (node->euler >  80 ) { node->euler =  80; }
		} else {
			//Normalise to between +- 360
			while (node->euler > 360) { node->euler -= 360; }
			while (node->euler < -360) { node->euler += 360; }
		}
	(*n)++;
	for (int i=0; i<node->noofchildren; i++)
	{
		extractAngles(NewTH, node->child[i], n);
	}
}



bool jacobian(NODE *node)
{
	float errorTolerance = 10; 
	float closeTol = 0.05;
	ColumnVector TH = ColumnVector(noofnodes).fill(0);
	//Matrix W = Matrix(noofnodes, noofnodes).fill(0);
	ColumnVector W = ColumnVector(noofnodes).fill(1);
	Matrix J = Matrix(2, noofnodes).fill(0);
	Matrix dX = Matrix(2, noofnodes).fill(0);
	NODE *startNode= node;

	//recursively fill the jacobian matrix
	int m = 0;
	fillJ(&TH, &W, &J, &dX, node, &m);

	std::cout << std::endl;
	std::cout << TH.transpose() << "\n\n" << dX << "\n" << J <<std::endl;
		
	//Calculate Distance of end effector to Target. If close, Stop.
	//if (distToTarget(end) < closeTol) {
		 //return true;
	//}

	Matrix JJ = J*J.pseudo_inverse();
	
	//If error is small, stop iterating
	//If large, halve dX
	
	Matrix error = Matrix(noofnodes, noofnodes);
	error = (identity_matrix(JJ.rows(),JJ.cols()) - (JJ)) * dX;
	while ( sqrt(error.sumsq().sum(1)(0, 0)) > errorTolerance)
	{
		//std::cout << "Error " << sqrt(error.sumsq().sum(1)(0, 0)) << std::endl;
		error = (identity_matrix(JJ.rows(),JJ.cols()) - (JJ)) * dX;
		dX = quotient(dX, Matrix(dX.rows(),dX.cols()).fill(2.0));
	}
	

	ColumnVector NewTH = ColumnVector(noofnodes);
	NewTH = TH + ((J.pseudo_inverse()*dX*W));

	std::cout <<"NewTH " << NewTH.transpose() << std::endl;
	
	//std::cout << NewTH << std::endl;
	//Update node angles.
	node = startNode;
	int n = 0;
	extractAngles(&NewTH, node, &n);
	
	//std::cout << "Nodes Updated" << std::endl;
	return false;
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
	glutSolidSphere(0.1, 17, 17);

	for (int i=0; i < nooftargets; i++)
	{
		glPushMatrix();
			glColor3f(0, 1, 0);
			glTranslatef(targetList[i]->pos[0], targetList[i]->pos[1], 0);
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
	//CCD(nodeList[8]);
	if (doJac)
	{
		jacobian(nodeList[0]);
	} else {
		CCD(nodeList[7]);
		CCD(nodeList[6]);
		CCD(nodeList[5]);
		CCD(nodeList[4]);
		CCD(nodeList[3]);
		CCD(nodeList[2]);
		CCD(nodeList[1]); 
		CCD(nodeList[0]);
	}
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
	t1->pos[0] = 2; t1->pos[1] = 3;

	t2->name = "LHandTarget";
	t2->pos[0] = -2; t2->pos[1] = 3;

	targetList[nooftargets++] = t1;
	targetList[nooftargets++] = t2;
}

int main (int argc, char **argv) {
	for (int i=0; i<256; i++) {
		keyStates[i] = false; keySpecialStates[i] = false; }
		
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("IK Demo - Y Shaped Chain");
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
