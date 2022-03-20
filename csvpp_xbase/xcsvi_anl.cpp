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
//  CSV File Input Control    Ver 1.06
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "xcsvi_anl.h"

// # Csv Interface(csv) ----------------------
void CsvAnl_C::SelClr(){
  unsigned int i;
  if (sel == NULL) {return;}
  for(i=0;i<(unsigned int)sel_su;++i){
	if (*(sel+i) != NULL) {delete [] *(sel+i);}
  }
  delete [] sel;
  delete [] dblFlg;
  sel    = NULL;
  dblFlg = NULL;
  sel_su = -1;
}

// # blank check ----------
bool CsvAnl_C::blankCheck(unsigned char *buff){
  if (buff == NULL) {return(true);}
  for(;;){
	if (*buff == 0x00) {break;}
	if (*buff != ' ') {return(false);}
	buff++;
  }
  return(true);
}

// # Sel Read  ----------
char *CsvAnl_C::CsvSelRd(bool pushFlg){
  char *buff = CsvSel_Rd(g_sel_ptr);

  if (buff == NULL) {return(NULL);}
  g_sel_ptr++;
  if (pushFlg == SP_PUSH) {
	sp_push(buff);
	sp_push2(buff);
  }
  return(buff);

}


char *CsvAnl_C::CsvSel_Rd(int clm){
  if (clm < 0      ) {return(NULL);}
  if (clm >= sel_su) {return(NULL);}
  return(*(sel + clm));
}

// # Sel Store  ----------
void CsvAnl_C::CsvSelStore(char *src,int sel_ptr,int len){
  *(sel + sel_ptr) = new char[len+1];
  mem_cpy(*(sel + sel_ptr),src,len);
  *(*(sel + sel_ptr) + len) = 0x00;
/*
  if (*(dblFlg+sel_ptr) == false) {return;}
  char *work =  *(sel + sel_ptr);
  int w_len = strlen(work);
  *(work+w_len-1) = 0x00;
  for(int i=0;i<w_len-1;i++){
    *(work+i) = *(work+i+1);
  }
*/
}



// # CSV File Read --------------------
int CsvAnl_C::Exec(char *buff){
  unsigned int  i;
  unsigned int  j;
  int  sel_ptr;
  int  len;
  int  mode;
  char *des;

  SelClr();

/*
  for(i=0;i<strlen(buff);++i){
	if (SJisCheck(buff,i) == true){continue;}
	if ((buff[i] < ' ') || (buff[i] > '~')){
	  if (((unsigned char)buff[i] < 0xa1) || ((unsigned char)buff[i] > 0xdf)){buff[i] = 0x20;}
	}
  }
*/

  if (buff[0] == 0x00) {sel_su = 0; return(0);}

  // Sel Count
  sel_su = 1;
  unsigned int length = (unsigned int)strlen(buff);
  for(mode=0,i=0;i<length;++i){
	if (*(buff+i) == '"'){
	  if (mode == 0){mode = 1;}
	  else          {mode = 0;}
	  continue;
	}
//	if ((*(buff+i) == ',') && (mode == 0)) {++sel_su;}
	if ((*(buff+i) == sep) && (mode == 0)) {++sel_su;}    

  }
  sel    = new char*[sel_su];
  dblFlg = new bool[sel_su];

  for(i=0;i<(unsigned int)sel_su;++i) {*(sel+i) = NULL; *(dblFlg+i) = false;}

  // Sel Data Setting
  des = &buff[0];
  sel_ptr=0;
  len=0;
  mode=0;
  for(i=0;i<length;++i){
    if (buff[i] == 0x00) {break;}
	if ((buff[i] == sep) && (mode == 0)){
	  CsvSelStore(des,sel_ptr,len);
	  len = 0;
	  ++sel_ptr;
	  des = &buff[i+1];
	  continue;
	}

	if (buff[i] == '"'){
	  if ((buff[i+1] == '"') && (mode == 1)){
	    if (dbl_cut_mode){
          int buff_len = strlen(buff);
		  for(j=i+1;j<(unsigned int)buff_len;++j){
            buff[j] = buff[j+1];
          }

	    } else {
          i++;
          len++;
        }
	  } else {
		if (mode == 0){mode = 1;}
		else          {mode = 0;}
		*(dblFlg+sel_ptr) = true;
		//buff[i] = ' ';
	  }
	}
	++len;
  }
  CsvSelStore(des,sel_ptr,len);
  g_sel_ptr = 0;

#ifdef DEBUG_CSV
for(int i=0;i<sel_su;i++){
  char *w = CsvSel_Rd(i);
  if (w == NULL) {
    printf("%d:NULL\n",i);
  } else {
    printf("%d:%s\n",i,w);
  }

}
#endif

  // double delete
  if (dbl_cut_mode){
    for(int i=0;i<sel_su;i++){
      char *str = *(sel+i);
      if (*(dblFlg+i) == false){continue;}
      int len = strlen(str);
      if (len<2) {*(dblFlg+i) == false; continue;}
      sp_push(str); sp_push2(str);
      len = strlen(str);
      if ((*str != '"') || (*(str+len-1) != '"')){*(dblFlg+i) == false; continue;}
      *(str+len-1) = 0x00;
      len = len-1;
      for(int k=0;k<len;k++){*(str+k)=*(str+k+1);}
    }
  }

  return(0);
}

void CsvAnl_C::mem_cpy(char *des,char *src,int len){
  int i;
  for(i=0;i<len;++i){*(des + i) = *(src + i);}
}


// - meaasage ------
char *CsvAnl_C::getClmMsg(int clm){
  char a[2];
  a[0] = clm+'A';
  a[1] = 0x00;

  msg_clm->set_string("[");
  msg_clm->cat_string(a);
  msg_clm->int_cat(line);
  msg_clm->cat_string("]");
  return(msg_clm->cc_str());
}

char *CsvAnl_C::getErrMsg(char *p_msg,int clm) {
  msg->set_string("CSV File Read Error:");
  msg->cat_string(p_msg);
  msg->cat_string(getClmMsg(clm));
  msg->cat_string(":[");
  char *w = CsvSel_Rd(clm);
  if (w==NULL) {
	msg->cat_string("NULL");
  }
  else {
	msg->cat_string(w);
  }
  msg->cat_string("]");
  msg->cat_string(" FILE=");
  msg->cat_string(file_name->cc_str());
  return(msg->cc_str());
}
