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

#ifndef _XCSVMAKE_OO_H_
#define _XCSVMAKE_OO_H_

//#define DEBUG_csvmake

#include <windows.h>
#include <tlhelp32.h>
#include "zschar.h"
#include "zfile.h"
#include "xread64.h"
#include "xwrite64.h"
#include "xwildcard.h"
#include "xexec.h"




class csvmake_oo_C{
  public:
    Variant objServiceManager;
    Variant objDesktop;
    Variant VariantArray_read;
    Variant MyStruct_read_0;
    Variant MyStruct_read_1;
    Variant objDocument;
    int Bounds_read[2];
    sChar *sss;
    String fileName;
    char   buff[MAX_PATH];
    char   chgFilePath[4096];

    MemHeader_T *it;
    MEM_FP      *del_fp;
    MEM_FP      *sel_fp;
    MEM_FP      *esel_fp;
    WildCard_T  *wild;
    sChar       *filter;
    sChar       *ext;
    int          moji_kind;
    csvmake_oo_C(){
      // •Ï”‚Ì‰ŠúÝ’è
      // Application->Initialize();  ‚ª•K—v

      it               = new MemHeader_T;
      del_fp           = new MEM_FP(it);
      sel_fp           = new MEM_FP(it);
      esel_fp          = new MEM_FP(it);
      wild             = new WildCard_T;
      filter           = new sChar("");
      sss              = new sChar("");
      ext              = new sChar("csv");
      moji_kind        = 0;
    }

    ~csvmake_oo_C(){
      sChar *s_str;
      sChar *get_file;
      MEM_LOOP(get_file,sChar,del_fp)
        for(;;){
          if (DeleteFile(get_file->c_str()) == true){break;}
        }
        printf("%s delete.\n",get_file->c_str());
        delete get_file;
      LOOP_END
      MEM_LOOP(s_str,sChar,sel_fp ) delete s_str; LOOP_END
      MEM_LOOP(s_str,sChar,esel_fp) delete s_str; LOOP_END

      delete sel_fp;
      delete esel_fp;
      delete del_fp;
      delete it;
      delete wild;
      delete sss;
      delete filter;
      delete ext;
    }
    void csvmake_begin();
    bool csvmake(char *xls_file,char *p_dir,char *p_filter,char *p_ext);
    void csvmake_end();
    void csvFileSave(int moji_kind,Variant objServiceManager,Variant objDocument,char *buff);

};

// -------------------------------------------------

class csvmake2_oo_C{
  public:
    sChar *sss;
    String fileName;
    String fileName_csv;
    int  Bounds_read[2];
    char chgSheetPath[4096];
    char chgFilePath[4096];
    char buff[MAX_PATH];
    bool delFlg;
    PgExec_C         exec;
    VariableArray_C *del_fp;

    VariableArray_C *sheet_fp;
    VariableArea_C  *filter;
    VariableArea_C  *get_file;
    sChar *xls_file;

    Variant objServiceManager;
    Variant objDesktop;
    Variant CoreReflection;
    Variant forName;
    Variant dispatcher;

    Variant dummy_opt;
    Variant loadComponentFromURL_opt;
    Variant loadComponentFromURL_CSV_opt;
    Variant GoToCell_opt;
    Variant Insert_opt;
    Variant InsertContents_opt;
    Variant saveComponentToURL_opt;

	Variant MyStruct_0;
	Variant MyStruct_1;
	Variant MyStruct_2;
	Variant MyStruct_3;
	Variant MyStruct_4;
	Variant MyStruct_5;
    csvmake2_oo_C(){
      del_fp     = new VariableArray_C(5);
      sheet_fp   = new VariableArray_C(5);
      sss        = new sChar("");
      filter     = new VariableArea_C(10);
      get_file   = new VariableArea_C(10);
      delFlg     = false;
      xls_file   = new sChar("");
    }


    ~csvmake2_oo_C(){
      sChar *s_str;
      delete sss;
      ARY_LOOP(s_str,sChar,del_fp )  delete s_str; LOOP_END
      ARY_LOOP(s_str,sChar,sheet_fp) delete s_str; LOOP_END
      delete sheet_fp;
      delete del_fp;
      delete filter;
      delete get_file;
      delete xls_file;
    }

    void csvmake2_oo_C::csvmake_begin();
    void csvmake2_oo_C::csvmake(char *p_xlsfile,char *p_filter);
    void csvmake2_oo_C::csvmake_end();






};


#endif // _XCSVMAKE_OO_H_
