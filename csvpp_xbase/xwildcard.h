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
// Wild Card
//

#ifndef _XWILDCARD_H_
#define _XWILDCARD_H_

//#include "xstr.h"
#include "ztoken.h"
#include "zlist.h"

// interface class -----------------
class WildCard_T{
  private:
  sChar  *all_str;
  sChar  *top_str;
  sChar  *end_str;
  MEM_FP *mid_fp;
  MemHeader_T *it;
  sCharGrep   *grep;

  public:
  WildCard_T() {
    it = new MemHeader_T;
    mid_fp = new MEM_FP(it);
    all_str = new sChar("");
    top_str = new sChar("");
    end_str = new sChar("");
    grep    = new sCharGrep("");
    }


  ~WildCard_T(){
    sChar *str;
    delete all_str;
    delete top_str;
    delete end_str;
    delete grep;
    MEM_LOOP(str,sChar,mid_fp) delete str; LOOP_END
    delete mid_fp;
    delete it;
    }

    void set(char *p_str);
    bool cmp(char *p_str);
    void pr();

};


#endif // _XWILDCARD_H_

