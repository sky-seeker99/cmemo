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
#include "zsjis_anl.h"


// # Jis-code checker -----------------------------------
bool sjisAnl_C::check(char *str,int ptr){
  unsigned char w1;
  unsigned char w2;
  if (flag == true){flag = false;return(true);}
  w1 = (unsigned char)*(str+ptr);
  w2 = (unsigned char)*(str+ptr+1);
  if   (((w1 >= 0x81) && (w1 <= 0x9f)) || ((w1 >= 0xe0) && (w1 <= 0xfe))){
    if (((w2 >= 0x40) && (w2 <= 0x7e)) || ((w2 >= 0x80) && (w2 <= 0xfc))){flag = true;}
  }
  return(flag);
}



