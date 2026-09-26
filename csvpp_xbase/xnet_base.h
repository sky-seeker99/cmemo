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
