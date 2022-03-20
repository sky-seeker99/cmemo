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
#include <string.h>
#include <stdio.h>
#include "xtool_ex.h"

// # ssChar ###############################################
// # Constructor/Destructor #####
ssChar::ssChar(char *p_str) : sChar(p_str){
  markPtr = 0;
  curPtr  = 0;
  tmpStr  = new char[1];
  sss     = new sChar("");
  *tmpStr = 0x00;
}

ssChar::~ssChar(){
  delete [] tmpStr;
  delete sss;
}

// # tmpSet #####
char *ssChar::tmpSet(char *w,int len)
  {
  int i;
  delete [] tmpStr;
  tmpStr = new char[len+1];
  char *src = w;
  char *dst = tmpStr;
  for(i=0;i<len;i++){*dst++ = *src++;}
  *dst = 0x00;
  return(tmpStr);
  }

// # before get #####
char *ssChar::beforeGet(bool flg)
  {
  if ((flg == false) || (*(c_str()+curPtr) == 0x00)){w_ptr = curPtr;} else {w_ptr = curPtr+1;}
  return(tmpSet(c_str(),w_ptr));
  }

// # before cut #####
void ssChar::beforeCut(bool flg)
  {
  if ((flg == false) || (*(c_str()+curPtr) == 0x00)){w_ptr = curPtr;} else {w_ptr = curPtr+1;}
  sss->set(c_str()+w_ptr);
  set(sss->c_str());
  markPtr = 0;
  curPtr  = 0;
  }

// # after get #####
char *ssChar::afterGet(bool flg)
  {
  if ((flg == true) || (*(c_str()+curPtr) == 0x00)){w_ptr = curPtr;} else {w_ptr = curPtr+1;}
  return(tmpSet(c_str()+w_ptr,strlen(c_str()+w_ptr)));
  }

// # after cut #####
void ssChar::afterCut(bool flg)
  {
  if ((flg == true) || (*(c_str()+curPtr) == 0x00)){w_ptr = curPtr;} else {w_ptr = curPtr+1;}
  *(c_str()+w_ptr) = 0x00;
  }


// # middle get #####
char *ssChar::middleGet(bool beforeFlg,bool afterFlg)
  {
  int before,after;

  if (markPtr == curPtr)
    {
    if ((beforeFlg) || (afterFlg)){return(tmpSet(c_str()+curPtr,1));}
    return(tmpSet(c_str()+curPtr,0));
    }
  
  if ((beforeFlg == true ) || (*(c_str()+markPtr) == 0x00)) {before = 0;} else {before = 1;}
  if ((afterFlg  == false) || (*(c_str()+curPtr ) == 0x00)) {after  = 0;} else {after  = 1;}
  
  return(tmpSet(c_str()+markPtr+before,curPtr-markPtr-before+after));
  }

// # middle cut #####
void ssChar::middleCut(bool beforeFlg,bool afterFlg)
  {
  int before,after;

  if (markPtr == curPtr)
    {
    if ((beforeFlg) || (afterFlg))
      {
      if (*(c_str()+curPtr) == 0x00){return;}
      sss->set(c_str()+curPtr+1);
      strcpy(c_str()+curPtr,sss->c_str());
      }
    return;
    }

  if ((beforeFlg == true ) || (*(c_str()+markPtr) == 0x00)) {before = 0;} else {before = 1;}
  if ((afterFlg  == false) || (*(c_str()+curPtr ) == 0x00)) {after  = 0;} else {after  = 1;}
  sss->set(c_str()+curPtr+after);
  strcpy(c_str()+markPtr+before,sss->c_str());
  markPtr = markPtr+before;
  curPtr  = markPtr;
  }

// # current string get #####
char *ssChar::curGetStr(int len)
  {
  char *w     = c_str()+curPtr;
  int   w_len = strlen(w);
  if (len > w_len){len = w_len;}
  return(tmpSet(w,len));
  }


// # search #####
char ssChar::srch(char *sep,bool flg,int *state) // sep ga 'S' no toki space mataha tab igai de hit!!
  {
    //    printf("srch<%s>\n",c_str());

  sv_state = *state;
  //  int state=*init_state; // 0:space 1:none space 2:space
  for(char *w=c_str()+curPtr;;curPtr++,w++)
    {
    char c = *w;
    if (c == 0x00){return(0x00);}
    //   printf("srch[%d][%c] sep:[%s]\n",*state,c,sep);

    sv_state = *state; // comment taisaku
    if ((*state == 0) && ((c != ' ') && (c != 0x09))){(*state)++;}
    if (flg)
      {
      if ((*state == 1) && ((c == ' ') || (c == 0x09))){(*state)++;}
      if ((*state == 2) && ((c != ' ') && (c != 0x09))){return(c);}
      }
    if (*state == 0){continue;}
    for(char *w_sep=sep;;w_sep++)
      {
      if (*w_sep == 0x00){break;}
      if (*w_sep == 'S' ) 
        {
	  if ((c != ' ' ) && (c != 0x09)){return(c);} 
        }
      if (*w_sep == c   ){return(c);}
      if ((*w_sep == ' ' ) && (c == 0x09)) {return(' ');}
      }
    }
  }


// # fChar ###############################################
// # Constructor/Destructor #####
fChar::fChar(int p_maxFileCnt) : ssChar("")
  {
  it = new MemHeader_T;
  inc_fp = new MEM_FP(it);
  fileCnt = 0;
  maxFileCnt = p_maxFileCnt;
  fp = new FILE*[p_maxFileCnt];
  for(int i=0;i<maxFileCnt;i++){*(fp+i) = NULL;}
  fg = new xFgets_T(1024);
#ifdef WIN
  it->alloc_ptr = (MM_PTR_T *) new sChar(".\\");
#endif
#ifdef UNIX
  it->alloc_ptr = (MM_PTR_T *) new sChar("./");
#endif

  inc_fp->mem_alloc();
  }

fChar::~fChar(void)
  {
  sChar *str;
  for(int i=0;i<fileCnt;i++){fclose(*(fp+i));}
  delete [] fp;
  delete fg;
  MEM_LOOP(str,sChar,inc_fp) delete str; LOOP_END
  delete inc_fp;
  delete it;
  }

// # search path add #####
void fChar::pathAdd(char *path)
  {
  sChar *str = new sChar(path);
#ifdef WIN
  if (*(path+strlen(path)-1) != '\\'){str->cat("\\");}
#endif
#ifdef UNIX
  if (*(path+strlen(path)-1) != '/'){str->cat("/");}
#endif
  it->alloc_ptr = str;
  inc_fp->mem_alloc();
  }


// # file open #####
bool fChar::fileOpen(char *fileName)
  {
  sChar *inc;
  sChar *ss;
  if (maxFileCnt <= fileCnt){return(false);}
  ss = new sChar("");
  MEM_LOOP(inc,sChar,inc_fp)
    ss->set(inc->c_str());
    ss->cat(fileName);
    *(fp+fileCnt)   = fopen(ss->c_str(),"rt");
    if (*(fp+fileCnt) != NULL){break;}
  LOOP_END
  delete ss;
  if (*(fp+fileCnt) == NULL){return(false);}
  fileCnt++;
  return(true);
  }

// # file read & cat #####
bool fChar::fileReadCat()
  {
  if (*(fp+fileCnt-1) == NULL){return(false);}
  for(;;)
    {
    if (fileCnt == 0){return(false);}
    char *w = fg->xFgets(*(fp+fileCnt-1));
    if (w != NULL)
      {
//printf("fileGet:[%s]",c_str());
      if (*(w+strlen(w)-1) == 0x0a){*(w+strlen(w)-1) = ' '; cat(w);}
      else                         {cat(w); cat(" ");}
//printf("[%s]\n",c_str());
      break;
      }
    fclose(*(fp+fileCnt-1));
    *(fp+fileCnt-1) = NULL;

    delete fg;
    fg = new xFgets_T(1024);
    fileCnt--;
    }
  return(true);
  }

// # string search & file read #####
char fChar::srchEx(char *sep,bool flg,int *state)
  {
  char w;
  //  int sv_state;
  //  sv_state = 0;
  for(;;)
    {
    w=srch(sep,flg,state);
    if (w != 0x00){break;}
    if (fileReadCat() == false){break;}
    }
  return(w);
  }


// # current string get #####
char *fChar::curGetStrEx(int len)
  {
  char *w;
  for(;;)
    {
    w     = c_str()+curPtr;
    int   w_len = strlen(w);
    if (len > w_len)
      {
      if (fileReadCat() == false){len = w_len; break;}
      }
    }

  return(tmpSet(w,len));
  }


