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
#ifndef _xbase_lang_state_h_
#define _xbase_lang_state_h_
#include "../csvpp_xbase/xarea.h"
#include "../csvpp_xbase/xcsvi_anl.h"
#include "../csvpp_xbase/xread64.h"
#include "../csvpp_xbase/xexpression.h"
#include "../csvpp_xbase/xstack_fifo_.h"
#include "../csvpp_xbase/xbase_lang.h"
#define CODE_ACT 10
#define CODE_STOP 11
#define CODE_GO 12
#define ERR_B01 20
#define ERR_B02 21
#define ERR_B03 22
#define ERR_B04 23
#define ERR_B05 24
#define ERR_B06 25
#define ERR_B07 26
#define ERR_B08 27
void userCodeForState_rtn(L_code_C *code);
class stateMove_C {
public:
    sChar *name;
    sChar *sv_name;
    int kind;
    unsigned char *link_this;
    stateMove_C(char *p0,char *p0_1,int p1);
    ~stateMove_C();
};
class state_C {
public:
    sChar *name;
    xbase_lang_C *code;
    state_C(unsigned char *p0,Expression_C *p1);
    ~state_C();
};
class xbase_lang_state_C {
public:
    MemHeader_T *it;
    MEM_FP *state_fp;
    MEM_FP *act_state_fp;
    VariableArray_C *stateMove_fp;
    VariableArray_C *act_fp;
    VariableArray_C *stop_fp;
    CsvAnl_C *csvi;
    int g_line_num;
    xbase_lang_C *curr_code_list;
    state_C *curr_state;
    VariableArray_C *start_fp;
    state_C *top_state;
    VariableArray_C *err_list;
    Expression_C *express;
    xbase_lang_C *top_code;
    xbase_lang_state_C(Expression_C *p0);
    ~xbase_lang_state_C();
    bool csvRead(char *buff,int line_num);
    void st_init_rtn();
    void st_rtn();
    void act_rtn();
    void stop_rtn();
    void go_rtn();
    void error(int code,char *msg);
    bool okCheck();
    void prgMoni();
    void errorMoni();
    void MainLoop_init();
    void userFunc_set(void (*p)(L_code_C *code));
    bool MainLoop();
    void under_rtn(stateMove_C *s);
};
#endif  // _xbase_lang_state_h_
