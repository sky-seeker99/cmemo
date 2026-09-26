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
  
