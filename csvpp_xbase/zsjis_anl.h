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

#ifndef _ZSJIS_ANL_H_
#define _ZSJIS_ANL_H_


// # shift jis-code Check Class ----------------
class sjisAnl_C{
  public:
    bool flag; // Shift-Jis flg
    void init(){flag = false;}
    sjisAnl_C(){
      init();
    }
    bool check(char *str){return(check(str,0));}
    bool check(char *str,int ptr);

    // - ŒÝŠ· -----
    bool SJisCheck(char *str){return(check(str));}
    bool SJisCheck(char *str,int ptr){return(check(str,ptr));}

};

// - ŒÝŠ· ------
#define JisInf_T sjisAnl_C

#endif // _ZSJIS_ANL_H_

