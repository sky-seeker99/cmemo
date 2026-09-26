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
// 64bit filepointer temporary write class
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <condefs.h>
#include "xtemp_file64.h"

// # temprorary file -> user file copy #####
void  FileTemp64_T::tempFileToCopy(char *p_name){
  if (in_fp  != NULL){delete in_fp;}
  if (out_fp != NULL){delete out_fp;}
  in_fp  = NULL;
  out_fp = NULL;
  
  unsigned char *buff = new unsigned char[buff_size];
  DeleteFile(p_name);
  Write64_C *o_fp = new Write64_C(p_name       ,"wb",buff_size);
  Read64_C  *i_fp = new Read64_C (name->c_str(),"rb",buff_size);
  if ((o_fp->okCheck()) && (i_fp->okCheck())){
    for(;;){
      int readSize = i_fp->read(buff,buff_size);
      if (readSize == 0){break;}
      o_fp->write(buff,readSize);
    }
  }
  delete [] buff;
  delete o_fp;
  delete i_fp;
}

// # read mode change #####
void  FileTemp64_T::readModeChg(char *mode){
  if (in_fp  != NULL){delete in_fp;}
  if (out_fp != NULL){delete out_fp;}
  in_fp  = NULL;
  out_fp = NULL;
  in_fp = new Read64_C(name->c_str(),mode,buff_size);
}

// # write mode change #####
void  FileTemp64_T::writeModeChg(char *mode){
  if (in_fp  != NULL){delete in_fp;}
  if (out_fp != NULL){delete out_fp;}
  in_fp  = NULL;
  out_fp = NULL;
  out_fp = new Write64_C(name->c_str(),mode,buff_size);
}


