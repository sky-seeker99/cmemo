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
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include <dir.h>

#include "ztool.h"
#include "zfilename_anl.h"
#include "xcsvi_anl.h"
#include "xread64.h"
#include "xwrite64.h"
#include "zcsvfile.h"

#define VERSION "choi memo II Version 0.99"

#include "_cmemo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------

__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
//  Form1->Caption = VERSION;
  AnsiString cmd;
  FileNameSep_T *fn;
  fn  = new FileNameSep_T();
  config_file = new sChar("");
  own_dir     = new sChar("");
  sv_file     = new VariableArea_C(10);
  sv_panel_width = panel_list->Width;

  cmd = ParamStr(0);
  fn->Set(cmd.c_str());
  config_file->cat(fn->DrvGet());
  config_file->cat(fn->DirGet());
  config_file->cat("cmemo2.csv");
  own_dir->cat(fn->DrvGet());
  own_dir->cat(fn->DirGet());
  chgFlg = false;
  csvi = new CsvAnl_C;

  confFileRead();

  DragAcceptFiles(Handle, true);
  TrayMessage(NIM_ADD);
  TrayMessage(NIM_MODIFY);
  visual_flg = true;
  delete fn;
}
//---------------------------------------------------------------------------


__fastcall TForm1::~TForm1()
{
  confFileWrite();
  delete sv_file;
  delete config_file;
  delete own_dir;
  TrayMessage(NIM_DELETE);
  delete csvi;
}

//---------------------------------------------------------------------------
void TForm1::saveFile(char *file_name){
  Write64_C *out_fp = new Write64_C(file_name,"wt",10240);
  VariableArea_C *file = new VariableArea_C(10);
  AnsiString w;

  writeText(sv_file->cc_str());
  for(int k=0;k<list->Items->Count;k++){
    out_fp->write("#head,");
    w = list->Items->Strings[k];
	out_fp->csvWrite((unsigned char *)w.c_str());
    out_fp->writeln("");
    // text file read & write
	file->set_string(own_dir->c_str());
	file->cat_string(w.c_str());
    file->cat_string(".txt");
	Read64_C *in_fp = new Read64_C((char *)file->c_str(),"rt",10240);
    if (in_fp->okCheck()) {
      TEXT_READ_LOOP(in_fp,str)
        out_fp->write("#body,");
        out_fp->csvWrite((unsigned char *)str);
        out_fp->writeln("");
      LOOP_END
    }
    delete in_fp;
    out_fp->writeln("#tail");
  }
  
  delete out_fp;
  delete file;
}

void TForm1::loadFile(char *file_name){
  Read64_C *in_fp = new Read64_C(file_name,"rt",10240);
  VariableArea_C *file = new VariableArea_C(10);
  if (in_fp->okCheck()==false) {return;}
  bool firstFlag = true;
  Write64_C *out_fp = NULL;

  writeText(sv_file->cc_str());
  TEXT_READ_LOOP(in_fp,str)
    csvi->Exec(str);
    int mode=0;
    CSV_LOOP(csvi,ustr,SP_PUSH)
      char *w_str = (char *)ustr;
      if (mode == 0){
        if (strcmp(w_str,"#body") == 0){mode = 1; continue;}
        if (strcmp(w_str,"#head") == 0){mode = 2; continue;}
        if (strcmp(w_str,"#tail") == 0){mode = 3; continue;}
      }
      else if (mode == 1){
        if (out_fp != NULL){out_fp->writeln(w_str);}
        mode = 0;
        continue;
      }
      else if (mode == 2){
        if (out_fp != NULL){delete out_fp;}
        file->set_string(own_dir->c_str());
        file->cat_string(w_str);
        file->cat_string(".txt");
        out_fp = new Write64_C(file->cc_str(),"wt",10240);
        if (firstFlag){
          list->Items->Clear();
          firstFlag = false;
        }
        list->Items->Add(w_str);
        
        mode = 0;
        continue;
      }
      else {
        if (out_fp != NULL){delete out_fp;}
        out_fp = NULL;
        mode = 0;
        continue;
      }
    LOOP_END
  LOOP_END
  if (out_fp != NULL){delete out_fp;}
  delete in_fp;
  delete file;
  if (firstFlag==false){
    list->ItemIndex = 0;
    edit->Text = list->Items->Strings[list->ItemIndex];
    list->Selected[list->ItemIndex]=1;
  }
  readText(edit->Text.c_str());
}


void TForm1::windowClose(){
  visual_flg = false;
  Hide();
}

void TForm1::fontChange(bool listFlag){
  if (listFlag) {
    font_diag->Font = list->Font;
  }
  else {
    font_diag->Font = memo->Font;
  }

  if (font_diag->Execute() == false){return;}
  if (listFlag) {
    list->Font = font_diag->Font;
  }
  else {
    memo->Font = font_diag->Font;
  }
  Form1->Visible = false;
  Form1->Visible = true;
}

void TForm1::colorChange(bool listFlag){
  if (listFlag) {
    color_diag->Color = list->Color;
  }
  else {
    color_diag->Color = memo->Color;
  }

  if (color_diag->Execute() == false){return;}
  if (listFlag) {
    list->Color = color_diag->Color;
  }
  else {
    memo->Color = color_diag->Color;
  }
  Form1->Visible = false;
  Form1->Visible = true;
}

void TForm1::downMemo(){
  if (list->ItemIndex >= list->Items->Count-1){return;}
  String sv = list->Items->Strings[list->ItemIndex+1];
  list->Items->Strings[list->ItemIndex+1] = list->Items->Strings[list->ItemIndex];
  list->Items->Strings[list->ItemIndex]   = sv;
  list->ItemIndex++;
  list->Selected[list->ItemIndex]=1;
}

void TForm1::upMemo(){
  if (list->ItemIndex <= 0){return;}
  String sv = list->Items->Strings[list->ItemIndex-1];
  list->Items->Strings[list->ItemIndex-1] = list->Items->Strings[list->ItemIndex];
  list->Items->Strings[list->ItemIndex]   = sv;
  list->ItemIndex--;
  list->Selected[list->ItemIndex]=1;
}

void TForm1::newMemo(){
  writeText(sv_file->cc_str());
  AnsiString sv = list->Items->Strings[list->Items->Count-1];
  list->Items->Add(sv);
  for(int k=list->Items->Count-2;k>=list->ItemIndex;k--){
    list->Items->Strings[k+1] = list->Items->Strings[k];
  }
  list->Items->Strings[list->ItemIndex] = edit->Text;
  readText(edit->Text.c_str());
  list->Selected[list->ItemIndex]=1;
}

void TForm1::renameMemo(){
  int sv = list->ItemIndex;
  writeText(sv_file->cc_str());
  chgFlg = true;
  list->Items->Strings[list->ItemIndex] = edit->Text;
  writeText(edit->Text.c_str());
  readText(edit->Text.c_str());
  list->ItemIndex = sv;
  list->Selected[sv]=1;
}

void TForm1::delMemo(){
  int count = list->Items->Count;
  int sv = list->ItemIndex;
  for(int k=list->Items->Count-1;k>=0;k--){
    if ((list->Selected[k]) && (count > 1)){
      list->Items->Delete(k);
      count--;
    }
  }

  if (sv > list->Items->Count-1) {
    sv = list->Items->Count-1;
  }

  writeText(sv_file->cc_str());
  edit->Text = list->Items->Strings[sv];
  list->ItemIndex = sv;
  readText(edit->Text.c_str());
  list->Selected[sv]=1;
}

void TForm1::nextMemo(){
  writeText(sv_file->cc_str());
  if (list->ItemIndex >= list->Items->Count-1){return;}
  list->Selected[list->ItemIndex]=0;
  list->ItemIndex++;
  list->Selected[list->ItemIndex]=1;
  String sv = list->Items->Strings[list->ItemIndex];
  readText(sv.c_str());
}

void TForm1::beforeMemo(){
  writeText(sv_file->cc_str());
  if (list->ItemIndex <= 0){return;}
  list->Selected[list->ItemIndex]=0;
  list->ItemIndex--;
  list->Selected[list->ItemIndex]=1;
  String sv = list->Items->Strings[list->ItemIndex];
  readText(sv.c_str());
}


//---------------------------------------------------------------------------

void TForm1::confFileRead(){
  CsvInf_T *csvinf = new CsvInf_T(config_file->c_str());
  char *str;

  if (csvinf->CsvError_Rd() == 0){
    for(;;){
      if (csvinf->CsvFgets() == -1){break;}
      if (csvinf->CsvSelSu_Rd() == 0) {continue;}
      if ((str = csvinf->CsvSel_Rd(0)) == NULL) {continue;}
      char *w_str = csvinf->CsvSel_Rd(1);
      sp_push(str);
      sp_push2(str);
      if (strcmp(str,"#height"     ) == 0){Height       = SujiConvEx(w_str); continue;}
      if (strcmp(str,"#width"      ) == 0){Width        = SujiConvEx(w_str); continue;}
      if (strcmp(str,"#top"        ) == 0){Top          = SujiConvEx(w_str); continue;}
      if (strcmp(str,"#left"       ) == 0){Left         = SujiConvEx(w_str); continue;}
      if (strcmp(str,"#back_color" ) == 0){
        int ii = SujiConvEx(w_str);
        if (ii < 0){ii = 16777215;}
        memo->Color = (TColor)ii;
        continue;
        }
      if (strcmp(str,"#back_color_list" ) == 0){
        int ii = SujiConvEx(w_str);
        if (ii < 0){ii = 16777215;}
        list->Color = (TColor)ii;
        continue;
        }
      if (strcmp(str,"#font_size"  ) == 0){
        memo->Font->Size = SujiConvEx(w_str);
        continue;
        }
      if (strcmp(str,"#font_style" ) == 0){
        memo->Font->Style = (TFontStyles)SujiConvEx(w_str);
        continue;
        }
      if (strcmp(str,"#font_color" ) == 0){
        int ii = SujiConvEx(w_str);
        if (ii < 0){ii = 0;}
        memo->Font->Color = (TColor)ii;
        continue;
        }
      if (strcmp(str,"#font_name") == 0){
        memo->Font->Name = w_str;
        continue;
        }
      if (strcmp(str,"#font_size_list"  ) == 0){
        list->Font->Size = SujiConvEx(w_str);
        continue;
        }
      if (strcmp(str,"#font_style_list" ) == 0){
        list->Font->Style = (TFontStyles)SujiConvEx(w_str);
        continue;
        }
      if (strcmp(str,"#font_color_list" ) == 0){
        int ii = SujiConvEx(w_str);
        if (ii < 0){ii = 0;}
        list->Font->Color = (TColor)ii;
        continue;
        }
      if (strcmp(str,"#font_name_list") == 0){
        list->Font->Name = w_str;
        continue;
        }
      if (strcmp(str,"#stay"      ) == 0){
        if (strcmp(w_str,"0") == 0){stay->Checked = false; FormStyle = fsNormal;}
        else                       {stay->Checked = true;  FormStyle = fsStayOnTop;}
        continue;
        }
      if (strcmp(str,"#listFlag"      ) == 0){
        if (strcmp(w_str,"0") == 0){listFlag->Checked = false;}
        else                       {listFlag->Checked = true; }
        continue;
        }
      if (strcmp(str,"#sv_panel_width"      ) == 0){
        sv_panel_width = SujiConvEx(w_str);
        continue;
        }
      if (strcmp(str,"#items") == 0){
        list->Items->Add(w_str);
        continue;
      }
      if (strcmp(str,"#items_no") == 0){
        list->ItemIndex = SujiConvEx(w_str);
        continue;
      }
    }
  }
  if (listFlag->Checked) {
    panel_list->Width = sv_panel_width;
    split->Width = 5;
  } else {
    panel_list->Width = 0;
    split->Width = 0;
  }


  if (list->Items->Count == 0){list->Items->Add("default"); list->ItemIndex = 0;}
  edit->Text = list->Items->Strings[list->ItemIndex];
  list->Selected[list->ItemIndex]=1;

  readText(edit->Text.c_str());
  DragAcceptFiles(Handle, true);
  TrayMessage(NIM_ADD);
  TrayMessage(NIM_MODIFY);
  delete csvinf;
}

void TForm1::readText(char *p_file){
  memo_header->Caption = p_file;
  memo->Visible = false;
  memo->Lines->Clear();
  sChar *file = new sChar(own_dir);
  file->cat(p_file);
  file->cat(".txt");
  Read64_C *in_fp = new Read64_C(file->c_str(),"rt",10240);
  if (in_fp->okCheck()) {
    TEXT_READ_LOOP(in_fp,str)
      memo->Lines->Add(str);
    LOOP_END
  }
  delete in_fp;
  memo->Visible = true;
  sv_file->set_string(p_file);
  chgFlg = false;
}



// # config file write #####
void TForm1::confFileWrite(){
  FILE *fp;
  writeText(sv_file->cc_str());
  fp = fopen(config_file->c_str(),"wt");
  fprintf(fp,"#height,%d\n",Height);
  fprintf(fp,"#width,%d\n",Width);
  fprintf(fp,"#top,%d\n",Top);
  fprintf(fp,"#left,%d\n",Left);
  fprintf(fp,"#back_color,%d\n",memo->Color);
  fprintf(fp,"#font_color,%d\n",memo->Font->Color );
  fprintf(fp,"#font_style,%d\n",memo->Font->Style );
  fprintf(fp,"#font_size,%d\n",memo->Font->Size );
  fprintf(fp,"#font_name,%s\n",memo->Font->Name.c_str());
  fprintf(fp,"#back_color_list,%d\n",list->Color);
  fprintf(fp,"#font_color_list,%d\n",list->Font->Color );
  fprintf(fp,"#font_style_list,%d\n",list->Font->Style );
  fprintf(fp,"#font_size_list,%d\n",list->Font->Size );
  fprintf(fp,"#font_name_list,%s\n",list->Font->Name.c_str());
  fprintf(fp,"#sv_panel_width,%d\n",sv_panel_width );
  if (stay->Checked){fprintf(fp,"#stay,1\n");}
  else              {fprintf(fp,"#stay,0\n");}
  if (listFlag->Checked){fprintf(fp,"#listFlag,1\n");}
  else                  {fprintf(fp,"#listFlag,0\n");}
  for(int i=0;i<list->Items->Count;i++){
    fprintf(fp,"#items,%s\n",list->Items->Strings[i]);
  }
  fprintf(fp,"#items_no,%d\n",list->ItemIndex );


  fclose(fp);
}

void TForm1::writeText(char *p_file){
  if (chgFlg == false) {return;}
  chgFlg = false;
  sChar *file = new sChar(own_dir);
  file->cat(p_file);
  file->cat(".txt");
  Write64_C *out_fp = new Write64_C(file->c_str(),"wt",10240);
  if (out_fp->okCheck()) {
    for(int i=0;i<memo->Lines->Count;i++){
      String a = memo->Lines->Strings[i];
      char *str = a.c_str();
      out_fp->writeln(str);
    }
  }
  delete out_fp;
}


void __fastcall TForm1::WMQueryEndSession(TWMQueryEndSession &WMQES)
{
//ここにＷindows終了直前にやりたいことを書く
//最後に、Ｗindowsが終了してもいいかを指定

confFileWrite();
    WMQES.Result = TRUE;    //Ｗindowsに「終了してもいいよ」と伝える
//  WMQES.Result = FALSE;   //Ｗindowsの終了を禁止する
}



void __fastcall TForm1::WMDropFiles(TWMDropFiles & Msg)
{
  char Buff[MAX_PATH]; //ファイル名
  int Count; //Dropされたファイル数

  Count = DragQueryFile((void*)Msg.Drop, -1, NULL, NULL ); //ファイルの数を得る
  for (int i = 0; i < Count; i++) {  //ファイルの数だけ繰り返す
    DragQueryFile((void*)Msg.Drop, i, Buff, sizeof(Buff)); //i番目のファイル名をBuffに格納
    // memo->Lines->Add(Buff);
    if (fileNameOnly->Checked  == false){
      memo->Lines->Insert(memo->CaretPos.y,Buff);
    }
    else {
      FileNameSep_T *fn = new FileNameSep_T();
      fn->Set(Buff);
      sChar *w = new sChar(fn->FileGet());
      if (strcmp(fn->ExtGet(),"")!=0) {
        w->cat(".");
        w->cat(fn->ExtGet());
      }
      memo->Lines->Insert(memo->CaretPos.y,w->c_str());
      delete fn;
      delete w;
    }
  }

  DragQueryFile((void*)Msg.Drop, 0, Buff, sizeof(Buff));
  DragFinish((void*)Msg.Drop); //終了処理
  TForm::Dispatch(&Msg);

}
void __fastcall TForm1::WMLButtonDown(TWMLButtonDown & Msg)
{
    //TODO この下にコードを追加してください
    SendMessage(Handle,WM_SYSCOMMAND,SC_MOVE | 2,0);
}

void __fastcall TForm1::CreateParams(TCreateParams& Params)
{
    //TODO この下にコードを追加してください
TForm::CreateParams(Params);
Params.Style |= WS_THICKFRAME;


}


void __fastcall TForm1::Quit1Click(TObject *Sender)
{
exit(0);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TForm1::DrawItem(TMessage& Msg)
{
     IconDrawItem((LPDRAWITEMSTRUCT)Msg.LParam);
     TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MyNotify(TMessage& Msg)
{
    POINT MousePos;

    switch(Msg.LParam)
    {
        case WM_RBUTTONUP:
            if (GetCursorPos(&MousePos))
            {
                PopupMenu1->PopupComponent = Form1;
                SetForegroundWindow(Handle);
                PopupMenu1->Popup(MousePos.x, MousePos.y);
            }
            break;
        case WM_LBUTTONUP:
           if (visual_flg == false){visual_flg = true;   Show();}
           else                    {visual_flg = false;  Hide(); /*iniFileSave(); */ }
           break;
        default:
           break;
    }
    TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------
//void __fastcall TForm1::FormDestroy(TObject *Sender)
//{
//  TrayMessage(NIM_DELETE);
//}
//---------------------------------------------------------------------------
bool __fastcall TForm1::TrayMessage(DWORD dwMessage)
{
   NOTIFYICONDATA tnd;
   PSTR pszTip;

   pszTip = TipText();

   tnd.cbSize          = sizeof(NOTIFYICONDATA);
   tnd.hWnd            = Handle;
   tnd.uID             = IDC_MYICON;
   tnd.uFlags          = NIF_MESSAGE | NIF_ICON | NIF_TIP;
   tnd.uCallbackMessage = MYWM_NOTIFY;

   if (dwMessage == NIM_MODIFY)
    {
        tnd.hIcon       = (HICON)IconHandle();
        if (pszTip)
           lstrcpyn(tnd.szTip, pszTip, sizeof(tnd.szTip));
        else
        tnd.szTip[0] = '\0';
    }
   else
    {
        tnd.hIcon = NULL;
        tnd.szTip[0] = '\0';
    }

   return (Shell_NotifyIcon(dwMessage, &tnd));
}
//---------------------------------------------------------------------------
HICON __fastcall TForm1::IconHandle(void)
{
        return (Image1->Picture->Icon->Handle);
//        return (Image2->Picture->Icon->Handle);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ToggleState(void)
{
    TrayMessage(NIM_MODIFY);
}
//---------------------------------------------------------------------------
PSTR __fastcall TForm1::TipText(void)
{
        return (VERSION);
}
//---------------------------------------------------------------------------

LRESULT IconDrawItem(LPDRAWITEMSTRUCT lpdi)
{
    HICON hIcon;

    hIcon = (HICON)LoadImage(g_hinst, MAKEINTRESOURCE(lpdi->CtlID), IMAGE_ICON,
        16, 16, 0);
    if (!hIcon)
        return(FALSE);

    DrawIconEx(lpdi->hDC, lpdi->rcItem.left, lpdi->rcItem.top, hIcon,
        16, 16, 0, NULL, DI_NORMAL);

    return(TRUE);
}


//---------------------------------------------------------------------------


void __fastcall TForm1::memoDblClick(TObject *Sender)
{
windowClose();

}
//---------------------------------------------------------------------------



void __fastcall TForm1::memoChange(TObject *Sender)
{
chgFlg = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::listClick(TObject *Sender)
{
  writeText(sv_file->cc_str());
  edit->Text = list->Items->Strings[list->ItemIndex];
  readText(edit->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
downMemo();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button3Click(TObject *Sender)
{
delMemo();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button6Click(TObject *Sender)
{
renameMemo();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
newMemo();
}


void __fastcall TForm1::Button2Click(TObject *Sender)
{
upMemo();    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::New1Click(TObject *Sender)
{
newMemo();    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Del1Click(TObject *Sender)
{
delMemo();    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Rename1Click(TObject *Sender)
{
renameMemo();
}
//---------------------------------------------------------------------------







void __fastcall TForm1::copy1Click(TObject *Sender)
{
memo->CopyToClipboard();    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::paste1Click(TObject *Sender)
{
memo->PasteFromClipboard();    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::cut1Click(TObject *Sender)
{
memo->CutToClipboard();    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::undo1Click(TObject *Sender)
{
memo->Undo();    
}
//---------------------------------------------------------------------------


void __fastcall TForm1::close2Click(TObject *Sender)
{
memo->Lines->Clear();
}
//---------------------------------------------------------------------------










void __fastcall TForm1::returncrlf2Click(TObject *Sender)
{
chgFlg = true;
writeText(sv_file->cc_str());
readText(sv_file->cc_str());    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::backcolor2Click(TObject *Sender)
{
colorChange(false);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::font1Click(TObject *Sender)
{
fontChange(false);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::versionClick(TObject *Sender)
{
  MessageDlg(VERSION, mtInformation  ,TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::listFlagClick(TObject *Sender)
{
  if (listFlag->Checked){listFlag->Checked = false; sv_panel_width = panel_list->Width; panel_list->Width = 0; split->Width=0;}
  else                  {listFlag->Checked = true;  panel_list->Width = sv_panel_width; split->Width=5; }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::stayClick(TObject *Sender)
{
  if (Form1->FormStyle == fsNormal){stay->Checked = true;  Form1->FormStyle = fsStayOnTop;}
  else                             {stay->Checked = false; Form1->FormStyle = fsNormal;}

  DragAcceptFiles(Handle, true);
  TrayMessage(NIM_ADD);
  TrayMessage(NIM_MODIFY);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::fileNameOnlyClick(TObject *Sender)
{
  if (fileNameOnly->Checked  == false){fileNameOnly->Checked = true; }
  else                                {fileNameOnly->Checked = false;}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::show1Click(TObject *Sender)
{
visual_flg = true;
Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::WMCopy(TMessage &Msg)
{
    COPYDATASTRUCT *copydata;
    copydata = (COPYDATASTRUCT*)Msg.LParam;

    AnsiString s = (char *)copydata->lpData;
    csvi->Exec(s.c_str());
    CSV_LOOP(csvi,ustr,SP_PUSH)
      char *str = (char *)ustr;
      if (strcmp(str,"#view") == 0){view_rtn(); continue;}
      if (strcmp(str,"#close") == 0){windowClose(); continue;}
      if (strcmp(str,"#load") == 0){file_rtn(true); continue;}
      if (strcmp(str,"#save") == 0){file_rtn(false); continue;}

    LOOP_END
}

void TForm1::file_rtn(bool readFlag){
    char *file=NULL;

    CSV_LOOP(csvi,ustr,SP_PUSH)
      char *str = (char *)ustr;
      if (file == NULL) {file = str; continue;}
    LOOP_END

    if (file==NULL) {
        return;
    }
    if (readFlag){loadFile(file);}
    else         {saveFile(file);}
}


void TForm1::view_rtn(){
    char *memo_name=NULL;
    char *naiyo=NULL;

    CSV_LOOP(csvi,ustr,SP_PUSH)
      char *str = (char *)ustr;
      if (memo_name == NULL) {memo_name = str; continue;}
      if (naiyo == NULL) {naiyo = str; continue;}
    LOOP_END

    if (naiyo==NULL) {
        return;
    }

  for(int k=0;k<list->Items->Count;k++){
    list->Selected[k]=0;
  }

  int idx = -1;
  for(int k=0;k<list->Items->Count;k++){
    if (strcmp(list->Items->Strings[k].c_str(),memo_name)==0){
      idx = k;
    }
  }

  edit->Text = memo_name;

  if (idx==-1) {
    newMemo();
  }
  else {
    list->ItemIndex = idx;
  }


  writeText(sv_file->cc_str());
  edit->Text = list->Items->Strings[list->ItemIndex];
  readText(edit->Text.c_str());
  list->Selected[list->ItemIndex]=1;

  memo->Visible = false;
  memo->Lines->Clear();

  int len = strlen(naiyo);
  char *w = new char[len+1];
  int mode = 0;
  int k=0;
  for(int i=0;i<len;i++){
    if (mode == 0) {
      if (*(naiyo+i)=='\\') {
        mode = 1;
      }
      else {
		*(w+k++) = *(naiyo+i);
      }
    }
    else {
      if (*(naiyo+i) == 'n') {
        *(w+k++) = 0x0d;
        *(w+k++) = 0x0a;
      }
      else {
        *(w+k++) = '\\';
        *(w+k++) = *(naiyo+i);
      }
      mode = 0;
    }

  }
  *(w+k) = 0x00;
  memo->Lines->Add(w);

  delete [] w;
  memo->Visible = true;


  visual_flg = true;
  Show();
}

void __fastcall TForm1::allmemosave1Click(TObject *Sender)
{
  if (save_di->Execute() == false){return;}
  saveFile(save_di->FileName.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TForm1::loadmemo1Click(TObject *Sender)
{
  if (open_di->Execute() == false) {return;}
  loadFile(open_di->FileName.c_str());
}
//---------------------------------------------------------------------------


void __fastcall TForm1::backcolor_listClick(TObject *Sender)
{
colorChange(true);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::fontlist1Click(TObject *Sender)
{
fontChange(true);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button4Click(TObject *Sender)
{
  if (listFlag->Checked){listFlag->Checked = false; sv_panel_width = panel_list->Width; panel_list->Width = 0; split->Width=0;}
  else                  {listFlag->Checked = true;  panel_list->Width = sv_panel_width; split->Width=5; }
    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
   beforeMemo();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{
  nextMemo();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button9Click(TObject *Sender)
{
  writeText(sv_file->cc_str());
  edit->Text = list->Items->Strings[0];
  readText(edit->Text.c_str());
  list->Selected[list->ItemIndex]=0;
  list->ItemIndex = 0;
  list->Selected[list->ItemIndex]=1;

  if (listFlag->Checked){
	listFlag->Checked = false;
	sv_panel_width = panel_list->Width;
	panel_list->Width = 0;
	split->Width=0;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ResetPosition1Click(TObject *Sender)
{
Form1->Left = 0;
Form1->Top  = 0;
}
//---------------------------------------------------------------------------

