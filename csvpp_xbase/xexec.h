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

#ifndef _XEXEC_H_
#define _XEXEC_H_
#include <windows.h>

#include <stdio.h>
#include <condefs.h>
#include <dir.h>
#include "xarea.h"
#include "zschar.h"
#include "zfilename_anl.h"


// # Program Exec ------------------
class PgExec_C {
  public:
    int  cmd_state;
    bool shellFlg;
    sChar *pg_dir;
  private:
    VariableArea_C *command;
    VariableArea_C *cmdLine;
    VariableArea_C *work;
    bool firstFlg;
    bool fileExistCheck(char *p_dir,char *p_cmd);
  public:
	PgExec_C()  {
	  cmd_state = SW_SHOWNORMAL;   // SW_HIDE
	  firstFlg= true;
	  command = new VariableArea_C("");
	  cmdLine = new VariableArea_C("");
	  work    = new VariableArea_C("");
	  shellFlg = false;
      pg_dir  = new sChar("");
	}

	PgExec_C::~PgExec_C(){
	  delete command;
	  delete cmdLine;
      delete work;
      delete pg_dir;
    }

    void dirSet(char *fullpath);
	void prmSet(char *str,bool spInh);
	void prmSet(char *str);
	char *getCommand(){return(command->cc_str());}
    void prmInit(){firstFlg = true; command->set_string("");}
	void Exec();
  };

#endif // _XEXEC_H_
