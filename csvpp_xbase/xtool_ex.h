/*

 * Copyright (c) 2003 Shigeru Kasuya (sky_seeker99@users.sourceforge.jp)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */
//
//  TOOLKIT LIBRARY
//

#ifndef _XTOOL_EX_H_
#define _XTOOL_EX_H_

#include "zfile.h"


class ssChar : public sChar{
  private:
    char *tmpStr;  // temporary
    sChar *sss;
    int   w_ptr;   // work
    int   sv_state;
  public:
    int   markPtr; // mark pointer
    int   curPtr;  // current pointer
    ssChar(char *src);
    ~ssChar();
    int   svStateGet  (void){return(sv_state);}
    void  stringSet   (char *str){set(str); curPtr = 0; markPtr = 0;}
    char *tmpSet(char *w,int len);
    char *beforeGet   (bool flg);
    void  beforeCut   (bool flg);
    char *beforeGetCut(bool flg){beforeGet(flg); beforeCut(flg); return(tmpStr);}
    char *afterGet    (bool flg);
    void  afterCut    (bool flg);
    char *afterGetCut (bool flg){afterGet(flg); afterCut(flg); return(tmpStr);}
    char *middleGet   (bool beforeFlg,bool afterFlg);
    void  middleCut   (bool beforeFlg,bool afterFlg);
    char *middleGetCut(bool beforeFlg,bool afterFlg){middleGet(beforeFlg,afterFlg); middleCut(beforeFlg,afterFlg); return(tmpStr);}
    char  sepGet      (void){return(*(c_str()+curPtr));}
    char  sepNextGet  (void){if (*(c_str()+curPtr)==0x00){return(0x00             );} return(*(c_str()+curPtr+1));}
    char  sepBackGet  (void){if (          curPtr ==   0){return(*(c_str()+curPtr));} return(*(c_str()+curPtr-1));}
    char *curGetStr   (int len);
    char  srch        (char *sep,bool flg,int *sv_state);
    void  nextStep    (void){if (*(c_str()+curPtr)!=0x00){curPtr++;}}
    void  backStep    (void){if (          curPtr  >   0){curPtr--;}}
    void  backMarkStep(void){curPtr  = markPtr;}
    void  markSet     (void){markPtr = curPtr; }
    void  nextMarkSet (void){markSet(); if (*(c_str()+markPtr)!=0x00){markPtr++;}}
    void  backMarkSet (void){markSet(); if (          markPtr  >   0){markPtr--;}}
  };


class fChar : public ssChar{
  private:
    MemHeader_T *it;       // MEM_FP Header
    MEM_FP   *inc_fp;      // File List
    int       maxFileCnt;  // max file pointer
    bool      fileReadCat();
  public:
    int       fileCnt;     // 0:none file
    FILE    **fp;          // file pointer
    xFgets_T *fg;          // variable length fgets class
    fChar(int p_maxFileCnt);
   ~fChar();
    bool  fileOpen      (char *fileName);
    char  srchEx        (char *sep,bool flg,int *state);
    char *curGetStrEx   (int   len     );
    void  pathAdd       (char *path);    
  };


#endif // _XTOOL_EX_H_

