#include "ESPLiveScript.h"

string script="\
external int value;\n\
external uint16_t *array;\n\
//external uint16_t array[10]; is the same \n\
void fillArray()\n\
{\n\
  for(int i=0;i<10;i++)\n\
  {\n\
    array[i]=i*3;\n\
  }\n\
}\n\
void change()\n\
{\n\
value=value+2;\n\
}\n\
void main()\n\
{  \n\
   printfln(\"value: %d \",value);\n\
}";

int variable=0;
uint16_t _array[10];
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

Parser p;
addExternal("value", externalType::value, (void *)&variable);
addExternal("array", externalType::value, (void *)_array);
Executable exec=p.parseScript(&script);
if(exec.isExeExists())
{
  
 variable=5;
 exec.execute("main");
  variable=240;
 exec.execute("main");
 variable=15;
 printf("old value:%d ",variable);
 exec.execute("change");
 printf("new value:%d\n",variable);
 exec.execute("fillArray");
 for (int i=0;i<10;i++)
 {
  printf("%d:%d\n",i,_array[i]);
 }
}

}

void loop() {
  // put your main code here, to run repeatedly:

}
