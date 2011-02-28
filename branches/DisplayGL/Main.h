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

#ifndef __main_h__
#define __main_h__

//#include <afxwin.h>
#include "DisplayGL.h"
#include "Extras.h"

DisplayGL* Output3D;
NODE* body;
MOCAPHEADER header;
NODE** nodelist;
NODE** effectorlist;  

DWORD lasttime;
long  frames;
char fps[6];

int lastX, lastY;
float centre[3];
int buttonFlag;

void OutputNodeList();
void loadFilename(const char* m_lpCmdLine);

/*
class Main //: public CWinApp
{
public:
  BOOL InitInstance(int argc, char **argv);
  BOOL OnIdle(LONG lCount);
};

class MainWnd //: public CWnd
{
public:
  MainWnd(const char* m_lpCmdLine);
  ~MainWnd();

private:
  DisplayGL* Output3D;
  NODE* body;
  MOCAPHEADER header;
  NODE** nodelist;           // Array containing all of the nodes that make up the body

  void OutputNodeList();

protected:

  int OnCreate(LPCREATESTRUCT lpCreateStruct);    // Called when the window is created
  void OnDestroy();
  void OnPaint();                                 // Called when repainting needs to be done
  void OnTimer(UINT nIDEvent);                    // Called in result of a timer message being sent
 // DECLARE_MESSAGE_MAP()
 
};
*/

#endif