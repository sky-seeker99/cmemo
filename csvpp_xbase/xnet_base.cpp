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
//  Winsock Control
//

#include "xnet_base.h"

// # Constructor/Destructor #####
NetworkInf_T::NetworkInf_T(LPCSTR lpServerName,int portNo){
  errorKbn = ERR_NONE;
  timeout  = 10000;
  if (WSAStartup(WINSOCK_VERSION,&wsaData)){
    // winsock.dll error
    WSACleanup();
    errorKbn = ERR_DLL;
    return;
    }

  if (wsaData.wVersion != WINSOCK_VERSION){
    // version error
    WSACleanup();
    errorKbn = ERR_VER;
    return;
    }

  lpHostEntry = gethostbyname(lpServerName);
  if (lpHostEntry == NULL){
    // host entry error
    WSACleanup();
    errorKbn = ERR_ENTRY;
    return;
    }

  sa.sin_family = AF_INET;
  sa.sin_addr   = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
  sa.sin_port   = htons(portNo);
  
  Socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if (Socket == INVALID_SOCKET){
    // socket error
    WSACleanup();
    errorKbn = ERR_SOCKET;
    return;
    }

  hEvent = WSACreateEvent();
  if (hEvent == WSA_INVALID_EVENT){
    // event error
    closesocket(Socket);
    WSACleanup();
    errorKbn = ERR_EVENT;
    return;
    }

  if (WSAEventSelect(Socket,hEvent,FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE) == SOCKET_ERROR)
    {
    // event select
    closesocket(Socket);
    WSACloseEvent(hEvent);
    WSACleanup();
    errorKbn = ERR_SELECT;
    return;
    }

  if (connect(Socket,(LPSOCKADDR)&sa,sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
    {
    if (WSAGetLastError() == WSAEWOULDBLOCK){errorKbn = ERR_BLOCK;  }
    else
      {
      errorKbn = ERR_CONNECT;
      closesocket(Socket);
      WSACloseEvent(hEvent);
      WSACleanup();
      return;
      }
    }

  if (netEvent() != -1)
    {
    if (events.lNetworkEvents & FD_CONNECT)
      {
      errorKbn = ERR_NONE;
      }
    else
      {
      errorKbn = ERR_CONNECT;
      closesocket(Socket);
      WSACloseEvent(hEvent);
      WSACleanup();
      return;
      }
    }
  else
    {
    errorKbn = ERR_CONNECT;
    closesocket(Socket);
    WSACloseEvent(hEvent);
    WSACleanup();
    return;
    }
  closeFlg = false;
  }

NetworkInf_T::~NetworkInf_T(){
  if (errorKbn == 0){
    closesocket(Socket);
    WSACloseEvent(hEvent);
    WSACleanup();
    }
  }

// # Network Event #####
// output:event.lNetworkEvents
int NetworkInf_T::netEvent(){
  int len;
  rcvBuffer[0] = 0x00;
  len = 0;
  if (closeFlg){return(-1);}
  WSAWaitForMultipleEvents(1,&hEvent,false,timeout,false);
  if (WSAEnumNetworkEvents(Socket,hEvent,&events) == SOCKET_ERROR){return(-1);}
  if (events.lNetworkEvents & FD_READ){
    if ((len = recv(Socket,rcvBuffer,sizeof(rcvBuffer),0)) == SOCKET_ERROR){return(-1);}
    *(rcvBuffer+len) = 0x00;
    }
  if (events.lNetworkEvents & FD_CLOSE){closeFlg = true;}
  return(len);
  }

// # Send #####
bool NetworkInf_T::netSend(char *buff){
  int len = strlen(buff);
  int sentSize;

  for(;;)
    {
    if (len <= 0){break;}
    sentSize = send(Socket,buff,len,0);
    if (sentSize < 0){sentSize = 0;}
    len  -= sentSize;
    buff += sentSize;
    }
  return(true);
  }



