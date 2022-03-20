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
#include "xexpression.h"

///// --------------------- enzanshi program --------------------------------
// # Keisan Routine -------------
T_TYPE CalcPlus  (T_TYPE p1,T_TYPE p2){return(p1 +  p2);}
T_TYPE CalcMinus (T_TYPE p1,T_TYPE p2){return(p1 -  p2);}
T_TYPE CalcMulti (T_TYPE p1,T_TYPE p2){return(p1 *  p2);}
T_TYPE CalcDivde (T_TYPE p1,T_TYPE p2){
  if (p2 == 0){return(0);}
  return(p1 /  p2);
}
#ifndef T_TYPE_FLOAT
T_TYPE CalcAnd   (T_TYPE p1,T_TYPE p2){return(p1 &  p2);}
T_TYPE CalcOr    (T_TYPE p1,T_TYPE p2){return(p1 |  p2);}
T_TYPE CalcAmari (T_TYPE p1,T_TYPE p2){return(p1 %  p2);}
T_TYPE CalcRShift(T_TYPE p1,T_TYPE p2){return(p1 >> p2);}
T_TYPE CalcLShift(T_TYPE p1,T_TYPE p2){return(p1 << p2);}
#endif
T_TYPE RonriOr   (T_TYPE p1,T_TYPE p2){
  if ((p1 == 1) || (p2 == 1)){return((T_TYPE)1);} 
  return((T_TYPE)0);
}
T_TYPE RonriAnd  (T_TYPE p1,T_TYPE p2){
  if ((p1 == 1) && (p2 == 1)){return((T_TYPE)1);}
  return((T_TYPE)0);
}
T_TYPE CompEq    (T_TYPE p1,T_TYPE p2){if (p1 == p2){return((T_TYPE)1);} return((T_TYPE)0);}
T_TYPE CompNe    (T_TYPE p1,T_TYPE p2){if (p1 != p2){return((T_TYPE)1);} return((T_TYPE)0);}
T_TYPE CompLt    (T_TYPE p1,T_TYPE p2){if (p1 >  p2){return((T_TYPE)1);} return((T_TYPE)0);}
T_TYPE CompRt    (T_TYPE p1,T_TYPE p2){if (p1 <  p2){return((T_TYPE)1);} return((T_TYPE)0);}
T_TYPE CompLe    (T_TYPE p1,T_TYPE p2){if (p1 >= p2){return((T_TYPE)1);} return((T_TYPE)0);}
T_TYPE CompRe    (T_TYPE p1,T_TYPE p2){if (p1 <= p2){return((T_TYPE)1);} return((T_TYPE)0);}

///// --------------------- code exec ---------------------------------------
void Expression_C::codeExec_Init(){
  // var_fp -> exec_var_fp
  varName_C *v;
  varExecCell_C *vec;
  MEM_LOOP(v,varName_C,var_fp)
    vec = new varExecCell_C(v->name->c_str(),v->num,v->init_val);
    exec_var_fp->mem_alloc(v->num,(unsigned char *)vec);
  LOOP_END
}

T_TYPE Expression_C::codeExec(expList_C *list,bool debugFlg){
  T_TYPE *stack = new T_TYPE[list->ary_fp->getStorePtr()];
  int ptr=0;
  expCell_C *exp;
  T_TYPE atai;

  ARY_LOOP(exp,expCell_C,list->ary_fp)
    if (exp->kind == K_CONST){
      *(stack+ptr)=exp->value;
      ptr++;
    }
    else if (exp->kind == K_VAR){
      varExecCell_C *vec = (varExecCell_C *)exec_var_fp->get((int)exp->value);
      *(stack+ptr)=vec->value;
      ptr++;
    }
    else if (exp->kind == K_ENZAN){
      if (ptr < 2){delete [] stack; return(0);}
      ptr--; T_TYPE a = *(stack+ptr);
      ptr--; T_TYPE b = *(stack+ptr);
      enzanExecCell_C *eec = (enzanExecCell_C *)exec_enzan_fp->get((int)exp->value);
      T_TYPE c = eec->rtn(a,b);

      if (debugFlg == true){
        printf("%d %s %d = %d\n",(int)a,eec->name->c_str(),(int)b,(int)c);
      }
      *(stack+ptr)=c;
      ptr++;
    }
    else {
      delete [] stack;
      return(0);
    }
  LOOP_END

  atai = *(stack);
  delete [] stack;
  return(atai);
}

void Expression_C::varListMon(){
  varExecCell_C *vec;

  printf("---- Variable define ------\n");
  ARY_LOOP(vec,varExecCell_C,exec_var_fp)
    printf("[%d]:%s\n",vec->num,vec->name->c_str());
  LOOP_END
}

void Expression_C::codeListMon(expList_C *list){
  expCell_C *exp;

  printf("---- Code list ------\n");
  int i=0;
  ARY_LOOP(exp,expCell_C,list->ary_fp) 
    if      (exp->kind == K_CONST){printf("[%03d]CONST:%d\n",i,(int)exp->value);}
    else if (exp->kind == K_VAR  ){
      varExecCell_C *vec = (varExecCell_C *)exec_var_fp->get((int)exp->value);
      if (vec == NULL){
        printf("[%03d]Error -> kind=%d  value=%d\n",i,exp->kind,(int)exp->value);
      }
      else {
        printf("[%03d]Variable:%s\n",i, vec->name->c_str());
      }
    }
    else if (exp->kind == K_ENZAN  ){
      enzanExecCell_C *eec = (enzanExecCell_C *)exec_enzan_fp->get((int)exp->value);
      printf("[%03d]Enzanshi:%s\n",i, eec->name->c_str());
    }
    else {
      printf("[%03d]Error -> kind=%d  value=%d\n",i,exp->kind,(int)exp->value);
    }
    i++;
  LOOP_END
}

void Expression_C::valueWrite(int var_no,T_TYPE value){
  varExecCell_C *vec = (varExecCell_C *)exec_var_fp->get(var_no);
  vec->value = value;
}

T_TYPE Expression_C::valueRead(int var_no){
  varExecCell_C *vec = (varExecCell_C *)exec_var_fp->get(var_no);
  return(vec->value);
}

///// --------------------- compile ---------------------------------------
// --------------------------
// expList_C  
// --------------------------
void expList_C::cat(expCell_C *e){
  ary_fp->mem_alloc((unsigned char *)e);
}

void expList_C::cat(VariableArray_C *list){
  expCell_C *e;
  ARY_LOOP(e,expCell_C,list)
    ary_fp->mem_alloc((unsigned char *)e);
  LOOP_END
  list->clear();
}


// --------------------------
// Code List Read & Store 
// --------------------------
expList_C *Expression_C::CodeListGet(int no){
  return((expList_C *)code_fp->get(no));
}

int Expression_C::CodeListStore(expList_C *list){
  code_fp->mem_alloc((unsigned char *)list);
  return(code_fp->getStorePtr()-1);
}

// --------------------------
// Variable Search & Store
// --------------------------
expCellKind Expression_C::VarSrch(char *key,T_TYPE *outVal){
  it->srch_key = key;
  if (var_fp->mem_srch() == 0){
    varName_C *v = (varName_C *)var_fp->mem_link_ptr_rd();
    *outVal = (T_TYPE)v->num;
    return(K_VAR);
  }
  *outVal = convStrNum<T_TYPE>(key);
  return(K_CONST);
}

int Expression_C::VarSrchStore(char *key){
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


bool Expression_C::VarStore(char *key,T_TYPE init_val){
  varName_C *v;
  it->srch_key = key;
  if (var_fp->mem_srch() == 0){
    return(false);
  }
  v = new varName_C(key,var_num,init_val);
  it->alloc_ptr = (MM_PTR_T *)v;
  var_fp->mem_srch_alloc();
  var_num++;
  return(true);
}

// --------------------------
// Enzanshi Search & Store
// --------------------------
enzanName_C *Expression_C::EnzanSrch(char *key){
  enzanName_C *ce;
  enzanName_C *sv_ce=NULL;
  unsigned int sv_len = 0;

  MEM_LOOP(ce,enzanName_C,enzan_fp)
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

bool Expression_C::EnzanStore(char *key,int power,T_TYPE  (* p_calc_rtn) (T_TYPE p1,T_TYPE p2)){
  enzanName_C *v;
  enzanExecCell_C *eec;
  it->srch_key = key;
  if (enzan_fp->mem_srch() == 0){
    return(false);
  }

  eec = new enzanExecCell_C(key,enzan_num,p_calc_rtn);
  exec_enzan_fp->mem_alloc((unsigned char *)eec);
  v = new enzanName_C(key,enzan_num,power);
  it->alloc_ptr = (MM_PTR_T *)v;
  enzan_fp->mem_srch_alloc();
  enzan_num++;
  return(true);
}

// ---------------
// Enzanshi Power
// ---------------
int Expression_C::CalcPower(enzanName_C *calc,enzanName_C *calc2)  {
  if (calc  == (enzanName_C *)MM_NULL) {return(1);}
  if (calc2 == (enzanName_C *)MM_NULL) {return(-1);}
  if (calc->power >  calc2->power) {return(-1);}
  if (calc->power == calc2->power) {return(0);}
  return(1);
}

// -------------
// Calc Check
// -------------
int Expression_C::enzanshiChk(char str){
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
void Expression_C::codeStore(expCell_C *e,expList_C *out){
  if (e->use == true){return;}
  e->use = true;
  if (e->kind == K_CODE){
    expList_C *code = CodeListGet((int)e->value);
    out->cat(code->ary_fp);
  }
  else {
    out->cat(e);
  }
}

void Expression_C::codeStore(enzanName_C *e,expList_C *out){
  expCell_C *exp = new expCell_C(K_ENZAN,e->num);
  dust_fp->mem_alloc((unsigned char *)exp);
  out->cat(exp);
}


// --------------
// Compile Main
// --------------
void Expression_C::compileMain(MEM_FP *a_fp,MEM_FP *e_fp,expList_C *out)  {
  enzanName_C *ce1;
  enzanName_C *ce2;
  McbArea_C *sv_ptr_a = a_fp->mem_mcb_top_set();
  McbArea_C *sv_ptr_e = e_fp->mem_mcb_top_set();
  expCell_C *atai1;
  expCell_C *atai2;

  if (sv_ptr_e == MM_NULL){atai1 = (expCell_C *)a_fp->mem_link_ptr_rd();  codeStore(atai1,out); return;}                                   
  else {                                                                                                               
    e_fp->mem_mcb_ptr_wr(sv_ptr_e); sv_ptr_e=e_fp->mem_mcb_next_rd(); ce1  =(enzanName_C *)e_fp->mem_link_ptr_rd(); 
    a_fp->mem_mcb_ptr_wr(sv_ptr_a); sv_ptr_a=a_fp->mem_mcb_next_rd(); atai1=(expCell_C   *)a_fp->mem_link_ptr_rd(); 
  }

  for(;;){
    if (sv_ptr_e == MM_NULL){
      a_fp->mem_mcb_end_set();
      atai2 = (expCell_C *)a_fp->mem_link_ptr_rd();
      codeStore(atai2,out);
      codeStore(atai1,out);
      codeStore(ce1  ,out);

      e_fp->mem_del();
      a_fp->mem_del();
      sv_ptr_e = e_fp->mem_mcb_top_set();       
      sv_ptr_a = a_fp->mem_mcb_top_set();       
      if (sv_ptr_e == MM_NULL){atai1 = (expCell_C *)a_fp->mem_link_ptr_rd(); codeStore(atai1,out); return;}                                   
      else {                                                                                                               
        e_fp->mem_mcb_ptr_wr(sv_ptr_e); sv_ptr_e=e_fp->mem_mcb_next_rd(); ce1  =(enzanName_C *)e_fp->mem_link_ptr_rd(); 
        a_fp->mem_mcb_ptr_wr(sv_ptr_a); sv_ptr_a=a_fp->mem_mcb_next_rd(); atai1=(expCell_C   *)a_fp->mem_link_ptr_rd(); 
      }
      continue;
    }

    e_fp->mem_mcb_ptr_wr(sv_ptr_e); sv_ptr_e=e_fp->mem_mcb_next_rd(); ce2  =(enzanName_C *)e_fp->mem_link_ptr_rd();
    a_fp->mem_mcb_ptr_wr(sv_ptr_a); sv_ptr_a=a_fp->mem_mcb_next_rd(); atai2=(expCell_C   *)a_fp->mem_link_ptr_rd();

    if (CalcPower(ce1,ce2)<=0){
      codeStore(atai2,out);
      codeStore(atai1,out);
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
expList_C *Expression_C::compile(char *str){
  sp_push3(str);
  err = false;
  expList_C *exp = new expList_C;
  CodeListStore(exp);

  compileLoop(str,exp);
  return(exp);
}

void Expression_C::compileLoop(char *str,expList_C *out){
  enzanName_C *ce;
  expCell_C   *exp_cell;
  int  ptr;
  int  rec;
  int  sv_ptr;
  long w_long;
  long result;
  long *w;
  
  expCellKind kind;
  T_TYPE       val;
  
  MEM_FP *a_fp;    // For CalcExecute
  MEM_FP *e_fp;    // For CalcExecute

  if (err == true){return;}


  VariableArea_C *key = new VariableArea_C(64);

  int  len = strlen(str);

  if (len == 0){err = true; return;}

  e_fp    = new MEM_FP(it);
  a_fp    = new MEM_FP(it);
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
        exp_cell = new expCell_C(kind,val);
        it->alloc_ptr = exp_cell;
        a_fp->mem_alloc();
        dust_fp->mem_alloc((unsigned char *)exp_cell);
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
        
        expList_C *exp = new expList_C;
        int exp_no = CodeListStore(exp);
        compileLoop(str+sv_ptr+1,exp);
        it->alloc_ptr = new expCell_C(K_CODE,exp_no);
        a_fp->mem_alloc();
        dust_fp->mem_alloc((unsigned char *)exp);
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
    exp_cell = new expCell_C(kind,val);
    it->alloc_ptr =  exp_cell;
    a_fp->mem_alloc();
    dust_fp->mem_alloc((unsigned char *)exp_cell);
  }
  if (rec != 0) {err = true;}
  compileMain(a_fp,e_fp,out);

  delete a_fp;
  delete e_fp;

  delete key;
  }

// - compile ok check -----
bool Expression_C::okCheck(){
  if (err){return(false);}
  return(true);
}


