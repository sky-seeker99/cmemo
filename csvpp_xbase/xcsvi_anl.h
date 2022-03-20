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
//  CSV File Input Control     Ver 1.05
//
#ifndef _XCSVI_ANL_
#define _XCSVI_ANL_

#include "ztool.h"

//#define DEBUG_CSV
 
#define SP_PUSH true
#define NON_SP_PUSH false

#ifndef LOOP_END
#define LOOP_END }
#endif

#define CSV_LOOP_(CSVI,STR,FLG) for(;;){ \
								 char *STR = CSVI->CsvSelRd(FLG); \
								 if (STR == NULL)           {break;} \
								 if (CSVI->blankCheck((unsigned char *)STR)) {continue;}

#define CSV_LOOP(CSVI,STR,FLG) for(;;){ \
								 unsigned char *STR = CSVI->CsvSelRd(FLG); \
								 if (STR == NULL)           {break;} \
								 if (CSVI->blankCheck(STR)) {continue;}

#define CSV_LOOP_NOCHK(CSVI,STR,FLG) for(;;){ \
								 unsigned char *STR = CSVI->CsvSelRd(FLG); \
								 if (STR == NULL)           {break;} 





// # Csv Interface(csv) ------------------
class CsvAnl_C {
  public:
    char    **sel;              // Sel Data
    bool     *dblFlg;           // double cortation exsist
    int       sel_su;           // Sel Data Su
    char      sep;              // kugiri moji
    bool      dbl_cut_mode;     // double cut mode = true:cut =false:no cut

  public:
	int       g_sel_ptr;

	// for Message
	int       line;             // Line
	VariableArea_C *msg;        // Message
	VariableArea_C *msg_clm;    // Message
	VariableArea_C *file_name;  // fileName

	CsvAnl_C()  {
	  sel          = NULL;
	  dblFlg       = NULL;
	  sel_su       = -1;
	  g_sel_ptr    = 0;
	  sep          = ',';
	  dbl_cut_mode = true;
	  line         = 0;
	  msg          = new VariableArea_C(256);
	  msg_clm      = new VariableArea_C(16);
	  file_name    = new VariableArea_C(256);
	}

	CsvAnl_C::~CsvAnl_C(){
	  SelClr();
	  delete msg;
	  delete msg_clm;
	  delete file_name;
    }
    bool  blankCheck(unsigned char *buff);
	int   CsvSelSu_Rd(){return(sel_su);}
	char *CsvSel_Rd     (int clm);
	char **CsvSel_Rd    (){return(sel);}
	char *CsvSelRd      (bool pushFlg);
	int   Exec          (char *buff);
	void  tsv_mode      (){sep = '\t';}
	void  dbl_nocut_mode(){dbl_cut_mode = false;}
	bool  dblFlgGet     (){if (g_sel_ptr == 0){return(false);} return(*(dblFlg+g_sel_ptr-1));}
	char **getBody      (){return(sel);}
	void  initLineNum   (){line=0;}
	void  plusLineNum   (){line++;}
	void  setFileName   (char *p_file){file_name->set_string(p_file);}
	char *getClmMsg     (int clm);
	char *getErrMsg     (char *msg,int clm);
  private:
	void SelClr();
	void CsvSelStore(char *src,int sel_ptr,int len);
	void mem_cpy(char *des,char *src,int len);

  };

#endif // _XCSVI_ANL_

