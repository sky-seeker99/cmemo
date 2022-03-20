/*

 * Copyright (c) 2003 Shigeru Kasuya (sky_seeker99@users.sourceforge.jp)
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
//
// Variable length Area template class
//

#ifndef _T_AREA_H_
#define _T_AREA_H_

template <class T_TYPE>
class VariableArea_T{
  private:
    T_TYPE *g_buff;
	int     g_buff_size;
	int     g_buff_ptr;

  public:
    VariableArea_T(int size);
    VariableArea_T(char *p);
    ~VariableArea_T();
    T_TYPE *check(int ptr);
    void set(int ptr,T_TYPE c);
    void set(T_TYPE c);
    void set_n(T_TYPE *str,int len){for(int i=0;i<len;i++){set(*(str+i));}}
    void set_string(char *str);
    void cat_string(char *str);
    void init(){g_buff_ptr = 0;}
    T_TYPE *c_str(){return(g_buff);}   
    char *cc_str(){return((char *)g_buff);}
    int  ptrGet(){return(g_buff_ptr);}
    int  len();
    void int_set(int p); 
    void int_cat(int p);
    char getChar(int ptr);
  
  private:
    void mem_cpy(char *des,char *src,int len);
};


//
// Variable Array Area template class
// ※アロケーション先も削除するので注意が必要
//

#ifdef ARY_LOOP
#undef ARY_LOOP
#endif
#ifdef ARY_LOOP_DIRECT
#undef ARY_LOOP_DIRECT
#endif

#define ARY_LOOP(PTR,TYPE,ARY) for(ARY->get_init();;){ \
								 TYPE *PTR = (TYPE *)ARY->get(); \
								 if (PTR == NULL)           {break;}
#define ARY_LOOP_DIRECT(PTR,TYPE,ARY) ARY->get_init(); \
                                 for(int i=0;i<ARY->getStorePtr();i++){ \
								 TYPE *PTR = (TYPE *)ARY->get();

#ifndef LOOP_END
  #define LOOP_END }
#endif

template <class T_TYPE>
class VariableArray_T{
  private:
    T_TYPE **g_ary;
    int      g_ary_size;
    int      g_ary_store_ptr;
    int      g_ary_get_ptr;
  public:
    bool     delFlag;
    VariableArray_T (int size);
    ~VariableArray_T();
    T_TYPE  *alloc_ptr;
    void     mem_alloc(T_TYPE *ptr);
    void     mem_alloc(int no,T_TYPE *ptr);
    void     mem_alloc(){mem_alloc(alloc_ptr);}
    void     clear();
    T_TYPE  *get(int no);
    T_TYPE  *get(){g_ary_get_ptr++; return(get(g_ary_get_ptr-1));}
    void     get_init(){g_ary_get_ptr = 0;}
    int      getStorePtr(){return(g_ary_store_ptr);}
    void     setStorePtr(int no){g_ary_store_ptr = no;}
    void     setReadPtr(int no){g_ary_get_ptr = no;}
    int      getReadPtr(){return(g_ary_get_ptr);}
    int      getWritePtr(){return(g_ary_store_ptr);}
    T_TYPE **getBody(void){return(g_ary);}
    void insert(int no,int insert_num);
};


//
// Variable Stack class  ----------------------------
//
template <class T_TYPE>
class VariableStack_T : public VariableArray_T<T_TYPE>{
  public:
    VariableStack_T(int size) : VariableArray_T<T_TYPE>(size){}
    void push(T_TYPE *ptr);
    T_TYPE *check();
    T_TYPE *pop();
};


//
// Variable Fifo class  ----------------------------
//
template <class T_TYPE>
class VariableFifo_T : public VariableArray_T<T_TYPE>{
  public:
    
    VariableFifo_T(int size): VariableArray_T<T_TYPE>(size){}
    void push(T_TYPE *ptr);
    T_TYPE *check();
    T_TYPE *pop();
    void defrag();  // たまにデフラグしないとエリアを大きくとるよ!
};


#endif // _T_AREA_H_

