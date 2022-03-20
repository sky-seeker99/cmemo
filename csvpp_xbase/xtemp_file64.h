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
