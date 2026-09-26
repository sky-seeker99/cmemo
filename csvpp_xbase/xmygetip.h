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

#ifndef _XMYGETIP_H_
#define _XMYGETIP_H_

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b)) 
#endif

#include <stdio.h>
#include <vcl.h>
#include <winsock2.h>
#include <ws2tcpip.h>

//  interface class -----------------
class getMyAdr_C{
  private:
    AnsiString getIp;
    AnsiString getName;
    bool okFlg;

  public:
    getMyAdr_C();
    bool okCheck(){return(okFlg);}
    char *ipAdr(){return(getIp.c_str());}
    char *hostName(){return(getName.c_str());}
};

#endif

