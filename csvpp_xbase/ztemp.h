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
// 64bit filepointer temporary write class
//

#ifndef _ZTEMP_H_
#define _ZTEMP_H_

#include <stdio.h>
#include "xarea.h"
#include "xread64.h"
#include "xwrite64.h"
#include "zschar.h"


// # Temporary Class ------------------
class tempFileInf_C{
  public:
    VariableArray_C *file_fp;  // cell:VariableArea_C
    VariableArea_C  *temp;
    VariableArea_C  *path;
    char            *curr_temp;
    Write64_C       *out_fp;
    Read64_C        *in_fp;

    void       closeTempFile  (                                    );
    Write64_C *openTempFile   (char *p_ext,char *mode,int buff_size);
    Write64_C *changeWriteMode(            char *mode,int buff_size);
    Read64_C  *changeReadMode (            char *mode,int buff_size);
    char      *getTempFileName(char *p_ext                         );
    void       removeAllFile  (                                    );
    void       tempFileToCopy (char *p_name          ,int buff_size);

    tempFileInf_C(){
      char  *env = getenv("TEMP");
      temp = new VariableArea_C(env);
      if (*(env + strlen(env) - 1) != '\\'){temp->cat_string("\\sk_temp");}
      else                                 {temp->cat_string("sk_temp"  );}
      path      = new VariableArea_C(10);
      file_fp   = new VariableArray_C(10);
      curr_temp = NULL;
      out_fp    = NULL;
      in_fp     = NULL;
    }

    ~tempFileInf_C(){
      closeTempFile();
      removeAllFile();
      delete file_fp;
      delete temp;
      delete path;
    }

};

// # Temporary Class ŒÝŠ· ------------------
class FileTemp_T{
  public:
    tempFileInf_C *temp;
    sChar *name;
    FILE  *fp;

    FileTemp_T(char *ext){
      temp = new tempFileInf_C;
      name = new sChar(temp->getTempFileName(ext));
      fp = fopen(name->c_str(),"wt");
    }

    ~FileTemp_T(){
      if (fp != NULL){fclose(fp);}
      delete name;
      delete temp;
    }

    void close(){if(fp != NULL){fclose(fp); fp=NULL;}}
    void TempToRename(char *p_name){RenameFile(name->c_str(),p_name);}
    char *GetName(){return(name->c_str());}
    FILE *GetFp(){return(fp);}
};

#endif // _ZTEMP_H_
