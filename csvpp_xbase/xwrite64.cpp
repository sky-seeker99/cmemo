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
// 64bit filepointer write class
//

#include "xwrite64.h"

// interface class -----------------
Write64_C::Write64_C(char *out_file,char *mode,int buff_size){  // mode = "wt" "wb" "at" "con"
  if (buff_size < 1024){buff_size = 1024;}

#ifdef W_BUFF_TEST
  buff_size = 10;
#endif

  FILE *fp;
  wb = NULL;
  wt = NULL;
  okFlg = false;
  sv_mode = new VariableArea_C(10);
  sv_mode->set_string(mode);

  if (strcmp(mode,"con")!=0){
    if (FileExists(out_file) == false){
      fp = fopen(out_file,"wt");
      if (fp != NULL){fclose(fp);}
    }
    if (strcmp(mode,"wt")==0){
      fp = fopen(out_file,"wt");
      if (fp != NULL){fclose(fp);}
    }
    out_fp = CreateFileA(out_file,GENERIC_READ|GENERIC_WRITE,0x00000000     ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if (out_fp == INVALID_HANDLE_VALUE){return;}
  }

  if ((strcmp(mode,"at")==0) || (strcmp(mode,"wt")==0) || (strcmp(mode,"con")==0)){wt = new File64TextWrite_C(buff_size);}
  else                                                                            {wb = new File64Write_C(buff_size);}
  if (strcmp(mode,"at")==0){
    SetFilePointer(out_fp,0,NULL,2);
  }
  if (strcmp(mode,"con")==0){wt->conSet();}
  okFlg  = true;
}
Write64_C::~Write64_C(){
  if (wb != NULL){wb->flush(out_fp); delete wb;}
  if (wt != NULL){wt->flush(out_fp); delete wt;}
  if (strcmp(sv_mode->cc_str(),"con")!=0){
    if (okFlg){CloseHandle(out_fp);}
  }
  delete sv_mode;
}


// # binary write #####
void Write64_C::write(unsigned char *buff,int len){
  if ((okFlg==false) || (wb == NULL)){return;}
  wb->write(out_fp,buff,len);
}

// # text write #####
void Write64_C::write(unsigned char *buff){
  if ((okFlg==false) || (wt == NULL)){return;}
  wt->write(out_fp,buff);
}
void Write64_C::writeln(unsigned char *buff){
  if ((okFlg==false) || (wt == NULL)){return;}
  wt->writeln(out_fp,buff);
}

// # seek #####
void Write64_C::seek(__int64 offset){
  if (okFlg==false){return;}
  if (wt != NULL){wt->seek(out_fp,offset);}
  if (wb != NULL){wb->seek(out_fp,offset);}
}

// # flush #####
void Write64_C::flush(){
  if (okFlg==false){return;}
  if (wt != NULL){return(wt->flush(out_fp));}
  if (wb != NULL){return(wb->flush(out_fp));}
}

// # return code set #####
void Write64_C::winRetSet(){
  if ((okFlg) && (wt != NULL)){wt->winRetSet();}
}
void Write64_C::unixRetSet(){
  if ((okFlg) && (wt != NULL)){wt->unixRetSet();}
}
void Write64_C::macRetSet(){
  if ((okFlg) && (wt != NULL)){wt->macRetSet();}
}

// # csv write #####
void Write64_C::csvWrite_sub(unsigned char *str,bool gawa){
  int j,k,len;
  bool dbl_flg;
  unsigned char *buff2;
  sjisAnl_C sj;

  len = strlen((char *)str);

  dbl_flg = false;
  buff2 = new unsigned char[len*2+1];
  *buff2 = 0x00;
  sj.init();
  for(k=0,j=0;j<len;++j){
    if (sj.check((char *)str,j) == true){
      *(buff2+k) = *(str+j); ++k;
      *(buff2+k) = 0x00;
      continue;
    }
    if (*(str+j) == '"'){dbl_flg = true; *(buff2+k) = *(str+j); ++k;}
    if (*(str+j) == ','){dbl_flg = true;}
    if (*(str+j) == 0x0a){dbl_flg = true;}
    if (*(str+j) == 0x0b){dbl_flg = true;}
    if (*(str+j) == 0x0d){dbl_flg = true;}

    *(buff2+k) = *(str+j); ++k;
    *(buff2+k) = 0x00;
  }
  if (gawa) {
    if (dbl_flg == true){write((unsigned char *)"\""); write(buff2); write((unsigned char *)"\"");}
    else                {write(buff2);}
  } else {
    write(buff2);
  }
  delete [] buff2;
}


// binary file interface class -----------------
File64Write_C::File64Write_C(int size) {
  g_buff      = new unsigned char[size];
  g_buff_size = size;
  g_buff_ptr  = 0;
  g_total_ptr = (__int64)0;
}


File64Write_C::~File64Write_C(){
  delete [] g_buff;
}

// # 1 byte write #####
bool File64Write_C::write_c(HANDLE out_fp,unsigned char c){
  unsigned long writeSize;

  *(g_buff + g_buff_ptr) = c;
  g_buff_ptr++;
  g_total_ptr++;

  if (g_buff_ptr == g_buff_size){
    g_buff_ptr = 0;
	if (WriteFile(out_fp,g_buff,g_buff_size,&writeSize,NULL) == 0){return(false);}
#ifdef W_BUFF_TEST
	printf("Write File size=%d\n", writeSize);
#endif

  }
  return(true);
}

// # flush #####
void File64Write_C::flush(HANDLE out_fp){
  unsigned long writeSize;

  if (g_buff_ptr == 0){return;}
  WriteFile(out_fp,g_buff,g_buff_ptr,&writeSize,NULL);

#ifdef W_BUFF_TEST
	printf("Write File size=%d\n", writeSize);
#endif

  g_buff_ptr = 0;
}

// # n byte write #####
bool File64Write_C::write(HANDLE out_fp,unsigned char *buff,int len){
  for(int i=0;i<len;i++){
	if (write_c(out_fp,*(buff+i)) == false){return(false);}
  }
  return(true);
}

// # seek #####
void File64Write_C::seek(HANDLE out_fp,__int64 offset){
  unsigned long size_lower;
  long size_upper_s;
  __int64 w;

  flush(out_fp);
  g_total_ptr = offset;
  w = g_total_ptr & (__int64)0xffffffff; size_lower = (unsigned long)w;
  w = g_total_ptr >> (__int64)32;        size_upper_s = (unsigned long)w;
  SetFilePointer(out_fp,size_lower,&size_upper_s,0);
}


// text file interface -------------------------
File64TextWrite_C::File64TextWrite_C(int bin_buff_size){
  ret0dFlg    = true;
  ret0aFlg    = true;
  wb          = new File64Write_C(bin_buff_size);
  conFlg      = false;
}
File64TextWrite_C::~File64TextWrite_C(){
  delete wb;
}


void File64TextWrite_C::write(HANDLE out_fp,unsigned char *buff){
  if (conFlg){printf("%s",buff); return;}
  wb->write(out_fp,buff,strlen((char *)buff));
}

void File64TextWrite_C::writeln(HANDLE out_fp,unsigned char *buff){
  write(out_fp,buff);
  if (conFlg){
	//if (ret0dFlg){printf("%c",0x0d);}
    if (ret0aFlg){printf("%c",0x0a);}
  } else {
    if (ret0dFlg){wb->write_c(out_fp,0x0d);}
    if (ret0aFlg){wb->write_c(out_fp,0x0a);}
  }
}

void File64TextWrite_C::flush(HANDLE out_fp){
  if (conFlg){return;}
  wb->flush(out_fp);
}

void File64TextWrite_C::winRetSet (){ret0dFlg = true;  ret0aFlg = true;}
void File64TextWrite_C::unixRetSet(){ret0dFlg = false; ret0aFlg = true;}
void File64TextWrite_C::macRetSet (){ret0dFlg = true;  ret0aFlg = false;}

