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


#include "xftp.h"

unsigned long callback_length;


// ---------------------
// # ftp callback
// ---------------------
void CALLBACK InternetStatusCallback(
  IN HINTERNET hInternet,
  IN DWORD Context,
  IN DWORD Status,
  IN LPVOID StatusInfo,
  IN DWORD StatusInfoSize){

  LPDWORD lpdwByte;
  switch(Status) {
    case INTERNET_STATUS_CONNECTING_TO_SERVER:printf("CONNECTING TO %s\n", (char*)StatusInfo); break;
    case INTERNET_STATUS_CONNECTED_TO_SERVER :printf("CONNECTED TO %s\n" , (char*)StatusInfo); break;
    case INTERNET_STATUS_RESPONSE_RECEIVED   :lpdwByte = (LPDWORD)StatusInfo; callback_length += *lpdwByte; printf("%dbyte\r",callback_length); break;
    case INTERNET_STATUS_CONNECTION_CLOSED   :printf("CONNECTION CLOSED\n");                   break;

    }
  }

// ---------------------
// # ftp base session
// ---------------------
ftpBase_T::ftpBase_T(char *host,char *id,char *pass,int port){   // Ver 0.92
  hSession = NULL;
  hService = NULL;
  hSession = InternetOpen( "csvftp", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );
  if(!hSession ){return;}
  InternetSetStatusCallback(hSession, InternetStatusCallback);
//  hService = InternetConnect( hSession,host,21,id,pass,INTERNET_SERVICE_FTP,0,0);
  hService = InternetConnect( hSession,host,port,id,pass,INTERNET_SERVICE_FTP,0,0);  // Ver 0.92
  printf("Connection Information (Host:%s / ID:%s / Port:%d)\n",host,id,port);
  }

ftpBase_T::~ftpBase_T(){
  if (hService){InternetCloseHandle(hService);}
  if (hSession){InternetCloseHandle(hSession);}
  }


// ------------------------
// # remote file list get
// ------------------------
// # remote file list cell #####
ftpFileGetCell_T::ftpFileGetCell_T(WIN32_FIND_DATA file,bool mode){
  int   i,len;
  char *str;
  char *x_str;
  attr  = file.dwFileAttributes;
  tim   = file.ftLastWriteTime;
  sizeH = file.nFileSizeHigh;
  sizeL = file.nFileSizeLow;

  len = strlen(file.cFileName);
  str = file.cFileName;
  x_str = str;

  if (mode) {
    for(i=0;i<len;++i,++str)
      {
      if (*str == ' '){x_str = str+1;}
      }
    }   

  name  = new sChar(x_str);
  }

ftpFileGetCell_T::~ftpFileGetCell_T(){
  delete name;
  }

// # remote file list interface #####
ftpFileGet_T::ftpFileGet_T(MemHeader_T *p_it,ftpBase_T *p_ftp_fp){
  it = p_it;
  mem_fp  = new MEM_FP(it);
  file_fp = new MEM_FP(it);
  dir_fp  = new MEM_FP(it);
  ftp_fp  = p_ftp_fp;
  wc      = new WildCard_T;
  file_get = false;
  dir_get  = false;
  }

void ftpFileGet_T::InfClear(){
  ftpFileGetCell_T *ffgc;
  sChar            *str;
  file_get = false;
  dir_get  = false;
  MEM_LOOP(ffgc,ftpFileGetCell_T,mem_fp ) delete ffgc; LOOP_END
  MEM_LOOP(str ,sChar           ,file_fp) delete str;  LOOP_END
  MEM_LOOP(str ,sChar           ,dir_fp ) delete str;  LOOP_END
  mem_fp->mem_del_all();
  file_fp->mem_del_all();
  dir_fp->mem_del_all();
  }

ftpFileGet_T::~ftpFileGet_T(){
  InfClear();
  delete mem_fp;
  delete file_fp;
  delete dir_fp;
  delete wc;
  }

MEM_FP *ftpFileGet_T::main(char *path_name,bool mode){
  sChar           *str;
  WIN32_FIND_DATA  finddata;
  HINTERNET        hFtpEnum;

  if(!ftp_fp->hService ){return(mem_fp);}
  bool flg = FtpSetCurrentDirectory(ftp_fp->hService, path_name);
  if (flg == false){return(mem_fp);}
  hFtpEnum = FtpFindFirstFile(ftp_fp->hService, NULL, &finddata, INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, 0 );
  InfClear();
  if( !hFtpEnum ){InternetCloseHandle(hFtpEnum); return(mem_fp);}

  do{
    it->alloc_ptr = (MM_PTR_T *) new ftpFileGetCell_T(finddata,mode);
    it->srch_key  = finddata.cFileName;
    mem_fp->mem_srch_alloc();
    }while(InternetFindNextFile(hFtpEnum, &finddata));

  InternetCloseHandle(hFtpEnum);
  return(mem_fp);
  }

MEM_FP *ftpFileGet_T::fileListGet(char *mask){
  if (file_get == true){return(file_fp);}

  ftpFileGetCell_T *ffgc;
  wc->set(mask);
  MEM_LOOP(ffgc,ftpFileGetCell_T,mem_fp)
    if (ffgc->attr & FILE_ATTRIBUTE_DIRECTORY) {continue;}
    if (wc->cmp(ffgc->name->c_str()) == false){continue;}
    it->alloc_ptr = (MM_PTR_T *)new sChar(ffgc->name->c_str());
    file_fp->mem_alloc();
  LOOP_END
  file_get = true;

  return(file_fp);
  }

MEM_FP *ftpFileGet_T::dirListGet(){
  if (dir_get == true){return(dir_fp);}

  ftpFileGetCell_T *ffgc;
  MEM_LOOP(ffgc,ftpFileGetCell_T,mem_fp)
    if ((ffgc->attr & FILE_ATTRIBUTE_DIRECTORY) == 0) {continue;}
    it->alloc_ptr = (MM_PTR_T *)new sChar(ffgc->name->c_str());
    dir_fp->mem_alloc();
  LOOP_END
  dir_get = true;

  return(dir_fp);
  }

// -------------------------
// # remote file controller 
// -------------------------
ftpControl_T::ftpControl_T(ftpBase_T *p_ftp_fp,FILE *p_log_fp){   // Ver 0.92
  type     = FTP_TRANSFER_TYPE_ASCII;
  ftp_fp   = p_ftp_fp;
  host_dir = new sChar("");
  serv_dir = new sChar("");
  w_host   = new sChar("");
  w_serv   = new sChar("");
  log_fp   = p_log_fp;       // Ver 0.92
  timecon  = false;          // Ver 0.95
  }

ftpControl_T::~ftpControl_T(){
  delete host_dir;
  delete serv_dir;
  delete w_host;
  delete w_serv;
  }

void ftpControl_T::downLoad(char *file){
  BOOL flg;
  if(!ftp_fp->hService ){return;}
  w_host->set(host_dir->c_str());
  w_host->cat(file);
  w_serv->set(serv_dir->c_str());
  w_serv->cat(file);

  fprintf(log_fp,"#log,ftp_get_file,%s,%s,",w_serv->c_str(), w_host->c_str());  // Ver 0.92
  flg = FtpGetFile(ftp_fp->hService, w_serv->c_str(), w_host->c_str(), false, 0, type, 3000 ); // Ver 0.92

  AnsiString Time_str = TimeToStr(Time());  AnsiString Date_str = DateToStr(Date());
  if (timecon){fprintf(log_fp,"%s,%s,",Date_str.c_str(),Time_str.c_str());} // Ver 0.95

  if (flg){fprintf(log_fp,"OK\n");}  // Ver 0.92
  else    {fprintf(log_fp,"NG\n");}  // Ver 0.92
  }

void ftpControl_T::upLoad(char *file){
  BOOL flg;
  if(!ftp_fp->hService ){return;}
  w_host->set(host_dir->c_str());
  w_host->cat(file);
  w_serv->set(serv_dir->c_str());
  w_serv->cat(file);

  fprintf(log_fp,"#log,ftp_put_file,%s,%s,",w_host->c_str(), w_serv->c_str());  // Ver 0.92
  flg = FtpPutFile(ftp_fp->hService, w_host->c_str(), w_serv->c_str(),type,0);   // Ver 0.92

  AnsiString Time_str = TimeToStr(Time());  AnsiString Date_str = DateToStr(Date());
  if (timecon){fprintf(log_fp,"%s,%s,",Date_str.c_str(),Time_str.c_str());} // Ver 0.95

  if (flg){fprintf(log_fp,"OK\n");}  // Ver 0.92
  else    {fprintf(log_fp,"NG\n");}  // Ver 0.92
  }


// Ver 0.94
void ftpControl_T::upLoad(char *file,char *srv_file){
  BOOL flg;
  if(!ftp_fp->hService ){return;}
  w_host->set(host_dir->c_str());
  w_host->cat(file);
  w_serv->set(serv_dir->c_str());
  w_serv->cat(srv_file);

  fprintf(log_fp,"#log,ftp_put_file,%s,%s,",w_host->c_str(), w_serv->c_str());  // Ver 0.92
  flg = FtpPutFile(ftp_fp->hService, w_host->c_str(), w_serv->c_str(),type,0);   // Ver 0.92

  AnsiString Time_str = TimeToStr(Time());  AnsiString Date_str = DateToStr(Date());
  if (timecon){fprintf(log_fp,"%s,%s,",Date_str.c_str(),Time_str.c_str());} // Ver 0.95

  if (flg){fprintf(log_fp,"OK\n");}  // Ver 0.92
  else    {fprintf(log_fp,"NG\n");}  // Ver 0.92
  }

// -------------------------
// # remote file interface
// -------------------------
ftpInf_T::ftpInf_T(char *host,char *id,char *pass,int port,FILE *log_fp){   // Ver 0.92
  connect = new ftpBase_T(host,id,pass,port);  // Ver 0.92
  hFtp    = new ftpControl_T(connect,log_fp);
  it      = new MemHeader_T;
  s_file_fp = new MEM_FP(it);
  u_file_fp = new MEM_FP(it);
  name_mode = false;
  wc      = new WildCard_T;
  }

ftpInf_T::~ftpInf_T(){
  sChar *str;

  MEM_LOOP(str,sChar,s_file_fp) delete str; LOOP_END
  MEM_LOOP(str,sChar,u_file_fp) delete str; LOOP_END
  delete s_file_fp;
  delete u_file_fp;
  delete connect;
  delete hFtp;
  delete it;
  delete wc;
  }

// # Ver 0.97 #####
void ftpInf_T::file_fp_clear(){
  sChar *str;

  printf("sel mask / unsel mask clear\n");
  MEM_LOOP(str,sChar,s_file_fp) delete str; LOOP_END
  MEM_LOOP(str,sChar,u_file_fp) delete str; LOOP_END
  s_file_fp->mem_del_all();
  u_file_fp->mem_del_all();
  }

// # Ver 0.97 #####
void ftpInf_T::s_file_fp_add(char *p_str){
  printf("sel mask set (%s)\n",p_str);
  it->alloc_ptr = new sChar(p_str);
  s_file_fp->mem_alloc();
  }

void ftpInf_T::u_file_fp_add(char *p_str){
  printf("unsel mask set (%s)\n",p_str);
  it->alloc_ptr = new sChar(p_str);
  u_file_fp->mem_alloc();
  }

bool ftpInf_T::file_mask_check(char *file)
  {
  sChar *str;
  int hit=0;
  MEM_LOOP(str,sChar,s_file_fp)
    hit = -1;
    wc->set(str->c_str());
    if (wc->cmp(file))
      {
      hit = 1;
      break;
      }
  LOOP_END
  if (hit == -1){return(false);}

  hit = 0;
  MEM_LOOP(str,sChar,u_file_fp)
    hit = -1;
    wc->set(str->c_str());
    if (wc->cmp(file))
      {
      hit = 1;
      break;
      }
  LOOP_END
  if (hit == 1){return(false);}
  return(true);
  }


bool ftpInf_T::connectCheck(){
  if (connect->hService){return(true);}
  return(false);
  }

void ftpInf_T::fileDownLoad(char *host,char *serv,char *mask){
  MEM_FP *mem_fp;
  sChar *str;

  ftpFileGet_T *fl;
  fl = new ftpFileGet_T(it,connect);
  fl->main(serv,name_mode);
  mem_fp = fl->fileListGet(mask);
  hFtp->setHostDir(host);
  hFtp->setServDir(serv);
  MEM_LOOP(str,sChar,mem_fp)
    if (file_mask_check(str->c_str()) == false){continue;}  // Ver 0.97

    printf("%s\n",str->c_str());
    callback_length = 0;
    hFtp->downLoad(str->c_str());
    printf("complete.               \n");
  LOOP_END
  delete fl;
  }

void ftpInf_T::fileUpLoad  (char *host,char *serv,char *mask){
  FileGet_T *fg;
  FileGetCell_T *fgc;
  sChar *w;
  fg = new FileGet_T(it);

  hFtp->setHostDir(host);
  hFtp->setServDir(serv);
  w = new sChar(hFtp->getHostDir());
  w->cat(mask);
  fg->get(w->c_str(),0);
  MEM_LOOP(fgc,FileGetCell_T,fg->mem_fp)
    if (fgc->attr & FA_DIREC){continue;}
    if (fgc->attr & FA_LABEL){continue;}

    if (file_mask_check(fgc->name->c_str()) == false){continue;}  // Ver 0.97

    printf("%s\n",fgc->name->c_str());
    callback_length = 0;
    hFtp->upLoad(fgc->name->c_str());
    printf("complete.               \n");
  LOOP_END

  delete w;
  delete fg;
  }

// Ver 0.94
void ftpInf_T::fileUpLoad(char *host,char *serv,char *mask,char *tmp){
  FileGet_T *fg;
  FileGetCell_T *fgc;
  sChar *w;
  fg = new FileGet_T(it);

  hFtp->setHostDir(host);
  hFtp->setServDir(serv);
  w = new sChar(hFtp->getHostDir());
  w->cat(mask);
  fg->get(w->c_str(),0);

  MEM_LOOP(fgc,FileGetCell_T,fg->mem_fp)
    if (fgc->attr & FA_DIREC){continue;}
    if (fgc->attr & FA_LABEL){continue;}
    callback_length = 0;
    w->set(fgc->name->c_str());
    w->cat(".");
    w->cat(tmp);
    printf("%s(%s)\n",fgc->name->c_str(),w->c_str());
    hFtp->upLoad(fgc->name->c_str(),w->c_str());
    printf("complete.               \n");
  LOOP_END

  MEM_LOOP(fgc,FileGetCell_T,fg->mem_fp)
    if (fgc->attr & FA_DIREC){continue;}
    if (fgc->attr & FA_LABEL){continue;}
    w->set(fgc->name->c_str());
    w->cat(".");
    w->cat(tmp);
    hFtp->servDelFile(fgc->name->c_str());
    hFtp->servRenameFile(w->c_str(),fgc->name->c_str());
  LOOP_END

  delete w;
  delete fg;
  }


void ftpInf_T::dirDownLoad(char *host,char *serv,char *dir ){
  dirDelHost(host,dir);
  dirDownLoad_inner(host,serv,dir);
  }

void ftpInf_T::dirDownLoad_inner(char *host,char *serv,char *dir ){
  MEM_FP *mem_fp;
  sChar *serv_str;
  sChar *host_str;
  sChar *str;
  ftpFileGet_T *fl;

  printf("[%s -> %s][%s]\n",serv,host,dir);

  // # directory set #####
  hFtp->setHostDir(host);
  hFtp->setServDir(serv);

  // # make directory #####
  hFtp->hostMakeDir(dir);

  // # dir catting #####
  host_str = new sChar(hFtp->getHostDir());
  host_str->cat(dir);
  serv_str = new sChar(hFtp->getServDir());
  serv_str->cat(dir);

  // # file down load #####
  fileDownLoad(host_str->c_str(),serv_str->c_str(),"*");

  // # dir get #####
  fl = new ftpFileGet_T(it,connect);
  fl->main(serv_str->c_str(),name_mode);
  mem_fp = fl->dirListGet();
  MEM_LOOP(str,sChar,mem_fp)
    if (strcmp(str->c_str(),".") == 0){continue;}
    if (strcmp(str->c_str(),"..") == 0){continue;}
    dirDownLoad_inner (host_str->c_str(),serv_str->c_str(),str->c_str());
  LOOP_END

  // # delete #####
  delete fl;
  delete serv_str;
  delete host_str;
  }

void ftpInf_T::dirUpLoad(char *host,char *serv,char *dir ){
  dirDelServ(serv,dir);
  dirUpLoad_inner(host,serv,dir );
  }

void ftpInf_T::dirUpLoad_inner(char *host,char *serv,char *dir ){
  MEM_FP *mem_fp;
  sChar *serv_str;
  sChar *host_str;
  sChar *w;
  sChar *str;
  FileGet_T *fg;
  FileGetCell_T *fgc;

  printf("[%s -> %s][%s]\n",host,serv,dir);

  // # directory set #####
  hFtp->setHostDir(host);
  hFtp->setServDir(serv);

  // # make directory #####
  hFtp->servMakeDir(dir);

  // # dir catting #####
  host_str = new sChar(hFtp->getHostDir());
  host_str->cat(dir);
  serv_str = new sChar(hFtp->getServDir());
  serv_str->cat(dir);

  // # file up load #####
  fileUpLoad(host_str->c_str(),serv_str->c_str(),"*");

  // # dir get #####
  w = new sChar(host_str->c_str());
  w->cat("\\*");
  fg = new FileGet_T(it);
  fg->get(w->c_str(),FA_DIREC);
  MEM_LOOP(fgc,FileGetCell_T,fg->mem_fp)
    if ((fgc->attr & FA_DIREC) == 0){continue;}
    if (fgc->attr & FA_LABEL){continue;}
    if (strcmp(fgc->name->c_str(),".") == 0){continue;}
    if (strcmp(fgc->name->c_str(),"..") == 0){continue;}
    dirUpLoad_inner(host_str->c_str(),serv_str->c_str(),fgc->name->c_str());
  LOOP_END

  // # delete #####
  delete fg;
  delete serv_str;
  delete host_str;
  }

void ftpInf_T::dirDelServ(char *serv,char *dir ){
  MEM_FP *mem_fp;
  sChar *serv_str;
  sChar *str;
  ftpFileGet_T *fl;

  printf("[%s][%s] delete\n",serv,dir);

  // # directory set #####
  hFtp->setServDir(serv);

  // # dir catting #####
  serv_str = new sChar(hFtp->getServDir());
  serv_str->cat(dir);

  // # dir get #####
  fl = new ftpFileGet_T(it,connect);
  fl->main(serv_str->c_str(),name_mode);
  mem_fp = fl->fileListGet("*");
  hFtp->setServDir(serv_str->c_str());
  MEM_LOOP(str,sChar,mem_fp)
    hFtp->servDelFile(str->c_str());
  LOOP_END

  mem_fp = fl->dirListGet();
  MEM_LOOP(str,sChar,mem_fp)
    if (strcmp(str->c_str(),".") == 0){continue;}
    if (strcmp(str->c_str(),"..") == 0){continue;}
    dirDelServ(serv_str->c_str(),str->c_str());
  LOOP_END

  hFtp->setServDir(serv);
  hFtp->servDelDir(dir);

  // # delete #####
  delete fl;
  delete serv_str;
  }


// # server file delete Ver 0.92
void ftpInf_T::fileDelServ(char *serv,char *mask ){
  MEM_FP *mem_fp;
  ftpFileGet_T *fl;
  sChar *str;

  // # dir get #####
  fl = new ftpFileGet_T(it,connect);
  fl->main(serv,name_mode);
  mem_fp = fl->fileListGet(mask);

  // # delete file #####
  hFtp->setServDir(serv);
  MEM_LOOP(str,sChar,mem_fp)
    hFtp->servDelFile(str->c_str());
  LOOP_END

  delete fl;
  }

// # host file delete Ver 0.92
void ftpInf_T::fileDelHost(char *host,char *mask ){
  FileGet_T *fg;
  FileGetCell_T *fgc;
  sChar *w;

  // # dir get #####
  fg = new FileGet_T(it);
  w = new sChar(host);
  if (*(host+strlen(host)-1) != '\\'){w->cat("\\");}
  w->cat(mask);

  fg->get(w->c_str(),faReadOnly | faHidden | faSysFile | faArchive);

  // # delete file #####
  hFtp->setHostDir(host);
  MEM_LOOP(fgc,FileGetCell_T,fg->mem_fp)
    hFtp->hostDelFile(fgc->name->c_str());
  LOOP_END

  delete w;
  delete fg;
  }

void ftpInf_T::dirDelHost(char *host,char *dir ){
  MEM_FP *mem_fp;
  sChar *host_str;
  sChar *str;
  sChar *w;
  FileGet_T *fg;
  FileGetCell_T *fgc;

  printf("[%s][%s] delete\n",host,dir);


  fg = new FileGet_T(it);

  // # directory set #####
  hFtp->setHostDir(host);

  // # dir catting #####
  host_str = new sChar(hFtp->getHostDir());
  host_str->cat(dir);

  // # dir get #####
  w = new sChar(host_str->c_str());
  w->cat("\\*");
  fg->get(w->c_str(),faReadOnly | faHidden | faSysFile | faArchive);
  hFtp->setHostDir(host_str->c_str());
  MEM_LOOP(fgc,FileGetCell_T,fg->mem_fp)
    hFtp->hostDelFile(fgc->name->c_str());
//    printf("%s delete.\n",fgc->name->c_str());
  LOOP_END
  fg->InfClear();
  fg->get(w->c_str(),FA_DIREC);
  MEM_LOOP(fgc,FileGetCell_T,fg->mem_fp)
    if ((fgc->attr & FA_DIREC) == 0){continue;}
    if (fgc->attr & FA_LABEL){continue;}
    if (strcmp(fgc->name->c_str(),".") == 0){continue;}
    if (strcmp(fgc->name->c_str(),"..") == 0){continue;}
    dirDelHost(host_str->c_str(),fgc->name->c_str());
  LOOP_END


  hFtp->setHostDir(host);
  hFtp->hostDelDir(dir);

  // # delete #####
  delete fg;
  delete host_str;
  delete w;
  }

void ftpInf_T::ListOutput(char *list_dir,char *outFile_dir,char *outFile_file){
  dir_fp  = fopen(outFile_dir ,"wt");
  file_fp = fopen(outFile_file,"wt");
  dir_cnt = 0;
  if ((dir_fp != NULL) && (file_fp != NULL))
    {
    ListOutput_sub(list_dir,"[top]","","");
    }

  if (dir_fp  != NULL){fclose(dir_fp );}
  else                {printf("open error.(file=%s)\n",outFile_dir);}
  if (file_fp != NULL){fclose(file_fp);}
  else                {printf("open error.(file=%s)\n",outFile_file);}
  }

void ftpInf_T::ListOutput_sub(char *dir,char *dir2,char *p_tag,char *p_tag2){
  ftpFileGet_T *fl;
  sChar *serv_str;
  ftpFileGetCell_T *file_cell;
  SYSTEMTIME t;
  MEM_FP *mem_fp;
  sChar *str;
  sChar *sv;
  sChar *tag;
  sChar *tag2;

  printf("[%s] listing\n",dir);

  // # directory set #####
  hFtp->setServDir(dir);

  // # dir catting #####
  serv_str = new sChar(hFtp->getServDir());

  // # file ouput #####
  fl = new ftpFileGet_T(it,connect);
  mem_fp = fl->main(serv_str->c_str(),name_mode);
  bool first = true;
  MEM_LOOP(file_cell,ftpFileGetCell_T,mem_fp)
    if (file_cell->attr & FILE_ATTRIBUTE_DIRECTORY) {continue;}
    FileTimeToSystemTime( &file_cell->tim,&t );
    if (first){first = false; fprintf(file_fp,"#file,%d",dir_cnt);}
    else      {               fprintf(file_fp,"#file,");}
    fprintf(file_fp,",%s",file_cell->name->c_str());
    fprintf(file_fp,",%d",file_cell->sizeH);
    fprintf(file_fp,",%d",file_cell->sizeL);
    fprintf(file_fp,",%4d/%02d/%02d",t.wYear,t.wMonth,t.wDay);
    fprintf(file_fp,",%2d:%02d:%02d",t.wHour,t.wMinute,t.wSecond);
    fprintf(file_fp,",%x",file_cell->attr);
    fprintf(file_fp,",%s",serv_str->c_str());
    if (*p_tag == 0x00)
      {
      fprintf(file_fp,",");
      }
    else
      {
      fprintf(file_fp,",%s",dir2);
      }
    fprintf(file_fp,"\n");
  LOOP_END

  // # dir output #####
  if (*p_tag == 0x00)
    {
    fprintf(dir_fp,"#root,%s,%d",dir,dir_cnt);
    fprintf(dir_fp,",root-%s",dir);
    }
  else
    {
    fprintf(dir_fp,"#dir,%s,%d",dir2,dir_cnt);
    fprintf(dir_fp,"%s,%s",p_tag,dir2);
    }
  fprintf(dir_fp,"\n");

  // # dir conut #####
  ++dir_cnt;
  tag = new sChar("");
  tag2 = new sChar("");

  // # dir recursive #####
  sv = new sChar(hFtp->getServDir());
  mem_fp = fl->dirListGet();

  int cnt = 0;
  MEM_LOOP(str,sChar,mem_fp)
    if (strcmp(str->c_str(),".") == 0){continue;}
    if (strcmp(str->c_str(),"..") == 0){continue;}
    ++cnt;
  LOOP_END

  int i=0;
  MEM_LOOP(str,sChar,mem_fp)
    if (strcmp(str->c_str(),".") == 0){continue;}
    if (strcmp(str->c_str(),"..") == 0){continue;}
    tag->set(p_tag2);
    tag->cat(",");
    tag2->set(p_tag2);
    tag2->cat(",");
    if (i == cnt-1)
      {
      tag->cat("„¯");
      tag2->cat("");
      }
    else
      {
      tag->cat("„°");
      tag2->cat("„«");
      }

    serv_str->set(sv->c_str());
    serv_str->cat(str->c_str());
    ListOutput_sub(serv_str->c_str(),str->c_str(),tag->c_str(),tag2->c_str());
    ++i;
  LOOP_END

  delete tag;
  delete tag2;
  delete fl;
  delete sv;
  delete serv_str;
  }


void ftpInf_T::dirMakeServ(char *serv,char *dir )
  {
  // # string separate #####
  StrSrchInf_T *ssc = new StrSrchInf_T;
  ssc->Set('/');
  ssc->Main(dir);

  // # directory set #####
  hFtp->setServDir(serv);

  // # make directory #####
  sChar *cur_dir = new sChar(serv);
  for(int i=0;i<ssc->GetSu();i++)
    {
    sChar *str = ssc->Get(i);
    hFtp->servMakeDir(str->c_str());
    char *w = cur_dir->c_str();
    if (*(w+strlen(w)-1) != '/'){cur_dir->cat("/");}
    cur_dir->cat(str->c_str());
    hFtp->setServDir(cur_dir->c_str());
    }

  delete cur_dir;
  delete ssc;
  }

void ftpInf_T::dirMakeHost(char *host,char *dir )
  {
  // # string separate #####
  StrSrchInf_T *ssc = new StrSrchInf_T;
  ssc->Set('\\');
  ssc->Main(dir);

  // # directory set #####
  hFtp->setHostDir(host);

  // # make directory #####
  sChar *cur_dir = new sChar(host);
  for(int i=0;i<ssc->GetSu();i++)
    {
    sChar *str = ssc->Get(i);
    hFtp->hostMakeDir(str->c_str());
    char *w = cur_dir->c_str();
    if (*(w+strlen(w)-1) != '\\'){cur_dir->cat("\\");}
    cur_dir->cat(str->c_str());
    hFtp->setHostDir(cur_dir->c_str());
    }

  delete cur_dir;
  delete ssc;
  }


// # server file rename Ver 0.94
void ftpInf_T::fileRenameServ(char *serv,char *old_name,char *new_name ){
  hFtp->setServDir(serv);
  hFtp->servRenameFile(old_name,new_name);
  }

// # host file rename Ver 0.94
void ftpInf_T::fileRenameHost(char *host,char *old_name,char *new_name ){
  hFtp->setHostDir(host);
  hFtp->hostRenameFile(old_name,new_name);
  }

