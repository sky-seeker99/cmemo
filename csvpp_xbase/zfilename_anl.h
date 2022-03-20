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


