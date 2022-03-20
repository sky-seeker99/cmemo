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
// Variable length Area class
//
#include <stdio.h>
#include <string.h>
#include <condefs.h>
#include <winnls.h>

#include "xutf_8.h"

char *Utf8Area_C::toUtf_8(char *buff){
  AnsiString as;
  as = buff;
  int size = as.WideCharBufSize();
  sizeCheck(size,true);
  as.WideChar(w_str,size);

  size = ::WideCharToMultiByte(CP_UTF8,0,w_str,-1,NULL,0,NULL,NULL);
  sizeCheck(size,false);
  ::WideCharToMultiByte(CP_UTF8,0,w_str,-1,utf_str,size,NULL,NULL);
  return(utf_str);
}


char *Utf8Area_C::fromUtf_8(char *buff){
  int size = ::MultiByteToWideChar(CP_UTF8,0,buff,-1,NULL,0);
  sizeCheck(size,true);
  ::MultiByteToWideChar(CP_UTF8,0,buff,-1,w_str,size);

  AnsiString as;
  as = w_str;
  size = as.Length();
  sizeCheck(size,false);
  strcpy(utf_str,as.c_str());
  return(utf_str);
}


void Utf8Area_C::sizeCheck(int size,bool wstrFlg){
  if (wstrFlg){
    if (w_str_size < size ){
      delete [] w_str;
      w_str_size = size * 2;
      w_str = new wchar_t[w_str_size+1];
    }
  }
  else {
    if (utf_str_size < size){
      delete [] utf_str;
      utf_str_size = size * 2;
      utf_str = new char[utf_str_size+1];
    }
  }
}

