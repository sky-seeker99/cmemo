/*
 * Copyright 2026 Shigeru Kasuya
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by me or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
