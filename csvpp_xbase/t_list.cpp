/*

 * Copyright (c) 2010 Shigeru Kasuya (sky_seeker99@users.sourceforge.jp)
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
//  LIST LIBRARY
//

#include <string.h>
#include "t_list.h"

// ---------------- UnderLayer Program Sample ------------------
template <class T_TYPE>
int xlist_srch0(T_TYPE *key,T_TYPE *link){
  xlist_type0 *mcb = (xlist_type0 *)link;
  int kbn = strcmp(mcb->key->c_str(),(char *)key);
  if (kbn == 0) {return(0);}
  if (kbn > 0) {return(-1);}
  return(1);
}

template <class T_TYPE>
int xlist_srch1(T_TYPE *key,T_TYPE *link){
  xlist_type1 *mcb = (xlist_type1 *)link;
  int *srch_cnt = (int *)key;
  if (mcb->cnt == *srch_cnt) {return(0);}
  if (mcb->cnt  > *srch_cnt) {return(-1);}
  return(1);
  }

// ---------------- Constructor/Destructor Program ------------------------
template <class T_TYPE>
void VariableList_T<T_TYPE>::initialize(VariableListHeader_C *p_it,int mode){
  top_ptr   = NULL;
  end_ptr   = NULL;
  cur_ptr   = NULL;
  it        = p_it;
  srch_mode = mode;
  if (mode == 0){
    mm_srch  = xlist_srch0;
  }
  else {
    mm_srch  = xlist_srch1;
  }
  delFlag = true;
}

template <class T_TYPE>
VariableList_T<T_TYPE>::VariableList_T(VariableListHeader_C *p_it){
  initialize(p_it,0);
}
template <class T_TYPE>
VariableList_T<T_TYPE>::VariableList_T(VariableListHeader_C *p_it,int dmy){
  initialize(p_it,1);
}


template <class T_TYPE>
VariableList_T<T_TYPE>::~VariableList_T(){
  mem_del_all();
}


// # Memory Control Block Program ------------------------
template <class T_TYPE>
McbArea_C* VariableList_T<T_TYPE>::mcb_alloc(void){
  McbArea_C *mm_area = (McbArea_C *)it->dust_ary->pop();
  if (mm_area == NULL){mm_area = new McbArea_C;}
  return(mm_area);
}

template <class T_TYPE>
void VariableList_T<T_TYPE>::mcb_link_before(McbArea_C *c_ptr){
  McbArea_C *mcb_src;
  McbArea_C *mcb_cpy;
  McbArea_C *sv_before;

  mcb_cpy = c_ptr;

  if (cur_ptr == NULL){
    // IT'S EMPTY
    top_ptr = c_ptr;
    end_ptr = c_ptr;
  }
  else{
    // SOURCE LINK
    mcb_src         = cur_ptr;
    sv_before       = (McbArea_C *)mcb_src->before;
    mcb_src->before = (T_TYPE *)c_ptr;

    // COPY LINK
    mcb_cpy->before = (T_TYPE *)sv_before;
    mcb_cpy->after  = (T_TYPE *)cur_ptr;

    // BEFORE LINK
    if (sv_before == NULL) {top_ptr = c_ptr;}
    else{
      mcb_src        = sv_before;
      mcb_src->after = (T_TYPE *)c_ptr;
    }
  }

  // MM_FP SET
  cur_ptr = c_ptr;
}

template <class T_TYPE>
void VariableList_T<T_TYPE>::mcb_link_after(McbArea_C *c_ptr){
  McbArea_C *mcb_src;
  McbArea_C *mcb_cpy;
  McbArea_C *sv_after;

  mcb_cpy = c_ptr;

  if (cur_ptr == NULL){
    // IT'S EMPTY
    top_ptr = c_ptr;
    end_ptr = c_ptr;
  }
  else {
    // SOURCE LINK
    mcb_src        = cur_ptr;
    sv_after       = (McbArea_C *)mcb_src->after;
    mcb_src->after = (T_TYPE *)c_ptr;

    // COPY LINK
    mcb_cpy->before = (T_TYPE *)cur_ptr;
    mcb_cpy->after  = (T_TYPE *)sv_after;

    // AFTER LINK
    if (sv_after == NULL) {end_ptr = c_ptr;}
    else {
      mcb_src         = sv_after;
      mcb_src->before = (T_TYPE *)c_ptr;
    }
  }

  // MM_FP SET
  cur_ptr = c_ptr;
}

template <class T_TYPE>
void VariableList_T<T_TYPE>::mcb_del(void){
  McbArea_C *mm_area;
  McbArea_C *mm_before;
  McbArea_C *mm_after;
  McbArea_C *sv_after;
  McbArea_C *sv_before;

  if (cur_ptr == NULL) {return;}

  mm_area   = cur_ptr;
  sv_after  = (McbArea_C *)mm_area->after;
  sv_before = (McbArea_C *)mm_area->before;

  // before link
  if (mm_area->before == NULL) {top_ptr = (McbArea_C *)mm_area->after;}
  else{
    mm_before        = (McbArea_C *)mm_area->before;
    mm_before->after = mm_area->after;
  }
  // after link
  if (mm_area->after  == NULL) {end_ptr = (McbArea_C *)mm_area->before;}
  else{
    mm_after           = (McbArea_C *)mm_area->after;
    mm_after->before   = mm_area->before;
  }

  mm_area->before = NULL;
  mm_area->after  = NULL;
  mm_area->link   = NULL;
  it->dust_ary->push((unsigned char *)mm_area);

  // Current pointer set
  cur_ptr = NULL;
  if (sv_before != NULL) {cur_ptr=sv_before;}
  if (sv_after  != NULL) {cur_ptr=sv_after;}
}


// ---------------- Main Program ------------------------
// -----------
//  MEM Search
// -----------------------------
// =0:MUCH
// =1:KEY IS CURRENT TO AFTER
// =-1:KEY IS CURRENT TO BEFORE
// -----------------------------
template <class T_TYPE>
int VariableList_T<T_TYPE>::mem_srch(T_TYPE *key){
  int srch_kbn,rtn_cd;
  McbArea_C *mm_area;
  McbArea_C *sv_ptr;

  srch_kbn = 2;

  if (cur_ptr == NULL) {cur_ptr = top_ptr;}
  if (cur_ptr == NULL) {return(-1);}

  sv_ptr = NULL;
  for(;;){
    if (cur_ptr == NULL){cur_ptr=sv_ptr; break;}
    mm_area = cur_ptr;
    rtn_cd = mm_srch(key,(T_TYPE *)mm_area->link);
    if (rtn_cd   == 0)        {break;}
    if (srch_kbn == 2)        {srch_kbn = rtn_cd;}
    if (srch_kbn != rtn_cd)   {break;}
    sv_ptr = cur_ptr;
    if (rtn_cd ==  1)         {cur_ptr = (McbArea_C *)mm_area->after;}
    if (rtn_cd == -1)         {cur_ptr = (McbArea_C *)mm_area->before;}
  }
  return(rtn_cd);
}

template <class T_TYPE>
int VariableList_T<T_TYPE>::mem_srch(){
  int rtn_cd;
  if (srch_mode == 0){
	rtn_cd = mem_srch((T_TYPE *)it->srch_key);
  }
  else {
	rtn_cd = mem_srch((T_TYPE *)&it->srch_cnt);
  }
  return(rtn_cd);
}


template <class T_TYPE>
T_TYPE *VariableList_T<T_TYPE>::mem_link_ptr_rd(void){
  if (cur_ptr == NULL){return(NULL);}
  return((T_TYPE *)cur_ptr->link);
}

template <class T_TYPE>
McbArea_C *VariableList_T<T_TYPE>::mem_mcb_ptr_rd(void){
  return(cur_ptr);
}

template <class T_TYPE>
McbArea_C *VariableList_T<T_TYPE>::mem_mcb_next_rd(void){
  if (cur_ptr == NULL){return(NULL);}
  return((McbArea_C *)cur_ptr->after);
}

template <class T_TYPE>
McbArea_C *VariableList_T<T_TYPE>::mem_mcb_before_rd(void){
  if (cur_ptr == NULL){return(NULL);}
  return((McbArea_C *)cur_ptr->before);
}

template <class T_TYPE>
McbArea_C *VariableList_T<T_TYPE>::mem_mcb_top_rd(void){
  return(top_ptr);
}

template <class T_TYPE>
McbArea_C *VariableList_T<T_TYPE>::mem_mcb_end_rd(void){
  return(end_ptr);
}

template <class T_TYPE>
void VariableList_T<T_TYPE>::mem_link_ptr_wr(T_TYPE *link){
  cur_ptr->link = link;
}

template <class T_TYPE>
void VariableList_T<T_TYPE>::mem_mcb_ptr_wr(McbArea_C *p_ptr){
  cur_ptr = p_ptr;
}

template <class T_TYPE>
McbArea_C *VariableList_T<T_TYPE>::mem_mcb_top_set(void){
  cur_ptr = top_ptr;
  return(cur_ptr);
}

template <class T_TYPE>
McbArea_C *VariableList_T<T_TYPE>::mem_mcb_end_set(void){
  cur_ptr = end_ptr;
  return(cur_ptr);
}

template <class T_TYPE>
McbArea_C *VariableList_T<T_TYPE>::mem_mcb_next_set(void){
  if (cur_ptr == NULL) {return(NULL);}
  cur_ptr = (McbArea_C *)cur_ptr->after;
  return(cur_ptr);
  }

template <class T_TYPE>
McbArea_C *VariableList_T<T_TYPE>::mem_mcb_before_set(void){
  if (cur_ptr == NULL) {return(NULL);}
  cur_ptr = (McbArea_C *)cur_ptr->before;
  return(cur_ptr);
}

template <class T_TYPE>
void VariableList_T<T_TYPE>::mem_alloc(T_TYPE *link){
  McbArea_C *mcb  = mcb_alloc();
  mcb->link       = link;
  mem_mcb_ptr_wr(end_ptr);
  mcb_link_after(mcb);
}

template <class T_TYPE>
void VariableList_T<T_TYPE>::mem_alloc(){
  mem_alloc((T_TYPE *)it->alloc_ptr);
}


template <class T_TYPE>
bool VariableList_T<T_TYPE>::mem_srch_alloc(T_TYPE *key,T_TYPE *link,bool delFlag){
  int rtn_cd = 0;

  for(;;){
    rtn_cd = mem_srch(key);
    if (rtn_cd != 0){break;}
    if (delFlag == false){return(false);}
    mem_del();
  }

  McbArea_C *mcb = mcb_alloc();
  mcb->link  = link;

  if (rtn_cd ==  1) {mcb_link_after (mcb);}
  if (rtn_cd == -1) {mcb_link_before(mcb);}
  mem_mcb_ptr_wr(mcb);
  return(true);
}

template <class T_TYPE>
void VariableList_T<T_TYPE>::mem_srch_alloc(){
  if (srch_mode == 0){
    mem_srch_alloc((T_TYPE *)it->srch_key,(T_TYPE *)it->alloc_ptr,false);
  }
  else {
    mem_srch_alloc((T_TYPE *)&it->srch_cnt,(T_TYPE *)it->alloc_ptr,false);
  }
}



template <class T_TYPE>
void VariableList_T<T_TYPE>::mem_del(){
  if (delFlag){
    T_TYPE *w = (T_TYPE *)cur_ptr->link;
    delete w;
  }
  mcb_del();
}

template <class T_TYPE>
void VariableList_T<T_TYPE>::mem_del_all(){
  McbArea_C *cur;

  cur = top_ptr;
  for(;;){
    if (cur == NULL) {break;}
    cur_ptr = cur;
    cur = mem_mcb_next_rd();
    mem_del();
  }
}

template <class T_TYPE>
void VariableList_T<T_TYPE>::list_ary(VariableArray_C *ary){
  for(mem_mcb_top_set();;mem_mcb_next_set()){
    if (mem_mcb_ptr_rd() == NULL) {break;}
    ary->mem_alloc(mem_link_ptr_rd());
  }
}


