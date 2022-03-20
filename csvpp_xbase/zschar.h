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

#ifndef _ZSCHAR_H_
#define _ZSCHAR_H_

#include "xarea.h"
#include "zsjis_anl.h"

// # string class ----------------
class sChar {
  public:
    VariableArea_C *area;
    sChar(char *src);
    sChar(char *src,int len);
    sChar(sChar *src);
    ~sChar();
    char *c_str (void)             {return(area->cc_str());}
    int   len   (void)             {return(area->len());}
    void  set   (char *src)        {area->set_string(src);}
    void  set   (char *src,int len){area->init(); area->set_n(src,len); area->set(0x00);}
    void  intset(int i)            {area->int_set(i);}
    void  cat   (char *src)        {area->cat_string(src);}
    void  intcat(int i)            {area->int_cat(i);}
    void  set   (sChar *src);
    void  push  (void);
    void  beta  (int size,char c);

    // - ŒÝŠ· -----
    void  Push  (void){push();}

};




// # string numeric class ----------------
class sCharNum : public sChar {
  public:
    int   c_int (void)             {return(SujiConvSuper(area->cc_str()));}
    unsigned int c_uint(void)      {return(uSujiConvEx(area->cc_str()));}

    VariableArea_C *sv_str;
    sCharNum(char *src) : sChar(src){
      sv_str     = new VariableArea_C(10);
    }
    ~sCharNum(){
      delete sv_str;
    }

  private:
    unsigned long uSujiConv(char *str);
    unsigned long uSujiConvEx(char *str);
    long SujiConv(char *str);
    long SujiConvEx(char *str);
    long SujiConvSuper(char *str);
    void long_decc(char *str,long in,int len);
    void long_hexc(char *str,long in,int len);
    void long_bitc(char *str,long in,int len);
    unsigned long u_decc_long(char *str,int len);
    long decc_long(char *str,int len);
    unsigned long u_hexc_long(char *str,int len);
    long hexc_long(char *str,int len);
    long bitc_long(char *str,int len);
    long bitc_long2(char *str,int len);
    unsigned long u_bitc_long(char *str,int len);
    void bitc_ex(char *str,int len);
};

// # string extend class ----------------
class sCharEX : public sChar {
  public:
    VariableArea_C *sv_str;
    sjisAnl_C      *sjis;

    sCharEX(char *src) : sChar(src){
      sv_str     = new VariableArea_C(10);
      sjis       = new sjisAnl_C;
    }
    ~sCharEX(){
      delete sv_str;
      delete sjis;
    }
    void set_index(char *src,unsigned int ptr,unsigned int len,bool ex);
    void DblIns   (char *p_str);
};


// # Grep class ----------------
class sCharGrep : public sChar {
  public:
    VariableArea_C *sv_str;
    VariableArea_C *srch_str;
    VariableArea_C *srch_key;
    sjisAnl_C      *sjis;
    sjisAnl_C      *sjis2;

    sCharGrep(char *src) : sChar(src){
      sv_str     = new VariableArea_C(10);
      srch_str   = new VariableArea_C(10);
      srch_key   = new VariableArea_C(10);
      sjis       = new sjisAnl_C;
      sjis2      = new sjisAnl_C;
    }
    ~sCharGrep(){
      delete sv_str;
      delete srch_key;
      delete srch_str;
      delete sjis;
      delete sjis2;
    }

    int grep              (char *srch_key,int offset){setGrep(srch_key,false); return(grepCore(offset));}
    int grepAa_same       (char *srch_key,int offset){setGrep(srch_key,true ); return(grepCore(offset));}
    int replace           (char *srch_key,char *rep_key,int offset){int hit_ptr = grep       (srch_key,offset); replaceCore(srch_key,rep_key,hit_ptr); return(hit_ptr);}
    int replaceAa_same    (char *srch_key,char *rep_key,int offset){int hit_ptr = grepAa_same(srch_key,offset); replaceCore(srch_key,rep_key,hit_ptr); return(hit_ptr);}
    int replace_all       (char *srch_key,char *rep_key);
    int replace_allAa_same(char *srch_key,char *rep_key);
  private:
    void Atoa             (char *src,char *dst,int len);
    bool mem_cmp          (char *cmp1,char *cmp2,int offset,int len);
    void setGrep          (char *p_srch_key,bool sameAaFlag);
    int  grepCore         (int offset);
    void replaceCore      (char *srch_key,char *rep_key,int hit_ptr);
};


#endif // _ZSCHAR_H_

