#include "ESPLiveScript.h"
#include "driver/gpio.h"

typedef struct
{
  Executable *ptr;
  char *name;
  uint32_t last;
  uint32_t _new;
} call_isr;
call_isr isr_struct;


static void IRAM_ATTR gpio_isr_handler(void *args) {
  call_isr *ll = (call_isr *)args;
  unsigned long currentTime = millis();

  if ((currentTime - ll->last) > 100) {
    ll->ptr->executeOnly(string(ll->name));
    ll->last = currentTime;  // Update the last interrupt time
  }
}
void setup_gpio_interrupt(Executable *ptr, char *str, int pin_num) {
  printf("Setting up the interrupt\n");
  gpio_config_t io_conf = {
    .pin_bit_mask = (1ULL << pin_num),
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_ENABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,  // Disable pull-down
    .intr_type = GPIO_INTR_NEGEDGE,
  };

  gpio_config(&io_conf);
  gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
  call_isr *_isr_struct = &isr_struct;
  _isr_struct->ptr = ptr;
  _isr_struct->name = str;
  _isr_struct->last = millis();
  gpio_isr_handler_add((gpio_num_t)pin_num, gpio_isr_handler, _isr_struct);
}

string script = R"EOF(
int number=0;
void increase()
{
  number++;
}

void main()
{  
   pinInterrupt(_execaddr_, "increase", 23);
   while(true)
   {
     printfln("number:%d",number);
   }
}
)EOF";

void setup() {
  Serial.begin(115200);
  addExternalFunction("pinInterrupt", "void", "uint32_t,char *,int", (void *)setup_gpio_interrupt);
  Parser p;
  Executable exec = p.parseScript(&script);
  if (exec.isExeExists()) {
    exec.execute("main");
  }
  else {
  printf("Error:%s\n",exec.error.error_message.c_str());
  }
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}
