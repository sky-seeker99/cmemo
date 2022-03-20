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
// Wild Card
//
#include <stdio.h>
#include <string.h>
#include "xwildcard.h"

// ------------
// printf
// ------------
void WildCard_T::pr()
  {
  sChar *str;

  printf("key[%s][%s][%s]\n",all_str->c_str(),top_str->c_str(),end_str->c_str());
  MEM_LOOP(str,sChar,mid_fp)
   printf("mid_key[%s]\n",str->c_str());
  LOOP_END

  }


// ------------
// keyword set
// ------------
void WildCard_T::set(char *p_str)
  {
  StrSrchInf_T *ssc;
  int max;
  int i;
  sChar *w;
  bool aster_flg;
  bool top_flg;
  
  all_str->set("");
  top_str->set("");
  end_str->set("");
  MEM_LOOP(w,sChar,mid_fp) delete w; LOOP_END
  mid_fp->mem_del_all();
  
  ssc = new StrSrchInf_T();
  ssc->Set('*');
  ssc->SetFlg();
  
  ssc->Main(p_str);
  max = ssc->GetSu();
  aster_flg = false;
  top_flg = true;
  for(i=0;i<max;++i)
    {
    w = ssc->Get(i);
    if (top_flg == true)
      {
      if (max == 1)
        {
        all_str->set(p_str);
        if (strcmp(p_str,"*") == 0){all_str->set("");}
        break;
        }
      if (strcmp(w->c_str(),"*") == 0){aster_flg = true;}
      else                            {top_str->set(w->c_str());}
      top_flg = false;
      continue;
      }
    
    if (i == max-1)
      {
      if (strcmp(w->c_str(),"*") == 0)
        {
        if (aster_flg == true){end_str->set("*");}
        }
      else
        {
        end_str->set(w->c_str());
        }  
      break;
      }
    
    if (strcmp(w->c_str(),"*") == 0)
      {
      if (aster_flg == false){aster_flg = true; continue;}
//      aster_flg = false;
      }
    it->alloc_ptr = (unsigned char *) new sChar(w->c_str());
    mid_fp->mem_alloc();
    aster_flg = false;
    }
  delete ssc;
  }
  
// ------------
// Compare
// ------------
bool WildCard_T::cmp(char *p_str)
  {
  int top_ptr;
  int end_ptr;
  int i,len;
  char *w;
  int p_str_len;
  char sv;
  sChar *key;

  if (strcmp(all_str->c_str(),"") != 0)
    {
    if (strcmp(all_str->c_str(),p_str) == 0){return(true);}
    return(false);
    }

  top_ptr = 0;
  end_ptr = p_str_len;
  p_str_len = strlen(p_str);
  sv = 0x00;
  if (strcmp(top_str->c_str(),"") != 0)
    {
    len = top_str->len();
    w   = top_str->c_str();
    if (p_str_len < len){return(false);}
    for(i=0;i<len;++i)
      {
      if (*(w+i) != *(p_str+i)){return(false);}
      }
    top_ptr = len;  
    p_str_len = p_str_len - len;
    }
  
  if (strcmp(end_str->c_str(),"") != 0)
    {
    len = end_str->len();
    w   = end_str->c_str();
    if (p_str_len < len){return(false);}
    for(i=0;i<len;++i)
      {
      if (*(w+len-i-1) != *(p_str+top_ptr+p_str_len-i-1)){return(false);}
      }
    end_ptr = p_str_len - len;
    sv = *(p_str+end_ptr+top_ptr);
    *(p_str+end_ptr+top_ptr) = 0x00;
    }

  grep->set(p_str+top_ptr);
  if (sv != 0x00){*(p_str+end_ptr+top_ptr) = sv;}

  top_ptr = 0;
  MEM_LOOP(key,sChar,mid_fp)
    top_ptr = grep->grep(key->c_str(),top_ptr);
    if (top_ptr == -1){return(false);}
  LOOP_END
  return(true);     
  }
  
  
   
