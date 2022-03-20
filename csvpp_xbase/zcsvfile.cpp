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

#include "zcsvfile.h"

int CsvInf_T::CsvError_Rd(){
  if (in_fp->okCheck()){return(0);}
  return(-1);
}
int CsvInf_T::CsvFgets(){
  buff = in_fp->read();
  if (buff == NULL){return(-1);}
  csvi->Exec(buff);
  line++;
  return(0);
}
int CsvInf_T::CsvSelSu_Rd(){
  if (buff == NULL){return(0);}
  return(csvi->CsvSelSu_Rd());
}
char *CsvInf_T::CsvSel_Rd(int no){
  if (buff == NULL){return(NULL);}
  return(csvi->CsvSel_Rd(no));
}

char **CsvInf_T::CsvSel_Rd(){
  if (buff == NULL){return(NULL);}
  return(csvi->CsvSel_Rd());
}

void CsvInf_T::CsvWrite(FILE *out_fp,char *str)
  {
  int j,k,len;
  bool dbl_flg;
  char *buff2;
  JisInf_T *sj;

  len = strlen(str);
  sj  = new JisInf_T;

  dbl_flg = false;
  buff2 = new char[len*2+1];
  *buff2 = 0x00;
  for(k=0,j=0;j<len;++j)
    {
    if (sj->SJisCheck(str,j) == true)
      {
      *(buff2+k) = *(str+j); ++k;
      *(buff2+k) = 0x00;
      continue;
      }
    if (*(str+j) == '"'){dbl_flg = true; *(buff2+k) = *(str+j); ++k;}
    if (*(str+j) == ','){dbl_flg = true;}
    *(buff2+k) = *(str+j); ++k;
    *(buff2+k) = 0x00;
    }
  if (dbl_flg == true){fprintf(out_fp,"\"%s\"",buff2);}
  else                {fprintf(out_fp,"%s",buff2);}
  delete [] buff2;
  delete sj;
  }

void CsvInf_T::CsvError2(char *msg,int clm)  // Ver 1.03
  {
  if ((csvi->sel_su > clm) && (clm >= 0))
	{
	if (*(csvi->sel+clm) != MM_NULL)
	  {
	  printf("%s  SEL=%c%d  FILE=%s  SEL_DATA=%s\n",msg,clm+'A',line,file_name->c_str(),*(csvi->sel+clm));
	  }
	else
	  {
	  printf("%s  SEL=%c%d  FILE=%s  SEL_DATA=NULL\n",msg,clm+'A',line,file_name->c_str());
	  }
	}
  else
	{
	printf("%s  SEL=%c%d  FILE=%s\n",msg,clm+'A',line,file_name->c_str());
	}
  }

void CsvInf_T::CsvError(char *msg,int clm)  // Ver 1.03
  {
  if ((csvi->sel_su > clm) && (clm >= 0))
	{
	if (*(csvi->sel+clm) != MM_NULL)
	  {
	  printf("CSV File Read Error:%s  SEL=%c%d  FILE=%s  SEL_DATA=%s\n",msg,clm+'A',line,file_name->c_str(),*(csvi->sel+clm));
	  }
	else
	  {
	  printf("CSV File Read Error:%s  SEL=%c%d  FILE=%s  SEL_DATA=NULL\n",msg,clm+'A',line,file_name->c_str());
	  }
	}
  else
	{
	printf("CSV File Read Error:%s  SEL=%c%d  FILE=%s\n",msg,clm+'A',line,file_name->c_str());
	}
  }

void CsvInf_T::CsvClmMsg(sChar *str,int clm) // Ver 1.05
  {
  char a[2];
  a[0] = clm+'A';
  a[1] = 0x00;
  str->cat("[");
  str->cat(a);
  str->intcat(line);
  str->cat("]");
  }

char *CsvInf_T::xFgetsBuff_Rd(){return(buff);}  // Ver 1.04

