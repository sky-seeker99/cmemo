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

#include "xnet_pop3.h"

//
//  Pop3 Control
//
pop3Inf_T::pop3Inf_T(char *csv_file,char *log_file,char *host,char *id,char *pass){
  char *str;

  c_ret[0] = 0x0d; // Ver 0.92
  c_ret[1] = 0x0a;
  c_ret[2] = 0x00;
  listGetFlg = false;
  listCnt    = 0;

  ssc    = NULL;
  ssc2   = NULL;
  log_fp = fopen(log_file,"wt");
  csv_fp = new CsvInf_T(csv_file);
  net_fp = new NetworkInf_T(host,110);
  ok_flg = true;

  it = new MemHeader_T;
  rcv_msg = new MEM_FP(it);

  if (csv_fp->CsvError_Rd() != 0   ){printf("csv-file open error.(%s)\n",csv_file);           ok_flg = false;}
  if (log_fp                == NULL){printf("log-file open error.(%s)\n",log_file);           ok_flg = false;}
  if (net_fp->errorChk()    != 0   ){printf("network error.(code=%d)\n" ,net_fp->errorChk()); ok_flg = false;}

/*  if (ok_flg == false){
    if (log_fp != NULL){fclose(log_fp);}
    delete csv_fp;
    delete net_fp;
    return;
    }
*/
  sv_str = new sChar("");

  ssc  = new StrSrchInf_T(); ssc->Set (0x0d); ssc->SetFlg (); ssc->Set (0x0a); // Ver 0.92
  ssc2 = new StrSrchInf_T(); ssc2->Set(' ' ); // ssc2->SetFlg();               // Ver 0.92
  okCheck("CONNECT");
  strSnd ("USER %s",id  ,"USER");  // Ver 0.93
  strSnd ("PASS %s",pass,"PASS");  // Ver 0.93

  for(;;)
    {
    if (csv_fp->CsvFgets()    == -1) {break;}
    if (csv_fp->CsvSelSu_Rd() ==  0) {continue;}
    if ((str = csv_fp->CsvSel_Rd(0)) == MM_NULL) {continue;}

#ifdef POP_DEBUG
    printf("command[%s]\n",str);
#endif
    sp_push(str); sp_push2(str); // Ver 0.97

    if (strcmp(str,"#get"        )==0) {net_rtn    (KBN_GET    ); continue;}
    if (strcmp(str,"#get_del"    )==0) {net_rtn    (KBN_GET_DEL); continue;}  // Ver 0.98
    if (strcmp(str,"#get_all"    )==0) {netall_rtn (KBN_GET    ); continue;}
    if (strcmp(str,"#get_del_all")==0) {netall_rtn (KBN_GET_DEL); continue;}  // Ver 0.96
    if (strcmp(str,"#list"       )==0) {netall_rtn (KBN_LIST   ); continue;}
    if (strcmp(str,"#del"        )==0) {net_rtn    (KBN_DEL    ); continue;}
    if (strcmp(str,"#del_all"    )==0) {netall_rtn (KBN_DEL    ); continue;}
    if (strcmp(str,"#timeout"    )==0) {net_rtn    (KBN_TIMEOUT); continue;}
    if (strcmp(str,"#check"      )==0) {mailCount  (           ); continue;}  // Ver 0.92
    }

  strSnd("QUIT","QUIT"); // Ver 0.93

  }

pop3Inf_T::~pop3Inf_T(){
//  if (ok_flg == false){return;}
  // Ver 0.96
  sChar *str;
  MEM_LOOP(str,sChar,rcv_msg)
    delete str;
  LOOP_END
  delete rcv_msg;
  delete it;

  if (ssc  != NULL){delete ssc;}
  if (ssc2 != NULL){delete ssc2;}
  if (log_fp != NULL){fclose(log_fp);}
  delete csv_fp;
  delete net_fp;
  delete sv_str;
  }

// # String Send Ver 0.93 #####
int pop3Inf_T::strSnd(char *str,char *okStr){
  if (ok_flg == false){okCheck(okStr); return(-1);}
  sprintf(sndBuff2,"%s%s",str,c_ret  );
  net_fp->netSend(sndBuff2);
  return(okCheck(okStr));
  }

int pop3Inf_T::strSnd(char *str,char *ss,char *okStr){sprintf(sndBuff,str,ss); return(strSnd(sndBuff,okStr));}
int pop3Inf_T::strSnd(char *str,int   ss,char *okStr){sprintf(sndBuff,str,ss); return(strSnd(sndBuff,okStr));}


// # OK Check #####
int pop3Inf_T::okCheck(char *cmd){
  int len;
  printf("%s -> ",cmd);

  if (ok_flg == false){fprintf(log_fp,"#result,%s,ERR\n",cmd); printf("network error\n"); return(-1);} // Ver 0.93

  for(;;)
    {
    if ((len = net_fp->netEvent()) == -1){ok_flg = false; fprintf(log_fp,"#result,%s,ERR\n",cmd); printf("network error.\n"); return(-1);}
    if (len > 0){break;}
    }

  ssc->Main(net_fp->getRcvBuff());

  if (ssc->GetSu() == 0)   // Ver 0.95
    {
    fprintf(log_fp,"#result,%s,ERR\n",cmd);
    printf("network error\n");
    ok_flg = false;
    return(-1);
    }

  w = ssc->Get(0);
  ssc2->Main(w->c_str());

  if (ssc2->GetSu() == 0)   // Ver 0.95
    {
    fprintf(log_fp,"#result,%s,ERR\n",cmd);
    printf("network error\n");
    ok_flg = false;
    return(-1);
    }

  w = ssc2->Get(0);
  printf("%s\n",w->c_str());
  if (mem_cmp(w->c_str(),"+OK",3) == 0){fprintf(log_fp,"#result,%s,OK\n" ,cmd); return(len );}
  else                                 {fprintf(log_fp,"#result,%s,ERR\n",cmd); printf("network error\n"); ok_flg = false; return(-1);}
  }

// #timeout/#del/#get #####
void pop3Inf_T::net_rtn(int kbn)
  {
  int i;
  char *str;
  int val;

  for(i=1;i<csv_fp->CsvSelSu_Rd();++i)
    {
    if ((str = csv_fp->CsvSel_Rd(i)) == MM_NULL) {continue;}
    val = SujiConvEx(str);
    break;
    }

  if (kbn == KBN_TIMEOUT){if (val < 0){val = 10000;} printf("timeout set = %d\n",val); net_fp->timeoutSet(val);}
  if (kbn == KBN_GET    ){if (val < 0){val = 0;    } mailRecieve(val);}
  if (kbn == KBN_DEL    ){if (val < 0){val = 0;    } mailDelete (val);}
  if (kbn == KBN_GET_DEL)  // Ver 0.98
    {
    if (val < 0){val = 0;}
    if (mailRecieve_ex(val))
      {
      if (mailDelete_ex(val))
        {
        fprintf(log_fp,"#getno,%d\n",val);
        sChar *str;
        MEM_LOOP(str,sChar,rcv_msg)
          fprintf(log_fp,"#wr,%s\n",str->c_str());
        LOOP_END
        fprintf(log_fp,"#delno,%d\n",val);
        }
      }
    }
  }

// #del_all/#get_all #####
void pop3Inf_T::netall_rtn(int kbn)
  {
  sChar *str;
  int cnt = mailCount();
  for(int i=0;i<cnt;++i)
    {
    if (kbn == KBN_GET ){mailRecieve    (i);}
    if (kbn == KBN_DEL ){mailDelete     (i);}
    if (kbn == KBN_LIST){mailHeadRecieve(i);}
    if (kbn == KBN_GET_DEL) // Ver 0.96
      {
      if (mailRecieve_ex(i))
        {
        if (mailDelete_ex(i))
          {
          fprintf(log_fp,"#getno,%d\n",i);
          MEM_LOOP(str,sChar,rcv_msg)
            fprintf(log_fp,"#wr,%s\n",str->c_str());
          LOOP_END
          fprintf(log_fp,"#delno,%d\n",i);
          }
        }
      }
    }
  }


// mail count #####
int pop3Inf_T::mailCount(void)
  {
  int len = strSnd("LIST","LIST");
  if (len == -1){return(0);}
  int cnt = recieve(len,NULL)-1; // Ver 0.92
  fprintf(log_fp,"#count,%d\n",cnt);
  return(cnt);
  }

// # mail delete #####
void pop3Inf_T::mailDelete(int val)
  {
  if (strSnd("DELE %d",val+1,"DEL") == -1){return;}
  fprintf(log_fp,"#delno,%d\n",val);
  }

// # mail recieve #####
void pop3Inf_T::mailRecieve(int val)
  {
  int len = strSnd("RETR %d",val+1,"RETR");
  if (len == -1){return;}
  fprintf(log_fp,"#getno,%d\n",val);
//  w = ssc2->Get(2);
//  fprintf(log_fp,"#size,%d\n",SujiConvEx(w->c_str()));
  recieve(len,"#wr"); // Ver 0.92
  }

// # mail head recieve #####
void pop3Inf_T::mailHeadRecieve(int val)
  {
  int len = strSnd("TOP %d 0",val+1,"TOP");
  if (len == -1){return;}
  fprintf(log_fp,"#getno,%d\n",val);
//  w = ssc2->Get(2);
//  fprintf(log_fp,"#size,%d\n",SujiConvEx(w->c_str()));
  recieve(len,"#wr"); // Ver 0.92
  }

// # recieve Ver 0.92 #####
int pop3Inf_T::recieve(int len,char *cmd)
  {
  int  cnt      = len;
  bool loop_flg = true;
  int  line     = 0;
  int mode = 0;

#ifdef POP_DEBUG
  printf("-------------\n");
#endif

  for(;;)
    {
    printf("recieve:%d\r",cnt);
    for(int i=0;i<ssc->GetSu();++i)
      {
      w = ssc->Get(i);


#ifdef POP_DEBUG
  printf("recieve[%s]\n",w->c_str());
#endif
      char *buff = w->c_str();

      if (i == ssc->GetSu()-1) // string tochu
        {
        if (*buff != 0x0d)
          {
          // printf("remain:%s\n",buff);
          sv_str->set(buff);
          break;
          }
        }

      if (*buff == 0x0d)
        {
        if (mode == 1)
          {
          if (cmd != NULL){fprintf(log_fp,"%s,\n",cmd);}
          }
        mode = 1;
        continue;
        }
      else
        {
        mode = 0;
        }
      if (strcmp(buff,".") == 0){loop_flg = false; break;}
      if (cmd != NULL){fprintf(log_fp,"%s,%s%s\n",cmd,sv_str->c_str(),w->c_str()); sv_str->set("");}
      line++;
      }
    if (loop_flg == false){printf("complete.         \n"); break;}
    if ((len = net_fp->netEvent()) == -1){fprintf(log_fp,"#error\n"); printf("socket error.         "); ok_flg = false; return(0);}
    ssc->Main(net_fp->getRcvBuff());
    }
  return(line);
  }

// ------------------------------------------------------------------------

// # mail delete (Ver 0.96) #####
bool pop3Inf_T::mailDelete_ex(int val)
  {
  if (strSnd("DELE %d",val+1,"DEL") == -1){return(false);}
  return(true);
  }

// # mail recieve(Ver 0.96) #####
bool pop3Inf_T::mailRecieve_ex(int val)
  {
  int len = strSnd("RETR %d",val+1,"RETR");
  if (len == -1){return(false);}
  if (recieve_ex(len)){return(true);}
  return(false);
  }

// # recieve (Ver 0.96) #####
bool pop3Inf_T::recieve_ex(int len)
  {
  int  cnt      = len;
  bool loop_flg = true;
//  int  line     = 0;
  int mode = 0;

  sChar *str;
  MEM_LOOP(str,sChar,rcv_msg)
    delete str;
  LOOP_END
  rcv_msg->mem_del_all();  

#ifdef POP_DEBUG
  printf("-------------\n");
#endif

  for(;;)
    {
    printf("recieve:%d\r",cnt);
    for(int i=0;i<ssc->GetSu();++i)
      {
      w = ssc->Get(i);


#ifdef POP_DEBUG
  printf("recieve[%s]\n",w->c_str());
#endif
      char *buff = w->c_str();

      if (i == ssc->GetSu()-1) // string tochu
        {
        if (*buff != 0x0d)
          {
          // printf("remain:%s\n",buff);
          sv_str->set(buff);
          break;
          }
        }

      if (*buff == 0x0d)
        {
        if (mode == 1)
          {
          //if (cmd != NULL){fprintf(log_fp,"%s,\n",cmd);}
          it->alloc_ptr = new sChar("");
          rcv_msg->mem_alloc();
          }
        mode = 1;
        continue;
        }
      else
        {
        mode = 0;
        }
      if (strcmp(buff,".") == 0){loop_flg = false; break;}
//      if (cmd != NULL)
//        {
        //fprintf(log_fp,"%s,%s%s\n",cmd,sv_str->c_str(),w->c_str()); sv_str->set("");
        str = new sChar(sv_str->c_str());
        sv_str->set("");
        str->cat(w->c_str());
        it->alloc_ptr = str;
        rcv_msg->mem_alloc();
//        }
//      line++;
      }
    if (loop_flg == false){printf("complete.         \n"); break;}
    if ((len = net_fp->netEvent()) == -1){fprintf(log_fp,"#error\n"); printf("socket error.         "); ok_flg = false; return(false);}
    ssc->Main(net_fp->getRcvBuff());
    }
  return(true);
  }

