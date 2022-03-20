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
// 64bit filepointer read class
//
#include "xread64.h"

Read64_C::Read64_C(char *file_name,char *mode,int buff_size){  // mode = "rt" "rb" "csv"
  if (buff_size < 1024){buff_size = 1024;}

#ifdef R_BUFF_TEST
  buff_size = 10;
#endif
  csvFlg=false;
  rb=NULL;
  rt=NULL;
  okFlg = true;
  in_fp  = CreateFileA(file_name ,GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
  if (in_fp  == INVALID_HANDLE_VALUE){okFlg = false; return;}
  if      (strcmp(mode,"rt" )==0){rt = new File64TextRead_C(buff_size,buff_size);}
  else if (strcmp(mode,"csv")==0){rt = new File64TextRead_C(buff_size,buff_size); csvFlg=true;}
  else                           {rb = new File64Read_C(buff_size);}
}
Read64_C::~Read64_C(){
  if (rb != NULL){delete rb;}
  if (rt != NULL){delete rt;}
  if (okFlg){CloseHandle(in_fp);}
}

// # binary read #####
int  Read64_C::read(unsigned char *buff,int len){
  if ((okFlg==false) || (rb == NULL)){return(0);}
  return(rb->read(in_fp,buff,len));
}

// # text read #####
char *Read64_C::read(){
  if ((okFlg==false) || (rt == NULL)){return(NULL);}
  if (csvFlg == false){return(rt->read(in_fp));}
  else                {return(rt->csv_read(in_fp));}
  
}

// # seek #####
void Read64_C::seek(__int64 offset){
  if (okFlg==false){return;}
  if (rt != NULL){rt->seek(in_fp,offset);}
  if (rb != NULL){rb->seek(in_fp,offset);}
}

// # tell #####
__int64 Read64_C::tell(){
  if (okFlg==false){return((__int64)-1);}
  if (rt != NULL){return(rt->tell());}
  if (rb != NULL){return(rb->tell());}
  return((__int64)0);
}

// # filesize get #####
__int64 Read64_C::sizeGet(){
  if (okFlg==false){return((__int64)-1);}
  if (rt != NULL){return(rt->sizeGet(in_fp));}
  if (rb != NULL){return(rb->sizeGet(in_fp));}
  return((__int64)0);
}

// # text return code #####
bool Read64_C::return0dGet(){
  if ((okFlg==false) || (rt == NULL)){return(false);}
  return(rt->return0dGet());
}
bool Read64_C::return0aGet(){
  if ((okFlg==false) || (rt == NULL)){return(false);}
  return(rt->return0aGet());
}


// binary file interface class -----------------
File64Read_C::File64Read_C(int size) {
  g_buff      = new unsigned char[size];
  g_buff_size = size;
  g_buff_ptr  = 0;
  g_read_size = 0;
  g_total_ptr = (__int64)0;
}


File64Read_C::~File64Read_C(){
  delete [] g_buff;
}

// # 1 byte read #####
bool File64Read_C::read_c(HANDLE in_fp,unsigned char *c){
  if (g_buff_ptr == g_read_size){
    g_buff_ptr = 0;
    g_read_size = 0;
    *c = 0;
    if (ReadFile(in_fp,g_buff,g_buff_size,&g_read_size,NULL) == 0){return(false);}

#ifdef R_BUFF_TEST
    //printf("read file size = %d\n",g_read_size);
#endif

    if (g_read_size == 0) {return(false);}
  }
  *c = *(g_buff + g_buff_ptr);

//printf("ptr=%x\n",g_buff_ptr);

  g_buff_ptr++;
  g_total_ptr++;
  return(true);
}

// # n byte read #####
int File64Read_C::read(HANDLE in_fp,unsigned char *buff,int len){
  int i;

  for(i=0;i<len;i++){
    if (read_c(in_fp,(buff+i)) == false){break;}
  }
  return(i);
}

// # file seek #####
void File64Read_C::seek(HANDLE in_fp,__int64 offset){
  unsigned long size_lower;
  long size_upper_s;
  __int64 w;
  __int64 amari;

  g_total_ptr = (offset / (__int64)g_buff_size);
  g_total_ptr = g_total_ptr * (__int64)g_buff_size;

  amari = offset - g_total_ptr;


//printf("seek g_total_ptr=%I64d\n",g_total_ptr);

  w = g_total_ptr & (__int64)0xffffffff; size_lower = (unsigned long)w;
  w = g_total_ptr >> (__int64)32;        size_upper_s = (unsigned long)w;
  SetFilePointer(in_fp,size_lower,&size_upper_s,0);

  // karayomi
  int len = (int)amari;
  g_buff_ptr  = 0;
  g_read_size = 0;
  unsigned char c;
  for(int i=0;i<len;++i){
    read_c(in_fp,&c);
  }

//printf("seek offset=%I64d amari=%I64d g_total_ptr=%I64d\n",offset,amari,g_total_ptr);

}

// # file size get #####
__int64 File64Read_C::sizeGet(HANDLE in_fp){
  long size_upper_s=0;
  unsigned long size_lower = SetFilePointer(in_fp,0,&size_upper_s,2);
  __int64 w = (__int64)size_lower + ((__int64)size_upper_s << (__int64)32);
  seek(in_fp,g_total_ptr);
  return(w);
}



// text file interface class -----------------
File64TextRead_C::File64TextRead_C(int bin_buff_size,int txt_buff_size){
  g_buff      = new VariableArea_C(txt_buff_size);
  ret0dFlg    = false;
  ret0aFlg    = false;
  rb          = new File64Read_C(bin_buff_size);
  sv          = 0;
  svFlg       = false;
  g_total_ptr = (__int64)0;
  sv_total_ptr = (__int64)0;
}
File64TextRead_C::~File64TextRead_C(){
  delete    g_buff;
  delete    rb;
}

char *File64TextRead_C::csv_read(HANDLE in_fp){
  unsigned char c;
  ret0dFlg = false;
  ret0aFlg = false;
  bool ptr_mask=false;

  g_buff->init();
  bool e=false;
  if (svFlg){
    g_buff->set(sv);
    svFlg=false;
    g_total_ptr=sv_total_ptr;
    ptr_mask=true;
  }

  int mode = 0;
  int dblFlgMode = 0;
  for(;;){
    if (ptr_mask == false){
      ptr_mask = true;
      g_total_ptr = rb->tell();
    }
    if (rb->read_c(in_fp,&c) == false){e=true; break;}

    if (mode == 0){
      if (c == '\"'){
        if      (dblFlgMode == 0){dblFlgMode = 1;}
        else if (dblFlgMode == 1){dblFlgMode = 0;}
        else                     {dblFlgMode = 0;}
      }
      else if (dblFlgMode == 1){dblFlgMode = 2;}

      if (dblFlgMode == 0){
        if (c == 0x0d){ret0dFlg = true; mode = 1; continue;}
        if (c == 0x0a){ret0aFlg = true; break;}
      }
      g_buff->set(c);
    }else{
      if (c == 0x0a){ret0aFlg = true;}
      else          {sv=c; svFlg=true; sv_total_ptr = rb->tell();}
      break;
    }
  }
  g_buff->set(0x00);

  if ((strcmp((char *)g_buff->c_str(),"")==0) && (e==true)){return(NULL);}
  else                                                     {return(g_buff->c_str());}
}


// text file interface class -----------------
char *File64TextRead_C::read(HANDLE in_fp){
  unsigned char c;
  ret0dFlg = false;
  ret0aFlg = false;
  bool ptr_mask=false;

  g_buff->init();
  bool e=false;
  if (svFlg){
    g_buff->set(sv);
    svFlg=false;
    g_total_ptr=sv_total_ptr; 
    ptr_mask=true;
  }
  
  int mode = 0;
  for(;;){
    if (ptr_mask == false){
      ptr_mask = true;
      g_total_ptr = rb->tell();
    }
    if (rb->read_c(in_fp,&c) == false){e=true; break;}
    if (mode == 0){
      if (c == 0x0d){ret0dFlg = true; mode = 1; continue;}
      if (c == 0x0a){ret0aFlg = true; break;}
      g_buff->set(c);
    }else{
      if (c == 0x0a){ret0aFlg = true;}
      else          {sv=c; svFlg=true; sv_total_ptr = rb->tell();}
      break;
    }
  }
  g_buff->set(0x00);

  if ((strcmp((char *)g_buff->c_str(),"")==0) && (e==true)){return(NULL);}
  else                                                     {return(g_buff->c_str());}
}


void File64TextRead_C::seek(HANDLE in_fp,__int64 offset){
  g_total_ptr = offset;
  svFlg = false;
  rb->seek(in_fp,offset);
}

// # file size get #####
__int64 File64TextRead_C::sizeGet(HANDLE in_fp){
  return(rb->sizeGet(in_fp));
}
