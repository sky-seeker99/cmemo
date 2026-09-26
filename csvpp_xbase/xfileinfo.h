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
// file information class
//

#ifndef _XFILEINFO_H_
#define _XFILEINFO_H_



class fileinfo_C{
  public:
    int c_Year;
    int c_Month;
    int c_Day;
    int c_Hour;
    int c_Minute;
    int c_Second;
    int c_Milliseconds;
    
    int a_Year;
    int a_Month;
    int a_Day;
    int a_Hour;
    int a_Minute;
    int a_Second;
    int a_Milliseconds;
    
    int r_Year;
    int r_Month;
    int r_Day;
    int r_Hour;
    int r_Minute;
    int r_Second;
    int r_Milliseconds;

    bool Get(char *file);
};  

#endif // _XFILEINFO_H_
