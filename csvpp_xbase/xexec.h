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
