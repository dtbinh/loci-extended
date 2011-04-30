//COMPILE WITH: g++ testccdFuncs.cpp -o testccd -lUnitTest++ && ./testccd

#include <unittest++/UnitTest++.h>
//#include "Extras.h"
#include "ccdFuncs.h"

#define d 0.001

TEST(test1)
{
	CHECK_EQUAL(1,1);
}


TEST(noMovement)
{
	NODE *n = new NODE;
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n, pos);

	CHECK_CLOSE(0, pos[0], d);
	CHECK_CLOSE(1, pos[1], d);
	CHECK_CLOSE(0, pos[0], d);
}
TEST(offset)
{
	NODE *n = new NODE;
	n->name = "singleOffsetNode";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	n->offset[0] = 0.3; n->offset[1] = 0.7; n->offset[2] = 9;
	n->euler[0] = 0;	n->euler[1] = 0;	n->euler[2] = 0;
	n->parent = NULL;
	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n, pos);

	CHECK_CLOSE(0.3, pos[0], d);
	CHECK_CLOSE(1.7, pos[1], d);
	CHECK_CLOSE(9, pos[2], d);
}

TEST(offsetX90)
{
	NODE *n = new NODE;
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	n->offset[0] = 0.3; n->offset[1] = 0.7; n->offset[2] = 9;
	n->euler[0] = 0; n->euler[1] = 0; n->euler[2] = -90;
	n->parent = NULL;
	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n, pos);

	CHECK_CLOSE(1.3, pos[0], d);
	CHECK_CLOSE(0.7, pos[1], d);
	CHECK_CLOSE(9, pos[2], d);
}

TEST(offsetXYZ)
{
	NODE *n = new NODE;
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	n->offset[0] = 0.1; n->offset[1] = 1.7; n->offset[2] = -2.2;
	n->euler[0] = 45; n->euler[1] = 20; n->euler[2] = 60;
	n->parent = NULL;
	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n, pos);

	CHECK_CLOSE(-0.391, pos[0], d);
	CHECK_CLOSE(2.263, pos[1], d);
	CHECK_CLOSE(-1.536, pos[2], d);
}

TEST(2Nodes)
{
	NODE *n = new NODE;
	NODE *n2 = new NODE;
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	//n->offset[0] = 0.1; n->offset[1] = 1.7; n->offset[2] = -2.2;
	//n->euler[0] = 45; n->euler[1] = 20; n->euler[2] = 60;
	SetupChildren(n, 1);
	n->children[0] = n2;
	n->parent = NULL;

	n2->name = "node2";
	n2->length[0] = 0; n2->length[1] = 1; n2->length[2] = 0;
	n2->parent = n;

	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n2, pos);

	CHECK_CLOSE(0, pos[0], d);
	CHECK_CLOSE(2, pos[1], d);
	CHECK_CLOSE(0, pos[2], d);
}

TEST(2Nodes1Offset)
{
	NODE *n = new NODE;
	NODE *n2 = new NODE;
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	n->offset[0] = 0.1; n->offset[1] = 1.7; n->offset[2] = -2.2;
	n->parent = NULL;
	//n->euler[0] = 45; n->euler[1] = 20; n->euler[2] = 60;
	SetupChildren(n, 1);
	n->children[0] = n2;

	n2->name = "node2";
	n2->length[0] = 0; n2->length[1] = 1; n2->length[2] = 0;
	n2->parent = n;

	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n2, pos);

	CHECK_CLOSE(0.1, pos[0], d);
	CHECK_CLOSE(3.7, pos[1], d);
	CHECK_CLOSE(-2.2, pos[2], d);
}

TEST(2Nodes2Offset)
{
	NODE *n = new NODE;
	NODE *n2 = new NODE;
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	n->offset[0] = 0.1; n->offset[1] = 1.7; n->offset[2] = -2.2;
	n->parent = NULL;
	//n->euler[0] = 45; n->euler[1] = 20; n->euler[2] = 60;
	SetupChildren(n, 1);
	n->children[0] = n2;

	n2->name = "node2";
	n2->length[0] = 0; n2->length[1] = 1; n2->length[2] = 0;
	n2->offset[0] = 0.2; n2->offset[1] = 0.3; n2->offset[2] = 0.2;
	n2->parent = n;

	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n2, pos);

	CHECK_CLOSE(0.3, pos[0], d);
	CHECK_CLOSE(4, pos[1], d);
	CHECK_CLOSE(-2, pos[2], d);
}

TEST(2NodesTop)
{
	NODE *n = new NODE;
	NODE *n2 = new NODE;
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	n->offset[0] = 0.1; n->offset[1] = 1.7; n->offset[2] = -2.2;
	n->euler[0] = 0; n->euler[1] = 0; n->euler[2] = 0;
	n->parent = NULL;
	SetupChildren(n, 1);
	n->children[0] = n2;

	n2->name = "node2";
	n2->length[0] = 0; n2->length[1] = 1; n2->length[2] = 0;
	n2->offset[0] = 0.2; n2->offset[1] = 0.2; n2->offset[2] = 0.2;
	n2->euler[0] = 45; n2->euler[1] = 0; n2->euler[2] = 0;
	n2->parent = n;

	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n2, pos);

	CHECK_CLOSE(0.3, pos[0], d);
	CHECK_CLOSE(3.607, pos[1], d);
	CHECK_CLOSE(-1.293, pos[2], d);
}

TEST(2NodesZY)
{
	NODE *n = new NODE;
	NODE *n2 = new NODE;
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	n->offset[0] = 0.1; n->offset[1] = 1.2; n->offset[2] = 2.2;
	n->euler[0] = 0; n->euler[1] = 0; n->euler[2] = -45;
	n->parent = NULL;
	SetupChildren(n, 1);
	n->children[0] = n2;

	n2->name = "node2";
	n2->length[0] = 0; n2->length[1] = 1; n2->length[2] = 0;
	n2->offset[0] = 0.2; n2->offset[1] = 0.2; n2->offset[2] = 0.2;
	n2->euler[0] = 0; n2->euler[1] = 70; n2->euler[2] = 0;
	n2->parent = n;

	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n2, pos);

	CHECK_CLOSE(1.714, pos[0], d);
	CHECK_CLOSE(2.814, pos[1], d);
	CHECK_CLOSE(2.400, pos[2], d);
}

TEST(3Nodes)
{
	NODE *n = new NODE;
	NODE *n2 = new NODE;
	NODE *n3 = new NODE;
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	//n->offset[0] = 0.1; n->offset[1] = 1.2; n->offset[2] = 2.2;
	//n->euler[0] = 0; n->euler[1] = 0; n->euler[2] = -45;
	SetupChildren(n, 1);
	n->children[0] = n2;
	n->parent = NULL;

	n2->name = "node2";
	n2->length[0] = 0; n2->length[1] = 1; n2->length[2] = 0;
	//n2->offset[0] = 0.2; n2->offset[1] = 0.2; n2->offset[2] = 0.2;
	//n2->euler[0] = 0; n2->euler[1] = 70; n2->euler[2] = 0;
	SetupChildren(n2, 1);
	n2->children[0] = n3; n2->parent = n;

	n3->name = "node2";
	n3->length[0] = 0; n3->length[1] = 1; n3->length[2] = 0;
	//n3->euler[0] = 0; n3->euler[1] = 70; n3->euler[2] = 0;
	n3->parent = n2;


	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n3, pos);

	CHECK_CLOSE(0, pos[0], d);
	CHECK_CLOSE(3, pos[1], d);
	CHECK_CLOSE(0, pos[2], d);
}

TEST(3NodesOffset)
{
	NODE *n = new NODE;
	NODE *n2 = new NODE;
	NODE *n3 = new NODE;
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	n->offset[0] = 0.1; n->offset[1] = 1.2; n->offset[2] = 2.2;
	n->parent = NULL;
	//n->euler[0] = 0; n->euler[1] = 0; n->euler[2] = -45;
	SetupChildren(n, 1);
	n->children[0] = n2;

	n2->name = "node2";
	n2->length[0] = 0; n2->length[1] = 1; n2->length[2] = 0;
	n2->offset[0] = 0.2; n2->offset[1] = 0.2; n2->offset[2] = 0.2;
	//n2->euler[0] = 0; n2->euler[1] = 70; n2->euler[2] = 0;
	SetupChildren(n2, 1);
	n2->children[0] = n3; n2->parent = n;

	n3->name = "node2";
	n3->length[0] = 0; n3->length[1] = 1; n3->length[2] = 0;
	n3->offset[0] = 0.3; n3->offset[1] = 0.7; n3->offset[2] = 0.3;
	//n3->euler[0] = 0; n3->euler[1] = 70; n3->euler[2] = 0;
	n3->parent = n2;


	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n3, pos);

	CHECK_CLOSE(0.6, pos[0], d);
	CHECK_CLOSE(5.1, pos[1], d);
	CHECK_CLOSE(2.7, pos[2], d);
}

TEST(3NodesEuler)
{
	NODE *n = new NODE;
	NODE *n2 = new NODE;
	NODE *n3 = new NODE;
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	//n->offset[0] = 0.1; n->offset[1] = 1.2; n->offset[2] = 2.2;
	n->euler[0] = 35; n->euler[1] = 0; n->euler[2] = 30;
	n->parent = NULL;
	SetupChildren(n, 1);
	n->children[0] = n2;

	n2->name = "node2";
	n2->length[0] = 0; n2->length[1] = 1; n2->length[2] = 0;
	//n2->offset[0] = 0.2; n2->offset[1] = 0.2; n2->offset[2] = 0.2;
	n2->euler[0] = 30; n2->euler[1] = 0; n2->euler[2] = 10;
	SetupChildren(n2, 1);
	n2->children[0] = n3; n2->parent = n;

	n3->name = "node3";
	n3->length[0] = 0; n3->length[1] = 1; n3->length[2] = 0;
	//n3->offset[0] = 0.3; n3->offset[1] = 0.7; n3->offset[2] = 0.3;
	n3->euler[0] = -40; n3->euler[1] = 0; n3->euler[2] = 70;
	n3->parent = n2;


	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n3, pos);

	CHECK_CLOSE(-1.734, pos[0], d);
	CHECK_CLOSE(1.095, pos[1], d);
	CHECK_CLOSE(1.362, pos[2], d);
}


TEST(froset)
{
	NODE *n = new NODE;
	SetupFrames(n, 2);
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	n->offset[0] = 0.3; n->offset[1] = 0.7; n->offset[2] = 9;
	n->froset[0][0] = 0.2; n->froset[0][1] = 0.3; n->froset[0][2] = 1;
	n->froset[1][0] = 0.3; n->froset[1][1] = 0.4; n->froset[1][2] = 1.1;
	n->euler[0] = 0; n->euler[1] = 0; n->euler[2] = 0;
	n->parent = NULL;
	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n, pos, 0);
	CHECK_CLOSE(0.5, pos[0], d);
	CHECK_CLOSE(2.0, pos[1], d);
	CHECK_CLOSE(10, pos[2], d);

	pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n, pos, 1);
	CHECK_CLOSE(0.6, pos[0], d);
	CHECK_CLOSE(2.1, pos[1], d);
	CHECK_CLOSE(10.1, pos[2], d);
}

TEST(freuler)
{
	NODE *n = new NODE;
	SetupFrames(n, 1);
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	//n->offset[0] = 0.3; n->offset[1] = 0.7; n->offset[2] = 9;
	n->euler[0] = 0; n->euler[1] = 0; n->euler[2] = 0;
	n->freuler[0][0] = 0; n->freuler[0][1] = 0; n->freuler[0][2] = 35;
	n->parent = NULL;
	//n->froset[1][0] = 0.3; n->froset[1][1] = 0.4; n->froset[1][2] = 1.1;
	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n, pos, 0);
	CHECK_CLOSE(-0.574, pos[0], d);
	CHECK_CLOSE( 0.819, pos[1], d);
	CHECK_CLOSE( 0, pos[2], d);
}

TEST(freulerEuler)
{
	NODE *n = new NODE;
	SetupFrames(n, 1);
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	//n->offset[0] = 0.3; n->offset[1] = 0.7; n->offset[2] = 9;
	n->euler[0] = 0; n->euler[1] = 0; n->euler[2] = 30;
	n->freuler[0][0] = 0; n->freuler[0][1] = 0; n->freuler[0][2] = 35;
	n->parent = NULL;
	//n->froset[1][0] = 0.3; n->froset[1][1] = 0.4; n->froset[1][2] = 1.1;
	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n, pos, 0);
	CHECK_CLOSE(-0.906, pos[0], d);
	CHECK_CLOSE( 0.423, pos[1], d);
	CHECK_CLOSE( 0, pos[2], d);
}

TEST(freulerEulerZX)
{
	NODE *n = new NODE;
	SetupFrames(n, 1);
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	//n->offset[0] = 0.3; n->offset[1] = 0.7; n->offset[2] = 9;
	n->euler[0] = 0; n->euler[1] = 0; n->euler[2] = 30;
	n->freuler[0][0] = 35; n->freuler[0][1] = 0; n->freuler[0][2] = 0;
	n->parent = NULL;
	//n->froset[1][0] = 0.3; n->froset[1][1] = 0.4; n->froset[1][2] = 1.1;
	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n, pos, 0);
	CHECK_CLOSE(-0.500, pos[0], d);
	CHECK_CLOSE( 0.709, pos[1], d);
	CHECK_CLOSE( 0.497, pos[2], d);
}


TEST(freulerEulerXZX)
{
	NODE *n = new NODE;
	SetupFrames(n, 1);
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	n->euler[0] = 30; n->euler[1] = 0; n->euler[2] = 30;
	n->freuler[0][0] = 35; n->freuler[0][1] = 0; n->freuler[0][2] = 0;
	n->parent = NULL;
	
	float pos[3];
   	pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n, pos, 0);
	CHECK_CLOSE(-0.433, pos[0], d);
	CHECK_CLOSE( 0.328, pos[1], d);
	CHECK_CLOSE( 0.840, pos[2], d);

	n->freuler[0][0] = 35; n->freuler[0][1] = 0; n->freuler[0][2] = 20;
   	pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n, pos, 0);
	CHECK_CLOSE(-0.519, pos[0], d);
	CHECK_CLOSE( 0.160, pos[1], d);
	CHECK_CLOSE( 0.840, pos[2], d);
}


TEST(2Nodefroffset)
{
	NODE *n = new NODE;
	NODE *n2 = new NODE;
	SetupFrames(n, 1);
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	n->froset[0][0] = 0.3; n->froset[0][1] = 0.4; n->froset[0][2] = 1.1;
	SetupChildren(n, 1);
	n->children[0] = n2;
	n->parent = NULL;
	
	SetupFrames(n2, 1);
	n2->name = "node2";
	n2->length[0] = 0; n2->length[1] = 1; n2->length[2] = 0;
	n2->froset[0][0] = 0.3; n2->froset[0][1] = 0.4; n2->froset[0][2] = 1.1;
	n2->parent = n;
	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n2, pos, 0);
	CHECK_CLOSE( 0.600, pos[0], d);
	CHECK_CLOSE( 2.800, pos[1], d);
	CHECK_CLOSE( 2.200, pos[2], d);
}


TEST(2Nodefreuler)
{
	NODE *n = new NODE;
	NODE *n2 = new NODE;
	SetupFrames(n, 1);
	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	//n->froset[0][0] = 0.3; n->froset[0][1] = 0.4; n->froset[0][2] = 1.1;
	n->freuler[0][0] = 30; n->freuler[0][1] = 130; n->freuler[0][2] = -45;
	n->parent = NULL;
	SetupChildren(n, 1);
	n->children[0] = n2;
	
	SetupFrames(n2, 1);
	n2->name = "node2";
	n2->length[0] = 0; n2->length[1] = 1; n2->length[2] = 0;
	n2->freuler[0][0] = 100; n2->freuler[0][1] = -60; n2->freuler[0][2] = 35;
	//n2->froset[0][0] = 0.3; n2->froset[0][1] = 0.4; n2->froset[0][2] = 1.1;
	n2->parent = n;
	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n2, pos, 0);
	CHECK_CLOSE( 0.655, pos[0], d);
	CHECK_CLOSE(-0.551, pos[1], d);
	CHECK_CLOSE( 0.066, pos[2], d);
}

TEST(3NodesfrEuler)
{
	NODE *n = new NODE;
	NODE *n2 = new NODE;
	NODE *n3 = new NODE;
	n->name = "node1";
	SetupFrames(n, 1);
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	//n->offset[0] = 0.1; n->offset[1] = 1.2; n->offset[2] = 2.2;
	n->freuler[0][0] = 35; n->freuler[0][1] = 0; n->freuler[0][2] = 30;
	SetupChildren(n, 1);
	n->children[0] = n2;
	n->parent = NULL;

	n2->name = "node2";
	SetupFrames(n2, 1);
	n2->length[0] = 0; n2->length[1] = 1; n2->length[2] = 0;
	//n2->offset[0] = 0.2; n2->offset[1] = 0.2; n2->offset[2] = 0.2;
	n2->freuler[0][0] = 30; n2->freuler[0][1] = 0; n2->freuler[0][2] = 10;
	SetupChildren(n2, 1);
	n2->children[0] = n3; n2->parent = n;

	n3->name = "node3";
	SetupFrames(n3, 1);
	n3->length[0] = 0; n3->length[1] = 1; n3->length[2] = 0;
	//n3->offset[0] = 0.3; n3->offset[1] = 0.7; n3->offset[2] = 0.3;
	n3->freuler[0][0] = -40; n3->freuler[0][1] = 0; n3->freuler[0][2] = 70;
	n3->parent = n2;


	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n3, pos, 0);

	CHECK_CLOSE(-1.734, pos[0], d);
	CHECK_CLOSE(1.095, pos[1], d);
	CHECK_CLOSE(1.362, pos[2], d);
}

TEST(2NodeBothEuler)
{
	NODE *n = new NODE;
	NODE *n2 = new NODE;
	SetupFrames(n, 1);

	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	n->euler[0] = 0; n->euler[1] = 0; n->euler[2] = 30;
	n->freuler[0][0] = 0; n->freuler[0][1] = 0; n->freuler[0][2] = 20;
	SetupChildren(n, 1);
	n->children[0] = n2;
	n->parent = NULL;
	
	SetupFrames(n2, 1);
	n2->name = "node2";
	n2->length[0] = 0; n2->length[1] = 1; n2->length[2] = 0;
	n2->euler[0] = 35; n2->euler[1] = 0; n2->euler[2] = 0;
	n2->freuler[0][0] = 40; n2->freuler[0][1] = 0; n2->freuler[0][2] = 0;
	//n2->froset[0][0] = 0.3; n2->froset[0][1] = 0.4; n2->froset[0][2] = 1.1;
	n2->parent = n;
	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n2, pos, 0);
	CHECK_CLOSE(-0.964, pos[0], d);
	CHECK_CLOSE( 0.809, pos[1], d);
	CHECK_CLOSE( 0.966, pos[2], d);
}

TEST(2NodeBothEulerHarder)
{
	NODE *n = new NODE;
	NODE *n2 = new NODE;
	SetupFrames(n, 1);

	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	n->euler[0] = 20; n->euler[1] = 0; n->euler[2] = 30;
	n->freuler[0][0] = 55; n->freuler[0][1] = 0; n->freuler[0][2] = 20;
	SetupChildren(n, 1);
	n->children[0] = n2;
	n->parent = NULL;
	
	SetupFrames(n2, 1);
	n2->name = "node2";
	n2->length[0] = 0; n2->length[1] = 1; n2->length[2] = 0;
	n2->euler[0] = 35; n2->euler[1] = 0; n2->euler[2] = 0;
	n2->freuler[0][0] = 40; n2->freuler[0][1] = 0; n2->freuler[0][2] = 0;
	//n2->froset[0][0] = 0.3; n2->froset[0][1] = 0.4; n2->froset[0][2] = 1.1;
	n2->parent = n;
	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n2, pos, 0);
	CHECK_CLOSE(-0.170, pos[0], d);
	CHECK_CLOSE(-0.778, pos[1], d);
	CHECK_CLOSE( 1.372, pos[2], d);
}

TEST(2NodeBothEulerEvenHarder)
{
	NODE *n = new NODE;
	NODE *n2 = new NODE;
	SetupFrames(n, 1);

	n->name = "node1";
	n->length[0] = 0; n->length[1] = 1; n->length[2] = 0;
	n->euler[0] = 20; n->euler[1] = 0; n->euler[2] = 30;
	n->freuler[0][0] = 55; n->freuler[0][1] = 0; n->freuler[0][2] = 20;
	SetupChildren(n, 1);
	n->children[0] = n2;
	n->parent = NULL;
	
	SetupFrames(n2, 1);
	n2->name = "node2";
	n2->length[0] = 0; n2->length[1] = 1; n2->length[2] = 0;
	n2->euler[0] = 35; n2->euler[1] = 0; n2->euler[2] = 10;
	n2->freuler[0][0] = 40; n2->freuler[0][1] = 0; n2->freuler[0][2] = 60;
	//n2->froset[0][0] = 0.3; n2->froset[0][1] = 0.4; n2->froset[0][2] = 1.1;
	n2->parent = n;
	
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n2, pos, 0);
	CHECK_CLOSE(-0.250, pos[0], d);
	CHECK_CLOSE(-0.937, pos[1], d);
	CHECK_CLOSE( 1.030, pos[2], d);

	n->freuler[0][0] = 55; n->freuler[0][1] = 30; n->freuler[0][2] = 20;
	pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(n2, pos, 0);
	CHECK_CLOSE( 0.304, pos[0], d);
	CHECK_CLOSE(-0.736, pos[1], d);
	CHECK_CLOSE( 1.170, pos[2], d);
}


int main()
{
	return UnitTest::RunAllTests();
}
