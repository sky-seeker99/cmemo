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
