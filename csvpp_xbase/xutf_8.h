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

#ifndef _XUTF_8_H_
#define _XUTF_8_H_


class Utf8Area_C{

  public:
    wchar_t *w_str;
    int w_str_size;
    char *utf_str;
    int utf_str_size;
  
    Utf8Area_C(int size) {
      w_str = new wchar_t[size+1];
      w_str_size = size;
      utf_str = new char [size+1];
      utf_str_size = size;
    }
    
    ~Utf8Area_C(){
      delete [] w_str;
      delete [] utf_str;
    }
    char *toUtf_8(char *buff);
    char *fromUtf_8(char *buff);

  private:  
    void sizeCheck(int size,bool wstrFlg);

};


#endif // _XUTF_8_H_

