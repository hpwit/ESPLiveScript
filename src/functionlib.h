#include <string>
#pragma once
#ifndef __FUNCTION_LIB
#define __FUNCTION_LIB
string division="";
//string division="\
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
}\n\
";

#endif