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
//  TOOLKIT LIBRARY
//

#include "t_tool.h"

/// templete version --------------



template <class T>
bool convStrNumCheck(char *str){
  bool minus;
  T out;
  int i;

  minus = false;
  i = 0;
  if (*str == '-'){
    for(i=1;;++i){
      if (*(str+i) != ' '){break;}
    }
    minus = true;
  }

  out = convStrNumSub<T>(str+i);
  if (out == (T)-1){return(false);}
  return(true);
}


template <class T>
T convStrNum(char *str){
  bool minus;
  T out;
  int i;

  minus = false;
  i = 0;
  if (*str == '-'){
    for(i=1;;++i){
      if (*(str+i) != ' '){break;}
    }
    minus = true;
  }

  out = convStrNumSub<T>(str+i);
  if (out == (T)-1){return((T)-1);}
  if (minus == true){out = out * (T)-1;}
  return(out);
}

template <class T>
T convStrNumSub(char *str){
  char *area;
  int len;
  T out;
  int i;

  len = strlen(str);

  for(i=0;i<len;++i){
    if ((*(str+i) == '0') && (*(str+i+1) == 'x')){
      *(str+i) = '\'';
      *(str+i+1) = 'h';
    }

    if (*(str+i) == '\''){
      str = str+i;
      len = strlen(str);
    }
  }

  area = new char [len+1];
  strcpy(area,str);

  if ((*str     == '\'') && (len > 2)) {bitc_ex(area+2,len-2);}
  else                                 {bitc_ex(area,len);}
  out = convStrNumSub_2<T>(area);
  delete [] area;
  return(out);
}

template <class T>
T convStrNumSub_2(char *str){
  T out = (T)-1;
  if      (*str     != '\'') {out = decc_long_t<T>(str  ,strlen(str  ));}
  else if (*(str+1) == 'h' ) {out = hexc_long_t<T>(str+2,strlen(str+2));}
  else if (*(str+1) == 'b' ) {out = bitc_long_t<T>(str+2,strlen(str+2));}
  else if (*(str+1) == 'd' ) {out = decc_long_t<T>(str+2,strlen(str+2));}
  return(out);
}


template <class T>
T hexc_long_t(char *str,int len){
  T w,out;
  long w2;

  out = (T)0;
  w   = (T)1;
  for(int i=0;i<len;++i){
    w2 = *(str + i) - '0';
    if (w2 > 9)
      {
      if ((*(str + i)  >= 'a') && (*(str + i) <= 'f')) {w2 = *(str + i) - 'a' + 10;}
      if ((*(str + i)  >= 'A') && (*(str + i) <= 'F')) {w2 = *(str + i) - 'A' + 10;}
      }
    if ((w2 < 0) || (w2 > 16)) {return((T)-1);}
    out = out + w * (T)w2;
    w = w * (T)16;
  }
  return(out);
}


template <class T>
T bitc_long_t(char *str,int len){
  T w,out;
  long w2;

  out = (T)0;
  w   = (T)1;
  for(int i=0;i<len;++i){
    w2 = *(str + i) - '0';
    if ((w2 < 0) || (w2 > 1)) {return((T)-1);}
    out = out + w * (T)w2;
    w = w * (T)2;
  }
  return(out);
}


template <class T>
T decc_long_t(char *str,int len){
  T w,out;
  long w2;

  out = (T)0;
  w   = (T)1;
  for(int i=0;i<len;++i){
    w2 = *(str + i) - '0';
    if ((w2 < 0) || (w2 > 9)) {return((T)-1);}
    out = out + w * (T)w2;
    w = w * (T)10;
  }
  return(out);
}


template <>
bool convStrNumCheck<double>(char *str){
//  bool minus;
  double out;
  int i;

//  minus = false;
  i = 0;
  if (*str == '-'){
    for(i=1;;++i){
      if (*(str+i) != ' '){break;}
    }
//    minus = true;
  }

  out = doubleConv(str+i);
  if (out == (double)-1){return(false);}
  return(true);
}

template <>
double convStrNum<double>(char *str){
  bool minus;
  double out;
  int i;

  minus = false;
  i = 0;
  if (*str == '-'){
    for(i=1;;++i){
      if (*(str+i) != ' '){break;}
    }
    minus = true;
  }

  out = doubleConv(str+i);
  if (out == (double)-1){return((double)-1);}
  if (minus == true){out = out * (double)-1;}
  return(out);
}


double doubleConv(char *str){
  int i;
  int len;
  int mode;
  int w2;
  double w,out;

  out = (double)0;
  w   = (double)1;
  mode = 0;
  len = strlen(str);
  for(i=0;i<len;++i){
    if ((*(str + i) == '.') && (mode == 0)) {mode = 1; w = (double)0.1; continue;}
    w2 = *(str + i) - '0';
    if ((w2 < 0) || (w2 > 9)) {return((double)-1);}
    if (mode == 0){
      out = out * w;
      out = out + (double)w2;
      w = (double)10;
    }
    else{
      out = out + (double)w2 * w;
      w = w * (double)0.1;
    }
  }
  return(out);
}

