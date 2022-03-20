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

#ifndef _XKANJI_H_
#define _XKANJI_H_

#include <string.h>
#include <stdio.h>
#include "zcsvfile.h"
#include "zfile.h"

#define K_MAC 0
#define K_WIN 1
#define K_UNIX 2
#define K_SJIS_EUC 0
#define K_EUC_SJIS 1



// # gaiji_setting #####
class GaijiC {
  public:
    unsigned char first_str;
    unsigned char upper;
    unsigned char lower;
    unsigned char first_ch;
    unsigned char second_ch;
    bool          first_sign;
    bool          second_sign;
    GaijiC(int first_str,int upper,int lower,int first_ch,int second_ch);
};


 // # kanji class (kanji) ----------------
class kanji_T{
  public:
    MEM_FP      *sjis_fp;
    MEM_FP      *euc_fp;
    MemHeader_T *it;

    char ins_ret[3];
    unsigned char o1;
    unsigned char o2;
    unsigned char gj_o1;
    unsigned char gj_o2;


	bool sJisCheck(char *str);
        bool sJisHankakuCheck(char *str);
	bool eucCheck(char *str);
	bool eucHankakuCheck(char *str);
	void eucSjis(char *str);
	void sjisEuc(char *str);
	char out1(void){return((char)o1);}
	char out2(void){return((char)o2);}
	
	void sjis_euc_conv(FILE *in_fp,FILE *out_fp);
    void euc_sjis_conv(FILE *in_fp,FILE *out_fp);
    void return_code_setting(int kbn);
    void gaiji_setting(char *ini_file);
    void sjis_euc_set(CsvInf_T *csvi,int kbn);
    bool ext_sjis_euc(char *str,int kbn);
    void ext_print(MEM_FP *mem_fp,char *c);

    bool   NULL_flg;
    bool   d_flg;
    bool   a_flg;
    int    return_check(char *buff,int len);
    kanji_T();
    ~kanji_T();

  };
  
#endif // _XKANJI_H_  
  
