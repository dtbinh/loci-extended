#include <stdio.h>
#include <string.h>
#include <math.h>
#include "BVHFormat.h"

bool BVHFormat::ImportData(const char *filename)
{
  int read, i, j, where;
  int pos[8];           // Used to determine the position of the next char to write
  char line[8][40];     // Used to store the attribute and the corresponding value
  char buffer[4097];
  int section = 0;      // Indicates which section is currently being processed
  NODE *curnode=0;      // Used to indicate the current node that is being processed
  int index, channels = 0;
  bool endsite = false;

  header->callib = 1.0f;
  header->scalefactor = 1.0f;
  header->noofsegments = 0;
  header->noofeffectors = 0;
  header->noofframes = 0;
  header->datarate = 0;
  
  xpos = 1;
  ypos = 2;
  zpos = 0;
  
  header->euler[0][1] = header->euler[0][0] = 0;
  header->euler[0][2] = 1;
  
  header->euler[1][2] = header->euler[1][1] = 0;
  header->euler[1][0] = 1;
  
  header->euler[2][2] = header->euler[2][0] = 0;
  header->euler[2][1] = 1;

  header->callib = 0.03f;
  header->degrees = true;
  header->scalefactor = 1.0f;


  FILE *file = fopen(filename, "rb");
  if (file)
  {
    // Process the "Hierarchy" section of the file
    read = fread(buffer,1,4096,file);
    buffer[read] = '\0';
    i = strstrEx(buffer, "HIERARCHY");
    i+=strstrEx(buffer + i, char(10));
    while (buffer[++i] < 32);

    where = pos[0] = pos[1] = pos[2] = pos[3] = pos[4] = pos[5] = pos[6] = pos[7] = 0;
    // Process each line in the header
    while (read)
    {
      while (i<read)
      {
        if ((buffer[i] == char(10) && pos[0]) || (section==2 && where==3))
        {
          // Process line
          line[7][pos[7]] = line[6][pos[6]] = line[5][pos[5]] = line[4][pos[4]] = line[3][pos[3]] = line[2][pos[2]] = line[1][pos[1]] = line[0][pos[0]] = '\0';
          if (!section)
          {
            // Process Hierarchy
            if (strcompEx(line[0], "ROOT"))
            {
              if (root)
              {
                strcpy(error, "BVH file contains more than one skeleton which is currently unsupported");
                fclose(file);
                return false;
              }
              else
              {
                EnlargeNodeList();
                root = nodelist[header->noofsegments++] = (NODE*) malloc(sizeof(NODE));
                root->name = (char*) malloc(strlen(line[1]) + 1);
                strcpy(root->name, line[1]);
                root->DOFs = 0;
                SetupChildren(root, 0);
                SetupColour(root);
                SetupEuler(root);
                root->parent = 0;
                root->length[2] = root->length[1] = root->length[0] = 0.0f;
                curnode = root;
              }
            }
            else if (strcompEx(line[0], "JOINT"))
            {
              IncreaseChildren(curnode);
              EnlargeNodeList();

              curnode->children[curnode->noofchildren-1] = nodelist[header->noofsegments++] = (NODE*) malloc(sizeof(NODE));
              curnode->children[curnode->noofchildren-1]->parent = curnode;
              curnode = curnode->children[curnode->noofchildren-1];
              
              curnode->name = (char*) malloc(strlen(line[1]) + 1);
              strcpy(curnode->name, line[1]);
              curnode->DOFs = 0;

			  SetupColour(curnode);
              SetupChildren(curnode,0);
			  
              
              SetupEuler(curnode);
			//  SetupWSpace(curnode);
              curnode->length[2] = curnode->length[1] = curnode->length[0] = 0.0f;

            }
            else if (strcompEx(line[0], "OFFSET"))
            {
              float x, y, z, rx, ry, rz;
              x = (float) atof(line[1]) * header->callib;
              y = (float) atof(line[2]) * header->callib;
              z = (float) atof(line[3]) * header->callib;
              rx = ry = rz = 0.0f;
              if (!endsite)
              {
                SetupOffset(curnode, x, y, z);
                if (curnode!=root && (curnode->parent->length[0]==0.0f && curnode->parent->length[1]==0.0f && curnode->parent->length[2]==0.0f))
                {
                  SetupEuler(curnode->parent, rx, ry, rz);
                  curnode->parent->length[0] = x;
                  curnode->parent->length[1] = y;
                  curnode->parent->length[2] = z;
                }
              }
              else
              {
                curnode->length[0] = x;
                curnode->length[1] = y;
                curnode->length[2] = z;
              }
            }
            else if (strcompEx(line[0], "CHANNELS") && !endsite)
            {
              channels+=atoi(line[1]);
              int d=2;
              while (line[d] && d < 8)
              {
                if ((line[d][0]&0xdf)=='X')
                {
                  if ((line[d][1]&0xdf)=='R')
                  {
                    curnode->DOFs|=XROT;
                  }
                  else if ((line[d][1]&0xdf)=='P')
                    curnode->DOFs|=XTRA;
                }
                else if ((line[d][0]&0xdf)=='Y')
                {
                  if ((line[d][1]&0xdf)=='R')
                  {
                    curnode->DOFs|=YROT;
                  }
                  else if ((line[d][1]&0xdf)=='P')
                    curnode->DOFs|=YTRA;
                }
                else if ((line[d][0]&0xdf)=='Z')
                {
                  if ((line[d][1]&0xdf)=='R')
                  {
                    curnode->DOFs|=ZROT;
                  }
                  else if ((line[d][1]&0xdf)=='P')
                    curnode->DOFs|=ZTRA;
                }
                ++d;
              }
            }
            else if (strcompEx(line[0], "END") && strcompEx(line[1], "SITE"))
              endsite = true;
            else if (line[0][0]=='}')
            {
              if (endsite)
                endsite = false;
              else
                curnode = curnode->parent;
            }
            else if (strcompEx(line[0], "MOTION"))
            {
              ++section;
            }
          }
          else if (section==1)
          {
            // Process Motion
            if (strcompEx(line[0], "FRAMES:"))
            {
              header->noofframes = atoi(line[1]);
              for (int i=0; i<header->noofsegments; ++i)
                SetupFrames(nodelist[i], header->noofframes);
              header->currentframe = 0;
            }
            else if (strcompEx(line[0], "FRAME") && strcompEx(line[1], "TIME:"))
            {
              header->datarate = (int) (1 / (atof(line[2])));
              if ((int) (0.49 + (1 / atof(line[2]))) > header->datarate)
                ++header->datarate;
            }
            if (header->datarate && header->noofframes)
            {
              ++section;
              curnode = root;
              index = 0;
              endsite = false;
            }
          }
          else
          {
            //Process DOFs
            if (header->currentframe < header->noofframes)
            {
              if (curnode->DOFs == 231)
              {
                if (!endsite)
                {
                  curnode->froset[header->currentframe][0] = (float) atof(line[0]) * header->callib;
                  curnode->froset[header->currentframe][1] = (float) atof(line[1]) * header->callib;
                  curnode->froset[header->currentframe][2] = (float) atof(line[2]) * header->callib;
                  endsite = true;
                }
                else
                {
                  curnode->freuler[header->currentframe][xpos] = (float) atof(line[1]);
                  curnode->freuler[header->currentframe][ypos] = (float) atof(line[2]);
                  curnode->freuler[header->currentframe][zpos] = (float) atof(line[0]);


				  calcFLSpace(curnode, header->currentframe);
                  curnode->scale[header->currentframe] = 1.0f;
                  curnode = nodelist[++index];
                  endsite = false;
                }
              }
              else
              {
                curnode->froset[header->currentframe][0] = curnode->froset[header->currentframe][1] = curnode->froset[header->currentframe][2] = 0.0f;
                curnode->freuler[header->currentframe][xpos] = (float) atof(line[1]);
                curnode->freuler[header->currentframe][ypos] = (float) atof(line[2]);
                curnode->freuler[header->currentframe][zpos] = (float) atof(line[0]);
                curnode->scale[header->currentframe] = 1.0f;

				calcFLSpace(curnode, header->currentframe);
				
                if (index+1 < header->noofsegments)
                  curnode = nodelist[++index];
                else
                {
                  ++header->currentframe;
                  curnode = nodelist[index=0];
                }
              }
            }
            else
              ++section;
          }
          



          if (section!=2)
          {
            // Move onto the next line and clear current line information
            j=strstrEx(buffer + i, char(10));
            if (j==-1)
            {
              if (buffer[4095]!=10)
              {
                read = fread(buffer, 1, 4096, file);
                i = strstrEx(buffer, char(10));
              }
              else
              {
                read = fread(buffer, 1, 4096, file);
                i=0;
              }
              buffer[4096] = '\0';
            }
            else
              i+=j;
          }
          where = pos[0] = pos[1] = pos[2] = pos[3] = pos[4] = pos[5] = pos[6] = pos[7] = 0;
        }

        if (buffer[i] > 44 && buffer[i] < 126)
          line[where][pos[where]++] = buffer[i++];
        else if ((buffer[i]==32 || buffer[i]==9) && pos[where]>0)
        {
          ++where;
          ++i;
        }
        else
          ++i;
      }
      read = fread(buffer, 1, 4096, file);
      buffer[4096] = '\0';
      i=0;
    }
    fclose(file);
	SearchEffector(this->GetRootNode());
    return true;
  }
  else
  {
    strcpy(error, "Cannot open file ");
	strcat(error, filename);
    return false;
  }

  //NODE *curseg;
  
}

void BVHFormat::calcFLSpace(NODE* curnode, int currentframe)
{
 // std::cout << "Calculating flspace " << curnode->name;
  float x,y,z;

  float pos[3]; pos[0] = 0; pos[1] = 0; pos[2] = 0;
  float euler[3];
  euler[0] = curnode->euler[0] + curnode->freuler[currentframe][0];
  euler[1] = curnode->euler[1] + curnode->freuler[currentframe][1];
  euler[2] = curnode->euler[2] + curnode->freuler[currentframe][2];
  //std::cout << curnode->length[0] << ", " << curnode->length[1] << ", " << curnode->length[2] << std::endl;
  //std::cout << euler[0] << ", " << euler[1] << ", " << euler[2] << std::endl;
  localPos(curnode->length, euler, pos);
  x = pos[0];
  y = pos[1];
  z = pos[2];

  //std::cout << x << ", " << y << ", " << z << std::endl;
  x += curnode->offset[0]; x+= curnode->froset[currentframe][0];
  y += curnode->offset[1]; x+= curnode->froset[currentframe][1];
  z += curnode->offset[2]; x+= curnode->froset[currentframe][2];

  curnode->flspace[currentframe][0] = x; 
  curnode->flspace[currentframe][1] = y; 
  curnode->flspace[currentframe][2] = z;
}



void BVHFormat::IncreaseChildren(NODE* node)
{
  int i;
  NODE **temp;
  if (node->children)
  {
    // Parent already has children
    temp = node->children;
    temp = (NODE**) malloc(sizeof(NODE*) * node->noofchildren);
    for (i=0; i<node->noofchildren; ++i)
      temp[i] = node->children[i];
    free(node->children);
    node->children = (NODE**) malloc(sizeof(NODE*) * ++node->noofchildren);
    for (i=0; i<node->noofchildren; ++i)
      node->children[i] = temp[i];
    free(temp);
  }
  else
    SetupChildren(node, ++node->noofchildren);
}
