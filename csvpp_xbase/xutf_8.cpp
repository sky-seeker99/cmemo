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
// Variable length Area class
//
#include <stdio.h>
#include <string.h>
#include <condefs.h>
#include <winnls.h>

#include "xutf_8.h"

char *Utf8Area_C::toUtf_8(char *buff){
  AnsiString as;
  as = buff;
  int size = as.WideCharBufSize();
  sizeCheck(size,true);
  as.WideChar(w_str,size);

  size = ::WideCharToMultiByte(CP_UTF8,0,w_str,-1,NULL,0,NULL,NULL);
  sizeCheck(size,false);
  ::WideCharToMultiByte(CP_UTF8,0,w_str,-1,utf_str,size,NULL,NULL);
  return(utf_str);
}


char *Utf8Area_C::fromUtf_8(char *buff){
  int size = ::MultiByteToWideChar(CP_UTF8,0,buff,-1,NULL,0);
  sizeCheck(size,true);
  ::MultiByteToWideChar(CP_UTF8,0,buff,-1,w_str,size);

  AnsiString as;
  as = w_str;
  size = as.Length();
  sizeCheck(size,false);
  strcpy(utf_str,as.c_str());
  return(utf_str);
}


void Utf8Area_C::sizeCheck(int size,bool wstrFlg){
  if (wstrFlg){
    if (w_str_size < size ){
      delete [] w_str;
      w_str_size = size * 2;
      w_str = new wchar_t[w_str_size+1];
    }
  }
  else {
    if (utf_str_size < size){
      delete [] utf_str;
      utf_str_size = size * 2;
      utf_str = new char[utf_str_size+1];
    }
  }
}

