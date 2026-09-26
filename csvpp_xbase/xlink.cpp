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




