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
