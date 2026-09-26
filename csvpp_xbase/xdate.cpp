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

#include "xdate.h"

// # Date Class ----------
Date_T::Date_T(){
  ssc = new StrSrchInf_T();
  ssc->Set(' ');
  ssc->Set('/');
  }
Date_T::~Date_T(){
  delete ssc;
  }
int Date_T::Conv(int y,int m,int d){
  int i;

  i=(y-1)/4 + (y-1)*365 + (m-1)*31 + d;
  if (m > 2)
    {
    if ((y%4) == 0){i-=2;}
    else           {i-=3;}
    }
  if (m > 4){i-=1;}
  if (m > 6){i-=1;}
  if (m > 9){i-=1;}
  if (m > 11){i-=1;}
  return(i);
  }
bool Date_T::StrSet(char *str){
  sChar *w;
  ssc->Main(str);
  if (ssc->GetSu() != 3){return(false);}
  w = ssc->Get(0); yy = SujiConvEx(w->c_str());
  w = ssc->Get(1); mm = SujiConvEx(w->c_str());
  w = ssc->Get(2); dd = SujiConvEx(w->c_str());
  value = Conv(yy,mm,dd);
  return(true);
  }

void Date_T::IntSet(int p_value){
  value = p_value;
  for(yy=value/365;;++yy)
    {
    if (value < Conv(yy,1,1)){--yy; break;}
    }

  for(mm=0;;++mm)
    {
    if (value < Conv(yy,mm,1)){--mm; break;}
    }
  dd = value - Conv(yy,mm,1)+1;
  }

// # Time Class ----------
Time_T::Time_T(){
  ssc = new StrSrchInf_T();
  ssc->Set(' ');
  ssc->Set(':');
  }
Time_T::~Time_T(){
  delete ssc;
  }
int Time_T::Conv(int h,int m,int s){
  int i;
  i = h*60*60 + m*60 + s;
  return(i);
  }
bool Time_T::StrSet(char *str){
  sChar *w;
  ssc->Main(str);
  if (ssc->GetSu() != 3){return(false);}
  w = ssc->Get(0); hh = SujiConvEx(w->c_str());
  w = ssc->Get(1); mm = SujiConvEx(w->c_str());
  w = ssc->Get(2); ss = SujiConvEx(w->c_str());
  value = Conv(hh,mm,ss);
  return(true);
  }
void Time_T::IntSet(int p_value){
  value = p_value;
  hh = value / 3600; value -= hh*3600;
  mm = value / 60;   value -= mm*60;
  ss = value;
  value = p_value;
  }




