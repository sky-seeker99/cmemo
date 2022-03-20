/*

 * Copyright (c) 2010 Shigeru Kasuya (sky_seeker99@users.sourceforge.jp)
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
#include <string.h> 
#include <stdio.h> 
#include "xarea.h"
#include "zschar.h"



// # basic ------------------------
sChar::sChar(char *src){
  if (src == NULL){area = new VariableArea_C(10); area->set(0x00);}
  else            {area = new VariableArea_C(src);}
}
sChar::sChar(char *src,int len){
  if (src == NULL){area = new VariableArea_C(10);}
  else            {area->set_n(src,len);}
  area->set(0x00);
}
sChar::sChar(sChar *src){
  if      (src          == NULL){area = new VariableArea_C(10); area->set(0x00);}
  else if (src->c_str() == NULL){area = new VariableArea_C(10); area->set(0x00);}
  else                          {area = new VariableArea_C(src->c_str());}
}
sChar::~sChar(){
  delete area;
}

void sChar::set(sChar *src){
  if (src          == NULL){area->set(0,0x00); return;}
  if (src->c_str() == NULL){area->set(0,0x00); return;}
  area->set_string(src->c_str());
}
void sChar::push(void){
  VariableArea_C *sss = new VariableArea_C(100);  // 2013.8.17

  // - After Push ---
  bool flg = false;
  int i;
  for (i=area->len()-1;i>=0;i--){
    char w = area->getChar(i);
    if      (w == 0x0a){flg = true; continue;}
    else if (w != 0x20){break;}
  }
  if (flg){i++; area->set(i,0x0a);}
  i++;
  area->set(i,0x00);
  // - Before Push ---
  for (int i=0;;i++){
    char w = area->getChar(i);
    if (w == 0x00){break;}
    if (w != 0x20){
      sss->set_string(area->cc_str()+i);
      area->set_string(sss->cc_str());
      break;
    }
  }

  delete sss; // 2013.8.17
}
void sChar::beta(int size,char c){
  area->init();
  for(int i=0;i<size;++i) {area->set(c);}
  area->set(0x00);
}


// # to int ------------------------
long sCharNum::bitc_long(char *str,int len){
  int i;
  long w,out,w2;

  out = 0;
  w   = 1;
  for(i=0;i<len;++i){
    w2 = *(str + i) - '0';
    if ((w2 < 0) || (w2 > 1)) {return(-1);}
    out = out + w * w2;
    w = w * 2;
  }
  return(out);
}

void sCharNum::bitc_ex(char *str,int len){
  int i,j,k;
  char sv;

  i = len - (len /2) * 2;
  i = i + len/2;

  for(j=0;j<i;++j){
    k = len - j -1;
    sv = *(str + j);
    *(str + j) = *(str + k);
    *(str + k) = sv;
  }
}


unsigned long sCharNum::u_bitc_long(char *str,int len){
  int i;
  unsigned long w,out,w2;

  out = 0;
  w   = 1;
  for(i=0;i<len;++i){
    w2 = *(str + i) - '0';
    if ((w2 < 0) || (w2 > 1)) {return(-1);}
    out = out + w * w2;
    w = w * 2;
  }
  return(out);
}

long sCharNum::bitc_long2(char *str,int len){
  int i;
  long w,out,w2;

  out = 0;
  w   = 1;
  for(i=0;i<len;++i){
    w2 = *(str + i) - '0';
    if ((w2 < 0) || (w2 > 1)) {return(0);}
    out = out + w * w2;
    w = w * 2;
  }
  return(out);
}

long sCharNum::hexc_long(char *str,int len){
  int i;
  long w,out,w2;

  out = 0;
  w   = 1;
  for(i=0;i<len;++i){
    w2 = *(str + i) - '0';
    if (w2 > 9){
      if ((*(str + i)  >= 'a') && (*(str + i) <= 'f')) {w2 = *(str + i) - 'a' + 10;}
      if ((*(str + i)  >= 'A') && (*(str + i) <= 'F')) {w2 = *(str + i) - 'A' + 10;}
    }
    if ((w2 < 0) || (w2 > 16)) {return(-1);}
    out = out + w * w2;
    w = w * 16;
  }
  return(out);
}

unsigned long sCharNum::u_hexc_long(char *str,int len){
  int i;
  unsigned long w,out,w2;

  out = 0;
  w   = 1;
  for(i=0;i<len;++i){
    w2 = *(str + i) - '0';
    if (w2 > 9){
      if ((*(str + i)  >= 'a') && (*(str + i) <= 'f')) {w2 = *(str + i) - 'a' + 10;}
      if ((*(str + i)  >= 'A') && (*(str + i) <= 'F')) {w2 = *(str + i) - 'A' + 10;}
    }
    if ((w2 < 0) || (w2 > 16)) {return(-1);}
    out = out + w * w2;
    w = w * 16;
  }
  return(out);
}

long sCharNum::decc_long(char *str,int len){
  int i;
  long w,out,w2;

  out = 0;
  w   = 1;
  for(i=0;i<len;++i){
    w2 = *(str + i) - '0';
    if ((w2 < 0) || (w2 > 9)) {return(-1);}
    out = out + w * w2;
    w = w * 10;
  }
  return(out);
}

unsigned long sCharNum::u_decc_long(char *str,int len){
  int i;
  unsigned long w,out,w2;

  out = 0;
  w   = 1;
  for(i=0;i<len;++i){
    w2 = *(str + i) - '0';
    if ((w2 < 0) || (w2 > 9)) {return(-1);}
    out = out + w * w2;
    w = w * 10;
  }
  return(out);
}

void sCharNum::long_bitc(char *str,long in,int len){
  int i;
  unsigned long a,b;

  if ((len > 10000) || (len < 1)) {printf("long_bitc error \n"); return;}
  a = in;
  for (i=0;i<len;++i){
    if (a == 0) {*(str + i) = '0'; continue;}
    b = a - (a/2)*2;
    *(str + i) = '0' + b;
    a = a / 2;
  }
}

void sCharNum::long_hexc(char *str,long in,int len){
  int i;
  long a,b;

  if ((len > 8) || (len < 1)) {printf("long_hexc error \n"); return;}
  a = in;
  for (i=0;i<len;++i){
    if (a == 0) {*(str + i) = '0'; continue;}
    b = a - (a/16)*16;
    if (b < 10) {*(str + i) = '0' + b;}
    else        {*(str + i) = 'A' + b - 10;}
    a = a / 16;
  }
}

void sCharNum::long_decc(char *str,long in,int len){
  int i;
  long a,b;

  if ((len > 10) || (len < 1)) {printf("long_decc error \n"); return;}
  a = in;
  for (i=0;i<len;++i){
    if (a == 0) {*(str + i) = '0'; continue;}
    b = a - (a/10)*10;
    *(str + i) = '0' + b;
    a = a / 10;
  }
}

long sCharNum::SujiConvSuper(char *str){
  bool minus;
  long out;
  int i;

  minus = false;
  i = 0;
  if (*str == '-'){
    for(i=1;;++i){
      if (*(str+i) != ' '){break;}
    }
    minus = true;
  }

  out = SujiConvEx(str+i);
  if (out == -1){return(-1);}
  if (minus == true){out = out * -1;}
  return(out);
}

long sCharNum::SujiConvEx(char *str){
  int len;
  long out;
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
  sv_str->set_string(str);
  char *area = sv_str->cc_str();
  if ((*str     == '\'') && (len > 2)) {bitc_ex(area+2,len-2);}
  else                                 {bitc_ex(area,len);}
  out = SujiConv(area);
  return(out);
}

long sCharNum::SujiConv(char *str){
  if (*str     != '\'') {return(decc_long(str  ,strlen(str  )));}
  if (*(str+1) == 'h' ) {return(hexc_long(str+2,strlen(str+2)));}
  if (*(str+1) == 'b' ) {return(bitc_long(str+2,strlen(str+2)));}
  if (*(str+1) == 'd' ) {return(decc_long(str+2,strlen(str+2)));}
  return(-1);
}

unsigned long sCharNum::uSujiConvEx(char *str){
  int len;
  unsigned long out;

  len = strlen(str);
  for(int i=0;i<len;++i){
	if ((*(str+i) == '0') && (*(str+i+1) == 'x')){
	  *(str+i) = '\'';
	  *(str+i+1) = 'h';
	}
	if (*(str+i) == '\''){
	  str = str+i;
	  len = strlen(str);
	}
  }
  sv_str->set_string(str);
  char *area = sv_str->cc_str();

  if ((*str     == '\'') && (len > 2)) {bitc_ex(area+2,len-2);}
  else                                 {bitc_ex(area,len);}
  out = uSujiConv(area);
  return(out);
}

unsigned long sCharNum::uSujiConv(char *str){
  if (*str     != '\'') {return(u_decc_long(str  ,strlen(str  )));}
  if (*(str+1) == 'h' ) {return(u_hexc_long(str+2,strlen(str+2)));}
  if (*(str+1) == 'b' ) {return(u_bitc_long(str+2,strlen(str+2)));}
  if (*(str+1) == 'd' ) {return(u_decc_long(str+2,strlen(str+2)));}
  return(0);
}


// # extend ------------------------
void sCharEX::set_index(char *src,unsigned int ptr,unsigned int len,bool ex){
  if (src == NULL){return;}
  if (len < 1){return;}
  bool flag=false;
  for(int i=area->len();i<(int)(ptr+len);i++){
    flag=true;
    area->set(i,' ');
  }
  if (flag){
    area->set(ptr+len,0x00);
  }

  int src_len = strlen(src);
  int chg_ptr = 0;
  for(int i=0;i<(int)len;++i){
    if (ex == false){chg_ptr = i+ptr;}
    else            {chg_ptr = ptr+len-i-1;}
    if (src_len <= i){area->set(chg_ptr,' ');}
    else             {area->set(chg_ptr,*(src+i));}
  }
}

void sCharEX::DblIns(char *p_str){
  int  add    = 0;
  bool insFlg = false;
  area->set_string(p_str);
  int len = area->len();

  for(int i=0;i<len;i++){
    char w_str = area->getChar(i);
    if (w_str == 0x00){break;}
    if (w_str == ',' ){insFlg = true;        continue;}
    if (w_str == 0x0a){insFlg = true;        continue;}
    if (w_str == 0x0b){insFlg = true;        continue;}
    if (w_str == 0x0d){insFlg = true;        continue;}
    if (w_str == '"' ){insFlg = true; add++; continue;}
  }
  if (insFlg){add += 2;}

  sv_str->init();
  if (insFlg){sv_str->set('"');}
  for(int i=0;i<len;i++){
    char w_str = area->getChar(i);
    sv_str->set(w_str);
    if (w_str == '"'){sv_str->set(w_str);}
  }
  if (insFlg){sv_str->set('"');}
  sv_str->set(0x00);
  area->set_string(sv_str->cc_str());
}


// # grep extend ------------------------
void sCharGrep::Atoa(char *src,char *dst,int len){
  int i;
  bool flag=false;
  sjis->init();
  for(i=0;i<len;i++){
    flag = sjis->check(src,i);
    if ((flag == false) && ((*(src+i) >= 'A') && (*(src+i) <= 'Z'))){*(dst+i) = *(src+i) + 'a' - 'A';}
    else                                                            {*(dst+i) = *(src+i);            }
  }
  *(dst+i) = 0x00;
}

bool sCharGrep::mem_cmp(char *cmp1,char *cmp2,int offset,int len){
  bool flag = true;
  bool f1,f2;
  sjis->init();
  sjis2->init();

  for(int i=0;i<offset;++i){
    sjis2->check(cmp2,i);
  }

  for(int i=0;i<len;++i){
    f1 = sjis->check(cmp1,i);
    f2 = sjis2->check(cmp2,i+offset);
    if (f1 != f2){flag = false; break;}
    if (*(cmp1+i) != *(cmp2+i+offset)){flag = false; break;}
  }
  return(flag);
}

void sCharGrep::setGrep(char *p_srch_key,bool sameAaFlag){
  srch_str->set_string(area->cc_str());
  srch_key->set_string(p_srch_key);
  if (sameAaFlag){
    Atoa(area->cc_str(),srch_str->cc_str(),area->len());
    Atoa(p_srch_key    ,srch_key->cc_str(),strlen(p_srch_key));
  }
}

int sCharGrep::grepCore(int offset){
  int len      = srch_str->len();
  int srch_len = srch_key->len();
  for(int i=offset;i<len-srch_len+1;++i){
    if (mem_cmp(srch_key->cc_str(),srch_str->cc_str(),i,srch_len)){return(i);}
  }
  return(-1);
}

void sCharGrep::replaceCore(char *srch_key,char *rep_key,int hit_ptr){
  if (hit_ptr == -1){return;}
  int len     = area->len();
  int rep_len = strlen(rep_key);
  int key_len = strlen(srch_key);
  sv_str->init();
  for(int i=0              ;i<hit_ptr;++i){sv_str->set(area->getChar(i));}  // before string copy
  for(int i=0              ;i<rep_len;++i){sv_str->set(*(rep_key+i));    }  // key replace
  for(int i=hit_ptr+key_len;i<len    ;++i){sv_str->set(area->getChar(i));}  // after string copy
  sv_str->set(0x00);
  area->set_string(sv_str->cc_str());
}

int sCharGrep::replace_all(char *srch_key,char *rep_key){
  setGrep(srch_key,false);
  int ptr = 0;
  int rep_len = strlen(rep_key);
  int cnt;
  for(cnt=0;;++cnt){
    if ((ptr = grepCore(ptr)) == -1){break;}
    replaceCore(srch_key,rep_key,ptr);
    srch_str->set_string(area->cc_str());
    ptr += rep_len;
  }
  return(cnt);
}

int sCharGrep::replace_allAa_same(char *srch_key,char *rep_key){
  setGrep(srch_key,true);
  int ptr = 0;
  int rep_len = strlen(rep_key);
  int cnt;
  for(cnt=0;;++cnt){
    if ((ptr = grepCore(ptr)) == -1){break;}
    replaceCore(srch_key,rep_key,ptr);
    srch_str->set_string(area->cc_str());
    Atoa(area->cc_str(),srch_str->cc_str(),area->len());
    ptr += rep_len;
  }
  return(cnt);
}


