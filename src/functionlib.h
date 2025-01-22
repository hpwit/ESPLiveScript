#include <string>

#pragma once
#ifndef __FUNCTION_LIB
#define __FUNCTION_LIB

 list<int> add_on;
//string division="";
string _sync="\
uint32_t _handle_;\n\
uint32_t _execaddr_;\n\
__ASM__ void sync()\n\
{\n\
\"entry a1,32\" \n\
\"l32r a4,@__handle_\" \n\
\"l32i a10,a4,0\" \n\
\"callExt a8,@__sync\"\n\
\"retw.n\" \n\
}@";
string division="\
__ASM__ float __div(float a,float b)\n\
{ \n\
\"entry a1,16\" \n\
\"div0.s f3, f2\"\n\
\"nexp01.s f4, f2\"\n\
\"const.s f5, 1\"\n\
\"maddn.s f5, f4, f3\"\n\
\"mov.s f6, f3\"\n\
\"mov.s f7, f2\"\n\
\"nexp01.s f2, f1\"\n\
\"maddn.s f6, f5, f6\"\n\
\"const.s f5, 1\"\n\
\"const.s f0, 0\"\n\
\"neg.s f8, f2\"\n\
\"maddn.s f5, f4, f6\"\n\
\"maddn.s f0, f8, f3\"\n\
\"mkdadj.s f7, f1\"\n\
\"maddn.s f6, f5, f6\"\n\
\"maddn.s f8, f4, f0\"\n\
\"const.s f3, 1\"\n\
\"maddn.s f3, f4, f6\"\n\
\"maddn.s f0, f8, f6\"\n\
\"neg.s f2, f2\"\n\
\"maddn.s f6, f3, f6\"\n\
\"maddn.s f2, f4, f0\"\n\
\"addexpm.s f0, f7\"\n\
\"addexp.s f6, f7\"\n\
\"divn.s f0, f2, f6\"\n\
\"retw.n\"\n\
}@";

string _rand="\
__ASM__ uint32_t rand(uint32_t mod) \n\
{\n\
\"entry a1,56\" \n\
\"l32r a4,@_stack_rand(d)\" \n\
\"l32i a3,a4,0\" \n\
\"rsr a14,234\" \n\
\"mov a13,a14\" \n\
\"mull a14,a14,a14\" \n\
\"mull a14,a14,a13\" \n\
\"mull a14,a14,a14\" \n\
\"add a14,a14,a13\" \n\
\"addi a14,a13,1\" \n\
\"remu a2,a14,a3\" \n\
//\"l32r a4,@_stackr\" \n\
//\"s32i a15,a4,0\" \n\
\"retw.n\" \n\
}@";

string _copycode="\
__ASM__ void copy(uint8_t *dest,uint8_t *from,uint16_t size) \n\
{ \n\
   \"entry a1,80\"\n\
   \"l32r a4,@_stack_copy\" \n\
   \"l32i a5,a4,0\" \n\
   \"l32i a6,a4,4\" \n\
   \"l16ui a7,a4,8\" \n\
   \"loop:\" \n\
   \"l8ui a8,a6,0\" \n\
   \"s8i a8,a5,0\" \n\
   \"addi a6,a6,1\" \n\
   \"addi a5,a5,1\" \n\
   \"addi a7,a7,-1\" \n\
   \"bnez a7,loop\" \n\
   \"retw.n\" \n\
}@";
string _memset="\
__ASM__ void memset(uint8_t *obj,uint8_t val, uint16_t size )\n\
{\n\
   \"entry a1,80\" \n\
   \"l32r a4,@_stack_memset\" \n\
   \"l32i a5,a4,0\" \n\
   \"l8ui a6,a4,4\" \n\
   \"l16ui a7,a4,6\" \n\
   \"loop_memset:\" \n\
   \"s8i a6,a5,0\" \n\
   \"addi a5,a5,1\" \n\
   \"addi a7,a7,-1\" \n\
   \"bnez a7,loop_memset\" \n\
   \"retw.n\" \n\
}@";

string _millis="\
__ASM__ uint32_t millis()\n\
{\n\
\"rsr a14,234\" \n\
\"l32r a13,__base_millis\"\n\
}\n\
@";

string _fill="\
__ASM__ void fill(uint8_t *dest, uint8_t *obj, uint8_t objsize,uint16_t nb_iteration) \n\
{\n\
   \"entry a1,80\" \n\
   \"l32r a4,@_stack_fill\" \n\
   \"l32i a5,a4,0\" \n\
   \"l32i a6,a4,4\" \n\
   \"l8ui a7,a4,8\" \n\
   \"l16ui a8,a4,10\" \n\
   \"fill_loop_main_fill:\" \n\
   \"mov a9,a7\" \n\
   \"mov a10,a6\" \n\
   \"fill_loop_main_second:\" \n\
   \"l8ui a11,a10,0\" \n\
   \"s8i a11,a5,0\" \n\
   \"addi a5,a5,1\" \n\
   \"addi a10,a10,1\" \n\
   \"addi a9,a9,-1\" \n\
   \"bnez a9, fill_loop_main_second\" \n\
   \"addi a8,a8,-1\" \n\
   \"bnez a8,fill_loop_main_fill\" \n\
   \"retw.n\" \n\
}@";

string _arduino="\n\
void main(){\n\
setup();\n\
while(2>1)\n\
{\n\
loop();\n\
}\n\
}\n\
";
string base_ext_functions="\n\
define true 1\n\
define false 0\n\
@";

//external void printfln(char * s,Args a);\n
string empty_header="";
int stdlib_size=5;
string stdlib[]={"sync","rand","copy","memset","fill","arduino"};
 string * _stdlib[]={&_sync,&_rand,&_copycode,&_memset,&_fill,&_arduino};
string * _stdlib_header[]={&empty_header,&empty_header,&empty_header,&empty_header,&empty_header};

int findLibFunction(string name)
{
    //int pos=-1;
    for(int i=0;i<stdlib_size;i++)
    {
        if(stdlib[i].compare(name)==0)
            return i;
    }
    return -1;
}


#endif