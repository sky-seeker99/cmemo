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
