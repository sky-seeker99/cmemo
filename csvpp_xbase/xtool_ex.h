/*
 * Copyright 2026 Shigeru Kasuya
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by me or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

