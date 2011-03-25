//mkoctfile -lGL -lglut -lunittest++ --link-stand-alone testIKtest3D.cpp -o ./bin/testIKtest3D && ./bin/testIKtest3D
#define MAIN
#include <unittest++/UnitTest++.h>
#include "nodeFuncs.h"
#define D 0.004

TEST(RotMat000)
{

	Matrix pRotMat = Matrix(3,3).fill(0);
	Matrix pRotMatF = Matrix(3,3).fill(0);
	pRotMat = fillRotMat(0, 0, 0);


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

TEST(RotMatZ30)
{
	Matrix pRotMat = Matrix(3,3).fill(0);
	Matrix pRotMatF = Matrix(3,3).fill(0);
	pRotMat = fillRotMat(0, 0, 40/180.0*PI);


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
	pRotMat = fillRotMat(60/180.0*PI, 0, 0);

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
	pRotMat = fillRotMat(0, 25/180.0*PI, 0);

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
	pRotMat = fillRotMat(30/180.0*PI, 25/180.0*PI, 0);

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
	pRotMat = fillRotMat(30/180.0*PI, 0, 75/180.0*PI);

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
	pRotMat = fillRotMat(30/180.0*PI, -80/180.0*PI, 75/180.0*PI);

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

na->length[0] = 0; 	na->length[1] = 1; na->length[2] = 0;
na->child = NULL; 	na->parent = n;
na->euler[0] = 0;	na->euler[1] = 70;	na->euler[2] = 0;


float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(na, pos);
CHECK_CLOSE(0.949, pos[0], D);
CHECK_CLOSE(1.414, pos[1], D);
CHECK_CLOSE(-0.664, pos[2], D);
}

TEST(DoubleNodeXZRotXZ)
{
NODE *n = new NODE;
NODE *na = new NODE;
n->name = "node"; na->name = "node2";
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = na; n->parent = NULL;
n->euler[0] = 30;	n->euler[1] = 0;	n->euler[2] = -45;

na->length[0] = 0; 	na->length[1] = 1; na->length[2] = 0;
na->child = NULL; 	na->parent = n;
na->euler[0] = 55;	na->euler[1] = 0;	na->euler[2] = -20;


float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(na, pos);
CHECK_CLOSE(1.168, pos[0], D);
CHECK_CLOSE(0.348, pos[1], D);
CHECK_CLOSE(1.288, pos[2], D);
}

TEST(DoubleNodeXYZRotXYZ)
{
NODE *n = new NODE;
NODE *na = new NODE;
n->name = "node"; na->name = "node2";
n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
n->child = na; n->parent = NULL;
n->euler[0] = 30;	n->euler[1] = 130;	n->euler[2] = -45;

na->length[0] = 0; 	na->length[1] = 1; na->length[2] = 0;
na->child = NULL; 	na->parent = n;
na->euler[0] = 100;	na->euler[1] = -60;	na->euler[2] = 35;


float pos[3];
pos[0] = 0; pos[1] = 0; pos[2] = 0;
calcEndPos(na, pos);
CHECK_CLOSE(0.819, pos[0], D);
CHECK_CLOSE(0.611, pos[1], D);
CHECK_CLOSE(0.640, pos[2], D);
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


int main()
{
	return UnitTest::RunAllTests();
	/*
	std::cout << pRotMat << std::endl;

	std::cout << "World" << std::endl;
	exit(0);
	*/
}
