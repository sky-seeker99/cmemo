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
 
#ifndef _XFILE_INTERFACE_
#define _XFILE_INTERFACE_ 
 

 
#include "zfile.h" 
#include "zfilename_anl.h"

// # file-interface class (file_inf) ----------------
class file_inf{
  public:
    FILE   *out_fp;
    FILE   *in_fp;
    sChar *temp_name;
    sChar *in_file_name;
    bool   ok_flg;
    bool   in_con_flg;
    bool   out_con_flg;

  void file_list_get(MemHeader_T *it,MEM_FP *mem_fp,char *file_mask);
  bool same_file_open(char *file);
  void same_file_close();
  bool file_open(char *in_file,char *out_file);
  void file_close();
  };
  
#endif // _XFILE_INTERFACE_  
  
