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
 
#include <condefs.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <dos.h>
#include "xfile_interface.h"

// # in-file != out-file open #####
bool file_inf::file_open(char *in_file,char *out_file)
  {
  ok_flg = true;
  if (strcmp(in_file,out_file) == 0)
    {
    if (strcmp(in_file,"-") != 0)
      {
      ok_flg = false;
      return(false);
      }
    }

  if (strcmp(in_file,"-") == 0) // console in
    {
    in_con_flg = true;
    FileTemp_T *temp = new FileTemp_T("txt");
    in_file_name = new sChar(temp->GetName());
    delete temp;
    FILE *temp_fp = fopen(in_file_name->c_str(),"wt");  // chg
    char bb[4096];
    for(;;)
      {
      if (gets(bb) == NULL){break;}
 // printf("%s\n",bb);
      fprintf(temp_fp,"%s\n",bb);
      }
    fclose(temp_fp);
    }
  else
    {
    in_con_flg = false;
    in_file_name = new sChar(in_file);
    }
  in_fp   = fopen(in_file_name->c_str() ,"rt");

  if (in_fp == NULL)
    {
    printf("file open error.(file=%s)\n",in_file_name->c_str());
    if (in_con_flg) // console in
      {
      String s = in_file_name->c_str();
      DeleteFile(s);
      }
    delete in_file_name;
    ok_flg = false;
    return(false);
    }
    
  if (strcmp(out_file,"-") == 0)
    {
    out_con_flg = true;
    out_fp = NULL;
    }
  else
    {
    out_con_flg = false;  
    out_fp = fopen(out_file,"wb");  // chg
    if (out_fp == NULL)
      {
      printf("file open error.(file=%s)\n",in_file_name->c_str());
      if (in_con_flg) // console in
        {
        String s = in_file_name->c_str();
        DeleteFile(s);
        }
      delete in_file_name;
      ok_flg = false;
      return(false);
      }
    }
    
    
  if (in_con_flg)
    {
    if (out_con_flg){fprintf(stderr," stdin -> stdout\n");}
    else            {fprintf(stderr," stdin -> %s\n",out_file);}
    }
  else
    {
    if (out_con_flg){fprintf(stderr," %s -> stdout\n",in_file);}
    else            {fprintf(stderr," %s -> %s\n",in_file,out_file);}
    }  
  
    
  return(true);
  }


// # in-file != out-file close #####
void file_inf::file_close()
  {
  if (ok_flg == false){return;}
  if (in_con_flg) // console in
    {
    String s = in_file_name->c_str();
    DeleteFile(s);
    }
  delete in_file_name;
  if (in_fp  != NULL){fclose(in_fp );}
  if (out_fp != NULL){fclose(out_fp);}
  }

 
// # wild card file list get #####
void file_inf::file_list_get(MemHeader_T *it,MEM_FP *mem_fp,char *file_mask)
  {
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
bool file_inf::same_file_open(char *file)
  {
  FileTemp_T *temp = new FileTemp_T("tmp");
  temp_name = new sChar(temp->GetName());
  delete temp;
  out_fp  = fopen(temp_name->c_str(),"wb"); // chg
  in_fp   = fopen(file              ,"rt");
  ok_flg = true;
  if ((out_fp == NULL) || (in_fp == NULL))
    {
    ok_flg = false;
    if (out_fp == NULL  ){printf("file open error.(file=%s)\n",temp_name->c_str());}
    if (in_fp  == NULL  ){printf("file open error.(file=%s)\n",file);}
    if (out_fp != NULL){fclose(out_fp);}
    if (in_fp  != NULL){fclose(in_fp);}
    out_fp= NULL;
    in_fp = NULL;
    String s = temp_name->c_str();
    DeleteFile(s);
    delete temp_name;
    }
  in_file_name = new sChar(file);
  return(ok_flg);
  }

// # in-file = out-file open #####
void file_inf::same_file_close()
  {
  if (ok_flg == false){return;}
  if (out_fp != NULL  ){fclose(out_fp);}
  if (in_fp  != NULL  ){fclose(in_fp);}
  out_fp = fopen(in_file_name->c_str(),"wb");  // chg
  fclose(out_fp);
  printf("convert %s\n",in_file_name->c_str());
  int out_hp  = open(in_file_name->c_str(),O_RDWR   | O_BINARY);
  int in_hp   = open(temp_name->c_str()   ,O_RDONLY | O_BINARY);
  char ch[10240];

  lseek(in_hp,0,SEEK_SET);
  lseek(out_hp,0,SEEK_SET);
  for(;;)
    {
    int buff_size = read (in_hp, ch, 10240);
    if (buff_size <= 0){break;}
    write(out_hp, ch, buff_size);

//printf("size=%d\n",buff_size);

    }
  if (out_hp != -1  ){close(out_hp);}
  if (in_hp  != -1  ){close(in_hp);}
  printf("complete.\n");
  String s = temp_name->c_str();
  DeleteFile(s);
  delete temp_name;
  delete in_file_name;
  }
  
    
  
