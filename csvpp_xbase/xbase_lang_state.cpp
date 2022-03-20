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
#include <stdio.h>
#include <string.h>
#include "xbase_lang_state.h"
void userCodeForState_rtn(L_code_C *code) {
    if ((code->code_kind == CODE_ACT)
            || (code->code_kind == CODE_STOP)
            || (code->code_kind == CODE_GO)) {
        stateMove_C *s = (stateMove_C *)code->link;
        xbase_lang_state_C *t = (xbase_lang_state_C *)s->link_this;
        t->under_rtn(s);
    }
}
stateMove_C::stateMove_C(char *p0,char *p0_1,int p1) {
    name = new sChar(p0);
    sv_name = new sChar(p0_1);
    kind = p1;
    link_this = NULL;
}
stateMove_C::~stateMove_C() {
    delete name;
    delete sv_name;
}
state_C::state_C(unsigned char *p0,Expression_C *p1) {
    name = new sChar((char *)p0);
    code = new xbase_lang_C(p1);
}
state_C::~state_C() {
    delete name;
    delete code;
}
xbase_lang_state_C::xbase_lang_state_C(Expression_C *p0) {
    it = new MemHeader_T;
    state_fp = new MEM_FP(it);
    act_state_fp = new MEM_FP(it);
    stateMove_fp = new VariableArray_C(10);
    act_fp = new VariableArray_C(10);
    stop_fp = new VariableArray_C(10);
    err_list = new VariableArray_C(10);
    csvi = new CsvAnl_C;
    express = p0;
    g_line_num = 0;
    curr_state = NULL;
    start_fp = new VariableArray_C(10);
    top_code = new xbase_lang_C(express);
    curr_code_list = top_code;
    top_state = NULL;
}
xbase_lang_state_C::~xbase_lang_state_C() {
    sChar *w;
    stateMove_C *s;
    L_err_C *err;
    MEM_LOOP(curr_state,state_C,state_fp) delete curr_state;
    LOOP_END  delete state_fp;
    ARY_LOOP(w,sChar,start_fp) delete w;
    LOOP_END  delete start_fp;
    ARY_LOOP(s,stateMove_C,stateMove_fp) delete s;
    LOOP_END  delete stateMove_fp;
    ARY_LOOP(err,L_err_C,err_list) delete err;
    LOOP_END  delete err_list;
    delete csvi;
    delete top_code;
    delete act_state_fp;
    delete act_fp;
    delete stop_fp;
}
bool xbase_lang_state_C::csvRead(char *buff,int line_num) {
    g_line_num = line_num;
    csvi->Exec(buff);
    if (curr_code_list->csvRead(buff,line_num)) {
        return(true);
    }

    bool matchFlg = false;
    CSV_LOOP(csvi,str,SP_PUSH)
    if (strcmp(str,"#st_init") == 0) {
        matchFlg = true;
        st_init_rtn();
        break;
    } else if (strcmp(str,"#st") == 0) {
        matchFlg = true;
        st_rtn();
        break;
    } else if (strcmp(str,"#act") == 0) {
        matchFlg = true;
        act_rtn();
        break;
    } else if (strcmp(str,"#stop") == 0) {
        matchFlg = true;
        stop_rtn();
        break;
    } else if (strcmp(str,"#go") == 0) {
        matchFlg = true;
        go_rtn();
        break;
    }
    break;
    LOOP_END

    return(matchFlg);
}
void xbase_lang_state_C::st_init_rtn() {
    sChar *w;
    CSV_LOOP(csvi,str,SP_PUSH)
    w = new sChar((char *)str);
    start_fp->mem_alloc((unsigned char *)w);
    LOOP_END
}
void xbase_lang_state_C::st_rtn() {
    char *label=NULL;
    CSV_LOOP(csvi,str,SP_PUSH)
    if (label==NULL) {
        label = str;
        it->srch_key = str;
        if (state_fp->mem_srch() == 0) {
            error(ERR_B01,str);
            return;
        }
        curr_state = new state_C(str,express);
        if (top_state == NULL) {
            top_state = curr_state;
        }
        curr_code_list = curr_state->code;
        it->alloc_ptr = curr_state;
        state_fp->mem_srch_alloc();
        break;
    }
    LOOP_END

    if (label == NULL) {
        error(ERR_B02,"");
    }

}
void xbase_lang_state_C::act_rtn() {
    char *label=NULL;
    CSV_LOOP(csvi,str,SP_PUSH)
    if (label==NULL) {
        if (curr_code_list == NULL) {
            error(ERR_B03,str);
            return;
        }
        label = str;
        break;
    }
    LOOP_END

    if (label == NULL) {
        error(ERR_B04,"");
        return;
    }
    L_code_C *code = new L_code_C(CODE_ACT);
    stateMove_C *s = new stateMove_C(label,curr_state->name->c_str(),CODE_ACT);
    s->link_this = (unsigned char *)this;
    code->link = (unsigned char *)s;
    curr_code_list->curr_code_list->mem_alloc((unsigned char *)code);
    stateMove_fp->mem_alloc((unsigned char *)s);

}
void xbase_lang_state_C::stop_rtn() {
    char *label=NULL;
    CSV_LOOP(csvi,str,SP_PUSH)
    if (label==NULL) {
        if (curr_code_list == NULL) {
            error(ERR_B05,str);
            return;
        }
        label = str;
        break;
    }
    LOOP_END

    if (label == NULL) {
        error(ERR_B06,"");
        return;
    }
    L_code_C *code = new L_code_C(CODE_STOP);
    stateMove_C *s = new stateMove_C(label,curr_state->name->c_str(),CODE_STOP);
    s->link_this = (unsigned char *)this;
    code->link = (unsigned char *)s;
    curr_code_list->curr_code_list->mem_alloc((unsigned char *)code);
    stateMove_fp->mem_alloc((unsigned char *)s);

}
void xbase_lang_state_C::go_rtn() {
    char *label=NULL;
    CSV_LOOP(csvi,str,SP_PUSH)
    if (label==NULL) {
        if (curr_code_list == NULL) {
            error(ERR_B07,str);
            return;
        }
        label = str;
        break;
    }
    LOOP_END

    if (label == NULL) {
        error(ERR_B08,"");
        return;
    }
    L_code_C *code = new L_code_C(CODE_GO);
    stateMove_C *s = new stateMove_C(label,curr_state->name->c_str(),CODE_GO);
    s->link_this = (unsigned char *)this;
    code->link = (unsigned char *)s;
    curr_code_list->curr_code_list->mem_alloc((unsigned char *)code);
    stateMove_fp->mem_alloc((unsigned char *)s);

}
void xbase_lang_state_C::error(int code,char *msg) {
    L_err_C *err = new L_err_C(msg,g_line_num,code);
    err_list->mem_alloc((unsigned char *)err);

}
bool xbase_lang_state_C::okCheck() {
    if (err_list->getStorePtr() > 0) {
        return(false);
    }
    if (top_code->okCheck() == false) {
        return(false);
    }
    MEM_LOOP(curr_state,state_C,state_fp)
    if (curr_state->code->okCheck() == false) {
        return(false);
    }
    LOOP_END
    return(true);

}
void xbase_lang_state_C::prgMoni() {
    printf("[state]:top\n");
    top_code->prgMoni();
    MEM_LOOP(curr_state,state_C,state_fp)
    printf("[state]:%s\n",curr_state->name->c_str());
    curr_state->code->prgMoni();
    LOOP_END
}
void xbase_lang_state_C::errorMoni() {
    L_err_C *err;
    char *msg;
    ARY_LOOP(err,L_err_C,err_list)
    if (err->error_code == ERR_B01) {
        msg = "#stの状態が重複しています";
    } else if (err->error_code == ERR_B02) {
        msg = "#st文でパラメータが足りません";
    } else if (err->error_code == ERR_B03) {
        msg = "#act文は#stの中でしか使用できません。";
    } else if (err->error_code == ERR_B04) {
        msg = "#act文でパラメータが足りません";
    } else if (err->error_code == ERR_B05) {
        msg = "#stop文は#stの中でしか使用できません。";
    } else if (err->error_code == ERR_B06) {
        msg = "#stop文でパラメータが足りません";
    } else if (err->error_code == ERR_B07) {
        msg = "#go文は#stの中でしか使用できません。";
    } else if (err->error_code == ERR_B08) {
        msg = "#go文でパラメータが足りません";
    }
    if (strcmp(err->message->cc_str(),"")==0) {
        printf("[line:%-4d][code:%-2d][%s]\n",err->line,err->error_code,msg);
    } else {
        printf("[line:%-4d][code:%-2d][%s - %s]\n",err->line,err->error_code,err->message->cc_str(),msg);
    }
    LOOP_END
    top_code->errorMoni();
    MEM_LOOP(curr_state,state_C,state_fp)
    curr_state->code->errorMoni();
    LOOP_END
}
void xbase_lang_state_C::MainLoop_init() {
    sChar *w;
    if (start_fp->getStorePtr() > 0) {
        ARY_LOOP(w,sChar,start_fp)
        it->srch_key = w->c_str();
        if (state_fp->mem_srch() != 0) {
            continue;
        }
        it->alloc_ptr = state_fp->mem_link_ptr_rd();
        act_state_fp->mem_srch_alloc();
        LOOP_END
    } else {
        it->srch_key = top_state->name->c_str();
        it->alloc_ptr = top_state;
        act_state_fp->mem_srch_alloc();
    }

    top_code->codeExec_Init();
    top_code->execUserCode_rtn = userCodeForState_rtn;
    top_code->codeExec();

    MEM_LOOP(curr_state,state_C,state_fp)
    curr_state->code->execUserCode_rtn = userCodeForState_rtn;
    curr_state->code->codeExec_Init();
    LOOP_END
}
void xbase_lang_state_C::userFunc_set(void (*p)(L_code_C *code)) {
    top_code->execUserCode_rtn = p;
    MEM_LOOP(curr_state,state_C,state_fp)
    curr_state->code->execUserCode_rtn = p;
    LOOP_END
}
bool xbase_lang_state_C::MainLoop() {
    bool flg=false;
    MEM_LOOP(curr_state,state_C,act_state_fp)
    curr_state->code->codeExec();
    LOOP_END

    ARY_LOOP(curr_state,state_C,stop_fp)
    it->srch_key = curr_state->name->c_str();
    if (act_state_fp->mem_srch() != 0) {
        continue;
    }
    act_state_fp->mem_del();
    LOOP_END
    ARY_LOOP(curr_state,state_C,act_fp)
    it->srch_key = curr_state->name->c_str();
    if (act_state_fp->mem_srch() == 0) {
        continue;
    }
    it->alloc_ptr = curr_state;
    act_state_fp->mem_srch_alloc();
    LOOP_END
    MEM_LOOP(curr_state,state_C,act_state_fp)
    flg=true;
    LOOP_END
    return(flg);

}
void xbase_lang_state_C::under_rtn(stateMove_C *s) {
    sChar *w;
    state_C *c;
    if ((s->kind == CODE_ACT) || (s->kind == CODE_GO)) {
        it->srch_key = s->name->c_str();
        if (state_fp->mem_srch() == 0) {
            c = (state_C *)state_fp->mem_link_ptr_rd();
            act_fp->mem_alloc((unsigned char *)c);
        }
    }

    if ((s->kind == CODE_STOP) || (s->kind == CODE_GO)) {
        if (s->kind == CODE_STOP) {
            it->srch_key = s->name->c_str();
        } else                      {
            it->srch_key = s->sv_name->c_str();
        }
        if (act_state_fp->mem_srch() == 0) {
            c = (state_C *)act_state_fp->mem_link_ptr_rd();
            stop_fp->mem_alloc((unsigned char *)c);
        }
    }

}
