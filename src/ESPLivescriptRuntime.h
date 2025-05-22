
#ifndef __ASM_PARSER
#include <string>
#include <vector>
using namespace std;


#include "string_function.h"
 void pushToConsole(string str, bool force)
{
#ifdef __CONSOLE_ESP32
    LedOS.pushToConsole(str, force);
#else

    ESP_LOGD("ESPLiveScript","%s\r\n", str.c_str());

#endif
}

void pushToConsole(string str)
{
    pushToConsole(str, false);
}
uint8_t *binary_data = NULL;
uint8_t *tmp_binary_data = NULL;
uint8_t *tmp_exec = NULL;
uint8_t *binary_header = NULL;
uint32_t _address_data = 0;
uint32_t _tmp_data_address = 0;
uint32_t _address_instr = 0;
uint16_t _instr_size = 0;
uint16_t tmp_instr_size = 0;
uint16_t _data_size = 0;
uint16_t tmp_data_size = 0;
uint16_t binary_header_size = 0;
enum varTypeEnum
{
    __none__,
    __uint8_t__,
    __uint16_t__,
    __uint32_t__,
    __int__,
    __s_int__,
    __float__,
    __void__,
    __CRGB__,
    __CRGBW__,
    __char__,
    __Args__,
    __bool__,
    __userDefined__,
    __unknown__
};
enum class externalType
{
  function,
  value,
};
typedef struct
{
     void *ptr;
  externalType type;
  string name;
  string signature;
  string shortname;
 
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
  uint16_t line;
  uint16_t pos;
};

typedef struct
{
  string name;
  uint32_t address;
  string variables;
  uint32_t variableaddress;
  int args_num;
} globalcall;

typedef struct {
  
    string json;
    uint8_t type;
    uint32_t address;
} jsonVariable;

typedef struct
{
  error_message_struct error;
  vector<globalcall> functions;
  vector<jsonVariable> jsonVars;
  uint32_t *start_program = NULL;
  uint8_t *data = NULL;
  uint32_t links;
  uint32_t binary_size;
  uint32_t data_size;
  uint32_t total_size;

} executable;

typedef struct
{
    error_message_struct error;
    uint8_t *binary_data;
    uint8_t *function_data;
    uint16_t instruction_size;
     uint16_t tmp_instruction_size;
      uint16_t function_size;
      uint16_t data_size;


} Binary;
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

#endif
#include "FS.h"

#include  "asm_external.h"
#include "execute_asm.h"
#include "execute.h"