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


#include "xcsvmake_new.h"

#ifdef BCB5
#include <vcl/utilcls.h>
#else
#include <ComObj.hpp>
#endif

void csvmake_C::csvmake2_begin(){
  process_check("EXCEL.EXE",true);
  excel       = CreateOleObject            ("Excel.Application");
  if (visual_flg)  {excel.OlePropertySet   ("Visible", true   );}
  else             {excel.OlePropertySet   ("Visible", false  );}
  application = excel.OlePropertyGet       ("Application"      );
  application.OlePropertySet               ("DisplayAlerts",false);
  books       = excel.OlePropertyGet       ("Workbooks"        );
}

void csvmake_C::csvmake2_end(){
  application.OleFunction( "Quit" );
  process_check("EXCEL.EXE",false);
}


void csvmake_C::csvmake_begin(){
  process_check("EXCEL.EXE",true);
  excel = CreateOleObject("Excel.Application");
  excel.OlePropertySet( "Visible", false );
  books = excel.OlePropertyGet( "Workbooks" );
}

void csvmake_C::csvmake_end(){
  application = excel.OlePropertyGet( "Application" );
  application.OlePropertySet("DisplayAlerts",false);
  application.OleFunction( "Quit" );
  process_check("EXCEL.EXE",false);
}

bool csvmake_C::csvmake(char *p_xlsfile,char *p_dir,char *p_filter,char *p_ext,char *p_suffix){
  AnsiString str;
  char *dir    = p_dir;

  if (p_filter != NULL){filter->set_string(p_filter);}  else {filter->set(0,0x00);}
  if (p_ext    != NULL){ext->set_string(p_ext);}        else {ext->set_string("csv");}
  if (p_suffix != NULL){suffix->set_string(p_suffix); suffix->cat_string("_");}  else {suffix->set(0,0x00);}

  if (_fullpath(chgFilePath, p_xlsfile, 4096) == NULL){printf("file_name error. (%s)\n", p_xlsfile);        return(false);}
  if (FileExist(chgFilePath)                  != 0   ){printf("File Not Found! (File = %s)\n",chgFilePath); return(false);}

  printf("%s\n",chgFilePath);
  books.OleFunction( "Open", chgFilePath);

  Variant b_count = books.OlePropertyGet("Count");
  Variant book    = books.OlePropertyGet("Item",&b_count );
  Variant sheets  = book.OlePropertyGet ("Worksheets");

  Variant s_count = sheets.OlePropertyGet("Count");
  long max = s_count;

  for(long i=1;i<max+1;++i){
    s_count = i;
    Variant sheet = sheets.OlePropertyGet ("Item",&s_count);
    Variant name  = sheet.OlePropertyGet("Name");
    sheet.OleFunction("Activate");
    str = name;
    sprintf(buff,"%s",str);
    bool hit_sel = false;
    bool first   = true;
    ARY_LOOP(s_str,sChar,sel_fp )
      if (first){first=false;}
      wild->set(s_str->c_str());
      if (wild->cmp(buff)){hit_sel=true; break;}
    LOOP_END
    if (first){hit_sel = true;}
    if (hit_sel == false){continue;}

    bool hit_esel = false;
    ARY_LOOP(s_str,sChar,esel_fp)
      wild->set(s_str->c_str());
      if (wild->cmp(buff)){hit_esel=true; break;}
    LOOP_END
    if (hit_esel == true){continue;}
    
    sprintf(buff,"%s\\%s%s.%s",dir,suffix->cc_str(),str,ext->cc_str());
    if (*dir != 0x00){
      if (*(dir + strlen(dir)-1) == '\\'){sprintf(buff,"%s%s%s.%s",dir,suffix->cc_str(),str,ext->cc_str());}
    }

    // full path change
    if (_fullpath(chgFilePath, buff, 4096) == NULL){printf("  ->%s(NG)\n", buff); continue;}
    strcpy(buff,chgFilePath);

    bool ok = true;
    if (FileExist(buff) == 0){ok = DeleteFile(buff);}
    if (ok){
      FILE *chk_fp = fopen(buff,"wt");
      if (chk_fp == NULL){ok = false;}
      else               {fclose(chk_fp); DeleteFile(buff);}
    }

    if (ok == true){
      if (tmpFlag){   // -tmp
        FileTemp_T *temp = new FileTemp_T("csv");
        get_file->set_string(temp->GetName());
        delete temp;
        printf("  ->%s\n",get_file->c_str());
        sheet.OleFunction("SaveAs",get_file->cc_str(),6);
        Read64_C  *in_fp  = new Read64_C (get_file->cc_str(),"rb",10240);
        Write64_C *out_fp = new Write64_C(buff,"wb",10240);
        __int64 size = in_fp->sizeGet();
        unsigned char c;
        printf("  ->%s\n",buff);
        for(__int64 kk=(__int64)0;kk<size;kk++){
          in_fp->read(&c,1);
          out_fp->write(&c,1);
        }
        delete in_fp;
        delete out_fp;
        s_str = new sChar(get_file->cc_str());
        del_fp->mem_alloc((unsigned char*)s_str);
      }
      else if (strcmp(filter->cc_str(),"") != 0){     // Filter
        FileTemp_T *temp = new FileTemp_T("csv");
        get_file->set_string(temp->GetName());
        delete temp;
        printf("  ->%s\n",get_file->c_str());
        sheet.OleFunction("SaveAs",get_file->cc_str(),6);
        sprintf(str_cnt,"%03d",total_cnt++);
        exec.prmSet(filter->cc_str());
		exec.prmSet(get_file->cc_str());
        exec.prmSet(buff);
        exec.prmSet(str_cnt);
        exec.prmSet(dir);
        exec.prmSet(str.c_str());
        exec.Exec();
        printf("  ->%s\n",buff);
        s_str = new sChar(get_file->cc_str());
        del_fp->mem_alloc((unsigned char*)s_str);
      }
      else {   // no Filter
        printf("  ->%s\n",buff);
        sheet.OleFunction("SaveAs",buff,6);
      }
    }
    else{
      printf("  ->%s(NG)\n",buff);
    }
  }

  book.OleFunction("Close",false);
  books.OleFunction("Close");

  return(true);
}

void csvmake_C::process_check(char *srch_name,bool getFlag){
  if (pchkFlag == false){return;}

  HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hSnap == INVALID_HANDLE_VALUE) {return;}
  PROCESSENTRY32 p;

  if (getFlag){
    ARY_LOOP(pc,procCell,proc_fp) 
      delete pc; 
    LOOP_END
    proc_fp->clear();
  }

  BOOL flag = Process32First(hSnap, &p);
  while(flag){

#ifdef DEBUG_csvmake
printf("process %d %s\n",getFlag,p.szExeFile);
#endif

	if (strcmp(p.szExeFile, srch_name)==0){
      if (getFlag){
        pc = new procCell(p.szExeFile,p.th32ProcessID);
        proc_fp->mem_alloc((unsigned char *)pc);
      }
      else {
        bool hitFlag=false;
        ARY_LOOP(pc,procCell,proc_fp)
          if (pc->proc_id == p.th32ProcessID){hitFlag=true; break;}
        LOOP_END
        if (hitFlag == false){
          HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,p.th32ProcessID);
          TerminateProcess(hProcess, 0);
//          printf("remain excel process delete. (pid=%d)\n",p.th32ProcessID);
		  printf("%s process delete. (pid=%d)\n",p.szExeFile,p.th32ProcessID);
		  DWORD dwExitCode = 0;
		  GetExitCodeProcess(hProcess, &dwExitCode);
          CloseHandle(hProcess);
        }
      }
    }
    flag = Process32Next(hSnap, &p);
  }
  CloseHandle(hSnap);
}


void csvmake_C::csvmake2(char *p_xlsfile,char *p_filter){
  Variant  xWorkbook;
  bool newFlg = false;

  long max;
  bool hit;
  long i,j,idx;
  AnsiString str;

  Variant Workbook;
  Variant xWorksheets;
  Variant xWorksheet;
  Variant xName;
  Variant xRange;

  bool Sheet1Flg = false;
  bool Sheet2Flg = false;
  bool Sheet3Flg = false;


  if (p_filter != NULL){filter->set_string(p_filter);}  else {filter->set(0,0x00);}

  if (_fullpath(chgFilePath, p_xlsfile, 4096) == NULL){
    printf("file_name error. (%s)\n",p_xlsfile);
    return;
  }

  if (delFlg){DeleteFile(chgFilePath);}

  if (FileExist(chgFilePath) != 0){
    xWorkbook = books.OleFunction("Add");
    xWorkbook.OleFunction("SaveAs",chgFilePath);
    books.OleFunction("Close");
    newFlg = true;
  }

  xWorkbook         = books.OleFunction("Open",chgFilePath);
  Variant xReadOnly = xWorkbook.OlePropertyGet("ReadOnly");
  bool read_only = xReadOnly;
  if (read_only == true){printf("file used! (File = %s)\n",chgFilePath); return;}

  printf("%s\n",chgFilePath);

  xWorksheets = xWorkbook.OlePropertyGet("Worksheets");

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


    if (strcmp(filter->c_str(),"") != 0){Workbook = books.OleFunction("Open", get_file->cc_str() );}
    else                                {Workbook = books.OleFunction("Open", chgSheetPath );}

    Variant Worksheets  = Workbook.OlePropertyGet    ("Worksheets"       );
    Variant S_Count     = Worksheets.OlePropertyGet  ("Count"            );
    Variant Worksheet   = Worksheets.OlePropertyGet  ("Item",&S_Count    );
                          Worksheet.OleFunction      ("Activate"         );
    Variant Range       = Worksheet.OlePropertyGet   ("Range","$A$1:$IV$65530");
                          Range.OleFunction          ("Copy"             );
    Variant xS_Count    = xWorksheets.OlePropertyGet ("Count"           );

    max = xS_Count;
    hit = false;
    for(i=1;i<max+1;++i){
      xS_Count = i;
      xWorksheet  = xWorksheets.OlePropertyGet("Item",&xS_Count  );
      xName       = xWorksheet.OlePropertyGet ("Name");
                    xWorksheet.OleFunction    ("Activate");
      str = xName;
      if (strcmp(str.c_str(),sheet_name->c_str()) == 0){hit = true; break;}
    }

    if (strcmp(sheet_name->c_str(),"Sheet1")==0){Sheet1Flg=true;}
    if (strcmp(sheet_name->c_str(),"Sheet2")==0){Sheet2Flg=true;}
    if (strcmp(sheet_name->c_str(),"Sheet3")==0){Sheet3Flg=true;}
    if (hit == false){
      xWorksheet = xWorksheets.OleFunction("Add");
      xWorksheet.OlePropertySet("Name",sheet_name->c_str());
    }

    xWorksheet.OleFunction("Activate");
    xRange = xWorksheet.OlePropertyGet("Range","A1");
    if (pasteFlg) {xRange.OleFunction("PasteSpecial");}
    else          {xRange.OleFunction("PasteSpecial" , -4163);}
                  
    xRange = xWorksheet.OlePropertyGet  ("Range","A1"      );
    xRange.OleFunction("Copy");
    if (pasteFlg) {xRange.OleFunction("PasteSpecial");}
    else          {xRange.OleFunction("PasteSpecial" , -4163);}
    Workbook.OleFunction       ("Close");
    if (xmlFlg == false){xWorkbook.OleFunction      ("Save");} 
    else                {xWorkbook.OleFunction      ("SaveAs",chgFilePath,46);}
    xS_Count = xWorksheets.OlePropertyGet ("Count");
    max = xS_Count;
    if (newFlg) {
       newFlg = false;
       for(i=1;i<max+1;++i)
         {
         xS_Count = i;
         xWorksheet  = xWorksheets.OlePropertyGet("Item",&xS_Count  );
         xName       = xWorksheet.OlePropertyGet ("Name");
                       xWorksheet.OleFunction    ("Activate");
         str = xName;
         if (((strcmp(str.c_str(),"Sheet1")==0) && (Sheet1Flg == false))
          || ((strcmp(str.c_str(),"Sheet2")==0) && (Sheet2Flg == false))
          || ((strcmp(str.c_str(),"Sheet3")==0) && (Sheet3Flg == false)))
           {
           xWorksheet.OleFunction      ("Delete");
           max--;
           i--;
           }
         }
      if (xmlFlg == false){
      xWorkbook.OleFunction      ("Save"            );
      }
      else{
      xWorkbook.OleFunction      ("SaveAs",chgFilePath,46);  // Ver 1.01
      }
    }
  LOOP_END
  delete sheet_name;
}