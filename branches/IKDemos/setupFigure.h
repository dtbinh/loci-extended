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
	hips->offset[0] = 0; hips->offset[1] = 1*s; hips->offset[2] = 0;
	hips->euler[0] = 0; hips->euler[1] = 0; hips->euler[2] = 0;
	hips->parent = NULL;
	hips->noofchildren = 3;
	incChildren(hips, hips->noofchildren);
	hips->child[0] = chest;
	hips->child[1] = leftupleg;
	hips->child[2] = rightupleg;
	hips->limXmin = -360; hips->limXmax = 360;
	hips->limYmin = -360; hips->limYmax = 360;
	hips->limZmin = -360; hips->limZmax = 360;

	chest->name = "Chest";
	chest->length[0] = 0; chest->length[1] = 0.429*s; chest->length[2] = 0;
	chest->offset[0] = 0; chest->offset[1] = 0; chest->offset[2] = 0;
	chest->euler[0] = 0; chest->euler[1] = 0; chest->euler[2] = 0;
	chest->parent = hips;
	chest->noofchildren = 3;
	incChildren(chest, chest->noofchildren);
	chest->child[0] = neck;
	chest->child[1] = leftcollar;
	chest->child[2] = rightcollar;
	chest->limXmin = -360; chest->limXmax = 360;
	chest->limYmin = -360; chest->limYmax = 360;
	chest->limZmin = -360; chest->limZmax = 360;


	neck->name = "Neck";
	neck->length[0] = 0; neck->length[1] = 0.079*s; neck->length[2] = 0;
	neck->offset[0] = 0; neck->offset[1] = 0; neck->offset[2] = 0;
	neck->euler[0] = 0; neck->euler[1] = 0; neck->euler[2] = 0;
	neck->parent = chest;
	neck->noofchildren = 1;
	incChildren(neck, neck->noofchildren);
	neck->child[0] = head;
	neck->limXmin = -360; neck->limXmax = 360;
	neck->limYmin = -360; neck->limYmax = 360;
	neck->limZmin = -360; neck->limZmax = 360;

	head->name = "Head";
	head->length[0]; head->length[1] = 0.135*s; head->length[2] = 0;
	head->offset[0] = 0; head->offset[1] = 0; head->offset[2] = 0;
	head->euler[0] = 0; head->euler[1] = 0; head->euler[2] = 0;
	neck->parent = chest;
	head->parent = chest;
	head->noofchildren = 0;
	head->limXmin = -360; head->limXmax = 360;
	head->limYmin = -360; head->limYmax = 360;
	head->limZmin = -360; head->limZmax = 360;

	leftcollar->name = "leftCollar";
	leftcollar->length[0] = 0.137*s; leftcollar->length[1] = 0.061*s; leftcollar->length[2] = -0.055*s;
	leftcollar->offset[0] = 0.034*s; leftcollar->offset[1] = 0; leftcollar->offset[2] = 0.056*s;
	leftcollar->euler[0] = 0; leftcollar->euler[1] = 0; leftcollar->euler[2] = 0;
	leftcollar->parent = chest;
	leftcollar->noofchildren = 1;
	incChildren(leftcollar, leftcollar->noofchildren);
	leftcollar->child[0] = leftuparm;
	leftcollar->limXmin = -360; leftcollar->limXmax = 360;
	leftcollar->limYmin = -360; leftcollar->limYmax = 360;
	leftcollar->limZmin = -360; leftcollar->limZmax = 360;

	leftuparm->name = "LeftUpArm";
	leftuparm->length[0] = 0.007;
	leftuparm->length[1] = -0.310;
	leftuparm->length[2] = -0.002;
	leftuparm->offset[0] = 0; leftuparm->offset[1] = 0; leftuparm->offset[2] = 0;
	leftuparm->euler[0] = 0; leftuparm->euler[1] = 0; leftuparm->euler[2] = 0;
	leftuparm->parent = leftcollar;
	leftuparm->noofchildren = 1;
	incChildren(leftuparm, leftuparm->noofchildren);
	leftuparm->child[0] = leftlowarm;
	leftuparm->limXmin = -360; leftuparm->limXmax = 360;
	leftuparm->limYmin = -360; leftuparm->limYmax = 360;
	leftuparm->limZmin = -360; leftuparm->limZmax = 360;



	leftlowarm->name = "leftLowArm";
	leftlowarm->length[0] = 0.002; leftlowarm->length[1] = -0.311*s; leftlowarm->length[2] = 0.065*s;
	leftlowarm->offset[0] = 0; leftlowarm->offset[1] = 0; leftlowarm->offset[2] = 0;
	leftlowarm->euler[0] = 0; leftlowarm->euler[1] = 0; leftlowarm->euler[2] = 0;
	leftlowarm->parent = leftuparm;
	leftlowarm->noofchildren = 1;
	incChildren(leftlowarm, leftlowarm->noofchildren);
	leftlowarm->child[0] = lefthand;
	leftlowarm->limXmin = -360; leftlowarm->limXmax = 360;
	leftlowarm->limYmin = -360; leftlowarm->limYmax = 360;
	leftlowarm->limZmin = -360; leftlowarm->limZmax = 360;

	lefthand->name = "leftHand";
	lefthand->parent = leftlowarm;
	lefthand->noofchildren = 0;
	lefthand->length[0] = 0.004*s; lefthand->length[1] = -0.200*s; lefthand->length[2] = 0.051*s;
	lefthand->offset[0] = 0; lefthand->offset[1] = 0; lefthand->offset[2] = 0;
	lefthand->euler[0] = 0; lefthand->euler[1] = 0; lefthand->euler[2] = 0;
	lefthand->limXmin = -360; lefthand->limXmax = 360;
	lefthand->limYmin = -360; lefthand->limYmax = 360;
	lefthand->limZmin = -360; lefthand->limZmax = 360;




	rightcollar->name = "rightCollar";
	rightcollar->length[0] = -0.141*s; rightcollar->length[1] = 0.061*s; rightcollar->length[2] = -0.054*s;
	rightcollar->offset[0] = -0.034*s; rightcollar->offset[1] = 0; rightcollar->offset[2] = 0.056*s;
	rightcollar->euler[0] = 0; rightcollar->euler[1] = 0; rightcollar->euler[2] = 0;
	rightcollar->parent = chest;
	rightcollar->noofchildren = 1;
	incChildren(rightcollar, rightcollar->noofchildren);
	rightcollar->child[0] = rightuparm;
	rightcollar->limXmin = -20; rightcollar->limXmax = 20;
	rightcollar->limYmin = -45; rightcollar->limYmax = 17;
	rightcollar->limZmin = -30; rightcollar->limZmax = 16;

	rightuparm->name = "rightUpArm";
	rightuparm->length[0] = -0.008*s; rightuparm->length[1] = -0.313*s; rightuparm->length[2] = -0.002*s;
	rightuparm->offset[0] = 0; rightuparm->offset[1] = 0; rightuparm->offset[2] = 0;
	rightuparm->euler[0] = 0; rightuparm->euler[1] = 0; rightuparm->euler[2] = 0;
	rightuparm->noofchildren = 1;
	rightuparm->parent = rightcollar;
	incChildren(rightuparm, rightuparm->noofchildren);
	rightuparm->child[0] = rightlowarm;
	rightuparm->limXmin = -360; rightuparm->limXmax = 360;
	rightuparm->limYmin = -90; rightuparm->limYmax = 90;
	rightuparm->limZmin = -90; rightuparm->limZmax = 90;

	rightlowarm->name = "rightLowArm";
	rightlowarm->length[0] = 0.00; rightlowarm->length[1] = -0.308*s; rightlowarm->length[2] = 0.065*s;
	rightlowarm->offset[0] = 0; rightlowarm->offset[1] = 0; rightlowarm->offset[2] = 0;
	rightlowarm->euler[0] = 0; rightlowarm->euler[1] = 0; rightlowarm->euler[2] = 0;
	rightlowarm->parent = rightuparm;
	rightlowarm->noofchildren = 1;
	incChildren(rightlowarm, rightlowarm->noofchildren);
	rightlowarm->child[0] = righthand;
	rightlowarm->limXmin = -20; rightlowarm->limXmax = 20;
	rightlowarm->limYmin = -20; rightlowarm->limYmax = 20;
	rightlowarm->limZmin = -120; rightlowarm->limZmax = 0;

	righthand->name = "rightHand";
	righthand->parent = rightlowarm;
	righthand->noofchildren = 0;
	righthand->length[0] = -0.004*s; righthand->length[1] = -0.200*s; righthand->length[2] = 0.051*s;
	righthand->offset[0] = 0; righthand->offset[1] = 0; righthand->offset[2] = 0;
	righthand->euler[0] = 0; righthand->euler[1] = 0; righthand->euler[2] = 0;
	//righthand->limXmin = -20; righthand->limXmax = 20;
	//righthand->limYmin = -80; righthand->limYmax = 80;
	//righthand->limZmin = -20; righthand->limZmax = 20;



//////////////////////////////////////////////////
	leftupleg->name = "LeftUpLeg";
	leftupleg->length[0] = -0.013*s; leftupleg->length[1] = -0.527*s; leftupleg->length[2] =  0.051*s;
	leftupleg->offset[0] = 0.117*s;
	leftupleg->offset[1] = -0.188*s;
	leftupleg->offset[2] = 0;
	leftupleg->parent = hips;
	leftupleg->noofchildren = 1;
	incChildren(leftupleg, leftupleg->noofchildren);
	leftupleg->child[0] = leftlowleg;
	leftupleg->limXmin = -360; leftupleg->limXmax = 360;
	leftupleg->limYmin = -360; leftupleg->limYmax = 360;
	leftupleg->limZmin = -360; leftupleg->limZmax = 360;

	leftlowleg->name = "LeftLowLeg";
	leftlowleg->length[0] = -0.001*s; leftlowleg->length[1] = -0.515*s; leftlowleg->length[2] =  -0.044*s;
	leftlowleg->offset[0] = 0; leftlowleg->offset[1] = 0; leftlowleg->offset[2] = 0;
	leftlowleg->parent = leftupleg;
	leftlowleg->noofchildren = 1;
	incChildren(leftlowleg, leftlowleg->noofchildren);
	leftlowleg->child[0] = leftfoot;
	leftlowleg->limXmin = -360; leftlowleg->limXmax = 360;
	leftlowleg->limYmin = -360; leftlowleg->limYmax = 360;
	leftlowleg->limZmin = -360; leftlowleg->limZmax = 360;

	leftfoot->name = "LeftFoot";
	leftfoot->length[0] = 0;
	leftfoot->length[1] = -0.118*s;
	leftfoot->length[2] = 0.157*s;
	leftfoot->offset[0] = 0; leftfoot->offset[1] = 0; leftfoot->offset[2] = 0;
	leftfoot->parent = leftlowleg;
	leftfoot->noofchildren = 0;
	leftfoot->limXmin = -360; leftfoot->limXmax = 360;
	leftfoot->limYmin = -360; leftfoot->limYmax = 360;
	leftfoot->limZmin = -360; leftfoot->limZmax = 360;

	rightupleg->name = "rightUpLeg";
	rightupleg->length[0] = 0.013*s; rightupleg->length[1] = -0.528*s; rightupleg->length[2] =  0.051*s;
	rightupleg->offset[0] = -0.117*s;
	rightupleg->offset[1] = -0.188*s;
	rightupleg->offset[2] = 0;
	rightupleg->parent = hips;
	rightupleg->noofchildren = 1;
	incChildren(rightupleg, rightupleg->noofchildren);
	rightupleg->child[0] = rightlowleg;
	rightupleg->limXmin = -360; rightupleg->limXmax = 360;
	rightupleg->limYmin = -360; rightupleg->limYmax = 360;
	rightupleg->limZmin = -360; rightupleg->limZmax = 360;

	rightlowleg->name = "rightLowLeg";
	rightlowleg->length[0] = 0.00; rightlowleg->length[1] = -0.514*s; rightlowleg->length[2] =  -0.044*s;
	rightlowleg->offset[0] = 0; rightlowleg->offset[1] = 0; rightlowleg->offset[2] = 0;
	rightlowleg->parent = rightupleg;
	rightlowleg->noofchildren = 1;
	incChildren(rightlowleg, rightlowleg->noofchildren);
	rightlowleg->child[0] = rightfoot;
	rightlowleg->limXmin = -360; rightlowleg->limXmax = 360;
	rightlowleg->limYmin = -360; rightlowleg->limYmax = 360;
	rightlowleg->limZmin = -360; rightlowleg->limZmax = 360;

	rightfoot->name = "rightFoot";
	rightfoot->length[0] = 0;
	rightfoot->length[1] = -0.121*s;
	rightfoot->length[2] = 0.157*s;
	rightfoot->offset[0] = 0; rightfoot->offset[1] = 0; rightfoot->offset[2] = 0;
	rightfoot->parent = rightlowleg;
	rightfoot->noofchildren = 0;
	rightfoot->limXmin = -360; rightfoot->limXmax = 360;
	rightfoot->limYmin = -360; rightfoot->limYmax = 360;
	rightfoot->limZmin = -360; rightfoot->limZmax = 360;




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

