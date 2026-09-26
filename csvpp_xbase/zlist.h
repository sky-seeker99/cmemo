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
