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
// Wild Card
//

#ifndef _XWILDCARD_H_
#define _XWILDCARD_H_

//#include "xstr.h"
#include "ztoken.h"
#include "zlist.h"

// interface class -----------------
class WildCard_T{
  private:
  sChar  *all_str;
  sChar  *top_str;
  sChar  *end_str;
  MEM_FP *mid_fp;
  MemHeader_T *it;
  sCharGrep   *grep;

  public:
  WildCard_T() {
    it = new MemHeader_T;
    mid_fp = new MEM_FP(it);
    all_str = new sChar("");
    top_str = new sChar("");
    end_str = new sChar("");
    grep    = new sCharGrep("");
    }


  ~WildCard_T(){
    sChar *str;
    delete all_str;
    delete top_str;
    delete end_str;
    delete grep;
    MEM_LOOP(str,sChar,mid_fp) delete str; LOOP_END
    delete mid_fp;
    delete it;
    }

    void set(char *p_str);
    bool cmp(char *p_str);
    void pr();

};


#endif // _XWILDCARD_H_

