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
//  FILE LIBRARY
//

#ifndef _XFILE_H_
#define _XFILE_H_

#include <stdio.h>
#include <condefs.h>
#include <dir.h>
#include "zlist.h"

// ProtoType Define
int  FileExist    (char *file_name);
void FileSeq(char *out_file,char *file_name,char *ext);

// # Recurcive File Control Class -----------------
class FileCtl_B{
  public:
    sChar *file;  // File Name
    int    ptr;   // File Pointer
    int    line;  // Line
    int    kbn;   // kbn shikibetu
  public:
    FileCtl_B(char *p_file,int p_ptr,int p_line,int p_kbn);
    FileCtl_B(char *p_file,int p_ptr,int p_line);
    ~FileCtl_B();
    int   Get();
    char *File_Get();
    int   Kbn_Get();
    int   Line_Get();
  };

class FileCtl_T{
  private:
    MemHeader_T *it;      // MEM_FP Header
    MEM_FP      *mem_fp;  // File List
  public:
   FileCtl_T();
   ~FileCtl_T();
   void Pop();
   void Push(char *file,int ptr,int line,int kbn);
   void Push(char *file,int ptr,int line);
   int Kbn_Get();
   int Ptr_Get();
   int Line_Get();
   char *Name_Get();
   };

// # Sequence File Class   ---------------------------
class FileSeq_B{
  public:
    sChar     *key;
    sChar     *file_name;
    FileSeq_B(sChar *p_file_name,sChar *p_ext,char *p_key);
    ~FileSeq_B();
    char     *GetFileName();
    char     *GetKey     ();
  };

class FileSeq_T{
  public:
    sChar       *file_name;
    sChar       *ext;
    MEM_FP      *mem_fp;
    MemHeader_T *m;
    FileSeq_T   (char *p_file_name,char *p_ext);
    ~FileSeq_T  ();
    char *SeqSet(char *key);
    char *SeqGet(char *key);
    MEM_FP *MemFpGet();
  };

// # File Tool --------------------------
void  FileSeq(char *out_file,char *file_name,char *ext);
int   FileExist(char *file_name);
void  FileCat(char *des,char *src);
void  FileNull(char *des);

// # Super Length Line File Read -------------
class xFgets_T{
  public:
  char *buff;       // Buffer
  bool  end_flg;    // End Flg
  int   xFgets_Len; // Default Length
  xFgets_T(int len);
  ~xFgets_T();
  char *xFgets(FILE *fp);  // =NULL:End Of File
  };

// # File Information Class --------------
class FileGetCell_T{
  public:
  long           size;	// ファイルサイズ
  unsigned long  attr;	// 見つかった属性
  unsigned short time;	// ファイル時刻
  unsigned short date;	// ファイル日付
  sChar          *name; // 見つかったファイル名
  FileGetCell_T(long p_size,unsigned long p_attr,unsigned short p_time,unsigned short p_date,char *p_name);
  ~FileGetCell_T();
  };

class FileGet_T{
  public:
  MemHeader_T *it;          // Mem Interface
  MEM_FP      *mem_fp;      // File List
  void InfClear();
  FileGet_T(MemHeader_T *p_it);
  ~FileGet_T();
  void get(char *path_name,int attr);
  };

// # File Handle Class ------------------
class FileHandleCell_T{
  public:
  sChar *name;
  sChar *mode;
  FILE  *fp;
  int    clm;
  int    cnt;
  FileHandleCell_T(char *p_name,char *p_ext,char *p_mode);
  ~FileHandleCell_T();
  void fwrite(char *str);
  void fwriteln(char *str);
  };

class FileHandle_T{
  public:
  MemHeader_T *it;          // Mem Interface
  MEM_FP      *mem_fp;      // File List
  FileHandle_T(MemHeader_T *p_it);
  ~FileHandle_T();
  void ClmPlus(char *name);
  void ClmMinus(char *name);
  void ClmSet(char *name,int p_clm);
  void CntSet(char *name,int p_cnt);
  void Fclose(char *name);
  char *Fopen(char *file,char *ext,char *mode);
  void Fwrite(char *name,char *str,bool rtn_mode);
  };

// # Temporary Class ------------------
class FileTemp_T{
  public:
  sChar *name;
  char  *env;
  FILE  *fp;
  FileTemp_T(char *ext);
  ~FileTemp_T();
  void close(){if(fp != NULL){fclose(fp); fp=NULL;}}
  void TempToRename(char *name);
  char *GetName(){return(name->c_str());}
  FILE *GetFp(){return(fp);}
  };


// # File Name Separate Class ------------------
/*
class FileNameSep_T{
  public:
  sChar *full_path;
  sChar *drv;
  sChar *dir;
  sChar *file;
  sChar *ext;
  FileNameSep_T();
  ~FileNameSep_T();
  void Set(char *path);
  void SetSub(sChar *s_str,char *str,int st,int en);
  char *DrvGet (){return(drv->c_str ());}
  char *DirGet (){return(dir->c_str ());}
  char *FileGet(){return(file->c_str());}
  char *ExtGet (){return(ext->c_str ());}
  };
*/
#endif // _XFILE_H_

