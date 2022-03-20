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
//  TOKEN LIBRARY
//

#include <string.h>
#include <stdio.h>
#include "xtoken.h"

//#define TOKEN_DEBUG 1

// # constructor / destructor #####
tChar::tChar(MemHeader_T *p_it,int p_maxFileCnt) : fChar(p_maxFileCnt)
  {
  it = p_it;
  comment_fp = new MEM_FP(it);
  }

tChar::~tChar(void)
  {
  commentClear();
  delete comment_fp;
  }


// # double skip #####
void tChar::double_skip(void)
  {
  int w_state=0;
  nextStep();
  for(;;)
    {
    char c=srchEx("\"",false,&w_state);
    if (c == '"')
      {
      if (sepBackGet() != '\\'){nextStep(); break;}
      nextStep();
      }
    }
  nextStep();
  }


// # line comment skip #####
void tChar::line_comment_skip(void)
  {
  //printf("%d:line comment [%s] -> ",curPtr,c_str());
  char *w = afterGetCut(true);
  sCharGrep *xx = new sCharGrep(w);
  it->alloc_ptr = xx; 
  comment_fp->mem_alloc();
  //printf("[%s]\n",c_str());

  if (xx->grep("translate_off",0) != -1)
    {
    for(;;)
      {
      if (xx->grep("translate_on",0) != -1){break;}
      w = fg->xFgets(*(fp+fileCnt-1));
      if (w == NULL){break;}
      if (*(w+strlen(w)-1) == 0x0a){*(w+strlen(w)-1) = 0x00;}
      sChar *w_xx = new sChar(w);
      it->alloc_ptr = w_xx;
      comment_fp->mem_alloc();
      }
    }

  }


// # area comment skip #####
void tChar::area_comment_skip(void)
  {
  int w_state = 0;
  markSet();
  nextStep();
  for(;;)
    {
    char c=srchEx("/",false,&w_state);
    if (c == 0x00){break;}
    if (sepBackGet() == '*'){break;}
    nextStep();
    }
  char *w = middleGetCut(true,true);
  sCharGrep *xx = new sCharGrep(w);
  it->alloc_ptr = xx;
  comment_fp->mem_alloc();


  if (xx->grep("translate_off",0) != -1)
    {
    for(;;)
      {
      if (xx->grep("translate_on",0) != -1){break;}
      char *w = fg->xFgets(*(fp+fileCnt-1));
      if (w == NULL){break;}
      if (*(w+strlen(w)-1) == 0x0a){*(w+strlen(w)-1) = 0x00;}
      sChar *w_xx = new sChar(w);
      it->alloc_ptr = w_xx; 
      comment_fp->mem_alloc();
      }
    }


  }

  
// # srchToken #####
char tChar::srchToken(char *p_sep,bool flg)
  {
  char c;
  sChar *sep = new sChar("/\"");
  sep->cat(p_sep);

#ifdef TOKEN_DEBUG
  printf("srchToken begin sep[%s] flg[%d] string[%s] curptr[%d]\n",sep->c_str(),flg,c_str(),curPtr);
#endif

  int w_state = 0;
  for(;;)
    {
    c = srchEx(sep->c_str(),flg,&w_state);
    int sv_state = svStateGet();

    if (c == '"')
      {
      double_skip();
      w_state = sv_state;
      continue;
      }
    if (c == '/')
      {
      if (sepNextGet() == '/')
        {
        line_comment_skip();
        w_state = sv_state;
        continue;
        }
      if (sepNextGet() == '*')
        {
        area_comment_skip();
        w_state = sv_state;
        continue;
        }
      nextStep();
      continue;
      }
    break;
    }
  delete sep;

#ifdef TOKEN_DEBUG
  printf("srchToken end ptr[%d] state:[%d] sep:[%c] string[%s]\n",curPtr,w_state,c,c_str());
#endif

  return(c);
  }


// # srchSikiToken #####
char tChar::srchSikiToken(char *p_sep)
  {
  sChar *sep = new sChar(p_sep);
  sep->cat("(");
  char c = srchToken(sep->c_str());
  if (c != '('){return(c);}

 // int w_state = 0;
  nextStep();
  sep->cat(")");
  int level;
  level = 1;
  for(;;nextStep())
    {
    c = srchToken(sep->c_str(),false);
    if (c == 0x00){break;}


//printf("[%d][%c][%s]\n",level,c,curGetStr(100));

    if (c == '('){level++; continue;}
    if (c == ')')
      {
      level--;
      if (level < 0){break;}
//      if (level == 0)
//        {
//        if (*p_sep == ')'){break;}
//        }
      continue;
      }

    if (level == 0){break;}
    }

  delete sep;
  return(c);
  }



