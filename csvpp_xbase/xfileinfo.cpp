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




