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
#include <condefs.h>
#include "ztemp.h"


// temp file close
void tempFileInf_C::closeTempFile(){
  if (in_fp  != NULL){delete in_fp;}
  if (out_fp != NULL){delete out_fp;}
  in_fp  = NULL;
  out_fp = NULL;
}

// temp file open
Write64_C *tempFileInf_C::openTempFile(char *p_ext,char *mode,int buff_size){
  closeTempFile();
  out_fp = new Write64_C(getTempFileName(p_ext),mode,buff_size);
  return(out_fp);
}


// temp file mode chnage
Read64_C *tempFileInf_C::changeReadMode(char *mode,int buff_size){
  closeTempFile();
  in_fp = new Read64_C(curr_temp,mode,buff_size);
  return(in_fp);
}

Write64_C *tempFileInf_C::changeWriteMode(char *mode,int buff_size){
  closeTempFile();
  out_fp = new Write64_C(curr_temp,mode,buff_size);
  return(out_fp);
}


// get temp file name
char *tempFileInf_C::getTempFileName(char *p_ext){
  for(int i=0;;++i){
    path->set_string(temp->cc_str());
    path->int_cat(i);
    path->cat_string(".");
    path->cat_string(p_ext);
    if (FileExists(path->cc_str()) == true){continue;}
    break;
  }
  VariableArea_C *w = new VariableArea_C(path->cc_str());
  file_fp->mem_alloc((unsigned char *)w);
  curr_temp = w->cc_str();
  return(curr_temp);
}

// remove temp file
void tempFileInf_C::removeAllFile(){
  VariableArea_C *w;
  ARY_LOOP(w,VariableArea_C,file_fp)
    for(int i=0;i<10;i++){
      if (FileExists(w->cc_str()) == false){break;}
      if (DeleteFile(w->cc_str())){break;}
      Sleep(500);
    }
  LOOP_END
  ARY_LOOP(w,VariableArea_C,file_fp)
    delete w; 
  LOOP_END
  file_fp->clear();
}

// current temp file copy
void  tempFileInf_C::tempFileToCopy(char *p_name,int buff_size){
  closeTempFile();
  if (curr_temp == NULL){return;}
  unsigned char *buff = new unsigned char[buff_size];
  Write64_C *o_fp = new Write64_C(p_name   ,"wb",buff_size);
  Read64_C  *i_fp = new Read64_C (curr_temp,"rb",buff_size);
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


