#pragma once
#ifndef __ASM_PARSER
#define __ASM_PARSER
#include <stdio.h>
using namespace std;
#include <string>
#include <stdlib.h>
#include <memory>
#include <vector>
#include <list>
// #include <iostream>
// #include <functional>
// #include <Arduino.h>
#ifndef __TEST_DEBUG
#include "esp_heap_caps.h"
#endif
#include "string_function.h"
#include "asm_parser_LMbin.h"
#include "asm_external.h"
#define ALIGN_INSTR 4
#define ALIGN_DATA 4

bool __parser_debug = false;

uint8_t *binary_data = NULL;
uint8_t *tmp_exec = NULL;
int _address_data = 0;
int _address_instr = 0;
int _instr_size = 0;
int _data_size=0;

// vector<result_parse_line> _asm_parsed;
parsedLines _asm_parsed;

void addInstr(result_parse_line operande, parsedLines *asm_parsed)
{
  if (operande.op != opCodeType::data && operande.op != opCodeType::number)
  {
    operande.address = _address_instr;

    if (operande.align == true)
    {
      int add_size = 0;
      int op = (_address_instr % ALIGN_INSTR);
      /*
        switch(op)
        {
          case 2:
          add_size=2;
          break;
          case 1:
          add_size=3;
          break;
          case 3:
          add_size=5;
          break;
          default:
          add_size=0;
          break;
        }
        */
      if (op > 0)
        add_size = ALIGN_INSTR - op;
      operande.address += add_size;
    }
    memcpy(tmp_exec + operande.address, &operande.bincode, operande.size);
    // printf("parseASM instr %d\r\n",operande.address);
    _address_instr = operande.size + operande.address;
    if (operande.op != opCodeType::standard)
      asm_parsed->push_back(operande);
  }
  else
  {
    int add_size = 0;
    int op = (_address_data % ALIGN_DATA);
    if (op > 0)
    {
      add_size = ALIGN_DATA - op;
      _address_data += add_size;
    }
    asm_parsed->last()->bincode = _address_data;
    _address_instr += 4;
    memcpy(binary_data + _address_data, operande.getText(), operande.size);
    // printf("parseASM eddr_adta %d\r\n",_address_data);
    _address_data += operande.size;
  }
}

result_parse_line *getInstrAtPos(int pos)
{
  int i = 0;
  if (pos >= _asm_parsed.size() || pos < 0)
  {
    return (*_asm_parsed.end());
  }
  for (vector<result_parse_line *>::iterator it = _asm_parsed.begin(); it != _asm_parsed.end(); it++)
  {
    if (i == pos)
    {

      return *it;
    }
    i++;
  }
  return (*_asm_parsed.end());
}
vector<result_parse_line *>::iterator getIterAtPos(int pos)
{
  int i = 0;
  if (pos >= _asm_parsed.size() || pos < 0)
  {
    return _asm_parsed.end();
  }
  for (vector<result_parse_line *>::iterator it = _asm_parsed.begin(); it != _asm_parsed.end(); it++)
  {
    if (i == pos)
    {

      return it;
    }
    i++;
  }
  return _asm_parsed.end();
}
class opRegister
{
public:
  static result_parse_operande parse(std::string s)
  {
    error_message_struct error;
    result_parse_operande res;
    char *endptr = NULL;
    error.error = 0;
    s = trim(s);
    if (s.at(0) == 'a')
    {

      string s2 = s.substr(1, s.size());
      if (s2.size() > 0)
      {
        int value = strtol(s2.c_str(), &endptr, 10);
        if (*endptr == 0)
        {
          if (value >= 0 and value <= 15)
          {
            res.value = value;
            res.error = error;
            return res;
          }
        }
      }
    }
    error.error_message = string_format("Unknown register %s\n", s.c_str());
    error.error = 1;
    res.error = error;
    return res;
  }
};
class opFloatRegister
{
public:
  static result_parse_operande parse(std::string s)
  {
    error_message_struct error;
    result_parse_operande res;
    char *endptr = NULL;
    error.error = 0;
    s = trim(s);
    if (s.at(0) == 'f')
    {

      string s2 = s.substr(1, s.size());
      if (s2.size() > 0)
      {
        int value = strtol(s2.c_str(), &endptr, 10);
        if (*endptr == 0)
        {
          if (value >= 0 and value <= 15)
          {
            res.value = value;
            res.error = error;
            return res;
          }
        }
      }
    }
    error.error_message = string_format("Unknown register %s\n", s.c_str());
    error.error = 1;
    res.error = error;
    return res;
  }
};
class opBooleanRegister
{
public:
  static result_parse_operande parse(std::string s)
  {
    error_message_struct error;
    result_parse_operande res;
    char *endptr = NULL;
    error.error = 0;
    s = trim(s);
    if (s.at(0) == 'b')
    {

      string s2 = s.substr(1, s.size());
      if (s2.size() > 0)
      {
        int value = strtol(s2.c_str(), &endptr, 10);
        if (*endptr == 0)
        {
          if (value >= 0 and value <= 15)
          {
            res.value = value;
            res.error = error;
            return res;
          }
        }
      }
    }
    error.error_message = string_format("Unknown register %s\n", s.c_str());
    error.error = 1;
    res.error = error;
    return res;
  }
};

class opHex
{
public:
  static result_parse_operande parse(std::string s, uint32_t min, uint32_t max)
  {
    error_message_struct error;
    result_parse_operande res;
    char *endptr = NULL;
    error.error = 0;
    s = trim(s);

    if (s.size() > 0)
    {
      int value = strtol(s.c_str(), &endptr, 10);
      if (*endptr == 0)
      {
        if (value >= min and value <= max)
        {
          res.value = value;
          res.error = error;
          return res;
        }
      }
    }

    error.error_message = string_format("incorrect value %s should be between %d and %d\n", s.c_str(), min, max);
    error.error = 1;
    res.error = error;
    return res;
  }
};

class opNumber
{
public:
  static result_parse_operande parse(std::string s, int min, int max)
  {
    error_message_struct error;
    result_parse_operande res;
    char *endptr = NULL;
    error.error = 0;
    s = trim(s);

    if (s.size() > 0)
    {
      int value = strtol(s.c_str(), &endptr, 10);
      if (*endptr == 0)
      {
        if (value >= min and value <= max)
        {
          res.value = value;
          res.error = error;
          return res;
        }
      }
    }

    error.error_message = string_format("incorrect value %s should be between %d and %d\n", s.c_str(), min, max);
    error.error = 1;
    res.error = error;
    return res;
  }
};

class oplabel
{
public:
  static result_parse_operande parse(std::string s)
  {
    error_message_struct error;
    result_parse_operande res;
    // char *endptr = NULL;
    error.error = 0;
    s = trim(s);

    if (s.size() > 0)
    {
      res.label = s;
      res.error = error;
      return res;
    }

    error.error_message = string_format("label %s is not valid\n", s.c_str());
    error.error = 1;
    res.error = error;
    return res;
  }
};

result_parse_operande operandeParse(string s, operandeType optype)
{

  if (optype == operandeType::registers)
  {
    return opRegister::parse(s);
  }
  if (optype == operandeType::floatregisters)
  {
    return opFloatRegister::parse(s);
  }

  if (optype == operandeType::boolregisters)
  {
    return opBooleanRegister::parse(s);
  }

  if (optype == operandeType::l0_255)
  {
    return opNumber::parse(s, 0, 255);
  }

  if (optype == operandeType::lm32_95)
  {
    return opNumber::parse(s, -32, 95);
  }

  if (optype == operandeType::lm2048_2047)
  {
    return opNumber::parse(s, -2048, 2047);
  }

  if (optype == operandeType::l0_15)
  {
    return opNumber::parse(s, 0, 15);
  }

  if (optype == operandeType::l0_32760)
  {
    return opNumber::parse(s, 0, 32760);
  }

  if (optype == operandeType::l0_240)
  {
    return opNumber::parse(s, 0, 240);
  }

  if (optype == operandeType::l0_31)
  {
    return opNumber::parse(s, 0, 31);
  }

  if (optype == operandeType::lm128_127)
  {
    return opNumber::parse(s, -128, 127);
  }

  if (optype == operandeType::lm32768_32512)
  {
    return opNumber::parse(s, -32768, 35512);
  }

  if (optype == operandeType::l1_31)
  {
    return opNumber::parse(s, 1, 31);
  }

  if (optype == operandeType::l7_22)
  {
    return opNumber::parse(s, 7, 22);
  }

  if (optype == operandeType::l0_60)
  {
    return opNumber::parse(s, 0, 60);
  }

  if (optype == operandeType::lm64_m4)
  {
    return opNumber::parse(s, -64, -4);
  }

  if (optype == operandeType::l0_510)
  {
    return opNumber::parse(s, 0, 510);
  }

  if (optype == operandeType::lm8_7)
  {
    return opNumber::parse(s, -8, 7);
  }

  if (optype == operandeType::l0_1020)
  {
    return opNumber::parse(s, 0, 1020);
  }
  if (optype == operandeType::lm1_15)
  {
    return opNumber::parse(s, -1, 15);
  }
  if (optype == operandeType::l0_FFFFFFFF)
  {
    return opHex::parse(s, 0, 0xFFFFFFFF);
  }

  if (optype == operandeType::label)
  {
    return oplabel::parse(s);
  }
  result_parse_operande res;
  res.error.error = 1;
  res.error.error_message = "Uknown operande";
  return res;
}

result_parse_line parseOperandes(string str, int nboperande, operandeType *optypes, int size, uint32_t (*createbin)(uint32_t *val))
{
  result_parse_line result;

  uint32_t values[4];
  asm_Error.error = 0;
  result.align = false;
  result.size = size;
  vector<string> operandes = split(str, ",");
  if (operandes.size() == nboperande)
  {
    for (int i = 0; i < nboperande; i++)
    {
      result_parse_operande res = operandeParse(operandes[i], optypes[i]);
      if (res.error.error)
      {
        asm_Error.error = res.error.error;
        asm_Error.error_message = asm_Error.error_message + res.error.error_message;
      }
      else
      {
        if (optypes[i] == operandeType::label)
        {
          result.addText(res.label);
        }
        values[i] = res.value;
      }
    }
    if (asm_Error.error == 0)
    {
      if (createbin)
      {
        result.bincode = createbin(values);
        // result.size=size;
      }
    }
  }
  else
  {
    asm_Error.error = 1;
    asm_Error.error_message = string_format("asm_Error:expected %d arguments got %d\n", nboperande, operandes.size());
  }
  result.op = opCodeType::standard;
  return result;
}

int findLabel(string s, parsedLines *asm_parsed)
{
  int res = -1;
  int i = 0;
  for (vector<result_parse_line *>::iterator it = asm_parsed->begin(); it != asm_parsed->end(); it++)
  {
    if ((*it)->op == opCodeType::label || (*it)->op == opCodeType::data_label || (*it)->op == opCodeType::number_label)
    {
      if (trim(string((*it)->getText())).compare(trim(s)) == 0)
      {
        return i;
      }
    }
    i++;
  }
  return res;
}

int findFunction(string s, vector<result_parse_line> *asm_parsed)
{
  int res = -1;
  int i = 0;
  for (vector<result_parse_line>::iterator it = asm_parsed->begin(); it != asm_parsed->end(); it++)
  {
    if ((*it).op == opCodeType::function_declaration)
    {
      // if ((*it).name.compare(s) == 0)
      if (strcmp((*it).getText(), s.c_str()) == 0)
      {
        return i;
      }
    }
    i++;
  }
  return res;
}

result_parse_line parseline(line sp, parsedLines *asm_parsed)
{

  if (sp.opcde.find(":") != -1)
  {
    result_parse_line res;
    asm_Error.error = 0;
    res.op = opCodeType::label;
    res.size = 0;

    res.align = false;
    res.addText(trim(sp.opcde.substr(0, sp.opcde.find(":"))));
    // printf("processing label:%s\n",res.getText());
    if (findLabel(string(res.getText()), asm_parsed) != -1)
    {
      asm_Error.error = 1;
      // res.error.error_message = string_format("label %s is already declared in line %d\n", res.name.c_str(), (*asm_parsed)[findLabel(res.name, asm_parsed)].line);
      asm_Error.error_message = string_format("label %s is already declare\n", res.getText());
    }
    if (sp.opcde.substr(0, 2).compare("@_") == 0)
    {
      res.align = true;
    }
    return res;
  }
  if (sp.opcde.compare("add") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_add, 3, bin_add);
  }
  if (sp.opcde.compare("sub") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_sub, 3, bin_sub);
  }
  if (sp.opcde.compare("quou") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_quou, 3, bin_quou);
  }
  if (sp.opcde.compare("add.n") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_add, 2, bin_add_n);
  }
  if (sp.opcde.compare("addi") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_addi, 3, bin_addi);
  }
  if (sp.opcde.compare("addi.n") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_addi_n, 2, bin_addi_n);
  }
  if (sp.opcde.compare("and") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_and, 3, bin_and);
  }
  if (sp.opcde.compare("or") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_and, 3, bin_mov);
  }
  if (sp.opcde.compare("bnez") == 0)
  {

    result_parse_line ps = parseOperandes(sp.operandes, 2, op_bnez, 3, bin_bnez);
    ps.op = opCodeType::jump;
    ps.calculateOfssetJump = jump_bnez;

    return ps;
  }
  if (sp.opcde.compare("j") == 0)
  {

    result_parse_line ps = parseOperandes(sp.operandes, 1, op_j, 3, bin_j);
    ps.op = opCodeType::jump;
    ps.calculateOfssetJump = jump_j;

    return ps;
  }
 if (sp.opcde.compare("bt") == 0)
  {

    result_parse_line ps = parseOperandes(sp.operandes, 2, op_jumpfloat, 3, bin_bt);
    ps.op = opCodeType::jump;
    ps.calculateOfssetJump = jump_blt;

    return ps;
  }
   if (sp.opcde.compare("bf") == 0)
  {

    result_parse_line ps = parseOperandes(sp.operandes, 2, op_jumpfloat, 3, bin_bf);
    ps.op = opCodeType::jump;
    ps.calculateOfssetJump = jump_blt;

    return ps;
  }
   if (sp.opcde.compare("olt.s") == 0)
  {

    result_parse_line ps = parseOperandes(sp.operandes, 3, op_floatco, 3, bin_olts);
   // ps.op = opCodeType::jump;
   // ps.calculateOfssetJump = jump_bge;

    return ps;
  }
   if (sp.opcde.compare("oeq.s") == 0)
  {

    result_parse_line ps = parseOperandes(sp.operandes, 3, op_floatco, 3, bin_oeqs);
   // ps.op = opCodeType::jump;
   // ps.calculateOfssetJump = jump_bge;

    return ps;
  }
     if (sp.opcde.compare("ole.s") == 0)
  {

    result_parse_line ps = parseOperandes(sp.operandes, 3, op_floatco, 3, bin_oles);
   // ps.op = opCodeType::jump;
   // ps.calculateOfssetJump = jump_bge;

    return ps;
  }

  if (sp.opcde.compare("blt") == 0)
  {

    result_parse_line ps = parseOperandes(sp.operandes, 3, op_blt, 3, bin_blt);
    ps.op = opCodeType::jump;
    ps.calculateOfssetJump = jump_blt;

    return ps;
  }
  if (sp.opcde.compare("bge") == 0)
  {

    result_parse_line ps = parseOperandes(sp.operandes, 3, op_bge, 3, bin_bge);
    ps.op = opCodeType::jump;
    ps.calculateOfssetJump = jump_bge;

    return ps;
  }
  if (sp.opcde.compare("beq") == 0)
  {

    result_parse_line ps = parseOperandes(sp.operandes, 3, op_beq, 3, bin_beq);
    ps.op = opCodeType::jump;
    ps.calculateOfssetJump = jump_beq;

    return ps;
  }
  if (sp.opcde.compare("bne") == 0)
  {

    result_parse_line ps = parseOperandes(sp.operandes, 3, op_bne, 3, bin_bne);
    ps.op = opCodeType::jump;
    ps.calculateOfssetJump = jump_bne;

    return ps;
  }

  if (sp.opcde.compare("l32i") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_l32i, 3, bin_l32i);
  }
  if (sp.opcde.compare("l32i.n") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_l32i_n, 2, bin_l32i_n);
  }
  if (sp.opcde.compare("l32r") == 0)
  {
    result_parse_line ps = parseOperandes(sp.operandes, 2, op_l32r, 3, bin_l32r);
    ps.op = opCodeType::jump_32aligned;
    ps.calculateOfssetJump = jump_l32r;
    return ps;
  }

  if (sp.opcde.compare("call8") == 0)
  {
    result_parse_line ps = parseOperandes(sp.operandes, 1, op_call8, 3, bin_call8);
    ps.op = opCodeType::jump_32aligned;
    ps.calculateOfssetJump = jump_call8;
    return ps;
  }
  if (sp.opcde.compare("call12") == 0)
  {
    result_parse_line ps = parseOperandes(sp.operandes, 1, op_call8, 3, bin_call12);
    ps.op = opCodeType::jump_32aligned;
    ps.calculateOfssetJump = jump_call8;
    return ps;
  }

  if (sp.opcde.compare("rsr") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_rsr, 3, bin_rsr);
  }
  if (sp.opcde.compare("wsr") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_rsr, 3, bin_wsr);
  }
  if (sp.opcde.compare("mov") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_mov, 3, bin_abs);
  }
  if (sp.opcde.compare("abs") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_mov, 3, bin_mov);
  }
  if (sp.opcde.compare("sll") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_mov, 3, bin_sll);
  }
  if (sp.opcde.compare("ssl") == 0)
  {
    return parseOperandes(sp.operandes, 1, op_ssl, 3, bin_ssl);
  }
  if (sp.opcde.compare("srl") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_mov, 3, bin_srl);
  }
  if (sp.opcde.compare("mull") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_add, 3, bin_mull);
  }
  if (sp.opcde.compare("mov.n") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_mov_n, 2, bin_mov_n);
  }
  if (sp.opcde.compare("neg") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_neg, 3, bin_neg);
  }
  if (sp.opcde.compare("movi") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_movi, 3, bin_movi);
  }
  if (sp.opcde.compare("movi.n") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_movi_n, 2, bin_movi_n);
  }
  if (sp.opcde.compare("s32i") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_s32i, 3, bin_s32i);
  }
  if (sp.opcde.compare("s32i.n") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_s32i_n, 2, bin_s32i_n);
  }
  if (sp.opcde.compare("s8i") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_s8i, 3, bin_s8i);
  }
  if (sp.opcde.compare("l8ui") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_l8ui, 3, bin_l8ui);
  }
  if (sp.opcde.compare("s16i") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_s16i, 3, bin_s16i);
  }
  if (sp.opcde.compare("l16si") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_l16si, 3, bin_l16si);
  }
  if (sp.opcde.compare("l16ui") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_l16ui, 3, bin_l16ui);
  }
  if (sp.opcde.compare("slli") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_slli, 3, bin_slli);
  }
  if (sp.opcde.compare("remu") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_remu, 3, bin_remu);
  }
  if (sp.opcde.compare("entry") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_entry, 3, bin_entry);
  }
  if (sp.opcde.compare("nop.n") == 0)
  {
    return parseOperandes(sp.operandes, 0, NULL, 2, bin_nop_n);
  }
  if (sp.opcde.compare("nop") == 0)
  {
    return parseOperandes(sp.operandes, 0, NULL, 3, bin_nop);
  }

  if (sp.opcde.compare("callx8") == 0)
  {
    return parseOperandes(sp.operandes, 1, op_callx8, 3, bin_callx8);
  }
  if (sp.opcde.compare("extui") == 0)
  {
    return parseOperandes(sp.operandes, 4, op_extui, 3, bin_extui);
  }

  if (sp.opcde.compare("retw.n") == 0)
  {
    return parseOperandes(sp.operandes, 0, NULL, 2, bin_retw_n);
  }
  if (sp.opcde.compare(".bytes") == 0)
  {
    char *endptr = NULL;
    // vector<string> sf=split(sp.operandes," ");
    uint16_t value;
    string suite;

    string depart = trim(sp.operandes);
    if (depart.find_first_of(" ") < depart.size())
    {
      suite = depart.substr(depart.find_first_of(" ") + 1, depart.size());
      value = strtol(depart.substr(0, depart.find_first_of(" ")).c_str(), &endptr, 10);
    }
    else
    {
      suite = depart;
      value = strtol(depart.c_str(), &endptr, 10);
    }
    // value = strtol(sp.operandes.c_str(), &endptr, 10);
    //  int value = strtol(sp.operandes.substr(0, sp.operandes.find_first_of(" ")).c_str(), &endptr, 10);
    //  printf("first of %d\n",sp.operandes.find_first_of(" "));
    //  printf("on a /%s/%s/%d\n",depart.c_str(), suite.c_str(),value);

    if (*endptr == 0)
    {
      result_parse_line ps;
      ps.op = opCodeType::data;
      result_parse_line *ps1 = getInstrAtPos((*asm_parsed).size() - 1); //&(*asm_parsed)[(*asm_parsed).size() - 1];
      if (ps1->op == opCodeType::label)
      {
        ps1->size = 4;
        ps1->op = opCodeType::data_label;
        ps1->align = true;
        asm_Error.error = 0;
        ps.size = value;
        vector<string> sf = split(suite, " ");
        string name = "";
        for (int i = 0; i < sf.size(); i++)
        {
          unsigned int __num = 0;
          sscanf(sf[i].c_str(), "%x", &__num);
          // printf("%s \r\n",sf[i].c_str());
          name = name + (char)__num;
        }
        if (sf.size() > 0)
          ps.addText(name, value);
        sf.clear();
        return ps;
      }
      else
      {
        // result_parse_line ps;
        asm_Error.error = 1;
        asm_Error.error_message = "Prior instruction is not a label";
        return ps;
      }
    }
    else
    {
      result_parse_line ps;
      asm_Error.error = 1;
      asm_Error.error_message = "Not Valid size for bytes";
      return ps;
    }
  }
  if (sp.opcde.compare(".word") == 0)
  {
    result_parse_line ps;
    ps.op = opCodeType::number;
    result_parse_line *ps1 = getInstrAtPos((*asm_parsed).size() - 1); // &(*asm_parsed)[(*asm_parsed).size() - 1];
    if (ps1->op == opCodeType::label)
    {
      ps1->size = 4;
      ps1->op = opCodeType::number_label;
      ps1->align = true;
      asm_Error.error = 0;
      ps.size = 4;
      ps.addText(sp.operandes);
      // printf(".woprd %s\n\r", ps.name.c_str());
      //  ps.name = ps.name + '\n' + '\0';
    }
    return ps;
    // return parseOperandes(sp.operandes, 1, op_word, 4, bin_word);
  }

  if (sp.opcde.compare(".align") == 0)
  {
    result_parse_line ps;
    asm_Error.error = 0;
    ps.align = false;
    ps.op = opCodeType::not_known;
    ps.size = 0;
    return ps;
  }

  if (sp.opcde.compare(".global") == 0)
  {
    result_parse_line ps;
    ps = parseOperandes(sp.operandes, 1, op_global, 0, NULL);
    // printf("operandes %s\r\n",ps.getText());
    ps.op = opCodeType::function_declaration;
    return ps;
  }
  if (sp.opcde.compare(".var") == 0)
  {
    result_parse_line ps;
    ps = parseOperandes(sp.operandes, 1, op_global, 0, NULL);
    // printf("operandes %s\r\n",ps.getText());
    ps.op = opCodeType::variable;
    return ps;
  }
  if (sp.opcde.compare("callExt") == 0)
  {
    result_parse_line ps;
    ps = parseOperandes(sp.operandes, 2, op_movExt, 0, bin_movExt);
    if (asm_Error.error != 0)
    {
      return ps;
    }
    int i = findLink(string(ps.getText()), externalType::function);
    if (i == -1)
    {
      asm_Error.error = 1;
      asm_Error.error_message = string_format("External function %s not found\n", ps.getText());
    }
    else
    {
      // string debugsav=ps.debugtxt;
      int savbin = ps.bincode;
      ps = parseOperandes(string_format("a%d,a2,%d", ps.bincode, i * 4), 3, op_l32i, 3, bin_l32i);
      // ps.debugtxt = "call ext function";
      // ps.op = opCodeType::external_call;
      //(*asm_parsed).push_back(ps);
      addInstr(ps, asm_parsed);
      ps = parseOperandes(string_format("a%d", savbin), 1, op_callx8, 3, bin_callx8);
      // ps.debugtxt=debugsav;
      // ps.op = opCodeType::external_call;
    }
    // ps.op=opCodeType::external_call;
    return ps;
  }

  if (sp.opcde.compare("movExt") == 0)
  {
    result_parse_line ps;
    ps = parseOperandes(sp.operandes, 2, op_movExt, 0, bin_movExt);
    // ps.op=opCodeType::external_call;
    int i = findLink(string(ps.getText()), externalType::value);
    if (i == -1)
    {
      asm_Error.error = 1;
      asm_Error.error_message = string_format("External variable %s not found\n", ps.getText());
    }
    else
    {
      // string debugsav=ps.debugtxt;
      ps = parseOperandes(string_format("a%d,a2,%d", ps.bincode, i * 4), 3, op_l32i, 3, bin_l32i);
      // ps.debugtxt=debugsav;
      // ps.op = opCodeType::external_var;
    }
    return ps;
  }

  if (sp.opcde.compare(".text") == 0)
  {
    result_parse_line ps;
    sp.operandes = trim(sp.operandes);

    if (sp.operandes.size() > 0)
    {

      ps.op = opCodeType::data;
      result_parse_line *ps1 = getInstrAtPos((*asm_parsed).size() - 1); //&(*asm_parsed)[(*asm_parsed).size() - 1];
      if (ps1->op == opCodeType::label)
      {
        ps1->size = 4;
        ps1->op = opCodeType::data_label;
        ps1->align = true;
        asm_Error.error = 0;
        ps.size = sp.operandes.size() + 2;
        // ps.name = sp.operandes;
        ps.addText(sp.operandes + '\n' + '\0');
      }
    }
    else
    {
      asm_Error.error = 1;
      asm_Error.error_message = "no valuable string";
    }
    return ps;
  }
  if (sp.opcde.compare(".type") == 0)
  {
    result_parse_line ps;
    ps.op = opCodeType::not_known;
    asm_Error.error = 0;
    ps.align = false;
    ps.size = 0;
    return ps;
  }

  /************float operator********/
  if (sp.opcde.compare("lsi") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_lsi, 3, bin_lsi);
  }
  if (sp.opcde.compare("ssi") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_ssi, 3, bin_ssi);
  }
  if (sp.opcde.compare("rfr") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_rfr, 3, bin_rfr);
  }
  if (sp.opcde.compare("wfr") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_wfr, 3, bin_wfr);
  }

  if (sp.opcde.compare("add.s") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_adds, 3, bin_adds);
  }

  if (sp.opcde.compare("sub.s") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_subs, 3, bin_subs);
  }
  if (sp.opcde.compare("float.s") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_floats, 3, bin_floats);
  }
  if (sp.opcde.compare("floor.s") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_floors, 3, bin_floors);
  }
  if (sp.opcde.compare("mul.s") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_muls, 3, bin_muls);
  }
  if (sp.opcde.compare("trunc.s") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_truncs, 3, bin_truncs);
  }
  if (sp.opcde.compare("round.s") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_truncs, 3, bin_rounds);
  }

  if (sp.opcde.compare("div0.s") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_div0s, 3, bin_div0s);
  }
  if (sp.opcde.compare("divn.s") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_divns, 3, bin_divns);
  }
  if (sp.opcde.compare("const.s") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_consts, 3, bin_consts);
  }

  if (sp.opcde.compare("mov.s") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_movs, 3, bin_movs);
  }
  if (sp.opcde.compare("abs.s") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_movs, 3, bin_abss);
  }
  if (sp.opcde.compare("maddn.s") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_maddns, 3, bin_maddns);
  }
  if (sp.opcde.compare("nexp01.s") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_nexp01s, 3, bin_nexp01s);
  }
  if (sp.opcde.compare("neg.s") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_negs, 3, bin_negs);
  }
  if (sp.opcde.compare("addexpm.s") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_addexpms, 3, bin_addexpms);
  }
  if (sp.opcde.compare("addexp.s") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_addexps, 3, bin_addexps);
  }
  if (sp.opcde.compare("mkdadj.s") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_mkdadjs, 3, bin_mkdadjs);
  }
  result_parse_line res;
  error_message_struct err;
  err.error = 1;
  err.error_message = string_format("Opcode %s not found", sp.opcde.c_str());
  asm_Error = err;
  return res;
}

line splitOpcodeOperande(string s)
{
  line res;
  s = trim(s);

  if (s.size() < 2)
  {
    res.error = 1;
    return res;
  }
  res.error = 0;
  // string s=trim(s2);
  string opcode = trim(s.substr(0, s.find_first_of(" ")));
  string operandes;
  if (s.find_first_of(" ") < s.size())
  {
    operandes = s.substr(s.find_first_of(" "), s.size());
  }
  else
  {
    operandes = "";
  }
  res.opcde = opcode;
  res.operandes = operandes;
  return res;
}

error_message_struct parseASM(Text *_header, Text *_content, parsedLines *asm_parsed)
{
  // list<string> lines = *_lines;
  error_message_struct main_error;
  main_error.error = 0;
  main_error.error_message = "";
  optimize(_content);
  //_content->display();
  // printf("optimized done\r\n");

  tmp_exec = NULL;
  binary_data = NULL;
  _address_data = 0;
  _address_instr = 0;
  _instr_size = 0;
  _data_size =0;
  int _nb_data = 0;
  int _size = 0;
  for (int i = 0; i < _header->size(); i++)
  {
    string str = _header->textAt(i);
    if (str.compare(0, 6, ".bytes") == 0)
    {
      int h = 0;
      _nb_data++;
      vector<string> v = split(str, " ");
      sscanf(v[1].c_str(), "%d", &h);
      _size += h;
    }
  }
  // printf("taille %d\r\n",_size);
  int _nb_align_label = 0;
  int _nb_not_aligned_label = 0;
  for (int i = 0; i < _content->size(); i++)
  {
    string str = _header->textAt(i);
    if (str.compare(0, 2, "@_") == 0)
    {
      _nb_align_label++;
    }
    else
    {
      if (str.find(":") != -1)
      {
        _nb_not_aligned_label++;
      }
    }
  }

  _instr_size = (_nb_align_label + 1) * ALIGN_INSTR + (_content->size() - _nb_not_aligned_label) * 3;
  // printf("taille instr %d\r\n",_instr_size);

  _instr_size = (_instr_size / 8) * 8 + 8;
  pushToConsole("Creation of an %d bytes binary and %d bytes data\r\n", _instr_size, _size);

  tmp_exec = (uint8_t *)malloc(_instr_size);
  if (_size > 0)
  {
    _size += ALIGN_DATA * _nb_data;
    binary_data = (uint8_t *)malloc((_size / 4) * 4 + 4);
    _data_size=(_size / 4) * 4 + 4;
  }
  // printf("her:\r\n");
  pushToConsole("Parsing %d assembly lines ...\r\n ", _header->size() + _content->size());

  int size = _header->size();
  int tmp_size = size;
  for (int i = 0; i < size; i++)
  {

    if (__parser_debug)
    {
      printf("on parse line: %d : %s\r\n", i, _header->front().c_str());
    }
    //  printf("on parse line: %d : %s\r\n",i,_header->front().c_str());
    line res = splitOpcodeOperande(_header->front());
    if (!res.error)
    {
      result_parse_line re_sparse = parseline(res, asm_parsed);
      if (__parser_debug)
      {
        // re_sparse.debugtxt = _lines->front();
      }
      re_sparse.line = i + 1;
      // printf("%d %s %d\r\n",i+1,_lines->front().c_str(),sizeof(re_sparse));

      if (asm_Error.error)
      {
        main_error.error = 1;
        main_error.error_message += string_format("line:%d %s\r\n", i, asm_Error.error_message.c_str());
      }
      else
      {
        // printf("befoire line:%d mem:%u\r\n",i, esp_get_free_heap_size());
        // asm_parsed->push_back(re_sparse);
        addInstr(re_sparse, asm_parsed);
        updateMem();
        // printf("afetr line:%d mem:%u\r\n",i, esp_get_free_heap_size());
      }
    }
    _header->pop_front();
  }

  size = _content->size();
  for (int i = tmp_size; i < size + tmp_size; i++)
  {
    if (__parser_debug)
    {
      printf("on parse line: %d : %s\r\n", i, _content->front().c_str());
    }
    if (_content->front().compare(" ") != 0)
    {
      // printf("on parse line: %d : %s\r\n",i,_lines->front().c_str());
      line res = splitOpcodeOperande(_content->front());
      if (!res.error)
      {
        result_parse_line re_sparse = parseline(res, asm_parsed);
        if (__parser_debug)
        {
          // re_sparse.debugtxt = _lines->front();
        }
        //  printf("on parse line: %d : %s\r\n",i,_content->front().c_str());
        re_sparse.line = i + 1;
        // printf("%d %s %d\r\n",i+1,_lines->front().c_str(),sizeof(re_sparse));

        if (asm_Error.error)
        {
          main_error.error = 1;
          main_error.error_message += string_format("line:%d %s\r\n", i, asm_Error.error_message.c_str());
        }
        else
        {
          // printf("befoire line:%d mem:%u\r\n",i, esp_get_free_heap_size());
          // asm_parsed->push_back(re_sparse);
          addInstr(re_sparse, asm_parsed);
          updateMem();
          // printf("afetr line:%d mem:%u\r\n",i, esp_get_free_heap_size());
        }
      }
    }
    _content->pop_front();
  }

  // printf("Done.\r\n");
  if (main_error.error == 1)
  {
    free(binary_data);
    free(tmp_exec);
  }
  return main_error;
}

/*
error_message_struct parseASM(string s, vector<result_parse_line> *asm_parsed)
{
  vector<string> lines = split(s, "\n");
  return parseASM(&lines, asm_parsed);
}
*/
void createAddress(parsedLines *asm_parsed)
{
  uint32_t add_instr = 0;
  uint32_t add_data = 0;
  // printf("create address\r\n");
  // vector<result_parse_line>::iterator it = (*asm_parsed).begin();
  // int i = 0;
  for (vector<result_parse_line *>::iterator it = asm_parsed->begin(); it != asm_parsed->end(); it++)
  {

    if ((*it)->op != opCodeType::data && (*it)->op != opCodeType::number)
    {

      // printf("%s %s\n\r",it->debugtxt.c_str(), it->name.c_str());
      // result_parse_line  *re_sparse=&asmParsed[i];

      (*it)->address = add_instr;
      if ((*it)->align == true)
      {
        // printf("%s %s %d\n\r",it->debugtxt.c_str(), it->name.c_str(),add_instr & 3);
        if ((add_instr & 3) != 0)
        {
          result_parse_line add_op;

          if ((add_instr & 3) == 2)
          {
            add_op.size = 2;
            add_op.bincode = bin_nop_n(0);
          }
          if ((add_instr & 3) == 1)
          {
            add_op.size = 3;
            add_op.bincode = bin_nop(0);
          }
          if ((add_instr & 3) == 3)
          {
            add_op.size = 5;
            add_op.bincode = bin_nop(0);
          }
          add_op.address = add_instr;
          // add_op.debugtxt = string_format("nop filler %d", add_op.size);

          asm_Error.error = 0;
          (*it)->address = add_instr + add_op.size;
          // printf("function not aligned\t %s \t %s\n", it->debugtxt.c_str(), it->name.c_str());
          // printf("one insert \n");
          it = asm_parsed->insert(it, add_op);
          // printf("apres insert \n");
          it++;
          add_instr = add_instr + add_op.size;
          // add_instr += 4 - (add_instr & 3);
        }
      }
      add_instr += (*it)->size;
    }
    else
    {
      // we suppose that above it's the label
      (*it)->address = add_data;
      vector<result_parse_line *>::iterator prev_it = it--;
      it++;
      (*prev_it)->bincode = add_data;
      if (it != asm_parsed->end())
        add_data += (*it)->size;
    }
  }
  printf("Done\r\n");
}
void dumpmem(uint32_t *dump)
{
  /*
  printf("\n%s", "                   ");
  for (int i = 0; i < 8; i++)
  {
    printf("-----");
  }
  printf("\n%s", "                   ");
  for (int i = 0; i < 8; i++)
  {
    printf(" %02x |", i);
  }
  printf("\n");
  printf("%s", "                   ");
  for (int i = 0; i < 8; i++)
  {
    printf("-----");
  }
  for (int j = 0; j < 30; j++)
  {
    printf("\nline:%2d %8x: ", j, dump);
    for (int i = 0; i < 2; i++)
    {
      printf(" %02x | %02x | %02x | %02x |", ((*dump)) & 0xFF, ((*dump) >> 8) & 0xFF, ((*dump) >> 16) & 0xFF, ((*dump) >> 24) & 0xFF);
      dump++;
    }
  }
  printf("\n\n\n");
  */
}

void printparsdAsm(uint32_t start_address, parsedLines *asm_parsed)
{
  for (vector<result_parse_line *>::iterator it = asm_parsed->begin(); it != asm_parsed->end(); it++)
  {
    result_parse_line re_sparse = *(*it);
    if (asm_Error.error)
    {
      // printf("%8x \t %s\t %s\n", re_sparse.address + start_address, re_sparse.debugtxt.c_str(), re_sparse.error.error_message.c_str());
    }
    else
    {
      if (re_sparse.op == opCodeType::label)
      {
        printf("%8x \t <%s>:\n", (unsigned int)(re_sparse.address + start_address), re_sparse.getText());
      }
      else
      {
        // printf("%8x \t %6x\t %s\n", re_sparse.address + start_address, re_sparse.bincode, re_sparse.debugtxt.c_str());
      }
    }
  }
}

void flagLabel32aligned(parsedLines *asm_parsed)
{
  // return;
  pushToConsole("Flag label(s) to align ...\r\n");
  // uint32_t add = 0;
  // vector<result_parse_line>::iterator it = (*asm_parsed).begin();
  for (vector<result_parse_line *>::iterator it = asm_parsed->begin(); it != asm_parsed->end(); it++)
  {
    if ((*it)->op == opCodeType::jump_32aligned || (*it)->op == opCodeType::function_declaration)
    {
      int i = findLabel(string((*it)->getText()), asm_parsed);
      if (i != -1)
      {
        result_parse_line *parse_line = getInstrAtPos(i); // &(*asm_parsed)[i];
        parse_line->align = true;
      }
    }
  }
  pushToConsole("Done.\r\n");
}

error_message_struct calculateJump(parsedLines *asm_parsed)
{

  pushToConsole("Calculating jumps ...\r\n");

  error_message_struct error;
  error.error = 0;
  error.error_message = "";

  // return error;
  for (vector<result_parse_line *>::iterator it = asm_parsed->begin(); it != asm_parsed->end(); it++)
  {
    result_parse_line *parse_line = *it;
    if ((parse_line->op == opCodeType::jump) || (parse_line->op == opCodeType::jump_32aligned))
    {
      int index = findLabel(string(parse_line->getText()), asm_parsed);
      if (index != -1)
      {
        if (parse_line->calculateOfssetJump)
        {
          parse_line->bincode = parse_line->calculateOfssetJump(parse_line->bincode, parse_line->address, getInstrAtPos(index)->address);
        }
        else
        {
          error.error = 1;
          // error.error_message += string_format("asm_Error:No method to calcuylate jump offset for %s\n", parse_line->debugtxt.c_str());
        }
      }
      else
      {
        error.error = 1;
        error.error_message += string_format("line : %d label %s not found\n", parse_line->line, parse_line->getText());
      }
    }
  }
  // printf("Done.\r\n");
  return error;
}
error_message_struct calculateJump(uint8_t *exec, parsedLines *asm_parsed)
{

  pushToConsole("Calculating jumps2...\r\n");

  error_message_struct error;
  error.error = 0;
  error.error_message = "";
  for (vector<result_parse_line *>::iterator it = asm_parsed->begin(); it != asm_parsed->end(); it++)
  {
    result_parse_line *parse_line = *it;
    if ((parse_line->op == opCodeType::jump) || (parse_line->op == opCodeType::jump_32aligned))
    {
      int index = findLabel(string(parse_line->getText()), asm_parsed);
      if (index != -1)
      {
        if (parse_line->calculateOfssetJump)
        {
          parse_line->bincode = parse_line->calculateOfssetJump(parse_line->bincode, parse_line->address, getInstrAtPos(index)->address);
          // printf("line:%d %d\r\n",parse_line->address,parse_line->size);
          memcpy(exec + parse_line->address, &parse_line->bincode, parse_line->size);
        }
        else
        {
          error.error = 1;
          // error.error_message += string_format("asm_Error:No method to calcuylate jump offset for %s\n", parse_line->debugtxt.c_str());
        }
      }
      else
      {
        error.error = 1;
        printf("b ou foune\r\n");
        error.error_message += string_format("line : %d label %s not found\n", parse_line->line, parse_line->getText());
      }
    }
  }
  // printf("Done.\r\n");
  return error;
}

void createAbsoluteJump(uint8_t *exec, parsedLines *asm_parsed, uint32_t address)
{
  int nb_data = 0;
  for (vector<result_parse_line *>::iterator it = asm_parsed->begin(); it != asm_parsed->end(); it++)
  {
    if ((*it)->op == opCodeType::data_label || (*it)->op == opCodeType::number_label)
    {
      //  it++;
      // uint32_t content = (*(it))->address + address;
      uint32_t content = (*it)->bincode + address;
      //  it--; // à cause de l'"it++"
      // printf("on veut mapper address:%s %ld\r\n",(*it)->getText(),(*it)->bincode);
      // (*it)->address = content;
      // printf("absoluit line:%d\r\n",nb_data*4);
      uint32_t *new_adr = (uint32_t *)exec + nb_data; // (*asm_parsed)[i].address / 4;
      // printf("new content %x atr adress %x\n",content,(uint32_t)new_adr);
      nb_data++;
      memcpy(new_adr, &content, 4);
    }
  }
}

#ifndef __TEST_DEBUG

executable createBinary(parsedLines *asm_parsed)
{

  executable exe;
  exe.error.error = 0;

  for (vector<result_parse_line *>::iterator it = asm_parsed->begin(); it != asm_parsed->end(); it++)
  {
    if ((*it)->op == opCodeType::function_declaration)
    {
      // printf("look for %s\n",(*it)->getText());
      if (*(*it)->getText() == '\0')
        continue;
      int index = findLabel(string((*it)->getText()), asm_parsed);
      if (index == -1)
      {
        exe.error.error = 1;
        exe.error.error_message = string_format("Impossible to find start function :%s", (*it)->getText());
        return exe;
      }
      else
      {
        globalcall gc;
        vector<result_parse_line *>::iterator it2 = it;
        it2++;
        if ((*it2)->op == opCodeType::variable)
        {
          string stackstring = string((*it)->getText()).insert(2, "stack_");
          // printf(" qss %s %ld  %s\r\n",stackstring.c_str(),getInstrAtPos(findLabel(
          //  stackstring, asm_parsed))->bincode,(*it2)->getText());
          gc.variableaddress = getInstrAtPos(findLabel( stackstring, asm_parsed))->bincode;
        }
        vector<string> args = split(trim(string((*it2)->getText())), " ");
        int num = 0;
        sscanf(args[0].c_str(), "%d", &num);
        gc.name = string((*it)->getText());
        gc.args_num = num;

        gc.variables = string((*it2)->getText());
        gc.address = getInstrAtPos(index)->address; //(*asm_parsed)[index].address;

        exe.functions.push_back(gc);
      }
    }
  }

  if (exe.functions.size() == 0)
  {
    exe.error.error = 1;
    exe.error.error_message = "No global start function found";
    return exe;
  }
  // int data_size = 0;
  // int last_one = -1;
  // int nb_data = 0;
  // int i = 0;

  uint32_t *exec = (uint32_t *)heap_caps_malloc(_instr_size, MALLOC_CAP_EXEC);
  updateMem();

  error_message_struct error = calculateJump(tmp_exec, asm_parsed);
  if (error.error == 1)
  {

    exe.error = error;
    _asm_parsed.clear();
    all_text.clear();
    return exe;
  }

  createAbsoluteJump(tmp_exec, asm_parsed, (uint32_t)binary_data);

  memcpy(exec, tmp_exec, _instr_size);

  free(tmp_exec);

  for (int i = 0; i < exe.functions.size(); i++)
  {

    exe.functions[i].address = (uint32_t)((exe.functions[i].address) / 4);
  }
  exe.start_program = exec;
  exe.data = binary_data;
  exe.binary_size=_instr_size;
  exe.data_size=_data_size;

  return exe;
}

executable createExectutable(Text *_header, Text *_content, bool display)
{

  executable exec;
  _asm_parsed.clear();

  __parser_debug = display;
  error_message_struct err = parseASM(_header, _content, &_asm_parsed);

  _header->clear();
  _content->clear();

  if (err.error == 0)
  {

    exec = createBinary(&_asm_parsed);

    if (exec.error.error == 0)
    {

      if (display == true)
      {
        printparsdAsm((uint32_t)exec.start_program, &_asm_parsed);
      }

      replaceExternal("start_program", (void *)exec.start_program);
      uint32_t dd = (uint32_t)createExternalLinks();
      exec.links = dd;
      exec.error.error = 0;
    }
  }
  else
  {

    exec.error = err;
    exec.error.error = 1;
  }
  _asm_parsed.clear();
  all_text.clear();
  return exec;
}

void executeBinaryAsm(uint32_t *j, uint32_t *c)
{
  pushToConsole("Executing asm code ...\r\n");

  asm volatile("l32i a10,%1,0\n\t"
               "l32i a15,%0,0\n\t"
               "callx8 a15"
               : : "r"(j), "r"(c)
               :);

  // pushToConsoleForce("Execution Done.\r\n");
  // free(exec);
}

error_message_struct executeBinary(string function, executable ex, uint32_t handle, Arguments arguments)
{
  error_message_struct res;
  // uint32_t toexecute;
  res.error = 0;
  for (int i = 0; i < ex.functions.size(); i++)
  {
    if (ex.functions[i].name.compare(function) == 0)
    {
      // printf("address of function %s :%x\n",ex.functions[i].name.c_str(), ex.functions[i].address);

      //
      ex.functions[i].address = (uint32_t)(ex.start_program + ex.functions[i].address);
      uint32_t *t = (uint32_t *)ex.data;
      *t = handle;
      uint8_t *var = (ex.data + ex.functions[i].variableaddress);
      if (ex.functions[i].args_num == arguments._args.size())
      {
        vector<string> args = split(trim(ex.functions[i].variables), " ");
        for (int i = 1; i < args.size(); i++)
        {
          int _size = 0;
          sscanf(args[i].c_str(), "%d", &_size);
          if(arguments._args[i-1].vartype==__float__)
               memcpy(var, &arguments._args[i - 1].floatval, _size);
          else
           memcpy(var,  &arguments._args[i - 1].intval, _size);
          var += _size;
        }
      }
      else
      {
        res.error = 1;
        res.error_message = string_format("Expected %d arguments got %d\n", ex.functions[i].args_num, arguments.size());
        return res;
      }

      executeBinaryAsm(&ex.functions[i].address, &ex.links);

      // printf("address of function %s :%x\n",ex.functions[i].name.c_str(), toexecute);
      //  executeBinaryAsm(&toexecute, &ex.links);

      // freeBinary(ex);
      return res;
    }
  }
  res.error = 1;
  res.error_message = string_format("Impossible to execute %s: not found\n", function.c_str());
  return res;
}

error_message_struct executeBinary(executable ex, uint32_t handle)
{
  Arguments args;
  return executeBinary(ex.functions[0].name, ex, handle, args);
}
void freeBinary(executable *ex)
{
  if (ex->start_program != NULL)
  {

    heap_caps_free(ex->start_program);
  }

  ex->start_program = NULL;

  if (ex->data != NULL)
  {

    heap_caps_free(ex->data);
  }
  ex->data = NULL;
  binary_data = NULL;
  tmp_exec = NULL;
}
#endif
#endif