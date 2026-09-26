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

