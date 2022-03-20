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
#include "xbase_lang.h"
L_err_C::L_err_C(char *p0,int p1,int p2) {
    message = new VariableArea_C(10);
    message->set_string(p0);
    line = p1;
    error_code = p2;
}
L_err_C::~L_err_C() {
    delete message;
}
L_code_C::L_code_C(int p0) {
    code_kind = p0;
    link = NULL;
}
L_code_C::~L_code_C() {
    L_if_C *w_if;
    L_for_C *w_for;
    L_set_C *w_set;
    L_printf_C *w_printf;
    if (link == NULL) {
        return;
    }

    if (code_kind == CODE_SET) {
        w_set = (L_set_C *)link;
        delete w_set;
    } else if (code_kind == CODE_IF) {
        w_if = (L_if_C *)link;
        delete w_if;
    } else if (code_kind == CODE_FOR) {
        w_for = (L_for_C *)link;
        delete w_for;
    } else if (code_kind == CODE_PRINTF) {
        w_printf = (L_printf_C *)link;
        delete w_printf;
    }
}
L_if_C::L_if_C(expList_C *p) {
    siki = p;
    code_list_true = new VariableArray_C(10);
    code_list_false = new VariableArray_C(10);
}
L_if_C::~L_if_C() {
    L_code_C *code;
    ARY_LOOP(code,L_code_C,code_list_true) delete code;
    LOOP_END  delete code_list_true;
    ARY_LOOP(code,L_code_C,code_list_false) delete code;
    LOOP_END  delete code_list_false;
}
L_for_C::L_for_C(int p0,expList_C *p1) {
    var_num = p0;
    loop = p1;
    code_list = new VariableArray_C(10);
}
L_for_C::~L_for_C() {
    L_code_C *code;
    ARY_LOOP(code,L_code_C,code_list) delete code;
    LOOP_END  delete code_list;
}
L_set_C::L_set_C(int p0,expList_C *p1) {
    var_num = p0;
    siki = p1;
}
L_printf_C::L_printf_C(char *p) {
    format = new VariableArea_C(10);
    format->set_string(p);
    siki = new VariableArray_C(10);
}
L_printf_C::~L_printf_C() {
    delete format;
    delete siki;
}
xbase_lang_C::xbase_lang_C(Expression_C *p) {
    code_list = new VariableArray_C(10);
    code_stack = new VariableStack_C(10);
    err_list = new VariableArray_C(10);
    w_pr_format = new VariableArea_C(10);
    express = p;
    csvi = new CsvAnl_C;
    curr_code_list = code_list;
    curr_code = NULL;
    code_stack->push((unsigned char *)code_list);
}
xbase_lang_C::~xbase_lang_C() {
    L_code_C *code;
    L_err_C *err;
    int *mode;
    ARY_LOOP(code,L_code_C,code_list) delete code;
    LOOP_END  delete code_list;
    ARY_LOOP(err,L_err_C,err_list) delete err;
    LOOP_END  delete err_list;
    delete code_stack;
    delete csvi;
}
bool xbase_lang_C::csvRead(char *buff,int line_num) {
    g_line_num = line_num;
    bool matchFlg = false;
    csvi->Exec(buff);

    CSV_LOOP(csvi,str,SP_PUSH)
    if (strcmp(str,"#if") == 0) {
        matchFlg = true;
        if_rtn();
        break;
    } else if (strcmp(str,"#else") == 0) {
        matchFlg = true;
        else_rtn();
        break;
    } else if (strcmp(str,"#endif") == 0) {
        matchFlg = true;
        endif_rtn();
        break;
    } else if (strcmp(str,"#for") == 0) {
        matchFlg = true;
        for_rtn();
        break;
    } else if (strcmp(str,"#endfor") == 0) {
        matchFlg = true;
        endfor_rtn();
        break;
    } else if (strcmp(str,"#set") == 0) {
        matchFlg = true;
        set_rtn();
        break;
    } else if (strcmp(str,"#printf") == 0) {
        matchFlg = true;
        printf_rtn();
        break;
    } else if (strcmp(str,"#break") == 0) {
        matchFlg = true;
        break_rtn();
        break;
    } else if (strcmp(str,"#continue") == 0) {
        matchFlg = true;
        continue_rtn();
        break;
    }
    break;
    LOOP_END

    return(matchFlg);
}
void xbase_lang_C::if_rtn() {
    char *siki = NULL;
    CSV_LOOP(csvi,str,SP_PUSH)
    if (siki == NULL) {
        siki = (char*)str;
        break;
    }
    LOOP_END

    if (siki == NULL) {
        error(ERR_C01,"");
        return;
    }
    expList_C *exp = express->compile(siki);
    if (express->okCheck() == false) {
        error(ERR_C02,siki);
        return;
    }

    // コード登録
    L_code_C *code = new L_code_C(CODE_IF);
    L_if_C *lfc = new L_if_C(exp);
    code->link = (unsigned char *)lfc;
    curr_code_list->mem_alloc((unsigned char *)code);

    // カレントの変更
    code_stack->push((unsigned char *)curr_code_list);
    curr_code = code;
    curr_code_list = lfc->code_list_true;

}
void xbase_lang_C::else_rtn() {
    if (curr_code == NULL) {
        error(ERR_C03,"");
        return;
    }
    if (curr_code->code_kind != CODE_IF) {
        error(ERR_C03,"");
        return;
    }

    // カレントの変更
    L_if_C *lfc = (L_if_C *)curr_code->link;
    curr_code_list = lfc->code_list_false;


}
void xbase_lang_C::endif_rtn() {
    if (curr_code == NULL) {
        error(ERR_C04,"");
        return;
    }
    if (curr_code->code_kind != CODE_IF) {
        error(ERR_C04,"");
        return;
    }

    // カレントの変更
    curr_code_list = (VariableArray_C *)code_stack->pop();
    VariableArray_C *tmp_list = (VariableArray_C *)code_stack->check();
    if (tmp_list == NULL) {
        curr_code = NULL;
    } else                 {
        curr_code = (L_code_C *)tmp_list->get
                    (tmp_list->getStorePtr()-1);
    }

}
void xbase_lang_C::for_rtn() {
    char *var = NULL;
    char *siki = NULL;
    CSV_LOOP(csvi,str,SP_PUSH)
    if (var == NULL) {
        var = (char*)str;
        continue;
    }
    if (siki == NULL) {
        siki = (char*)str;
        break;
    }
    LOOP_END

    if (siki == NULL) {
        error(ERR_C05,"");
        return;
    }
    expList_C *exp = express->compile(siki);
    if (express->okCheck() == false) {
        error(ERR_C06,siki);
        return;
    }
    int num = express->VarSrchStore(var);

    // コード登録
    L_code_C *code = new L_code_C(CODE_FOR);
    L_for_C *lfc = new L_for_C(num,exp);
    code->link = (unsigned char *)lfc;
    curr_code_list->mem_alloc((unsigned char *)code);

    // カレントの変更
    code_stack->push((unsigned char *)curr_code_list);
    curr_code = code;
    curr_code_list = lfc->code_list;

}
void xbase_lang_C::endfor_rtn() {
    if (curr_code == NULL) {
        error(ERR_C07,"");
        return;
    }
    if (curr_code->code_kind != CODE_FOR) {
        error(ERR_C07,"");
        return;
    }

    // カレントの変更
    curr_code_list = (VariableArray_C *)code_stack->pop();
    VariableArray_C *tmp_list = (VariableArray_C *)code_stack->check();
    if (tmp_list == NULL) {
        curr_code = NULL;
    } else                 {
        curr_code = (L_code_C *)tmp_list->get
                    (tmp_list->getStorePtr()-1);
    }

}
void xbase_lang_C::set_rtn() {
    char *var = NULL;
    char *siki = NULL;
    CSV_LOOP(csvi,str,SP_PUSH)
    if (var == NULL) {
        var = (char*)str;
        continue;
    }
    if (siki == NULL) {
        siki = (char*)str;
        break;
    }
    LOOP_END
    if (siki == NULL) {
        error(ERR_C08,"");
        return;
    }
    expList_C *exp = express->compile(siki);
    if (express->okCheck() == false) {
        error(ERR_C09,siki);
        return;
    }
    int num = express->VarSrchStore(var);

    // コード登録
    L_code_C *code = new L_code_C(CODE_SET);
    L_set_C *lsc = new L_set_C(num,exp);
    code->link = (unsigned char *)lsc;
    curr_code_list->mem_alloc((unsigned char *)code);

}
void xbase_lang_C::printf_rtn() {
    char *format = NULL;
    L_printf_C *lpc;
    expList_C *exp;
    CSV_LOOP(csvi,str,SP_PUSH)
    if (format == NULL) {
        format = (char*)str;
        // コード登録
        L_code_C *code = new L_code_C(CODE_PRINTF);
        lpc = new L_printf_C(format);
        code->link = (unsigned char *)lpc;
        curr_code_list->mem_alloc((unsigned char *)code);
        continue;
    }
    exp = express->compile(str);
    if (express->okCheck() == false) {
        error(ERR_C11,str);
    }
    lpc->siki->mem_alloc((unsigned char *)exp);
    LOOP_END
    if (format == NULL) {
        error(ERR_C10,"");
        return;
    }

}
void xbase_lang_C::break_rtn() {
    if (curr_code == NULL) {
        error(ERR_C12,"");
        return;
    }
    VariableArray_C *tmp_list;
    L_code_C *w_code;
    bool matchFlg = false;
    ARY_LOOP(tmp_list,VariableArray_C,code_stack)
    w_code = (L_code_C *)tmp_list->get
             (tmp_list->getStorePtr()-1);
    if (w_code->code_kind == CODE_FOR) {
        matchFlg = true;
        break;
    }
    LOOP_END

    if (matchFlg == false) {
        error(ERR_C12,"");
        return;
    }

    // コード登録
    L_code_C *code = new L_code_C(CODE_BREAK);
    curr_code_list->mem_alloc((unsigned char *)code);
}
void xbase_lang_C::continue_rtn() {
    if (curr_code == NULL) {
        error(ERR_C13,"");
        return;
    }
    VariableArray_C *tmp_list;
    L_code_C *w_code;
    bool matchFlg = false;
    ARY_LOOP(tmp_list,VariableArray_C,code_stack)
    w_code = (L_code_C *)tmp_list->get
             (tmp_list->getStorePtr()-1);
    if (w_code->code_kind == CODE_FOR) {
        matchFlg = true;
        break;
    }
    LOOP_END

    if (matchFlg == false) {
        error(ERR_C13,"");
        return;
    }

    // コード登録
    L_code_C *code = new L_code_C(CODE_CONTINUE);
    curr_code_list->mem_alloc((unsigned char *)code);
}
void xbase_lang_C::error(int code,char *msg) {
    L_err_C *err = new L_err_C(msg,g_line_num,code);
    err_list->mem_alloc((unsigned char *)err);

}
bool xbase_lang_C::okCheck() {
    if (err_list->getStorePtr() > 0) {
        return(false);
    }
    return(true);

}
void xbase_lang_C::codeExec_Init() {
    express->codeExec_Init();
}
void xbase_lang_C::prgMoni() {
    if (okCheck() == false) {
        return;
    }
    express->varListMon();
    prgMoni_sub(code_list,0);
}
void xbase_lang_C::prgMoni_sub(VariableArray_C *list,int nest) {
    L_code_C *code;
    L_set_C *w_set;
    L_if_C *w_if;
    L_for_C *w_for;
    L_printf_C *w_printf;
    expList_C *siki;

    ARY_LOOP(code,L_code_C,list)
    for(int i=0;i<nest;i++) {
        printf("  ");
    }
    if (code->code_kind == CODE_SET) {
        w_set = (L_set_C *)code->link;
        printf("#set,%d\n",w_set->var_num);
        express->codeListMon(w_set->siki);
    } else if (code->code_kind == CODE_IF) {
        w_if = (L_if_C *)code->link;
        printf("#if\n");
        express->codeListMon(w_if->siki);
        prgMoni_sub(w_if->code_list_true,nest+1);
        printf("#else\n");
        prgMoni_sub(w_if->code_list_false,nest+1);
        printf("#endif\n");
    } else if (code->code_kind == CODE_FOR) {
        w_for = (L_for_C *)code->link;
        printf("#for\n");
        express->codeListMon(w_for->loop);
        prgMoni_sub(w_for->code_list,nest+1);
        printf("#endfor\n");
    } else if (code->code_kind == CODE_PRINTF) {
        w_printf = (L_printf_C *)code->link;
        printf("#printf,%s\n",w_printf->format->cc_str());
        ARY_LOOP(siki,expList_C,w_printf->siki)
        express->codeListMon(siki);
        LOOP_END
    } else if (code->code_kind == CODE_BREAK) {
        printf("#break\n");
    } else if (code->code_kind == CODE_CONTINUE) {
        printf("#continue\n");
    } else {
        printf("#user_define %d\n",code->code_kind);
    }
    LOOP_END

}
void xbase_lang_C::errorMoni() {
    L_err_C *err;
    char *msg;
    ARY_LOOP(err,L_err_C,err_list)
    if (err->error_code == ERR_C01) {
        msg = "#if文で条件式がありません。";
    } else if (err->error_code == ERR_C02) {
        msg = "#if文で条件式にシンタックスエラーがあります";
    } else if (err->error_code == ERR_C03) {
        msg = "#else文で対応するif文がありません。";
    } else if (err->error_code == ERR_C04) {
        msg = "#endif文で対応するif文がありません。";
    } else if (err->error_code == ERR_C05) {
        msg = "#for文でパラメータが足りません";
    } else if (err->error_code == ERR_C06) {
        msg = "#for文の繰り返しの式にシンタックスエラーがあります";
    } else if (err->error_code == ERR_C07) {
        msg = "#endfor文で対応するfor文がありません。";
    } else if (err->error_code == ERR_C08) {
        msg = "#set文でパラメータが足りません";
    } else if (err->error_code == ERR_C09) {
        msg = "#set文の式にシンタックスエラーがあります";
    } else if (err->error_code == ERR_C10) {
        msg = "#printf文でパラメータが足りません";
    } else if (err->error_code == ERR_C11) {
        msg = "#printf文の式にシンタックスエラーがあります";
    } else if (err->error_code == ERR_C12) {
        msg = "#break文で対応するfor文がありません。";
    } else if (err->error_code == ERR_C13) {
        msg = "#continue文で対応するfor文がありません。";
    }

    if (strcmp(err->message->cc_str(),"")==0) {
        printf("[line:%-4d][code:%-2d][%s]\n",err->line,err->error_code,msg);
    } else {
        printf("[line:%-4d][code:%-2d][%s - %s]\n",err->line,err->error_code,err->message->cc_str(),msg);
    }
    LOOP_END

}
void xbase_lang_C::codeExec() {
    if (okCheck() == false) {
        return;
    }
    contFlg=false;
    breakFlg=false;
    returnFlg=false;
    codeExec_sub(code_list,0);

}
void xbase_lang_C::codeExec_sub(VariableArray_C *list,int nest) {
    L_code_C *code;
    L_set_C *w_set;
    L_if_C *w_if;
    L_for_C *w_for;
    L_printf_C *w_printf;
    expList_C *siki;

    if ((contFlg == true) ||  (breakFlg == true) ||  (returnFlg == true)) {
        return;
    }

    ARY_LOOP(code,L_code_C,list)
    if      (code->code_kind == CODE_SET     ) {
        w_set    = (L_set_C *)code->link;
        execSet_rtn   (w_set);
    } else if (code->code_kind == CODE_IF      ) {
        w_if     = (L_if_C *)code->link;
        execIf_rtn    (w_if,nest+1);
    } else if (code->code_kind == CODE_FOR     ) {
        w_for    = (L_for_C *)code->link;
        execFor_rtn   (w_for,nest+1);
    } else if (code->code_kind == CODE_PRINTF  ) {
        w_printf = (L_printf_C *)code->link;
        execPrintf_rtn(w_printf);
    } else if (code->code_kind == CODE_BREAK   ) {
        execBreak_rtn ();
    } else if (code->code_kind == CODE_CONTINUE) {
        execContinue_rtn();
    } else                                      {
        execUserCode_rtn(code);
    }
    if ((contFlg == true) ||  (breakFlg == true) ||  (returnFlg == true)) {
        break;
    }
    LOOP_END

}
void xbase_lang_C::execSet_rtn(L_set_C *p) {
    T_TYPE w = express->codeExec(p->siki,false);
    express->valueWrite(p->var_num,w);

}
void xbase_lang_C::execIf_rtn(L_if_C *p,int nest) {
    T_TYPE w = express->codeExec(p->siki,false);
    if (w == (T_TYPE)0) {
        codeExec_sub(p->code_list_false,nest);
    } else               {
        codeExec_sub(p->code_list_true,nest);
    }

}
void xbase_lang_C::execFor_rtn(L_for_C *p,int nest) {
    T_TYPE loop = express->codeExec(p->loop,false);
    T_TYPE w = (T_TYPE)0;
    express->valueWrite(p->var_num,w);
    for(;;) {
        if (w >= loop) {
            break;
        }
        codeExec_sub(p->code_list,nest);
        w = express->valueRead(p->var_num);
        w++;
        express->valueWrite(p->var_num,w);
        if (contFlg == true) {
            contFlg = false;
        }
        if (breakFlg == true) {
            breakFlg = false;
            break;
        }
        if (returnFlg == true) {
            break;
        }
    }

}
void xbase_lang_C::execPrintf_rtn(L_printf_C *p) {
    char *format = p->format->cc_str();
    int len = strlen(format);

    int state=0;
    int var_num=0;
    T_TYPE val;
    w_pr_format->init();
    for(int i=0;i<len;i++) {
        w_pr_format->set
        (*(format+i));
        if (state == 0) {
            if (*(format+i) == '%') {
                state = 1;
            }
        } else if (state == 1) {
            if ((*(format+i) == 'd') || (*(format+i) == 'x') || (*(format+i) == 'D') || (*(format+i) == 'X')) {
                state = 0;
                w_pr_format->set
                (0x00);
                expList_C *siki = (expList_C *)p->siki->get
                                  (var_num);
                var_num++;
                if (siki == NULL) {
                    val = (T_TYPE)0;
                } else             {
                    val = express->codeExec(siki,false);
                }
                printf(w_pr_format->cc_str(),val);
                w_pr_format->init();
            }
        }
    }
    w_pr_format->set
    (0x00);
    printf(w_pr_format->cc_str());
    printf("\n");
}
void xbase_lang_C::execContinue_rtn() {
    contFlg = true;
}
void xbase_lang_C::execBreak_rtn() {
    breakFlg = true;
}
