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
#include <windows.h>
#include "xfileinfo.h"

bool fileinfo_C::Get(char *file){
  HANDLE handle;
  FILETIME   c;
  FILETIME   a;
  FILETIME   r;
  FILETIME   chg;
  SYSTEMTIME st;

  handle = CreateFile(file,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
  if(handle == INVALID_HANDLE_VALUE){return(false);}
  GetFileTime(handle,&c,&a,&r);
  CloseHandle(handle);

  // 作成日時
  FileTimeToLocalFileTime(&c, &chg);  FileTimeToSystemTime(&chg, &st);
  c_Year         =    st.wYear;
  c_Month        =    st.wMonth;
  c_Day          =    st.wDay;
  c_Hour         =    st.wHour;
  c_Minute       =    st.wMinute;
  c_Second       =    st.wSecond;
  c_Milliseconds =    st.wMilliseconds;

  // 最終アクセス日時
  FileTimeToLocalFileTime(&a, &chg);  FileTimeToSystemTime(&chg, &st);
  a_Year         =    st.wYear;
  a_Month        =    st.wMonth;
  a_Day          =    st.wDay;
  a_Hour         =    st.wHour;
  a_Minute       =    st.wMinute;
  a_Second       =    st.wSecond;
  a_Milliseconds =    st.wMilliseconds;

  // 最終更新日時
  FileTimeToLocalFileTime(&r, &chg);  FileTimeToSystemTime(&chg, &st);
  r_Year         =    st.wYear;
  r_Month        =    st.wMonth;
  r_Day          =    st.wDay;
  r_Hour         =    st.wHour;
  r_Minute       =    st.wMinute;
  r_Second       =    st.wSecond;
  r_Milliseconds =    st.wMilliseconds;

  return (true);
}




