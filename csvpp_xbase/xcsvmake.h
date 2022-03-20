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

#ifndef _XCSVMAKE_H_
#define _XCSVMAKE_H_

#include "xtemp_file64.h"

// xls->csv interface class -----------------
class XlsCsv_C{
  public:
    sChar *g_out_dir;  // 出力ディレクトリ   

    XlsCsv_C(){
      g_out_dir = new sChar("");
      
      // OLE Initialize
      Application->Initialize();
      excel = CreateOleObject("Excel.Application");
      excel.OlePropertySet( "Visible", false );
      books = excel.OlePropertyGet( "Workbooks" );
    }

    ~XlsCsv_C(){
      delete g_out_dir;
      
      // OLE End
      application = excel.OlePropertyGet( "Application" );
      application.OlePropertySet("DisplayAlerts",false);
      application.OleFunction( "Quit" );
    }                               
    
    void xls2csv(char *xls_file,char *suffix,char *ext,bool tempFlg);
    void outDirSet(char *dir);    

  private: 
    Variant application;
    Variant excel;
    Variant books;
    Variant b_count;
    Variant book;
    Variant sheets;
    Variant s_count;
    Variant sheet;
    Variant name;


};

#endif // _XCSVMAKE_H_
