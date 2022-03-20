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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <condefs.h>


#include "xfile_interface64.h"

 // # csv-file != in-file open #####
bool fileInf64_C::csv_file_open(char *p_csv_file,char *p_csv_mode,char *p_in_file,char *p_in_mode){
  char in_file[4096];
  char csv_file[4096];

  ok_flg = false;
  if (_fullpath(in_file ,p_in_file ,4096) == NULL){return(false);}
  if (_fullpath(csv_file,p_csv_file,4096) == NULL){return(false);}
  if (strcmp(in_file,csv_file) == 0){return(false);}

  in_fp  = new Read64_C(in_file,p_in_mode,buff_size);
  csv_fp = new Read64_C(csv_file,p_csv_mode,buff_size);

  if (in_fp->okCheck() == false){return(false);}
  if (csv_fp->okCheck() == false){return(false);}

  fprintf(stderr," csv file = %s / input file = %s\n",csv_file,in_file);

  ok_flg = true;
  return(true);
}


 // # in-file != out-file open #####
bool fileInf64_C::file_open(char *p_in_file,char *p_in_mode,char *p_out_file,char *p_out_mode){
  char in_file[4096];
  char out_file[4096];

  ok_flg = false;
  in_con_flg = false;
  if (_fullpath(in_file ,p_in_file ,4096) == NULL){return(false);}
  if (_fullpath(out_file,p_out_file,4096) == NULL){return(false);}
  if (strcmp(in_file,out_file) == 0){return(false);}

  in_fp  = new Read64_C(in_file,p_in_mode,buff_size);
  out_fp = new Write64_C(out_file,p_out_mode,buff_size);

  if (in_fp->okCheck() == false){return(false);}
  if (out_fp->okCheck() == false){return(false);}

  fprintf(stderr," %s -> %s\n",in_file,out_file);

  ok_flg = true;
  return(true);
}


 // # in-file != out-file open #####
bool fileInf64_C::file_open(char *p_in_file,char *p_out_file){
  char in_file[4096];
  char out_file[4096];
  
  ok_flg = false;
  if (strcmp(p_in_file,"-")==0){strcpy(in_file,"-");}
  else if (_fullpath(in_file ,p_in_file ,4096) == NULL){return(false);}
  if (strcmp(p_out_file,"-")==0){strcpy(out_file,"-");}
  else if (_fullpath(out_file,p_out_file,4096) == NULL){return(false);}
  if ((strcmp(in_file,out_file) == 0) && (strcmp(in_file,"-") != 0)){return(false);}

  temp = new FileTemp64_T("txt","wt",buff_size);
  if (strcmp(in_file,"-") == 0) { // console in
    in_con_flg = true;
    unsigned char bb[4096];
    for(;;){
	  if (gets((char *)bb) == NULL){break;}

	  int len = strlen((char *)bb);
	  if (len > 0) {
		for(int i=len-1;i>=0;i--){
		  if ((*(bb+i)==0x0a) || (*(bb+i)==0x0d)) {
			*(bb+i)=0x00;
		  } else {
            break;
          }
		}
	  }
	  temp->out_fp->writeln(bb);
    }
    temp->readModeChg("rt");
    in_fp = temp->in_fp;
  } else {
    in_con_flg = false;
    in_fp = new Read64_C(in_file,"rt",buff_size);
  }

  if (in_fp->okCheck() == false){return(false);}
    
  if (strcmp(out_file,"-") == 0){
    out_con_flg = true;
    out_fp = new Write64_C(out_file,"con",buff_size);
    }
  else{
    out_con_flg = false; 
    out_fp = new Write64_C(out_file,"wt",buff_size);
  }
  if (out_fp->okCheck() == false){return(false);}
    
  if (in_con_flg){
    if (out_con_flg){fprintf(stderr," stdin -> stdout\n");}
    else            {fprintf(stderr," stdin -> %s\n",out_file);}
  }
  else{
    if (out_con_flg){fprintf(stderr," %s -> stdout\n",in_file);}
    else            {fprintf(stderr," %s -> %s\n",in_file,out_file);}
  }  
  
  ok_flg = true;  
  return(true);
}


// # in-file != out-file close #####
void fileInf64_C::file_close(){
  if (temp   != NULL){delete temp;   }
  else if (in_fp  != NULL){delete in_fp;  }
  if (out_fp != NULL){delete out_fp; }
  in_fp =NULL;
  out_fp=NULL;
  temp  =NULL;

}

// # in-file != out-file close #####
void fileInf64_C::csv_file_close(){
  if (in_fp != NULL){delete in_fp;}
  if (csv_fp != NULL){delete csv_fp;}
  in_fp  = NULL;
  csv_fp = NULL;
}

 
// # wild card file list get #####
void fileInf64_C::file_list_get(char *file_mask){
  sChar *ww;
  MEM_LOOP(ww,sChar,mem_fp)
    delete ww;
  LOOP_END
  mem_fp->mem_del_all();

  char *w;
  FileNameAnl_C *fns = new  FileNameAnl_C;
  FileGet_T     *fg = new FileGet_T(it);
  FileGetCell_T *fgc;
  fg->get(file_mask,0);
  fns->Set(file_mask);
  sChar *dir =new sChar("");
  w = fns->DrvGet(); if (*w != 0x00){dir->cat(w);}
  w = fns->DirGet(); if (*w != 0x00){dir->cat(w);}
  

  sChar *file=new sChar("");
  MEM_LOOP(fgc,FileGetCell_T,fg->mem_fp)
    if (fgc->attr & FA_DIREC){continue;}
    if (fgc->attr & FA_LABEL){continue;}
    file->set(dir->c_str());
    file->cat(fgc->name->c_str());
    it->alloc_ptr = new sChar(file->c_str());
    mem_fp->mem_alloc();
  LOOP_END

  delete file;
  delete fns;
  delete dir;
  delete fg;
  }
 
 
// # in-file = out-file open #####
bool fileInf64_C::same_file_open(char *p_in_file){
  char in_file[4096];
  ok_flg = false;
  
  if (_fullpath(in_file ,p_in_file ,4096) == NULL){return(false);}
  temp   = new FileTemp64_T("txt","wt",buff_size);
  out_fp = temp->out_fp;
  in_fp  = new Read64_C(in_file,"rt",buff_size);

  if (in_fp->okCheck()  == false){return(false);}
  if (out_fp->okCheck() == false){return(false);}
  sv_in_file->set(in_file);
  ok_flg = true;
  return(ok_flg);
}

// # in-file = out-file open #####
bool fileInf64_C::same_file_open_bin(char *p_in_file){
  char in_file[4096];
  ok_flg = false;

  if (_fullpath(in_file ,p_in_file ,4096) == NULL){return(false);}
  temp   = new FileTemp64_T("txt","wb",buff_size);
  out_fp = temp->out_fp;
  in_fp  = new Read64_C(in_file,"rb",buff_size);

  if (in_fp->okCheck()  == false){return(false);}
  if (out_fp->okCheck() == false){return(false);}
  sv_in_file->set(in_file);
  ok_flg = true;
  return(ok_flg);
}

// # in-file = out-file open #####
void fileInf64_C::same_file_close(){
  delete in_fp;
  if (ok_flg){temp->tempFileToCopy(sv_in_file->c_str());}
  delete temp;
  
  in_fp  = NULL;
  out_fp = NULL;
  temp   = NULL;
}
  
    
  
