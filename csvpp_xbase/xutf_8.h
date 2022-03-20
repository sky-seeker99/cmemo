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
// Variable length Area class
//

#ifndef _XUTF_8_H_
#define _XUTF_8_H_


class Utf8Area_C{

  public:
    wchar_t *w_str;
    int w_str_size;
    char *utf_str;
    int utf_str_size;
  
    Utf8Area_C(int size) {
      w_str = new wchar_t[size+1];
      w_str_size = size;
      utf_str = new char [size+1];
      utf_str_size = size;
    }
    
    ~Utf8Area_C(){
      delete [] w_str;
      delete [] utf_str;
    }
    char *toUtf_8(char *buff);
    char *fromUtf_8(char *buff);

  private:  
    void sizeCheck(int size,bool wstrFlg);

};


#endif // _XUTF_8_H_

