#pragma once
#ifndef _ASM_STRUCT
#define _ASM_STRUCT

#include <stdlib.h>
#include <vector>
#include <string>

#include "functionlib.h"
using namespace std;
#define EOF_TEXTARRAY 9999
#define EOF_VARTYPE 255
int __exe_size=0;
template <class T>
class Stack
{
public:
    Stack() {}
    Stack(T def)
    {
        _default = def;
    }
    void push(T a)
    {
        _stack.push_back(a);
    }
    T pop()
    {
        if (_stack.size() < 1)
            return _default;
        T sav = _stack.back();
        _stack.pop_back();
        return sav;
    }
    T get()
    {

        if (_stack.size() < 1)
            return _default;
        return _stack.back();
    }
    void duplicate()
    {
        if (_stack.size() > 0)
            _stack.push_back(_stack.back());
        else
        {
            _stack.push_back(_default);
            _stack.push_back(_default);
        }
    }
    T front()
    {
        if (_stack.size() > 0)
            return _stack.front();
        else
            return _default;
    }
    void swap()
    {

        T sav = pop();
        T sav2 = pop();
        push(sav);
        push(sav2);
    }
    void increase()
    {
        //  if( typeid(T).hash_code()==typeid(int).hash_code())
        //  {
        int sav = (int)pop();
        push(sav + 1);
        //  }
    }
    void decrease()
    {
        //  if( typeid(T).hash_code()==typeid(int).hash_code())
        //  {
        
        int sav = (int)pop();
        push(sav - 1);
        //  }
    }
    void clear()
    {
        _stack.clear();
        _stack.shrink_to_fit();
    }
    vector<T> _stack;
    T _default;
};

class Text
{
public:
    Text()
    {
        //_texts=t;
        _texts.clear();
        _texts.shrink_to_fit();
        position = 0;
        // _texts.push_back(cc);
        _it = _texts.begin();
    }
    int findText(char * str)
    {
         #ifdef __SPEED
        return -1;
        #endif
       // for (int i = 0; i < _texts.size(); i++)
       for (int i =  _texts.size()-1; i >=0; i--)
        {
            if (strcmp(str,_texts[i]) == 0)
            {
                return i;
            }
        }
        return -1;
    }
    int addText(string str,uint16_t si)
    {
              char *m = (char *)malloc(si + 1);
        memcpy(m, str.c_str(), si);
        m[str.size()] = 0;
        _texts.push_back(m);
        position++;
        return _texts.size() - 1;
    }
    int addText(string str)
    {
        int pos = findText((char *)str.c_str());
        if (pos > -1)
        {
                    #ifdef __TEST_DEBUG
        printf(" find text:%d %s\r\n",pos,str.c_str());
        #endif
            return pos;
        }
        char *m = (char *)malloc(str.size() + 1);
        memcpy(m, str.c_str(), str.size());
        m[str.size()] = 0;
        _texts.push_back(m);
        position++;
        #ifdef __TEST_DEBUG
        printf(" addtext:%d %s\r\n",_texts.size(),str.c_str());
        #endif
        return _texts.size() - 1;
    }
    void addAfter(int pos, string s)
    {
        _it = getChildAtPos(pos);
        // printf(" on recupere %d:%s\n",pos,(*__it).c_str());
        // if((*_it).compare(s)!=0)
        //{
        addAfter(s);
        position--;
        _it = getChildAtPos(position);
        position++;
        // }
    }
    void addBefore(int pos, string s)
    {
        _it = getChildAtPos(pos-1);
        // printf(" on recupere %d:%s\n",pos,(*__it).c_str());
        // if((*_it).compare(s)!=0)
        //{
        addBefore(s);
        position--;
        _it = getChildAtPos(position);
        position++;
        // }
    }
    /*
    void addAfter(string str)
    {
        int pos = findText(str);
        if (pos > -1)
        {
            _it = _texts.insert(next(_it), _texts[pos]);
        }
        else
        {
            char *m = (char *)malloc(str.size() + 1);
            memcpy(m, str.c_str(), str.size());
            m[str.size()] = 0;

            if(_it==_texts.end())
            {
                _texts.push_back(m);
                _it=_texts.end();
                _it--;
            }
            else
            {
             _it = _texts.insert(next(_it), m);
            }
        }
        position++;
    }*/
    void addAfter(string str)
    {
        int pos = findText((char *)str.c_str());
        char *tmp;
        if (pos > -1)
        {
            tmp = _texts[pos];
        }
        else
        {
            tmp = (char *)malloc(str.size() + 1);
            memcpy(tmp, str.c_str(), str.size());
            tmp[str.size()] = 0;
        }
        if (_it == _texts.end())
        {
            _texts.push_back(tmp);
            _it = _texts.end();
            _it--;
        }
        else
        {
            _it = _texts.insert(next(_it), tmp);
        }
        position++;
    }
    string back()
    {
              if (_texts.size() > 0)
            return string(_texts.back());
        else
            return "";
    }
    string front()
    {
        if (_texts.size() > 0)
            return string(_texts.front());
        else
            return "";
    }
    string textAt(int pos)
    {
      if(pos>0 and pos<_texts.size())
      {
        return string(_texts[pos]);
      }
      return "";
    }
    void pop_front()
    {
        if (_texts.size() > 0)
        {
            if (!isReused(0))
                free(_texts[0]);
            _texts.erase(_texts.begin());
        }
    }
    void addAfterNoDouble(string s)
    {

       // char *str;
        if (_it != _texts.end())
        {

            if (s.compare(string(*_it)) == 0)
            {

                return;
            }
        }

        addAfter(s);
    }
    void addBefore(string s)
    {
        int pos = findText((char *)s.c_str());
        if (pos > -1)
        {
            _it = _texts.insert(_it, _texts[pos]);
        }
        else
        {
            char *m = (char *)malloc(s.size() + 1);
            memcpy(m, s.c_str(), s.size());
            m[s.size()] = 0;
            _it = _texts.insert(_it, m);
        }
        _it++;
        position++;
    }
    void replaceText(int pos, string str)
    {
        if (pos >= 0 and pos < size())
        {
            if (!isReused(pos))
            {
                free(_texts[pos]);
            }
            char *m = (char *)malloc(str.size() + 1);
            memcpy(m, str.c_str(), str.size());
            m[str.size()] = 0;
            _texts[pos] = m;
        }
    }
    vector<char *>::iterator getChildAtPos(int pos)
    {
        int i = 0;
        if (pos >= _texts.size() || pos < 0)
        {
            return _texts.end();
        }
        for (vector<char *>::iterator it = _texts.begin(); it != _texts.end(); it++)
        {
            if (i == pos)
            {
                return it;
            }
            i++;
        }
        return _texts.end();
    }
    void putIteratorAtPos(int pos)
    {
        _it = getChildAtPos(pos);
        //position=pos-1;
    }
    void end()
    {
        _it = getChildAtPos(_texts.size() - 1);
    }
    void clear()
    {
// #ifndef __SPEED
       // int kk = 0;
        for (int i = 0; i < _texts.size(); i++)
        {
            char *c1 = _texts[i];
            if (c1 != NULL)
            {
                if (i != _texts.size() - 1)
                {
                    for (int j = i + 1; j < _texts.size(); j++)
                    {
                        if (_texts[j] == c1)
                            _texts[j] = NULL;
                    }
                }
            }
        }
//#endif
        for (int i = 0; i < _texts.size(); i++)
        {
            if (_texts[i] != NULL)
            {
                free(_texts[i]);
               // kk++;
            }
        }
        _texts.clear();
        _texts.shrink_to_fit();
        sp.clear();
        position = 0;
        _it = _texts.begin();
    }
    int size()
    {
        return _texts.size();
    }
    char *getText(int pos)
    {
        if (pos >= 0 and pos < _texts.size())
        {
            return _texts[pos];
        }
        else
        {
            return cc;
        }
    }
    bool isReused(int pos)
    {
        #ifdef __SPEED
        return false;
        #endif
        if (pos < 0 or pos >= _texts.size())
        {
            return false;
        }
        char *c = _texts[pos];
        for (int i = 0; i < _texts.size(); i++)
        {
            if (i != pos and strcmp(c, _texts[i]) == 0)
            {
                return true;
            }
        }
        return false;
    }
    void pop()
    {
        if (size() > 0)
        {
            if (!isReused(_texts.size() - 1))
            {
                free(_texts.back());
            }
            _texts.pop_back();
            _texts.shrink_to_fit();
            position--;
            _it = _texts.end();
            _it--;
        }
    }
    int get()
    {

        return position - 1;
    }
    void begin()
    {
        _it = _texts.begin();
        position = 0;
    }
    void display()
    {
        for (char *c : _texts)
        {
            printf("%s\r\n", c);
        }
    }
    Stack<int> sp;
    vector<char *> _texts;

private:
    char cc[1] = {'\0'};
    int position;
    vector<char *>::iterator _it;
};

Text all_text = Text();

enum class operandeType
{
  registers,
  floatregisters,
  boolregisters,
  l0_255,
  lm32_95,
  lm2048_2047,
  l0_15,
  lm1_15,
  l0_32760,
  l0_240,
  l0_31,
  lm128_127,
  lm32768_32512,
  l1_31,
  l7_22,
  l0_60,
  lm64_m4,
  l0_510,
  lm8_7,
  l0_1020,
  label,
  l0_FFFFFFFF,

};

enum class opCodeType
{
  standard,
  call,
  jump,
  jump_32aligned,
  call_32aligned,
  label,
  function_declaration,
  external_var,
  external_call,
  data,
  number,
  number_label,
  data_label,
  variable,
  not_known
};

enum class externalType
{
  function,
  value,
};

typedef struct
{
  externalType type;
  string name;
  void *ptr;
  int offset;
} asm_external;

typedef struct
{
  string opcde;
  string operandes;
  int error;
} line;

struct error_message_struct
{
  string error_message;
  int error;
};
error_message_struct asm_Error;

typedef struct
{
  error_message_struct error;
  int value;
  string label;
  uint32_t opcode;
} result_parse_operande;

class result_parse_line
{
  public:
  result_parse_line(){}
  // error_message_struct  error;
char *getText()
    {
        return all_text.getText(nameref);
    }
        void addText(string t)
    {
        nameref = all_text.addText(t);
    }
            void addText(string t,uint16_t si)
    {
        nameref = all_text.addText(t,si);
    }
  uint32_t bincode;
  uint32_t size;
  opCodeType op;
  int16_t nameref=EOF_TEXTARRAY;
  uint32_t address;
  bool align;
  // string debugtxt;
  int line;
  uint32_t (*calculateOfssetJump)(uint32_t value, uint32_t current_address, uint32_t destination_address) = NULL;
};

class parsedLines
{

  public:
  parsedLines(){};

  int size()
  {
    return parsed_lines.size();
  }
  
  result_parse_line *getChildAtPos(int pos)
  {
    if(pos>=0 and pos<parsed_lines.size())
    {
      return parsed_lines[pos];
    }
    else
    {
      return NULL;
    }

  }
  result_parse_line  *push_back(result_parse_line res)
  {
    result_parse_line *tmp=(result_parse_line *)malloc(sizeof(result_parse_line));
    memcpy(tmp,&res,sizeof(result_parse_line));
    parsed_lines.push_back(tmp);
    return parsed_lines.back();
  }


    vector<result_parse_line *>::iterator begin()
    {
      return parsed_lines.begin();
    }
    vector<result_parse_line *>::iterator end()
    {
      return parsed_lines.end();
    }

  void clear()
  {
    for(int i=0;i<parsed_lines.size();i++)
    {
      free(parsed_lines[i]);
    }
    parsed_lines.clear();
    parsed_lines.shrink_to_fit();
  }

vector<result_parse_line *>::iterator insert(vector<result_parse_line *>::iterator t,result_parse_line op)
{
  result_parse_line *tmp=(result_parse_line *)malloc(sizeof(result_parse_line));
    memcpy(tmp,&op,sizeof(result_parse_line));
  return parsed_lines.insert(t,tmp);
}
result_parse_line * last()
{
  return parsed_lines.back();
}

  vector<result_parse_line *> parsed_lines;
};

enum asmInstruction
{
  s8i,
  l8ui,
  l16ui,
  l16si,
  s16i,
  l32i,
  s32i,
  lsi,
  ssi,
  add,
  sub,
  quou,
  mull,
  adds,
  subs,
  quous,
  muls,
  neg,
  negs

};

string asmInstructionsName[] =
    {
        "s8i",
        "l8ui",
        "l16ui",
        "l16si",
        "s16i",
        "l32i",
        "s32i",
        "lsi",
        "ssi",
        "add",
        "sub",
        "quou",
        "mull",
        "add.s",
        "sub.s",
        "quou",
        "mul.s",
        "neg",
        "neg.s"

};

typedef struct
{
  string name;
  uint32_t address;
  string variables;
  uint32_t variableaddress;
  int args_num;
} globalcall;

typedef struct
{
  error_message_struct error;
  vector<globalcall> functions;
  uint32_t *start_program = NULL;
  uint8_t *data = NULL;
  uint32_t links;
  uint32_t binary_size;
  uint32_t data_size;
  uint32_t total_size;

} executable;

operandeType op_mov_n[2] = {operandeType::registers, operandeType::registers};
operandeType *op_mov = op_mov_n; //[3] = {operandeType::registers, operandeType::registers};
operandeType op_l8ui[3] = {operandeType::registers, operandeType::registers, operandeType::l0_255};
operandeType *op_s8i = op_l8ui; //[3] = {operandeType::registers, operandeType::registers, operandeType::l0_255};
operandeType op_l16si[3] = {operandeType::registers, operandeType::registers, operandeType::l0_510};
operandeType *op_l16ui = op_l16si; //[3] = {operandeType::registers, operandeType::registers, operandeType::l0_510};
operandeType *op_s16i = op_l16si;  //[3] = {operandeType::registers, operandeType::registers, operandeType::l0_510};
operandeType op_l32i_n[3] = {operandeType::registers, operandeType::registers, operandeType::l0_60};
operandeType op_s32i[3] = {operandeType::registers, operandeType::registers, operandeType::l0_1020};
operandeType *op_s32i_n = op_l32i_n; //[3] = {operandeType::registers, operandeType::registers, operandeType::l0_60};
operandeType *op_l32i = op_s32i;     // [3] = {operandeType::registers, operandeType::registers, operandeType::l0_1020};
operandeType op_lsi[3] = {operandeType::floatregisters, operandeType::registers, operandeType::l0_1020};
operandeType *op_ssi = op_lsi; //[3] = {operandeType::floatregisters, operandeType::registers, operandeType::l0_1020};
operandeType op_rfr[2] = {operandeType::registers, operandeType::floatregisters};
operandeType op_wfr[2] = {operandeType::floatregisters, operandeType::registers};
operandeType op_add[3] = {operandeType::registers, operandeType::registers, operandeType::registers};
operandeType *op_quou = op_add;
operandeType *op_sub = op_add;
operandeType *op_mull = op_add;
operandeType op_adds[3] = {operandeType::floatregisters, operandeType::floatregisters, operandeType::floatregisters};
operandeType *op_quous = op_adds;
operandeType *op_subs = op_adds;
operandeType *op_muls = op_adds;
operandeType *op_neg = op_mov;
operandeType op_nexp01s[2] = {operandeType::floatregisters, operandeType::floatregisters};

operandeType *op_negs = op_nexp01s;
operandeType *asmInstructionOperandes[] =
    {
        op_s8i,
        op_l8ui,
        op_l16ui,
        op_l16si,
        op_s16i,
        op_l32i,
        op_s32i,
        op_lsi,
        op_ssi,
        op_add,  // add
        op_sub,  // sub
        op_quou, // quou
        op_mull, // mull
        op_adds,
        op_subs,
        op_quou,
        op_muls,
        op_neg,
        op_negs,
};
operandeType *__l;
operandeType __op;

string getRegType(asmInstruction instr, int pos)
{
  if (instr >= 19)
  {
    printf("to hight\r\n");
    return " ";
  }

  __l = asmInstructionOperandes[instr];
  __op = __l[pos];

  if (__op == operandeType::registers)
  {
    return "a";
  }
  else if (__op == operandeType::floatregisters)
  {

    return "f";
  }
  else
  {
    return "unknown";
  }
}

#endif