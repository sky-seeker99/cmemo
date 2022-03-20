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
