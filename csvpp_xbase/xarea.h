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

#ifndef _XAREA_H_
#define _XAREA_H_

#include "t_area_instance.h"

class VariableArea_C : public VariableArea_T<unsigned char>{
  public:
    VariableArea_C(int size) : VariableArea_T<unsigned char >(size) {}
    VariableArea_C(char *p)  : VariableArea_T<unsigned char >(p) {}
};

//
// Variable Array Area class
//


class VariableArray_C : public VariableArray_T <unsigned char>{
  public:
    VariableArray_C(int size) : VariableArray_T<unsigned char >(size) {delFlag = false;}
};

#endif // _XAREA_H_

