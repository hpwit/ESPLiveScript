#pragma once
#ifndef __EXECUTE_ASM
#define __EXECUTE_ASM
using namespace std;
#ifndef __TEST_DEBUG
#include "FS.h"
#include "esp_heap_caps.h"
#endif
#include <string>
#include <stdlib.h>
#include <memory>
#include <vector>
#include "asm_parser_LMbin.h"
#include "asm_external.h"
using namespace std;
#define ALIGN_INSTR 4
#define ALIGN_DATA 4
class _arguments
{
public:
    _arguments()
    {
        vartype = __unknown__;
    }
    _arguments(int val)
    {
        vartype = __int__;
        intval = val;
    }
    _arguments(float val)
    {
        vartype = __float__;
        floatval = val;
    }

    varTypeEnum vartype;
    int intval;
    float floatval;
};

class Arguments
{
public:
    Arguments() {}
    void add(int val)
    {
        _args.push_back(_arguments(val));
    }
    void add(float val)
    {
        _args.push_back(_arguments(val));
    }
    void clear()
    {
        _args.clear();
        _args.shrink_to_fit();
    }
    void add(_arguments a)
    {
        _args.push_back(a);
    }
    int size()
    {
        return _args.size();
    }
    vector<_arguments> _args;
};
void freeBinary(Binary *bin)
{
  if (bin->binary_data)
    free(bin->binary_data);
  if (bin->function_data)
    free(bin->function_data);
  // delete(bin);
}
void loadBinary(char *name, fs::FS &fs, Binary *bin)
{
  File root = fs.open(name);

  char ver[19];
  root.read((uint8_t *)ver, 19);
  if (strcmp(ver, "ESPLiveScript1.0.1") != 0)
  {
    bin->error.error = 1;
    bin->error.error_message = string_format("wrong format expected ESPLiveScript1.0.1 got %s",(const char *)ver);
    return;
  }

  root.read((uint8_t *)&bin->tmp_instruction_size, 2);
  // printf("instr_iz :%d\n",bin->tmp_instruction_size);

  root.read((uint8_t *)&bin->instruction_size, 2);
  // printf("instr_iz :%d\n",bin->instruction_size);

  root.read((uint8_t *)&bin->data_size, 2);
  // printf("instr_iz :%d\n",bin->data_size);

  root.read((uint8_t *)&bin->function_size, 2);
  // printf("instr_iz :%d\n",bin->function_size);

  uint8_t *tmp = (uint8_t *)malloc(bin->tmp_instruction_size);

  root.read(tmp, bin->tmp_instruction_size);
  bin->binary_data = tmp;
  uint8_t *tmp2 = (uint8_t *)malloc(bin->function_size);
  root.read(tmp2, bin->function_size);
  bin->function_data = tmp2;
  root.close();
}

executable _createExcutablefromBinary(Binary *bin)
{
  executable exe;
  error_message_struct error;
  exe.error.error = 0; 

  printf("on cree un executbale de taiile %d et data %d \n",bin->instruction_size,bin->data_size);
  uint32_t *exec = (uint32_t *)heap_caps_malloc(bin->instruction_size, MALLOC_CAP_EXEC);
  if (binary_data != NULL)
    free(binary_data);
  binary_data = (uint8_t *)malloc(bin->data_size);

  error = decodeBinaryHeader(bin->binary_data, bin->function_data, (uint32_t)binary_data, (uint32_t)exec, &exe, bin->instruction_size);

  if (exe.functions.size() == 0)
  {
    exe.error.error = 1;
    exe.error.error_message = "No global start function found";
    return exe;
  }
  if (error.error == 1)
  {

    exe.error = error;
    _asm_parsed.clear();
    all_text.clear();
    return exe;
  }

  memcpy(exec, bin->binary_data, bin->instruction_size);

  // free(tmp_exec);

  for (int i = 0; i < exe.functions.size(); i++)
  {

    exe.functions[i].address = (uint32_t)((exe.functions[i].address) / 4);
  }
  exe.start_program = exec;
  exe.data = binary_data;
  exe.binary_size = _instr_size;
  exe.data_size = _data_size;

  return exe;
}
executable createExectutable(Binary *bin)
{

  executable exec;
  exec.binary_size = 0;
  exec.data_size = 0;
  // _asm_parsed.clear();

  __parser_debug = false;

  // Binary bin = createBinary(_footer, _header, _content,display);
  exec = _createExcutablefromBinary(bin);

  freeBinary(bin);

  return exec;
}

void executeBinaryAsm(uint32_t *j) //, uint32_t *c)
{
#ifdef __CONSOLE_ESP32
  string s = string_format("Executing asm code @%x", j);
  LedOS.pushToConsole(s, false);
#else
  // printf("Executing asm code ...\r\n");
#endif
  asm volatile( //"l32i a10,%1,0\n\t"
      "l32i a15,%0,0\n\t"
      "callx8 a15"
      : : "r"(j) //, "r"(c)
      :);
#ifdef __CONSOLE_ESP32
  // LedOS.pushToConsole("Execution Done.",true);
#else
  // printf("Execution Done.\n");
#endif
  // free(exec);
}

error_message_struct executeBinary(string function, executable ex, uint32_t handle, void *exePtr, Arguments arguments)
{
  error_message_struct res;
  // uint32_t toexecute;
  res.error = 0;
  for (int i = 0; i < ex.functions.size(); i++)
  {
    string ftofind = ex.functions[i].name;
    if (ex.functions[i].name.find_first_of("(") != string::npos)
    {
      ftofind = ex.functions[i].name.substr(0, ex.functions[i].name.find_first_of("("));
    }

    if (ftofind.compare(function) == 0)
    {
      // printf("address of function %s :%x\n",ex.functions[i].name.c_str(), ex.functions[i].address);

      //
      ex.functions[i].address = (uint32_t)(ex.start_program + ex.functions[i].address);
      uint32_t *t = (uint32_t *)ex.data;
      *t = handle;
      t++;
      *t = (uint32_t)exePtr;

      uint8_t *var = (ex.data + ex.functions[i].variableaddress);
      if (ex.functions[i].args_num == arguments._args.size())
      {
        vector<string> args = split(trim(ex.functions[i].variables), " ");
        for (int i = 1; i < args.size(); i++)
        {
          int _size = 0;
          sscanf(args[i].c_str(), "%d", &_size);
          if (arguments._args[i - 1].vartype == __float__)
            memcpy(var, &arguments._args[i - 1].floatval, _size);
          else
            memcpy(var, &arguments._args[i - 1].intval, _size);
          var += _size;
        }
      }
      else
      {
        res.error = 1;
        res.error_message = string_format("Expected %d arguments got %d\n", ex.functions[i].args_num, arguments.size());
        return res;
      }

      executeBinaryAsm(&ex.functions[i].address); //, &ex.links);

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
  return executeBinary(ex.functions[0].name, ex, handle, NULL, args);
}
void freeExecutable(executable *ex)
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
 // binary_data = NULL;
 // tmp_exec = NULL;
}

#endif