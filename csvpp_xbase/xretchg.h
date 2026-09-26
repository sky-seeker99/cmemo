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
 
 
#ifndef _XRETCHG_H_ 
#define _XRETCHG_H_

#include "xfile.h"

class csvretchg_T{
    public:
    csvretchg_T(char *p_file,char *ins_string);
    ~csvretchg_T();
    void convert(FILE *in_fp,FILE *out_fp,char *ins_string);

};

#endif // _XRETCHG_H_

