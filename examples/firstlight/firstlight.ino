#include "ESPLiveScript.h"

string script="\
void main()\n\
{\n\
for(int i=0;i<20;i++)\n\
    {\n\
        printfln(\"i:%2d  3*i:%2d\",i,3*i);\n\
    }\n\
}";
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

Parser p;
Executable exec=p.parseScript(&script);
if(exec.isExeExists())
{
 exec.execute("main");
}

}

void loop() {
  // put your main code here, to run repeatedly:

}
