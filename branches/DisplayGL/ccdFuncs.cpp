#include "ccdFuncs.h"

#include <iostream>
#include <cmath>
#include <map>

float degRad(float deg) { return deg*PI/180.0; }
float radDeg(float rad) { return rad*180.0/PI; }

NODE *nodeList[20];
typedef std::map<NODE*, float*> NodeAxisMap;
NodeAxisMap nodeAxisMap;


//TODO Set this
bool doLimit = true;

/**
 * getEndEffector 
 * Given a NODE, an empty int and empty allocated list of NODEs
 * fill the int with the number of end effectors
 * fill the list with pointers to NODEs
 */
void getEndEffector(NODE *cur, int *noofends, NODE**retList)
{
	if (cur->noofchildren == 0)
	{
		retList[*noofends] = cur;
		*noofends += 1;
		return;
	} else {
		for (int i=0; i<cur->noofchildren; i++)
		{
			getEndEffector(cur->children[i], noofends, retList);
		}
	}
}

void transpose(float *J, int m, int n, float *JT)
{
	std::cout << "Transposing "<< m << "x" << n << std::endl;
	for (int i=0; i<m; i++)
	{
		for (int j=0; j<n; j++)
		{
			JT[(j*m)+i] = J[(i*n)+j];
		}
	}
}

bool mult(float* A, int m1, int n1, float* B, int m2, int n2, float* res)
{
	
	//float resA[m1*n2];
	//if (n1 != m2) { return false; }
	if ((m1 == n1) && (m2 == n2) && (m1 == m2)) {
		//Two Square Matrixis
		//assert(n1 == m2);
		for (int i=0; i<n1; i++)	
		{
			for (int j=0; j<n1; j++)
			{
				float sum = 0;
				//Index for results matrix
				int ind = (i*n1) + j;
				//std::cout << ind << " =";
				for (int k = 0; k< m1; k++)
				{
					//Index for matrix A and B
					int bi = k*n1 + j;
					int ai = i*m1 + k;
					//std::cout << " " << ai << bi << " -> (" << A[ai] << "*" << B[bi] << ")";
					sum += (A[ai] * B[bi]);
				}
				//std::cout << " = " << sum ;
				res[ind] = sum;
				//std::cout << std::endl;
			}
		}
	} else if ((m1 == n1) && (n1 == m2) && (n2 == 1)) {
		//Square Matrix * Column Vector
		for (int i= 0; i<n1; i++)
		{
			float sum = 0;
			for (int j=0; j<m1; j++)
			{
				int ai = i*m1+j;
				int bi = j;
				//std::cout << " " << ai << bi << " -> (" << A[ai] << "*" << B[bi] << ")";
				sum += A[ai]*B[bi];
			}
			//std::cout << std::endl;
			
			res[i] = sum;
		}
		
	}
	//std::cout << m1 << " " << n1 << " - " << m2 << " " << n2 << std::endl;
	//transpose(resA, 3, 3, res);
	/*
	for (int i=0; i<n1; i++)	
	{
		for (int j=0; j<n1; j++)
		{
			int sum = 0;
			int ind = (i*n1) + j;
			std::cout << ind << " =";
			for (int k = 0; k< n1; k++)
			{
				float a = A[k*n1 + j];
				float b = B[i*n1 + k];
				float ai = k*n1 + j;
				float bi = i*n1 + k;
				std::cout << " " << ai << bi  << " -> (" << a << "*" << b << ")";
				sum += (a * b);
			}
			res[ind] = sum;
			std::cout << std::endl;
		}
	}
	*/
	return true;
}



void fillRotMat(float a, float b, float g, float *rotMat)
{
	rotMat[(0*3)+0] = cos(b)*cos(g);
	rotMat[(0*3)+1] = -cos(a)*sin(g) + sin(a)*sin(b)*cos(g);;
	rotMat[(0*3)+2] = sin(a)*sin(g) + cos(a)*sin(b)*cos(g);

	rotMat[(1*3)+0] = cos(b)*sin(g);
	rotMat[(1*3)+1] = cos(a)*cos(g) + sin(a)*sin(b)*sin(g);
	rotMat[(1*3)+2] = -sin(a)*cos(g) + cos(a)*sin(b)*sin(g);

	rotMat[(2*3)+0] = -sin(b);
	rotMat[(2*3)+1] = sin(a)*cos(b);
	rotMat[(2*3)+2] = cos(a)*cos(b);
	
	return;
}



#ifdef OCTAVE
#include <octave/oct.h>

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
	
	return rotMat;
}
#endif

void radAngleAxisRot(float rotAng, float *axis, float *eulers)
{
	//Matrix axisMat = Matrix(3,3);
	float axisMat[3*3];
	axisMat[0*3+0] = 1+ (1-cos(rotAng))*(axis[0]*axis[0]-1);
	axisMat[0*3+1] = -axis[2]*sin(rotAng)+(1-cos(rotAng))*axis[0]*axis[1];
	axisMat[0*3+2] = axis[1]*sin(rotAng)+(1-cos(rotAng))*axis[0]*axis[2];

	axisMat[1*3+0] = axis[2]*sin(rotAng)+(1-cos(rotAng))*axis[0]*axis[1];
	axisMat[1*3+1] = 1 + (1-cos(rotAng))*(axis[1]*axis[1]-1);
	axisMat[1*3+2] = -axis[0]*sin(rotAng)+(1-cos(rotAng))*axis[1]*axis[2];
	
	axisMat[2*3+0] = -axis[1]*sin(rotAng)+(1-cos(rotAng))*axis[0]*axis[2];
	axisMat[2*3+1] = axis[0]*sin(rotAng)+(1-cos(rotAng))*axis[1]*axis[2];
	axisMat[2*3+2] = 1 + (1-cos(rotAng))*(axis[2]*axis[2]-1);

	float x, y, z;
	
	if (axisMat[1*3+0] > 0.998)
	{
		std::cout << "sing NORTH" << std::endl;
		 //Singluarity at north Pole
		 y = atan2(axisMat[0*3+2], axisMat[2*3+2]);
		 x = PI/2;
		 z = 0;
	} else if (axisMat[1*3+0] < -0.998)
	{
		std::cout << "sing SOUTH" << std::endl;
		//Singularit at south pole
		y = atan2(axisMat[0*3+2], axisMat[2*3+2]);
		x = -PI/2;
		z = 0;
	} else {
		y = atan2(-axisMat[2*3+0], axisMat[0*3+0]);
		x = atan2(-axisMat[1*3+2], axisMat[1*3+1]);
		z = asin(-axisMat[1*3+0]);
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
	//std::cout << "calcEndPos" << end->name << std::flush;
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
	
	//Matrix pRotMatF = Matrix(3,3).fill(0);
	//Matrix pRotMat = Matrix(3,3).fill(0);
	float pRotMatF[9];
	float pRotMat[9];
	for (int i=0; i<9; i++) { pRotMat[i] = 0; pRotMatF[i] = 0; }

	do 
	{
		//Matrix pRotMatT = Matrix(3,3).fill(0);
		float pRotMatT[9]; 
		if (isnan(end->euler[0]) || isnan(end->euler[1]) || isnan(end->euler[2])) { return; } 
		//std::cout << end->euler[0] << " " << end->euler[1] << " " << end->euler[2] << std::endl;
		//std::cout << end->name << "degRads " << degRad(end->euler[0]) << " " << degRad(end->euler[1]) << " " << degRad(end->euler[2]) << std::endl;
		fillRotMat(degRad(end->euler[0]), degRad(end->euler[1]), degRad(end->euler[2]), pRotMatT);
		
		//std::cout << "pRotMatT " ;
		//for (int i=0; i<9; i++) { std::cout << pRotMat[i] << " ";  } std::cout << std::endl;
		bool hasParent = false;
		for (int i=0; i<9; i++)
		{
			if (pRotMat[i] != 0) { hasParent = true; }
		}

		//for (int i=0; i<9; i++) { pRotMatF[i] = pRotMatT[i]; }

		if (!hasParent)
		{		
		   	//pRotMatF = pRotMatT;
			//std::cout << "not has parent" << std::endl;
			for (int i=0; i<9; i++) { pRotMatF[i] = pRotMatT[i]; }
		} else { 
			//pRotMatF = pRotMatT *pRotMat;
			if (! mult(pRotMatT, 3, 3, pRotMat, 3, 3, pRotMatF) )
			{
				std::cout << "Mult Fail: pRotMatT*pRotMat" << std::endl;
			}
		}
		//pRotMat = pRotMatF;
		for (int i=0; i<9; i++) { pRotMat[i] = pRotMatF[i]; }
		//for (int i=0; i<9; i++) { std::cout << pRotMat[i] << " ";  } std::cout << std::endl;


		if (end->parent)
		 { end = end->parent;	}
		else { end = NULL; }
	} while (end);
	end = thisEnd;

	float lenVect[3];
	float posVect[3];

	lenVect[0] = end->length[0];
	lenVect[1] = end->length[1];
	lenVect[2] = end->length[2];

	if (!mult(pRotMatF, 3, 3, lenVect, 3, 1, posVect) ) {
		 std::cout << "Mult Fail: pRotMatF*lenVect" << std::endl;
	}
	//posVect = pRotMatF*lenVect;


	float h = sqrt(end->length[0]*end->length[0] + end->length[1]*end->length[1] + end->length[2]*end->length[2]);
	float h2 = sqrt(posVect[0]*posVect[0] + posVect[1]*posVect[1] + posVect[2]*posVect[2]);

	
	//std::cout << posVect[0] << " " << posVect[1] << " " << posVect[2] << std::endl;
	//std::cout << pos[0] << ", " << pos[1] << ", " << pos[2] << std::endl;
	//assert(h == h2);
	pos[0] += posVect[0] + end->offset[0];
	pos[1] += posVect[1] + end->offset[1];
	pos[2] += posVect[2] + end->offset[2];

	//Simple Trig to get the end vector position of this node.
	//pos[0] += h*-sin(theta + degRad(end->euler[0]+pThetaX));
	//pos[1] += h*cos(theta + degRad(end->euler[0]+pThetaX));
	//pos[2] += 0; 

	//std::cout << end->name << " End Pos = " << pos[0] << ", " << pos[1] << std::endl;

	return;
}




float distToTarget(NODE *node, TARGET *tar)
{
	std::cout << "dTT " << node->name << " " << tar->name << " " << std::endl;;
	float a,b,c,d;
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(node, pos);
	//std::cout << pos[0] << ", " << pos[1] << ", " << pos[2] << std::endl;
	b = pos[0]- tar->pos[0];
	c = pos[1]- tar->pos[1];
	d = pos[2]- tar->pos[2];

	a = sqrt(b*b + c*c + d*d);
	return a;
}

NODE* getFirstEndEffector(NODE *seg)
{
	if (seg->noofchildren > 0)
	{
		getFirstEndEffector(seg->children[0]);
	} else {
		return seg;
	}
}

void CCD(NODE *cur, TARGET *tar)
{
	if (!cur) { return; }
	if (!tar) { return; }
	std::cout << cur->name << std::endl;
	NODE *end = getFirstEndEffector(cur);
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
   	tj[0] = tar->pos[0] - jpos[0];
   	tj[1] = tar->pos[1] - jpos[1];
   	tj[2] = tar->pos[2] - jpos[2];

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
	//float sinA = (axis[0]+axis[1]+axis[2])/(ejSqr*tjSqr);
	float sinA = minejdottj/(ejSqr*tjSqr);
	//std::cout << "Sins " <<  sinA << std::endl;

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
	
	//Limit cosA - eliminates rounding errors.
	cosA = cosA>-1?cosA:-1;
	cosA = cosA<1?cosA:1;
	float rotAng = acos(cosA);
	if (rotAng < 0.01 ) { return; }
	if (sinA < 0) { rotAng = -rotAng; }
	//if (axis[2] < 0) { rotAng = -rotAng; }
	//std::cout << "RotAng: " << radDeg(rotAng) << std::endl;
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

	//std::cout << "Eulers: " << cur->euler[0] << " " << cur->euler[1] << " " <<cur->euler[2] << std::
}
