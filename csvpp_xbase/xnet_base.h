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
//  Winsock Control Header file
//

#ifndef _XNET_BASE_H_
#define _XNET_BASE_H_

#include <winsock2.h>


// # Error kind #####
#define ERR_NONE    0
#define ERR_DLL     1
#define ERR_VER     2
#define ERR_ENTRY   3
#define ERR_SOCKET  4
#define ERR_EVENT   5
#define ERR_SELECT  6
#define ERR_BLOCK   7
#define ERR_CONNECT 8

// # Network Interface (ni) #####
class NetworkInf_T{
  public:
  int              errorKbn;
  WSADATA          wsaData;
  LPHOSTENT        lpHostEntry;
  SOCKADDR_IN      sa;
  SOCKET           Socket;
  WSAEVENT         hEvent;
  WSANETWORKEVENTS events;
  char             rcvBuffer[1024];
  bool             closeFlg;
  int              timeout;

  NetworkInf_T(LPCSTR lpServerName,int portNo);
  ~NetworkInf_T();
  char *getRcvBuff(){return(rcvBuffer);}
  int  netEvent();
  bool netSend(char *buff);
  int  errorChk(){return(errorKbn);}
  bool closeChk(){return(closeFlg);}
  void timeoutSet(int val){timeout = val;}
  
  };

#endif // _XNET_BASE_H_
