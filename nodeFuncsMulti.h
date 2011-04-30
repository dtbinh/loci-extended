#ifndef _NODEFUNCSMULTI_H_
#define _NODEFUNCSMULTI_H_
#include <cmath>

#include <octave/oct.h>
#include <map>
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
	float offset[3];
	float weight;
	NODE *parent;
	NODE **child;
	int noofchildren;
	TARGET *target;
	float limXmin; float limXmax;
	float limYmin; float limYmax;
	float limZmin; float limZmax;
};

typedef std::map<NODE*, float*> NodeAxisMap;
NodeAxisMap nodeAxisMap;

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
	if (seg->noofchildren >= 1)
	{
		getFirstEndEffector(seg->child[0]);
	}
	else
	{
		return seg;
	}
}

TARGET* getFirstTarget(NODE *cur)
{
	//std::cout << "G-t for " << cur->name << " - " << std::flush;
	//std::cout << "nooftargets:" << *nooftargets << " " << std::flush;
	
	if (cur->target != NULL) {
		return cur->target;
   	} else {
		if (cur->child[0])
		{
			getFirstTarget(cur->child[0]);
		} else { return NULL; }
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

void radAngleAxisRot(float rotAng, float *axis, float *eulers)
{
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
		//std::cout << "sing NORTH" << std::endl;
		 //Singluarity at north Pole
		 y = atan2(axisMat(0,2), axisMat(2,2));
		 x = PI/2;
		 z = 0;
	} else if (axisMat(1,0) < -0.998)
	{
		//std::cout << "sing SOUTH" << std::endl;
		//Singularit at south pole
		y = atan2(axisMat(0,2), axisMat(2,2));
		x = -PI/2;
		z = 0;
	} else {
		y = atan2(-axisMat(2,0), axisMat(0,0));
		x = atan2(-axisMat(1,2), axisMat(1,1));
		z = asin(-axisMat(1,0));
	}

	eulers[0] = x;
	eulers[1] = y;
	eulers[2] = z;

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

	Matrix pRotMatF = Matrix(3,3).fill(0);
	Matrix pRotMat = Matrix(3,3).fill(0);

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
	pos[0] += posVect(0) + end->offset[0];
	pos[1] += posVect(1) + end->offset[1];
	pos[2] += posVect(2) + end->offset[2];

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
	tList = (TARGET**) malloc(sizeof(TARGET*) * nooftargets*3);
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

		glColor3f(1.0f/nooftar, 1.0f/nooftar, 0 );
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
	//std::cout << "ej: = " << ej[0] << ", " << ej[1] << " " << ej[2] << std::endl;
	//std::cout << "tj: = " << tj[0] << ", " << tj[1] << " " << tj[2] << std::endl;

	float ejdottj = ej[0]*tj[0] + ej[1]*tj[1] + ej[2]*tj[2];
	float minejdottj = - ej[0]*tj[0] - ej[1]*tj[1] - ej[2]*tj[2];
	float ejSqr = sqrt(ej[0]*ej[0] + ej[1]*ej[1] + ej[2]*ej[2]);
	float tjSqr = sqrt(tj[0]*tj[0] + tj[1]*tj[1] + tj[2]*tj[2]);

	//std::cout << "ejdottj " << ejdottj << std::endl;

	if (tjSqr < 0.05) { return; }
	float cosA = ejdottj/(ejSqr*tjSqr);

	//Axis = axis to rotate around. CrossProduct of ej+tj
	float axis[3];
	axis[0] = (ej[1]*tj[2])-(ej[2]*tj[1]);
	axis[1] = (ej[2]*tj[0])-(ej[0]*tj[2]);
	axis[2] = (ej[0]*tj[1])-(ej[1]*tj[0]);

	//Normalise Rotation axis
	float lenAxis = sqrt((axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2]));
	float sinA = minejdottj/(ejSqr*tjSqr);
	axis[0] /= lenAxis;
	axis[1] /= lenAxis;
	axis[2] /= lenAxis;

	//A pervious iteration saved the axis of rotation. If it is similar to the new one, use the previously saved one.
	//TODO Check for benefits
	if (nodeAxisMap[cur]) 
	{ 
		float axisO[3];
		axisO[0] = nodeAxisMap[cur][0];
	   	axisO[1] = nodeAxisMap[cur][1]; 
	   	axisO[2] = nodeAxisMap[cur][2];

		//std::cout << axis[0] << axis[1] << axis[2] << std::endl;
		//std::cout << axisO[0] << axisO[1] << axisO[2] <<  " ";
		float adotao = axis[0]*axisO[0] + axis[1]*axisO[1] + axis[2]*axisO[2];
		//std::cout << adotao ;  
		adotao = adotao>-1?adotao:-1;
		adotao = adotao<1?adotao:1;

		//acos(0.984) = 10 degrees?
		if (adotao > 0.984) 
		{
			//std::cout << "EQUAL" << std::endl;
			axis[0] = axisO[0];
			axis[1] = axisO[1];
			axis[2] = axisO[2];
		}
	}
	//Set this nodes's axis as the axis for next time
	nodeAxisMap[cur] = axis;

	//std::cout << axis[0] << " " << axis[1] << " " << axis[2] << std::endl;

	//Limit cosA - eliminates rounding errors.
	cosA = cosA>-1?cosA:-1;
	cosA = cosA<1?cosA:1;
	//std::cout << cosA << std::endl;
	float rotAng = acos(cosA);
	if (rotAng < 0.01 ) { return; }
	if (sinA < 0) { rotAng = -rotAng; }
	//std::cout << "rotAng " << rotAng << std::endl;

	//Rotate by acos(cosA) around axis
	//Convert to Euler Rotations angles
	//
	float eulers[3];
	radAngleAxisRot(rotAng, axis, eulers);
	
	cur->euler[0] += radDeg(eulers[0]);
	cur->euler[1] += radDeg(eulers[1]);
	cur->euler[2] += radDeg(eulers[2]);

	//std::cout << cur->euler[0] << " " << cur->euler[1] << " " << cur->euler[2] << std::endl;
	//Normalise to between +- 360
	
	if (cur->euler[0] > 360) { cur->euler[0] -= 360; } else if (cur->euler[0] < -360) { cur->euler[0] += 360; }
	if (cur->euler[1] > 360) { cur->euler[1] -= 360; } else if (cur->euler[1] < -360) { cur->euler[1] += 360; }
	if (cur->euler[2] > 360) { cur->euler[2] -= 360; } else if (cur->euler[2] < -360) { cur->euler[2] += 360; }

	

	
	float lim = 180;
	
	if ((cur->limXmax) &&(cur->euler[0] > cur->limXmax)) { cur->euler[0] = cur->limXmax; }
	if ((cur->limXmin) &&(cur->euler[0] < cur->limXmin)) { cur->euler[0] = cur->limXmin; }
	if ((cur->limYmax) &&(cur->euler[1] > cur->limYmax)) { cur->euler[1] = cur->limYmax; }
	if ((cur->limYmin) &&(cur->euler[1] < cur->limYmin)) { cur->euler[1] = cur->limYmin; }
	if ((cur->limZmax) &&(cur->euler[2] > cur->limZmax)) { cur->euler[2] = cur->limZmax; }
	if ((cur->limZmin) &&(cur->euler[2] < cur->limZmin)) { cur->euler[2] = cur->limZmin; }
	//if (cur->euler[0] < -lim) { cur->euler[0] = -lim; }
	//if (cur->euler[1] > lim) { cur->euler[1] = lim; }
	//if (cur->euler[1] < -lim) { cur->euler[1] = -lim; }
	//if (cur->euler[2] > lim) { cur->euler[2] = lim; }
	//if (cur->euler[2] < -lim) { cur->euler[2] = -lim; }
	
	//std::cout << cur->name << " " << cur->euler[0] << " " << cur->euler[1] << " " << cur->euler[2] << std::endl;


}

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

bool jacobianSing(NODE *node)
{
	float errorTolerance = 10; 
	float closeTol = 0.05;

	NODE *end = getFirstEndEffector(node);
	if (distToTarget(end) < closeTol) { return true; }

	int noofnodes = 4;
	Matrix TH = Matrix(3,noofnodes);
	//Matrix W = Matrix(noofnodes, noofnodes).fill(0);
	ColumnVector W = ColumnVector(noofnodes);
	Matrix S = Matrix(3, noofnodes);
	Matrix V = Matrix(3, noofnodes);
	Matrix J = Matrix(3, noofnodes);
	Matrix dX = Matrix(3, noofnodes);
	NODE *startNode= node;

	

	int m = 0;
	float axis[3*noofnodes];
	while (node)
	{
		TH(0,m) = node->euler[0];
		TH(1,m) = node->euler[1];
		TH(2,m) = node->euler[2];
		//W(m,m) = node->weight;
		//W(m) = node->weight;
		W(m) = 1;

		float epos[3]; epos[0] = 0; epos[1] = 0; epos[2] = 0;
		float ppos[3]; ppos[0] = 0; ppos[1] = 0; ppos[2] = 0;
		float endpos[3]; endpos[0] = 0; endpos[1] = 0; endpos[2] = 0;
		calcEndPos(node, epos);
		if (node->parent) { calcEndPos(node->parent, ppos); }
		calcEndPos(end, endpos);
		TARGET *tar = getFirstTarget(end);

		//dX = distance from target to end effector
		dX(0, m) = tar->pos[0] - endpos[0];   //Minimise dX
		dX(1, m) = tar->pos[1] - endpos[1];
		dX(2, m) = tar->pos[2] - endpos[2];
		
		float ej[3]; float tj[3];
		ej[0] = endpos[0] - ppos[0];
		ej[1] = endpos[1] - ppos[1];
		ej[2] = endpos[2] - ppos[2];


		tj[0] = tar->pos[0] - ppos[0];
		tj[1] = tar->pos[1] - ppos[1];
		tj[2] = tar->pos[2] - ppos[2];


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
		if (node->noofchildren > 0) {node = node->child[0]; }
		else { node = NULL; } 
	}

	//Calculate Distance of end effector to Target. If close, Stop.
	//std::cout << "dist = " << distToTarget(end) << std::endl;
	
	
	
	
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
	//ColumnVector W = ColumnVector(noofnodes).fill(1);
	//std::cout << J.pseudo_inverse()*dX << std::endl;
	NewTH = (J.pseudo_inverse()*dX*W);
	//std::cout << TH << " + " << NewTH << std::endl;
	

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
/*
			while (node->euler[0] > 360) { node->euler[0] -= 360; }
			while (node->euler[0] < -360) { node->euler[0] += 360; }
			while (node->euler[1] > 360) { node->euler[1] -= 360; }
			while (node->euler[1] > 360) { node->euler[1] -= 360; }
			while (node->euler[2] < -360) { node->euler[2] += 360; }
			while (node->euler[2] < -360) { node->euler[2] += 360; }
*/

if ((node->limXmax) &&(node->euler[0] > node->limXmax)) { node->euler[0] = node->limXmax; }
	if ((node->limXmin) &&(node->euler[0] < node->limXmin)) { node->euler[0] = node->limXmin; }
	if ((node->limYmax) &&(node->euler[1] > node->limYmax)) { node->euler[1] = node->limYmax; }
	if ((node->limYmin) &&(node->euler[1] < node->limYmin)) { node->euler[1] = node->limYmin; }
	if ((node->limZmax) &&(node->euler[2] > node->limZmax)) { node->euler[2] = node->limZmax; }
	if ((node->limZmin) &&(node->euler[2] < node->limZmin)) { node->euler[2] = node->limZmin; }

			if (node->noofchildren > 0) { 		node = node->child[0]; }
		} else { std::cout << "Error: Array too long for chain" << std::endl; }
	}
	return false;
}

#endif
