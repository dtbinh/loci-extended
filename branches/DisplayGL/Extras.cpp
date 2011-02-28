#include "Extras.h"
#pragma warning(disable: 4996)

int strstrEx(const char *string, const char *strCharSet)
{
  // Like strstr but not case sensitve and returns the offset within the string of the start of strCharSet
  // Assumes that strCharSet is already presented in upper case
  int i = 0, j = 0, k = 0;
  if (!strCharSet[0])
    return -1;
  while (string[i])
  {
    while (((string[i] > 96 && string[i] < 123) ? string[i]&0xdf : string[i])!= strCharSet[0] && string[i++]);
    j=++i;
    while (((string[j] > 96 && string[j] < 123) ? string[j]&0xdf : string[j])==strCharSet[++k] && string[j++]);
    if (strCharSet[k])
      k=0;
    else
      return i - 1;
  }
  return -1;
}

int strstrEx(const char *string, char strChar)
{
  // Like strchr but not case sensitve and returns the offset within the string of the start of strCharSet
  // Assumes that strChar is already presented in upper case
  int i = 0;
  if (!strChar)
    return -1;
  while (string[i] && (string[i++]&0xdf)!=strChar);
  if (string[i])
    return i-1;
  else
    return -1;
}

bool strcompEx(const char *string, const char* strCharSet)
{
  int i=0;
  while(string[i] && strCharSet[i] && (string[i]&0xdf)==strCharSet[i++]);
  if (!string[i] && !strCharSet[i])
    return true;
  else
    return false;
}

void MessageBoxa(const char* message)
{
	std::cout << message << std::endl;
}

void localRotate(float* length, int axis, float angle, float* pos)
{
	angle = angle*(PI/180);
	float x, y, z; x = y = z = 0;
	if (axis == 0)
	{
		y = length[1]*cos(angle) + length[2]*sin(angle);
		z = (length[1] * -sin(angle)) + (length[2]*cos(angle));
	} else if (axis == 1)
	{
		x += length[0]*cos(angle) - length[2]*sin(angle);
		z += length[0]*sin(angle) + length[2]*cos(angle);
	} else if (axis == 2)
	{
		x += length[0]*cos(angle) + length[1]*sin(angle);
		y += - length[0]*sin(angle) + length[1]*cos(angle);
	} else { throw "LocalRotateError: INvalid axis"; }

	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
	return;
}

void localPos(float* length, float* euler, float* pos)
{
	float start[3];
	start[0] = length[0]; start[1] = length[1]; start[2] = length[2];
	float res[3], resa[3], resb[3];
	if (euler[0])
	{
		localRotate(start, 0, euler[0], res);
		start[0] = res[0]; start[1] = res[1]; start[2] = res[2];
	}

	if (euler[1])
	{
		localRotate(start, 1, euler[1], res);
		start[0] = res[0]; start[1] = res[1]; start[2] = res[2];
	}
    if (euler[2])
	{
		localRotate(start, 2, euler[2], res);
		start[0] = res[0]; start[1] = res[1]; start[2] = res[2];
	}

	if ((res[0] == 0) && (res[1] == 0) && (res[2] == 0))
	{
		pos[0] = length[0]; pos[1] = length[1]; pos[2] = length[2];
	} else {
		pos[0] = res[0]; pos[1] = res[1]; pos[1] = res[1];
	}
	return;
}

/*
void localPos(const NODE* seg, long currentframe, float *pos)
{
	pos[0] = seg->offset[0] + seg->froset[currentframe][0];
	pos[1] = seg->offset[1] + seg->froset[currentframe][1];
	pos[2] = seg->offset[2] + seg->froset[currentframe][2];
	float nodeLen = sqrt (sqr(seg->length[0]) + sqr(seg->length[1]) + sqr(seg->length[2])) * seg->scale[currentframe];

	float tx, ty, tz;
	tx = ty = tz = 0;
	ty += nodeLen * cos(seg->freuler[currentframe][0]);
	tx += nodeLen * sin(seg->freuler[currentframe][0]);

	tx += nodeLen * cos(seg->freuler[currentframe][1]);
	tz += nodeLen * sin(seg->freuler[currentframe][1]);

	tx += nodeLen * cos(seg->freuler[currentframe][2]);
	ty += nodeLen * sin(seg->freuler[currentframe][2]);

	pos[0] += tx;
	pos[1] += ty;
	pos[2] += tz;


	if (seg->parent)
		{
			float posP[3];
			localPos( seg->parent, currentframe, posP );

			if (posP != NULL) 
			{
				pos[0] += posP[0];
				pos[1] += posP[1];
				pos[2] += posP[2];
			}
		}

	return;
}
*/
