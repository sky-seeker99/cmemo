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
 
#ifndef _XFILE_INTERFACE64_
#define _XFILE_INTERFACE64_
 
#include "zfile.h" 
#include "zfilename_anl.h"
#include "xtemp_file64.h"
 
// # file-interface class (fileInf64_C) ----------------
class fileInf64_C{
  public:
    MemHeader_T  *it;
    MEM_FP       *mem_fp;
    Read64_C     *csv_fp;
    Read64_C     *in_fp;
    Write64_C    *out_fp;
    FileTemp64_T *temp;
    int           buff_size;
    bool          ok_flg;
    bool          in_con_flg;
    bool          out_con_flg;
    sChar         *sv_in_file;

    fileInf64_C(int p_buff_size){
      buff_size = p_buff_size * 1024;
      if (buff_size < 10240){buff_size = 10240;}
      it     = new MemHeader_T;
      mem_fp = new MEM_FP(it);
      out_fp = NULL;
      in_fp  = NULL;
      csv_fp = NULL;
      temp   = NULL;
      sv_in_file = new sChar("");
    }

   ~fileInf64_C(){
      sChar *ww;
      MEM_LOOP(ww,sChar,mem_fp) delete ww; LOOP_END
      if (out_fp != NULL){delete out_fp;}
      if (in_fp  != NULL){delete in_fp ;}
      if (csv_fp != NULL){delete csv_fp ;}
      if (temp   != NULL){delete temp  ;}
      delete sv_in_file;
      delete mem_fp;
      delete it;
   }
  
    void file_list_get(char *file_mask);
    bool same_file_open(char *file);
    bool same_file_open_bin(char *file);
    void same_file_close();
	bool file_open(char *in_file,char *out_file);
	bool file_open(char *p_in_file,char *p_in_mode,char *p_out_file,char *p_out_mode);
    bool csv_file_open(char *p_csv_file,char *p_csv_mode,char *p_in_file,char *p_in_mode);
	void file_close();
    void csv_file_close();
  };
  
#endif // _XFILE_INTERFACE64_  
  
