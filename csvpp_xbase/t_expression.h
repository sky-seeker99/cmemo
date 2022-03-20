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

#ifndef __T_EXPRESSION_H_
#define __T_EXPRESSION_H_

#include "t_area_instance.h"
#include "t_tool_instance.h"
#include "ztool.h"

/// 識別
enum expCellKind {
    K_CONST,    /// 固定値
    K_VAR,      /// 変数
    K_ENZAN,    /// 演算子
    K_CODE      /// コンパイル結果列
};


// # Keisan Routine ---------------
template <class T_TYPE> T_TYPE CalcPlus  (T_TYPE p1,T_TYPE p2);
template <class T_TYPE> T_TYPE CalcMinus (T_TYPE p1,T_TYPE p2);
template <class T_TYPE> T_TYPE CalcMulti (T_TYPE p1,T_TYPE p2);
template <class T_TYPE> T_TYPE CalcDivde (T_TYPE p1,T_TYPE p2);
template <class T_TYPE> T_TYPE CalcAnd   (T_TYPE p1,T_TYPE p2);
template <class T_TYPE> T_TYPE CalcOr    (T_TYPE p1,T_TYPE p2);
template <class T_TYPE> T_TYPE CalcEor   (T_TYPE p1,T_TYPE p2); // 2015.8.8
template <class T_TYPE> T_TYPE CalcAmari (T_TYPE p1,T_TYPE p2);
template <class T_TYPE> T_TYPE CalcRShift(T_TYPE p1,T_TYPE p2);
template <class T_TYPE> T_TYPE CalcLShift(T_TYPE p1,T_TYPE p2);
template <class T_TYPE> T_TYPE RonriOr   (T_TYPE p1,T_TYPE p2);
template <class T_TYPE> T_TYPE RonriAnd  (T_TYPE p1,T_TYPE p2);
template <class T_TYPE> T_TYPE CompEq    (T_TYPE p1,T_TYPE p2);
template <class T_TYPE> T_TYPE CompNe    (T_TYPE p1,T_TYPE p2);
template <class T_TYPE> T_TYPE CompLt    (T_TYPE p1,T_TYPE p2);
template <class T_TYPE> T_TYPE CompRt    (T_TYPE p1,T_TYPE p2);
template <class T_TYPE> T_TYPE CompLe    (T_TYPE p1,T_TYPE p2);
template <class T_TYPE> T_TYPE CompRe    (T_TYPE p1,T_TYPE p2);


// # 演算子領域 cell --------------
template <class T_TYPE> 
class enzanExecCell_T{
  public:
    sChar *name;    // 演算子
    int    num;     // 演算子ナンバー
    T_TYPE  (* rtn) (T_TYPE p1,T_TYPE p2); // 計算プログラム

    enzanExecCell_T(char *p_name,int p_num,T_TYPE  (* p_calc_rtn) (T_TYPE p1,T_TYPE p2)){
      name = new sChar(p_name);
      num  = p_num;
      rtn  = p_calc_rtn;
    }
    ~enzanExecCell_T(){
      delete name;
    }
};

// # 変数領域 cell --------------
template <class T_TYPE> 
class varExecCell_T{
  public:
    sChar *name;   // 変数名
    int    num;    // 変数ナンバー
    T_TYPE value;  // 値

    varExecCell_T(char *p0,int p1,T_TYPE p2){
      name  = new sChar(p0);
      num   = p1;
      value = p2;
    }
    ~varExecCell_T(){
      delete name;
    }

};


// # 変数/固定値/演算子 cell --------------
template <class T_TYPE> 
class expCell_T{
  public:
    expCellKind kind;  // 識別
    T_TYPE value;      // 固定値の時は値、変数の時は変数配列のインデックス、演算子の時は演算子ナンバー、コンパイル結果列の時はコンパイル結果列ナンバー
    bool use;
    expCell_T(expCellKind p0,T_TYPE p1){
      kind  = p0;
      value = p1;
      use = false;
    }
};

// # expCell_C 列 (コンパイル結果を格納するexpCell_C列)  ---------------------------
template <class T_TYPE> 
class expList_T{
  public:
    VariableArray_T<expCell_T<T_TYPE> > *ary_fp;
    expList_T(){
      ary_fp = new VariableArray_T<expCell_T<T_TYPE> >(10);
      ary_fp->delFlag = false;
    }
    ~expList_T(){
      delete ary_fp;   ///  中身は削除しないので注意!!!
    }

    void cat(expCell_T<T_TYPE> *e);
    void cat(VariableArray_T<expCell_T<T_TYPE> > *list);
};

// # 変数名サーチ cell ----------------------
template <class T_TYPE> 
class varName_T{
  public:
    sChar *name;     /// 変数名
    int    num;      /// 変数配列のインデックス
    T_TYPE init_val; /// 初期値
    
    varName_T(char *p0,int p1,T_TYPE p2){
      name = new sChar(p0);
      num  = p1;
      init_val = p2;
    }
    ~varName_T(){
      delete name;
    }
};


// #  演算子サーチ cell --------------
class enzanName_C{
  public:
    sChar *name;    // 演算子
    int    num;     // 演算子ナンバー
    int    power;   // 優先順位

    enzanName_C(char *p0,int p1,int p2){
      name  = new sChar(p0);
      num   = p1;
      power = p2;
    }
    ~enzanName_C(){
      delete name;
    }
};

// #  演算実行 cell --------------
template <class T_TYPE>
class codeExecValue_T{
  public:
  T_TYPE value;
  codeExecValue_T(T_TYPE p){
    value = p;
  };
};


// # Expression Core ---------------
template <class T_TYPE>
class expCore_T{
  public:
    // ---- compile ------------
    VariableListHeader_C               *it;          // Mem Interface
    VariableList_T<varName_T<T_TYPE> > *var_fp;      // Variable List  
    int              var_num;                        // Variable Total Number
    VariableList_T<enzanName_C>        *enzan_fp;    // Enzanshi List  
    int              enzan_num;                      // Enzanshi Total Number
    VariableArray_T<expList_T<T_TYPE> > *code_fp;    // Compile Code Array
    int              code_num;                       // Compile Code Total Number
    VariableArray_T<expList_T<T_TYPE> > *dust_fp;    // collect Array cell
    VariableArray_T<expCell_T<T_TYPE> > *dust_e_fp;  // collect cell

    bool             err;                            // Error Flg

    // ---- exec ---------------
    VariableArray_T<varExecCell_T<T_TYPE> >   *exec_var_fp;   // variable array
    VariableArray_T<enzanExecCell_T<T_TYPE> > *exec_enzan_fp; // variable array

    varExecCell_T<T_TYPE>                     *exec_v;

    VariableStack_T<codeExecValue_T<T_TYPE> > *stack_fp;         // stack
    VariableStack_T<codeExecValue_T<T_TYPE> > *stack_dust_fp;    // stack collect Array cell

    // ---- debug message ---------------
    expCore_T(){
      it            = new VariableListHeader_C;
      var_fp        = new VariableList_T<varName_T<T_TYPE> >(it);
      enzan_fp      = new VariableList_T<enzanName_C>(it);
      dust_fp       = new VariableArray_T<expList_T<T_TYPE> >(10);
      dust_e_fp     = new VariableArray_T<expCell_T<T_TYPE> >(10);
      code_fp       = new VariableArray_T<expList_T<T_TYPE> >(10);
      exec_var_fp   = new VariableArray_T<varExecCell_T<T_TYPE> >(10);
      exec_enzan_fp = new VariableArray_T<enzanExecCell_T<T_TYPE> >(10);
      stack_fp      = new VariableStack_T<codeExecValue_T<T_TYPE> >(10);
      stack_dust_fp = new VariableStack_T<codeExecValue_T<T_TYPE> >(10);
      var_num   = 0;
      enzan_num = 0;
      code_num  = 0;
      err       = false;
      EnzanStore("||",401,RonriOr   );
      EnzanStore("&&",402,RonriAnd  );
      EnzanStore("==",301,CompEq    );
      EnzanStore("!=",302,CompNe    );
      EnzanStore(">" ,303,CompLt    );
      EnzanStore("<" ,304,CompRt    );
      EnzanStore(">=",305,CompLe    );
      EnzanStore("<=",306,CompRe    );
      EnzanStore("+" ,201,CalcPlus  );
      EnzanStore("-" ,202,CalcMinus );
      EnzanStore("*" ,203,CalcMulti );
      EnzanStore("/" ,204,CalcDivde );
    }
    ~expCore_T(){
      delete var_fp;
      delete enzan_fp;
      delete code_fp;
      delete dust_fp;
      delete dust_e_fp;
      delete exec_var_fp;
      delete exec_enzan_fp;
      delete stack_fp;
      delete stack_dust_fp;
      delete it;
    }

  // ---- compile -----
  public:
    expList_T<T_TYPE> *compile(char *str);
    bool okCheck();
    expCellKind VarSrch(char *key,T_TYPE *outVal);
    bool VarStore(char *key,T_TYPE init_val);
    int  VarSrchStore(char *key);

    bool EnzanStore(char *key,int power,T_TYPE  (* p_calc_rtn) (T_TYPE p1,T_TYPE p2));
  private:
    expList_T<T_TYPE> *CodeListGet(int no);
    int CodeListStore(expList_T<T_TYPE> *list);
    enzanName_C *EnzanSrch(char *key);
    int CalcPower(enzanName_C *calc,enzanName_C *calc2)  ;
    int enzanshiChk(char str);
    void codeStore(expCell_T<T_TYPE> *e,expList_T<T_TYPE> *out);
    void codeStore(enzanName_C *e,expList_T<T_TYPE> *out);
    void compileMain(VariableList_T<expCell_T<T_TYPE> > *a_fp,VariableList_T<enzanName_C> *e_fp,expList_T<T_TYPE> *out)  ;
    void compileLoop(char *str,expList_T<T_TYPE> *out);


  // ---- code exec before (option) ----
  public:
    char *codeExecBefore_begin(expList_T<T_TYPE> *list,bool *firstFlag);
    void  codeExecBefore_end(T_TYPE value); 

  // ---- code exec ----
  public:
    void   codeExec_Init();
    T_TYPE codeExec(expList_T<T_TYPE> *list,bool debugFlg);
    void   codeListMon(expList_T<T_TYPE> *list);
    void   varListMon();
    void   valueWrite(int var_no,T_TYPE value);
    T_TYPE valueRead(int var_no);
  private:
    T_TYPE pop();
    void   push(T_TYPE val);
};


// # Expression Interface ---------------
template <class T_TYPE>
class Expression_T : public expCore_T<T_TYPE>{
  public:
    Expression_T<T_TYPE>() : expCore_T<T_TYPE>(){
      EnzanStore("&" ,205,CalcAnd   );
      EnzanStore("|" ,206,CalcOr    );
      EnzanStore("%" ,207,CalcAmari );
      EnzanStore(">>",208,CalcRShift);
      EnzanStore("<<",209,CalcLShift);
      EnzanStore("^" ,210,CalcEor   );  // 2015.8.8
    }
};


template <class T_TYPE>
class ExpressionFloat_T : public expCore_T<T_TYPE>{
  public:
    ExpressionFloat_T<T_TYPE>() : expCore_T<T_TYPE>(){
    }
};


#endif // __XEXPRESSION_H_
