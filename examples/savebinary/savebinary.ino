#include "ESPLiveScript.h"
#include "FS.h"
#include <LittleFS.h>
string script = R"EOF(
void main()
{
for(int i=0;i<20;i++)
    {
        printfln("i:%2d  3*i:%2d",i,3*i);
    }
}
)EOF";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Parser p;
  Binary bin = p.parseScriptBinary(&script);
  Binary bin2;
  LittleFS.begin(true);
  if (!bin.error.error) {
    // display the content of the bin
    // displayBinary(&bin);
    // save the binary
    saveBinary("/test.bin", LittleFS, &bin);
    freeBinary(&bin);

    loadBinary("/test.bin", LittleFS, &bin2);
    Executable ex;

    ex.createExecutableFromBinary(&bin2);
    if (ex.isExeExists())
      ex.execute("main");

  } else {
    printf("%s\n\r", bin.error.error_message.c_str());
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
