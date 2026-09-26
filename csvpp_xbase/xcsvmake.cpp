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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <condefs.h>
#include "xcsvmake.h"

// output dir
void XlsCsv_C::outDirSet(char *dir){
  g_out_dir->set(dir);
  int len = strlen(dir);
  if (len > 0){
    if (*(dir+len-1) != '\\'){g_out_dir->cat("\\");}
  }
}

// xls -> csv
void XlsCsv_C::xls2csv(char *xls_file,char *suffix,char *ext,bool tempFlg){
  bool ok;
  sChar *str;
  long max;
  long i;
  AnsiString astr;

  if (FileExists(xls_file) == false){
    printf("Excel File not found. file=%s\n",xls_file);
    return;
  }

  FileTemp64_T *temp;
  sChar *command;
  sChar *get_file;

  printf("%s->\n",xls_file);
  str = new sChar("");
  books.OleFunction( "Open", xls_file);

  b_count = books.OlePropertyGet("Count");
  book    = books.OlePropertyGet("Item",&b_count );
  sheets  = book.OlePropertyGet ("Worksheets");
  s_count = sheets.OlePropertyGet("Count");
  max = s_count;

  char *out_dir = g_out_dir->c_str();
  
  for(i=1;i<max+1;++i){
    s_count = i;
    sheet = sheets.OlePropertyGet ("Item",&s_count);
    name  = sheet.OlePropertyGet("Name");
    astr = name;

    sheet.OleFunction("Activate");
    str->set(out_dir);
    if (suffix != NULL){
      str->cat(suffix);
      str->cat("_");
    }
    str->cat(astr.c_str());
    if (ext != NULL){
      str->cat(".");
      str->cat(ext);
    }
    
    ok = true;
    if (FileExists(str->c_str()) == true){ok = DeleteFile(str->c_str());}
    if (ok == true){
      if (tempFlg == false){
        FILE *chk_fp = fopen(str->c_str(),"wt");
        if (chk_fp == NULL){printf("  ->%s(NG)\n",str->c_str());}
        else{
          fclose(chk_fp);
          DeleteFile(str->c_str());

          printf("  ->%s\n"    ,str->c_str());
          sheet.OleFunction("SaveAs",str->c_str(),6);
        }
      } else {
        temp = new FileTemp64_T("$$$","wt",10240);
        delete temp->out_fp;
        temp->out_fp = NULL;
        FILE *chk_fp = fopen(temp->name->c_str(),"wt");
        if (chk_fp == NULL){printf("  ->%s(NG)\n",temp->name->c_str());}
        else{
          fclose(chk_fp);
          DeleteFile(temp->name->c_str());

          FILE *chk_fp = fopen(str->c_str(),"wt");
          if (chk_fp == NULL){printf("  ->%s(NG)\n",str->c_str());}
          else{
            fclose(chk_fp);
            DeleteFile(str->c_str());
            printf("  ->temp_file->%s\n"    ,str->c_str());
            sheet.OleFunction("SaveAs",temp->name->c_str(),6);
            temp->tempFileToCopy(str->c_str());
            delete temp;
          }  
        }
      }
    } else {
      printf("  ->%s(NG)\n",str->c_str());
    }
  }
  book.OleFunction("Close",false);
  delete str;
}


