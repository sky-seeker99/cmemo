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
// ISO2022 Converter
//

#ifndef _XISO2022_H_
#define _XISO2022_H_

#include "zschar.h"

// interface class -----------------
class Iso2022_T{
  private:
  sChar *en_sep;
  sChar *dis_sep;
  unsigned int en_sep_len;
  unsigned int dis_sep_len;
  unsigned char *string;
  unsigned char c1;
  unsigned char c2;

  public:
  Iso2022_T(char *p_en_sep,char *p_dis_sep) {
    dis_sep = new sChar(p_dis_sep);
    en_sep  = new sChar(p_en_sep);
    dis_sep_len = strlen(p_dis_sep);
    en_sep_len  = strlen(p_en_sep);
    string      = NULL;
    }
    
  Iso2022_T(){ // for encord
    char p_en_sep [5];
    char p_dis_sep[5];
    
    p_en_sep[0] = 0x1b;
    p_en_sep[1] = 0x24;
    p_en_sep[2] = 0x42;
    p_en_sep[3] = 0x00;
    p_dis_sep[0] = 0x1b;
    p_dis_sep[1] = 0x28;
    p_dis_sep[2] = 0x42;
    p_dis_sep[3] = 0x00;
    dis_sep_len = strlen(p_dis_sep);
    en_sep_len  = strlen(p_en_sep);

    dis_sep = new sChar(p_dis_sep);
    en_sep  = new sChar(p_en_sep);
    string  = NULL;
    }  

  Iso2022_T(int dmy){ // for decord
    char p_en_sep [5];
    char p_dis_sep[5];
    
    p_en_sep[0] = 0x1b;
    p_en_sep[1] = 0x24;
    p_en_sep[2] = 0x01;
    p_en_sep[3] = 0x00;
    p_dis_sep[0] = 0x1b;
    p_dis_sep[1] = 0x28;
    p_dis_sep[2] = 0x01;
    p_dis_sep[3] = 0x00;
    dis_sep_len = strlen(p_dis_sep);
    en_sep_len  = strlen(p_en_sep);

    dis_sep = new sChar(p_dis_sep);
    en_sep  = new sChar(p_en_sep);
    string  = NULL;
    }  

  ~Iso2022_T(){  
    delete en_sep;
    delete dis_sep;
    if (string != NULL){delete [] string;}
    }

  public:  
  char *decord   (unsigned char *str);
  char *encord   (unsigned char *str);
  void sjis_jis  (unsigned char p_c1,unsigned p_c2);
  void jis_sjis  (unsigned char p_c1,unsigned p_c2);
  bool sep_check (unsigned char *str,bool mode);
  bool sjis_check(unsigned char *str);
};

#endif // _XISO2022_H_


