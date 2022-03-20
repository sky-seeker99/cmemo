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
// host file library
//

#ifndef _XFTP_H_
#define _XFTP_H_

#include <stdio.h>
#include <condefs.h>
#include <direct.h>
#include <wininet.h>

#include "zschar.h"
#include "zlist.h"
#include "xwildcard.h"
#include "zfile.h"


// # ftp callback #####
extern unsigned long callback_length;
void CALLBACK InternetStatusCallback(
        IN HINTERNET hInternet,
        IN DWORD Context,
        IN DWORD Status,
        IN LPVOID StatusInfo,
        IN DWORD StatusInfoSize);

// # ftp base session #####
class ftpBase_T{
public:
  HINTERNET hSession;
  HINTERNET hService;

  ftpBase_T(char *host,char *id,char *pass,int port); // Ver 0.92
  ~ftpBase_T();
  void timeoutSet(int p_timeout){
    DWORD TimeOut;
    TimeOut = p_timeout * 1000;
    //InternetSetOption( hSession, INTERNET_OPTION_RECEIVE_TIMEOUT,  &TimeOut, sizeof(TimeOut) );
    // Ver 0.95a

    printf("timeout set = %d ms\n", TimeOut);
    InternetSetOption( hSession, INTERNET_OPTION_CONNECT_TIMEOUT        ,  &TimeOut, sizeof(TimeOut) );    // インターネットへの接続のタイムアウト時間を設定する。
    InternetSetOption( hSession, INTERNET_OPTION_SEND_TIMEOUT           ,  &TimeOut, sizeof(TimeOut) );    // 非データの送信要求のタイムアウト時間。
    InternetSetOption( hSession, INTERNET_OPTION_RECEIVE_TIMEOUT        ,  &TimeOut, sizeof(TimeOut) );    // 非データの受信要求のタイムアウト時間。
    InternetSetOption( hSession, INTERNET_OPTION_DATA_SEND_TIMEOUT      ,  &TimeOut, sizeof(TimeOut) );    // データの送信要求のタイムアウト時間。
    InternetSetOption( hSession, INTERNET_OPTION_DATA_RECEIVE_TIMEOUT   ,  &TimeOut, sizeof(TimeOut) );    // データの受信要求のタイムアウト時間。
    }


  void retrySet(int p_retry_cnt,int p_retry_interval){
    DWORD InterVal;
    DWORD RetryCnt;

    InterVal = p_retry_interval * 1000;
    RetryCnt = p_retry_cnt;
    // Ver 0.96

    printf("retry set count = %d retry interval = %d ms\n", RetryCnt,InterVal);
    InternetSetOption( hSession, INTERNET_OPTION_CONNECT_RETRIES        ,  &RetryCnt, sizeof(RetryCnt) );    // インターネットへの接続のリトライ回数。
    InternetSetOption( hSession, INTERNET_OPTION_CONNECT_BACKOFF        ,  &InterVal, sizeof(InterVal) );    // リトライ時の待機時間。
    }


  };

// # remote file list cell #####
class ftpFileGetCell_T{
public:
  sChar   *name;
  DWORD    sizeH;
  DWORD    sizeL;  
  DWORD    attr;
  FILETIME tim;

  ftpFileGetCell_T(WIN32_FIND_DATA file,bool mode);
  ~ftpFileGetCell_T();
  };

// # remote file list interface #####
class ftpFileGet_T{
private:
  MemHeader_T *it;          // Mem Interface
  MEM_FP      *mem_fp;      // File list all (ftpFileGetCell_T)
  MEM_FP      *file_fp;     // File only list (sChar)
  MEM_FP      *dir_fp;      // Directory only list (sChar) 
  ftpBase_T   *ftp_fp;      // ftp handler
  WildCard_T  *wc;          // wild card interface
  bool         file_get;    // File list get ?
  bool         dir_get;     // Directory list get ?

public:
  void InfClear();
  ftpFileGet_T(MemHeader_T *p_it,ftpBase_T *ftp_fp);
  ~ftpFileGet_T();
  MEM_FP *main(char *path_name,bool mode);
  MEM_FP *fileListGet(char *mask);
  MEM_FP *dirListGet ();
  };

// # remote file control interface #####
class ftpControl_T{
private:
  DWORD        type;        // FTP_TRANSFER_TYPE_ASCII / FTP_TRANSFER_TYPE_BINARY
  sChar       *host_dir;    // host side directory
  sChar       *serv_dir;    // server side directory
  sChar       *w_host;      // full path host file name
  sChar       *w_serv;      // full path server file name
  ftpBase_T   *ftp_fp;      // ftp handler
  FILE        *log_fp;      // file pointer
  bool         timecon;     // time stamp     Ver 0.95

public:
       ftpControl_T(ftpBase_T *ftp_fp,FILE *log_fp);
      ~ftpControl_T();
  void setTransAscMode(){type = FTP_TRANSFER_TYPE_ASCII;}
  void setTransBinMode(){type = FTP_TRANSFER_TYPE_BINARY;}
  void setHostDir (char *d){host_dir->set(d); if (*(d+strlen(d)-1) != '\\'){host_dir->cat("\\");}}
  void setServDir (char *d){serv_dir->set(d); if (*(d+strlen(d)-1) != '/' ){serv_dir->cat("/"); }}
  char *getHostDir (){return(host_dir->c_str());}
  char *getServDir (){return(serv_dir->c_str());}
  void  timeconSet(){timecon=true;}     // Ver 0.95

  void hostMakeDir(char *d)
    {
    bool flg; // Ver 0.92
    w_host->set(host_dir->c_str());
    w_host->cat(d);
    fprintf(log_fp,"#log,host_make_dir,%s,%s,",host_dir->c_str(),d);  // Ver 0.92
    flg = CreateDir (w_host->c_str());                  // Ver 0.92

    AnsiString Time_str = TimeToStr(Time());  AnsiString Date_str = DateToStr(Date());
    if (timecon){fprintf(log_fp,"%s,%s,",Date_str.c_str(),Time_str.c_str());} // Ver 0.95

    if (flg){fprintf(log_fp,"OK\n");}  // Ver 0.92
    else    {fprintf(log_fp,"NG\n");}  // Ver 0.92
    }
  void hostDelDir (char *d)
    {
    char *str;
    int dd;
    char cmd[256];
    str = host_dir->c_str();
    if (*(str+1) == ':'){
      dd = 0;
      if ((*str >= 'A') && (*str <= 'Z')){dd = *str - 'A';}
      if ((*str >= 'a') && (*str <= 'z')){dd = *str - 'a';}
      setdisk(dd);
      SetCurrentDir(str+2);
      }
    else{
      SetCurrentDir(str);
      }

    fprintf(log_fp,"#log,host_del_dir,%s,%s,",host_dir->c_str(),d);  // Ver 0.92
    sprintf(cmd,"rmdir %s",d);
    int flg = system(cmd);

    AnsiString Time_str = TimeToStr(Time());  AnsiString Date_str = DateToStr(Date());
    if (timecon){fprintf(log_fp,"%s,%s,",Date_str.c_str(),Time_str.c_str());} // Ver 0.95

    if (flg == 0){fprintf(log_fp,"OK\n");}  // Ver 0.92
    else         {fprintf(log_fp,"NG\n");}  // Ver 0.92
    }
  void hostDelFile(char *d)
    {
    bool flg;   // Ver 0.92
    w_host->set(host_dir->c_str());
    w_host->cat(d);

    fprintf(log_fp,"#log,host_del_file,%s,%s,",host_dir->c_str(),d);  // Ver 0.92
    flg = DeleteFile(w_host->c_str()); // Ver 0.92

    AnsiString Time_str = TimeToStr(Time());  AnsiString Date_str = DateToStr(Date());
    if (timecon){fprintf(log_fp,"%s,%s,",Date_str.c_str(),Time_str.c_str());} // Ver 0.95

    if (flg){fprintf(log_fp,"OK\n");}  // Ver 0.92
    else    {fprintf(log_fp,"NG\n");}  // Ver 0.92
    }
  void servDelDir (char *d)
    {
    fprintf(log_fp,"#log,serv_del_dir,%s,%s,",serv_dir->c_str(),d);  // Ver 0.92
    bool flg = FtpSetCurrentDirectory(ftp_fp->hService,serv_dir->c_str());
    flg = FtpRemoveDirectory(ftp_fp->hService, d);

    AnsiString Time_str = TimeToStr(Time());  AnsiString Date_str = DateToStr(Date());
    if (timecon){fprintf(log_fp,"%s,%s,",Date_str.c_str(),Time_str.c_str());} // Ver 0.95

    if (flg){fprintf(log_fp,"OK\n");}  // Ver 0.92
    else    {fprintf(log_fp,"NG\n");}  // Ver 0.92
    }
  void servMakeDir(char *d)
    {
    bool flg;   // Ver 0.92
    fprintf(log_fp,"#log,serv_make_dir,%s,%s,",serv_dir->c_str(),d);  // Ver 0.92
    w_serv->set(serv_dir->c_str());
    w_serv->cat(d);
    flg = FtpCreateDirectory(ftp_fp->hService, w_serv->c_str());

    AnsiString Time_str = TimeToStr(Time());  AnsiString Date_str = DateToStr(Date());
    if (timecon){fprintf(log_fp,"%s,%s,",Date_str.c_str(),Time_str.c_str());} // Ver 0.95

    if (flg){fprintf(log_fp,"OK\n");}  // Ver 0.92
    else    {fprintf(log_fp,"NG\n");}  // Ver 0.92
    }
  void servDelFile(char *d)
    {
    bool flg;   // Ver 0.92
    fprintf(log_fp,"#log,serv_del_file,%s,%s,",serv_dir->c_str(),d);  // Ver 0.92
    w_serv->set(serv_dir->c_str());
    w_serv->cat(d);
    flg = FtpDeleteFile(ftp_fp->hService, w_serv->c_str());

    AnsiString Time_str = TimeToStr(Time());  AnsiString Date_str = DateToStr(Date());
    if (timecon){fprintf(log_fp,"%s,%s,",Date_str.c_str(),Time_str.c_str());} // Ver 0.95

    if (flg){fprintf(log_fp,"OK\n");}  // Ver 0.92
    else    {fprintf(log_fp,"NG\n");}  // Ver 0.92
    }
  void downLoad   (char *file);
  void upLoad     (char *file);
  void upLoad     (char *file,char *srv_file);


  // Ver 0.94
  void hostRenameFile(char *old_name,char *new_name)
    {
    bool flg;
//    sChar *str_new;
//    sChar *str_old;
//    str_new = new sChar(host_dir->c_str()); str_new->cat(new_name);
//    str_old = new sChar(host_dir->c_str()); str_new->cat(old_name);

    SetCurrentDir(host_dir->c_str());

    fprintf(log_fp,"#log,host_rename_file,%s,%s,%s,",host_dir->c_str(),old_name,new_name);
//    flg = RenameFile(str_old->c_str(),str_new->c_str());
    flg = RenameFile(old_name,new_name);

    AnsiString Time_str = TimeToStr(Time());  AnsiString Date_str = DateToStr(Date());
    if (timecon){fprintf(log_fp,"%s,%s,",Date_str.c_str(),Time_str.c_str());} // Ver 0.95

    if (flg){fprintf(log_fp,"OK\n");}
    else    {fprintf(log_fp,"NG\n");}
//    delete str_new;
//    delete str_old;
    }

  // Ver 0.94
  void servRenameFile (char *old_name,char *new_name)
    {
    fprintf(log_fp,"#log,serv_rename_file,%s,%s,%s,",serv_dir->c_str(),old_name,new_name);
    bool flg = FtpSetCurrentDirectory(ftp_fp->hService,serv_dir->c_str());
    flg = FtpRenameFile(ftp_fp->hService, old_name,new_name);

    AnsiString Time_str = TimeToStr(Time());  AnsiString Date_str = DateToStr(Date());
    if (timecon){fprintf(log_fp,"%s,%s,",Date_str.c_str(),Time_str.c_str());} // Ver 0.95

    if (flg){fprintf(log_fp,"OK\n");}
    else    {fprintf(log_fp,"NG\n");}
    }

  };


// # remote file control interface #####
class ftpInf_T{
private:
  ftpControl_T *hFtp;
  ftpBase_T    *connect;
  MemHeader_T  *it;
  MEM_FP       *s_file_fp;
  MEM_FP       *u_file_fp;
  int           dir_cnt;
  FILE         *dir_fp;
  FILE         *file_fp;
  WildCard_T   *wc;          // wild card interface
  void dirDownLoad_inner(char *host,char *serv,char *dir );
  void dirUpLoad_inner  (char *host,char *serv,char *dir );
  void ListOutput_sub   (char *list_dir,char *dir,char *tag,char *tag2);
  bool          name_mode;  // スペース区切りでファイル名を区切る

public:
  ftpInf_T(char *host,char *id,char *pass,int port,FILE *log_fp);   // Ver 0.92
  ~ftpInf_T();
  void timeconSet(){hFtp->timeconSet();}
  bool connectCheck();
  void fileDownLoad(char *host,char *serv,char *mask);
  void fileUpLoad  (char *host,char *serv,char *mask);
  void fileUpLoad  (char *host,char *serv,char *mask,char *tmp); // Ver 0.94
  void fileDelServ (char *serv,char *mask           ); // Ver 0.92
  void fileDelHost (char *host,char *mask           ); // Ver 0.92
  void fileRenameServ (char *serv,char *old_name,char *new_name ); // Ver 0.94
  void fileRenameHost (char *host,char *old_name,char *new_name ); // Ver 0.94
  void dirDownLoad (char *host,char *serv,char *dir );
  void dirUpLoad   (char *host,char *serv,char *dir );
  void dirDelServ  (char *serv,char *dir            );
  void dirDelHost  (char *host,char *dir            );
  void ListOutput  (char *outFile_dir,char *outFile_file,char *list_dir);
  void dirMakeServ (char *serv,char *dir            ); // Ver 0.93
  void dirMakeHost (char *host,char *dir            ); // Ver 0.93
  void ascSet      (){hFtp->setTransAscMode();}
  void binSet      (){hFtp->setTransBinMode();}
  void timeoutSet  (int timeout){connect->timeoutSet(timeout);}
  void retrySet    (int cnt,int interval){connect->retrySet(cnt,interval);}
  void xmode_set   (){name_mode = true;}

  void file_fp_clear(); // Ver 0.97
  void s_file_fp_add(char *p_str); // Ver 0.97
  void u_file_fp_add(char *p_str); // Ver 0.97
  bool file_mask_check(char *file); // Ver 0.97
  };

#endif // _XFTP_H_
