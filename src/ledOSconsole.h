
#pragma once
#ifndef _CONSOLE
#define _CONSOLE

#include <stdlib.h>
#include <vector>

#include "Arduino.h"
#include "string_function.h"
#define PROG_MODE 16
#define EXIT_MODE_PROG 5

// #ifndef MIN(a,b)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
// #endif

// #ifndef MAX(a,b)
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
// #endif

#define _push(x) Serial.print(x)

using namespace std;

typedef struct
{
  int x;
  int y;
  int internaly;
} coord;

typedef struct
{
  const char *White = "\u001b[37m";
  const char *Black = "\u001b[30m";
  const char *Red = "\u001b[38;5;196m";
  const char *Green = "\u001b[32m";
  const char *Yellow = "\u001b[33m";
  const char *Blue = "\u001b[34m";
  const char *Magenta = "\u001b[35m";
  const char *Cyan = "\u001b[36m";
  const char *BWhite = "\u001b[38;5;255m";
  const char *BBlack = "\u001b[30;1m";
  const char *BRed = "\u001b[31;1m";
  const char *BGreen = "\u001b[38;5;46m";
  const char *BYellow = "\u001b[33;1m";
  const char *BBlue = "\u001b[34;1m";
  const char *BMagenta = "\u001b[35;1m";
  const char *BCyan = "\u001b[36;1m";
  const char *Grey = "\u001b[38;5;240m";
} _termColor;
_termColor termColor;

typedef struct
{
  const char *White = "\u001b[47m";
  const char *Black = "\u001b[40m";
  const char *Red = "\u001b[41m";
  const char *Green = "\u001b[42m";
  const char *Yellow = "\u001b[43m";
  const char *Blue = "\u001b[44m";
  const char *Magenta = "\u001b[45m";
  const char *Cyan = "\u001b[46m";
  const char *BWhite = "\u001b[47;1m";
  const char *BBlack = "\u001b[40;1m";
  const char *BRed = "\u001b[41;1m";
  const char *BGreen = "\u001b[48;2;0;255;0m";
  const char *BYellow = "\u001b[43;1m";
  const char *BBlue = "\u001b[44;1m";
  const char *BMagenta = "\u001b[45;1m";
  const char *BCyan = "\u001b[46;1m";
} _termBackgroundColor;
_termBackgroundColor termBackgroundColor;

typedef struct
{

  const char *Bold = "\u001b[1m";
  const char *Underline = "\u001b[4m";
  const char *Reversed = "\u001b[7m";
} _termFormat;
_termFormat termFormat;

typedef struct
{
  const char *ENDLINE = "\r\n";
  const char *ESC_RESET = "\u001b[0m";
  const char *DEFAULT_PROMPT = "\u001b[7mLedOS\u001b[1m\u25b6";
  const char *PROGRAM_PROMPT = "";
  const char *DOWN = "\u001b[100B";
  const char *MOVEUP = "\u001b[0F";
  const char *MOVEDOWN = "\u001b[0E";
  const char *BACK = "\u001b[1D";
  const char *FORWARD = "\u001b[1C";
  const char *LEFT = "\u001b[100D";
  const char *SCROLLUP = "\u001b[0S";
  const char *SAVE = "\u001b[s";
  const char *RESTORE = "\u001b[u";
  const char *SHOWCURSOR = "\u001b[25h";
  const char *DELINE = "\u001b[2K";
  const char *HIDECURSOR = "\u001b[25l";
  const char *BEGIN_OF_LINE = "\u001b[0G";
  const char *ERASE_FROM_CURSOR_TO_EOL = "\u001b[0K";
} _config;
_config config;

class console;
typedef struct
{
  uint8_t esc_code;
  void (*command)(console *cons);
} console_esc_command;

typedef struct
{
  string keyword;
  void (*command)(console *cons, vector<string> args);
} console_keyword_command;

enum consoleMode
{
  keyword,
  edit,
  execute
};
string moveleft(int i)
{
  return string_format("\u001b[%dD", i);
}

string moveright(int i)
{
  return string_format("\u001b[%dC", i);
}

string defaultPrompt(console *cons);
string editPrompt(console *cons);
void insertLine(console *cons);
void _list(console *cons, int start, int len);
class console
{
public:
  string (*prompt)(console *cons);
  int height = 200;
  int width = 200;
  coord internal_coordinates;
  vector<char *> defaultPromptFormatColor;
  vector<char *> editorPromptFormatColor;
  string sentence;
  vector<console_esc_command> esc_commands;
  vector<console_keyword_command> keyword_commands;
  consoleMode cmode;
  vector<string> script;
  bool displayf = true;
  string defaultformat = config.ESC_RESET; //+termBackgroundColor.Black+termColor.BGreen;
  string errorformat = config.ESC_RESET;   //+termBackgroundColor.Black+termColor.Red+termFormat.Bold;
  string editprompt = config.ESC_RESET;    //+termBackgroundColor.Black+termColor.White;
  string editcontent = config.ESC_RESET;   //+termBackgroundColor.Black+termColor.BWhite+termFormat.Bold;
  string footerformat = config.ESC_RESET;
  string currentformat;

  int index_sentence;

  console()
  {
    // initEscCommands();
    footerformat += termFormat.Reversed;
    footerformat += termColor.BWhite;

    // defaultformat+=termBackgroundColor.BBlack;
    // defaultformat+="\u001b[49m";
    defaultformat += termColor.BGreen;

    // errorformat+=termBackgroundColor.BBlack;
    errorformat += termColor.Red;

    // editprompt+=termBackgroundColor.BBlack;
    editprompt += termColor.Grey;
    editprompt += "\u001b[48;5;236m";

    // editcontent+=termBackgroundColor.BBlack;
    editcontent += termColor.BWhite;

    cmode = keyword;
    prompt = &defaultPrompt;
    currentformat = defaultformat;
  }

  bool analyseEscCommand(char c)
  {
    for (int i = 0; i < esc_commands.size(); i++)
    {
      console_esc_command esc_command = esc_commands[i];
      if (esc_command.esc_code == c)
      {
        esc_command.command(this);
        return true;
      }
    }
    return false;
  }

  bool analyseKeywordCommand(string c, vector<string> args)
  {
    for (int i = 0; i < keyword_commands.size(); i++)
    {
      console_keyword_command keyword_command = keyword_commands[i];
      if (keyword_command.keyword == c)
      {
        keyword_command.command(this, args);
        return true;
      }
    }
    return false;
  }

  static void cls(console *cons, vector<string> args)
  {

    Serial.print("\u001b[2J\u001b[0;0H");
    cons->internal_coordinates.y = 1;
    cons->internal_coordinates.internaly = 0;
    cons->internal_coordinates.x = 1;
  }

  void cls(console *cons)
  {
    vector<string> args;
    cls(cons, args);
  }
  void gotoline()
  {
    internal_coordinates.y++;
    internal_coordinates.internaly++;
    internal_coordinates.x = 1;
    if (cmode == edit)
    {
      // internal_coordinates.y >= height &&
      if (internal_coordinates.internaly >= height - 1)
      {
        internal_coordinates.internaly = height - 2;
        Serial.print("\u001b[0S");
        Serial.print("\u001b[1G");
        Serial.print("\u001b[0K");

        return;
      }
      else
      {
        // internal_coordinates.internaly++;
        Serial.printf("%s", config.ENDLINE);
      }
    }
    else
    {
      Serial.printf("%s", config.ENDLINE);
    }
  }

  void run()
  {
    char c;
    // getConsoleSize();
    printf("welcome\r\n");

    // Serial.printf("%d:%d:%s",width,height,config.DEFAULT_PROMPT);
    _push(prompt(this).c_str());
    // internal_coordinates=getCursorPos();
    internal_coordinates.x = 1;
    internal_coordinates.y = 1;
    internal_coordinates.internaly = 0;
    displayf = false;
    // displayfooter();

    while (1)
    {
      if (Serial.available() > 0)
      {
        c = Serial.read();
        bool res = analyseEscCommand(c);

        if (res == false)
        {
          switch (c)
          {

          case 127:
            if (internal_coordinates.x > 1)
            {
              if (internal_coordinates.x > sentence.size())
              {
                sentence = sentence.substr(0, sentence.length() - 1);
                internal_coordinates.x--;
                Serial.print("\u001b[1D");
                Serial.printf("\u001b[0K");
              }
              else
              {
                string save = sentence.substr(internal_coordinates.x - 1, sentence.size());
                sentence = sentence.substr(0, internal_coordinates.x - 2) + sentence.substr(internal_coordinates.x - 1, sentence.size());
                internal_coordinates.x--;

                Serial.printf("\u001b[1D");
                _push(config.SAVE);
                Serial.printf("\u001b[0K%s", save.c_str());
                _push(config.RESTORE);
              }
              if (cmode == edit)
              {
                if (internal_coordinates.y <= script.size())
                {
                  script[internal_coordinates.y - 1] = sentence;
                }
              }
            }
            else
            {
              if (cmode == edit)
              {
                _push(config.SAVE);
                string save_sentence = script[internal_coordinates.y - 2];
                script.erase(script.begin() + internal_coordinates.y - 1);
                internal_coordinates.y--;
                int save=internal_coordinates.y;
                int save2=internal_coordinates.internaly;
                
                _push(config.LEFT);
                _list(this, internal_coordinates.y+1, height - internal_coordinates.internaly);
                internal_coordinates.y=save;
              internal_coordinates.internaly=save2-1;
                if(internal_coordinates.internaly+1<height-2 && internal_coordinates.y<=script.size())
                {
                  _push(config.LEFT);
                  _push(config.DELINE);
                }
                _push(config.RESTORE);
                _push(config.LEFT);
                _push(config.MOVEUP);
                sentence=save_sentence;
                internal_coordinates.x=sentence.size()+1;
                _push(moveright(5 + sentence.size()).c_str());
              }
            }
            break;
          case '\r':
          {

            if (cmode == keyword)
            {
              if (sentence.size() < 1)
              {
                gotoline();
                _push(prompt(this).c_str());
                continue;
              }

              currentformat = defaultformat;
              vector<string> cmd_line = split(trim(sentence), " ");

              if (trim(cmd_line[0]).size() > 0)
              {

                vector<string> args;
                if (cmd_line.size() > 1)
                {
                  for (int i = 1; i < cmd_line.size(); i++)
                  {
                    if (trim(cmd_line[i]).size() > 0)
                    {
                      args.push_back(cmd_line[i]);
                    }
                  }
                }
                bool res = analyseKeywordCommand(cmd_line[0], args);
                if (res == false)
                {
                  gotoline();
                  Serial.printf("%sLedOS commande not found: %s", errorformat.c_str(), sentence.c_str());
                  gotoline();
                }
                else
                {
                  gotoline();
                }
              }
              else
              {
                gotoline();
              }
              _push(prompt(this).c_str());
              sentence = "";
              delay(10);
            }
            else if (cmode == edit)
            {
              currentformat = editcontent;
              if (displayf == true)
              {
                if (internal_coordinates.y > script.size())
                {
                  script.push_back(sentence.substr(0, internal_coordinates.x - 1));
                  string save = sentence.substr(internal_coordinates.x - 1, sentence.size());
                  _push(config.ERASE_FROM_CURSOR_TO_EOL);
                  gotoline();
                  if (displayf == true)
                  {
                    _push(prompt(this).c_str());
                  }
                  _push(save.c_str());
                  sentence = save;
                  internal_coordinates.x = 1;
                  _push(config.LEFT);
                  _push(moveright(5).c_str());
                }
                else
                {

                  insertLine(this);
                }
              }
              else
              {
                script.push_back(sentence);
                _push(config.ENDLINE);
                internal_coordinates.y++;
                internal_coordinates.x = 1;
                if (internal_coordinates.internaly < height - 2)
                {
                  internal_coordinates.internaly++;
                }
                sentence = "";
              }
            }
          }
          break;
          default:
          {
            if (displayf == true)
            {
              _push(config.SAVE);
              Serial.printf("%c%s", c, sentence.substr(internal_coordinates.x - 1, sentence.size()).c_str());
              sentence = sentence.substr(0, internal_coordinates.x - 1) + c + sentence.substr(internal_coordinates.x - 1, sentence.size());
              // Serial.printf("%c%s",c,sentence.substr(internal_coordinates.x-1,sentence.size()));
              //  if(displayf==true)
              _push(config.RESTORE);
              _push(config.FORWARD);
              internal_coordinates.x++;
            }
            else
            {
              sentence += c;

              Serial.write(c);
            
              internal_coordinates.x++;
            }
          }
          break;
          }
        }
        if (displayf == true)
          displayfooter();
      }
    }
  }

  void displayfooter()
  {

    Serial.printf(config.SAVE);
    Serial.printf(config.HIDECURSOR);
    Serial.printf(config.DOWN);
    Serial.printf(config.LEFT);
    Serial.print("\u001b[0K");
    Serial.printf("%sPos x:%3d y:%3d height:%d curpos:%d script size:%d  mem:%12d%s", footerformat.c_str(), internal_coordinates.x, internal_coordinates.y, height, internal_coordinates.internaly, script.size(), esp_get_free_heap_size(), currentformat.c_str());
    Serial.printf(config.RESTORE);
    // Serial.printf(config.SHOWCURSOR);
  }

  void addEscCommand(uint8_t esc_code, void (*command)(console *cons))
  {
    console_esc_command es;
    es.esc_code = esc_code;
    es.command = command;
    esc_commands.push_back(es);
  }

  void addKeywordCommand(string keyword, void (*command)(console *cons, vector<string> args))
  {
    console_keyword_command es;
    es.keyword = keyword;
    es.command = command;
    keyword_commands.push_back(es);
  }

  string getEscapeSequence()
  {
    bool first = true;
    char c;
    string res;
    while (1)
    {
      if (Serial.available() > 0)
      {
        c = Serial.read();
        res += c;
        if (c >= 65 and c <= 90)
        {
          return res;
        }
      }
    }
  }

  coord getCursorPos()
  {
    coord co;
    int x, y;
    do
    {
      Serial.print("\u001b[6n");
      string res = getEscapeSequence();
      sscanf(res.c_str(), "\033[%d;%dR", &y, &x);
    } while (x < 0 or x > width or y < 0 or y > height);

    co.x = x;
    co.y = y;
    return co;
  }
  void getConsoleSize()
  {
    Serial.printf("%s", config.SAVE);
    Serial.printf("%s", config.DOWN);
    Serial.printf("\u001b[10000C");
    coord co = getCursorPos();
    width = co.x;
    height = co.y;
    Serial.printf("%s", config.RESTORE);
  }
};

void switchfooter(console *cons)
{
  cons->displayf = !cons->displayf;
  if (cons->displayf == true)
  {
    coord c = cons->getCursorPos();
    if (c.y == cons->height)
    {
      Serial.print("\u001b[0S");
      Serial.printf("\u001b[1A");

      cons->displayfooter();
    }
  }
  else
  {
    Serial.printf(config.SAVE);
    Serial.printf(config.HIDECURSOR);
    Serial.printf(config.DOWN);
    Serial.printf(config.LEFT);
    Serial.print("\u001b[0K");
    Serial.printf(config.RESTORE);
  }
}
void exitProgMode(console *cons)
{
  if (cons->cmode == edit)
  {

    if (cons->internal_coordinates.y > cons->script.size())
    {
      if (cons->sentence.size() > 0)
        cons->script.push_back(cons->sentence);
      ;
      cons->sentence = "";
      // cons->gotoline();
    }
    else
    {
      cons->script[cons->internal_coordinates.y - 1] = cons->sentence;
      cons->sentence = "";
    }
    cons->cmode = keyword;
    cons->prompt = &defaultPrompt;
    cons->currentformat = cons->defaultformat;
    /*
    Serial.printf("%s",config.ESC_RESET);
    Serial.printf("%s",termColor.Magenta);
    Serial.printf("Exiting edit mode to re enter Ctrl+p");
    */
    cons->cls(cons);
    cons->displayf = false;
    // cons->gotoline();
    // Serial.printf("%s",config.ESC_RESET);
    _push(cons->prompt(cons).c_str());
    // cons->gotoline();
  }
}

void test(console *cons)
{
  cons->getConsoleSize();
  Serial.printf("%d %d\r\n", cons->width, cons->height);
}
void _list(console *cons)
{
  cons->cls(cons);
  for (int i = 0; i < cons->script.size(); i++)
  {
    _push(config.DELINE);
    Serial.printf("%s%3d %s %s", cons->editprompt.c_str(), i + 1, cons->editcontent.c_str(), cons->script[i].c_str());
    // Serial.printf("%s%s", cons->prompt(cons).c_str(), cons->script[i].c_str());

    cons->gotoline();
  }
}

void _list(console *cons, int start, int len)
{
  // je peux en mettre height-internaly+1
  int end = start + len - 1;

  for (int i = start - 1; i < end; i++)
  {

    if (i < cons->script.size())
    {
      _push(config.DELINE);
      //
      Serial.printf("%s%3d %s %s", cons->editprompt.c_str(), i + 1, cons->editcontent.c_str(), cons->script[i].c_str());
      // Serial.printf("%s%s", cons->prompt(cons).c_str(), cons->script[i].c_str());
      if (cons->internal_coordinates.internaly < cons->height - 2)
      {
        cons->gotoline();
      }
      else
      {
        cons->sentence = cons->script[i];
        cons->internal_coordinates.x = cons->sentence.size() + 1;
      }
    }
  }
}
void displayline(console *cons, int i)
{
  if (i - 1 >= cons->script.size())
    return;
  Serial.printf("%s%3d %s %s", cons->editprompt.c_str(), i, cons->editcontent.c_str(), cons->script[i - 1].c_str());
  // Serial.printf("%s%s", cons->prompt(cons).c_str(), cons->script[i].c_str());
}
void clear(console *cons, vector<string> args)
{

  cons->script.clear();
  // cons->gotoline();
}

void enterProgMode(console *cons)
{
  if (cons->cmode == keyword)
  {
    cons->displayf = true;
    cons->cmode = edit;
    cons->height = 200;
    cons->width = 200;
    cons->prompt = &editPrompt;
    cons->sentence = "";
    cons->currentformat = cons->editcontent;
    cons->getConsoleSize();
    cons->cls(cons);
    cons->displayf = true;
    _list(cons, 1, cons->height - 1);
    
    if (cons->script.size() < cons->height - 1)
    {
      _push(cons->prompt(cons).c_str());
    }
  }
  else
  {
    exitProgMode(cons);
  }
}

void scrollup(console *cons)
{
  _push("\u001b[1S");
}

void editorup(console *cons)
{
  if (cons->cmode != edit)
  {
    return;
  }
  int y = cons->internal_coordinates.internaly;
  if (cons->internal_coordinates.y > cons->script.size())
  {
    if (cons->sentence.size() > 0)
      cons->script.push_back(cons->sentence);
  }
  else
  {
    cons->script[cons->internal_coordinates.y - 1] = cons->sentence;
  }
  if (y >= 1)
  {

    cons->sentence = cons->script[cons->internal_coordinates.y - 2];
    int x = cons->sentence.size() + 5;
    _push(config.MOVEUP);
    _push(moveright(x).c_str());
    cons->internal_coordinates.y--;
    cons->internal_coordinates.x = x - 5 + 1;
    if (cons->internal_coordinates.internaly > 0)
      cons->internal_coordinates.internaly--;
  }
  else if (y == 0)
  {
    if (cons->internal_coordinates.y > 1)
    {
      cons->internal_coordinates.y--;
      _push("\u001b[0T");
      _push("\u001b[0;0H");

      int save = cons->internal_coordinates.y;
      _list(cons, cons->internal_coordinates.y, 1);
      _push("\u001b[0;0H");

      cons->internal_coordinates.y = save;
      _push(moveright(cons->script[cons->internal_coordinates.y - 1].size() + 5).c_str());
      cons->internal_coordinates.internaly = 0;
      cons->internal_coordinates.x = cons->script[cons->internal_coordinates.y - 1].size() + 1;
    }
  }
  cons->sentence = cons->script[cons->internal_coordinates.y - 1];
}

void editordown(console *cons)
{
  if (cons->cmode != edit)
  {
    return;
  }
  int y = cons->internal_coordinates.internaly;
  if (cons->internal_coordinates.y > cons->script.size())
  {
    if (cons->sentence.size() > 0)
      cons->script.push_back(cons->sentence);
  }
  else
  {
    cons->script[cons->internal_coordinates.y - 1] = cons->sentence;
  }
  if (y < cons->height - 2)
  {

    if (cons->internal_coordinates.y < cons->script.size())
    {
      cons->internal_coordinates.y++;
      cons->sentence = cons->script[cons->internal_coordinates.y - 1];
      int x = cons->sentence.size() + 5;
      _push(config.MOVEDOWN);
      _push(moveright(x).c_str());
      cons->internal_coordinates.x = x - 5 + 1;
      if (cons->internal_coordinates.internaly < cons->height - 2)
        cons->internal_coordinates.internaly++;
    }
  }
  else if (y == cons->height - 2)
  {

    // Serial.printf("jkjdfksd");
    if (cons->internal_coordinates.y < cons->script.size())
    {
      _push("\u001b[1S");
      _push("\u001b[2K");
      _push("\u001b[100D");
      cons->internal_coordinates.y++;
      displayline(cons, cons->internal_coordinates.y);
      cons->sentence = cons->script[cons->internal_coordinates.y - 1];
      int x = cons->sentence.size() + 5;
      // _push(moveright(x).c_str());
      cons->internal_coordinates.x = x - 5 + 1;
      // _push("\u001b[0;0H");
    }
  }
}

void editorleft(console *cons)
{
  if (cons->internal_coordinates.x > 1)
  {
    cons->internal_coordinates.x--;
    _push(config.BACK);
  }
}
void editorright(console *cons)
{
  if (cons->internal_coordinates.x <= cons->sentence.size())
  {
    cons->internal_coordinates.x++;
    _push(config.FORWARD);
  }
}

void extraEscCommand(console *cons)
{
  char c1 = Serial.read();
  char c2 = Serial.read();
  if (c1 == 91)
  {
    switch (c2)
    {
    case 65:
    {
      if (cons->cmode == edit)
      {
        editorup(cons);
      }
    }
    break;
    case 66:
      if (cons->cmode == edit)
      {
        editordown(cons);
      }
      break;
    case 67:
      editorright(cons);
      break;
    case 68:
      editorleft(cons);
      break;
    default:
      break;
    }
  }
}
// cons->gotoline();

void top(console *cons)
{
  Serial.printf("Closing the consoles");
  Serial.printf("%s", config.ENDLINE);
}

void initEscCommands(console *cons)
{
  cons->addEscCommand(16, enterProgMode);
  cons->addEscCommand(27, extraEscCommand);
  cons->addEscCommand(15, test);
  cons->addEscCommand(5, scrollup);
  cons->addEscCommand(22, switchfooter);
  // cons->addKeywordCommand("_list", _list);
  cons->addKeywordCommand("cls", cons->cls);
  cons->addKeywordCommand("clear", clear);
  // cons->addEscCommand(27,top);
}

string defaultPrompt(console *cons)
{
  return string_format("%sLedOS>", cons->defaultformat.c_str(), cons->internal_coordinates.x, cons->internal_coordinates.y);
}

string editPrompt(console *cons)
{
  return string_format("%s%3d %s ", cons->editprompt.c_str(), cons->internal_coordinates.y, cons->editcontent.c_str());
}

void insertLine(console *cons)
{

  cons->script[cons->internal_coordinates.y - 1] = cons->sentence.substr(0, cons->internal_coordinates.x - 1);
  _push(config.ERASE_FROM_CURSOR_TO_EOL);
  cons->script.push_back(""); // on ajoute une ligne
  for (int i = cons->script.size() - 1; i >= cons->internal_coordinates.y + 1; i--)
  {
    cons->script[i] = cons->script[i - 1];
  }
  cons->script[cons->internal_coordinates.y] = cons->sentence.substr(cons->internal_coordinates.x - 1, cons->sentence.size());
  cons->internal_coordinates.y++;
  cons->sentence = cons->sentence.substr(cons->internal_coordinates.x - 1, cons->sentence.size());
  int save = cons->internal_coordinates.y;
  int save2 = cons->internal_coordinates.internaly;
  if (cons->internal_coordinates.internaly < cons->height - 2)
  {
    save2++;
    _push(config.MOVEDOWN);

    _push(config.LEFT);
    _push(config.SAVE);
    int line_dispo = cons->height - cons->internal_coordinates.internaly - 2;
    _list(cons, save, line_dispo);
    cons->internal_coordinates.y = save;
    cons->internal_coordinates.internaly = save2;
    _push(config.RESTORE);
    if (cons->displayf == true)
    {
      _push(cons->prompt(cons).c_str());
    }
  }
  else
  {

    _push(config.SCROLLUP);
    _push(config.LEFT);
    // _push(config.DOWN);
    _push(config.DELINE);
    _push(config.BEGIN_OF_LINE);
    displayline(cons, cons->internal_coordinates.y);
    cons->internal_coordinates.y = save;
    cons->internal_coordinates.internaly = save2;
    cons->internal_coordinates.x = 1;
  }
}

#endif