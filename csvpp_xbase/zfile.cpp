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


#include "zfile.h"

// # Recurcive File Control Class ----------------
FileCtl_B::FileCtl_B(char *p_file,int p_ptr,int p_line,int p_kbn){
  file = new sChar(p_file);
  ptr  = p_ptr;
  kbn  = p_kbn;
  line = p_line;
  }
FileCtl_B::FileCtl_B(char *p_file,int p_ptr,int p_line){
  file = new sChar(p_file);
  ptr  = p_ptr;
  kbn  = 0;
  line = p_line;
  }
FileCtl_B::~FileCtl_B(){
  delete file;
  }

FileCtl_T::FileCtl_T(){
  it     = new MemHeader_T();
  mem_fp = new MEM_FP(it);
  }
FileCtl_T::~FileCtl_T(){
  FileCtl_B   *fb;
  MEM_LOOP(fb,FileCtl_B,mem_fp)
  delete fb;
  LOOP_END
  delete mem_fp;
  delete it;
  }
char *FileCtl_T::Name_Get(){
  FileCtl_B   *fb;
  mem_fp->mem_mcb_end_set();
  if (mem_fp->mem_mcb_ptr_rd() == MM_NULL){return((char *)MM_NULL);}
  fb = (FileCtl_B *)mem_fp->mem_link_ptr_rd();
  return(fb->File_Get());
  }
int FileCtl_T::Ptr_Get(){
  FileCtl_B   *fb;
  mem_fp->mem_mcb_end_set();
  if (mem_fp->mem_mcb_ptr_rd() == MM_NULL){return(0);}
  fb = (FileCtl_B *)mem_fp->mem_link_ptr_rd();
  return(fb->Get());
  }
int FileCtl_T::Kbn_Get(){
  FileCtl_B   *fb;
  mem_fp->mem_mcb_end_set();
  if (mem_fp->mem_mcb_ptr_rd() == MM_NULL){return(0);}
  fb = (FileCtl_B *)mem_fp->mem_link_ptr_rd();
  return(fb->Kbn_Get());
  }
int FileCtl_T::Line_Get(){
  FileCtl_B   *fb;
  mem_fp->mem_mcb_end_set();
  if (mem_fp->mem_mcb_ptr_rd() == MM_NULL){return(0);}
  fb = (FileCtl_B *)mem_fp->mem_link_ptr_rd();
  return(fb->Line_Get());
  }
void FileCtl_T::Push(char *file,int ptr,int line){
  FileCtl_B   *fb;
  fb = new FileCtl_B(file,ptr,line);
  it->alloc_ptr = (unsigned char *)fb;
  mem_fp->mem_alloc();
  }
void FileCtl_T::Push(char *file,int ptr,int line,int kbn){
  FileCtl_B   *fb;
  fb = new FileCtl_B(file,ptr,line,kbn);
  it->alloc_ptr = (unsigned char *)fb;
  mem_fp->mem_alloc();
  }
void FileCtl_T::Pop(){
  FileCtl_B   *fb;
  mem_fp->mem_mcb_end_set();
  if (mem_fp->mem_mcb_ptr_rd() == MM_NULL){return;}
  fb = (FileCtl_B *)mem_fp->mem_link_ptr_rd();
  delete fb;
  mem_fp->mem_del();
  }

// # Sequence File Class ------------------------
FileSeq_B::FileSeq_B(sChar *p_file_name,sChar *p_ext,char *p_key){
  char buff[MAX_PATH];
  FileSeq(buff,p_file_name->c_str(),p_ext->c_str());
  file_name = new sChar(buff);
  key       = new sChar(p_key);
  }
FileSeq_B::~FileSeq_B(){
  delete file_name;
  delete key;
  }
char *FileSeq_B::GetFileName(){return(file_name->c_str());}
char *FileSeq_B::GetKey     (){return(key->c_str());}

// # Sequence File Class ------------------------
FileSeq_T::FileSeq_T(char *p_file_name,char *p_ext){
  file_name = new sChar(p_file_name);
  ext       = new sChar(p_ext);
  m         = new MemHeader_T();
  mem_fp    = new MEM_FP(m);
  }
FileSeq_T::~FileSeq_T(){
  FileSeq_B *fsb;
  delete file_name;
  delete ext;
  delete m;
  MEM_LOOP(fsb,FileSeq_B,mem_fp)
    DeleteFile(fsb->GetFileName());
    delete fsb;
  LOOP_END
  delete mem_fp;
  }
char *FileSeq_T::SeqSet(char *key){
  FileSeq_B *fsb;
  fsb          = new FileSeq_B(file_name,ext,key);
  m->alloc_ptr = (unsigned char *)fsb;
  m->srch_key  = key;
  mem_fp->mem_srch_alloc();
  return(fsb->GetFileName());
  }
char *FileSeq_T::SeqGet(char *key){
  FileSeq_B *fsb;
  m->srch_key = key;
  if (mem_fp->mem_srch() == 0)
    {
    fsb = (FileSeq_B *)mem_fp->mem_link_ptr_rd();
    return(fsb->GetFileName());
    }
  return((char *)MM_NULL);
  }
MEM_FP *FileSeq_T::MemFpGet(){return(mem_fp);}

// # Sequence File No --------------------
void FileSeq(char *out_file,char *file_name,char *ext){
  int i;
  FILE *fp;

  for(i=0;;++i)
    {
    sprintf(out_file,"%s%d.%s",file_name,i,ext);
    if (FileExist(out_file) == 0){continue;}
    fp = fopen(out_file,"wt");
    fclose(fp);
    break;
    }
  }

// # File Tools   ---------------------------
int  FileExist(char *file_name){
//  FILE *fp;

  if (file_name == (char *)MM_NULL){return(-1);}
  if (FileExists(file_name) == false) {return(-1);}

//  if ((fp = fopen(file_name,"rt")) == NULL) {return(-1);}
//  fclose(fp);
  return(0);
  }

void  FileCat(char *des,char *src){
  FILE *out_fp;
  FILE *in_fp;
  char  buff[2048];

  if ((in_fp  = fopen(src,"rt")) == NULL) {return;}
  if ((out_fp = fopen(des,"at")) == NULL) {fclose(in_fp); return;}

  for(;;)
    {
    if (fgets(buff,2048-1,in_fp) == 0) {break;}
    fprintf(out_fp,"%s",buff);
    }

  fclose(in_fp);
  fclose(out_fp);
  }

void  FileNull(char *des){
  FILE *out_fp;

  if ((out_fp = fopen(des,"wt")) == NULL) {return;}
  fclose(out_fp);
  }


// # Super Length Line File Read --------------------
xFgets_T::xFgets_T(int len) {
  buff       = NULL;
  end_flg    = false;
  xFgets_Len = len;
  }
xFgets_T::~xFgets_T(){
  if (buff != NULL){delete [] buff;}
  }
char *xFgets_T::xFgets(FILE *fp)  // =NULL:End Of File
  {
  char *tmp;
  char *sv;
  unsigned int   len;
  unsigned int   i;
  int   j;
  if (end_flg == true){return(NULL);}
  if (buff != NULL){delete [] buff;}
  len     = xFgets_Len;
  buff    = new char [len+1];
  buff[0] = 0x00;
  tmp = new char[xFgets_Len];
  for(;;)
    {
    if (fgets(tmp,xFgets_Len-1,fp) == 0) {end_flg = true; break;}
    if (strlen(buff) + strlen(tmp) + 5 > len)
      {
      len = strlen(buff) + strlen(tmp) + 5;
      sv = new char [strlen(buff)+1];
      strcpy(sv,buff);
      delete [] buff;
      buff = new char[len];
      strcpy(buff,sv);
      delete [] sv;
      }
    j = strlen(buff);
    for(i=0;i<strlen(tmp);++i)
      {
      *(buff + j + i    ) = *(tmp + i);
      *(buff + j + i + 1) = 0x00;
      }
    if (*(tmp+strlen(tmp)-1) == 0x0a){break;}
    }
  delete [] tmp;
  return(buff);
  }

// # File Information Class ----------------
FileGetCell_T::FileGetCell_T(long p_size,unsigned long p_attr,unsigned short p_time,unsigned short p_date,char *p_name){
  size = p_size;
  attr = p_attr;
  time = p_time;
  date = p_date;
  name = new sChar(p_name);
  }
FileGetCell_T::~FileGetCell_T(){
  delete name;
  }

// # File Information Class ----------------
void FileGet_T::InfClear(){
  FileGetCell_T *fgc;
  MEM_LOOP(fgc,FileGetCell_T,mem_fp) delete fgc; LOOP_END
  mem_fp->mem_del_all();
  }
FileGet_T::FileGet_T(MemHeader_T *p_it){
  it     = p_it;
  mem_fp = new MEM_FP(it);
  }
FileGet_T::~FileGet_T(){
  InfClear();
  delete mem_fp;
  }
void FileGet_T::get(char *path_name,int attr){
  struct ffblk ffblk;
  FileGetCell_T *fgc;       // File Information
  InfClear();
  int done;
  done = findfirst(path_name,&ffblk,attr);
  while (!done){
    fgc = new FileGetCell_T(ffblk.ff_fsize,ffblk.ff_attrib,ffblk.ff_ftime,ffblk.ff_fdate,ffblk.ff_name);
    it->alloc_ptr = (MM_PTR_T *)fgc;
    mem_fp->mem_alloc();
    done = findnext(&ffblk);
    }
  }

// # File Handle Class ---------------------
FileHandleCell_T::FileHandleCell_T(char *p_name,char *p_ext,char *p_mode){
  int i;
  mode = new sChar(p_mode);
  name = new sChar("");
  fp = NULL;
  clm = 0;
  cnt = 1;
  if ((strcmp(mode->c_str(),"wt") == 0)
   || (strcmp(mode->c_str(),"at") == 0))
    {
    name->set(p_name);
    if (*p_ext != 0x00)
      {
      name->cat(".");
      name->cat(p_ext);
      }
    fp = fopen(name->c_str(),mode->c_str());
    }
  if (strcmp(mode->c_str(),"uniq") == 0)
    {
    for(i=0;;++i)
      {
      name->set(p_name);
      name->intcat(i);
      if (*p_ext != 0x00)
        {
        name->cat(".");
        name->cat(p_ext);
        }
      if (FileExist(name->c_str()) == 0){continue;}
      fp = fopen(name->c_str(),"wt");
      break;
      }
    }
  if (fp == NULL){delete name; delete mode;}
  }
FileHandleCell_T::~FileHandleCell_T(){
  if (fp == NULL){return;}
  fclose(fp);
  delete name;
  delete mode;
  }
void FileHandleCell_T::fwrite(char *str)
  {
  int i;
  for(i=0;i<clm;++i){fprintf(fp," ");}
  fprintf(fp,"%s",str);
  }

void FileHandleCell_T::fwriteln(char *str)
  {
  fwrite(str);
  fprintf(fp,"\n");
  }


FileHandle_T::FileHandle_T(MemHeader_T *p_it){
  it     = p_it;
  mem_fp = new MEM_FP(it);
  }
FileHandle_T::~FileHandle_T(){
  FileHandleCell_T *fhc;
  MEM_LOOP(fhc,FileHandleCell_T,mem_fp) delete fhc; LOOP_END
  delete mem_fp;
  }
void FileHandle_T::Fclose(char *name){
  FileHandleCell_T *fhc;
  it->srch_key = name;
  if (mem_fp->mem_srch() == 0)
    {
    fhc = (FileHandleCell_T *)mem_fp->mem_link_ptr_rd();
    delete fhc;
    mem_fp->mem_del();
    }
  }
char *FileHandle_T::Fopen(char *file,char *ext,char *mode){
  FileHandleCell_T *fhc;
  fhc = new FileHandleCell_T(file,ext,mode);
  if (fhc->fp == NULL){delete fhc; return(NULL);}
  Fclose(fhc->name->c_str());
  it->alloc_ptr = (unsigned char *)fhc;
  mem_fp->mem_srch_alloc();
  return(fhc->name->c_str());
  }
void FileHandle_T::Fwrite(char *name,char *str,bool rtn_mode){
  FileHandleCell_T *fhc;
  it->srch_key = name;
  if (mem_fp->mem_srch() == 0)
	{
	fhc = (FileHandleCell_T *)mem_fp->mem_link_ptr_rd();
	if (rtn_mode) {
      fhc->fwriteln(str);
	}
	else {
	  fhc->fwrite(str);
	}
    }
  }
void FileHandle_T::ClmPlus(char *name)
  {
  FileHandleCell_T *fhc;
  it->srch_key = name;
  if (mem_fp->mem_srch() == 0)
    {
    fhc = (FileHandleCell_T *)mem_fp->mem_link_ptr_rd();
    fhc->clm += fhc->cnt;
    }
  }
void FileHandle_T::ClmMinus(char *name)
  {
  FileHandleCell_T *fhc;
  it->srch_key = name;
  if (mem_fp->mem_srch() == 0)
    {
    fhc = (FileHandleCell_T *)mem_fp->mem_link_ptr_rd();
    if (fhc->clm >= 1){fhc->clm -= fhc->cnt;}
    if (fhc->clm <  0){fhc->clm = 0;}
    }
  }
void FileHandle_T::ClmSet(char *name,int p_clm)
  {
  FileHandleCell_T *fhc;
  it->srch_key = name;
  if (mem_fp->mem_srch() == 0)
    {
    fhc = (FileHandleCell_T *)mem_fp->mem_link_ptr_rd();
    fhc->clm = p_clm;
    }
  }
void FileHandle_T::CntSet(char *name,int p_cnt)
  {
  FileHandleCell_T *fhc;
  it->srch_key = name;
  if (mem_fp->mem_srch() == 0)
    {
    fhc = (FileHandleCell_T *)mem_fp->mem_link_ptr_rd();
    fhc->cnt = p_cnt;
    }
  }

// # File Temporary Class ---------------------
FileTemp_T::FileTemp_T(char *p_ext){
  int i;
  sChar *ww;

  env  = getenv("TEMP");
  name = new sChar(env);
  if (*(env + strlen(env) - 1) != '\\'){name->cat("\\sk_temp");}
  else                                 {name->cat("sk_temp"  );}
  ww = new sChar("");
  for(i=0;;++i)
    {
    ww->set(name->c_str());
    ww->intcat(i);
    ww->cat(".");
    ww->cat(p_ext);
    if (FileExist(ww->c_str()) == 0){continue;}
    fp = fopen(ww->c_str(),"wt");
    name->set(ww->c_str());
    break;
    }
  delete ww;
  }

FileTemp_T::~FileTemp_T(){
  if (fp != NULL){fclose(fp);}
  DeleteFile(name->c_str());
  delete name;
  }

void  FileTemp_T::TempToRename(char *p_name){
  RenameFile(name->c_str(),p_name);
  }

/*
// # File Name Separate Class ------------------
FileNameSep_T::FileNameSep_T(){
  drv       = new sChar("");
  dir       = new sChar("");
  file      = new sChar("");
  ext       = new sChar("");
  }

FileNameSep_T::~FileNameSep_T(){
  delete drv;
  delete dir;
  delete file;
  delete ext;
  }

void FileNameSep_T::Set(char *str){
  int i,len;
  int drv_ptr;
  int dir_ptr;
  int ext_ptr;
  JisInf_T *sj;

  len = strlen(str);
  sj  = new JisInf_T;

  drv_ptr = -1;
  dir_ptr = -1;
  ext_ptr = -1;
  for(i=0;i<len;++i)
    {
    if (sj->SJisCheck(str,i) == true){continue;}
    if (*(str+i) == ':' ){drv_ptr = i;}
    if (*(str+i) == '\\'){dir_ptr = i;}
    if (*(str+i) == '.' ){ext_ptr = i;}
    }
  // drive -------
  if (drv_ptr == -1){drv->set("");             }
  else              {SetSub(drv,str,0,drv_ptr);   }
  // directory -------
  if (dir_ptr == -1){dir->set("");                  }
  else              {SetSub(dir,str,drv_ptr+1,dir_ptr);}
  // ext -------
  if (ext_ptr == -1){ext->set(""); ext_ptr = len; }
  else              {SetSub(ext,str,ext_ptr+1,len-1);}
  // file --------
  SetSub(file,str,dir_ptr+1,ext_ptr-1);

  delete sj;
  }

void FileNameSep_T::SetSub(sChar *s_str,char *str,int st,int en){
  char *w;
  int i;

  if (en < st){s_str->set(""); return;}
  w = new char[strlen(str)+1];
  for(i=0;i<en-st+1;++i)
    {
    *(w+i) = *(str+i+st);
    *(w+i+1) = 0x00;
    }
  s_str->set(w);  
  delete [] w;
  }
*/
