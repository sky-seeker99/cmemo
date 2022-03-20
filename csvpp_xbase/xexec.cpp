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
//  Program Exec
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xexec.h"


void PgExec_C::dirSet(char *p){
  FileNameAnl_C fn_anl;
  fn_anl.set(p);
  pg_dir->set(fn_anl.getDrive());
  pg_dir->cat(fn_anl.getDir());
}


void PgExec_C::prmSet(char *str){
  prmSet(str,false);
}

void PgExec_C::prmSet(char *str,bool spInh){
  int len = strlen(str);
  bool spFlg = false;
  bool dirCatFlag=false;

  if (firstFlg){
    firstFlg=false;
    char *w = pg_dir->c_str();
    if (*w != 0x00){
      if (fileExistCheck(pg_dir->c_str(),str)){
        dirCatFlag=true;
      }
    }
  }
  else {
    command->cat_string(" ");
  }
  for(int i=0;i<len;i++){
    if (*(str+i) == ' '){spFlg=true; break;}
  }
  if (dirCatFlag){
    char *w = pg_dir->c_str();
    int len = strlen(w);
    for(int i=0;i<len;i++){
      if (*(w+i) == ' '){spFlg=true; break;}
    }
  }
  if (spInh) {
    spFlg = false;  
  }

  if (spFlg){command->cat_string("\"");}
  if (dirCatFlag){command->cat_string(pg_dir->c_str());}
  command->cat_string(str);
  if (spFlg){command->cat_string("\"");}
 
}


bool PgExec_C::fileExistCheck(char *p_dir,char *p_cmd){
  work->set_string(p_dir);
  work->cat_string(p_cmd);
  if (FileExists(work->cc_str())) {return(true);}
  work->set_string(p_dir);
  work->cat_string(p_cmd);
  work->cat_string(".exe");
  if (FileExists(work->cc_str())) {return(true);}
  work->set_string(p_dir);
  work->cat_string(p_cmd);
  work->cat_string(".bat");
  if (FileExists(work->cc_str())) {return(true);}
  work->set_string(p_dir);
  work->cat_string(p_cmd);
  work->cat_string(".com");
  if (FileExists(work->cc_str())) {return(true);}
  return(false);
}


void PgExec_C::Exec(){
  if (shellFlg == false){
	  system(command->cc_str());
  }
  else
    {
    cmdLine->set_string("/C \"");
    cmdLine->cat_string(command->cc_str());
    cmdLine->cat_string("\"");
    ShellExecute(NULL,NULL,"cmd",cmdLine->cc_str(),NULL,cmd_state);
  }
  firstFlg = true;
  command->set_string("");
}

