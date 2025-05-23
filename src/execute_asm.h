#pragma once
#ifndef __EXECUTE_ASM
#define __EXECUTE_ASM
#include <ArduinoJson.h>

JsonVariant  getfromJson( JsonDocument obj, string str)
{
   vector<string> tile;
   JsonVariant jso=obj;
  tile = split(str, ".");
   if(tile.size()>=1)
      for(int i=0;i<tile.size()-1;i++)
      {
        //rintf("d:%s\n",tile[i]);
        jso=jso[tile[i].c_str()];
      }
  JsonVariant res=jso[tile[tile.size()-1].c_str()];
  //tile.empty();
  tile.clear();
  return res;
}
void freeBinary(Binary *bin)
{
  #ifndef __TEST_DEBUG
  if (bin->binary_data)
    heap_caps_free(bin->binary_data);
  if (bin->function_data)
    heap_caps_free(bin->function_data);
    bin->binary_data=NULL;
    bin->function_data=NULL;
  // delete(bin);
  #endif
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
        uint32_t content;
        uint32_t *new_adr;
#ifndef __TEST_DEBUG
         content = (uint32_t)((external_links[index].ptr));

         new_adr = (uint32_t *)exec + nb_data;
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

        // printf("calculate ext %s\n\r", (*it)->getText());
        /*
        bincode = jump_call8(bincode, _address + (uint32_t)_exec, (uint32_t)external_links[index].ptr);
        memcpy(exec + _address, &bincode, 3);
        */
       #ifndef __TEST_DEBUG
        uint32_t content = (uint32_t)((external_links[index].ptr));

        uint32_t *new_adr = (uint32_t *)exec + nb_data;
        #else
        uint32_t content;
        uint32_t *new_adr;
        #endif
        memcpy(new_adr, &content, 4);

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
    case 5:
    {
      jsonVariable jso;
      memcpy(&text_size, binary_header, 2);
      binary_header = binary_header + 2;
      textptr = (char *)binary_header;
      binary_header = binary_header + text_size;
      jso.json= string(textptr);
      memcpy(&_address, binary_header, 4);
      jso.address=_address;
      binary_header = binary_header + 4;
      uint8_t type;
      memcpy(&type, binary_header, 1);
      jso.type=type;
      binary_header = binary_header + 1;
      finalexe->jsonVars.push_back(jso);
     // printf("found %s  %d %s\n",jso.json.c_str(),jso.address,varTypeEnumNames[jso.type].c_str());
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
  #ifndef __TEST_DEBUG
  uint32_t *exec = (uint32_t *)heap_caps_malloc(bin->instruction_size, MALLOC_CAP_EXEC);
#else
uint32_t *exec = (uint32_t *)malloc(bin->instruction_size);
  #endif

  uint8_t *_binary_data = (uint8_t *)malloc(bin->data_size);
  #ifndef __TEST_DEBUG

  error = decodeBinaryHeader(bin->binary_data, bin->function_data, _binary_data, (uint32_t)exec, &exe, bin->instruction_size);

#else
  error = decodeBinaryHeader(bin->binary_data, bin->function_data, _binary_data, 0, &exe, bin->instruction_size);
#endif
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
#ifndef __TEST_DEBUG
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
#endif
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
  #ifndef __TEST_DEBUG
  string s = string_format("Executing asm code @%x", j);
  pushToConsole(s, false);

  asm volatile( //"l32i a10,%1,0\n\t"
      "l32i a15,%0,0\n\t"
      "callx8 a15"
      : : "r"(j) //, "r"(c)
      :);

      #endif
}

error_message_struct updateParameters(executable ex,string json)
{
  // uint32_t toexecute;
 //printf("execut |%s|\n", json.c_str());
error_message_struct res;
if(json!="")
{
  JsonDocument doc;
    DeserializationError error = deserializeJson(doc, json);
 res.error = 0;
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
   Serial.println(error.f_str());
    res.error=1;
    res.error_message="deserializeJson() failed: ";
    return res;
  }
   uint8_t *var = ex.data;
  for(int i=0;i<ex.jsonVars.size();i++)
  {
   // printf(" taking:%s\n",ex.jsonVars[i].json.c_str());
    JsonVariant p=getfromJson(doc,ex.jsonVars[i].json);
    switch(ex.jsonVars[i].type)
    {
      case __uint32_t__:
      {
      uint32_t jk=(uint32_t)p;
      
      memcpy(var+ex.jsonVars[i].address, &jk, 4);
     
       }
        break;
      
      case __int__:
      { 
      int jk=(int)p;
     //  printf(" taking:%s %d %d\n",ex.jsonVars[i].json.c_str(),jk,ex.jsonVars[i].address);
      memcpy((uint8_t*)(ex.data+ex.jsonVars[i].address), &jk, 4);
      }
      break;
      case __float__:
      {
      float jk=(float)p;
      memcpy(var+ex.jsonVars[i].address, &jk, 4);
      }
      break;
      case __char__:
      {
      string jk=(string)p;
      memcpy(var+ex.jsonVars[i].address, jk.c_str(), jk.size());
      }
      break;
      default:
      break;

    }
  }
}
else
{
  printf("no json\n");
}
return res;
}

error_message_struct executeBinary(string function, executable ex, uint32_t handle, void *exePtr, Arguments arguments,string json)
{

error_message_struct res;
 //printf("executvith %s %s\n",function.c_str(), json.c_str());
res=updateParameters(ex,json);
if(res.error==1)
 return res;
 
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
      #ifndef __TEST_DEBUG
      ex.functions[i].address = (uint32_t)(ex.start_program + ex.functions[i].address);
      #endif
      uint32_t *t = (uint32_t *)ex.data;
    // t++;
      *t = handle;
     t++;
      *t = handle;
      t++;
      #ifndef __TEST_DEBUG
      *t = (uint32_t)exePtr;
      #endif
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


error_message_struct executeBinary(string function, executable ex, uint32_t handle, void *exePtr, Arguments arguments)
{
  return executeBinary(function,ex,handle,exePtr,arguments,"");
}
error_message_struct executeBinary(executable ex, uint32_t handle)
{
  Arguments args;
  return executeBinary(ex.functions[0].name, ex, handle, NULL, args);
}
void freeExecutable(executable *ex)
{
  #ifndef __TEST_DEBUG
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
  #endif
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

#ifdef USE_FASTLED
#define FORCE_REFERENCE(var)  asm volatile( "" : : "r" (var) )
#define K255 255
#define K171 171
#define K170 170
#define K85  85

CRGB hsv( uint8_t hue,uint8_t sat,uint8_t val)
{
    // Yellow has a higher inherent brightness than
    // any other color; 'pure' yellow is perceived to
    // be 93% as bright as white.  In order to make
    // yellow appear the correct relative brightness,
    // it has to be rendered brighter than all other
    // colors.
    // Level Y1 is a moderate boost, the default.
    // Level Y2 is a strong boost.
    const uint8_t Y1 = 1;
    const uint8_t Y2 = 0;
    
    // G2: Whether to divide all greens by two.
    // Depends GREATLY on your particular LEDs
    const uint8_t G2 = 0;
    
    // Gscale: what to scale green down by.
    // Depends GREATLY on your particular LEDs
    const uint8_t Gscale = 0;
    
    
   // uint8_t hue = hsv.hue;
   // uint8_t sat = hsv.sat;
   // uint8_t val = hsv.val;
    
    uint8_t offset = hue & 0x1F; // 0..31
    
    // offset8 = offset * 8
    uint8_t offset8 = offset;
    {
#if defined(__AVR__)
        // Left to its own devices, gcc turns "x <<= 3" into a loop
        // It's much faster and smaller to just do three single-bit shifts
        // So this business is to force that.
        offset8 <<= 1;
        asm volatile("");
        offset8 <<= 1;
        asm volatile("");
        offset8 <<= 1;
#else
        // On ARM and other non-AVR platforms, we just shift 3.
        offset8 <<= 3;
#endif
    }
    
    uint8_t third = scale8( offset8, (256 / 3)); // max = 85
    
    uint8_t r, g, b;
    
    if( ! (hue & 0x80) ) {
        // 0XX
        if( ! (hue & 0x40) ) {
            // 00X
            //section 0-1
            if( ! (hue & 0x20) ) {
                // 000
                //case 0: // R -> O
                r = K255 - third;
                g = third;
                b = 0;
                FORCE_REFERENCE(b);
            } else {
                // 001
                //case 1: // O -> Y
                if( Y1 ) {
                    r = K171;
                    g = K85 + third ;
                    b = 0;
                    FORCE_REFERENCE(b);
                }
                if( Y2 ) {
                    r = K170 + third;
                    //uint8_t twothirds = (third << 1);
                    uint8_t twothirds = scale8( offset8, ((256 * 2) / 3)); // max=170
                    g = K85 + twothirds;
                    b = 0;
                    FORCE_REFERENCE(b);
                }
            }
        } else {
            //01X
            // section 2-3
            if( !  (hue & 0x20) ) {
                // 010
                //case 2: // Y -> G
                if( Y1 ) {
                    //uint8_t twothirds = (third << 1);
                    uint8_t twothirds = scale8( offset8, ((256 * 2) / 3)); // max=170
                    r = K171 - twothirds;
                    g = K170 + third;
                    b = 0;
                    FORCE_REFERENCE(b);
                }
                if( Y2 ) {
                    r = K255 - offset8;
                    g = K255;
                    b = 0;
                    FORCE_REFERENCE(b);
                }
            } else {
                // 011
                // case 3: // G -> A
                r = 0;
                FORCE_REFERENCE(r);
                g = K255 - third;
                b = third;
            }
        }
    } else {
        // section 4-7
        // 1XX
        if( ! (hue & 0x40) ) {
            // 10X
            if( ! ( hue & 0x20) ) {
                // 100
                //case 4: // A -> B
                r = 0;
                FORCE_REFERENCE(r);
                //uint8_t twothirds = (third << 1);
                uint8_t twothirds = scale8( offset8, ((256 * 2) / 3)); // max=170
                g = K171 - twothirds; //K170?
                b = K85  + twothirds;
                
            } else {
                // 101
                //case 5: // B -> P
                r = third;
                g = 0;
                FORCE_REFERENCE(g);
                b = K255 - third;
                
            }
        } else {
            if( !  (hue & 0x20)  ) {
                // 110
                //case 6: // P -- K
                r = K85 + third;
                g = 0;
                FORCE_REFERENCE(g);
                b = K171 - third;
                
            } else {
                // 111
                //case 7: // K -> R
                r = K170 + third;
                g = 0;
                FORCE_REFERENCE(g);
                b = K85 - third;
                
            }
        }
    }
    
    // This is one of the good places to scale the green down,
    // although the client can scale green down as well.
    if( G2 ) g = g >> 1;
    if( Gscale ) g = scale8_video_LEAVING_R1_DIRTY( g, Gscale);
    
    // Scale down colors if we're desaturated at all
    // and add the brightness_floor to r, g, and b.
    if( sat != 255 ) {
        if( sat == 0) {
            r = 255; b = 255; g = 255;
        } else {
            uint8_t desat = 255 - sat;
            desat = scale8_video( desat, desat);

            uint8_t satscale = 255 - desat;
            //satscale = sat; // uncomment to revert to pre-2021 saturation behavior

            //nscale8x3_video( r, g, b, sat);
#if (FASTLED_SCALE8_FIXED==1)
            r = scale8_LEAVING_R1_DIRTY( r, satscale);
            g = scale8_LEAVING_R1_DIRTY( g, satscale);
            b = scale8_LEAVING_R1_DIRTY( b, satscale);
            cleanup_R1();
#else
            if( r ) r = scale8( r, satscale) + 1;
            if( g ) g = scale8( g, satscale) + 1;
            if( b ) b = scale8( b, satscale) + 1;
#endif
            uint8_t brightness_floor = desat;
            r += brightness_floor;
            g += brightness_floor;
            b += brightness_floor;
        }
    }
    
    // Now scale everything down if we're at value < 255.
    if( val != 255 ) {
        
        val = scale8_video_LEAVING_R1_DIRTY( val, val);
        if( val == 0 ) {
            r=0; g=0; b=0;
        } else {
            // nscale8x3_video( r, g, b, val);
#if (FASTLED_SCALE8_FIXED==1)
            r = scale8_LEAVING_R1_DIRTY( r, val);
            g = scale8_LEAVING_R1_DIRTY( g, val);
            b = scale8_LEAVING_R1_DIRTY( b, val);
            cleanup_R1();
#else
            if( r ) r = scale8( r, val) + 1;
            if( g ) g = scale8( g, val) + 1;
            if( b ) b = scale8( b, val) + 1;
#endif
        }
    }
    
    // Here we have the old AVR "missing std X+n" problem again
    // It turns out that fixing it winds up costing more than
    // not fixing it.
    // To paraphrase Dr Bronner, profile! profile! profile!
    //asm volatile(  ""  :  :  : "r26", "r27" );
    //asm volatile (" movw r30, r26 \n" : : : "r30", "r31");
    return CRGB(r,g,b);
}

#endif
class INIT_PARSER
{
public:
    INIT_PARSER()
    {
        addExternalFunction("printf", "void", "char *,Args", (void *)artiPrintf);
        addExternalFunction("printfln", "void", "char *,Args", (void *)artiPrintfln);
        addExternalFunction("error", "void", "int,uint32_t,uint32_t", (void *)&showError);
  #ifdef USE_FASTLED
        addExternalFunction("hsv", "CRGB","int,int,int", (void *)hsv);
  #endif

    }
};
INIT_PARSER initialization_parser;
#endif