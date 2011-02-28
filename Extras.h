#ifndef __extras_h__
#define __extras_h__
#include <stdlib.h>
#include <iostream>
#include <cmath>

#pragma warning(disable: 4996)

#define XROT 1
#define YROT 2
#define ZROT 4
#define XTRA 32
#define YTRA 64
#define ZTRA 128
#define PI 3.141592

typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef unsigned int UINT;

struct NODE
{
  char *name;
  float length[3];    // length of segment
  float offset[3];    // Transitional offset with respect to the end of the parent link
  float euler[3];     // Rotation
  float colour[3];
  int noofchildren;
  NODE **children;    // Array of pointers to child nodes
  NODE *parent;       // Back pointer to parent node
  float **froset;     // Array of offsets for each frame
  float **freuler;    // Array of angles for each frame
  float **flspace;    // World Space Position of node end.
  float *scale;       // Array of scalefactors for each frame
  BYTE DOFs;          // Used to determine what DOFs the segment has
};

struct MOCAPHEADER
{
  // Assumes that all angles are in degrees if not then they need to be converted
  int noofsegments;       // Number of body segments
  int noofeffectors;
  long noofframes;     // Number of frames
  int datarate;         // Number of frames per second
  int euler[3][3];      // Specifies how the euler angle is defined
  float callib;         // Scale factor for converting current translational units into meters
  bool degrees;         // Are the rotational measurements in degrees
  float scalefactor;    // Global Scale factor
  long currentframe;    // Stores the currentframe to render
};

int strstrEx(const char *string, const char *strCharSet);
int strstrEx(const char *string, char strChar);
bool strcompEx(const char *string, const char *strCharSet);
float sqr(float a);
//void localPos(const NODE * node, long currentframe, float* pos);

void MessageBoxa(const char * message);

inline void SetupChildren(NODE* seg, int children)
{
  seg->noofchildren = children;
  if (children)
  {
    seg->children = (NODE**) malloc(sizeof(NODE*) * children);
	seg->colour[0] = 0.0f;
  }
  else
  {
    seg->children = 0;
    seg->colour[0] = 0.8f;
			//	  if (curnode->noofchildren-1 == 0)
			 // {

			  //}
  }
    //TODO Maybe Specify End Effector.
}

inline void SetupOffset(NODE* seg, float x = 0.0f, float y = 0.0f, float z = 0.0f)
{
  seg->offset[0] = x;
  seg->offset[1] = y;
  seg->offset[2] = z;
}

inline void SetupEuler(NODE* seg, float r1 = 0.0f, float r2 = 0.0f, float r3 = 0.0f)
{
  seg->euler[0] = r1;
  seg->euler[1] = r2;
  seg->euler[2] = r3;
}

inline void SetupColour(NODE* seg, float r = 0.0f, float g = 0.0f, float b = 0.0f)
{
  seg->colour[0] = r; 
  seg->colour[1] = g; 
  seg->colour[2] = b; 
}

inline void SetupFrames(NODE* seg, long frames)
{
  seg->froset = (float**) malloc (sizeof(float*) * frames);
  seg->freuler = (float**) malloc (sizeof(float*) * frames);
  seg->flspace = (float**) malloc (sizeof(float*) * frames);
  seg->scale = (float*) malloc (sizeof(float) * frames);

  for (long i=0; i<frames; ++i)
  {
    seg->froset[i] = (float*) malloc(sizeof(float*) * 3);
    seg->freuler[i] = (float*) malloc(sizeof(float*) * 3);
    seg->flspace[i] = (float*) malloc(sizeof(float*) * 3);
  }
}

inline float sqr(float a)
{
  return a*a;
}

#include <sys/time.h>
inline unsigned GetTickCount()
{
	struct timeval tv;
	if (gettimeofday(&tv, NULL) != 0) return 0;

	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

struct MyPoint
{
	int x;
	int y;
};

void localRotate(float* length, int axis, float angle, float* pos);
void localPos(float* length, float* euler, float* pos);

#endif
