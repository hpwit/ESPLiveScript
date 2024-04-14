#pragma once
#ifndef _ASM_STRUCT
#define _ASM_STRUCT

#include <stdlib.h>
//#include <vector>
//#include <string>
enum  class operandeType
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
    void * ptr;
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

typedef struct 
{
 error_message_struct error;
 int value;
 string label;
 uint32_t opcode;
} result_parse_operande;

typedef struct result_parse_line
{
     error_message_struct  error;
        uint32_t bincode;
        uint32_t size;
        opCodeType op;
        string name;
        uint32_t address;
        bool align;
        string debugtxt;
        int line;
        uint32_t (*calculateOfssetJump)(uint32_t value,uint32_t current_address,uint32_t destination_address)=NULL;

};

typedef struct {
  string name;
  uint32_t address;
} globalcall; 

typedef struct
{
  error_message_struct error;
  vector<globalcall> functions;
  uint32_t *start_program=NULL;
  uint8_t *data=NULL;
  uint32_t links;

} executable;




#endif