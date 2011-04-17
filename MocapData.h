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

#ifndef __mocapdata_h__
#define __mocapdata_h__
#include "Extras.h"

class MocapData
{
public:
  MocapData();
  MocapData(MOCAPHEADER *header);
  ~MocapData();

  void SetHeader(MOCAPHEADER *header);      // Sets up a pointer to the header structure
  virtual bool ImportData(const char* filename) = 0;    // Starts the import of the HTR file
  NODE* GetRootNode();                      // Returns the root node
  NODE** GetNodeList();                      // Returns an array that holds all the nodes
  NODE** GetEffectorList();
  void AddEffector(NODE *seg);
  void SearchEffector(NODE *seg);
  

  const char* GetError();

protected:
  NODE *root, **nodelist; 
  NODE **effectorlist;
  int effectorLen;
  MOCAPHEADER* header;
  int xpos, ypos, zpos;
  virtual void EnlargeEffectorList() =0;

  char error[255];          // Used to store any errors that occured
};

inline MocapData::MocapData() : root(0), nodelist(0), header(0), xpos(0), ypos(0), zpos(0)
{};

inline MocapData::MocapData(MOCAPHEADER *header) : root(0), nodelist(0), effectorlist(0), header(header), xpos(0), ypos(0), zpos(0)
{};

inline MocapData::~MocapData()
{};

inline void MocapData::SetHeader(MOCAPHEADER *header)
{
  this->header = header;
};

inline NODE* MocapData::GetRootNode()
{
  return root;
};

inline NODE** MocapData::GetNodeList()
{
  return nodelist;
};

inline NODE** MocapData::GetEffectorList()
{
  return effectorlist;
};

inline const char* MocapData::GetError()
{
  return error;
};

inline void MocapData::AddEffector(NODE *seg)
{
			//	  std::cout << "Enlarge Children " << seg->name << std::endl;
				  EnlargeEffectorList();
				  effectorlist[header->noofeffectors++] = seg;

}

//Search for an and effector of current chain, add it to the list of effectors
inline void MocapData::SearchEffector(NODE *seg)
{
	//std::cout << "SearchEffector " << seg->name << std::endl;
    if (seg->children)
    {
      for (int i=0; i< seg->noofchildren; i++)
        SearchEffector(seg->children[i]);
    } else {
		//Node is an end Effector.
		AddEffector(seg);

	}
}



#endif
