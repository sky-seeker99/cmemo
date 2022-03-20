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

#ifndef _XTOKEN_H_
#define _XTOKEN_H_

#include "xtool_ex.h"

class tChar : public fChar{
  private:
    MemHeader_T *it;
    MEM_FP *comment_fp;
    void    double_skip(void);
    void    line_comment_skip(void);
    void    area_comment_skip(void);
  public:
    tChar(MemHeader_T *p_it,int p_maxFileCnt);
   ~tChar();
    MEM_FP *commentGet  (void){return(comment_fp);}
    void    commentClear(void){sChar *str; MEM_LOOP(str,sChar,comment_fp) delete str; LOOP_END comment_fp->mem_del_all();}
    char    srchToken        (char *sep,bool flg);
    char    srchToken        (char *sep){return(srchToken(sep,false));}
    char    srchNonSpaceToken(char *sep){return(srchToken(sep,true ));}
    char    srchSikiToken    (char *sep);
  };

#endif // _XTOKEN_H_
