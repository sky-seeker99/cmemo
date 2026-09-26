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

#ifndef _XNET_SMTP_H_
#define _XNET_SMTP_H_

#include "xnet_base.h"
#include "ztoken.h"
#include "zcsvfile.h"


//#define SMTP_DEBUG 1

//
//  Smtp Control Header
//
class smtpInf_T{
  public:
  NetworkInf_T *net_fp;
  FILE *in_fp;
  FILE *log_fp;
  bool ok_flg;
  char sndBuff[4096];
  char sndBuff2[4096];
  char buff[4096];
  int  line_cnt;
  char c_ret[3]; // Ver 0.91

  void wr_rtn(char *str);
  void mail_from_rtn(char *str);
  void rcpt_to_rtn(char *str);
  void timeout_rtn(char *str);
  bool okCheck(char *cmd,char *ok_str);
  int  strSnd(char *str,char *ss,char *okw,char *okStr);
  int  strSnd(char *str,char *okw,char *okStr);
  ~smtpInf_T();
  smtpInf_T(char *csv_file,char *log_file,char *host);
  };

#endif // _XNET_SMTP_H_
