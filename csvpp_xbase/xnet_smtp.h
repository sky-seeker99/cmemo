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
