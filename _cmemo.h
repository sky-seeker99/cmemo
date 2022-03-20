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

#ifndef _cmemoH
#define _cmemoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <stdio.h>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>

#include <Buttons.hpp>


#define MYWM_NOTIFY         (WM_APP+100)
#define IDC_MYICON                     1006
extern HINSTANCE g_hinst;
LRESULT IconDrawItem(LPDRAWITEMSTRUCT lpdi);


//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE 管理のコンポーネント
    TFontDialog *font_diag;
    TColorDialog *color_diag;
	TPopupMenu *PopupMenu1;
	TMenuItem *Quit1;
	TImage *Image1;
    TPanel *panel_list;
	TSplitter *split;
	TPanel *Panel2;
	TListBox *list;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TEdit *edit;
	TButton *Button5;
	TButton *Button6;
    TPopupMenu *PopupMenu2;
    TMenuItem *New1;
    TMenuItem *Del1;
    TMenuItem *Rename1;
    TPopupMenu *PopupMenu3;
    TMenuItem *copy1;
    TMenuItem *paste1;
    TMenuItem *cut1;
    TMenuItem *undo1;
    TMenuItem *setting;
    TMenuItem *close2;
    TMenuItem *version;
    TMenuItem *font1;
    TMenuItem *backcolor2;
    TMenuItem *returncrlf2;
    TMenuItem *listFlag;
    TMenuItem *stay;
    TMenuItem *fileNameOnly;
    TMenuItem *show1;
	TSaveDialog *save_di;
	TOpenDialog *open_di;
	TMenuItem *allmemosave1;
	TMenuItem *loadmemo1;
    TMenuItem *backcolor_list;
    TMenuItem *fontlist1;
    TPanel *Panel1;
    TPanel *Panel3;
    TPanel *Panel4;
    TMemo *memo;
    TButton *Button4;
    TButton *Button7;
    TButton *Button8;
    TLabel *memo_header;
	TButton *Button9;
	TMenuItem *ResetPosition1;
	void __fastcall Quit1Click(TObject *Sender);
	void __fastcall memoDblClick(TObject *Sender);
	void __fastcall memoChange(TObject *Sender);
	void __fastcall listClick(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Button6Click(TObject *Sender);
    void __fastcall Button5Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall New1Click(TObject *Sender);
    void __fastcall Del1Click(TObject *Sender);
    void __fastcall Rename1Click(TObject *Sender);
    void __fastcall copy1Click(TObject *Sender);
    void __fastcall paste1Click(TObject *Sender);
    void __fastcall cut1Click(TObject *Sender);
    void __fastcall undo1Click(TObject *Sender);
    void __fastcall close2Click(TObject *Sender);
    void __fastcall returncrlf2Click(TObject *Sender);
    void __fastcall backcolor2Click(TObject *Sender);
    void __fastcall font1Click(TObject *Sender);
    void __fastcall versionClick(TObject *Sender);
    void __fastcall listFlagClick(TObject *Sender);
    void __fastcall stayClick(TObject *Sender);
    void __fastcall fileNameOnlyClick(TObject *Sender);
    void __fastcall show1Click(TObject *Sender);
	void __fastcall allmemosave1Click(TObject *Sender);
	void __fastcall loadmemo1Click(TObject *Sender);
    void __fastcall backcolor_listClick(TObject *Sender);
    void __fastcall fontlist1Click(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall Button7Click(TObject *Sender);
    void __fastcall Button8Click(TObject *Sender);
	void __fastcall Button9Click(TObject *Sender);
	void __fastcall ResetPosition1Click(TObject *Sender);
private:	// ユーザー宣言
    void __fastcall DrawItem(TMessage& Msg);
    void __fastcall MyNotify(TMessage& Msg);
    bool __fastcall TrayMessage(DWORD dwMessage);
    HICON __fastcall IconHandle(void);
    void __fastcall ToggleState(void);
    PSTR __fastcall TipText(void);
    bool  visual_flg;

public:		// ユーザー宣言
	__fastcall TForm1(TComponent* Owner);
	__fastcall ~TForm1();
	void __fastcall WMDropFiles(TWMDropFiles & Msg);
	void __fastcall WMLButtonDown(TWMLButtonDown & Msg);
	virtual void __fastcall CreateParams(TCreateParams& Params);
	// Windows 終了時のルーチン
	void __fastcall	WMQueryEndSession(TWMQueryEndSession &WMQES);	//これがイベントハンドラになる
    void __fastcall WMCopy(TMessage &Msg);

	sChar *config_file;
	sChar *own_dir;
    VariableArea_C *sv_file;
    int sv_panel_width;
    CsvAnl_C *csvi;
    void view_rtn();
    void file_rtn(bool readFlag);

	bool chgFlg;
	void readText(char *file);
	void writeText(char *file);
	void confFileRead();
	void confFileWrite();
	void listRead();

    void nextMemo();
    void beforeMemo();
    void newMemo();
    void delMemo();
    void renameMemo();
    void upMemo();
    void downMemo();
    void fontChange(bool flag);
    void colorChange(bool flag);
    void windowClose();

    void saveFile(char *file_name);
    void loadFile(char *file_name);

protected:

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WMDropFiles)
		VCL_MESSAGE_HANDLER(WM_LBUTTONDOWN, TWMLButtonDown, WMLButtonDown)
        VCL_MESSAGE_HANDLER(WM_COPYDATA, TMessage, WMCopy)
		MESSAGE_HANDLER(WM_DRAWITEM,TMessage,DrawItem)
		MESSAGE_HANDLER(MYWM_NOTIFY,TMessage,MyNotify)
		MESSAGE_HANDLER(WM_QUERYENDSESSION,TWMQueryEndSession,WMQueryEndSession)
	END_MESSAGE_MAP(TForm)

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
