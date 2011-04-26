#include "nodeFuncsMulti.h"

#define s 2;
void incChildren(NODE * seg, int noof)
{
	seg->child = (NODE**) malloc(sizeof(NODE*)*noof);
}

void setupFigure(NODE** nl, int &lennl, TARGET** tl)
{
	NODE *hips = new NODE;
	NODE *chest = new NODE;
	NODE *neck = new NODE;
	NODE *head = new NODE;

	NODE *leftcollar = new NODE;
	NODE *leftuparm = new NODE;
	NODE *leftlowarm = new NODE;
	NODE *lefthand = new NODE;
	lefthand->target = tl[0];
	
	NODE *rightcollar = new NODE;
	NODE *rightuparm = new NODE;
	NODE *rightlowarm = new NODE;
	NODE *righthand = new NODE;
	righthand->target = tl[1];

	NODE *leftupleg = new NODE;
	NODE *leftlowleg = new NODE;
	NODE *leftfoot = new NODE;

	NODE *rightupleg = new NODE;
	NODE *rightlowleg = new NODE;
	NODE *rightfoot = new NODE;



	hips->name = "Hips";
	hips->length[0] = 0*s; hips->length[1] = 0.188*s; hips->length[2] = 0;
	hips->offset[1] = 1*s;
	hips->parent = NULL;
	hips->noofchildren = 3;
	incChildren(hips, hips->noofchildren);
	hips->child[0] = chest;
	hips->child[1] = leftupleg;
	hips->child[2] = rightupleg;

	chest->name = "Chest";
	chest->length[0] = 0; chest->length[1] = 0.429*s; chest->length[2] = 0;
	chest->parent = hips;
	chest->noofchildren = 3;
	incChildren(chest, chest->noofchildren);
	chest->child[0] = neck;
	chest->child[1] = leftcollar;
	chest->child[2] = rightcollar;


	neck->name = "Neck";
	neck->length[0] = 0; neck->length[1] = 0.079*s; neck->length[2] = 0;
	neck->parent = chest;
	neck->noofchildren = 1;
	incChildren(neck, neck->noofchildren);
	neck->child[0] = head;

	head->name = "Head";
	head->length[0]; head->length[1] = 0.135*s; head->length[2] = 0;
	head->parent = chest;
	head->noofchildren = 0;

	leftcollar->name = "leftCollar";
	leftcollar->length[0] = 0.137*s; leftcollar->length[1] = 0.061*s; leftcollar->length[2] = -0.055*s;
	leftcollar->offset[0] = 0.034*s; leftcollar->offset[1] = 0; leftcollar->offset[2] = 0.056*s;
	leftcollar->parent = chest;
	leftcollar->noofchildren = 1;
	incChildren(leftcollar, leftcollar->noofchildren);
	leftcollar->child[0] = leftuparm;

	leftuparm->name = "LeftUpArm";
	leftuparm->length[0] = 0.007;
	leftuparm->length[1] = -0.310;
	leftuparm->length[2] = -0.002;
	leftuparm->offset[0] = 0; leftuparm->offset[1] = 0; leftuparm->offset[2] = 0;
	leftuparm->parent = leftcollar;
	leftuparm->noofchildren = 1;
	incChildren(leftuparm, leftuparm->noofchildren);
	leftuparm->child[0] = leftlowarm;



	leftlowarm->name = "leftLowArm";
	leftlowarm->length[0] = 0.002; leftlowarm->length[1] = -0.311*s; leftlowarm->length[2] = 0.065*s;
	leftlowarm->offset[0] = 0; leftlowarm->offset[1] = 0; leftlowarm->offset[2] = 0;
	leftlowarm->parent = leftuparm;
	leftlowarm->noofchildren = 1;
	incChildren(leftlowarm, leftlowarm->noofchildren);
	leftlowarm->child[0] = lefthand;

	lefthand->name = "leftHand";
	lefthand->parent = leftlowarm;
	lefthand->noofchildren = 0;
	lefthand->length[0] = 0.004*s; lefthand->length[1] = -0.200*s; lefthand->length[2] = 0.051*s;
	lefthand->offset[0] = 0; lefthand->offset[1] = 0; lefthand->offset[2] = 0;




	rightcollar->name = "rightCollar";
	rightcollar->length[0] = -0.141*s; rightcollar->length[1] = 0.061*s; rightcollar->length[2] = -0.054*s;
	rightcollar->offset[0] = -0.034*s; rightcollar->offset[1] = 0; rightcollar->offset[2] = 0.056*s;
	rightcollar->parent = chest;
	rightcollar->noofchildren = 1;
	incChildren(rightcollar, rightcollar->noofchildren);
	rightcollar->child[0] = rightuparm;

	rightuparm->name = "rightUpArm";
	rightuparm->length[0] = -0.008*s; rightuparm->length[1] = -0.313*s; rightuparm->length[2] = -0.002*s;
	rightuparm->noofchildren = 1;
	rightuparm->parent = rightcollar;
	incChildren(rightuparm, rightuparm->noofchildren);
	rightuparm->child[0] = rightlowarm;

	rightlowarm->name = "rightLowArm";
	rightlowarm->length[0] = 0.00; rightlowarm->length[1] = -0.308*s; rightlowarm->length[2] = 0.065*s;
	rightlowarm->parent = rightuparm;
	rightlowarm->noofchildren = 1;
	incChildren(rightlowarm, rightlowarm->noofchildren);
	rightlowarm->child[0] = righthand;

	righthand->name = "rightHand";
	righthand->parent = rightlowarm;
	righthand->noofchildren = 0;
	righthand->length[0] = -0.004*s; righthand->length[1] = -0.200*s; righthand->length[2] = 0.051*s;



//////////////////////////////////////////////////
	leftupleg->name = "LeftUpLeg";
	leftupleg->length[0] = -0.013*s; leftupleg->length[1] = -0.527*s; leftupleg->length[2] =  0.051*s;
	leftupleg->offset[0] = 0.117*s;
	leftupleg->offset[1] = -0.188*s;
	leftupleg->parent = hips;
	leftupleg->noofchildren = 1;
	incChildren(leftupleg, leftupleg->noofchildren);
	leftupleg->child[0] = leftlowleg;

	leftlowleg->name = "LeftLowLeg";
	leftlowleg->length[0] = -0.001*s; leftlowleg->length[1] = -0.515*s; leftlowleg->length[2] =  -0.044*s;
	leftlowleg->parent = leftupleg;
	leftlowleg->noofchildren = 1;
	incChildren(leftlowleg, leftlowleg->noofchildren);
	leftlowleg->child[0] = leftfoot;

	leftfoot->name = "LeftFoot";
	leftfoot->length[0] = 0;
	leftfoot->length[1] = -0.118*s;
	leftfoot->length[2] = 0.157*s;
	leftfoot->parent = leftlowleg;
	leftfoot->noofchildren = 0;

	rightupleg->name = "rightUpLeg";
	rightupleg->length[0] = 0.013*s; rightupleg->length[1] = -0.528*s; rightupleg->length[2] =  0.051*s;
	rightupleg->offset[0] = -0.117*s;
	rightupleg->offset[1] = -0.188*s;
	rightupleg->parent = hips;
	rightupleg->noofchildren = 1;
	incChildren(rightupleg, rightupleg->noofchildren);
	rightupleg->child[0] = rightlowleg;

	rightlowleg->name = "rightLowLeg";
	rightlowleg->length[0] = 0.00; rightlowleg->length[1] = -0.514*s; rightlowleg->length[2] =  -0.044*s;
	rightlowleg->parent = rightupleg;
	rightlowleg->noofchildren = 1;
	incChildren(rightlowleg, rightlowleg->noofchildren);
	rightlowleg->child[0] = rightfoot;

	rightfoot->name = "rightFoot";
	rightfoot->length[0] = 0;
	rightfoot->length[1] = -0.121*s;
	rightfoot->length[2] = 0.157*s;
	rightfoot->parent = rightlowleg;
	rightfoot->noofchildren = 0;




	nl[lennl++] = hips;
	nl[lennl++] = chest;
	nl[lennl++] = neck;

	nl[lennl++] = leftcollar;
	nl[lennl++] = leftuparm;
	nl[lennl++] = leftlowarm;
	nl[lennl++] = lefthand;

	nl[lennl++] = rightcollar;
	nl[lennl++] = rightuparm;
	nl[lennl++] = rightlowarm;
	nl[lennl++] = righthand;

	nl[lennl++] = leftupleg;
	nl[lennl++] = leftlowleg;
	nl[lennl++] = leftfoot;
	nl[lennl++] = rightupleg;
	nl[lennl++] = rightlowleg;
	nl[lennl++] = rightfoot;

}

