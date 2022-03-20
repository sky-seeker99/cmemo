/*

 * Copyright (c) 2003 Shigeru Kasuya (sky_seeker99@users.sourceforge.jp)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
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


