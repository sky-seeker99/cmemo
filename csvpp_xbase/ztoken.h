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
//  String Tool
//

#ifndef _ZTOKEN_H_
#define _ZTOKEN_H_

#include "xarea.h"
#include "zschar.h"

class tokenCell_C{
  public:
    char srch_ch;
    bool srch_flg;
    tokenCell_C(char p){
      srch_ch  = p;
      srch_flg = false;
    }
};

class tokenAnl_C{
  public:
    VariableArray_C *srch_ary;  // Search Charactor  cell:tokenCell_C
    VariableArray_C *srch_str;  // Result String  cell:VariableArea_C
    int              str_su;    // Result String Su
//    VariableArea_C  *g_buff;    // buffer
    VariableArea_C  *w_buff;    // work
    tokenCell_C     *ssc;       // work
    sjisAnl_C       *sj;
    bool             kanjiMode; // 漢字はスキップ

    tokenAnl_C();
    ~tokenAnl_C();
    void  setKey    (char  key );
    void  setKeyFlag(          );
    void  set       (char *buff);
    char *get       (int   idx );
    int   getNum    (          );
    void  sjisMode  (          ){kanjiMode = true;}

  private:
    VariableArea_C *storeString();
};



// - 互換 ----
class StrSrchInf_T {
  public:
    tokenAnl_C *token;
    VariableArray_C *ary;
    StrSrchInf_T (){
      token = new tokenAnl_C;
      token->sjisMode();
      ary = new VariableArray_C(10);
    }
    ~StrSrchInf_T(){
      sChar *s;
      ARY_LOOP(s,sChar,ary) delete s; LOOP_END
      delete token;
      delete ary;
    }
    sChar *Get   (int   idx ){
      sChar *s = new sChar(token->get(idx));
      ary->mem_alloc((unsigned char *)s);
      return(s);
    }
    int   GetSu  (          ){return(token->str_su);}
    void  Set    (char  key ){token->setKey(key);}
    void  Main   (char *buff){
      sChar *s;
      ARY_LOOP(s,sChar,ary) delete s; LOOP_END
      ary->clear();
      token->set(buff);
    }
//    void  StrClr (          );
//    void  StrCopy(          );
    void  SetFlg (          ){token->setKeyFlag();}

};

#endif // _ZTOKEN_H_




