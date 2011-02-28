/*************************************************************************

      Graphics Research Group - Department of Computer Science
                  University of Sheffield, UK
                      Michael Meredith
                     Copyright (c) 2000
                    All Rights Reserved.


  Permission is hereby granted, free of charge, to use this software 
  and its documentation without restriction, including without limitation 
  the rights to use, copy, modify, merge and publish, subject to the following 
  conditions:

   1. The code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Any modifications must be clearly marked as such.

   3. Original authors' names are not deleted.

   4. The authors' names are not used to endorse or promote products
      derived from this software without specific prior written
      permission from the author.

   5. This software is used in a research, non-profit capacity only and not 
      for commercial or profit applications unless specific prior written
      permission from the author is given.


  THE UNIVERSITY OF SHEFFIELD AND THE CONTRIBUTORS TO THIS WORK
  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
  SHALL THE UNIVERSITY OF SHEFFIELD NOR THE CONTRIBUTORS BE LIABLE
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
  THIS SOFTWARE.

*************************************************************************/

#ifndef __displaygl_h__
#define __displaygl_h__

//#include <afxwin.h>
//#include "gl\freeglut.h"
#include <GL/glut.h>
//#include "gl\glaux.h"
#include "Extras.h"
#include <vector>

class DisplayGL// : public CWnd//, public CWinThread, 
{
public:
  DisplayGL();
  ~DisplayGL();
  void SetupSegments(NODE* segs, MOCAPHEADER* header, NODE** effectorlist);  // Used to pass the display a body structure that holds all of the required information
  void Draw();                                          // Redraws the 3D object
  MOCAPHEADER* header;

private:
  //std::vector<NODE> endEffectorList;
  //int endEffectorLen;

  NODE* body;
  NODE** effectorlist;
  //HGLRC hrc;
  //CClientDC*	m_pDC;
  int m_pDC;
  //CPalette m_cPalette, *m_pOldPalette;

  //float lasttime;
  DWORD lasttime;
  long  frames;
  char fps[6];

  MyPoint last;
  float centre[3];

  void Init();                          // Initilises GL display area
 
  unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift);  // Aux function to CreateRGBPalette

  void EvaluateChildren(NODE* node);    // Used recursively to locate and position child nodes in the hier
  //void gEvaluateChildren(NODE* node);
  void nodeLocalPos(NODE* seg, long currentframe, float *pos);



protected:
  

  //int OnCreate(LPCREATESTRUCT lpCreateStruct);    // Called when the window is created
  //DECLARE_MESSAGE_MAP()
  
};
#endif
