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


#include "xkanji.h"

//#define SJIS_EUC_DEBUG 1
//#define EUC_SJIS_DEBUG 1

//#define GAIJI_DEBUG 1

// # gaiji cell #####
GaijiC::GaijiC(int p_first_str,int p_upper,int p_lower,int p_first_ch,int p_second_ch)
  {
  first_str = (unsigned char)p_first_str;
  upper = (unsigned char)p_upper;
  lower = (unsigned char)p_lower;
  if (p_first_ch < 0) {first_sign = true;   first_ch  = (unsigned char)(p_first_ch * -1);}
  else                {first_sign = false;  first_ch  = (unsigned char)p_first_ch;}
  if (p_second_ch < 0){second_sign = true;  second_ch = (unsigned char)(p_second_ch * -1);}
  else                {second_sign = false; second_ch = (unsigned char)p_second_ch;}
  }


// # constructor/destructor #####
kanji_T::kanji_T()
  {
  it = new MemHeader_T;
  sjis_fp = new MEM_FP(it);
  euc_fp  = new MEM_FP(it);
  }

kanji_T::~kanji_T()
  {
  GaijiC *gj;

  MEM_LOOP(gj,GaijiC,sjis_fp) delete gj; LOOP_END
  MEM_LOOP(gj,GaijiC,euc_fp ) delete gj; LOOP_END
  delete sjis_fp;
  delete euc_fp;
  delete it;
  }



// # return code check & delete #####
int kanji_T::return_check(char *buff,int len)
  {
  d_flg = false;
  a_flg = false;
  NULL_flg = false;

  if (*buff == 0x00){NULL_flg=true; return(0);}
  int ptr;
  for(ptr=len;ptr>=0;ptr--)
    {
    unsigned char c = *(buff+ptr);
    if ((c != 0x0d) && (c != 0x0a) && (c != 0x00)){break;}
    if (c == 0x0a)
      {
      if (a_flg == false){*(buff+ptr)=0x00; a_flg = true;}
      else               {break;}
      }
    if (c == 0x0d)
      {
      if (d_flg == false){*(buff+ptr)=0x00; d_flg = true; break;}
      else               {break;}
      }
    }
  return(ptr+1);
  }

// # sjis check #####
bool kanji_T::sJisCheck(char *str)
  {
  unsigned char w1;
  unsigned char w2;

  bool shift_flg = false;
  w1 = (unsigned char)*(str);
  w2 = (unsigned char)*(str+1);
  if (((w1 >= 0x81) && (w1 <= 0x9f))
   || ((w1 >= 0xe0) && (w1 <= 0xfe)))
    {
    if (((w2 >= 0x40) && (w2 <= 0x7e))
     || ((w2 >= 0x80) && (w2 <= 0xfc)))
      {
      shift_flg = true;
      }
    }
  return(shift_flg);
  }

bool kanji_T::sJisHankakuCheck(char *str)
  {
  unsigned char w1;

  bool shift_flg = false;
  w1 = (unsigned char)*(str);
  if ((w1 >= 0xa1) && (w1 <= 0xdf))
    {
    shift_flg = true;
    }
  return(shift_flg);
  }

// # euc check #####
bool kanji_T::eucCheck(char *str)
  {
  unsigned char w1;
  unsigned char w2;

  bool shift_flg = false;
  w1 = (unsigned char)*(str);
  w2 = (unsigned char)*(str+1);
  if ((w1 >= 0xa1) && (w1 <= 0xfe))
    {
    if ((w2 >= 0xa1) && (w2 <= 0xfe))
      {
      shift_flg = true;
      }
    }
  return(shift_flg);
  }

bool kanji_T::eucHankakuCheck(char *str)
  {
  unsigned char w1;
  unsigned char w2;

  bool shift_flg = false;
  w1 = (unsigned char)*(str);
  w2 = (unsigned char)*(str+1);
  if (w1 == 0x8e)
    {
    if ((w2 >= 0xa1) && (w2 <= 0xdf))
      {
      shift_flg = true;
      }
    }
  return(shift_flg);
  }




// # euc -> sjis  #####
void kanji_T::eucSjis(char *str)
  {
  if (ext_sjis_euc(str,K_EUC_SJIS))
    {
    o1 = gj_o1;
    o2 = gj_o2;
    return;
    }

  unsigned char c1;
  unsigned char c2;
  c1 = (int)*(str);
  c2 = (int)*(str+1);

  c1 = c1 - 0x80;
  c2 = c2 - 0x80;

  if (c1 % 2) {
    c1 = ((c1 + 1) / 2) + 0x70;
    c2 = c2 + 0x1f;
  } else {
    c1 = (c1 / 2) + 0x70;
    c2 = c2 + 0x7d;
  }
  if (c1 >= 0xa0) { c1 = c1 + 0x40; }
  if (c2 >= 0x7f) { c2 = c2 + 1; }

  o1 = c1;
  o2 = c2;
  }


// # sjis -> euc  #####
void kanji_T::sjisEuc(char *str)
  {
  if (ext_sjis_euc(str,K_SJIS_EUC))
    {
    o1 = gj_o1;
    o2 = gj_o2;
    return;
    }

  unsigned char c1;
  unsigned char c2;
  c1 = (int)*(str);
  c2 = (int)*(str+1);

  if (c1 >= 0xe0) { c1 = c1 - 0x40; }
  if (c2 >= 0x80) { c2 = c2 - 1; }
  if (c2 >= 0x9e) {
    c1 = (c1 - 0x70) * 2;
    c2 = c2 - 0x7d;
  } else {
    c1 = ((c1 - 0x70) * 2) - 1;
    c2 = c2 - 0x1f;
  }

  o1 = c1 + 0x80;
  o2 = c2 + 0x80;
  }


// # return code setting #####
void kanji_T::return_code_setting(int kbn)
  {
  if (kbn == K_WIN ){ins_ret[0] = 0x0d; ins_ret[1] = 0x0a; ins_ret[2] = 0x00;}
  if (kbn == K_UNIX){ins_ret[0] = 0x0a; ins_ret[1] = 0x00; ins_ret[2] = 0x00;}
  if (kbn == K_MAC ){ins_ret[0] = 0x0d; ins_ret[1] = 0x00; ins_ret[2] = 0x00;}
  }


// # sjis -> euc convert #####
void kanji_T::sjis_euc_conv(FILE *in_fp,FILE *out_fp)
{
  xFgets_T *xf = new xFgets_T(4096);
  char *xx_buff;
  char c1,c2;
  for(;;)
    {
    xx_buff = xf->xFgets(in_fp);
    if (xx_buff == NULL){break;}
    int len = strlen(xx_buff);
    len = return_check(xx_buff,len);

    char *buff = new char[len+1];
    strcpy(buff,xx_buff);

#ifdef SJIS_EUC_DEBUG
  printf("buff=%s len=%d  [ ",buff,len);
  for(int i=0;i<len;i++){printf("%x ",(unsigned char)*(buff+i));}
  printf("]\n");
#endif

    for(int i=0;i<len;i++)
      {
      if (sJisCheck(buff+i))
        {
        sjisEuc(buff+i);
        *(buff+i)   = out1();
        *(buff+i+1) = out2();
        i++;
        }
      else if (sJisHankakuCheck(buff+i))
        {
        char *w_buff = new char[len+2];
        strcpy(w_buff,buff);
        *(w_buff+i) = 0x8e;
        for(int j=0;j<len-i;j++)
          {
          *(w_buff+i+j+1) = *(buff+i+j);
          }
        delete [] buff;
        buff = w_buff;
        len++;
        *(buff+len) = 0x00;
        i++;
        }
      }

    // printf("ins_ret %x %x\n",ins_ret[0],ins_ret[1],ins_ret[2]);

    if (NULL_flg == false)
      {
      if ((d_flg == false) && (a_flg == false))
        {
        if (out_fp == NULL){ printf(       "%s",buff);}
        else               {fprintf(out_fp,"%s",buff);}
        }
      else
        {
        if (out_fp == NULL){ printf(       "%s%s",buff,ins_ret);}
        else               {fprintf(out_fp,"%s%s",buff,ins_ret);}
        }
      }
    delete [] buff;

    }
  delete xf;
  }


// # euc -> sjis convert #####
void kanji_T::euc_sjis_conv(FILE *in_fp,FILE *out_fp)
  {
  xFgets_T *xf = new xFgets_T(4096);
  char *buff;
  char c1,c2;
  for(;;)
    {
    buff = xf->xFgets(in_fp);
    if (buff == NULL){break;}
    int len = strlen(buff);
    len = return_check(buff,len);

#ifdef EUC_SJIS_DEBUG
  printf("buff=%s len=%d  [ ",buff,len);
  for(int i=0;i<len;i++){printf("%x ",(unsigned char)*(buff+i));}
  printf("]\n");
#endif

    for(int i=0;i<len;i++)
      {
      if (eucCheck(buff+i))
        {
        eucSjis(buff+i);
        *(buff+i)   = out1();
        *(buff+i+1) = out2();
        i++;
        }
      else if (eucHankakuCheck(buff+i))
        {
        for(int j=0;j<len-i-1;j++){*(buff+i+j) = *(buff+i+j+1);}
        len=len-1;
        *(buff+len)=0x00;
        }
      }
    if (NULL_flg){continue;}

    if ((d_flg == false) && (a_flg == false))
      {
      if (out_fp == NULL){ printf(       "%s",buff);}
      else               {fprintf(out_fp,"%s",buff);}
      }
    else
      {
      if (out_fp == NULL){ printf(       "%s%s",buff,ins_ret);}
      else               {fprintf(out_fp,"%s%s",buff,ins_ret);}
      }
    }
  delete xf;
  }



// # gaiji change #####
bool kanji_T::ext_sjis_euc(char *str,int kbn)
  {
  unsigned char i1;
  unsigned char i2;

  if (*str     == 0x00){return(false);}
  if (*(str+1) == 0x00){return(false);}
  i1 = *str;
  i2 = *(str+1);

  MEM_FP *mem_fp;
  if (kbn == K_SJIS_EUC){mem_fp = sjis_fp;}
  else                  {mem_fp = euc_fp;}

  bool flg=false;
  GaijiC *gj;
  MEM_LOOP(gj,GaijiC,mem_fp)


#ifdef GAIJI_DEBUG
  printf("input [%x %x] check [%x %x - %x]\n",i1,i2,gj->first_str,gj->upper,gj->lower);
#endif


    if ((gj->first_str == i1) && (gj->upper >= i2) && (gj->lower <= i2))
      {
      if (gj->first_sign ){gj_o1 = i1 - gj->first_ch;}
      else                {gj_o1 = i1 + gj->first_ch;}
      if (gj->second_sign){gj_o2 = i2 - gj->second_ch;}
      else                {gj_o2 = i2 + gj->second_ch;}
      flg = true;
      break;
      }
  LOOP_END

  return(flg);
  }


// # gaiji setting #####
void kanji_T::gaiji_setting(char *file)
  {
  CsvInf_T *csvi = new CsvInf_T(file);
  if (csvi->CsvError_Rd() != 0){delete csvi; return;}

  for(;;)
    {
    if (csvi->CsvFgets()    == -1){break;}
    if (csvi->CsvSelSu_Rd() ==  0){continue;}
    char *str = csvi->CsvSel_Rd(0);
    if (strcmp(str,"#sjis_euc") == 0){sjis_euc_set(csvi,K_SJIS_EUC);}
    if (strcmp(str,"#euc_sjis") == 0){sjis_euc_set(csvi,K_EUC_SJIS);}
    }
  delete csvi;

#ifdef GAIJI_DEBUG
  ext_print(sjis_fp,"SJIS->EUC");
  ext_print(euc_fp,"EUC->SJIS");
#endif  
  }


void kanji_T::ext_print(MEM_FP *mem_fp,char *c)
  {
  GaijiC *gj;
  bool first_flg=true;
  MEM_LOOP(gj,GaijiC,mem_fp)
    if (first_flg)
      {
      first_flg=false;
      printf("ŠOŽšÝ’è(%s)\n",c);
      }
    printf("[0x%x%x ` 0x%x%x]",gj->first_str,gj->lower,gj->first_str,gj->upper);
    printf(" ˆê•¶Žš–Ú‚Ì·•ª:");
    if (gj->first_sign){printf("-");} else {printf("+");}
    printf("%03d  ",gj->first_ch);
    printf(" “ñ•¶Žš–Ú‚Ì·•ª:");
    if (gj->second_sign){printf("-");} else {printf("+");}
    printf("%03d\n",gj->second_ch);

  LOOP_END
  if (first_flg == false){printf("\n");}
  }

void kanji_T::sjis_euc_set(CsvInf_T *csvi,int kbn)
  {
  char *str;
  char *first_str;
  char *upper;
  char *lower;
  char *first_ch;
  char *second_ch;

  first_str = NULL;
  upper = NULL;
  lower = NULL;
  first_ch = NULL;
  second_ch = NULL;

  MEM_FP *mem_fp;
  if (kbn == K_SJIS_EUC){mem_fp = sjis_fp;}
  else                  {mem_fp = euc_fp;}

  for(int i=1;i<csvi->CsvSelSu_Rd();++i)
    {
    if ((str = csvi->CsvSel_Rd(i)) == MM_NULL) {continue;}
    if (first_str == NULL){first_str = str; continue;}
    if (upper     == NULL){upper     = str; continue;}
    if (lower     == NULL){lower     = str; continue;}
    if (first_ch  == NULL){first_ch  = str; continue;}
    if (second_ch == NULL){second_ch = str; }

    GaijiC *gj = new GaijiC(
       SujiConvSuper(first_str)
      ,SujiConvSuper(upper)
      ,SujiConvSuper(lower)
      ,SujiConvSuper(first_ch)
      ,SujiConvSuper(second_ch)
    );
    it->alloc_ptr = gj;
    mem_fp->mem_alloc();
    break;
    }
  }

