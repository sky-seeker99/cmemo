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


#include "xcsvmake_oo.h"
#ifdef BCB5
#include <vcl/utilcls.h>
#else
#include <ComObj.hpp>
#endif



void csvmake2_oo_C::csvmake_begin(){
  try         {objServiceManager = CreateOleObject("com.sun.star.ServiceManager");}
  catch (...) {return ;}
  CoreReflection = objServiceManager.OleFunction("createInstance","com.sun.star.reflection.CoreReflection");
  objDesktop     = objServiceManager.OleFunction("createInstance","com.sun.star.frame.Desktop");
  dispatcher     = objServiceManager.OleFunction("createInstance","com.sun.star.frame.DispatchHelper");
  forName        = CoreReflection.OleFunction   ("forName","com.sun.star.beans.PropertyValue");

  MyStruct_0 = objServiceManager.OleFunction("Bridge_GetStruct","com.sun.star.beans.PropertyValue");
  MyStruct_1 = objServiceManager.OleFunction("Bridge_GetStruct","com.sun.star.beans.PropertyValue");
  MyStruct_2 = objServiceManager.OleFunction("Bridge_GetStruct","com.sun.star.beans.PropertyValue");
  MyStruct_3 = objServiceManager.OleFunction("Bridge_GetStruct","com.sun.star.beans.PropertyValue");
  MyStruct_4 = objServiceManager.OleFunction("Bridge_GetStruct","com.sun.star.beans.PropertyValue");
  MyStruct_5 = objServiceManager.OleFunction("Bridge_GetStruct","com.sun.star.beans.PropertyValue");


  Bounds_read[0] = 0; Bounds_read[1] =-1; dummy_opt                    = VarArrayCreate(Bounds_read, 1, varVariant);
  Bounds_read[0] = 0; Bounds_read[1] = 1; loadComponentFromURL_opt     = VarArrayCreate(Bounds_read, 1, varVariant);
  Bounds_read[0] = 0; Bounds_read[1] = 2; loadComponentFromURL_CSV_opt = VarArrayCreate(Bounds_read, 1, varVariant);
  Bounds_read[0] = 0; Bounds_read[1] = 0; GoToCell_opt                 = VarArrayCreate(Bounds_read, 1, varVariant);
  Bounds_read[0] = 0; Bounds_read[1] = 1; Insert_opt                   = VarArrayCreate(Bounds_read, 1, varVariant);
  Bounds_read[0] = 0; Bounds_read[1] = 5; InsertContents_opt           = VarArrayCreate(Bounds_read, 1, varVariant);
  Bounds_read[0] = 0; Bounds_read[1] = 1; saveComponentToURL_opt       = VarArrayCreate(Bounds_read, 1, varVariant);



}

void csvmake2_oo_C::csvmake_end(){
}

void csvmake2_oo_C::csvmake(char *p_xlsfile,char *p_filter){
  bool newFlg = false;

  bool hit;
  long i,j,idx;
  AnsiString str;

  bool Sheet1Flg = false;
  bool Sheet2Flg = false;
  bool Sheet3Flg = false;


  if (p_filter != NULL){filter->set_string(p_filter);}  else {filter->set(0,0x00);}

  if (_fullpath(chgFilePath, p_xlsfile, 4096) == NULL){
    printf("file_name error. (%s)\n",p_xlsfile);
    return;
  }

  if (delFlg){DeleteFile(chgFilePath);}

/*
  if (FileExist(chgFilePath) != 0){
    xWorkbook = books.OleFunction("Add");
    xWorkbook.OleFunction("SaveAs",chgFilePath);
    books.OleFunction("Close");
    newFlg = true;
  }
*/


strcpy(chgFilePath,"D:/csvpp_source_tc/csvmake_oo/csvmake_oo_test/aaa.ods");





  sss->set("file:///");
  sss->cat(chgFilePath);
  fileName = sss->c_str();
  xls_file->set(sss->c_str());
//  MyStruct_0.OlePropertySet("Name","Hidden"  );        MyStruct_0.OlePropertySet("Value",false);                          loadComponentFromURL_opt.PutElement    (MyStruct_0, 0);
//  MyStruct_1.OlePropertySet("Name","ReadOnly");        MyStruct_1.OlePropertySet("Value",false);                          loadComponentFromURL_opt.PutElement    (MyStruct_1, 1);
//  Variant objDocument       = objDesktop.OleFunction("loadComponentFromURL",fileName, "_blank", 0, loadComponentFromURL_opt);
  Variant objDocument       = objDesktop.OleFunction("loadComponentFromURL",fileName, "_blank", 0, dummy_opt);
  Variant CurrentController = objDocument.OlePropertyGet("CurrentController");
  Variant objFrame          = CurrentController.OlePropertyGet("Frame");
  Variant oWindow           = objFrame.OlePropertyGet("ContainerWindow");
//  oWindow.OleFunction("setVisible",false);
  oWindow.OleFunction("setVisible",true);
  Variant oSheets           = objDocument.OlePropertyGet("Sheets");  // ScTableSheetsObj
  Variant nSheetCount       = oSheets.OlePropertyGet("Count");
  long max                  = nSheetCount;

  bool read_only = true;
  printf("%s\n",chgFilePath);

  sChar *sheet_name = new sChar("");

  ARY_LOOP(s_str,sChar,sheet_fp)
    if (_fullpath(chgSheetPath,s_str->c_str(),4096) == NULL){printf("  <-%s(file_name error)\n", s_str->c_str()); continue;}
    if (FileExist(chgSheetPath) != 0){printf("  <-%s(file not found)\n",chgSheetPath); continue;}

    if (strcmp(filter->cc_str(),"") != 0){
      FileTemp_T *temp = new FileTemp_T("csv");
      get_file->set_string(temp->GetName());
      delete temp;

      FILE *w_fp = fopen(get_file->c_str(),"wt");
      fclose(w_fp);
      exec.prmSet(filter->cc_str());
      exec.prmSet(chgSheetPath);
      exec.prmSet(get_file->cc_str());
      exec.Exec();

      if (FileExist(get_file->cc_str()) != 0) {
        printf("%s not found.\n",get_file->cc_str());
        continue;
      }
      printf("  <- %s\n",get_file->c_str());
      s_str = new sChar(get_file->cc_str());
      del_fp->mem_alloc((unsigned char*)s_str);
    }
    else {
      printf("  <-%s\n",chgSheetPath);
    }

    char *file_name = chgSheetPath;
    int j=0; buff[0]=0x00;
    int lenght = (int)strlen(file_name);
    int sv = -1;
    for(int i=0;i<lenght;++i){
      if (*(file_name+i) == '\\'){j=0; buff[0]=0x00; continue;}
      if (*(file_name+i) == '.' ){sv = j;}
      buff[j]   = *(file_name+i);
      buff[j+1] = 0x00;
      ++j;
    }
    if (sv != -1){buff[sv] = 0x00;}

    if (strlen(buff) > 31){
      printf("  sheet name cutting [%s]->",buff);
      *(buff+31) = 0x00;
      printf("[%s]\n",buff);
    }

    sheet_name->set(buff);

    // - csv file open -------------------------------
    char *csv_file;
    if (strcmp(filter->c_str(),"") != 0){csv_file = get_file->cc_str();}
    else                                {csv_file = chgSheetPath;}

    sss->set("file:///");
    sss->cat(csv_file);
    fileName_csv = sss->c_str();
    MyStruct_0.OlePropertySet("Name","ReadOnly");        MyStruct_0.OlePropertySet("Value",true);                           loadComponentFromURL_CSV_opt.PutElement(MyStruct_0, 0);
    MyStruct_1.OlePropertySet("Name","FilterName");      MyStruct_1.OlePropertySet("Value","Text - txt - csv (StarCalc)");  loadComponentFromURL_CSV_opt.PutElement(MyStruct_1, 1);
    MyStruct_2.OlePropertySet("Name","FilterOptions");   MyStruct_2.OlePropertySet("Value","44,34,64,1,2/1/1/1/1");         loadComponentFromURL_CSV_opt.PutElement(MyStruct_2, 2);
    Variant objDocument_csv       = objDesktop.OleFunction              ("loadComponentFromURL",fileName_csv, "_blank", 0, loadComponentFromURL_CSV_opt);
    Variant CurrentController_csv = objDocument_csv.OlePropertyGet      ("CurrentController");
    Variant objFrame_csv          = CurrentController_csv.OlePropertyGet("Frame");
    Variant oWindow_csv           = objFrame_csv.OlePropertyGet            ("ContainerWindow");
//    oWindow_csv.OleFunction("setVisible",false);
    oWindow_csv.OleFunction("setVisible",true);

    // - all select -------------------------------
    MyStruct_0.OlePropertySet("Name","ToPoint");
    MyStruct_0.OlePropertySet("Value","$A$1:$AMJ$1048576");
    GoToCell_opt.PutElement                (MyStruct_0, 0);
    dispatcher.OleFunction            ("executeDispatch",objFrame_csv,".uno:GoToCell", "", 0,GoToCell_opt);
    // - copy  -------------------------------
    dispatcher.OleFunction            ("executeDispatch",objFrame_csv,".uno:Copy"    , "", 0,dummy_opt);
    // - sheet search ------------------------
    hit = false;
    for(int i=0;i<max;i++){
      Variant sheet = oSheets.OleFunction("GetByIndex",i);  // ScTableSheetObj
      Variant name  = sheet.OlePropertyGet("Name");
      str = name;

      if (strcmp(str.c_str(),sheet_name->c_str()) == 0){
        hit = true;

        Variant Sheets = objDocument.OleFunction("getSheets");
        Variant Sheet = Sheets.OleFunction("getByName", name);
        Variant  View = objDocument.OleFunction("getCurrentController");
        View.OleFunction("Select", Sheet);

        break;
      }
    }

    if (strcmp(sheet_name->c_str(),"Sheet1")==0){Sheet1Flg=true;}
    if (strcmp(sheet_name->c_str(),"Sheet2")==0){Sheet2Flg=true;}
    if (strcmp(sheet_name->c_str(),"Sheet3")==0){Sheet3Flg=true;}

    // - sheet add ---------------------------
    if (hit == false){
      MyStruct_0.OlePropertySet("Name","Name");       MyStruct_0.OlePropertySet("Value",sheet_name->c_str());  Insert_opt.PutElement(MyStruct_0, 0);
      MyStruct_1.OlePropertySet("Name","Index");      MyStruct_1.OlePropertySet("Value",1);                    Insert_opt.PutElement(MyStruct_1, 1);
      dispatcher.OleFunction("executeDispatch",objFrame,".uno:Insert"    , "", 0,Insert_opt);
    }

    // - all select -------------------------------
    MyStruct_0.OlePropertySet("Name","ToPoint");
    MyStruct_0.OlePropertySet("Value","$A$1:$AMJ$1048576");
    GoToCell_opt.PutElement           (MyStruct_0, 0);
    dispatcher.OleFunction            ("executeDispatch",objFrame,".uno:GoToCell", "", 0,GoToCell_opt);

    // - paste ------------------------------------
    MyStruct_0.OlePropertySet("Name","Flags");           MyStruct_0.OlePropertySet("Value","S");                            InsertContents_opt.PutElement          (MyStruct_0, 0);
    MyStruct_1.OlePropertySet("Name","FormulaCommand");  MyStruct_1.OlePropertySet("Value",0);                              InsertContents_opt.PutElement          (MyStruct_1, 1);
    MyStruct_2.OlePropertySet("Name","SkipEmptyCells");  MyStruct_2.OlePropertySet("Value",false);                          InsertContents_opt.PutElement          (MyStruct_2, 2);
    MyStruct_3.OlePropertySet("Name","Transpose");       MyStruct_3.OlePropertySet("Value",false);                          InsertContents_opt.PutElement          (MyStruct_3, 3);
    MyStruct_4.OlePropertySet("Name","AsLink");          MyStruct_4.OlePropertySet("Value",false);                          InsertContents_opt.PutElement          (MyStruct_4, 4);
    MyStruct_5.OlePropertySet("Name","MoveMode");        MyStruct_5.OlePropertySet("Value",4);                              InsertContents_opt.PutElement          (MyStruct_5, 5);
    dispatcher.OleFunction            ("executeDispatch",objFrame,".uno:InsertContents", "", 0,InsertContents_opt);




    objDocument_csv.OleFunction( "close",true );
  LOOP_END

  delete sheet_name;
  fileName = xls_file->c_str();
//  objDocument.OleFunction ("store");
//  objDocument.OleFunction ("storeAsURL",fileName,dummy_opt);
//  objDocument.OleFunction ("StoreAsUrl",fileName,dummy_opt);


//  MyStruct_0.OlePropertySet("Name","URL");
//  MyStruct_0.OlePropertySet("Value","file://aaaaa");
//  GoToCell_opt.PutElement                (MyStruct_0, 0);
//  dispatcher.OleFunction            ("executeDispatch",objFrame,".uno:SaveAs", "", 0,GoToCell_opt);
  dispatcher.OleFunction            ("executeDispatch",objFrame,".uno:Save", "", 0,dummy_opt);
//  objDocument.OleFunction( "close",true );

  objDocument.OleFunction ("dispose");

}

/// --------------------------------------------------------------------------

void csvmake_oo_C::csvmake_begin(){

  try         {objServiceManager = CreateOleObject("com.sun.star.ServiceManager");}
  catch (...) {return ;}
  if (VarType(objServiceManager) == varDispatch){objDesktop = objServiceManager.OleFunction("createInstance","com.sun.star.frame.Desktop");}
  Variant CoreReflection    = objServiceManager.OleFunction("createInstance","com.sun.star.reflection.CoreReflection");
  Variant forName           = CoreReflection.OleFunction("forName","com.sun.star.beans.PropertyValue");


  Bounds_read[0] = 0;
  Bounds_read[1] = 1;
  fileName = sss->c_str();
  VariantArray_read = VarArrayCreate(Bounds_read, 1, varVariant);
  MyStruct_read_0 = objServiceManager.OleFunction("Bridge_GetStruct","com.sun.star.beans.PropertyValue");
  MyStruct_read_1 = objServiceManager.OleFunction("Bridge_GetStruct","com.sun.star.beans.PropertyValue");
  MyStruct_read_0.OlePropertySet("Name","Hidden");
  MyStruct_read_0.OlePropertySet("Value",false);
  MyStruct_read_1.OlePropertySet("Name","ReadOnly");
  MyStruct_read_1.OlePropertySet("Value",true);
  VariantArray_read.PutElement(MyStruct_read_0, 0);
  VariantArray_read.PutElement(MyStruct_read_1, 1);
  sss->set("");
}

void csvmake_oo_C::csvmake_end(){

}

bool csvmake_oo_C::csvmake(char *p_xlsfile,char *p_dir,char *p_filter,char *p_ext){
  AnsiString str;
  char *dir    = p_dir;
  sChar *s_str;

  if (p_filter != NULL){filter->set(p_filter);}  else {filter->set("");}
  if (p_ext    != NULL){ext->set(p_ext);}        else {ext->set("csv");}

  if (_fullpath(chgFilePath, p_xlsfile, 4096) == NULL){printf("file_name error. (%s)\n", p_xlsfile);        return(false);}
  if (FileExist(chgFilePath)                  != 0   ){printf("File Not Found! (File = %s)\n",chgFilePath); return(false);}

  printf("%s\n",chgFilePath);

  sss->set("file:///");
  sss->cat(chgFilePath);
  fileName = sss->c_str();

  objDocument       = objDesktop.OleFunction("loadComponentFromURL",fileName, "_blank", 0, VariantArray_read);

  objDocument.OleFunction("addActionLock");
  Variant CurrentController = objDocument.OlePropertyGet("CurrentController");
  Variant Frame = CurrentController.OlePropertyGet("Frame");
  Variant oWindow = Frame.OlePropertyGet("ContainerWindow");
  oWindow.OleFunction("setVisible",false);
  Variant oSheets           = objDocument.OlePropertyGet("Sheets");  // ScTableSheetsObj
  Variant nSheetCount       = oSheets.OlePropertyGet("Count");
  long max =  nSheetCount;
  int total_cnt = 1;
  for(int i=0;i<max;i++){
    Variant sheet = oSheets.OleFunction("GetByIndex",i);  // ScTableSheetObj
    Variant name  = sheet.OlePropertyGet("Name");
    str = name;
    sprintf(buff,"%s",str);
    Variant Sheets = objDocument.OleFunction("getSheets");
    Variant Sheet = Sheets.OleFunction("getByName", name);
    Variant  View = objDocument.OleFunction("getCurrentController");
//    View.OleFunction("setActiveSheet", Sheet);
    View.OleFunction("Select", Sheet);

    // 書き出すcsvファイルをスキャン
    bool hit_sel = false;
    bool first   = true;
    MEM_LOOP(s_str,sChar,sel_fp )
      if (first){first=false;}
      wild->set(s_str->c_str());
      if (wild->cmp(buff)){hit_sel=true; break;}
    LOOP_END
    if (first){hit_sel = true;}
    if (hit_sel == false){continue;}

    // 書き出さないcsvファイルをスキャン
    bool hit_esel = false;
    MEM_LOOP(s_str,sChar,esel_fp)
      wild->set(s_str->c_str());
      if (wild->cmp(buff)){hit_esel=true; break;}
    LOOP_END
    if (hit_esel == true){continue;}

    // 出力用csvファイル名を作成
    sprintf(buff,"%s\\%s.%s",dir,str,ext->c_str());
    if (*dir != 0x00){
      if (*(dir + strlen(dir)-1) == '\\'){sprintf(buff,"%s%s.%s",dir,str,ext->c_str());}
    }

    // CSVファイル書き込み部
    bool ok = true;
    if (FileExist(buff) == 0){ok = DeleteFile(buff);}
    if (ok){
      FILE *chk_fp = fopen(buff,"wt");
      if (chk_fp == NULL){ok = false;}
      else               {fclose(chk_fp); DeleteFile(buff);}
    }


    if (ok == true){
      // Filter ----------------
      if (strcmp(filter->c_str(),"") != 0){
        FileTemp_T *temp = new FileTemp_T("csv");
        sChar *get_file = new sChar(temp->GetName());
        delete temp;

        //printf("  ->%s\n",get_file->c_str());
        csvFileSave(moji_kind,objServiceManager,objDocument,get_file->c_str());

        sChar *command  = new sChar(filter);
        command->cat(" ");
        command->cat(get_file->c_str());
        command->cat(" ");
        command->cat(buff);
        command->cat(" ");

        char str_cnt[5];
        sprintf(str_cnt,"%03d",total_cnt++);
        command->cat(str_cnt);
        command->cat(" ");
        command->cat(dir);
        command->cat(" ");
        command->cat(str.c_str());

        system(command->c_str());
        printf("  ->%s\n",buff);
        delete command;
        it->alloc_ptr = get_file;
        del_fp->mem_alloc();
      } else {
        FILE *chk_fp = fopen(buff,"wt");
        if (chk_fp == NULL){printf("  ->%s(NG)\n",buff);}
        else {
          fclose(chk_fp);
          DeleteFile(buff);
          //printf("  ->%s\n",buff);
          csvFileSave(moji_kind,objServiceManager,objDocument,buff);
        }
      }
    } else {printf("  ->%s(NG)\n",buff);}
  }
  objDocument.OleFunction( "close",true );
  return(true);
}

// CSVファイル書き込み
void csvmake_oo_C::csvFileSave(int moji_kind,Variant objServiceManager,Variant objDocument,char *buff){
  printf("  ->%s\n",buff);

  sCharGrep *sss = new sCharGrep("file:///");
  sss->cat(buff);
  sss->replace_all("\\","/");
  String fileName = sss->c_str();

  Variant VariantArray_write;
  int Bounds_write[2];
  Bounds_write[0] = 0;
  Bounds_write[1] = 1;
  Variant MyStruct_write_0 = objServiceManager.OleFunction("Bridge_GetStruct","com.sun.star.beans.PropertyValue");
  Variant MyStruct_write_1 = objServiceManager.OleFunction("Bridge_GetStruct","com.sun.star.beans.PropertyValue");

  VariantArray_write = VarArrayCreate(Bounds_write, 1, varVariant);
  MyStruct_write_0.OlePropertySet("Name","FilterName");
  MyStruct_write_0.OlePropertySet("Value","Text - txt - csv (StarCalc)");
  VariantArray_write.PutElement(MyStruct_write_0, 0);

  MyStruct_write_1.OlePropertySet("Name","FilterOptions");
  if (moji_kind == 2){
    MyStruct_write_1.OlePropertySet("Value","44,34,76,1,2/1/1/1/1"); // UTF-8
  }
  else if (moji_kind == 0){
    MyStruct_write_1.OlePropertySet("Value","44,34,64,1,2/1/1/1/1"); // SHIFT-JIS
  }
  else {
    MyStruct_write_1.OlePropertySet("Value","44,34,69,1,2/1/1/1/1"); // EUC
  }
  VariantArray_write.PutElement(MyStruct_write_1, 1);

  objDocument.OleFunction ("storeAsURL", fileName, VariantArray_write);

  delete sss;

}


