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


