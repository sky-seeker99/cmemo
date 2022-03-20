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
