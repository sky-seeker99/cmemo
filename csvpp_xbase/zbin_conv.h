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

#ifndef _ZBIN_CONV_H_
#define _ZBIN_CONV_H_
#include <string.h>
#include "zschar.h"
#include "ztool.h"

// # string class ----------------
class binConv_C {
  public:
    sChar *w;
    sChar *rtn_cd;
    char *conv(char *in);
    binConv_C(){
      w      = new sChar("");
      rtn_cd = new sChar("");
    }
    ~binConv_C(){
      delete w;
      delete rtn_cd;
    }
};

#endif // _ZBIN_CONV_H_
