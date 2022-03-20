/*

 * Copyright (c) 2010 Shigeru Kasuya (sky_seeker99@users.sourceforge.jp)
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

#ifndef _ZBIN_CONV_H_
#define _ZBIN_CONV_H_
#include <string.h>
#include "zschar.h"
#include "ztool.h"

// # string class ----------------
class binConv_C {
  public:
    sChar *w;
    sChar *rtn_cd;
    char *conv(char *in);
    binConv_C(){
      w      = new sChar("");
      rtn_cd = new sChar("");
    }
    ~binConv_C(){
      delete w;
      delete rtn_cd;
    }
};

#endif // _ZBIN_CONV_H_
