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
// Variable length Area class
//
#include <stdio.h>
#include <string.h>

#include "t_area.h"

template <class T_TYPE>
VariableArea_T<T_TYPE>::VariableArea_T(int size) {
  g_buff      = new T_TYPE[size];
  g_buff_size = size;
  g_buff_ptr  = 0;
}

template <class T_TYPE>
VariableArea_T<T_TYPE>::VariableArea_T(char *p) {
  int size = strlen(p)+1;
  g_buff      = new T_TYPE[size];
  g_buff_size = size;
  g_buff_ptr  = size;
  strcpy((char *)g_buff,p);
}

template <class T_TYPE>
VariableArea_T<T_TYPE>::~VariableArea_T(){
  delete [] g_buff;
}

template <class T_TYPE>
int  VariableArea_T<T_TYPE>::len(){return(strlen((char *)g_buff));}


template <class T_TYPE>
void VariableArea_T<T_TYPE>::mem_cpy(char *des,char *src,int len){
  int i;
  for(i=0;i<len;++i){*(des + i) = *(src + i);}
}

template <class T_TYPE>
T_TYPE *VariableArea_T<T_TYPE>::check(int ptr){
  if (ptr >= g_buff_size-1){
    char *sv_buff = new char[g_buff_size];
    mem_cpy(sv_buff,(char *)g_buff,g_buff_size);
    delete [] g_buff;
    g_buff = new T_TYPE[ptr*2];
    mem_cpy((char *)g_buff,sv_buff,g_buff_size);
    g_buff_size = ptr * 2;
    delete [] sv_buff;
  }
  return(g_buff);
}


template <class T_TYPE>
void VariableArea_T<T_TYPE>::set(int ptr,T_TYPE c){
  T_TYPE *buff = check(ptr);
  *(buff+ptr) = c;
}

template <class T_TYPE>
void VariableArea_T<T_TYPE>::set(T_TYPE c){
  char *buff = (char *)check(g_buff_ptr);
  *(buff+g_buff_ptr) = c;
  g_buff_ptr++;
}

template <class T_TYPE>
void VariableArea_T<T_TYPE>::set_string(char *str){
  char *buff = (char *)check(strlen(str)+1);
  for(g_buff_ptr=0;;g_buff_ptr++){
    char c = *(str + g_buff_ptr);
    *(buff + g_buff_ptr) = c;
    if (c == 0x00){break;}
  }
}

template <class T_TYPE>
void VariableArea_T<T_TYPE>::cat_string(char *str){
  g_buff_ptr = strlen((char *)g_buff);
  char *buff = (char *)check(g_buff_ptr+strlen(str)+1);
  for(int i=0;;g_buff_ptr++,i++){
    char c = *(str + i);
    *(buff + g_buff_ptr) = c;
    if (c == 0x00){break;}
  }
}

template <class T_TYPE>
void VariableArea_T<T_TYPE>::int_set(int p){
  char *buff = (char *)check(24);
  sprintf(buff,"%d",p);
  g_buff_ptr = strlen(buff)+1;
} 


template <class T_TYPE>
void VariableArea_T<T_TYPE>::int_cat(int p){
  char su[25];
  char *buff = (char *)check(24+strlen((char *)g_buff));
  sprintf(su,"%d",p);
  strcat(buff,su);
  g_buff_ptr = strlen(buff)+1;
}


template <class T_TYPE>
char VariableArea_T<T_TYPE>::getChar(int ptr){
  if (g_buff_ptr <= ptr){return(0x00);}
  return(*(g_buff+ptr));
}


//
// Variable Array class  ----------------------------
//

template <class T_TYPE>
void VariableArray_T<T_TYPE>::clear(){
  if (delFlag){
    ARY_LOOP_DIRECT(w,T_TYPE,this)
      if (w != NULL){delete w;}
    LOOP_END
  }
  g_ary_store_ptr = 0;
}


template <class T_TYPE>
VariableArray_T<T_TYPE>::VariableArray_T(int size){
  g_ary            = new T_TYPE *[size];
  g_ary_size       = size;
  g_ary_store_ptr  = 0;
  g_ary_get_ptr    = 0;
  for(int i=0;i<g_ary_size;i++){
    *(g_ary+i) = NULL;
  }
  delFlag = true;
}
template <class T_TYPE>
VariableArray_T<T_TYPE>::~VariableArray_T(){
  clear();
  delete [] g_ary;
}

// # store #####
template <class T_TYPE>
void VariableArray_T<T_TYPE>::mem_alloc(T_TYPE *ptr){
  // re alloc
  if (g_ary_size <= g_ary_store_ptr){
    T_TYPE **sv_ary;
    sv_ary = new T_TYPE *[g_ary_size];
    for(int i=0;i<g_ary_size;i++){*(sv_ary+i) = *(g_ary+i);}
    delete [] g_ary;
    g_ary = new T_TYPE *[g_ary_size*2];
    for(int i=0;i<g_ary_size;i++){*(g_ary+i) = *(sv_ary+i);}
    for(int i=g_ary_size;i<g_ary_size*2;i++){*(g_ary+i) = NULL;}
    g_ary_size *= 2;
    delete [] sv_ary;
  }
  *(g_ary+g_ary_store_ptr) = ptr;
  g_ary_store_ptr++;
}

template <class T_TYPE>
void VariableArray_T<T_TYPE>::mem_alloc(int no,T_TYPE *ptr){
  // re alloc
  if (g_ary_size <= no){
    T_TYPE **sv_ary;
    sv_ary = new T_TYPE *[g_ary_size];
    for(int i=0;i<g_ary_size;i++){*(sv_ary+i) = *(g_ary+i);}
    delete [] g_ary;
    g_ary = new T_TYPE *[g_ary_size*2];
    for(int i=0;i<g_ary_size;i++){*(g_ary+i) = *(sv_ary+i);}
    for(int i=g_ary_size;i<g_ary_size*2;i++){*(g_ary+i) = NULL;}
    g_ary_size *= 2;
    delete [] sv_ary;
  }
  *(g_ary+no) = ptr;
  if (g_ary_store_ptr < no+1){g_ary_store_ptr = no+1;}
}



// # get #####
template <class T_TYPE>
T_TYPE *VariableArray_T<T_TYPE>::get(int no){
  if (g_ary_store_ptr <= no){return(NULL);}
  if (0 > no){return(NULL);}  // S.Kasuya 2014.4.19

  return(*(g_ary+no));
}

// # insert #####
template <class T_TYPE>
void VariableArray_T<T_TYPE>::insert(int no,int insert_num){
  if (no < g_ary_store_ptr){
    for(int i=g_ary_store_ptr-1;i>=no;i--){
      mem_alloc(i+insert_num,get(i));
      mem_alloc(i,NULL);
    }
  }
  else {
    for(int i=g_ary_store_ptr;i<insert_num+no;i++){
      mem_alloc(i,NULL);
    }
    g_ary_store_ptr = insert_num+no;
  }

}



//
// Variable Stack class  ----------------------------
//

// # push #####
template <class T_TYPE>
void VariableStack_T<T_TYPE>::push(T_TYPE *str){
  mem_alloc(str);
}

// # check #####
template <class T_TYPE>
T_TYPE *VariableStack_T<T_TYPE>::check(){
  int ptr = getStorePtr();
  if (ptr == 0){return(NULL);}
  return(get(ptr-1));
}

// # pop #####
template <class T_TYPE>
T_TYPE *VariableStack_T<T_TYPE>::pop(){
  int ptr = getStorePtr();
  if (ptr == 0){return(NULL);}
  T_TYPE *ret = get(ptr-1);
  setStorePtr(ptr-1);
  return(ret);
}


//
// Variable Fifo class  ----------------------------
//

// # push #####
template <class T_TYPE>
void VariableFifo_T<T_TYPE>::push(T_TYPE *str){
  mem_alloc(str);
}

// # check #####
template <class T_TYPE>
T_TYPE *VariableFifo_T<T_TYPE>::check(){
  if (getReadPtr() == getWritePtr()){return(NULL);}
  return(get(getReadPtr()));
}

// # pop #####
template <class T_TYPE>
T_TYPE *VariableFifo_T<T_TYPE>::pop(){
  if (getReadPtr() == getWritePtr()){return(NULL);}
  return(get());
}

// # defrag #####
template <class T_TYPE>
void VariableFifo_T<T_TYPE>::defrag(){
  int trans_size = getWritePtr() - getReadPtr();
  int start_ptr = getWritePtr();
  for(int ptr=0;ptr<trans_size;ptr++){
    T_TYPE *mem = get(ptr+start_ptr);
    mem_alloc(ptr,mem);
  }
  clear();
  get_init();
}


