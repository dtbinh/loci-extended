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

void calcEndPos(NODE *end, double *pos)
{
	NODE *thisEnd = end;
	//Recursivly Calculate the position of paren nodes
	if(end->parent != NULL)
	{
		calcEndPos(end->parent, pos);
	} else {
		pos[0] = 0; pos[1] = 0; pos[2] = 0;
	}
	float pTheta = 0;
	while (end->parent) { pTheta += end->parent->euler; end = end->parent;}
	end = thisEnd;
	//TODO
}

#endif
