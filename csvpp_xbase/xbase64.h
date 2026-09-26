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

#ifndef _XBASE64_H_
#define _XBASE64_H_

#include "zschar.h"
#include "ztool.h"

//
// BASE64 Converter
//

const  char  b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

// interface class -----------------
class Base64_T{
  private:
  char  *out_buff;
  char  *out_buff_en;
  char  *string;
  char  *string_en;
  unsigned char g_c1,g_c2;
  sChar *en_sep;
  sChar *dis_sep;
  int en_sep_len;
  int dis_sep_len;

  public:
  Base64_T()
    {
    out_buff     = NULL;
    out_buff_en  = NULL;
    en_sep       = new sChar("=?ISO-2022-JP?B?");
    dis_sep      = new sChar("?=");
    en_sep_len   = strlen(en_sep->c_str());
    dis_sep_len  = strlen(dis_sep->c_str());
    string       = NULL;
    string_en    = NULL;
    }
  ~Base64_T()
    {
    delete en_sep;
    delete dis_sep;
    if (out_buff    != NULL){delete [] out_buff;}
    if (out_buff_en != NULL){delete [] out_buff_en;}
    if (string      != NULL){delete [] string;}
    if (string_en   != NULL){delete [] string_en;}
    }
    
  char *decode(unsigned char *str);
  char *encode(unsigned char *str);
  char *sjis_base64(char *buff);
  char *sjis_base64_attach(char *buff,int len);
  char *base64_sjis(char *buff);
  char *base64_sjis_attach(char *buff);  
  void  sjis_jis(unsigned char p_c1,unsigned p_c2);
  void  jis_sjis(unsigned char p_c1,unsigned p_c2);
  int   sep_check(unsigned char *str,bool mode);
  bool  sjis_check(unsigned char *str);
  void  en_set(char *str){en_sep->set(str); en_sep_len = strlen(en_sep->c_str());}
  void  dis_set(char *str){dis_sep->set(str); dis_sep_len = strlen(dis_sep->c_str());}

  };

#endif // _XBASE64_H_
