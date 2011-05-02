//mkoctfile -lGL -lglut -lunittest++ --link-stand-alone testIKtest3D.cpp -o ./bin/testIKtest3D && ./bin/testIKtest3D
#define MAIN
#define OCTAVE 
#include <unittest++/UnitTest++.h>
#include "nodeFuncs.h"
#define D 0.004

void printSqrArr(float *arr, int m, int n)
{
	for ( int i=0; i<m; i++)
	{
		for (int j=0; j<n; j++)
		{
			std::cout << arr[(i*3)+j] << "\t";
		}
		std::cout << std::endl;
	}
}
TEST(mult)
{
	float a[9];
	float b[9];
	float ans[9];

	for (int i=0; i<9; i++) { a[i] = 0; b[i] = 0; }

	a[0] = 1; a[4] = 1; a[8] = 1;

	b[0] = 1; b[1] = 2; b[2] = 3;
	b[3] = 4.5; b[4] = 5; b[5] = 6;
	b[6] = 7; b[7] = 8; b[8] = 9;

	mult(a, 3, 3, b, 3, 3, ans);

	CHECK_EQUAL(1, ans[0]);
	CHECK_EQUAL(2, ans[1]);
	CHECK_EQUAL(3, ans[2]);

	CHECK_EQUAL(4.5, ans[3]);
	CHECK_EQUAL(5, ans[4]);
	CHECK_EQUAL(6, ans[5]);

	CHECK_EQUAL(7, ans[6]);
	CHECK_EQUAL(8, ans[7]);
	CHECK_EQUAL(9, ans[8]);
}



TEST(mult2)
{
	float a[9];
	float b[9];
	float ans[9];

	for (int i=0; i<9; i++) { a[i] = 0; b[i] = 0; ans[i] = 0; }

	a[0] = 1; a[4] = 2; a[8] = 3;

	b[0] = 1; b[1] = 2; b[2] = 3;
	b[3] = 4; b[4] = 5; b[5] = 6;
	b[6] = 7; b[7] = 8; b[8] = 9;

	mult(a, 3, 3, b, 3, 3, ans);

	//printSqrArr(ans, 3, 3);

	CHECK_EQUAL(1, ans[0]);
	CHECK_EQUAL(2, ans[1]);
	CHECK_EQUAL(3, ans[2]);

	CHECK_EQUAL(8, ans[3]);
	CHECK_EQUAL(10, ans[4]);
	CHECK_EQUAL(12, ans[5]);

	CHECK_EQUAL(21, ans[6]);
	CHECK_EQUAL(24, ans[7]);
	CHECK_EQUAL(27, ans[8]);
}

TEST(mult3)
{
	float a[9];
	float b[9];
	float ans[9];

	for (int i=0; i<9; i++) { a[i] = 0; b[i] = 0; ans[i] = 0; }

	a[0] = 2; a[1] = 4; a[2] = 6;
	a[3] = 3; a[4] = 5; a[5] = 1;
	a[6] = 0; a[7] = 9; a[8] = 3;

	b[0] = 1; b[1] = 2; b[2] = 3;
	b[3] = 4; b[4] = 5; b[5] = 6;
	b[6] = 7; b[7] = 8; b[8] = 9;

	mult(a, 3, 3, b, 3, 3, ans);

	//printSqrArr(ans, 3, 3);

	CHECK_EQUAL(60, ans[0]);
	CHECK_EQUAL(72, ans[1]);
	CHECK_EQUAL(84, ans[2]);

	CHECK_EQUAL(30, ans[3]);
	CHECK_EQUAL(39, ans[4]);
	CHECK_EQUAL(48, ans[5]);

	CHECK_EQUAL(57, ans[6]);
	CHECK_EQUAL(69, ans[7]);
	CHECK_EQUAL(81, ans[8]);
}

TEST(mult4)
{
	float a[9];
	float b[9];
	float ans[9];

	for (int i=0; i<9; i++) { a[i] = 0; b[i] = 0; ans[i] = 0; }

	a[0] = 3; a[1] = 1; a[2] = 0;
	a[3] = 3; a[4] = 5; a[5] = 0;
	a[6] = 0; a[7] = 0; a[8] = 0;

	b[0] = 1; b[1] = 2; b[2] = 3;
	b[3] = 4; b[4] = 5; b[5] = 6;
	b[6] = 7; b[7] = 8; b[8] = 9;

	mult(a, 3, 3, b, 3, 3, ans);

	//printSqrArr(ans, 3, 3);

	CHECK_EQUAL(7, ans[0]);
	CHECK_EQUAL(11, ans[1]);
	CHECK_EQUAL(15, ans[2]);

	CHECK_EQUAL(23, ans[3]);
	CHECK_EQUAL(31, ans[4]);
	CHECK_EQUAL(39, ans[5]);

	CHECK_EQUAL(0, ans[6]);
	CHECK_EQUAL(0, ans[7]);
	CHECK_EQUAL(0, ans[8]);
}

TEST(multVect)
{
	float a[3];
	float b[9];
	float ans[3];

	for (int i=0; i<9; i++) { b[i] = 0; }
	for (int i=0; i<3; i++) { a[i] = 0; ans[i] = 0; }

	a[0] = 1; a[1] = 2; a[2] = 3;

	b[0] = 1; b[1] = 2; b[2] = 3;
	b[3] = 4; b[4] = 5; b[5] = 6;
	b[6] = 7; b[7] = 8; b[8] = 9;

	mult(b, 3, 3, a, 3, 1, ans);
	CHECK_EQUAL(14, ans[0]);
	CHECK_EQUAL(32, ans[1]);
	CHECK_EQUAL(50, ans[2]);
}


TEST(multVect2)
{
	float a[3];
	float b[9];
	float ans[3];

	for (int i=0; i<9; i++) { b[i] = 0; }
	for (int i=0; i<3; i++) { a[i] = 0; ans[i] = 0; }

	a[0] = 1; a[1] = 1; a[2] = 1;

	b[0] = 1; b[1] = 2; b[2] = 3;
	b[3] = 4; b[4] = 5; b[5] = 6;
	b[6] = 7; b[7] = 8; b[8] = 9;

	mult(b, 3, 3, a, 3, 1, ans);

	CHECK_EQUAL(6, ans[0]);
	CHECK_EQUAL(15, ans[1]);
	CHECK_EQUAL(24, ans[2]);
}
TEST(RotMat000)
{
	
	Matrix pRotMat = Matrix(3,3).fill(0);
	Matrix pRotMatT = Matrix(3,3).fill(0);
	Matrix pRotMatF = Matrix(3,3).fill(0);

	pRotMatT = fillRotMat(0, 0, 0);
	float pRotArr[9];
	fillRotMat(0, 0, 0, pRotArr);
	
	for (int i= 0; i< 3; i++)
	{
		pRotMat(i, 0) = pRotArr[i*3+0];
		pRotMat(i, 1) = pRotArr[i*3+1];
		pRotMat(i, 2) = pRotArr[i*3+2];
	}
	


	pRotMatF = pRotMat*pRotMat.transpose();

	CHECK_EQUAL(pRotMat, pRotMatT);

	
	CHECK_CLOSE(1, pRotMatF(0,0), 0.004);
	CHECK_CLOSE(0, pRotMatF(0,1), 0.004);
	CHECK_CLOSE(0, pRotMatF(0,2), 0.004);
                          
	CHECK_CLOSE(0, pRotMatF(1,0), 0.004);
	CHECK_CLOSE(1, pRotMatF(1,1), 0.004);
	CHECK_CLOSE(0, pRotMatF(1,2), 0.004);
                          
	CHECK_CLOSE(0, pRotMatF(2,0), 0.004);
	CHECK_CLOSE(0, pRotMatF(2,1), 0.004);
	CHECK_CLOSE(1, pRotMatF(2,2), 0.004);
}

TEST(RotMatZ30)
{
	Matrix pRotMat = Matrix(3,3).fill(0);
	Matrix pRotMatF = Matrix(3,3).fill(0);

	pRotMat = fillRotMat(0, 0, 0);
	float pRotArr[9];
	fillRotMat(0, 0, 40/180.0*PI, pRotArr);
	for (int i= 0; i< 3; i++)
	{
		pRotMat(i, 0) = pRotArr[i*3+0];
		pRotMat(i, 1) = pRotArr[i*3+1];
		pRotMat(i, 2) = pRotArr[i*3+2];
	}
	

	pRotMatF = pRotMat*pRotMat.transpose();

	CHECK_CLOSE(1, pRotMatF(0,0), 0.004);
	CHECK_CLOSE(0, pRotMatF(0,1), 0.004);
	CHECK_CLOSE(0, pRotMatF(0,2), 0.004);
                          
	CHECK_CLOSE(0, pRotMatF(1,0), 0.004);
	CHECK_CLOSE(1, pRotMatF(1,1), 0.004);
	CHECK_CLOSE(0, pRotMatF(1,2), 0.004);
                          
	CHECK_CLOSE(0, pRotMatF(2,0), 0.004);
	CHECK_CLOSE(0, pRotMatF(2,1), 0.004);
	CHECK_CLOSE(1, pRotMatF(2,2), 0.004);
}


TEST(RotMatX60)
{
	Matrix pRotMat = Matrix(3,3).fill(0);

	float pRotArr[9];
	fillRotMat(60/180.0*PI, 0, 0, pRotArr);
	for (int i= 0; i< 3; i++)
	{
		pRotMat(i, 0) = pRotArr[i*3+0];
		pRotMat(i, 1) = pRotArr[i*3+1];
		pRotMat(i, 2) = pRotArr[i*3+2];
	}
	Matrix pRotMatF = Matrix(3,3).fill(0);
	pRotMatF = pRotMat*pRotMat.transpose();

	CHECK_CLOSE(1, pRotMatF(0,0), 0.004);
	CHECK_CLOSE(0, pRotMatF(0,1), 0.004);
	CHECK_CLOSE(0, pRotMatF(0,2), 0.004);
                          
	CHECK_CLOSE(0, pRotMatF(1,0), 0.004);
	CHECK_CLOSE(1, pRotMatF(1,1), 0.004);
	CHECK_CLOSE(0, pRotMatF(1,2), 0.004);
                          
	CHECK_CLOSE(0, pRotMatF(2,0), 0.004);
	CHECK_CLOSE(0, pRotMatF(2,1), 0.004);
	CHECK_CLOSE(1, pRotMatF(2,2), 0.004);
}


TEST(RotMatY25)
{
	Matrix pRotMat = Matrix(3,3).fill(0);

	float pRotArr[9];
	fillRotMat(0, 25/180.0*PI, 0, pRotArr);
	for (int i= 0; i< 3; i++)
	{
		pRotMat(i, 0) = pRotArr[i*3+0];
		pRotMat(i, 1) = pRotArr[i*3+1];
		pRotMat(i, 2) = pRotArr[i*3+2];
	}
	Matrix pRotMatF = Matrix(3,3).fill(0);
	pRotMatF = pRotMat*pRotMat.transpose();

	CHECK_CLOSE(1, pRotMatF(0,0), 0.004);
	CHECK_CLOSE(0, pRotMatF(0,1), 0.004);
	CHECK_CLOSE(0, pRotMatF(0,2), 0.004);
                          
	CHECK_CLOSE(0, pRotMatF(1,0), 0.004);
	CHECK_CLOSE(1, pRotMatF(1,1), 0.004);
	CHECK_CLOSE(0, pRotMatF(1,2), 0.004);
                          
	CHECK_CLOSE(0, pRotMatF(2,0), 0.004);
	CHECK_CLOSE(0, pRotMatF(2,1), 0.004);
	CHECK_CLOSE(1, pRotMatF(2,2), 0.004);
}

TEST(RotMatx30y25)
{
	Matrix pRotMat = Matrix(3,3).fill(0);
	Matrix pRotMatT = Matrix(3,3).fill(0);
	
	float pRotArr[9];
	fillRotMat(30/180.0*PI, 25/180.0*PI, 0, pRotArr);
	for (int i= 0; i< 3; i++)
	{
		pRotMat(i, 0) = pRotArr[i*3+0];
		pRotMat(i, 1) = pRotArr[i*3+1];
		pRotMat(i, 2) = pRotArr[i*3+2];
	}
	Matrix pRotMatF = Matrix(3,3).fill(0);
	pRotMatF = pRotMat*pRotMat.transpose();

	CHECK_CLOSE(1, pRotMatF(0,0), 0.004);
	CHECK_CLOSE(0, pRotMatF(0,1), 0.004);
	CHECK_CLOSE(0, pRotMatF(0,2), 0.004);
                          
	CHECK_CLOSE(0, pRotMatF(1,0), 0.004);
	CHECK_CLOSE(1, pRotMatF(1,1), 0.004);
	CHECK_CLOSE(0, pRotMatF(1,2), 0.004);
                          
	CHECK_CLOSE(0, pRotMatF(2,0), 0.004);
	CHECK_CLOSE(0, pRotMatF(2,1), 0.004);
	CHECK_CLOSE(1, pRotMatF(2,2), 0.004);
}

TEST(RotMatx30z75)
{
	Matrix pRotMat = Matrix(3,3).fill(0);
	//pRotMat = fillRotMat(30/180.0*PI, 0, 75/180.0*PI);

	float pRotArr[9];
	fillRotMat(30/180.0*PI, 0, 75/180.0*PI, pRotArr);
	for (int i= 0; i< 3; i++)
	{
		pRotMat(i, 0) = pRotArr[i*3+0];
		pRotMat(i, 1) = pRotArr[i*3+1];
		pRotMat(i, 2) = pRotArr[i*3+2];
	}
	Matrix pRotMatF = Matrix(3,3).fill(0);
	pRotMatF = pRotMat*pRotMat.transpose();

	CHECK_CLOSE(1, pRotMatF(0,0), 0.004);
	CHECK_CLOSE(0, pRotMatF(0,1), 0.004);
	CHECK_CLOSE(0, pRotMatF(0,2), 0.004);
                          
	CHECK_CLOSE(0, pRotMatF(1,0), 0.004);
	CHECK_CLOSE(1, pRotMatF(1,1), 0.004);
	CHECK_CLOSE(0, pRotMatF(1,2), 0.004);
                          
	CHECK_CLOSE(0, pRotMatF(2,0), 0.004);
	CHECK_CLOSE(0, pRotMatF(2,1), 0.004);
	CHECK_CLOSE(1, pRotMatF(2,2), 0.004);
}

TEST(RotMatx30y80z75)
{
	Matrix pRotMat = Matrix(3,3).fill(0);
	//pRotMat = fillRotMat(30/180.0*PI, -80/180.0*PI, 75/180.0*PI);

	float pRotArr[9];
	fillRotMat(30/180.0*PI, -80/180.0*PI, 75/180.0*PI, pRotArr);
	for (int i= 0; i< 3; i++)
	{
		pRotMat(i, 0) = pRotArr[i*3+0];
		pRotMat(i, 1) = pRotArr[i*3+1];
		pRotMat(i, 2) = pRotArr[i*3+2];
	}
	Matrix pRotMatF = Matrix(3,3).fill(0);
	pRotMatF = pRotMat*pRotMat.transpose();

	CHECK_CLOSE(1, pRotMatF(0,0), 0.004);
	CHECK_CLOSE(0, pRotMatF(0,1), 0.004);
	CHECK_CLOSE(0, pRotMatF(0,2), 0.004);
                          
	CHECK_CLOSE(0, pRotMatF(1,0), 0.004);
	CHECK_CLOSE(1, pRotMatF(1,1), 0.004);
	CHECK_CLOSE(0, pRotMatF(1,2), 0.004);
                          
	CHECK_CLOSE(0, pRotMatF(2,0), 0.004);
	CHECK_CLOSE(0, pRotMatF(2,1), 0.004);
	CHECK_CLOSE(1, pRotMatF(2,2), 0.004);
}


//CalcEndPos Single Rotations + single Nodes
TEST(calcEndPosNoRot)
{
	NODE *n = new NODE;
	n->name = "node";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	n->child = NULL; n->parent = NULL;
	n->euler[0] = 0;	n->euler[1] = 0;	n->euler[2] = 0;

	float pos[3];
	pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n, pos);

	//std::cout << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;
	CHECK_CLOSE(0, pos[0], D);
	CHECK_CLOSE(1, pos[1], D);
	CHECK_CLOSE(0, pos[2], D);
}



TEST(calcEndPosXRot)
{
NODE *n = new NODE;
n->name = "node";
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = NULL; n->parent = NULL;
n->euler[0] = 40;	n->euler[1] = 0;	n->euler[2] = 0;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(n, pos);
CHECK_CLOSE(0, pos[0], D);
CHECK_CLOSE(0.766, pos[1], D);
CHECK_CLOSE(0.643, pos[2], D);
}


TEST(calcEndPosZRot)
{
NODE *n = new NODE;
n->name = "node";
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = NULL; n->parent = NULL;
n->euler[0] = 0;	n->euler[1] = 0;	n->euler[2] = 30;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(n, pos);
CHECK_CLOSE(-0.5, pos[0], D);
CHECK_CLOSE(0.866, pos[1], D);
CHECK_CLOSE(0, pos[2], D);
}


TEST(calcEndPosYRot)
{
NODE *n = new NODE;
n->name = "node";
n->length[0] = 1; n->length[1] = 0; n->length[2] = 0;
n->child = NULL; n->parent = NULL;
n->euler[0] = 0;	n->euler[1] = -90;	n->euler[2] = 0;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(n, pos);
CHECK_CLOSE(0, pos[0], D);
CHECK_CLOSE(0, pos[1], D);
CHECK_CLOSE(1, pos[2], D);
}


//CalcEndPos Single Node + Double Rotation
TEST(calcEndPosXZRot)
{
NODE *n = new NODE;
n->name = "node";
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = NULL; n->parent = NULL;
n->euler[0] = 45;	n->euler[1] = 0;	n->euler[2] = 45;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(n, pos);
CHECK_CLOSE(-0.5, pos[0], D);
CHECK_CLOSE(0.5, pos[1], D);
CHECK_CLOSE(0.707, pos[2], D);
}

//CalcEndPos Single Node + Double Rotation
TEST(calcEndPosXYZRot)
{
NODE *n = new NODE;
n->name = "node";
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = NULL; n->parent = NULL;
n->euler[0] = 45;	n->euler[1] = 20;	n->euler[2] = 60;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(n, pos);
CHECK_CLOSE(-0.491, pos[0], D);
CHECK_CLOSE(0.563, pos[1], D);
CHECK_CLOSE(0.664, pos[2], D);
}


//CalcEndPos DOuble Node 
TEST(DoubleNode)
{
NODE *n = new NODE;
NODE *na = new NODE;
n->name = "node"; na->name = "node2";
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = na; n->parent = NULL;
n->euler[0] = 0;	n->euler[1] = 0;	n->euler[2] = 0;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

na->length[0] = 0; 	na->length[1] = 1; na->length[2] = 0;
na->child = NULL; 	na->parent = n;
na->euler[0] = 0;	na->euler[1] = 0;	na->euler[2] = 0;


float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(na, pos);
CHECK_CLOSE(0, pos[0], D);
CHECK_CLOSE(2, pos[1], D);
CHECK_CLOSE(0, pos[2], D);
}

TEST(DoubleNodeRotTop)
{
	NODE *n = new NODE;
	NODE *na = new NODE;
	n->name = "node"; na->name = "node2";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	n->child = na; n->parent = NULL;
	n->euler[0] = 0;	n->euler[1] = 0;	n->euler[2] = 0;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

	na->length[0] = 0; 	na->length[1] = 1; na->length[2] = 0;
	na->child = NULL; 	na->parent = n;
	na->euler[0] = 45;	na->euler[1] = 0;	na->euler[2] = 0;

	float pos[3];
	pos[0] = 0; pos[1] = 0; pos[2] = 0;

	calcEndPos(na, pos);

	CHECK_CLOSE(0, pos[0], D);
	CHECK_CLOSE(1.707, pos[1], D);
	CHECK_CLOSE(0.707, pos[2], D);
}


TEST(DoubleNodeZRotZ)
{
NODE *n = new NODE;
NODE *na = new NODE;
n->name = "node"; na->name = "node2";
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = na; n->parent = NULL;
n->euler[0] = 0;	n->euler[1] = 0;	n->euler[2] = -45;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

na->length[0] = 0; 	na->length[1] = 1; na->length[2] = 0;
na->child = NULL; 	na->parent = n;
na->euler[0] = 0;	na->euler[1] = 0;	na->euler[2] = -45;


float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(na, pos);
CHECK_CLOSE(1.707, pos[0], D);
CHECK_CLOSE(0.707, pos[1], D);
CHECK_CLOSE(0, pos[2], D);
}

TEST(DoubleNodeZRotY)
{
NODE *n = new NODE;
NODE *na = new NODE;
n->name = "node"; na->name = "node2";
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = na; n->parent = NULL;
n->euler[0] = 0;	n->euler[1] = 0;	n->euler[2] = -45;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

na->length[0] = 0; 	na->length[1] = 1; na->length[2] = 0;
na->child = NULL; 	na->parent = n;
na->euler[0] = 0;	na->euler[1] = 70;	na->euler[2] = 0;


float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(na, pos);
CHECK_CLOSE(1.414, pos[0], D);
CHECK_CLOSE(1.414, pos[1], D);
CHECK_CLOSE(0, pos[2], D);
}

TEST(DoubleNodeXZRotXZ)
{
NODE *n = new NODE;
NODE *na = new NODE;
n->name = "node"; na->name = "node2";
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = na; n->parent = NULL;
n->euler[0] = 30;	n->euler[1] = 0;	n->euler[2] = -45;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

na->length[0] = 0; 	na->length[1] = 1; na->length[2] = 0;
na->child = NULL; 	na->parent = n;
na->euler[0] = 55;	na->euler[1] = 0;	na->euler[2] = -20;


float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(na, pos);
CHECK_CLOSE(0.792, pos[0], D);
CHECK_CLOSE(0.514, pos[1], D);
CHECK_CLOSE(1.479, pos[2], D);
}

TEST(DoubleNodeXYZRotXYZ)
{
NODE *n = new NODE;
NODE *na = new NODE;
n->name = "node"; na->name = "node2";
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = na; n->parent = NULL;
n->euler[0] = 30;	n->euler[1] = 130;	n->euler[2] = -45;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

na->length[0] = 0; 	na->length[1] = 1; na->length[2] = 0;
na->child = NULL; 	na->parent = n;
na->euler[0] = 100;	na->euler[1] = -60;	na->euler[2] = 35;


float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(na, pos);
CHECK_CLOSE(0.655, pos[0], D);
CHECK_CLOSE(-0.551, pos[1], D);
CHECK_CLOSE(0.066, pos[2], D);
}


TEST(TripleNodeNoRot)
{
NODE *n = new NODE;
NODE *na = new NODE;
NODE *nb = new NODE;
n->name = "node"; na->name = "node2"; nb->name = "node3"; 
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = na; n->parent = NULL;
n->euler[0] = 0;	n->euler[1] = 0;	n->euler[2] = 0;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

na->length[0] = 0; 	na->length[1] = 1; na->length[2] = 0;
na->child = nb; 	na->parent = n;
na->euler[0] = 0;	na->euler[1] = 0;	na->euler[2] = 0;

nb->length[0] = 0; 	nb->length[1] = 1; nb->length[2] = 0;
nb->child = NULL; 	nb->parent = na;
nb->euler[0] = 0;	nb->euler[1] = 0;	nb->euler[2] = 0;


float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(nb, pos);
CHECK_CLOSE(0, pos[0], D);
CHECK_CLOSE(3, pos[1], D);
CHECK_CLOSE(0, pos[2], D);
}

TEST(TripleNodeTopRot)
{
NODE *n = new NODE;
NODE *na = new NODE;
NODE *nb = new NODE;
n->name = "node"; na->name = "node2"; nb->name = "node3"; 
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = na; n->parent = NULL;
n->euler[0] = 0;	n->euler[1] = 0;	n->euler[2] = 0;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

na->length[0] = 0; 	na->length[1] = 1; na->length[2] = 0;
na->child = nb; 	na->parent = n;
na->euler[0] = 0;	na->euler[1] = 0;	na->euler[2] = 0;

nb->length[0] = 0; 	nb->length[1] = 1; nb->length[2] = 0;
nb->child = NULL; 	nb->parent = na;
nb->euler[0] = 0;	nb->euler[1] = 0;	nb->euler[2] = 45;


float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(nb, pos);
CHECK_CLOSE(-0.707, pos[0], D);
CHECK_CLOSE(2.707, pos[1], D);
CHECK_CLOSE(0, pos[2], D);
}


TEST(TripleNodeMidRot)
{
NODE *n = new NODE;
NODE *na = new NODE;
NODE *nb = new NODE;
n->name = "node"; na->name = "node2"; nb->name = "node3"; 
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = na; n->parent = NULL;
n->euler[0] = 0;	n->euler[1] = 0;	n->euler[2] = 0;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

na->length[0] = 0; 	na->length[1] = 1; na->length[2] = 0;
na->child = nb; 	na->parent = n;
na->euler[0] = 30;	na->euler[1] = 0;	na->euler[2] = 0;

nb->length[0] = 0; 	nb->length[1] = 1; nb->length[2] = 0;
nb->child = NULL; 	nb->parent = na;
nb->euler[0] = 0;	nb->euler[1] = 0;	nb->euler[2] = 0;


float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(nb, pos);
CHECK_CLOSE(0, pos[0], D);
CHECK_CLOSE(2.732, pos[1], D);
CHECK_CLOSE(1, pos[2], D);
}


TEST(TripleNodeMidTopRot)
{
NODE *n = new NODE;
NODE *na = new NODE;
NODE *nb = new NODE;
n->name = "node"; na->name = "node2"; nb->name = "node3"; 
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = na; n->parent = NULL;
n->euler[0] = 0;	n->euler[1] = 0;	n->euler[2] = 0;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

na->length[0] = 0; 	na->length[1] = 1; na->length[2] = 0;
na->child = nb; 	na->parent = n;
na->euler[0] = 30;	na->euler[1] = 0;	na->euler[2] = 0;

nb->length[0] = 0; 	nb->length[1] = 1; nb->length[2] = 0;
nb->child = NULL; 	nb->parent = na;
nb->euler[0] = 30;	nb->euler[1] = 0;	nb->euler[2] = 0;


float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(nb, pos);
CHECK_CLOSE(0, pos[0], D);
CHECK_CLOSE(2.366, pos[1], D);
CHECK_CLOSE(1.366, pos[2], D);
}


TEST(TripleNodeLowMidTopRot)
{
NODE *n = new NODE;
NODE *na = new NODE;
NODE *nb = new NODE;
n->name = "node"; na->name = "node2"; nb->name = "node3"; 
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = na; n->parent = NULL;
n->euler[0] = -50;	n->euler[1] = 0;	n->euler[2] = 0;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

na->length[0] = 0; 	na->length[1] = 1; na->length[2] = 0;
na->child = nb; 	na->parent = n;
na->euler[0] = 30;	na->euler[1] = 0;	na->euler[2] = 0;

nb->length[0] = 0; 	nb->length[1] = 1; nb->length[2] = 0;
nb->child = NULL; 	nb->parent = na;
nb->euler[0] = 30;	nb->euler[1] = 0;	nb->euler[2] = 0;


float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(nb, pos);
CHECK_CLOSE(0, pos[0], D);
CHECK_CLOSE(2.567, pos[1], D);
CHECK_CLOSE(-0.934, pos[2], D);
}

TEST(TripleNodeAllDir)
{
NODE *n = new NODE;
NODE *na = new NODE;
NODE *nb = new NODE;
n->name = "node"; na->name = "node2"; nb->name = "node3"; 
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = na; n->parent = NULL;
n->euler[0] = 0;	n->euler[1] = 0;	n->euler[2] = 45;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

na->length[0] = 0; 	na->length[1] = 1; na->length[2] = 0;
na->child = nb; 	na->parent = n;
na->euler[0] = 30;	na->euler[1] = 0;	na->euler[2] = 0;

nb->length[0] = 0; 	nb->length[1] = 1; nb->length[2] = 0;
nb->child = NULL; 	nb->parent = na;
nb->euler[0] = 30;	nb->euler[1] = 0;	nb->euler[2] = 0;


float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(nb, pos);
CHECK_CLOSE(-1.673, pos[0], D);
CHECK_CLOSE(1.673, pos[1], D);
CHECK_CLOSE(1.366, pos[2], D);
}

TEST(TripleNodeAllDirs)
{
NODE *n = new NODE;
NODE *na = new NODE;
NODE *nb = new NODE;
n->name = "node"; na->name = "node2"; nb->name = "node3"; 
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = na; n->parent = NULL;
n->euler[0] = 35;	n->euler[1] = 0;	n->euler[2] = 30;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

na->length[0] = 0; 	na->length[1] = 1; na->length[2] = 0;
na->child = nb; 	na->parent = n;
na->euler[0] = 30;	na->euler[1] = 0;	na->euler[2] = 10;

nb->length[0] = 0; 	nb->length[1] = 1; nb->length[2] = 0;
nb->child = NULL; 	nb->parent = na;
nb->euler[0] = -40;	nb->euler[1] = 0;	nb->euler[2] = 70;


float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(nb, pos);
CHECK_CLOSE(-1.734, pos[0], D);
CHECK_CLOSE(1.095, pos[1], D);
CHECK_CLOSE(1.362, pos[2], D);
}

TEST(TripleNodeBroke)
{
NODE *n = new NODE;
NODE *na = new NODE;
NODE *nb = new NODE;
n->name = "node"; na->name = "node2"; nb->name = "node3"; 
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = na; n->parent = NULL;
n->euler[0] = 30;	n->euler[1] = 0;	n->euler[2] = 0;
n->offset[0] = 0; 	n->offset[1] = 0;	n->offset[2] = 0;
n->parent = NULL;

na->length[0] = 0; 	na->length[1] = 1; na->length[2] = 0;
na->child = nb; 	na->parent = n;
na->euler[0] = -60;	na->euler[1] = 0;	na->euler[2] = 0;

nb->length[0] = 0; 	nb->length[1] = 1; nb->length[2] = 0;
nb->child = NULL; 	nb->parent = na;
nb->euler[0] = 0;	nb->euler[1] = 0;	nb->euler[2] = 60;


float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(nb, pos);
CHECK_CLOSE(-0.866, pos[0], D);
CHECK_CLOSE(2.165, pos[1], D);
CHECK_CLOSE(-0.250, pos[2], D);
}

TEST (axisAngleRad90Y)
{
	float eul[3]; eul[0] = 0; eul[1] = 0; eul[2] = 0;
	
	float axis[3];
	axis[0] = 0;
	axis[1] = 1;
	axis[2] = 0;
	radAngleAxisRot(degRad(90), axis, eul);
	
	CHECK_CLOSE(0, eul[0], D);
	CHECK_CLOSE(degRad(90), eul[1], D);
	CHECK_CLOSE(0, eul[2], D);
}

TEST (axisAngleRad45X)
{
	float eul[3]; eul[0] = 0; eul[1] = 0; eul[2] = 0;
	
	float axis[3];
	axis[0] = 1;
	axis[1] = 0;
	axis[2] = 0;
	radAngleAxisRot(degRad(45), axis, eul);
	
	CHECK_CLOSE(degRad(45), eul[0], D);
	CHECK_CLOSE(0, eul[1], D);
	CHECK_CLOSE(0, eul[2], D);
}

/*
TEST (axisAngleRad90XZ)
{
	float eul[3]; eul[0] = 0; eul[1] = 0; eul[2] = 0;
	
	float axis[3];
	axis[0] = sqrt(1);
	axis[1] = 0;
	axis[2] = sqrt(1);
	radAngleAxisRot(degRad(45), axis, eul);
	
	CHECK_CLOSE(degRad(45), eul[0], D);
	CHECK_CLOSE(0, eul[1], D);
	CHECK_CLOSE(0, eul[2], D);
}
*/

int main()
{
	return UnitTest::RunAllTests();
	/*
	std::cout << pRotMat << std::endl;

	std::cout << "World" << std::endl;
	exit(0);
	*/
}
