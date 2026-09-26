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
// ISO2022 Converter
//

#ifndef _XISO2022_H_
#define _XISO2022_H_

#include "zschar.h"

// interface class -----------------
class Iso2022_T{
  private:
  sChar *en_sep;
  sChar *dis_sep;
  unsigned int en_sep_len;
  unsigned int dis_sep_len;
  unsigned char *string;
  unsigned char c1;
  unsigned char c2;

  public:
  Iso2022_T(char *p_en_sep,char *p_dis_sep) {
    dis_sep = new sChar(p_dis_sep);
    en_sep  = new sChar(p_en_sep);
    dis_sep_len = strlen(p_dis_sep);
    en_sep_len  = strlen(p_en_sep);
    string      = NULL;
    }
    
  Iso2022_T(){ // for encord
    char p_en_sep [5];
    char p_dis_sep[5];
    
    p_en_sep[0] = 0x1b;
    p_en_sep[1] = 0x24;
    p_en_sep[2] = 0x42;
    p_en_sep[3] = 0x00;
    p_dis_sep[0] = 0x1b;
    p_dis_sep[1] = 0x28;
    p_dis_sep[2] = 0x42;
    p_dis_sep[3] = 0x00;
    dis_sep_len = strlen(p_dis_sep);
    en_sep_len  = strlen(p_en_sep);

    dis_sep = new sChar(p_dis_sep);
    en_sep  = new sChar(p_en_sep);
    string  = NULL;
    }  

  Iso2022_T(int dmy){ // for decord
    char p_en_sep [5];
    char p_dis_sep[5];
    
    p_en_sep[0] = 0x1b;
    p_en_sep[1] = 0x24;
    p_en_sep[2] = 0x01;
    p_en_sep[3] = 0x00;
    p_dis_sep[0] = 0x1b;
    p_dis_sep[1] = 0x28;
    p_dis_sep[2] = 0x01;
    p_dis_sep[3] = 0x00;
    dis_sep_len = strlen(p_dis_sep);
    en_sep_len  = strlen(p_en_sep);

    dis_sep = new sChar(p_dis_sep);
    en_sep  = new sChar(p_en_sep);
    string  = NULL;
    }  

  ~Iso2022_T(){  
    delete en_sep;
    delete dis_sep;
    if (string != NULL){delete [] string;}
    }

  public:  
  char *decord   (unsigned char *str);
  char *encord   (unsigned char *str);
  void sjis_jis  (unsigned char p_c1,unsigned p_c2);
  void jis_sjis  (unsigned char p_c1,unsigned p_c2);
  bool sep_check (unsigned char *str,bool mode);
  bool sjis_check(unsigned char *str);
};

#endif // _XISO2022_H_


