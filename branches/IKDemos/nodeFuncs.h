#include <cmath>
#include <iostream>



const float PI = 3.14159265f;
float radDeg (float rad) { return rad*180.0/PI; }
float degRad (float rad) { return rad*PI/180.0; }

struct NODE 
{
	const char *name;
	float length[3];
	float euler[3];
	float offset[3];
	float weight;
	NODE *parent;
	NODE *child;
};

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
	
	//Matrix pRotMatF = Matrix(3,3).fill(0);
	//Matrix pRotMat = Matrix(3,3).fill(0);
	float pRotMatF[9];
	float pRotMat[9];
	for (int i=0; i<9; i++) { pRotMat[i] = 0; pRotMatF[i] = 0; }

	do 
	{
		//Matrix pRotMatT = Matrix(3,3).fill(0);
		float pRotMatT[9]; 
		fillRotMat(degRad(end->euler[0]), degRad(end->euler[1]), degRad(end->euler[2]), pRotMatT);
		
		bool hasParent = false;
		for (int i=0; i<9; i++)
		{
			if (pRotMat[i] != 0) { hasParent = true; }
		}

		//for (int i=0; i<9; i++) { pRotMatF[i] = pRotMatT[i]; }

		if (!hasParent)
		{		
		   	//pRotMatF = pRotMatT;
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

#ifdef OCTAVE
/*
 * calcEndPos
 * take a NODE and a float array of length 3
 * fill the array with x,y,z coords of the nodes's end position
 */
void oldcalcEndPos(NODE *end, float *pos)
{
	NODE *thisEnd = end;
	//Recursively calculate the parent nodes
	
	if (end->parent != NULL)
	{
		oldcalcEndPos(end->parent, pos);
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
		if ((mx_el_eq(pRotMat, 0) == boolMatrix(3,3,1))) {
		   	pRotMatF = pRotMatT; 
		} else {
			std::cout << "OLd Has Parent" << std::endl;
			std::cout << pRotMatT << pRotMat << std::endl;
			pRotMatF = pRotMatT * pRotMat;
		}
		pRotMat = pRotMatF;
		std::cout << pRotMat << std::endl;

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

