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
// Variable length Area stack&fifo class
//

#ifndef _XAREA_STACK_H_
#define _XAREA_STACK_H_

#include "t_area_instance.h"

class VariableStack_C : public VariableStack_T<unsigned char>{
  public:
    VariableStack_C(int size): VariableStack_T<unsigned char>(size){delFlag=false;}
};

class VariableFifo_C : public VariableFifo_T<unsigned char>{
  public:
    VariableFifo_C(int size): VariableFifo_T<unsigned char>(size){delFlag=false;}
};

#endif // _XAREA_H_

