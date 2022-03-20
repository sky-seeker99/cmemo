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




