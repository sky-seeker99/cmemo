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
// 64bit filepointer read class
//     

#ifndef _XREAD64_H_
#define _XREAD64_H_

#include <condefs.h>
#include "xarea.h"

#ifndef LOOP_END
#define LOOP_END }
#endif

#define TEXT_READ_LOOP(FP,STR) for(;;){ \
                                 char *STR = FP->read(); \
                                 if (STR == NULL){break;}



//#define R_BUFF_TEST

// binary file interface class -----------------
class File64Read_C{
  private:
    unsigned char *g_buff;
    unsigned long  g_buff_ptr;
    int            g_buff_size;
    unsigned long  g_read_size;
    __int64        g_total_ptr;

  public:
    File64Read_C(int size);
    ~File64Read_C();
    bool read_c(HANDLE in_fp,unsigned char *c);
    int  read  (HANDLE in_fp,unsigned char *buff,int len);
    void seek  (HANDLE in_fp,__int64 offset);
    __int64 tell(){return(g_total_ptr);}
    __int64 sizeGet(HANDLE in_fp);
};


// text file interface -------------------------
class File64TextRead_C{
  private:
    File64Read_C *rb;
    bool  ret0dFlg;
    bool  ret0aFlg;
    VariableArea_C *g_buff;
    __int64 g_total_ptr;
    
    unsigned char sv;
    bool svFlg;
    __int64 sv_total_ptr;

  public:
    File64TextRead_C(int bin_buff_size,int txt_buff_size);
    ~File64TextRead_C();
    char *read(HANDLE in_fp);
    char *csv_read(HANDLE in_fp);
    void  seek(HANDLE in_fp,__int64 offset);
    __int64 sizeGet(HANDLE in_fp);
    bool  return0dGet(){return(ret0dFlg);}
    bool  return0aGet(){return(ret0aFlg);}
    __int64 tell(){return(g_total_ptr);}
};



// interface -----------------------------------
class Read64_C{
  private:
    HANDLE in_fp;
    bool okFlg;
    File64Read_C *rb;
    File64TextRead_C *rt;
    bool csvFlg;
  public:
    Read64_C(char *file_name,char *mode,int buff_size); // mode = "rt" "rb" "csv"
    ~Read64_C();
    bool okCheck(){return(okFlg);}
    int  read(unsigned char *buff,int len);    // for binary file
    char *read();                              // for text file
    void seek(__int64 offset);
    __int64 tell();
    __int64 sizeGet();
    bool return0dGet();
    bool return0aGet();
    void csvModeSet(){csvFlg=true;}

};

#endif // _XREAD64_H_
