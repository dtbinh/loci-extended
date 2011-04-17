#ifndef __ccdFuncs_h__
#define __ccdFuncs_h__

#include "Extras.h"

#include <iostream>
#include <cmath>
#include <map>

float degRad(float deg);
float radDeg(float rad);

struct TARGET
{
	const char *name;
	float pos[3];
	NODE *end;
};

/**
 * getEndEffector 
 * Given a NODE, an empty int and empty allocated list of NODEs
 * fill the int with the number of end effectors
 * fill the list with pointers to NODEs
 */
void getEndEffector(NODE *cur, int *noofends, NODE**retList);
void transpose(float *J, int m, int n, float *JT);
bool mult(float* A, int m1, int n1, float* B, int m2, int n2, float* res);
void fillRotMat(float a, float b, float g, float *rotMat);
void radAngleAxisRot(float rotAng, float *axis, float *eulers);
void calcEndPos(NODE *end, float *pos);
float distToTarget(NODE *node, TARGET *tar);
NODE* getFirstEndEffector(NODE *seg);
void CCD(NODE *cur, TARGET *tar);
#endif

