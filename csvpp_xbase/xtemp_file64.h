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

#ifndef _XTEMP_FILE64_H_
#define _XTEMP_FILE64_H_

#include "xtoken.h"
#include "xwrite64.h"
#include "xread64.h"


// # Temporary Class ------------------
class FileTemp64_T{
  public:
    sChar *name;
    char  *env;
    Write64_C *out_fp;
    Read64_C  *in_fp;
	int buff_size;
    
    FileTemp64_T(char *p_ext,char *mode,int size){
      int i;
      sChar *ww;
      out_fp = NULL;
      buff_size = size;
      if (buff_size < 10240){buff_size = 10240;}
      env  = getenv("TEMP");
      name = new sChar(env);
      if (*(env + strlen(env) - 1) != '\\'){name->cat("\\sk_temp");}
      else                                 {name->cat("sk_temp"  );}
      ww = new sChar("");
      for(i=0;;++i)
        {
        ww->set(name->c_str());
        ww->intcat(i);
        ww->cat(".");
        ww->cat(p_ext);
        if (FileExists(ww->c_str()) == true){continue;}
        out_fp = new Write64_C(ww->c_str(),mode,buff_size);
		name->set(ww->c_str());
        break;
        }
      delete ww;
      in_fp = NULL;
    }

    ~FileTemp64_T(){
      if (in_fp  != NULL){delete in_fp;}
      if (out_fp != NULL){delete out_fp;}
      DeleteFile(name->c_str());
      delete name;
    }
    void  tempFileToCopy(char *p_name);
    void  readModeChg(char *mode);
    void  writeModeChg(char *mode);

};

#endif // _XTEMP_FILE64_H_
