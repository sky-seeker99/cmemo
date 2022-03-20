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
// file information class
//

#ifndef _XFILEINFO_H_
#define _XFILEINFO_H_



class fileinfo_C{
  public:
    int c_Year;
    int c_Month;
    int c_Day;
    int c_Hour;
    int c_Minute;
    int c_Second;
    int c_Milliseconds;
    
    int a_Year;
    int a_Month;
    int a_Day;
    int a_Hour;
    int a_Minute;
    int a_Second;
    int a_Milliseconds;
    
    int r_Year;
    int r_Month;
    int r_Day;
    int r_Hour;
    int r_Minute;
    int r_Second;
    int r_Milliseconds;

    bool Get(char *file);
};  

#endif // _XFILEINFO_H_
