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
//---------------------------------------------------------------------------
#include <stdio.h>
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------

#pragma argsused

void send(HWND hwndTarget,char *str){
  COPYDATASTRUCT data;
  data.dwData = 1;
  data.cbData = strlen(str)+ 1;
  data.lpData = (void*)str;
  SendMessage(hwndTarget,WM_COPYDATA,(WPARAM)Application->Handle,(LPARAM)&data);
}

void msg(){
  printf("cmemo_menu_remote.exe memo_name message [-c] [-s file] [-l file]\n");
  printf("ex)\n");
  printf("cmemo_menu_remote.exe memo1 aaa\\nbbb\n");
  printf("cmemo_menu_remote.exe -c\n");
  printf("cmemo_menu_remote.exe -s c:\\memo_data.csv\n");
  printf("cmemo_menu_remote.exe -l c:\\memo_data.csv\n");
  exit(1);
}

void sendString(char *str){
  HWND hwndTarget = FindWindow(NULL,"ÇøÇÂÇ¢ÉÅÉÇáU");
  if (hwndTarget != NULL) {
    send(hwndTarget,str);
  }
  else {
    AnsiString Msg = "ÇøÇÂÇ¢ÉÅÉÇáU not Found!";
    ShowMessage ( Msg);
  }
}


int main(int argc, char* argv[])
{
  char *p0 = NULL;
  char *p1 = NULL;
  char *save_file = NULL;
  char *load_file = NULL;
  char str[10240];
  bool closeFlg=false;
  printf("ÇøÇÂÇ¢ÉÅÉÇáU remote exec Ver 1.01\n");

  int mode = 0;
  for(int i=1;i<argc;i++){
    if (mode == 0) {
      if (strcmp(argv[i],"-c") == 0) {closeFlg = true; continue;}
      if (strcmp(argv[i],"-s") == 0) {mode = 1; continue;}
      if (strcmp(argv[i],"-l") == 0) {mode = 2; continue;}
      if (p0 == NULL) {p0 = argv[i]; continue;}
      if (p1 == NULL) {p1 = argv[i]; continue;}
    }
    else if (mode == 1) {
      save_file = argv[i];
      mode = 0;
      continue;
    }
    else if (mode == 2) {
      load_file = argv[i];
      mode = 0;
      continue;
    }
  }
  if (closeFlg) {
    sprintf(str,"#close");
    sendString(str);
    return 0;
  }

  if (save_file != NULL) {
    sprintf(str,"#save,%s",save_file);
    sendString(str);
    return 0;
  }
  if (load_file != NULL) {
    sprintf(str,"#load,%s",load_file);
    sendString(str);
    return 0;
  }
  if (p1 == NULL) {
    msg();
  }
  sprintf(str,"#view,%s,%s",p0,p1);
  sendString(str);

return 0;
}
//---------------------------------------------------------------------------
