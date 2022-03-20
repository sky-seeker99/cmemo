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
// BASE64 Converter
//

#include "xbase64.h"

// -------
// decord
// -------
char *Base64_T::decode(unsigned char *str)
  {
  unsigned int i,j,ptr;
  unsigned int len;
  bool mode;
  int sv_ptr;
  char sv_char;
  int sep_len;
  char *w_buff;

  if (string != NULL){delete [] string;}
  len = strlen((char *)str);
  string = new char[len*2+1];

  mode    = false;
  ptr     = 0;
  *string = 0x00;
  for(i=0;i<len;)
    {
    if ((sep_len = sep_check(str+i,mode)) > 0)
      {
      if (mode == true)
        {
        sv_char    = *(str+i);
        *(str+i) = 0x00;
        w_buff = base64_sjis(str+sv_ptr);

        strcat(string,w_buff);
        ptr += strlen(w_buff);
        *(str+i) = sv_char;
        i=i+sep_len;
        mode = false;
        }
      else
        {
        i=i+sep_len;
        sv_ptr = i;
        mode = true;
        }
      continue;
      }

    if (mode == false)
      {
      *(string+ptr) = *(str + i); ++ptr; ++i;
      *(string+ptr) = 0x00;
      }
    else
      {
      ++i;
      }
    }
  if (mode == true)
    {
    strcat(string,base64_sjis(str + sv_ptr));
    }

  return(string);
  }


// ---------------------------
// base64 -> sjis for attach
// ---------------------------
char *Base64_T::base64_sjis_attach(char *buff)
  {
  int   c1,c2,c3,c4,i,len,buff_len;
  unsigned char    *p,*q,a[5];

  for(p=buff;*p!='\0';p++) if(*p == '=') *p = '\0';
  if (out_buff != NULL){delete [] out_buff;}
  buff_len = strlen(buff);
  out_buff = new char[buff_len+1];

  p = buff;
  q = out_buff;

  while(*p != '\0')
    {
    a[0] = a[1] = a[2] = a[3] = 0;
    if(p[0] != '\0') a[0] = (unsigned char)(strchr(b64,p[0]) - b64);
    if(p[1] != '\0') a[1] = (unsigned char)(strchr(b64,p[1]) - b64);
    if(p[2] != '\0') a[2] = (unsigned char)(strchr(b64,p[2]) - b64);
    if(p[3] != '\0') a[3] = (unsigned char)(strchr(b64,p[3]) - b64);
    q[0] = (unsigned char)((a[0] << 2) | (a[1] >> 4));
    q[1] = (unsigned char)((a[1] << 4) | (a[2] >> 2));
    q[2] = (unsigned char)((a[2] << 6) |  a[3]);
    p = p + 4;
    q = q + 3;
    }
  *q = '\0';
  return(out_buff);
  }


// ----------------
// base64 -> sjis
// ----------------
char *Base64_T::base64_sjis(char *buff)
  {
  int   c1,c2,c3,c4,i,len,buff_len;
  unsigned char    *p,*q,a[5];

  for(p=buff;*p!='\0';p++) if(*p == '=') *p = '\0';
  if (out_buff != NULL){delete [] out_buff;}
  buff_len = strlen(buff);
  out_buff = new char[buff_len+1];

  p = buff;
  q = out_buff;

  while(*p != '\0')
    {
    a[0] = a[1] = a[2] = a[3] = 0;
    if(p[0] != '\0') a[0] = (unsigned char)(strchr(b64,p[0]) - b64);
    if(p[1] != '\0') a[1] = (unsigned char)(strchr(b64,p[1]) - b64);
    if(p[2] != '\0') a[2] = (unsigned char)(strchr(b64,p[2]) - b64);
    if(p[3] != '\0') a[3] = (unsigned char)(strchr(b64,p[3]) - b64);
    q[0] = (unsigned char)((a[0] << 2) | (a[1] >> 4));
    q[1] = (unsigned char)((a[1] << 4) | (a[2] >> 2));
    q[2] = (unsigned char)((a[2] << 6) |  a[3]);
    p = p + 4;
    q = q + 3;
    }
  *q = '\0';

//   for(i=0;i<(int)strlen(out_buff);++i){printf("[%x]",*(out_buff+i));}
//   printf("\n");


  len = strlen(out_buff) / 2 * 2;
  bool kanji;
  kanji = false;
  char *kkk = out_buff;
  for(i=0;i<len;)
    {
    if (out_buff[i]       == 0x1b){
      if (out_buff[i+1]   == 0x24){
//        if (out_buff[i+2] == 0x42){
          kanji = true;
          i = i+3;
          continue;
//          }
        }
      }

    if (out_buff[i]       == 0x1b){
      if (out_buff[i+1]   == 0x28){
//        if (out_buff[i+2] == 0x42){
          kanji = false;
          i = i+3;
          continue;
//          }
        }
      }

    if (kanji)
      {
      jis_sjis(*(out_buff+i),*(out_buff+i+1));
      *kkk++ = g_c1;
      *kkk++ = g_c2;
      i = i + 2;
      }
    else
      {
      *kkk++ = out_buff[i];
      i++;
      }
    }

  *kkk = 0x00;

  return(out_buff);
  }

// ----------
// jis->sjis
// ----------
void Base64_T::jis_sjis(unsigned char p_c1,unsigned p_c2)
  {
  g_c1 = p_c1;
  g_c2 = p_c2;

  if (g_c1 % 2)
    {
    g_c1 =((g_c1 +1)/2)+0x70;
    g_c2 =g_c2 +0x1f;
    }
  else
    {
    g_c1 =(g_c1 /2)+0x70;
    g_c2 =g_c2 +0x7d;
    }
  if (g_c1 >=0xa0){g_c1 =g_c1 +0x40;}
  if (g_c2 >=0x7f){g_c2 =g_c2 +1;}
  }

// ---------------
// separate check
// ---------------
int Base64_T::sep_check(unsigned char *str,bool mode)
  {
  unsigned int   len;
  unsigned char *sep;
  unsigned int i;

  if (mode == false){len = en_sep_len;  sep = (unsigned char *)en_sep->c_str();}
  else              {len = dis_sep_len; sep = (unsigned char *)dis_sep->c_str();}

  for(i=0;i<len;++i)
    {
    if (*(str+i) == 0x00)    {return(-1);}
    if (*(str+i) != *(sep+i))
      {
      if ((*(sep+i) >= 'A') && (*(sep+i) <= 'Z'))
        {
        if ((*(sep+i) - 'A' + 'a') == *(str+i)) {continue;}
        }
      return(-1);
      }
    }

  return(len);
  }


//--------------------------------------------------------------
// -------
// encode
// -------
char *Base64_T::encode(unsigned char *str)
  {
  unsigned int i,j,ptr;
  unsigned int len;
  bool mode;
  bool sv_mode;
  char *w_buff;
  int sv_ptr;
  char sv_char;

  if (string_en != NULL){delete [] string_en;}
  len = strlen((char *)str);
  string_en = new char[len*en_sep_len+len*dis_sep_len+len*2+1];

  *string_en = 0x00;

  sv_mode = false;
  ptr     = strlen(string_en);
  for(i=0;i<len;++i)
    {
    mode = sjis_check(str+i);
    if (sv_mode != mode)
      {
      if (mode == true) // jis mode
        {
        strcat(string_en,en_sep->c_str());
        ptr += en_sep_len;
        sv_ptr = i;
        ++i;
        }
      else              // ascii mode
        {
        sv_char = *(str+i);
        *(str+i) = 0x00;
        w_buff = sjis_base64(str+sv_ptr);
        *(str+i) = sv_char;
        strcat(string_en,w_buff);
        ptr += strlen(w_buff);
        strcat(string_en,dis_sep->c_str());
        ptr += dis_sep_len;
        *(string_en+ptr) = *(str+i); ++ptr;
        *(string_en+ptr) = 0x00;
        }
      sv_mode = mode;
      continue;
      }

    if (mode == true) {++i;}
    else
      {
      *(string_en+ptr) = *(str+i); ++ptr;
      *(string_en+ptr) = 0x00;
      }
    }

  if (mode == true)
    {
    w_buff = sjis_base64(str+sv_ptr);
    strcat(string_en,w_buff);
    strcat(string_en,dis_sep->c_str());
    }

//  strcat(string_en,dis_sep->c_str());

  return(string_en);
  }


// ----------------
// sjis -> base64
// ----------------
char *Base64_T::sjis_base64(char *buff)
  {
  unsigned char *p,*q,a[5];
  int buff_len;
  char *tmp;
  int i;


  if (out_buff_en != NULL){delete [] out_buff_en;}
  buff_len = strlen(buff);
  tmp      = new char[buff_len*2+8];

  tmp[0] = 0x1b;
  tmp[1] = 0x24;
  tmp[2] = 0x42;

  for(i=0;i<buff_len;i=i+2)
    {
    sjis_jis(*(buff+i),*(buff+i+1));
    tmp[i  +3] = g_c1;
    tmp[i+1+3] = g_c2;
    }
  buff_len += 3;  
  tmp[buff_len] = 0x1b; ++buff_len;
  tmp[buff_len] = 0x28; ++buff_len;
  tmp[buff_len] = 0x42; ++buff_len;
  tmp[buff_len] = 0x00;
  out_buff_en   = new char[buff_len*2+12];

  for(p=tmp,q=out_buff_en;;p+=3)
    {
    if (*p == 0x00){break;}
    mem_cpy(a,p,3);

    if (a[1] == 0x00)
      {
      *q = b64[(unsigned char)( a[0] >> 2)                    ]; ++q;
      *q = b64[(unsigned char)((a[0] << 4) & 0x3f)            ]; ++q;
      *q = b64[64                                             ]; ++q;
      *q = b64[64                                             ]; ++q;
      break;
      }
    if (a[2] == 0x00)
      {
      *q = b64[(unsigned char)( a[0] >> 2)                    ]; ++q;
      *q = b64[(unsigned char)((a[0] << 4 | a[1] >> 4) & 0x3f)]; ++q;
      *q = b64[(unsigned char)((a[1] << 2) & 0x3f)            ]; ++q;
      *q = b64[64                                             ]; ++q;
      break;
      }
    *q = b64[(unsigned char)( a[0] >> 2)                    ]; ++q;
    *q = b64[(unsigned char)((a[0] << 4 | a[1] >> 4) & 0x3f)]; ++q;
    *q = b64[(unsigned char)((a[1] << 2 | a[2] >> 6) & 0x3f)]; ++q;
    *q = b64[(unsigned char)( a[2] & 0x3f)                  ]; ++q;
    }
  *q = 0x00;

  delete [] tmp;
  return(out_buff_en);
  }

// ----------------
// sjis -> base64
// ----------------
char *Base64_T::sjis_base64_attach(char *buff,int buff_len)
  {
  unsigned char *p,*q,a[5];
  char *tmp;
  int i;


  if (out_buff_en != NULL){delete [] out_buff_en;}
  tmp = buff;
  out_buff_en   = new char[buff_len*2+12];

  int w_len = buff_len;
  for(p=tmp,q=out_buff_en;;p+=3)
    {
    if (w_len == 0){break;}
    if (w_len == 1)
      {
      w_len = w_len - 1;
      mem_cpy(a,p,1);
      *q = b64[(unsigned char)( a[0] >> 2)                    ]; ++q;
      *q = b64[(unsigned char)((a[0] << 4) & 0x3f)            ]; ++q;
      *q = b64[64                                             ]; ++q;
      *q = b64[64                                             ]; ++q;
      continue;
      }
    if (w_len == 2)
      {
      w_len = w_len - 2;
      mem_cpy(a,p,2);
      *q = b64[(unsigned char)( a[0] >> 2)                    ]; ++q;
      *q = b64[(unsigned char)((a[0] << 4 | a[1] >> 4) & 0x3f)]; ++q;
      *q = b64[(unsigned char)((a[1] << 2) & 0x3f)            ]; ++q;
      *q = b64[64                                             ]; ++q;
      w_len = 0;
      continue;
      }
    w_len = w_len - 3;
    mem_cpy(a,p,3);
    *q = b64[(unsigned char)( a[0] >> 2)                    ]; ++q;
    *q = b64[(unsigned char)((a[0] << 4 | a[1] >> 4) & 0x3f)]; ++q;
    *q = b64[(unsigned char)((a[1] << 2 | a[2] >> 6) & 0x3f)]; ++q;
    *q = b64[(unsigned char)( a[2] & 0x3f)                  ]; ++q;
    }
  *q = 0x00;

  // delete [] tmp;
  return(out_buff_en);
  }




// ----------
// sjis->jis
// ----------
void Base64_T::sjis_jis(unsigned char p_c1,unsigned p_c2)
  {
  g_c1 = p_c1;
  g_c2 = p_c2;
  if (g_c1 >=0xe0){g_c1 =g_c1 -0x40;}
  if (g_c2 >=0x80){g_c2 =g_c2 -1;}
  if (g_c2 >=0x9e)
    {
    g_c1 =(g_c1 -0x70)*2;
    g_c2 =g_c2 -0x7d;
    }
  else
    {
    g_c1 =((g_c1 -0x70)*2)-1;
    g_c2 =g_c2 -0x1f;
    }
  }


// ---------------
// sjis check
// ---------------
bool Base64_T::sjis_check(unsigned char *str){
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
