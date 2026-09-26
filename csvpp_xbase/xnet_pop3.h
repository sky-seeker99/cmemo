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

//
//  Pop3 Control Header
//

#ifndef _XNET_POP3_H_
#define _XNET_POP3_H_

#include "xnet_base.h"
#include "ztoken.h"
#include "zcsvfile.h"


//#define POP_DEBUG   1

#define KBN_TIMEOUT 0
#define KBN_GET     1
#define KBN_DEL     2
#define KBN_LIST    3
#define KBN_GET_DEL 4

class pop3Inf_T{
  public:
  CsvInf_T *csv_fp;
  NetworkInf_T *net_fp;
  MEM_FP *rcv_msg;  // Ver 0.96
  MemHeader_T *it;  // Ver 0.96
  FILE *log_fp;
  bool ok_flg;
  char *buff;
  sChar *w;
  StrSrchInf_T *ssc;
  StrSrchInf_T *ssc2;
  char sndBuff[4096];
  char sndBuff2[4096]; // Ver 0.93
  char c_ret[3];   // Ver 0.92
  bool listGetFlg; // Ver 0.92
  int  listCnt;    // Ver 0.92
  sChar *sv_str;

  void net_rtn        (int kbn  );
  void netall_rtn     (int kbn  );
  int  mailCount      (void     );
  void mailRecieve    (int val  );
  void mailHeadRecieve(int val  );
  void mailDelete     (int val  );
  void timeout_rtn    (void     );
  int  okCheck        (char *cmd);
  int recieve(int len,char *cmd);
  int strSnd(char *str,char *okStr);
  int strSnd(char *str,char *ss,char *okStr);
  int strSnd(char *str,int   ss,char *okStr);

  // Ver 0.96
  bool recieve_ex (int len);
  bool mailRecieve_ex(int val);
  bool mailDelete_ex (int val);


  ~pop3Inf_T();
  pop3Inf_T(char *csv_file,char *log_file,char *host,char *id,char *pass);

  };

#endif // _XNET_POP3_H_
