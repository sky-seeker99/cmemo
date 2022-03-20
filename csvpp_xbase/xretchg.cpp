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

    
