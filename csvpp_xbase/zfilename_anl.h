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

#ifndef _ZFILENAME_ANL_H_
#define _ZFILENAME_ANL_H_


#include "xarea.h"
#include "ztoken.h"

// # URL Name Separate Class ------------------
class UrlNameAnl_C{
  public:
    VariableArea_C *kind;
    VariableArea_C *site;
    VariableArea_C *dir;
    VariableArea_C *file;
    VariableArea_C *ext;
    UrlNameAnl_C(){
      kind      = new VariableArea_C(5);
      site      = new VariableArea_C(50);
      dir       = new VariableArea_C(20);
      file      = new VariableArea_C(20);
      ext       = new VariableArea_C(20);
    }
    ~UrlNameAnl_C(){
      delete kind;
      delete site;
      delete dir;
      delete file;
      delete ext;
    }
    void set(char *url);
    char *getKind (){return(kind->cc_str());}
    char *getSite (){return(site->cc_str());}
    char *getDir  (){return(dir->cc_str ());}
    char *getFile (){return(file->cc_str());}
    char *getExt  (){return(ext->cc_str ());}
};

// # File Name Separate Class ------------------
class FileNameAnl_C{
  public:
    VariableArea_C *drv;
    VariableArea_C *dir;
    VariableArea_C *file;
    VariableArea_C *ext;
    VariableArea_C *work;
    tokenAnl_C     *token;
    FileNameAnl_C(){
      drv       = new VariableArea_C(5);
      dir       = new VariableArea_C(50);
      file      = new VariableArea_C(20);
      ext       = new VariableArea_C(5);
      token     = new tokenAnl_C;
      token->setKey(':' ); token->setKeyFlag();
      token->setKey('.' ); token->setKeyFlag();
      token->setKey('\\'); token->setKeyFlag();
      token->sjisMode();
    }

    ~FileNameAnl_C(){
      delete drv;
      delete dir;
      delete file;
      delete ext;
      delete token;
    }


    void set(char *path);
    char *getDrive (){return(drv->cc_str ());}
    char *getDir   (){return(dir->cc_str ());}
    char *getFile  (){return(file->cc_str());}
    char *getExt   (){return(ext->cc_str ());}

    // - ŒÝŠ· -----
    void Set(char *path){set(path);}
    char *DrvGet   (){return(drv->cc_str ());}
    char *DirGet   (){return(dir->cc_str ());}
    char *FileGet  (){return(file->cc_str());}
    char *ExtGet   (){return(ext->cc_str ());}

};

// - ŒÝŠ· -----
#define FileNameSep_T FileNameAnl_C

#endif // _ZFILENAME_ANL_H_


