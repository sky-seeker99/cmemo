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
#ifndef _T_LIST_H_
#define _T_LIST_H_

#include <stdio.h>                                 
#include "xarea.h"
#include "xstack_fifo_.h"
#include "zschar.h"

// # DEFINE ----------------------------------------------------
// #define MCB_DEBUG 1   EXIST:DEBUG MODE
#define LIST_LOOP_EX(DES,CAST,FP) McbArea_C *sv_mcb_ptr = FP->mem_mcb_top_rd(); \
                                 for(;;){ \
									if (sv_mcb_ptr == NULL) {break;} \
                                    FP->mem_mcb_ptr_wr(sv_mcb_ptr); \
                                    sv_mcb_ptr = FP->mem_mcb_next_rd(); \
                                    DES = (CAST *)FP->mem_link_ptr_rd();
                                 
#define LIST_LOOP(DES,CAST,FP)  for(FP->mem_mcb_top_set();;FP->mem_mcb_next_set()){ \
                                 if (FP->mem_mcb_ptr_rd() == NULL) {break;}  \
                                 DES = (CAST *)FP->mem_link_ptr_rd();
#ifndef LOOP_END
#define LOOP_END }
#endif

// # Prototype -------------------------------------------------
template <class T_TYPE> int xlist_srch0(T_TYPE *key,T_TYPE *link);
template <class T_TYPE> int xlist_srch1(T_TYPE *key,T_TYPE *link);

// # Control Header ---------------------
typedef struct {
  sChar *key;
  } xlist_type0;
typedef struct {
  int   cnt;
} xlist_type1;

// # MCB(Memory Control Block) Structure -----------------
class McbArea_C {
  public:
    void *before;  // Current Before pointer
    void *after;   // Current After pointer
    void *link;    // link pointer
  McbArea_C(){
    before = NULL;
    after  = NULL;
    link   = NULL;
  }
};

// # Controller Interface --------------------------------------
class VariableListHeader_C {
  public:
    VariableStack_C *dust_ary;
    char *srch_key;
    int   srch_cnt;
    void *alloc_ptr;

    VariableListHeader_C(){
      dust_ary = new VariableStack_C(10);
      srch_key = NULL;
      srch_cnt = 0;
      alloc_ptr = NULL;
    }
    ~VariableListHeader_C(){
      McbArea_C *m;
      ARY_LOOP(m,McbArea_C,dust_ary) delete m; LOOP_END
      delete dust_ary;
    }
};

// # List Interface Class --------------------------------------
template <class T_TYPE>
class VariableList_T {
  public:
    bool delFlag;
    VariableList_T(VariableListHeader_C *p_it);
    VariableList_T(VariableListHeader_C *p_it,int dmy);
    ~VariableList_T();
    void           initialize(VariableListHeader_C *p_it,int mode);
    int            mem_srch(T_TYPE *key);
    int            mem_srch();
    T_TYPE        *mem_link_ptr_rd(void);
    McbArea_C     *mem_mcb_ptr_rd(void);
    McbArea_C     *mem_mcb_next_rd(void);
    McbArea_C     *mem_mcb_before_rd(void);
    McbArea_C     *mem_mcb_top_rd(void);
    McbArea_C     *mem_mcb_end_rd(void);
	void           mem_link_ptr_wr(T_TYPE *link);
	void           mem_mcb_ptr_wr(McbArea_C *p_ptr);
	McbArea_C     *mem_mcb_top_set(void);
    McbArea_C     *mem_mcb_end_set(void);
    McbArea_C     *mem_mcb_next_set(void);
    McbArea_C     *mem_mcb_before_set(void);
    void           mem_alloc(T_TYPE *link);
    void           mem_alloc();
    bool           mem_srch_alloc(T_TYPE *key,T_TYPE *link,bool delFlag);
    void           mem_srch_alloc();
    void           mem_del();
    void           mem_del_all();
    void           list_ary(VariableArray_C *ary);    

  private:
    McbArea_C *top_ptr;
    McbArea_C *end_ptr;
    McbArea_C *cur_ptr;
    int        srch_mode;
    
    VariableListHeader_C *it;
    int  (* mm_srch )(T_TYPE *key,T_TYPE *link);

    McbArea_C* mcb_alloc(void);
    void       mcb_link_before(McbArea_C *c_ptr);
    void       mcb_link_after(McbArea_C *c_ptr);
    void       mcb_del(void);
    
};


#endif // _T_LIST_H_
