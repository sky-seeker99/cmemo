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
// link information class
//

#ifndef _XLINK_H_
#define _XLINK_H_


class link_C{
  public:
    void            *g_psl;
    void            *g_ppf;
    HRESULT          hRes;
    bool             okFlg;
    WIN32_FIND_DATA  *wfd;
    char             dest_link[MAX_PATH];
    bool             linkCheckFlg;

    link_C();
    ~link_C();

    char *link_C::Get(char *file);
    bool link_C::dirHantei();
};  

#endif // _XLINK_H_

