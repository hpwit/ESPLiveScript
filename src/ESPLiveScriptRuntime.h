#include <vector>
#include "asm_external.h"
#include "execute_asm.h"
#include "execute.h"

#ifdef __CONSOLE_ESP32
#include "tokenizer.h"
Parser p = Parser();

void kill(Console *cons, vector<string> args)
{
    if (args.size() == 0)
    {
        if (SCExecutable.isRunning())
        {

            SCExecutable.kill();
        }
        else
        {
            pushToConsole("Nothing is currently running.", true);
        }
    }
    else
    {
        scriptRuntime.kill(args[0]);
    }
}

Arguments parseInputArgs(string variables)
{
    Arguments _args;
    vector<string> ar = split(variables, ",");
    for (int i = 0; i < ar.size(); i++)
    {
        // printf("var %d :%s\n", i, ar[i].c_str());
        if (ar[i].find(".") != -1)
        {
            float j = 0;
            sscanf(ar[i].c_str(), "%f", &j);
            _args.add(j);
        }
        else
        {
            int j = 0;
            sscanf(ar[i].c_str(), "%d", &j);
            _args.add(j);
        }
    }
    return _args;
}
void run(Console *cons, vector<string> args)
{
    Arguments _args;
    string progToRun = "";
    if (args.size() > 0)
    {
        if (args[0].find("(") != -1)
        {
            string variables = args[0].substr(1, args[0].size() - 2);
            _args = parseInputArgs(variables);
        }
        else
        {
            progToRun = args[0];
        }
        if (args.size() > 1)
        {
            string variables = args[1].substr(1, args[1].size() - 2);
            _args = parseInputArgs(variables);
        }
        // printf("%s\n\r",args[0].c_str());
    }
    if (progToRun.size() == 0)
    {
        if (SCExecutable.isRunning())
        {
            LedOS.pushToConsole("Something Already running kill it first ...");
            kill(cons, args);
        }
        SCExecutable.executeAsTask("main", _args);
    }
    else
    {
        scriptRuntime.executeAsTask(progToRun, _args);
    }
}
void kill_cEsc(Console *cons)
{
    LedOS.displayf = false;
    vector<string> f;
    if (cons->cmode == edit)
    {
        LedOS.storeCurrentLine();
    }
    kill(cons, f);
    if (cons->cmode == keyword)
    {
        _push(config.ENDLINE);
        _push(cons->prompt(cons).c_str());
    }
}
void parseasm(Console *cons, vector<string> args)
{
}
void compile_c(Console *cons, vector<string> args)
{
    pushToConsole("Compiling ...", true);
    Executable _scExec = p.parse_c(&cons->script);
    _scExec.name = cons->filename;
    scriptRuntime.addExe(_scExec);
    if (_scExec.exeExist)
    {

        pushToConsole(string_format("Compiling done. Handle number:%d", scriptRuntime._scExecutables.size()), true);
    }
    else
    {
        pushToConsole(_scExec.error.error_message);
    }
}
void free(Console *cons, vector<string> args)
{
    if (args.size() == 0)
    {
        if (SCExecutable.isRunning())
        {
            SCExecutable.kill();
        }
        SCExecutable.free();
    }
    else
    {
        scriptRuntime.deleteExe(args[0]);
    }
}
void saveBin(Console *cons, vector<string> args)
{
    Binary bin;
    if (args.size() > 0)
    {
        LedOS.pushToConsole("Compiling ...", true);
        __parser_debug = false;
        bin = p.parse_cBinary(&cons->script);
        saveBinary((char *)(fileSystem.current_path + args[0]).c_str(), *fileSystem.current_mount->fs, &bin);
    }
    else
        LedOS.pushToConsole("filename missing ...", true);
}
void binload(Console *cons, vector<string> args)
{
    Binary bin;
    Arguments _args;
    if (args.size() > 0)
    {
        if (SCExecutable.isRunning())
        {
            LedOS.pushToConsole("Something Already running kill it first ...");
            vector<string> k;
            kill(cons, k);
        }
        bool othercore = false;

        SCExecutable.free();
        //  bin = p.parse_cBinary(&cons->script);
        loadBinary((char *)(fileSystem.current_path + args[0]).c_str(), *fileSystem.current_mount->fs, &bin);
        if (bin.error.error)
        {
            pushToConsole(bin.error.error_message.c_str(), true);
            return;
        }
        if (args.size() > 1)
        {
            string variables = args[1].substr(1, args[1].size() - 2);
            _args = parseInputArgs(variables);
        }
        executable _executecmd = createExectutable(&bin);
        SCExecutable.setExecutable(_executecmd);
        SCExecutable.error = _executecmd.error;
        if (_executecmd.error.error == 1)
        {
            // exeExist = false;
            // Serial.printf(termColor.Red);

            pushToConsole(_executecmd.error.error_message.c_str(), true);
        }
        else
        {
            if (SCExecutable.exeExist)
            {

                exeExist = true;
                if (true)
                {
                    // vector<string> d;
                    //  d.push_back("main");
                    LedOS.pushToConsole("***********START RUN *********");
                    SCExecutable.executeAsTask("main", _args);
                    // run(cons, d);

                    if (cons->cmode == keyword)
                    {
                        _push(config.ENDLINE);
                        _push(cons->prompt(cons).c_str());
                    }
                }
                else
                {
                    LedOS.pushToConsole("Start program", true);
                    SCExecutable.execute("main");
                    // executeBinary("main", executecmd);
                    LedOS.pushToConsole("Execution done.", true);
                }
            }
        }
    }
    else

        LedOS.pushToConsole("filename missing ...", true);
}

void parse_c(Console *cons, vector<string> args)
{
    if (SCExecutable.isRunning())
    {
        LedOS.pushToConsole("Something Already running kill it first ...");
        vector<string> k;
        kill(cons, k);
    }
    bool othercore = false;

    SCExecutable.free();
    LedOS.pushToConsole("Compiling ...", true);
    if (args.size() > 0)
    {
        if (args[0].compare("&") != 0)
            __parser_debug = true;
        if (args[args.size() - 1].compare("&") == 0)
            othercore = true;
    }
    SCExecutable = p.parse_c(&cons->script);
    // if (p.parse_c(&cons->script))

    if (SCExecutable.exeExist)
    {

        exeExist = true;
        if (othercore)
        {
            vector<string> d;
            // d.push_back("main");
            LedOS.pushToConsole("***********START RUN *********");
            run(cons, d);

            if (cons->cmode == keyword)
            {
                _push(config.ENDLINE);
                _push(cons->prompt(cons).c_str());
            }
        }
        else
        {
            LedOS.pushToConsole("Start program", true);
            SCExecutable.execute("main");
            // executeBinary("main", executecmd);
            LedOS.pushToConsole("Execution done.", true);
        }
    }
    else
    {
        LedOS.pushToConsole(SCExecutable.error.error_message, true);
    }
    __parser_debug = false;
}

void parsec_cEsc(Console *cons)
{
    LedOS.displayf = false;
    vector<string> f;
    f.push_back("&");
    if (cons->cmode == edit)
    {
        LedOS.storeCurrentLine();
    }
    parse_c(cons, f);
    if (cons->cmode == keyword)
    {
        _push(config.ENDLINE);
        _push(cons->prompt(cons).c_str());
    }
}

void listExec(Console *cons, vector<string> args)
{
    scriptRuntime.listExec();
}

class __INIT_PARSER
{
public:
    __INIT_PARSER()
    {
        // __run_handle = NULL;
        LedOS.addKeywordCommand("compile", parse_c, "Compile and run a program add '&' for run on the second core");
        LedOS.addKeywordCommand("comp", compile_c, "Compile  a program");
        LedOS.addKeywordCommand("createbin", saveBin, "Compile  a program into binary");
        LedOS.addKeywordCommand("executebin", binload, "executebin");
        LedOS.addKeywordCommand("./", binload, "executebin");
        LedOS.addKeywordCommand("list", listExec, "list the compiled programs");
        LedOS.addKeywordCommand("free", free, "free the binary free x will free the program with handle x");
        LedOS.addKeywordCommand("run", run, "Run an already compiled program (always second Core) run x run program with handle x");
        LedOS.addKeywordCommand("kill", kill, "Stop a running program kill x kill program with handle x");
        LedOS.addKeywordCommand("parseasm", parseasm, "Parse assembly program");
        LedOS.addEscCommand(18, parsec_cEsc, "Compile and execute a program (always second Core)");
        LedOS.addEscCommand(11, kill_cEsc, "Stop a running program");
        // addExternal("__feed", externalType::function, (void *)feedTheDog);
    }
};
__INIT_PARSER _init_parser;

list<const char *> _parenthesis;
list<const char *> _curlybracket;
list<const char *> _bracket;
int _prevparenthesis;
int _prevcurlybracket;
int _prevbracket;
#define _NB_COLORS 3
const char *_colors[_NB_COLORS] = {

    termColor.Magenta,
    termColor.LBlue,
    termColor.Yellow,

};

void formatInit()
{
    _parenthesis.clear();
    _curlybracket.clear();
    _bracket.clear();
    _prevparenthesis = 0;
    _prevcurlybracket = 0;
    _prevbracket = 0;
}

void formatLine()
{
    _prevparenthesis = 0;
    _prevcurlybracket = 0;
    _prevbracket = 0;
}

string formatLine(string str)
{
    // Serial.printf("streing:%s\r\n",str.c_str());
    if (str.size() < 2)
        return str;
    string sd;
    sd = "";
    sd = sd + str;
    // _parent.clear();
    _for_display = true;
    // Script s(&str);
    main_script.clear();
    _tks = &__tks;
    all_text.clear();
    main_script.addContent((char *)sd.c_str());
    main_script.init();
    // Serial.printf("streing:%s\r\n",str.c_str());
    _tks->tokenize(&main_script, true, true, 1);
    // _tks->init();
    // Serial.printf("streing:%s\r\n",str.c_str());
    string res = "";

    _parenthesis.clear();
    _bracket.clear();
    _parenthesis.clear();
    _prevparenthesis = 0;
    _prevcurlybracket = 0;
    _prevbracket = 0;

    while (_tks->current()->getType() != TokenEndOfFile) // for (int i = 0; i < _tks->size(); i++)
    {
        Token tk = *_tks->current();
        //   Serial.printf("token %s\r\n",tk.getText());
        /* if (tk.type == TokenOpenCurlyBracket)
           {

           // char *color= (char *)_colors[_curlybracket.size()%_NB_COLORS];
           res = res + string_format("%s%s", _colors[_curlybracket.size() % _NB_COLORS], tk.text.c_str());
           _curlybracket.push_back(_colors[_curlybracket.size() % _NB_COLORS]);
           _prevcurlybracket++;
           }
           else if (tk.type == TokenCloseCurlyBracket)
           {

           if (_curlybracket.size() == 0)
           {
           res = res + string_format("%s%s", "\u001b[38;5;196m", tk.text.c_str());
           }
           else
           {
           // char * color=_curlybracket.back();
           _prevcurlybracket--;
           res = res + string_format("%s%s", _curlybracket.back(), tk.text.c_str());
           _curlybracket.pop_back();
           }
           } */
        if (tk.getType() == TokenOpenParenthesis)
        {
            _prevparenthesis++;
            res =
                res + string_format("%s%s",
                                    _colors[(_parenthesis.size() +
                                             2) %
                                            _NB_COLORS],
                                    tk.getText());
            _parenthesis.push_back(_colors
                                       [(_parenthesis.size() + 2) % _NB_COLORS]);
        }
        else if (tk.getType() == TokenCloseParenthesis)
        {

            if (_parenthesis.size() == 0)
            {
                res =
                    res + string_format("%s%s", "\u001b[38;5;196m",
                                        tk.getText());
            }
            else
            {
                _prevparenthesis--;
                res =
                    res + string_format("%s%s", _parenthesis.back(),
                                        tk.getText());
                _parenthesis.pop_back();
            }
        }
        else if (tk.getType() == TokenOpenBracket)
        {
            _prevbracket++;
            res =
                res + string_format("%s%s",
                                    _colors[_bracket.size() % _NB_COLORS],
                                    tk.getText());
            _bracket.push_back(_colors[_bracket.size() % _NB_COLORS]);
        }
        else if (tk.getType() == TokenCloseBracket)
        {

            if (_bracket.size() == 0)
            {
                res =
                    res + string_format("%s%s", "\u001b[38;5;196m",
                                        tk.getText());
            }
            else
            {
                _prevbracket--;
                res =
                    res + string_format("%s%s", _bracket.back(), tk.getText());
                _bracket.pop_back();
            }
        }
        /*
           else if (tk.type == TokenKeyword)
           {
           res = res + string_format("%s%s", KeywordTypeFormat[tk._keyword], tk.text.c_str());
           } */
        else
        {
            Token tkn = *_tks->peek(1);
            if (tk.getType() == TokenIdentifier && tkn.getType() == TokenOpenParenthesis)
            {
                res =
                    res + string_format("%s%s", tokenFormat[TokenFunction],
                                        tk.getText());
            }
            else
            {

                res =
                    res + string_format("%s%s", tokenFormat[tk.type],
                                        tk.getText());
            }
            // _tks->prev();
        }
        _tks->next();
    }

    _tks->clear();
    //  _parent.clear();

    _for_display = false;
    return res;
}

class __INIT_TOKEN
{
public:
    __INIT_TOKEN()
    {
        LedOS.addHightLightinf("sc", formatLine, formatInit, formatLine);
    }
};
__INIT_TOKEN _init_token;
#endif