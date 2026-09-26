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
#include "zsjis_anl.h"


// # Jis-code checker -----------------------------------
bool sjisAnl_C::check(char *str,int ptr){
  unsigned char w1;
  unsigned char w2;
  if (flag == true){flag = false;return(true);}
  w1 = (unsigned char)*(str+ptr);
  w2 = (unsigned char)*(str+ptr+1);
  if   (((w1 >= 0x81) && (w1 <= 0x9f)) || ((w1 >= 0xe0) && (w1 <= 0xfe))){
    if (((w2 >= 0x40) && (w2 <= 0x7e)) || ((w2 >= 0x80) && (w2 <= 0xfc))){flag = true;}
  }
  return(flag);
}



