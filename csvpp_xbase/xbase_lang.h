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
#ifndef _xbase_lang_h_
#define _xbase_lang_h_
#include "xarea.h"
#include "xcsvi_anl.h"
#include "xexpression.h"
#include "xstack_fifo_.h"
#define CODE_SET 0
#define CODE_IF 1
#define CODE_FOR 2
#define CODE_PRINTF 3
#define CODE_BREAK 4
#define CODE_CONTINUE 5
#define ERR_C01 0
#define ERR_C02 1
#define ERR_C03 2
#define ERR_C04 3
#define ERR_C05 4
#define ERR_C06 5
#define ERR_C07 6
#define ERR_C08 7
#define ERR_C09 8
#define ERR_C10 9
#define ERR_C11 10
#define ERR_C12 11
#define ERR_C13 12
class L_err_C {
public:
    VariableArea_C *message;
    int line;
    int error_code;
    L_err_C(char *p0,int p1,int p2);
    ~L_err_C();
};
class L_code_C {
public:
    int code_kind;
    unsigned char *link;
    L_code_C(int p0);
    ~L_code_C();
};
class L_if_C {
public:
    expList_C *siki;
    VariableArray_C *code_list_true;
    VariableArray_C *code_list_false;
    L_if_C(expList_C *p);
    ~L_if_C();
};
class L_for_C {
public:
    int var_num;
    expList_C *loop;
    VariableArray_C *code_list;
    L_for_C(int p0,expList_C *p1);
    ~L_for_C();
};
class L_set_C {
public:
    int var_num;
    expList_C *siki;
    L_set_C(int p0,expList_C *p1);
};
class L_printf_C {
public:
    VariableArea_C *format;
    VariableArray_C *siki;
    L_printf_C(char *p);
    ~L_printf_C();
};
class xbase_lang_C {
public:
    VariableArray_C *code_list;
    Expression_C *express;
    VariableArray_C *err_list;
    CsvAnl_C *csvi;
    int g_line_num;
    VariableArray_C *curr_code_list;
    L_code_C *curr_code;
    VariableStack_C *code_stack;
    VariableArea_C *w_pr_format;
    bool contFlg;
    bool breakFlg;
    bool returnFlg;
    void (*execUserCode_rtn)(L_code_C *code);
    xbase_lang_C(Expression_C *p);
    ~xbase_lang_C();
    bool csvRead(char *buff,int line_num);
    void if_rtn();
    void else_rtn();
    void endif_rtn();
    void for_rtn();
    void endfor_rtn();
    void set_rtn();
    void printf_rtn();
    void break_rtn();
    void continue_rtn();
    void error(int code,char *msg);
    bool okCheck();
    void codeExec_Init();
    void prgMoni();
    void prgMoni_sub(VariableArray_C *list,int nest);
    void errorMoni();
    void codeExec();
    void codeExec_sub(VariableArray_C *list,int nest);
    void execSet_rtn(L_set_C *p);
    void execIf_rtn(L_if_C *p,int nest);
    void execFor_rtn(L_for_C *p,int nest);
    void execPrintf_rtn(L_printf_C *p);
    void execContinue_rtn();
    void execBreak_rtn();
};
#endif  // _xbase_lang_h_
