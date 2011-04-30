#include "DisplayGL.h"
#include "ccdFuncs.h"
#include <iostream>
#include <cmath>

int nooftargets = 0;
TARGET *targetList[5];
bool doJac = false;

DisplayGL::DisplayGL() : body(0), header(0), m_pDC(0), lasttime(::GetTickCount()), frames(0)
{
  MessageBoxa("DisaplayGL Initialiser");
  fps[0] = '\0';
  centre[0] = centre[1] = centre[2] = 0.0f;
  targetList[0] = new TARGET;
  targetList[0]->name = "target1";
  targetList[0]->pos[0] = 0;
  targetList[0]->pos[1] = 0;
  targetList[0]->pos[2] = 0;
  nooftargets++;
  Init();
}


DisplayGL::~DisplayGL() {}

void DisplayGL::moveTarget(unsigned char key)
{
	if (key == 'j') { targetList[0]->pos[0] -= 0.1;	}
	if (key == 'l') { targetList[0]->pos[0] += 0.1;	}
	if (key == 'k') { targetList[0]->pos[1] -= 0.1;	}
	if (key == 'i') { targetList[0]->pos[1] += 0.1;	}
	if (key == 'u') { targetList[0]->pos[2] += 0.1;	}
	if (key == 'o') { targetList[0]->pos[2] -= 0.1;	}
}

void DisplayGL::Init()
{
 // MessageBoxa("DisplayGL.Init");
  GLfloat	fAspect;

  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  
  //CRect rect;
  int rect[4];
  glGetIntegerv(GL_VIEWPORT, rect);
 // GetClientRect();
  if (rect[3])
	  fAspect = (GLfloat)rect[2]/rect[3];
	else
	  fAspect = 1.0f;
 // std::cout << "ASPECT" << fAspect << std::endl;
	
  glViewport(0, 0, rect[3], rect[4]);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0f, fAspect, 0.0f, 10.0f);
  glMatrixMode(GL_MODELVIEW);

 // MessageBoxa("End of DisplayGL.Init");
}

void setupCCD(NODE *start, TARGET *target)
{
	NODE *end = getFirstEndEffector(start);
	NODE *node = end;
	std::cout << "setupCCD: root=" << start->name << " target=" << target->name << " end=" << end->name << std::endl;
	int count = 0;
	std::cout << distToTarget(end, target) << std::endl;
	while ((count < 30) && (distToTarget(end, target) > 0.05))
	{
		while (node)
		{
			std::cout << node->name << std::endl;
			CCD(node, target);
			node = node->parent;
		}
		node = end;
		count++;
	}
}

void doIK(NODE *start, NODE *end)
{
		int count = 0;
		if (doJac)
		{
			bool c = false;
			while ((count < 30) && (c == false))
			{
				std::cout << "Jacobian Not Implemented" << std::endl;
				//c = jacobianSing(start);
				count ++;
			}
		} else {
			std::cout << "Else " << std::endl;

			std::cout << end->name << " " << distToTarget(end) << std::endl;
			while ((count < 30) && (distToTarget(end) > 0.05))
			{
				//std::cout << distToTarget(nList[i]) << std::endl;
				NODE *node = end;
				do 
				{
					CCD(node, targetList[0]);
					node = node->parent;
				} while (node != start->parent);
				count++;
			}
		}

}


void DisplayGL::Draw()
{
	std::cout << header->euler[0][0] << " ";
	std::cout << header->euler[0][1] << " ";
	std::cout << header->euler[0][2] << std::endl;
	std::cout << header->euler[1][0] << " ";
	std::cout << header->euler[1][1] << " ";
	std::cout << header->euler[1][2] << std::endl;
	std::cout << header->euler[2][0] << " ";
	std::cout << header->euler[2][1] << " ";
	std::cout << header->euler[2][2] << std::endl;
  glPushMatrix();

  /*
  NODE *found = searchName(body, "RightCollar");
  if (found)
  {
  	//std::cout << "FOUND" << std::endl;	  
	setupCCD(found, targetList[0]);
  }
  */

  NODE *found = searchName(body, "Hips");
  if (found)
  {
  	//std::cout << "FOUND" << std::endl;	  
	//setupCCD(found, targetList[0]);
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(found, pos, header->currentframe);
	std::cout << header->currentframe << " Final hips Pos = " << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;
	glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2]);
		glColor3f(0, 0, 1);
		glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
  }
	glPushMatrix();
		glTranslatef(0, 0, 0);
		glColor3f(0, 0, 1);
		glutSolidSphere(0.05, 5, 5);
	glPopMatrix();


  found = NULL;
  found = searchName(body, "LeftHand");//"Head");
  if (found)
  {
	  found->target = targetList[0];
	doIK(found->parent->parent->parent, found);
	float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
	calcEndPos(found, pos, header->currentframe);
	//std::cout << header->currentframe << " Final head Pos = " << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;
	glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2]);
		glColor3f(0, 0, 1);
		glutSolidSphere(0.05, 5, 5);
	glPopMatrix();
  }

  glPushMatrix();
	if (body)
	  EvaluateChildren(body);
  glPopMatrix();

  for (int i=0; i<nooftargets; i++)
  {
	  TARGET *t = targetList[i];
	  glPushMatrix();
	    glColor3f(0, 1, 0);
	  	glTranslatef(t->pos[0], t->pos[1], t->pos[2]);
		glutWireSphere(0.1, 5, 5);
	  glPopMatrix();
  }



  glPopMatrix();
}
//
//#define PI 3.141592653f
/*
void DisplayGL::nodeLocalPos(NODE* seg, long currentframe, float *pos)
{
	if (seg == NULL) { std::cout << "error: nodeLocalPos passed null node" << std::endl; return; }
	std::cout << "SegName = " <<  seg->name ;
	while (seg->parent->parent)
	{
		seg = seg->parent;
		pos[0] = seg->parent->flspace[currentframe][0]; // + seg->flspace[currentframe][0];
		pos[1] = seg->parent->flspace[currentframe][1]; // + seg->flspace[currentframe][1];
		pos[2] = seg->parent->flspace[currentframe][2]; // + seg->flspace[currentframe][2];
	}
	std::cout << currentframe << seg->name << std::endl;

	return;
} 
*/

void DisplayGL::SetupSegments(NODE* segs, MOCAPHEADER *header, NODE** effectorlist)
{
  MessageBoxa("SetupSegments");
  this->header = header;
  this->effectorlist = effectorlist;
  body = segs;
}



void DisplayGL::EvaluateChildren(NODE* node)
{
 // MessageBoxa("Evaluate Children");
  glPushMatrix();
    glTranslatef(node->offset[0] + node->froset[header->currentframe][0], node->offset[1] + node->froset[header->currentframe][1], node->offset[2] + node->froset[header->currentframe][2]);

    glRotatef(node->euler[0], (float) header->euler[0][0], (float) header->euler[0][1], (float) header->euler[0][2]);
    glRotatef(node->euler[1], (float) header->euler[1][0], (float) header->euler[1][1], (float) header->euler[1][2]);
    glRotatef(node->euler[2], (float) header->euler[2][0], (float) header->euler[2][1], (float) header->euler[2][2]);

    glRotatef(node->freuler[header->currentframe][0], (float) header->euler[0][0], (float) header->euler[0][1], (float) header->euler[0][2]);
    glRotatef(node->freuler[header->currentframe][1], (float) header->euler[1][0], (float) header->euler[1][1], (float) header->euler[1][2]);
    glRotatef(node->freuler[header->currentframe][2], (float) header->euler[2][0], (float) header->euler[2][1], (float) header->euler[2][2]);

    glBegin(GL_LINES);
      glColor3f(node->colour[0], node->colour[1], node->colour[2]);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(node->length[0] * node->scale[header->currentframe], node->length[1] * node->scale[header->currentframe], node->length[2] * node->scale[header->currentframe]);
    glEnd();


    if (node->children)
    {
      for (int i=0; i<node->noofchildren; i++)
        EvaluateChildren(node->children[i]);
    } else {
		//Node is an end Effector.
		glPushMatrix();	
			glTranslatef(node->length[0] * node->scale[header->currentframe], node->length[1] * node->scale[header->currentframe], node->length[2] * node->scale[header->currentframe]);
			glScalef(1, 0.2f, 1);
			glutSolidSphere(0.1f, 17, 17);
		glPopMatrix();

		if (!strcmp(node->name, "Head") || !strcmp(node->name, "head") )
		{
			glColor3f(0.5f, 0, 0);
			glPushMatrix();	
				glTranslatef(node->length[0] * node->scale[header->currentframe], node->length[1] * node->scale[header->currentframe]-0.05f, node->length[2] * node->scale[header->currentframe]+0.05f);
				glScalef(0.2f, 0.2f, 1.0f);
				glutSolidSphere(0.1f, 17, 17);
			glPopMatrix();		
				glPushMatrix();	
				glTranslatef(node->length[0] * node->scale[header->currentframe], node->length[1] * node->scale[header->currentframe]-0.05f, node->length[2] * node->scale[header->currentframe]);
				glScalef(0.5f, 0.5f, 0.5f);
				glutSolidSphere(0.1f, 17, 17);
			glPopMatrix();	
		}

	}

  glPopMatrix();
}
