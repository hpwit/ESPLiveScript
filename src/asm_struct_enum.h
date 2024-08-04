#pragma once
#ifndef _ASM_STRUCT
#define _ASM_STRUCT

#include <stdlib.h>
#include <vector>
#include <string>

#include "functionlib.h"
using namespace std;
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

typedef struct result_parse_line
{
  // error_message_struct  error;
  uint32_t bincode;
  uint32_t size;
  opCodeType op;
  string name;
  uint32_t address;
  bool align;
  // string debugtxt;
  int line;
  uint32_t (*calculateOfssetJump)(uint32_t value, uint32_t current_address, uint32_t destination_address) = NULL;
};

class parsedLines
{

  public:
  parsedLines();

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
  
  void clear()
  {
    for(int i=0;i<parsed_lines.size();i++)
    {
      free(parsed_lines[i]);
    }
    parsed_lines.clear();
    parsed_lines.shrink_to_fit();
  }

 vector<result_parse_line *>::iterator getIterAtPos(int pos)
{
  int i = 0;
  if (pos >= parsed_lines.size() || pos < 0)
  {
    return parsed_lines.end();
  }
  for (vector<result_parse_line*>::iterator it = parsed_lines.begin(); it != parsed_lines.end(); it++)
  {
    if (i == pos)
    {

      return it;
    }
    i++;
  }
  return parsed_lines.end();
}
result_parse_line *getInstrAtPos(int pos)
{
  int i = 0;
  if (pos >= parsed_lines.size() || pos < 0)
  {
    return *parsed_lines.end();
  }
  for (vector<result_parse_line *>::iterator it = parsed_lines.begin(); it != parsed_lines.end(); it++)
  {
    if (i == pos)
    {

      return *it;
    }
    i++;
  }
  return *parsed_lines.end();
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
} globalcall;

typedef struct
{
  error_message_struct error;
  vector<globalcall> functions;
  uint32_t *start_program = NULL;
  uint8_t *data = NULL;
  uint32_t links;

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