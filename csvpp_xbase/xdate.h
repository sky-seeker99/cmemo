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
// Date/Time Class
//
#ifndef _XDATE_H_
#define _XDATE_H_

//#include "xstr.h"
#include "ztool.h"
#include "ztoken.h"

// # Date Class -----------------
class Date_T{
  public:
    int yy;    // ”N
    int mm;    // ŒŽ
    int dd;    // “ú
    int value; // ’l
    StrSrchInf_T *ssc; // String Separate Control
    Date_T();
    ~Date_T();
    int  Conv(int y,int m,int d);
    bool StrSet(char *str);
    void IntSet(int p_value);
  };

// # Time Class ------------------
class Time_T{
  public:
  int hh;    // Žž
  int mm;    // •ª
  int ss;    // •b
  int value; // ’l
  StrSrchInf_T *ssc; // String Separate Control
  Time_T();
  ~Time_T();
  int  Conv(int h,int m,int s);
  bool StrSet(char *str);
  void IntSet(int p_value);
  };


#endif // _XDATE_H_
