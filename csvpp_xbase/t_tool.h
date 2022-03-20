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
//  TOOLKIT LIBRARY
//

#ifndef _T_TOOL_H_
#define _T_TOOL_H_

// テンプレート版 ----- 
template <class T> bool convStrNumCheck(char *str);
template <class T> T convStrNum     (char *str);
template <class T> T convStrNumSub  (char *str);
template <class T> T convStrNumSub_2(char *str);
template <class T> T hexc_long_t    (char *str,int len);
template <class T> T bitc_long_t    (char *str,int len);
template <class T> T decc_long_t    (char *str,int len);
double doubleConv(char *str);



#endif // _T_TOOL_H_
