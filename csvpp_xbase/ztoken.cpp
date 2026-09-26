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
//  String Tool
//
#include <stdio.h>
#include "ztoken.h"

tokenAnl_C::tokenAnl_C(){
  srch_ary = new VariableArray_C(10);
  srch_str = new VariableArray_C(10);
  str_su   = 0;
  ssc      = NULL;
  w_buff   = NULL;
  sj       = new sjisAnl_C;
  kanjiMode = false;
}

tokenAnl_C::~tokenAnl_C(){
  ARY_LOOP(w_buff,VariableArea_C,srch_str) delete w_buff; LOOP_END
  ARY_LOOP(ssc   ,tokenCell_C ,srch_ary)   delete ssc;    LOOP_END
  delete srch_str;
  delete srch_ary;
  delete sj;
}

// Result String Get
char *tokenAnl_C::get(int idx){
  if (idx >= str_su){return(NULL);}
  VariableArea_C *a = (VariableArea_C *)srch_str->get(idx);
  if (a == NULL){return(NULL);}
  return(a->cc_str());
}

// Result String number Get
int tokenAnl_C::getNum() {return(str_su);}

// Flag Set
void tokenAnl_C::setKeyFlag(){
  int num = srch_ary->getStorePtr();
  if (num == 0){return;}
  ssc = (tokenCell_C *)srch_ary->get(num-1);
  ssc->srch_flg = true;
}

// key set
void tokenAnl_C::setKey(char key){
  ssc = new tokenCell_C(key);
  srch_ary->mem_alloc((unsigned char *)ssc);
}

// store string
VariableArea_C *tokenAnl_C::storeString(){
  VariableArea_C  *w_buff = (VariableArea_C *)srch_str->get(str_su);
  if (w_buff != NULL){
    w_buff->init();
  }
  else {
    w_buff = new VariableArea_C(10);
    srch_str->mem_alloc(str_su,(unsigned char *)w_buff);
  }
  str_su++;
  return(w_buff);
  
}

// Main 
void tokenAnl_C::set(char *p_buff){
  str_su = 0;
  w_buff = storeString();

  sj->init();
  for(char *buff = p_buff;;buff++){
    if (*buff == 0x00){break;}
    if (kanjiMode){
      if (sj->check(buff) == true){w_buff->set(*buff); continue;}
    }
    tokenCell_C *h_ssc=NULL;
    ARY_LOOP(ssc,tokenCell_C,srch_ary)
      if (*buff == ssc->srch_ch){h_ssc=ssc; break;}
    LOOP_END
    // Miss Hit
    if (h_ssc == NULL){
      w_buff->set(*buff);
      continue;
    }
    ssc = h_ssc;
    // Hit String (repeat hit)
    if (w_buff->ptrGet() == 0){
      if (ssc->srch_flg){
        w_buff->set(*buff);
        w_buff->set(0x00);
        w_buff = storeString();
      }
      continue;
    }

    // Hit String
    w_buff->set(0x00);
    w_buff = storeString();
    if (ssc->srch_flg){
      w_buff->set(*buff);
      w_buff->set(0x00);
      w_buff = storeString();
    }
  }
  // length == 0
  if ((w_buff->ptrGet() == 0) && (str_su > 1)){
    str_su--;
  }
  else {
    w_buff->set(0x00);
  }
}



