
#include "xmygetip.h"

getMyAdr_C::getMyAdr_C(){
  WSADATA data;
  char hostname[NI_MAXHOST + 1];
  char ip[max(NI_MAXHOST, INET6_ADDRSTRLEN) + 1];
  wchar_t w_getIp[INET6_ADDRSTRLEN+1];
  wchar_t w_getName[NI_MAXHOST+1];
  addrinfo  in_adr;
  addrinfo *out_adr = NULL;

  hostname[0] = 0x00;
  ip[0] = 0x00;
  okFlg = false;

  const DWORD version = MAKEWORD(2,0);
  if(WSAStartup(version, &data)){return;}

  if(gethostname(hostname, sizeof(hostname))){WSACleanup(); return;}

  ZeroMemory(&in_adr, sizeof(in_adr));
  in_adr.ai_family = PF_INET;
  in_adr.ai_flags = AI_CANONNAME;
  if(getaddrinfo(hostname, 0, &in_adr, &out_adr)){WSACleanup(); return;}
  if(getnameinfo(out_adr->ai_addr, (int)out_adr->ai_addrlen,ip,sizeof(ip),0,0,NI_NUMERICHOST) == 0){
    MultiByteToWideChar(CP_ACP, 0, ip, -1, w_getIp, INET6_ADDRSTRLEN + 1);
    getIp = w_getIp;
  }

  if(getnameinfo(out_adr->ai_addr, (int)out_adr->ai_addrlen,ip,sizeof(ip),0,0,NI_NAMEREQD) == 0){
    MultiByteToWideChar(CP_ACP, 0, ip, -1, w_getName, NI_MAXHOST + 1);
    getName = w_getName;
  }

  freeaddrinfo(out_adr);
  WSACleanup();
  okFlg = true;
}

