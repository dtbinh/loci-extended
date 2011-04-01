#include <cmath>
#include <octave/oct.h>
#include <iostream>

const float PI = 3.14159265f;
float radDeg (float rad) { return rad*180.0/PI; }
float degRad (float rad) { return rad*PI/180.0; }

int nooftargets = 0;
struct TARGET;
struct NODE 
{
	const char *name;
	float length[3];
	float euler[3];
	float weight;
	NODE *parent;
	NODE **child;
	int noofchildren;
	TARGET *target;
};

struct TARGET
{
	const char *name;
	float pos[3];
	NODE *end;
};

void getEndEffector(NODE *cur, int *noofends, NODE **retList)
{
	if (cur->noofchildren == 0)
	{
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
	if (seg->child[0])
	{
		getFirstEndEffector(seg->child[0]);
	}
	else
	{
		return seg;
	}
}

Matrix fillRotMat(float a, float b, float g)
{
	Matrix rotMat = Matrix(3,3); rotMat.fill(0);
	//XYZ Matrix - Wikipedia?
	
	
	rotMat(0,0) = cos(b)*cos(g);
	rotMat(0,1) = -cos(a)*sin(g) + sin(a)*sin(b)*cos(g);;
	rotMat(0,2) = sin(a)*sin(g) + cos(a)*sin(b)*cos(g);

	rotMat(1,0) = cos(b)*sin(g);
	rotMat(1,1) = cos(a)*cos(g) + sin(a)*sin(b)*sin(g);
	rotMat(1,2) = -sin(a)*cos(g) + cos(a)*sin(b)*sin(g);

	rotMat(2,0) = -sin(b);
	rotMat(2,1) = sin(a)*cos(b);
	rotMat(2,2) = cos(a)*cos(b);
	
	

	//XYZ website
	
	/*
	Matrix xMat = Matrix(3,3); xMat.fill(0);
	Matrix yMat = Matrix(3,3); xMat.fill(0);
	Matrix zMat = Matrix(3,3); xMat.fill(0);

	xMat(0,0) = 1;
	xMat(1,1) =  cos(a);
	xMat(1,2) = -sin(a);
	xMat(2,1) =  sin(a);
	xMat(2,2) =  cos(a);

	yMat(0,0) =  cos(a);
	yMat(0,2) =  sin(a);
	yMat(1,1) = 1;
	yMat(2,0) = -sin(a);
	yMat(2,2) =  cos(a);

	zMat(0,0) =  cos(a);
	zMat(0,1) = -sin(a);
	zMat(1,0) =  sin(a);
	zMat(1,1) =  cos(a);
	zMat(2,2) = 1;


	rotMat = xMat*yMat*zMat; 
	*/
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
	/*
	while ((end->parent != NULL)) { 
		end = end->parent;
	}
	*/

	do 
	{
		Matrix pRotMatT = Matrix(3,3).fill(0);
		pRotMatT = fillRotMat(degRad(end->euler[0]), degRad(end->euler[1]), degRad(end->euler[2]));
		if ((mx_el_eq(pRotMat, 0) == boolMatrix(3,3,1))){
		   	pRotMatF = pRotMatT; 
		} else {
			pRotMatF = pRotMatT *pRotMat;
		}
		pRotMat = pRotMatF;

		if (end->parent)
		 { end = end->parent;	}
		else { end = NULL; }
	} while (end);
	end = thisEnd;

	ColumnVector lenVect = ColumnVector(3);
	ColumnVector posVect = ColumnVector(3);

	lenVect(0) = end->length[0];
	lenVect(1) = end->length[1];
	lenVect(2) = end->length[2];

	posVect = pRotMatF*lenVect;


	float h = sqrt(end->length[0]*end->length[0] + end->length[1]*end->length[1] + end->length[2]*end->length[2]);
	float h2 = sqrt(posVect(0)*posVect(0) + posVect(1)*posVect(1) + posVect(2)*posVect(2));

	//assert(h == h2);
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

/*
	if (cur->euler[0] > 80) { cur->euler[0] = 80; }
	if (cur->euler[0] < -80) { cur->euler[0] = -80; }
	if (cur->euler[1] > 80) { cur->euler[1] = 80; }
	if (cur->euler[1] > 80) { cur->euler[1] = 80; }
	if (cur->euler[2] < -80) { cur->euler[2] = -80; }
	if (cur->euler[2] < -80) { cur->euler[2] = -80; }
*/
}
