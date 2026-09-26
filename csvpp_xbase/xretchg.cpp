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
//  CSVPP Front End Control cell inner return -> string repalce 
// 

#include <string.h>
#include <stdio.h>
#include <condefs.h>
#include "xretchg.h"

// # constructor #####
csvretchg_T::csvretchg_T(char *p_name,char *ins_string){
  FileTemp_T  *temp_fp        = new FileTemp_T("tmp");
  FILE        *out_fp         = temp_fp->fp;
  FILE        *in_fp          = fopen(p_name,"rt");
  if (in_fp == NULL){return;}

  convert(in_fp,out_fp,ins_string);

  fclose(in_fp);
  DeleteFile(p_name);
  fclose(out_fp);
  temp_fp->TempToRename(p_name);
  delete temp_fp;
}

// # destructor #####
csvretchg_T::~csvretchg_T(){
}

// # convert #####
void csvretchg_T::convert(FILE *in_fp,FILE *out_fp,char *ins_string){
xFgets_T *xf;
JisInf_T *sj;
char *buff;
int len,i;
char crlf[2];
bool dbl_flg;
sChar *str;

//crlf[0] = 0x0d;
crlf[0] = 0x0a;
crlf[1] = 0x00;

xf  = new xFgets_T(1024);
sj  = new JisInf_T;
str = new sChar("");
dbl_flg = false;
for(;;)
  {
  buff = xf->xFgets(in_fp);
  if (buff == NULL){break;}

  if (*(buff+strlen(buff)-1) < ' '){*(buff+strlen(buff)-1) = 0x00;}

  len = strlen(buff);
  for(i=0;i<len;++i)
    {
    if (sj->SJisCheck(buff,i) == true){continue;}
    if (*(buff+i) == '"')
      {
      if (dbl_flg == true){dbl_flg = false;}
      else                {dbl_flg = true;}
      }
    }
  str->cat(buff);
  if (dbl_flg == true)
    {
    str->cat(ins_string);
    }
  else
    {
    str->cat(crlf);
    fputs(str->c_str(),out_fp);
    str->set("");
    }
  }

if (strcmp(str->c_str(),"") != 0)
  {
  fprintf(out_fp,"%s",str->c_str());
  }

delete str;
delete xf;
delete sj;
}

    
