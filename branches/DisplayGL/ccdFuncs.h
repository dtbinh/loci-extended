#ifndef __ccdFuncs_h__
#define __ccdFuncs_h__
#include "Extras.h"

#include <iostream>
#include <cmath>

struct TARGET
{
	const char *name;
	float pos[3];
	NODE *end;
}
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
			getEndEffector(cur->child[i], noofends, retList);
		}
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
	
	return rotMat;
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
#endif
