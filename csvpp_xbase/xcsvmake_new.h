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
 
#ifndef _XCSVMAKE_NEW_H_
#define _XCSVMAKE_NEW_H_

//#define DEBUG_csvmake

#include <windows.h>
#include <tlhelp32.h>
#include "zschar.h"
#include "zfile.h"
#include "xread64.h"
#include "xwrite64.h"
#include "xwildcard.h"
#include "xexec.h"

class procCell{
  public:
  sChar *proc_name;
  DWORD  proc_id;
  
  procCell(char *p0,DWORD p1){
    proc_name = new sChar(p0);
    proc_id   = p1;
  }
};


class csvmake_C{
  public:
    VariableArray_C *del_fp;
    VariableArray_C *sel_fp;
    VariableArray_C *esel_fp;
    VariableArray_C *proc_fp;
    WildCard_T      *wild;
    PgExec_C         exec;
    char             buff[MAX_PATH];
    char             chgFilePath[4096];
    sChar           *s_str;
    char             str_cnt[5];
    VariableArea_C  *filter;
    VariableArea_C  *ext;
    VariableArea_C  *suffix;
    VariableArea_C  *get_file;
    Variant          excel;
    Variant          books;
    Variant          application;
    bool             pchkFlag;
    bool             tmpFlag;
    int              total_cnt;
    procCell        *pc;
    
    // csvmake2
    bool             delFlg;
    bool             visual_flg;
    bool             pasteFlg;
    bool             xmlFlg;
    char chgSheetPath[4096];
    VariableArray_C *sheet_fp;

    csvmake_C(){
      sheet_fp   = new VariableArray_C(5);
      proc_fp    = new VariableArray_C(5);
      del_fp     = new VariableArray_C(5);
      sel_fp     = new VariableArray_C(5);
      esel_fp    = new VariableArray_C(5);
      wild       = new WildCard_T;
      get_file   = new VariableArea_C(100);
      filter     = new VariableArea_C(10);
      ext        = new VariableArea_C(10);
      suffix     = new VariableArea_C(10);
      tmpFlag    = false;
      pchkFlag   = false;
      total_cnt  = 1;
      delFlg     = false;
      visual_flg = true;
      pasteFlg   = false;
      xmlFlg     = false;
    }

    ~csvmake_C(){

       ARY_LOOP(s_str,sChar,del_fp)
         bool okFlg=false;
         for(int i=0;i<10;i++){
           if (DeleteFile(s_str->c_str()) == true){okFlg=true; break;}
           Sleep(3000);
           printf("Retry delete %s..\n",s_str->c_str());
         }
         if (okFlg) {
           printf("%s delete.\n",s_str->c_str());
         }
         else {
           printf("%s can not delete?\n",s_str->c_str());
         }
       LOOP_END

       ARY_LOOP(pc,procCell,proc_fp)  delete pc; LOOP_END
       ARY_LOOP(s_str,sChar,del_fp )  delete s_str; LOOP_END
       ARY_LOOP(s_str,sChar,sel_fp )  delete s_str; LOOP_END
       ARY_LOOP(s_str,sChar,esel_fp)  delete s_str; LOOP_END
       ARY_LOOP(s_str,sChar,sheet_fp) delete s_str; LOOP_END
       delete proc_fp;
       delete sel_fp;
       delete esel_fp;
       delete del_fp;
       delete sheet_fp;
       delete wild;
       delete filter;
       delete ext;
       delete get_file;
       delete suffix;
       Sleep(500);
    }
    void csvmake_begin();
    bool csvmake(char *xls_file,char *p_dir,char *p_filter,char *p_ext,char *p_suffix);
    void csvmake_end();
    void excel_kill(){process_check("EXCEL.EXE",false);}
    void process_get(){process_check("EXCEL.EXE",true);}
    void csvmake2_begin();
    void csvmake2(char *p_xlsfile,char *p_filter);
    void csvmake2_end();
//  private:
    void process_check(char *srch_name,bool getFlag);

};


#endif // _XCSVMAKE_NEW_H_
