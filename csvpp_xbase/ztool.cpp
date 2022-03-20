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
//  TOOLKIT LIBRARY
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ztool.h"

// Tool --------------------------------------------------------------
void mem_cpy(char *des,char *src,int len)
  {
  int i;
  for(i=0;i<len;++i){*(des + i) = *(src + i);}
  }

int mem_cmp(char *des,char *src,int len)
  {
  int i;
  for(i=0;i<len;++i)
    {
    if (*(des + i) < *(src + i)) {return(-1);}
    if (*(des + i) > *(src + i)) {return(1);}
    }
  return(0);
  }

// # Pseud Circuit ----------------
char bitc_inv(char in){
  if (in == '1') {return('0');}
  if (in == '0') {return('1');}
  return(in);
}
char bitc_and(char in1,char in2){
  if ((in1 == '0') || (in2 == '0')) {return('0');}
  if ((in1 == '1') && (in2 == '1')) {return('1');}
  return('X');
}
char bitc_or(char in1,char in2){
  if ((in1 == '1') || (in2 == '1')) {return('1');}
  if ((in1 == '0') && (in2 == '0')) {return('0');}
  return('X');
}
char bitc_eor(char in1,char in2){
  if (((in1 == '1') || (in1 == '0')) && ((in2 == '1') || (in2 == '0'))){
    if (in1 == in2) {return('0');}
    return('1');
  }
return('X');
}
void bitc_crcgen(char crc[],char str[],char siki[],int crc_len,int len){
  char w;
  int  j,k;

  for(j=0;j<len;++j){
    w = bitc_eor(crc[crc_len-1],str[len-j-1]);
//  crc = crc << 1;
    for(k=crc_len-1;k>0;--k) {crc[k] = crc[k-1];}
    crc[0] = '0';
    for(k=0;k<crc_len;++k){
      crc[k] = bitc_eor(bitc_and(siki[k],w),crc[k]);
    }
  }
}

// # string change function ---------------------------
void bitc_cpy(char *des,char *src,int len){
  int i;
  for(i=0;i<len;++i){*(des + i) = *(src + i);}
}

void bit_dif_cpy(char *des,char *src,int  des_len,int src_len){
  int i;
  for(i=0;i<des_len;++i){
    if (src_len > i) {*(des + i) = *(src + i);}
    else             {*(des + i) = '0';       }
  }
}

void bitc_ex(char *str,int len){
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

void bitc_bintohex(char *des,char *src,int len){  // Binary Length
  int i,k,l;
  int data;

  k = 1;     // 4 Count
  data = 0;  // 1byte Data
  l = 0;     // des count
  for(i=0;i<len;++i){
    if (k == 1) {*(des + l) = 0x00;}
    if (*(des + l) == 0x00){
      if ((*(src + i) == '0') || (*(src + i) == '1')){
        if (*(src + i) == '1') {data  = data+k;}
      }
      else{
        *(des + l) = *(src + i);
      }
    }
    k = k * 2;
    if ((k == 16) || (i == len -1)){
      if (*(des + l) == 0x00){
        if (data > 9) {*(des + l) = 'A' + data -10;}
        else          {*(des + l) = '0' + data;}
      }
      k=1;
      ++l;
      data = 0;
    }
  }
}

void bitc_hextobin(char *des,char *src,int len){  // Binary Length
  int i,k,l;
  int data;

  data = -1; // 1byte Data
  l    =  0; // src Count
  k    =  1; // 4 Count
  for(i=0;i<len;++i){
    if ((i % 4) == 0){
      data = -1;
      if ((*(src + l) <= '9') && (*(src + l) >= '0')) {data = *(src+l) - '0';}
      if ((*(src + l) <= 'F') && (*(src + l) >= 'A')) {data = *(src+l) - 'A' + 10;}
      if ((*(src + l) <= 'f') && (*(src + l) >= 'a')) {data = *(src+l) - 'a' + 10;}
      ++l;
      k = 1;
    }

    if (data == -1) {*(des + i) = *(src + l - 1);}
    else {
      if ((data & k) == k) {*(des + i) = '1';}
      else                 {*(des + i) = '0';}
    }
    k = k * 2;
  }
}

long bitc_long(char *str,int len){
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

unsigned long u_bitc_long(char *str,int len){
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

long bitc_long2(char *str,int len){
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

long hexc_long(char *str,int len){
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

unsigned long u_hexc_long(char *str,int len){
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

long decc_long(char *str,int len){
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
  
unsigned long u_decc_long(char *str,int len){
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

void long_bitc(char *str,long in,int len){
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

void long_hexc(char *str,long in,int len){
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

void long_decc(char *str,long in,int len){
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

long SujiConvSuper(char *str){
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

long SujiConvEx(char *str){
  char *area;
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
  area = new char [len+1];
  strcpy(area,str);

  if ((*str     == '\'') && (len > 2)) {bitc_ex(area+2,len-2);}
  else                                 {bitc_ex(area,len);}
  out = SujiConv(area);
  delete [] area;
  return(out);
}

long SujiConv(char *str){
  if (*str     != '\'') {return(decc_long(str  ,strlen(str  )));}
  if (*(str+1) == 'h' ) {return(hexc_long(str+2,strlen(str+2)));}
  if (*(str+1) == 'b' ) {return(bitc_long(str+2,strlen(str+2)));}
  return(-1);
}

unsigned long uSujiConvEx(char *str){
  char *area;
  int len;
  unsigned long out;

  len = strlen(str);
  area = new char [len+1];
  strcpy(area,str);

  if ((*str     == '\'') && (len > 2)) {bitc_ex(area+2,len-2);}
  else                                 {bitc_ex(area,len);}
  out = uSujiConv(area);
  delete [] area;
  return(out);
}

unsigned long uSujiConv(char *str){
  if (*str     != '\'') {return(u_decc_long(str  ,strlen(str  )));}
  if (*(str+1) == 'h' ) {return(u_hexc_long(str+2,strlen(str+2)));}
  if (*(str+1) == 'b' ) {return(u_bitc_long(str+2,strlen(str+2)));}
  if (*(str+1) == 'd' ) {return(u_decc_long(str+2,strlen(str+2)));}
  return(0);
}


int SujiSize(int i){
  if (i >= 10000) {return(5);}
  if (i >= 1000 ) {return(4);}
  if (i >= 100  ) {return(3);}
  if (i >= 10   ) {return(2);}
  return(1);
}


double FreqConvSuper(char *str){
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

  out = FreqConv(str+i);
  if (out == (double)-1){return((double)-1);}
  if (minus == true){out = out * (double)-1;}
  return(out);
}


double FreqConv(char *str){
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

int Grep(char *str,char *srch_key){
  int i,j;
  int len;
  int srch_len;
  int ok;

  len      = strlen(str);
  srch_len = strlen(srch_key);

  for(i=0;i<len-srch_len+1;++i){
    ok = 1;
    for(j=0;j<srch_len;++j){
      if (*(str+i+j) != *(srch_key+j)){ok = 0; break;}
    }
    if (ok == 1){return(i);}
  }
  return(-1);
}


int GrepAa_same(char *str,char *srch_key){
  int i,j;
  int len;
  int srch_len;
  int ok;

  len      = strlen(str);
  srch_len = strlen(srch_key);

  char *w_str      = new char[len+1];
  char *w_srch_key = new char[srch_len+1];
  komoji(str,w_str,len);
  komoji(srch_key,w_srch_key,srch_len);

  for(i=0;i<len-srch_len+1;++i){
    ok = 1;
    for(j=0;j<srch_len;++j){
      if (*(w_str+i+j) != *(w_srch_key+j)){ok = 0; break;}
    }
    if (ok == 1){
      delete [] w_str;
      delete [] w_srch_key;
      return(i);
    }
  }
  delete [] w_str;
  delete [] w_srch_key;
  return(-1);
}


// # sjis check #####
bool sJisCheck(char *str){
  unsigned char w1;
  unsigned char w2;

  bool shift_flg = false;
  w1 = (unsigned char)*(str);
  w2 = (unsigned char)*(str+1);
  if (((w1 >= 0x81) && (w1 <= 0x9f)) || ((w1 >= 0xe0) && (w1 <= 0xfe))){
    if (((w2 >= 0x40) && (w2 <= 0x7e)) || ((w2 >= 0x80) && (w2 <= 0xfc))){
      shift_flg = true;
    }
  }
  return(shift_flg);
}


void komoji(char *src,char *dst,int len){
  int i;
  bool flg;

  int kanji=0;

  for(i=0;i<len;i++){
    if (kanji == 0) {
      if (sJisCheck(src+i)){
        kanji = 1;
      }
    }

    flg = false;
    if (kanji == 0) {
      if ((*(src+i) >= 'A') && (*(src+i) <= 'Z')){
        flg=true;
      }
    }
    if (flg == true){
      *(dst+i) = *(src+i) + 'a' - 'A';
    } else {
      *(dst+i) = *(src+i);
    }

    if (kanji==1) {
      kanji=2;
    }
    else if (kanji == 2) {
      kanji = 0;
    }

  }
  *(dst+i) = 0x00;


//printf("[%s] -> [%s]\n",src,dst);

}

/*
// # ransu function -----------------------------------
char bitc_ranc(){
 unsigned int ran_data;
 ran_data = (rand()>>RANL_MAX)&0x1;
 if(ran_data == 0){return('0');}
 return('1');
}

void bitc_ranl(char *str,int len){
  int i;

  for(i=0;i<len;++i){
    *(str + i) = bitc_ranc();
  }
}


//---------------------------
// atai to binary data
// 8'b10  -> 00000010(01000000)
// 8'h10  -> 00010000(00001000)
// 8'10   -> 00001010(01010000)
//---------------------------
sChar *ValueConv(char *in){
  sChar *rtn_cd;
  sChar *w;
  int    size;
  int    i,len;
  long   w_long;

  size = -1;
  len  = strlen(in);
  for(i=0;i<len;++i){
    if (*(in+i) == '\''){
      *(in+i) = 0x00;
      size = SujiConvEx(in);
      *(in+i) = '\'';
      break;
    }
  }
  if (size < 1){return((sChar *)NULL);}    // csvpp 1.27

  if (*(in+i+1) == 'h'){
    bitc_ex(in+i+2,strlen(in+i+2));
    rtn_cd = new sChar("");
    w      = new sChar("");
    rtn_cd->beta(size,'0');
    w     ->beta(strlen(in+i+2)*4,'0');
    bitc_hextobin(w->c_str(),in+i+2,strlen(in+i+2)*4);
    bit_dif_cpy(rtn_cd->c_str(),w->c_str(),size,strlen(in+i+2)*4);
    bitc_ex(in+i+2,strlen(in+i+2));
    delete w;
    return(rtn_cd);
  }

  if (*(in+i+1) == 'b'){
    bitc_ex(in+i+2,strlen(in+i+2));
    rtn_cd = new sChar("");
    rtn_cd->beta(size,'0');
    bit_dif_cpy(rtn_cd->c_str(),in+i+2,size,strlen(in+i+2));
    bitc_ex(in+i+2,strlen(in+i+2));
    return(rtn_cd);
  }

  w_long = SujiConvEx(in+i+1);
  if (w_long == -1){return((sChar *)NULL);}

  rtn_cd = new sChar("");
  rtn_cd->beta(size,'0');
  long_bitc(rtn_cd->c_str(),w_long,size);
  return(rtn_cd);
}
*/

// # String Control  -------------------------------
void sp_push(char *str){
  int i;
  int len;
  bool flg,hit;
  char *w;

  len = strlen(str);
  if (len == 0){return;}
  flg = false;
  hit = false;
  w = str + len-1;

  for (i=0;i<len;++i){
    if (i>0){w--;}

    if (*w == 0x0a) {flg = true; continue;}
    if (*w != 0x20){
      w++;
      if (flg) {*w++ = 0x0a;}
      *w = 0x00;
      hit = true;
      break;
    }
  }

  if (hit == false){
    w = str;
    if (flg) {*w++ = 0x0a;}
    *w = 0x00;
  }
}

void sp_push2(char *str){
  char *w;
  w = str;

  for (;;w++){
    if (*w == 0x00){break;}
    if (*w != 0x20){strcpy(str,w); break;}
  }
}
  
void sp_push3(char *str){
  char *w_str,*a_str;
  for(w_str=str,a_str=str;;w_str++)
    {
    if (*w_str == ' '){continue;}
    if (*w_str == 0x00){break;}
    *a_str++ = *w_str;
    }
  *a_str= 0x00;
}

void Atoa(char *str,int len){
  int i;
  for(i=0;i<len;++i) {if ((*(str + i) >= 'A') && (*(str + i) <= 'Z')) {*(str + i) = *(str + i) - 'A' + 'a';}}
}

void atoA(char *str,int len){
  int i;
  for(i=0;i<len;++i) {if ((*(str + i) >= 'a') && (*(str + i) <= 'z')) {*(str + i) = *(str + i) - 'a' + 'A';}}
}

char *strcat_realloc(char *str,char *src){
  int len;
  char *des;

  if (str == MM_NULL) {len = 0;}
  else                {len = strlen(str);}
  des = new char [len+strlen(src)+1];

  if (str == MM_NULL) {strcpy(des,src);}
  else                {sprintf(des,"%s%s",str,src); delete [] str;}
  return(des);
}

// 64 ƒrƒbƒg•ÏŠ·  ---------------------------------------------
__int64 SujiConvSuper64(char *str){
  bool minus;
  __int64 out;
  int i;

  minus = false;
  i = 0;
  if (*str == '-'){
    for(i=1;;++i){
      if (*(str+i) != ' '){break;}
    }
    minus = true;
  }

  out = SujiConvEx64(str+i);
  if (out == (__int64)-1){return((__int64)-1);}
  if (minus == true){out = out * (__int64)-1;}
  return(out);
}


__int64 SujiConvEx64(char *str){
  char *area;
  int len;
  __int64 out;
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
  out = SujiConv64(area);
  delete [] area;
  return(out);
}

__int64 SujiConv64(char *str){
  if (*str     != '\'') {return(decc_long64(str  ,strlen(str  )));}
  if (*(str+1) == 'h' ) {return(hexc_long64(str+2,strlen(str+2)));}
  if (*(str+1) == 'b' ) {return(bitc_long64(str+2,strlen(str+2)));}
  if (*(str+1) == 'd' ) {return(decc_long64(str+2,strlen(str+2)));}
  return((__int64)-1);
}


__int64 hexc_long64(char *str,int len){
  int i;
  __int64 w,out;
  long w2;

  out = (__int64)0;
  w   = (__int64)1;
  for(i=0;i<len;++i){
    w2 = *(str + i) - '0';
    if (w2 > 9){
      if ((*(str + i)  >= 'a') && (*(str + i) <= 'f')) {w2 = *(str + i) - 'a' + 10;}
      if ((*(str + i)  >= 'A') && (*(str + i) <= 'F')) {w2 = *(str + i) - 'A' + 10;}
    }
    if ((w2 < 0) || (w2 > 16)) {return((__int64)-1);}
    out = out + w * (__int64)w2;
    w = w * (__int64)16;


//  printf("out=%I64x w=%I64x",out,w);


  }
  return(out);
}


__int64 bitc_long64(char *str,int len){
  int i;
  __int64 w,out;
  long w2;

  out = (__int64)0;
  w   = (__int64)1;
  for(i=0;i<len;++i){
    w2 = *(str + i) - '0';
    if ((w2 < 0) || (w2 > 1)) {return((__int64)-1);}
    out = out + w * (__int64)w2;
    w = w * (__int64)2;
  }
  return(out);
}

__int64 decc_long64(char *str,int len){
  int i;
  __int64 w,out;
  long w2;

  out = (__int64)0;
  w   = (__int64)1;
  for(i=0;i<len;++i){
    w2 = *(str + i) - '0';
    if ((w2 < 0) || (w2 > 9)) {return((__int64)-1);}
    out = out + w * (__int64)w2;
    w = w * (__int64)10;
  }
  return(out);
}


