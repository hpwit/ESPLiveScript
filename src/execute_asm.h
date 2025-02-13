#pragma once
#ifndef __EXECUTE_ASM
#define __EXECUTE_ASM


void freeBinary(Binary *bin)
{
  if (bin->binary_data)
    heap_caps_free(bin->binary_data);
  if (bin->function_data)
    heap_caps_free(bin->function_data);
    bin->binary_data=NULL;
    bin->function_data=NULL;
  // delete(bin);
}



error_message_struct decodeBinaryHeader(uint8_t *exec, uint8_t *binary_header, uint8_t *_binary_data, uint32_t _exec, executable *finalexe, int offset)
{
  error_message_struct error;
  uint16_t nb_data = 0;
  uint16_t tmp_data = 0;
  uint16_t nb_objects = 0;
  uint8_t type;
  uint16_t text_size;
  uint32_t address;

  uint32_t bincode;
  uint32_t _address;
  uint16_t size;

  char *textptr;
  char *textptr2;
#ifndef __TEST_DEBUG
  address = (uint32_t)_binary_data;
#endif
  memcpy(&nb_objects, binary_header, 2);
  binary_header = binary_header + 2;
  // printf("nb objecst %u\n", nb_objects);
  for (int i = 0; i < nb_objects; i++)
  {
    memcpy(&type, binary_header, 1);
    binary_header++;
    // printf("type %d\r\n", type);
    switch (type)
    {
    case 0:
    {
      memcpy(&bincode, binary_header, 4);
      binary_header = binary_header + 4;
      memcpy(&nb_data, binary_header, 2);
      binary_header = binary_header + 2;
      uint32_t content = bincode + address;

      uint32_t *new_adr = (uint32_t *)exec + nb_data;
      memcpy(new_adr, &content, 4);
    }
    break;
    case 1:
    {
      memcpy(&text_size, binary_header, 2);
      binary_header = binary_header + 2;
      textptr = (char *)binary_header;
      binary_header = binary_header + text_size;
      memcpy(&nb_data, binary_header, 2);
      binary_header = binary_header + 2;
      int index = findLink(string(textptr).substr(6, 100), externalType::value);
      if (index > -1)
      {
#ifndef __TEST_DEBUG
        uint32_t content = (uint32_t)((external_links[index].ptr));

        uint32_t *new_adr = (uint32_t *)exec + nb_data;
        memcpy(new_adr, &content, 4);
#endif
        // printf("external var:%s\n\r", textptr);
      }
      else
      {
        error.error = 1;
        // printf("b ou foune\r\n");
        error.error_message += string_format("External variable %s not found\n\r", textptr);
      }
    }
    break;
    case 2:
    {
      memcpy(&text_size, binary_header, 2);
      binary_header = binary_header + 2;
      textptr = (char *)binary_header;
      binary_header = binary_header + text_size;
      // memcpy(&bincode, binary_header, 4);
      // binary_header = binary_header + 4;
      // memcpy(&_address, binary_header, 4);
      // binary_header = binary_header + 4;
      memcpy(&nb_data, binary_header, 2);
      binary_header = binary_header + 2;
      int index = findLink(string(textptr).substr(2, 100), externalType::function);
      if (index > -1)
      {
#ifndef __TEST_DEBUG
        // printf("calculate ext %s\n\r", (*it)->getText());
        /*
        bincode = jump_call8(bincode, _address + (uint32_t)_exec, (uint32_t)external_links[index].ptr);
        memcpy(exec + _address, &bincode, 3);
        */

        uint32_t content = (uint32_t)((external_links[index].ptr));

        uint32_t *new_adr = (uint32_t *)exec + nb_data;
        memcpy(new_adr, &content, 4);
#endif
        // printf("external func:%s\n\r", textptr);
      }
      else
      {
        error.error = 1;
        // printf("b ou foune\r\n");
        error.error_message += string_format("external function %s not found\n\r", textptr);
      }
    }
    break;
    case 3:
    {
      memcpy(&_address, binary_header, 4);
      binary_header = binary_header + 4;
      memcpy(&tmp_data, binary_header, 2);
      binary_header = binary_header + 2;
      memcpy(&size, binary_header, 2);
      binary_header = binary_header + 2;
      memcpy(_binary_data + _address, exec + offset + tmp_data, size);
    }
    break;

    case 4:
    {
      globalcall gc;
      memcpy(&text_size, binary_header, 2);
      binary_header = binary_header + 2;
      textptr = (char *)binary_header;
      binary_header = binary_header + text_size;
      gc.name = string(textptr);
      memcpy(&text_size, binary_header, 2);
      binary_header = binary_header + 2;
      textptr = (char *)binary_header;
      binary_header = binary_header + text_size;
      gc.variables = string(textptr);
      memcpy(&text_size, binary_header, 2);
      binary_header = binary_header + 2;
      gc.args_num = text_size;
      memcpy(&_address, binary_header, 4);
      binary_header = binary_header + 4;
      gc.address = _address;
      memcpy(&_address, binary_header, 4);
      binary_header = binary_header + 4;
      gc.variableaddress = _address;
      // printf("funcrion %s adfrees:%d\n\r",gc.name .c_str(),gc.address);
      finalexe->functions.push_back(gc);
    }
    break;
    default:
      error.error = 1;
      error.error_message = "Unknown type binary header invalid";
      return error;
      break;
    }
  }

  return error;
}
executable _createExcutablefromBinary(Binary *bin)
{
  executable exe;
  error_message_struct error;
  exe.error.error = 0;

  //printf("on cree un executbale de taiile %d et data %d \n",bin->instruction_size,bin->data_size);
  uint32_t *exec = (uint32_t *)heap_caps_malloc(bin->instruction_size, MALLOC_CAP_EXEC);

  uint8_t *_binary_data = (uint8_t *)malloc(bin->data_size);

  error = decodeBinaryHeader(bin->binary_data, bin->function_data, _binary_data, (uint32_t)exec, &exe, bin->instruction_size);

  if (exe.functions.size() == 0)
  {
    exe.error.error = 1;
    exe.error.error_message = "No global start function found";
    return exe;
  }
  if (error.error == 1)
  {

    exe.error = error;
   // _asm_parsed.clear();
   // all_text.clear();
    return exe;
  }

  memcpy(exec, bin->binary_data, bin->instruction_size);

  // free(tmp_exec);

  for (int i = 0; i < exe.functions.size(); i++)
  {

    exe.functions[i].address = (uint32_t)((exe.functions[i].address) / 4);
  }
  exe.start_program = exec;
  exe.data = _binary_data;
  exe.binary_size = _instr_size;
  exe.data_size = _data_size;

  return exe;
}
void displayBinary(Binary *bin)
{
  printf("char _bin[%d]={\n", 19 + 8 + bin->tmp_instruction_size + bin->function_size);
  printf("'E','S','P','L','i','v','e','S','c','r','i','p','t','1','.','0','.','1',0x0");
// char g[]="ESPLiveScript1.0.1";

  printf(",0x%x", bin->tmp_instruction_size& 0xff);
   printf(",0x%x", bin->tmp_instruction_size/256);
  printf(",0x%x", bin->instruction_size& 0xff);
  printf(",0x%x", bin->instruction_size/256);
  printf(",0x%x", bin->data_size& 0xff);
   printf(",0x%x", bin->data_size/256);
  printf(",0x%x", bin->function_size&0xff);
  printf(",0x%x", bin->function_size/256);
  for (int i = 0; i < bin->tmp_instruction_size; i++)
  {
    if(*(bin->binary_data + i)>=32 and *(bin->binary_data + i)<125)
    printf(",'%c'", *(bin->binary_data + i));
    else
    printf(",0x%x", *(bin->binary_data + i));
  }
  for (int i = 0; i < bin->function_size; i++)
  {    
    if(*(bin->function_data + i)>=32 and *(bin->function_data + i)<125)
    printf(",'%c'", *(bin->function_data + i));
    else
    printf(",0x%x", *(bin->function_data + i));
  }
  printf("}\n");
}

void binaryFromArray(char *_array,Binary *bin)
{
  bin->error.error = 0;
  char ver[19];
  char *arr=_array;
  if (strncmp(_array, "ESPLiveScript1.0.1",18) != 0)
  {
    bin->error.error = 1;
    bin->error.error_message = string_format("wrong format expected ESPLiveScript1.0.1 got %s", (const char *)ver);
    return;
  }
  arr+=19;
  memcpy(&bin->tmp_instruction_size,arr,2);
  arr+=2;
   memcpy(&bin->instruction_size,arr,2);
  arr+=2;
    memcpy(&bin->data_size,arr,2);
  arr+=2; 
      memcpy(&bin->function_size,arr,2);
  arr+=2; 
   uint8_t *tmp = (uint8_t *)malloc(bin->tmp_instruction_size);
   bin->binary_data = tmp;
   memcpy(tmp,arr,bin->tmp_instruction_size);
   arr+=bin->tmp_instruction_size;
   uint8_t *tmp2 = (uint8_t *)malloc(bin->function_size);
   bin->function_data = tmp2;
   memcpy(tmp2,arr,bin->function_size);
}

void loadBinary(char *name, fs::FS &fs, Binary *bin)
{
  File root = fs.open(name);
  bin->error.error = 0;
  char ver[19];
  root.read((uint8_t *)ver, 19);
  if (strcmp(ver, "ESPLiveScript1.0.1") != 0)
  {
    bin->error.error = 1;
    bin->error.error_message = string_format("wrong format expected ESPLiveScript1.0.1 got %s", (const char *)ver);
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

executable createExectutable(Binary *bin)
{

  executable exec;
  exec.binary_size = 0;
  exec.data_size = 0;

  exec = _createExcutablefromBinary(bin);

  freeBinary(bin);

  return exec;
}

void executeBinaryAsm(uint32_t *j) //, uint32_t *c)
{

  string s = string_format("Executing asm code @%x", j);
  pushToConsole(s, false);

  asm volatile( //"l32i a10,%1,0\n\t"
      "l32i a15,%0,0\n\t"
      "callx8 a15"
      : : "r"(j) //, "r"(c)
      :);
}


error_message_struct executeBinary(string function, executable ex, uint32_t handle, void *exePtr, Arguments arguments)
{
  error_message_struct res;
  // uint32_t toexecute;
// printf("execut %d\n", handle);
  res.error = 0;
  for (int i = 0; i < ex.functions.size(); i++)
  {
    string ftofind = ex.functions[i].name;
    if (ex.functions[i].name.find_first_of("(") != string::npos)
    {
      ftofind = ex.functions[i].name.substr(0, ex.functions[i].name.find_first_of("("));
    }
 // printf("coparing %s\n",ftofind.c_str());
    if (ftofind.compare(function) == 0)
    {
      // printf("address of function %s :%x\n",ex.functions[i].name.c_str(), ex.functions[i].address);

      //
      ex.functions[i].address = (uint32_t)(ex.start_program + ex.functions[i].address);
      uint32_t *t = (uint32_t *)ex.data;
    // t++;
      *t = handle;
     t++;
      *t = handle;
      t++;
      *t = (uint32_t)exePtr;
   //   printf("exx %x\n",(uint32_t)exePtr);
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
  res.error_message = string_format("Immpossible to execute %s: not found\n", function.c_str());
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

    heap_caps_aligned_free(ex->start_program);
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

void artiPrintf(char const *format, ...)
{
    va_list argp;
    va_start(argp, format);
    vprintf(format, argp);
    // printf("\r\n");
    va_end(argp);
}
void artiPrintfln(char const *format, ...)
{
    va_list argp;
    va_start(argp, format);
    vprintf(format, argp);
    printf("\r\n");
    va_end(argp);
}
void showError(int line, uint32_t size, uint32_t got)
{
    pushToConsole(string_format("Overflow error  max size: %d got %d", size, got), true);
}
class INIT_PARSER
{
public:
    INIT_PARSER()
    {
        addExternalFunction("printf", "void", "char *,Args", (void *)artiPrintf);
        addExternalFunction("printfln", "void", "char *,Args", (void *)artiPrintfln);
        addExternalFunction("error", "void", "int,uint32_t,uint32_t", (void *)&showError);
    }
};
INIT_PARSER initialization_parser;
#endif