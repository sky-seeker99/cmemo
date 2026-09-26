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

#ifndef _ZTOOL_H_
#define _ZTOOL_H_

#include "zlist.h"

// # Define ---------------------
#define st_cpy(des,src) if (des != NULL){delete []des;} \
                        if (src == NULL){des = NULL;} \
                        else \
                          { \
                          des = new char[strlen(src)+1];     \
                          strcpy(des,src); \
                          }

#define st_ini(des,src) if (src == NULL){des = NULL;} \
                        else \
                          { \
                          des = new char[strlen(src)+1];     \
                          strcpy(des,src); \
                          }

#define st_del(str)     if (str != NULL){delete [] str;}

// # tool ----------------
void mem_cpy(char *des,char *src,int len);
int mem_cmp(char *des,char *src,int len);


// # Pseud Circuit ----------------
char bitc_inv(char in           );
char bitc_and(char in1,char in2 );
char bitc_or (char in1,char in2 );
char bitc_eor(char in1,char in2 );
void bitc_crcgen(/* crc,str,siki,crc_len,len */);

// # Toolkit ---------------
void   bitc_cpy      (char *des,char *src ,int len );
void   bit_dif_cpy   (char *des,char *src ,int des_len,int src_len);
void   bitc_ex       (char *str           ,int len );
void   bitc_bintohex (char *des,char *src ,int len );
void   bitc_hextobin (char *des,char *src ,int len );
unsigned long u_bitc_long(char *str       ,int len );
unsigned long u_hexc_long(char *str       ,int len );
unsigned long u_decc_long(char *str       ,int len );
long   bitc_long     (char *str           ,int len );
long   bitc_long2    (char *str           ,int len );
long   hexc_long     (char *str           ,int len );
long   decc_long     (char *str           ,int len );
void   long_bitc     (char *str,long  in  ,int len );
void   long_hexc     (char *str,long  in  ,int len );
void   long_decc     (char *str,long  in  ,int len );
//char   bitc_ranc     (void                         );
//void   bitc_ranl     (char * str          ,int len );
long   SujiConv      (char *str                    );
long   SujiConvEx    (char *str                    );
unsigned long uSujiConv  (char *str                );
unsigned long uSujiConvEx(char *str                );
long SujiConvSuper   (char *str                    );
double FreqConv      (char *str                    );
double FreqConvSuper (char *str                    );
int    SujiSize      (int  w_int                   );
void   sp_push       (char *str                    );
void   sp_push2      (char *str                    );
void   sp_push3      (char *str                    );
void   Atoa          (char *str          ,int   len);
void   atoA          (char *str          ,int   len);
char  *strcat_realloc(char *str          ,char *src);
int    Grep          (char *str          ,char *key);
int    GrepAa_same   (char *str          ,char *key);
void   komoji        (char *src,char *dst,int len);
//sChar *ValueConv     (char *in                     );
bool   sJisCheck     (char *str                    );


// 64ƒrƒbƒg•ÏŠ· ----- 
__int64   SujiConvSuper64(char *str                   );
__int64   SujiConvEx64  (char *str                    );
__int64   SujiConv64    (char *str                    );
__int64   bitc_long64   (char *str           ,int len );
__int64   hexc_long64   (char *str           ,int len );
__int64   decc_long64   (char *str           ,int len );





#endif // _ZTOOL_H_
