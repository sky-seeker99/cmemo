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
#ifndef _ZLIST_NEW_H_
#define _ZLIST_NEW_H_

#include "t_list_instance.h"

// # List Interface Class --------------------------------------
class VariableList_C : public VariableList_T<unsigned char>{
  public:  
    VariableList_C(VariableListHeader_C *p_it) : VariableList_T<unsigned char>(p_it){delFlag=false;}
    VariableList_C(VariableListHeader_C *p_it,int dmy) : VariableList_T<unsigned char>(p_it,dmy){delFlag=false;}
};

//- ŒÝŠ·« ------------
#define MM_PTR_T unsigned char 
#define MM_NULL NULL
#define MEM_FP VariableList_C
#define MemHeader_T VariableListHeader_C
#define MEM_LOOP_EX(DES,CAST,FP) McbArea_C *sv_mcb_ptr = FP->mem_mcb_top_rd(); \
                                 for(;;){ \
                                    if (sv_mcb_ptr == MM_NULL) {break;} \
                                    FP->mem_mcb_ptr_wr(sv_mcb_ptr); \
                                    sv_mcb_ptr = FP->mem_mcb_next_rd(); \
                                    DES = (CAST *)FP->mem_link_ptr_rd();
                                 
#define MEM_LOOP(DES,CAST,FP)  for(FP->mem_mcb_top_set();;FP->mem_mcb_next_set()){ \
                                 if (FP->mem_mcb_ptr_rd() == MM_NULL) {break;}  \
                                 DES = (CAST *)FP->mem_link_ptr_rd();


#endif // _ZLIST_H_
