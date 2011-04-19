/*************************************************************************
*************************************************************************/
#pragma warning(disable: 4996)

//memory leak debugger
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>


#include "Main.h"
//#include "resource.h"
#include "HTRFormat.h"
#include "BVHFormat.h"
#include <iostream>
#include <cstring>
#include <cstdio>

float SPEED = 1.0f;
/*

BEGIN_MESSAGE_MAP(MainWnd, CWnd)
  ON_WM_PAINT()
  ON_WM_CREATE()
  ON_WM_DESTROY()
  ON_WM_TIMER()
END_MESSAGE_MAP()
*/
static float yOffset = 0;

void reshape(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)width/(GLfloat)height, 0.01, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void init()
{
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
}

void drawPlane()
{
	glColor4f(0.6f, 0.8f, 0.8f, 0.9f);
	
	float s = 6;
	glBegin(GL_QUADS);
		glNormal3f(0, 1, 0); glVertex3f(-6, 0, -6);
		glNormal3f(0, 1, 0); glVertex3f( 6, 0, -6);
		glNormal3f(0, 1, 0); glVertex3f( 6, 0,  6);
		glNormal3f(0, 1, 0); glVertex3f(-6, 0,  6);
	glEnd();
}

void display()
{
	static long starttime = 0; //GetTickCount();
	  static bool bBusy = false;
  if(bBusy || !body) 
	  return;
  bBusy = true;
	

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();
	//light();

	gluLookAt(4, 2, 4, 0, 0, 0, 0, 1, 0);

	//Rotate the entire scene. Controlled by user's mouse
	glRotatef(centre[0], 1.0f, 0.0f, 0.0f);
    glRotatef(centre[1], 0.0f, 1.0f, 0.0f);
    glRotatef(centre[2], 0.0f, 0.0f, 1.0f);
	

	glEnable(GL_STENCIL_TEST);
	glColorMask(0, 0, 0, 0);
	glDisable(GL_DEPTH_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	drawPlane();

	glColorMask(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glColor3f(0.0f, 0.0f, 0.0f);
	

	glPushMatrix();
	  glScalef(1, -1, 1);
	  glTranslatef(0, yOffset, 0);
	  Output3D->Draw();
	glPopMatrix();

	glDisable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	drawPlane();
	glDisable(GL_BLEND);

	glTranslatef(0, yOffset, 0);
	Output3D->Draw();

	  DWORD time = ::GetTickCount();
  if (lasttime < time - 2000)
  {
    gcvt(frames * 1000/ (float) (time - lasttime), 6, fps);
    frames = 0;
    lasttime = time;
    char buff[40];
    strcpy(buff, "DisplayGL - ");
    strcat(buff, fps);
   // SetWindowText(buff);
	glutSetWindowTitle(buff);
  }
  else 
  { ++frames; }

	glutSwapBuffers();
	  bBusy = false;

	 // static long starttime = GetTickCount();
  header.currentframe = ((GetTickCount()/(int)SPEED - starttime) * header.datarate / 1000) % header.noofframes;

}

void keyFunc(unsigned char key, int x, int y)
{
	if (key == 0x1B) { std::cout << "ESC - Quitting" << std::endl; exit(0); }
}
void mouseFunc(int button, int state, int x, int y)
{
	if ((state == GLUT_DOWN) && ((button == GLUT_LEFT_BUTTON) || (button == GLUT_RIGHT_BUTTON)))
	{
		lastX = x; lastY = y;
		if (button == GLUT_LEFT_BUTTON) { buttonFlag = 1; }
		else if (button == GLUT_RIGHT_BUTTON) { buttonFlag = 2; }
	}
	else if (state == GLUT_UP) { buttonFlag = 0; }
}

void motionFunc(int x, int y)
{
	
  if (buttonFlag == 1)
  {
    centre[1]+=(x - lastX);
    centre[0]+=(y - lastY);
  }
  else if (buttonFlag == 2)
  {
    centre[2]-=(x - lastX);
  }
  lastX = x; lastY = y;
}

void closeFunc()
{
	MessageBoxa("GLUT CLOSEFUNC");
	for (int i=0; i<header.noofsegments; ++i)
	{
		for (long j=0; j<header.noofframes; ++j)
		{
			free(nodelist[i]->froset[j]);
			free(nodelist[i]->freuler[j]);
		}
		free(nodelist[i]->scale);
		free(nodelist[i]->froset);
		free(nodelist[i]->freuler);
		free(nodelist[i]->children);
		free(nodelist[i]->name);
		free(nodelist[i]);
	}
	free(nodelist);
}

int main (int argc, char **argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutCreateWindow("DisplayGL");

	init();
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
//	glutTimerFunc(OnTimer);
	//glutCloseFunc(closeFunc);


	const char* filename = "";
	yOffset = 0;
	for (int i=0; i < argc; i++)
	{
		//std::cout << "." << argv[i] << "." << std::endl;
		if (!strcmp(argv[i], "-f")) { filename = argv[i+1];	}
		if (!strcmp(argv[i], "-o")) { yOffset = (float)atof(argv[i+1]);	}
	} 
//	std::cout << "Loading Filename " << filename << std::endl;
	loadFilename(filename);


	glutMainLoop();
}

/*
BOOL Main::InitInstance(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
  
  void* m_pMainWnd = new MainWnd(""); //TODO Pass the command line arg of the mocap filename to open.

  return TRUE;
}

BOOL Main::OnIdle(LONG lCount)
{
  std::cout << "Main::OnIdle " << lCount << " " << WM_TIMER << std::endl;
  //PostMessage(m_pMainWnd->m_hWnd, WM_TIMER, 0, 0);
  return true;
}
*/

void loadFilename (const char* m_lpCmdLine)
{
 // int mainWindowId;
  const char *pathname = "Data/"; //= (char*) malloc(50);
  Output3D = new DisplayGL();



  // Setup the basic body structure ready for the system to build on it
  char mocapfilename[200];
  if (m_lpCmdLine[0])
  {
	  std::cout << "Loading filename " << m_lpCmdLine << std::endl;
    strcpy(mocapfilename,m_lpCmdLine);  // get file to load in from the command line parameters
	//yOffset = 1;
  }
  else  // no specified file so use a default one
  {
    strcpy(mocapfilename, pathname);
  //  strcat(mocapfilename, "goal.htr");
    //strcat(mocapfilename, "catch.htr");
     // strcat(mocapfilename, "dance01.bvh");
    strcat(mocapfilename, "vogueShort.bvh");

	yOffset = 1;
  }

  MocapData* mocapfile;
  if (strstrEx(mocapfilename, ".HTR")!=-1)
    mocapfile = new HTRFormat(&header);
  else if (strstrEx(mocapfilename, ".BVH")!=-1)
    mocapfile = new BVHFormat(&header);
  else
    mocapfile = 0;


  if (!mocapfile)
  {
    MessageBoxa("Unrecognised file extension.");
    exit(1);
  }
  else if (!mocapfile->ImportData(mocapfilename))
  {
    MessageBoxa(mocapfile->GetError());
    exit(1);
  }
  else
  {
    body = mocapfile->GetRootNode();
	nodelist = mocapfile->GetNodeList();
	effectorlist = mocapfile->GetEffectorList();
    header.currentframe = 0;
    
	Output3D->SetupSegments(body, &header, effectorlist);
	
    //Output3D->ShowWindow(SW_SHOW);
	MessageBoxa("End MainWind Initialiser");
  }
}

/*

MainWnd::~MainWnd()
{ OnDestroy(); }

int MainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  return 0;
}

void MainWnd::OnDestroy()
{
  for (int i=0; i<header.noofsegments; ++i)
  {
    for (long j=0; j<header.noofframes; ++j)
    {
      free(nodelist[i]->froset[j]);
      free(nodelist[i]->freuler[j]);
    }
    free(nodelist[i]->scale);
    free(nodelist[i]->froset);
    free(nodelist[i]->freuler);
    free(nodelist[i]->children);
    free(nodelist[i]->name);
    free(nodelist[i]);
  }
  free(nodelist);
}

void MainWnd::OnPaint()
{

}

void MainWnd::OnTimer(UINT nIDEvent)
{
  static long starttime = GetTickCount();
  header.currentframe = ((GetTickCount() - starttime) * header.datarate / 1000) % header.noofframes;

  Output3D->Draw();
}
*/
void OutputNodeList()
{
  FILE *output = fopen("Debug.txt", "wb");
  char buffer[20];
  char nline[2];
  nline[0] = '\r';
  nline[1] = '\n';
  for (int j=0; j<header.noofsegments; ++j)
  {
    fwrite("Name: ", 6,1,output);
    fwrite(nodelist[j]->name,strlen(nodelist[j]->name),1,output);
    fwrite(nline,2,1,output);
    fwrite("Length X: ", 10, 1, output);
    gcvt(nodelist[j]->length[0], 6, buffer);
    fwrite(buffer,strlen(buffer),1,output);
    fwrite(nline,2,1,output);
    fwrite("Length Y: ", 10, 1, output);
    gcvt(nodelist[j]->length[1], 6, buffer);
    fwrite(buffer,strlen(buffer),1,output);
    fwrite(nline,2,1,output);
    fwrite("Length Z: ", 10, 1, output);
    gcvt(nodelist[j]->length[2], 6, buffer);
    fwrite(buffer,strlen(buffer),1,output);

    fwrite(nline,2,1,output);
    fwrite("Offest X: ", 10, 1, output);
    gcvt(nodelist[j]->offset[0], 6, buffer);
    fwrite(buffer,strlen(buffer),1,output);
    fwrite(nline,2,1,output);
    fwrite("Offest Y: ", 10, 1, output);
    gcvt(nodelist[j]->offset[1], 6, buffer);
    fwrite(buffer,strlen(buffer),1,output);
    fwrite(nline,2,1,output);
    fwrite("Offest Z: ", 10, 1, output);
    gcvt(nodelist[j]->offset[2], 6, buffer);
    fwrite(buffer,strlen(buffer),1,output);

    fwrite(nline,2,1,output);
    fwrite("Euler X: ", 9, 1, output);
    gcvt(nodelist[j]->euler[0], 6, buffer);
    fwrite(buffer,strlen(buffer),1,output);
    fwrite(nline,2,1,output);
    fwrite("Euler Y: ", 9, 1, output);
    gcvt(nodelist[j]->euler[1], 6, buffer);
    fwrite(buffer,strlen(buffer),1,output);
    fwrite(nline,2,1,output);
    fwrite("Euler Z: ", 9, 1, output);
    gcvt(nodelist[j]->euler[2], 6, buffer);
    fwrite(buffer,strlen(buffer),1,output);

    fwrite(nline,2,1,output);
    fwrite("Children: ", 10, 1, output);
    //itoa(nodelist[j]->noofchildren,buffer,10);
	sprintf(buffer, "%d", nodelist[j]->noofchildren);
    fwrite(buffer, strlen(buffer), 1, output);
    fwrite(nline,2,1,output);
    fwrite(nline,2,1,output);
  }
  fclose(output);
}