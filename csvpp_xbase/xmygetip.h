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

