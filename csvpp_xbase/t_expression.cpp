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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "t_expression.h"

///// --------------------- enzanshi program --------------------------------
// # Keisan Routine -------------
template <class T_TYPE> T_TYPE CalcPlus  (T_TYPE p1,T_TYPE p2){return(p1 +  p2);}
template <class T_TYPE> T_TYPE CalcMinus (T_TYPE p1,T_TYPE p2){return(p1 -  p2);}
template <class T_TYPE> T_TYPE CalcMulti (T_TYPE p1,T_TYPE p2){return(p1 *  p2);}
template <class T_TYPE> T_TYPE CalcDivde (T_TYPE p1,T_TYPE p2){
  if (p2 == 0){return(0);}
  return(p1 /  p2);
}

// #ifndef T_TYPE_FLOAT
template <class T_TYPE> T_TYPE CalcAnd   (T_TYPE p1,T_TYPE p2){return(p1 &  p2);}
template <class T_TYPE> T_TYPE CalcOr    (T_TYPE p1,T_TYPE p2){return(p1 |  p2);}
template <class T_TYPE> T_TYPE CalcAmari (T_TYPE p1,T_TYPE p2){return(p1 %  p2);}
template <class T_TYPE> T_TYPE CalcRShift(T_TYPE p1,T_TYPE p2){return(p1 >> p2);}
template <class T_TYPE> T_TYPE CalcLShift(T_TYPE p1,T_TYPE p2){return(p1 << p2);}
template <class T_TYPE> T_TYPE CalcEor   (T_TYPE p1,T_TYPE p2){return(p1 ^  p2);}  // 2015.8.8
// #endif
template <class T_TYPE> T_TYPE RonriOr   (T_TYPE p1,T_TYPE p2){
  if ((p1 == 1) || (p2 == 1)){return((T_TYPE)1);} 
  return((T_TYPE)0);
}
template <class T_TYPE> T_TYPE RonriAnd  (T_TYPE p1,T_TYPE p2){
  if ((p1 == 1) && (p2 == 1)){return((T_TYPE)1);}
  return((T_TYPE)0);
}
template <class T_TYPE> T_TYPE CompEq    (T_TYPE p1,T_TYPE p2){if (p1 == p2){return((T_TYPE)1);} return((T_TYPE)0);}
template <class T_TYPE> T_TYPE CompNe    (T_TYPE p1,T_TYPE p2){if (p1 != p2){return((T_TYPE)1);} return((T_TYPE)0);}
template <class T_TYPE> T_TYPE CompLt    (T_TYPE p1,T_TYPE p2){if (p1 >  p2){return((T_TYPE)1);} return((T_TYPE)0);}
template <class T_TYPE> T_TYPE CompRt    (T_TYPE p1,T_TYPE p2){if (p1 <  p2){return((T_TYPE)1);} return((T_TYPE)0);}
template <class T_TYPE> T_TYPE CompLe    (T_TYPE p1,T_TYPE p2){if (p1 >= p2){return((T_TYPE)1);} return((T_TYPE)0);}
template <class T_TYPE> T_TYPE CompRe    (T_TYPE p1,T_TYPE p2){if (p1 <= p2){return((T_TYPE)1);} return((T_TYPE)0);}

///// --------------------- code exec 前の変数への値設定 --------------------
///// --------------------- (他の構造の変数領域から式クラスへの変数領域へ値をコピーする。) --------------------------
template <class T_TYPE>
char *expCore_T<T_TYPE>::codeExecBefore_begin(expList_T<T_TYPE> *list,bool *firstFlag){  // firstFlagはtrueを代入してループさせる。
  exec_v=NULL;
  if (*firstFlag == true){
    list->ary_fp->get_init();
    *firstFlag = false;
  }

  for(;;){
    expCell_T<T_TYPE> *exp = list->ary_fp->get();
    if (exp == NULL){break;}
    if (exp->kind == K_VAR){
      exec_v = exec_var_fp->get((int)exp->value);
      break;
    }
  }
  if (exec_v == NULL){return(NULL);}
  return(exec_v->name->c_str());
}

template <class T_TYPE>
void expCore_T<T_TYPE>::codeExecBefore_end(T_TYPE value){ 
  if (exec_v == NULL){return;}
  exec_v->value = value;
}



///// --------------------- code exec ---------------------------------------
template <class T_TYPE>
void expCore_T<T_TYPE>::codeExec_Init(){
  // var_fp -> exec_var_fp
  varName_T<T_TYPE> *v;
  varExecCell_T<T_TYPE> *vec;

  exec_var_fp->clear();
  LIST_LOOP(v,varName_T<T_TYPE>,var_fp)
    vec = new varExecCell_T<T_TYPE>(v->name->c_str(),v->num,v->init_val);
    exec_var_fp->mem_alloc(v->num,vec);
  LOOP_END
}


template <class T_TYPE>
T_TYPE expCore_T<T_TYPE>::pop(){
  codeExecValue_T<T_TYPE> *w = stack_fp->pop();
  if (w == NULL) {
    return((T_TYPE)0);  
  }

  T_TYPE value = w->value;
  stack_dust_fp->push(w);
  return(value);
}

template <class T_TYPE>
void expCore_T<T_TYPE>::push(T_TYPE value){
  codeExecValue_T<T_TYPE> *w = stack_dust_fp->pop();
  if (w == NULL) {
    w = new codeExecValue_T<T_TYPE>(value);
  }
  else {
    w->value = value;
  }
  stack_fp->push(w);
}



template <class T_TYPE>
T_TYPE expCore_T<T_TYPE>::codeExec(expList_T<T_TYPE> *list,bool debugFlg){
  expCell_T<T_TYPE> *exp;
  T_TYPE value;

  ARY_LOOP(exp,expCell_T<T_TYPE>,list->ary_fp)
    if (exp->kind == K_CONST){
      push(exp->value);
    }
    else if (exp->kind == K_VAR){
      varExecCell_T<T_TYPE> *vec = exec_var_fp->get((int)exp->value);
      push(vec->value);
    }
    else if (exp->kind == K_ENZAN){
/*
      T_TYPE a = pop();
      T_TYPE b = pop();
*/
      T_TYPE b = pop();
      T_TYPE a = pop();


      enzanExecCell_T<T_TYPE> *eec = exec_enzan_fp->get((int)exp->value);
      T_TYPE c = eec->rtn(a,b);

      if (debugFlg == true){
        printf("%d %s %d = %d\n",(int)a,eec->name->c_str(),(int)b,(int)c);
      }
      push(c);
    }
    else {
      return(0);
    }
  LOOP_END

  value = pop();
  return(value);
}

template <class T_TYPE>
void expCore_T<T_TYPE>::varListMon(){
  varExecCell_T<T_TYPE> *vec;

  printf("---- Variable define ------\n");
  ARY_LOOP(vec,varExecCell_T<T_TYPE>,exec_var_fp)
    printf("[%d]:%s\n",vec->num,vec->name->c_str());
  LOOP_END
}

template <class T_TYPE>
void expCore_T<T_TYPE>::codeListMon(expList_T<T_TYPE> *list){
  expCell_T<T_TYPE> *exp;

  printf("---- Code list ------\n");
  int i=0;
  ARY_LOOP(exp,expCell_T<T_TYPE>,list->ary_fp)
    if      (exp->kind == K_CONST){printf("[%03d]CONST:%d\n",i,(int)exp->value);}
    else if (exp->kind == K_VAR  ){
      varExecCell_T<T_TYPE> *vec = exec_var_fp->get((int)exp->value);
      if (vec == NULL){
        printf("[%03d]Error -> kind=%d  value=%d\n",i,exp->kind,(int)exp->value);
      }
      else {
        printf("[%03d]Variable:%s\n",i, vec->name->c_str());
      }
    }
    else if (exp->kind == K_ENZAN  ){
      enzanExecCell_T<T_TYPE> *eec = exec_enzan_fp->get((int)exp->value);
      printf("[%03d]Enzanshi:%s\n",i, eec->name->c_str());
    }
    else {
      printf("[%03d]Error -> kind=%d  value=%d\n",i,exp->kind,(int)exp->value);
    }
    i++;
  LOOP_END
}

template <class T_TYPE>
void expCore_T<T_TYPE>::valueWrite(int var_no,T_TYPE value){
  varExecCell_T<T_TYPE> *vec = exec_var_fp->get(var_no);
  vec->value = value;
}

template <class T_TYPE>
T_TYPE expCore_T<T_TYPE>::valueRead(int var_no){
  varExecCell_T<T_TYPE> *vec = exec_var_fp->get(var_no);
  return(vec->value);
}

///// --------------------- compile ---------------------------------------
// --------------------------
// expList_C  
// --------------------------
template <class T_TYPE>
void expList_T<T_TYPE>::cat(expCell_T<T_TYPE> *e){
  ary_fp->mem_alloc(e);
}

template <class T_TYPE>
void expList_T<T_TYPE>::cat(VariableArray_T<expCell_T<T_TYPE> > *list){
  expCell_T<T_TYPE> *e;
  ARY_LOOP(e,expCell_T<T_TYPE>,list)
    ary_fp->mem_alloc(e);
  LOOP_END
  list->clear();
}


// --------------------------
// Code List Read & Store 
// --------------------------
template <class T_TYPE>
expList_T<T_TYPE> *expCore_T<T_TYPE>::CodeListGet(int no){
  return(code_fp->get(no));
}

template <class T_TYPE>
int expCore_T<T_TYPE>::CodeListStore(expList_T<T_TYPE> *list){
  code_fp->mem_alloc(list);
  return(code_fp->getStorePtr()-1);
}

// --------------------------
// Variable Search & Store
// --------------------------
template <class T_TYPE>
expCellKind expCore_T<T_TYPE>::VarSrch(char *key,T_TYPE *outVal){
  it->srch_key = key;
  if (var_fp->mem_srch() == 0){
    varName_T<T_TYPE> *v = var_fp->mem_link_ptr_rd();
    *outVal = (T_TYPE)v->num;
    return(K_VAR);
  }

  if (convStrNumCheck<T_TYPE>(key) == true){
    *outVal = convStrNum<T_TYPE>(key);
    return(K_CONST);
  }
  else {
    VarStore(key,(T_TYPE)0);
    *outVal = (T_TYPE)(var_num-1);
    return(K_VAR);
  }
}

template <class T_TYPE>
int expCore_T<T_TYPE>::VarSrchStore(char *key){
  varName_C *v;
  it->srch_key = key;
  if (var_fp->mem_srch() == 0){
    v = (varName_C *)var_fp->mem_link_ptr_rd();
  }
  else {
    v = new varName_C(key,var_num,(T_TYPE)0);
    it->alloc_ptr = (MM_PTR_T *)v;
    var_fp->mem_srch_alloc();
    var_num++;
  }
  return(v->num);

}


template <class T_TYPE>
bool expCore_T<T_TYPE>::VarStore(char *key,T_TYPE init_val){
  varName_T<T_TYPE> *v;
  it->srch_key = key;
  if (var_fp->mem_srch() == 0){
    return(false);
  }
  v = new varName_T<T_TYPE>(key,var_num,init_val);
  it->alloc_ptr = (MM_PTR_T *)v;
  var_fp->mem_srch_alloc();
  var_num++;
  return(true);
}

// --------------------------
// Enzanshi Search & Store
// --------------------------
template <class T_TYPE>
enzanName_C *expCore_T<T_TYPE>::EnzanSrch(char *key){
  enzanName_C *ce;
  enzanName_C *sv_ce=NULL;
  unsigned int sv_len = 0;

  LIST_LOOP(ce,enzanName_C,enzan_fp)
    int len = strlen(ce->name->c_str());
    if (mem_cmp(ce->name->c_str(),key,len) == 0){
      if (sv_len < (unsigned int)len){
        sv_ce  = ce;
        sv_len = len;
      }
    }
  LOOP_END
  return(sv_ce);
}

template <class T_TYPE>
bool expCore_T<T_TYPE>::EnzanStore(char *key,int power,T_TYPE  (* p_calc_rtn) (T_TYPE p1,T_TYPE p2)){
  enzanName_C *v;
  enzanExecCell_T<T_TYPE> *eec;
  it->srch_key = key;
  if (enzan_fp->mem_srch() == 0){
    return(false);
  }

  eec = new enzanExecCell_T<T_TYPE>(key,enzan_num,p_calc_rtn);
  exec_enzan_fp->mem_alloc(eec);
  v = new enzanName_C(key,enzan_num,power);
  it->alloc_ptr = (MM_PTR_T *)v;
  enzan_fp->mem_srch_alloc();
  enzan_num++;
  return(true);
}

// ---------------
// Enzanshi Power
// ---------------
template <class T_TYPE>
int expCore_T<T_TYPE>::CalcPower(enzanName_C *calc,enzanName_C *calc2)  {
  if (calc  == (enzanName_C *)MM_NULL) {return(1);}
  if (calc2 == (enzanName_C *)MM_NULL) {return(-1);}
  if (calc->power >  calc2->power) {return(-1);}
  if (calc->power == calc2->power) {return(0);}
  return(1);
}

// -------------
// Calc Check
// -------------
template <class T_TYPE>
int expCore_T<T_TYPE>::enzanshiChk(char str){
  if ((str >= 'a') && (str <= 'z')) {return(0);}
  if ((str >= 'A') && (str <= 'Z')) {return(0);}
  if ((str >= '0') && (str <= '9')) {return(0);}
  if (str == '_') {return(0);}
  if (str == '\'') {return(0);}
  return(-1);
}



// --------------
// Code Store
// --------------
template <class T_TYPE>
void expCore_T<T_TYPE>::codeStore(expCell_T<T_TYPE> *e,expList_T<T_TYPE> *out){
  if (e->use == true){return;}
  e->use = true;
  if (e->kind == K_CODE){
    expList_T<T_TYPE> *code = CodeListGet((int)e->value);
    out->cat(code->ary_fp);
  }
  else {
    out->cat(e);
  }
}

template <class T_TYPE>
void expCore_T<T_TYPE>::codeStore(enzanName_C *e,expList_T<T_TYPE> *out){
  expCell_T<T_TYPE> *exp = new expCell_T<T_TYPE>(K_ENZAN,e->num);
  dust_e_fp->mem_alloc(exp);
  out->cat(exp);
}


// --------------
// Compile Main
// --------------
template <class T_TYPE>
void expCore_T<T_TYPE>::compileMain(VariableList_T<expCell_T<T_TYPE> > *a_fp,VariableList_T<enzanName_C> *e_fp,expList_T<T_TYPE> *out)  {
  enzanName_C *ce1;
  enzanName_C *ce2;
  McbArea_C *sv_ptr_a = a_fp->mem_mcb_top_set();
  McbArea_C *sv_ptr_e = e_fp->mem_mcb_top_set();
  expCell_T<T_TYPE> *atai1;
  expCell_T<T_TYPE> *atai2;

  if (sv_ptr_e == NULL){atai1 = (expCell_T<T_TYPE> *)a_fp->mem_link_ptr_rd();  codeStore(atai1,out); return;}                                   
  else {                                                                                                               
    e_fp->mem_mcb_ptr_wr(sv_ptr_e); sv_ptr_e=e_fp->mem_mcb_next_rd(); ce1  =e_fp->mem_link_ptr_rd(); 
    a_fp->mem_mcb_ptr_wr(sv_ptr_a); sv_ptr_a=a_fp->mem_mcb_next_rd(); atai1=a_fp->mem_link_ptr_rd();
  }

  for(;;){
    if (sv_ptr_e == NULL){
      a_fp->mem_mcb_end_set();
      atai2 = a_fp->mem_link_ptr_rd();
/*    codeStore(atai2,out);
      codeStore(atai1,out);
      codeStore(ce1  ,out);
*/
      codeStore(atai1,out);
      codeStore(atai2,out);
      codeStore(ce1  ,out);




      e_fp->mem_del();
      a_fp->mem_del();
      sv_ptr_e = e_fp->mem_mcb_top_set();       
      sv_ptr_a = a_fp->mem_mcb_top_set();       
      if (sv_ptr_e == NULL){atai1 = a_fp->mem_link_ptr_rd(); codeStore(atai1,out); return;}
      else {                                                                                                               
        e_fp->mem_mcb_ptr_wr(sv_ptr_e); sv_ptr_e=e_fp->mem_mcb_next_rd(); ce1  =e_fp->mem_link_ptr_rd();
        a_fp->mem_mcb_ptr_wr(sv_ptr_a); sv_ptr_a=a_fp->mem_mcb_next_rd(); atai1=a_fp->mem_link_ptr_rd(); 
      }
      continue;
    }

    e_fp->mem_mcb_ptr_wr(sv_ptr_e); sv_ptr_e=e_fp->mem_mcb_next_rd(); ce2  =e_fp->mem_link_ptr_rd();
    a_fp->mem_mcb_ptr_wr(sv_ptr_a); sv_ptr_a=a_fp->mem_mcb_next_rd(); atai2=a_fp->mem_link_ptr_rd();

    if (CalcPower(ce1,ce2)<=0){
/*
      codeStore(atai2,out);
      codeStore(atai1,out);
      codeStore(ce1  ,out);
*/
      codeStore(atai1,out);
      codeStore(atai2,out);
      codeStore(ce1  ,out);

      a_fp->mem_mcb_before_set();
      e_fp->mem_mcb_before_set();

      a_fp->mem_del();
      e_fp->mem_del();
    }
    atai1 = atai2;
    ce1   = ce2;
  }
}

// --------------
// Compile
// --------------
template <class T_TYPE>
expList_T<T_TYPE> *expCore_T<T_TYPE>::compile(char *str){
  sp_push3(str);
  err = false;
  expList_T<T_TYPE> *exp = new expList_T<T_TYPE>;
  CodeListStore(exp);

  compileLoop(str,exp);
  return(exp);
}

template <class T_TYPE>
void expCore_T<T_TYPE>::compileLoop(char *str,expList_T<T_TYPE> *out){
  enzanName_C *ce;
  expCell_T<T_TYPE>   *exp_cell;
  int  ptr;
  int  rec;
  int  sv_ptr;
  long w_long;
  long result;
  long *w;
  
  expCellKind kind;
  T_TYPE       val;
  
  VariableList_T<expCell_T<T_TYPE> > *a_fp;    // For CalcExecute
  VariableList_T<enzanName_C>        *e_fp;    // For CalcExecute

  if (err == true){return;}


  VariableArea_C *key = new VariableArea_C(64);

  int  len = strlen(str);

  if (len == 0){err = true; return;}

  e_fp    = new VariableList_T<enzanName_C>(it);
  a_fp    = new VariableList_T<expCell_T<T_TYPE> >(it);
  e_fp->delFlag = false;
  a_fp->delFlag = false;

  rec     = 0;

  for(ptr=0;ptr<len;++ptr){
    if (*(str + ptr) == ' '){continue;}

    if (rec == 0){
      if (*(str+ptr) == ')'){err = true; break;}
      if (*(str+ptr) == '('){
        if (key->ptrGet() != 0) {err = true; key->init();}
        ++rec;
        sv_ptr = ptr;
        continue;
      }

      if (enzanshiChk(*(str+ptr)) != 0){
        key->set(0x00);
        kind = VarSrch(key->cc_str(),&val);
        exp_cell = new expCell_T<T_TYPE>(kind,val);
        it->alloc_ptr = exp_cell;
        a_fp->mem_alloc();
        dust_e_fp->mem_alloc(exp_cell);
        if ((kind == K_CONST) && (val == (T_TYPE)-1)){err = true;}
        
        ce = EnzanSrch(str+ptr);
        if (ce == NULL){err = true;} 
        else{
          it->alloc_ptr = ce;                                                
          e_fp->mem_alloc();                                                 
          ptr = ptr + strlen(ce->name->c_str()) - 1;                                  
        }
        key->init();
      }
      else{
        key->set(*(str+ptr));
      }
    }
    if (rec == 1){
      if (*(str+ptr) == ')'){
        --rec;
        *(str+ptr) = 0x00;
        
        expList_T<T_TYPE> *exp = new expList_T<T_TYPE>;
        int exp_no = CodeListStore(exp);
        compileLoop(str+sv_ptr+1,exp);
        expCell_T<T_TYPE> *ee = new expCell_T<T_TYPE>(K_CODE,exp_no);
        it->alloc_ptr = ee;
        a_fp->mem_alloc();
        dust_e_fp->mem_alloc(ee);
        ++ptr;
        if (*(str+ptr) != 0x00){
          ce = EnzanSrch(str+ptr);
          if (ce == NULL){err = true;} 
          else{
            it->alloc_ptr = ce;                                                
            e_fp->mem_alloc();                                                 
            ptr = ptr + strlen(ce->name->c_str()) - 1;                                  
          }
          key->init();
          continue;
        }
        else{
          break;
        }
      }
      if (*(str+ptr) == '('){++rec; continue;}
    }
    if (rec > 1){
      if (*(str+ptr) == ')'){--rec; continue;}
      if (*(str+ptr) == '('){++rec; continue;}
    }
  }
  if (key->ptrGet() != 0){
    key->set(0x00);
    kind = VarSrch(key->cc_str(),&val);
    exp_cell = new expCell_T<T_TYPE>(kind,val);
    it->alloc_ptr =  exp_cell;
    a_fp->mem_alloc();
    dust_e_fp->mem_alloc(exp_cell);
  }
  if (rec != 0) {err = true;}
  compileMain(a_fp,e_fp,out);

  delete a_fp;
  delete e_fp;

  delete key;
  }

// - compile ok check -----
template <class T_TYPE>
bool expCore_T<T_TYPE>::okCheck(){
  if (err){return(false);}
  return(true);
}


