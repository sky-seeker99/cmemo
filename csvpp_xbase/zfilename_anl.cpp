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

#include <stdio.h> 
#include "zfilename_anl.h"


// # Url Name Separate Class ------------------
void UrlNameAnl_C::set(char *str){
  int kindPtr  = -1;
  int sitePtr  = -1;
  int dirPtr   = -1;
  int filePtr  = -1;
  int extPtr   = -1;
  int startPtr = 0;
  int endPtr   = 0;

  // init
  kind->set(0,0x00);
  site->set(0,0x00);
  dir->set (0,0x00);
  file->set (0,0x00);
  ext->set (0,0x00);

  // token anl
  int len = strlen(str);
  int state = 0;
  for(int i=0;i<len;i++){
    char w = *(str+i);
    if (state == 0){  // ':' or '/' Search
      if (w == ':'){
        kindPtr = 0;
        sitePtr = i+1; 
        state   = 1;  // site search
      }
      else if (w == '/'){
        if (i == 0){
          sitePtr = 0; 
          state   = 1;  // site search
        }
        else {  // error
          dirPtr = 0;
          break;
        }
      }
    }
    else if (state == 1){  // site search not '/'
      if (w != '/'){
        state = 2;
      }
    }
    else if (state == 2){  // site string '/' search
      if (w == '/'){
        if (dirPtr == -1){dirPtr = i;}
        filePtr = i+1;
      }
      else if (w == '.'){
        extPtr = i+1;
      }
    }
  }

  if (extPtr < filePtr){extPtr = -1;}

  // kind name
  if (kindPtr != -1){
    kind->init();
	kind->set_n(str+kindPtr,sitePtr-kindPtr);
	kind->set(0x00);
  }

  // site name
  if (sitePtr != -1){
	if (dirPtr == -1){
	  site->set_string(str+sitePtr);
	}
	else {
	  site->init();
	  site->set_n(str+sitePtr,dirPtr-sitePtr);
  	  site->set(0x00);
	}
  }

  // dir name
  if (dirPtr != -1){
    dir->init();
    dir->set_n(str+dirPtr,filePtr-dirPtr);
    dir->set(0x00);
  }

  // file name
  if (filePtr != -1){
	if (extPtr == -1){
      file->set_string(str+filePtr);
    }
    else {
      file->init();
      file->set_n(str+filePtr,extPtr-filePtr-1);
      file->set(0x00);
    }
  }

  // ext name
  if (extPtr != -1){
    ext->set_string(str+extPtr);
  }


}


// # File Name Separate Class ------------------
void FileNameAnl_C::set(char *str){
  int drvPtr = -1;
  int dirPtr = -1;
  int extPtr = -1;
  int startPtr = 0;
  int endPtr   = 0;
  char *w;

  // init
  drv->set (0,0x00);
  dir->set (0,0x00);
  file->set(0,0x00);
  ext->set (0,0x00);

  // token anl
  token->set(str);
  for(int i=0;i<token->getNum();i++){
    w = token->get(i);
    if      ((*w == ':' ) && (drvPtr == -1)){drvPtr = i;}
    else if ( *w == '\\')                   {dirPtr = i; extPtr=-1;}
    else if ( *w == '.' )                   {extPtr = i;}
  }

  // drive name
  if (drvPtr != -1){
    for(int i=0;i<=drvPtr;i++){
      w = token->get(i);
      if (w == NULL){break;}
      drv->cat_string(w);
    }
  }

  // dir name
  if (drvPtr != -1){startPtr = drvPtr+1;}
  else             {startPtr = 0;}

  if (dirPtr != -1){
    for(int i=startPtr;i<=dirPtr;i++){
      w = token->get(i);
      if (w == NULL){break;}
      dir->cat_string(w);
    }
  }

  // file name
  if      (dirPtr != -1){startPtr = dirPtr+1;}
  else if (drvPtr != -1){startPtr = drvPtr+1;}
  else                  {startPtr = 0;}

  if (extPtr != -1){
    endPtr=extPtr-1;
  }
  else{
    endPtr=token->getNum();
  }

  for(int i=startPtr;i<=endPtr;i++){
    w = token->get(i);
    if (w == NULL){break;}
    file->cat_string(w);
  }
  
  // ext name
  if (extPtr != -1){
    for(int i=extPtr+1;i<=token->getNum();i++){
      w = token->get(i);
      if (w == NULL){break;}
      ext->cat_string(w);
    }
  }
}


