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

#include "xiso2022.h"

// -------
// decord
// -------
char *Iso2022_T::decord(unsigned char *str)
  {
  unsigned int i,j,ptr;
  unsigned int len;
  bool mode;

  if (string != NULL){delete [] string;}
  len = strlen((char *)str);
  string = new char[len+1];

  mode    = false;
  ptr     = 0;
  *string = 0x00;
  for(i=0;i<len;++i)
    {
    if (sep_check(str+i,mode) == true)
      {
      if (mode == true){i=i+dis_sep_len;  mode = false;}
      else             {i=i+en_sep_len; mode = true;}
      }
    if (mode == true)
      {
      jis_sjis(*(str+i),*(str+i+1));
      *(string+ptr) = c1;   ++ptr; ++i;
      *(string+ptr) = c2;   ++ptr;
      *(string+ptr) = 0x00;
      }
    else
      {
      *(string+ptr) = *(str + i); ++ptr;
      *(string+ptr) = 0x00;
      }
    }
  return(string);
  }

// -------
// encode
// -------
char *Iso2022_T::encord(unsigned char *str)
  {
  unsigned int i,j,ptr;
  unsigned int len;
  bool mode;
  bool sv_mode;

  if (string != NULL){delete [] string;}
  len = strlen((char *)str);
//  string = new char[len/3*en_sep_len+len/3*dis_sep_len+len+1];
  string = new char[(len/3+1)*en_sep_len+(len/3+1)*dis_sep_len+len+1];

  *string = 0x00;
  sv_mode = false;
  ptr     = 0;
  for(i=0;i<len;++i)
    {
    mode = sjis_check(str+i);
    if (sv_mode != mode)
      {
      if (mode == true) // jis mode
        {
        for(j=0;j<en_sep_len;++j) {*(string+ptr) = (unsigned char)*(en_sep->c_str()+j);  ++ptr; *(string+ptr) = 0x00;}
        }
      else              // ascii mode
        {
        for(j=0;j<dis_sep_len;++j){*(string+ptr) = (unsigned char)*(dis_sep->c_str()+j); ++ptr; *(string+ptr) = 0x00;}
        }
      sv_mode = mode;
      }
    if (mode == true)
      {
      sjis_jis(*(str+i),*(str+i+1));
      *(string+ptr) = c1;   ++ptr; ++i;
      *(string+ptr) = c2;   ++ptr;
      *(string+ptr) = 0x00;
      }
    else
      {
      *(string+ptr) = *(str+i); ++ptr;
      *(string+ptr) = 0x00;
      }
    }
  if (mode == true)
    {
    for(j=0;j<dis_sep_len;++j){*(string+ptr) = (unsigned char)*(dis_sep->c_str()+j); ++ptr; *(string+ptr) = 0x00;}
    }
  return(string);
  }

// ----------
// sjis->jis
// ----------
void Iso2022_T::sjis_jis(unsigned char p_c1,unsigned p_c2)
  {
  c1 = p_c1;
  c2 = p_c2;
  if (c1 >=0xe0){c1 =c1 -0x40;}
  if (c2 >=0x80){c2 =c2 -1;}
  if (c2 >=0x9e)
    {
    c1 =(c1 -0x70)*2;
    c2 =c2 -0x7d;
    }
  else
    {
    c1 =((c1 -0x70)*2)-1;
    c2 =c2 -0x1f;
    }
  }
  
// ----------
// jis->sjis
// ----------
void Iso2022_T::jis_sjis(unsigned char p_c1,unsigned p_c2)
  {
  c1 = p_c1;
  c2 = p_c2;

  if (c1 % 2)
    {
    c1 =((c1 +1)/2)+0x70;
    c2 =c2 +0x1f;
    }
  else
    {
    c1 =(c1 /2)+0x70;
    c2 =c2 +0x7d;
    }
  if (c1 >=0xa0){c1 =c1 +0x40;}
  if (c2 >=0x7f){c2 =c2 +1;}
  }

// ---------------
// separate check
// ---------------
bool Iso2022_T::sep_check(unsigned char *str,bool mode)
  {
  unsigned int len;
  unsigned int i;
  unsigned char *sep;
  bool ret;
  if (mode == false){len = en_sep_len;  sep = (unsigned char *)en_sep->c_str();}
  else              {len = dis_sep_len; sep = (unsigned char *)dis_sep->c_str();}

  ret = true;
  for(i=0;i<len;++i)
    {
    if (*(str+i) == 0x00){ret = false; break;}
    if (*(sep+i) == 0x01){continue;}
    if (*(str+i) != *(sep+i)){ret = false; break;}
    }

  return(ret);
  }

// ---------------
// sjis check
// ---------------
bool Iso2022_T::sjis_check(unsigned char *str){
  unsigned char w1;
  unsigned char w2;

  w1 = *str;
  w2 = *(str+1);
  if (((w1 >= 0x81) && (w1 <= 0x9f))
   || ((w1 >= 0xe0) && (w1 <= 0xef)))
    {
    if (((w2 >= 0x40) && (w2 <= 0x7e))
     || ((w2 >= 0x80) && (w2 <= 0xfc)))
      {
      return(true);
      }
    }
  return(false);
  }
