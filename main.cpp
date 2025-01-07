#include "ESPLiveScript.h"
#define __TEST_DEBUG
string script="\
void main()\n\
{\n\
for(int i=0;i<20;i++)\n\
   {\n\
       printfln(\"i:%2d 3xi:%2d\",i,3*i);\n\
   }\n\
}";
int main() {
 // put your setup code here, to run once:
 
 Parser _parser;
 Executable exec=_parser.parseScript(&script);
  return 0;

}
