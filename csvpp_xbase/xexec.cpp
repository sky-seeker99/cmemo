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

