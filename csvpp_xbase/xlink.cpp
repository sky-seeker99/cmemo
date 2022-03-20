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
#include <shlobj.h>
#include "xlink.h"


link_C::link_C(){
  IShellLink      *psl;
  IPersistFile    *ppf;

  g_ppf = NULL;
  g_psl = NULL;
  wfd   = NULL;
  okFlg = false;
  hRes = CoInitialize( NULL );
  if ((hRes == E_OUTOFMEMORY) || (hRes == E_INVALIDARG) || (hRes == E_UNEXPECTED)){return;}
  hRes = CoCreateInstance( CLSID_ShellLink, NULL,CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&psl );
  if ((hRes == CLASS_E_NOAGGREGATION) || (hRes == REGDB_E_CLASSNOTREG)){return;}
  hRes = psl->QueryInterface(IID_IPersistFile,(LPVOID *)&ppf);
  if (hRes != S_OK){return;}
  okFlg = true;
  wfd   = new WIN32_FIND_DATA;
  g_ppf = ppf;
  g_psl = psl;
  linkCheckFlg = false;
}

link_C::~link_C(){
  IShellLink      *psl;
  IPersistFile    *ppf;
  ppf = (IPersistFile *)g_ppf;
  psl = (IShellLink   *)g_psl;

  if (wfd != NULL){delete wfd;}
  if (ppf != NULL){ppf->Release();}
  if (psl != NULL){psl->Release();}
  CoUninitialize();
}


char *link_C::Get(char *file){
  if (okFlg == false){return(NULL);}
  IShellLink      *psl;
  IPersistFile    *ppf;
  ppf = (IPersistFile *)g_ppf;
  psl = (IShellLink   *)g_psl;

  OLECHAR wsz[MAX_PATH];
  MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, file, -1,wsz, MAX_PATH );
  if (ppf->Load(wsz, STGM_READ ) != S_OK){return(NULL);}
  if (linkCheckFlg) {
    if (psl->Resolve( NULL, SLR_NO_UI) != NOERROR ){return(NULL);}  // Ver 1.46
  }
  strcpy(dest_link,file);
  hRes = psl->GetPath(dest_link, MAX_PATH,wfd,SLGP_UNCPRIORITY);
  if (!SUCCEEDED(hRes)){return(NULL);}
  return(dest_link);
}

bool link_C::dirHantei(){
  if (wfd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){return(true);}
  return(false);
}




