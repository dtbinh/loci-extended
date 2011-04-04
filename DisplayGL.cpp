#include "DisplayGL.h"
#include <iostream>
#include <cmath>

DisplayGL::DisplayGL() : body(0), header(0), m_pDC(0), lasttime(::GetTickCount()), frames(0)
{
  MessageBoxa("DisaplayGL Initialiser");
  fps[0] = '\0';
  centre[0] = centre[1] = centre[2] = 0.0f;
  Init();
}

DisplayGL::~DisplayGL() {}

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


void DisplayGL::Draw()
{
  //MessageBoxa("DRAW");
  glPushMatrix();

   // glTranslatef(0.0f, 0.0f, 0.0f);
   // glRotatef(centre[0], 1.0f, 0.0f, 0.0f);
   // glRotatef(centre[1], 0.0f, 1.0f, 0.0f);
   // glRotatef(centre[2], 0.0f, 0.0f, 1.0f);


  /*
  std::cout << "Header euler" << std::endl;
  std::cout << header->euler[0][0] << " " << header->euler[0][1] << " " << header->euler[0][2] << std::endl;
  std::cout << header->euler[1][0] << " " << header->euler[1][1] << " " << header->euler[1][2] << std::endl;
  std::cout << header->euler[2][0] << " " << header->euler[2][1] << " " << header->euler[2][2] << std::endl;
*/	
  
	//for (int i = 0; i < header->noofeffectors; i++)
	//	{
		//	if (!strcmp(effectorlist[i]->name, "Head")) 
		//	{			
			//	if ( header->euler[0][1] = 1 ) { effectorlist[i]->euler[0] = 45; }
				//if ( header->euler[1][1] = 1 ) { effectorlist[i]->euler[1] = 45; }
				//if ( header->euler[2][1] = 1 ) { effectorlist[i]->euler[2] = 45; }
		//	}	
		//}
  

  glPushMatrix();
	if (body)
	  EvaluateChildren(body);
  glPopMatrix();


  //Draw Positions of end effectors
  
  
	if (body)
	{
		for (int i = 0; i < header->noofeffectors; i++)
		//for (int i = 0; i < 1 ; i++)
		{
			float *pos = new float[3];
			pos[0] = 0; pos[1] = 0; pos[2] = 0;
			nodeLocalPos(effectorlist[i], header->currentframe, pos);

			std::cout << i << "LocalPos="<< pos[0] << ", " << pos[1] << ", " << pos[2] << "\n" << std::endl;
			glPushMatrix();
			  glColor3f(0, 0, 0.8f);
			  glTranslatef(pos[0], pos[1], pos[2]);
			  glutSolidSphere(0.05, 5, 5);
			glPopMatrix();
			
			delete[] pos;
		}
		
	}
	

  glPopMatrix();
}
/*
 glPushMatrix();
   // glTranslatef(node->offset[0] + node->froset[header->currentframe][0], node->offset[1] + node->froset[header->currentframe][1], node->offset[2] + node->froset[header->currentframe][2]);

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
*/
//#define PI 3.141592653f
void DisplayGL::nodeLocalPos(NODE* seg, long currentframe, float *pos)
{
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
