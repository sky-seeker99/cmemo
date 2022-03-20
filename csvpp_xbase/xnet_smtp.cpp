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

#include "xnet_smtp.h"

//
//  Smtp Control
//
smtpInf_T::smtpInf_T(char *in_file,char *log_file,char *host){
  char *str;
  
  in_fp  = fopen(in_file ,"rt");
  log_fp = fopen(log_file,"wt");
  net_fp = new NetworkInf_T(host,25);
  ok_flg = true;
  c_ret[0] = 0x0d; // Ver 0.91
  c_ret[1] = 0x0a;
  c_ret[2] = 0x00;
  line_cnt = -1;

  if (in_fp                 == NULL){printf("in-file open error.(%s)\n" ,in_file );           ok_flg = false;}
  if (log_fp                == NULL){printf("log-file open error.(%s)\n",log_file);           ok_flg = false;}
  if (net_fp->errorChk()    != 0   ){printf("network error.(code=%d)\n" ,net_fp->errorChk()); ok_flg = false;}

/*  if (ok_flg == false){
    if (log_fp != NULL){fclose(log_fp);}
    if (in_fp  != NULL){fclose(in_fp );}
    log_fp = NULL;
    in_fp  = NULL;
    delete net_fp;
    net_fp = NULL;
    return;
    }
*/

  okCheck("CONNECT","220");
  strSnd("HELO %s",host,"HELO","250");

  for(;;)
    {
    if (ok_flg == false){break;}
    if (fgets(buff,4096-1,in_fp) == NULL){break;}
    if (*(buff+strlen(buff)-1) < ' '){*(buff+strlen(buff)-1) = 0x00;}
    if (mem_cmp(buff,"#mail_from,",11)==0) {mail_from_rtn(buff+11); continue;}
    if (mem_cmp(buff,"#rcpt_to,"  ,9 )==0) {rcpt_to_rtn  (buff+9 ); continue;}
    if (mem_cmp(buff,"#wr,"       ,4 )==0) {wr_rtn       (buff+4 ); continue;}
    if (mem_cmp(buff,"#timeout,"  ,9 )==0) {timeout_rtn  (buff+9 ); continue;}
    }

  if (line_cnt != -1){strSnd(".","DATA_END","250");}
  strSnd("QUIT","QUIT","221");
  }

smtpInf_T::~smtpInf_T(){
  if (in_fp  != NULL){fclose(in_fp);}
  if (log_fp != NULL){fclose(log_fp);}
  delete net_fp;
  }

// #timeout #####
void smtpInf_T::timeout_rtn(char *str)
  {
  int val;
  val = SujiConvEx(str);

  if (ok_flg == false){return;}

  if (val < 0){val = 10000;}
  printf("TIMEOUT->%d\n",val);
  net_fp->timeoutSet(val);
  }

// #rcpt_to #####
void smtpInf_T::rcpt_to_rtn(char *str)
  {
  if (line_cnt != -1){fprintf(log_fp,"#result,RCPT_TO,NG,Send Error\n"); return;}
  strSnd("RCPT TO:<%s>",str,"RCPT_TO","250");
  }

// #mail_from #####
void smtpInf_T::mail_from_rtn(char *str)
  {
  if (line_cnt != -1){fprintf(log_fp,"#result,RCPT_TO,NG,Send Error\n"); return;}
  strSnd("MAIL FROM:<%s>",str,"MAIL_FROM","250");
  }

// #wr #####
void smtpInf_T::wr_rtn(char *str)
  {
  if (line_cnt == -1){strSnd("DATA","DATA_BEGIN","354"); line_cnt = 0;}
  if (ok_flg == false){return;}
  printf("line:%d\r",line_cnt++);
  sprintf(sndBuff,"%s%s",str,c_ret); net_fp->netSend(sndBuff); // Ver 0.91
  }


// # String Send Ver 0.93 #####
int smtpInf_T::strSnd(char *str,char *okw,char *okStr){
  if (ok_flg == false){okCheck(okw,okStr); return(-1);}

#ifdef SMTP_DEBUG
  printf("\n>%s<\n",str);
#endif

  sprintf(sndBuff2,"%s%s",str,c_ret  );
  net_fp->netSend(sndBuff2);
  return(okCheck(okw,okStr));
  }
int smtpInf_T::strSnd(char *str,char *ss,char *okw,char *okStr){sprintf(sndBuff,str,ss); return(strSnd(sndBuff,okw,okStr));}


// ok check #####
bool smtpInf_T::okCheck(char *str,char *ok_str)
  {
  char *w;
  int len;
  //printf(">%s\n",str);

  if (ok_flg == false){printf("%s->NG network error\n",str); fprintf(log_fp,"#result,%s,NG,network error\n",str); return(true);} // Ver 0.93

  for(;;)
    {
    if ((len = net_fp->netEvent()) == -1){fprintf(log_fp,"#result,%s,NG\n",str); printf("network error.\n"); ok_flg = false; return(-1);}
    if (len > 0){break;}
    }
  w = net_fp->getRcvBuff();

#ifdef SMTP_DEBUG
  printf("[%s]\n",w);
#endif

  *(w+len) = 0x00;
  if (mem_cmp(w,ok_str,3) == 0){printf("%s->OK\n"   ,str  ); fprintf(log_fp,"#result,%s,OK\n"   ,str  ); return(false);}
  else                         {ok_flg = false; printf("%s->NG %s\n",str,w); fprintf(log_fp,"#result,%s,NG,%s\n",str,w); return(true );}
  }



