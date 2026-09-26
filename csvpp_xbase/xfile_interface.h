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
 
#ifndef _XFILE_INTERFACE_
#define _XFILE_INTERFACE_ 
 

 
#include "zfile.h" 
#include "zfilename_anl.h"

// # file-interface class (file_inf) ----------------
class file_inf{
  public:
    FILE   *out_fp;
    FILE   *in_fp;
    sChar *temp_name;
    sChar *in_file_name;
    bool   ok_flg;
    bool   in_con_flg;
    bool   out_con_flg;

  void file_list_get(MemHeader_T *it,MEM_FP *mem_fp,char *file_mask);
  bool same_file_open(char *file);
  void same_file_close();
  bool file_open(char *in_file,char *out_file);
  void file_close();
  };
  
#endif // _XFILE_INTERFACE_  
  
