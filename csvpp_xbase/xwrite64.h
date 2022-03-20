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
// 64bit filepointer write class
//

//#define W_BUFF_TEST 

#ifndef _XWRITE64_H_
#define _XWRITE64_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <condefs.h>
#include "xarea.h"
#include "zsjis_anl.h"

// binary file interface class -----------------
class File64Write_C{
  private:
    unsigned char *g_buff;
    int     g_buff_ptr;
    int     g_buff_size;
    __int64 g_total_ptr;

  public:
    File64Write_C(int size);
    ~File64Write_C();
    bool write_c(HANDLE out_fp,unsigned char c);
    void flush  (HANDLE out_fp);
    bool write  (HANDLE out_fp,unsigned char *buff,int len);
    void seek   (HANDLE out_fp,__int64 offset);
};

// text file interface -------------------------
class File64TextWrite_C{
  private:
    File64Write_C *wb;
    bool  ret0dFlg;
    bool  ret0aFlg;
    bool  conFlg;

  public:
    File64TextWrite_C(int bin_buff_size);
    ~File64TextWrite_C();    
    void write  (HANDLE out_fp,unsigned char *buff);
    void writeln(HANDLE out_fp,unsigned char *buff);
    void flush  (HANDLE out_fp);
    void seek   (HANDLE out_fp,__int64 offset){wb->seek(out_fp,offset);}
    void winRetSet ();
    void unixRetSet();
    void macRetSet ();
    void conSet(){conFlg=true;}
};


// interface -----------------------------------
class Write64_C{
  private:
    HANDLE out_fp;
    bool okFlg;
    File64Write_C *wb;
    File64TextWrite_C *wt;
    VariableArea_C *sv_mode;    
    void csvWrite_sub(unsigned char *str,bool gawa);
  public:
    unsigned char buff[4096];
    Write64_C(char *out_file,char *mode,int buff_size);  // mode = "wt" "wb" "at" "con"
    ~Write64_C();
    bool okCheck(){return(okFlg);}
    void write   (unsigned char *buff,int len);  // for binary file
    void write   (unsigned char *buff);          // for text file
    void writeln (unsigned char *buff);          // for text file
    void write   (void){write(buff);}            // for text file
    void writeln (void){writeln(buff);}          // for text file
    void write   (char *buff){unsigned char *w=buff; write  (w);} // for text file
    void writeln (char *buff){unsigned char *w=buff; writeln(w);} // for text file
    void csvWrite(unsigned char *str){csvWrite_sub(str,true);}    // for csv file
    void csvWrite_non_gawa(unsigned char *str){csvWrite_sub(str,false);}    // for csv file
    void seek    (__int64 offset);
    void flush();
    void winRetSet();
    void unixRetSet();
    void macRetSet();
};

#endif // _XWRITE64_H_

