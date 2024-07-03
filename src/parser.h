#pragma once
using namespace std;
using namespace std;

#include <string>
void pushToConsole(string str, bool force)
{
#ifdef __CONSOLE_ESP32
    LedOS.pushToConsole(str, force);
#else
#ifndef __TEST_DEBUG
    Serial.printf("%s\r\n", str.c_str());
#else
    printf("%s\r\n", str.c_str());
#endif
#endif
}

void pushToConsole(string str)
{
    pushToConsole(str, false);
}
// #include "tokenizer.h"
#include "functionlib.h"
#include "asm_parser.h"

#ifndef __TEST_DEBUG
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
// #include "soc/rtc_wdt.h"
#endif

#include "parsernodedef.h"
#ifndef __TEST_DEBUG
#include "execute.h"
#endif

class Parser
{

public:
    NodeToken root;
    Parser() {}

    string linepos()
    {
        string f = string_format(" at line:%d", current()->line); //, current()->pos);
        return f;
    }
    int size()
    {
        return _tks.size();
    }
    token *getTokenAtPos(int pos)
    {
        return _tks.getTokenAtPos(pos);
    }
    token *current()
    {
        return _tks.current();
    }
    token *next()
    {
        return _tks.next();
    }
    token *prev()
    {
        return _tks.prev();
    }
    token *peek(int index)
    {
        return _tks.peek(index);
    }
    bool Match(tokenType tt)
    {
        return _tks.Match(tt);
    }
    bool Match(tokenType tt, int index)
    {
        return _tks.Match(tt, index);
    }
    /*
    bool MatchKeyword(KeywordType t)
    {
        return _tks.MatchKeyword(t);
    }
    bool MatchKeyword(KeywordType t, int offset)
    {
        return _tks.MatchKeyword(t, offset);
    }
*/
    void parse()
    {

                    point_regnum=4;
    content.begin();
    header.begin();
        register_numr.clear();
    register_numl.clear();
    register_numl.push(15);
    register_numr.push(15);

        root._nodetype = programNode;
        point_regnum = 4;
        stack_size = 0;
        for_if_num = 0;
        block_statement_num = 0;
        local_var_num = 0;
        nb_argument = 0;
        _tks.init();
        current_cntx = &main_cntx;
        safeMode = false;
        saveReg = false;
        saveRegAbs = false;
       _asPointer = false;
       isPointer = false;
       isASM = false;   
        parseProgram();
        buildParents(&program);
    }

    bool parse_create()
    {
    //new
            point_regnum=4;
    content.begin();
    header.begin();
        register_numr.clear();
    register_numl.clear();
    register_numl.push(15);
    register_numr.push(15);
//new

        parse();
        if (Error.error)
        {

            while (Match(TokenEndOfFile) == false)
            {
                next();
            }
            clean();
            clean2();
            pushToConsole(Error.error_message.c_str(), true);
            return false;
        }
        else
        {
            // prettyPrint(program, "");
            pushToConsole("***********PARSING DONE*********");

            program.visitNode(&program);

            pushToConsole("***********COMPILING DONE*********");
            // p.cleanint();
            clean();

            pushToConsole("***********AFTER CLEAN*********");
            // printf("%d\n",_content.size());
            int s = _header.size();
            for (int i = 0; i < s; i++)
            {

                _content.push_front(_header.back());
                _header.pop_back();
            }

#ifndef __TEST_DEBUG
            pushToConsole("***********CREATE EXECUTABLE*********");
            executecmd = createExectutable(&_content, __parser_debug);
            // strcompile = "";
            clean2();
            if (executecmd.error.error == 0)
            {

                exeExist = true;
            }
            else
            {
                exeExist = false;
                // Serial.printf(termColor.Red);

                pushToConsole(executecmd.error.error_message.c_str(), true);
            }
            return exeExist;
#else
            return false;
#endif
        }
    }
    bool parse_c(list<string> *_script)
    {
        clean();
        clean2();
        __MEM();
        Script sc(&division);

        _tks.init();
        tokenizer(&sc);
        list_of_token.pop_back();

        _token_line = 1;

        for (string s : *_script)
        {
            // string g=s+'\0';
            Script sc(&s);
            _tks.init();
            tokenizer(&sc, false,false);
            _token_line++;
        }
        int f=_token_line;
        _index_token = list_of_token.begin();
        for (int i : add_on)
        {

            Script sc(_stdlib[i]);
            _tks.init();
            tokenizer(&sc, false,false);
        }
        // LedOS.script.clear();
        token t;
        t.type = TokenEndOfFile;
        t.line=f;
        list_of_token.push_back(t);
#ifdef __CONSOLE_ESP32
        // _script->clear();
#endif
        // printf("nb token:%d\r\n",list_of_token.size());
        __MEM();

        return parse_create();
    }
    bool parse_c(string *_script)
    {
        clean();
        clean2();

        Script sc(&division);

        _tks.init();
        tokenizer(&sc);
        list_of_token.pop_back();

        _token_line = 1;

        // string g=s+'\0';
        Script sc2(_script);
        _tks.init();
        tokenizer(&sc2, false,true);
        _token_line++;

        _index_token = list_of_token.begin();
        for (int i : add_on)
        {

            Script sc(_stdlib[i]);
            _tks.init();
            tokenizer(&sc, false,false);
        }
        // LedOS.script.clear();
        token t;
        t.type = TokenEndOfFile;
        list_of_token.push_back(t);
        return parse_create();
    }
    void clean()
    {
        clearContext(&main_cntx);
        _functions.clear();
        clearNodeToken(&program);
        list_of_token.clear();
        nb_args.clear();
    }
    void cleanint()
    {
        clearContext(current_cntx);
        clearNodeToken(&program);
    }
    void clean2()
    {
        _header.clear();
        _content.clear();
        _sp.clear();
        _target_stack.clear();
        _register_numr.clear();
        _register_numl.clear();
        _node_token_stack.clear();
        _other_tokens.clear();
        _types.clear();
        sav_t.clear();
        sav_token.clear();
        add_on.clear();
    }

    void parseCreateArguments()
    {
        Error.error = 0;
        NodeInputArguments arg;
        current_node = current_node->addChild(arg);
        if (Match(TokenCloseParenthesis))
        {
            // resParse result;
            Error.error = 0;
            // result._nd = arg;
            current_node = current_node->parent;
            // printf("on retourne with argh ide\n");
            return;
        }
        parseType();
        if (Error.error)
        {
            return;
        }
        parseVariableForCreation();
        if (Error.error)
        {
            return;
        }
        NodeToken _nd = nodeTokenList.pop();
        NodeToken t = nodeTokenList.pop();

        copyPrty(&t, &_nd);
        NodeDefLocalVariable var = NodeDefLocalVariable(_nd);

        // copyPrty(type._nd,&var);
        current_node->addChild(var);
        current_cntx->addVariable(var);
        // arg.addChild(nd);
        // next();
        // printf("current %s\n", tokenNames[current()->type].c_str());
        while (Match(TokenComma))
        {
            next();
            parseType();
            if (Error.error)
            {
                return;
            }
            parseVariableForCreation();
            if (Error.error)
            {
                return;
            }
            NodeToken _nd = nodeTokenList.pop();
            NodeToken t = nodeTokenList.pop();
            copyPrty(&t, &_nd);
            NodeDefLocalVariable var = NodeDefLocalVariable(_nd);

            // arg.addChild(var);
            current_node->addChild(var);
            current_cntx->addVariable(var);
            // next();
        }
        // prev();
        // resParse result;
        Error.error = 0;
        current_node = current_node->parent;
        return;
    }

    void getVariable(bool isStore)
    {

        current_cntx->findVariable(current(), false); // false
        // NodeToken *nd = search_result;
        if (search_result == NULL)
        {
            // //printf("hhheeheh\n");

            Error.error = 1;
            Error.error_message = string_format("impossible to find declaraiton for %s %s", current()->text.c_str(), linepos().c_str());
            next();
            return;
        }
        else
        {
            // token *vartoken = current();
            // auto var =
            // current_node = current_node->addChild(
            createNodeVariable(current(), search_result, isStore);
            next();
            if (Match(TokenOpenBracket))
            {
                // on parse
                next();

                parseExpr();
                if (Error.error)
                {
                    next();
                    return;
                }

                if (Match(TokenCloseBracket))
                {

                    // var.addChild(expr._nd);
                    // resParse res;
                    Error.error = 0;
                    current_node = current_node->parent;
                    // res._nd = var;
                    next();
                    return;
                }
                else
                {

                    Error.error = 1;
                    Error.error_message = string_format("expecting ] %s", current()->text.c_str());
                    next();
                    return;
                }
            }
            else
            {

                Error.error = 0;
                current_node = current_node->parent;
                // res._nd = var;
                // next();
                return;
            }
        }
    }

    void parseFactor()
    {
        // resParse result;
        Error.error = 0;
        // printf("entering factor line:%d pos:%d\n",current()->line,current()->pos);
        // token *_f = current();
        if (Match(TokenStar) && Match(TokenIdentifier, 1))
        {
            _asPointer = true;
            // printf("qsldkqsld\n");
            next();
            // return;
        }
        if (current()->type == TokenEndOfFile)
        {

            next();
            return;
        }

        else if (Match(TokenNumber))
        {

            // NodeNumber g = NodeNumber(current());
            current_node->addChild(NodeNumber(current()));
            next();

            Error.error = 0;
            // result._nd = g;
            // printf("exit factor\n");

            return;
        }

        else if (Match(TokenAddition) || Match(TokenSubstraction) || Match(TokenUppersand) || Match(TokenKeywordFabs) || Match(TokenKeywordAbs))
        {
            // token *t = current();
            // NodeUnitary g = NodeUnitary();
            current_node = current_node->addChild(NodeUnitary());
            sav_t.push_back(current());

            next();

            parseFactor();
            if (Error.error == 1)
            {
                return;
            }
            // NodeUnitary g = NodeUnitary(NodeOperator(t), res._nd);
            current_node->addChild(NodeOperator(sav_t.back()));
            sav_t.pop_back();
            current_node = current_node->parent;
            Error.error = 0;
            return;
        }

        else if (Match(TokenOpenParenthesis) && Match(TokenKeywordVarType, 1) && Match(TokenCloseParenthesis, 2) && Match(TokenOpenParenthesis, 3))
        {
            // on a (float) ....;
            next();
            //   NodeChangeType d=NodeChangeType(current());
            current_node = current_node->addChild(NodeChangeType(current()));
            next(); //)
            next(); //(
            next();
            parseExpr();
            if (Error.error == 1)
            {
                return;
            }
            if (Match(TokenCloseParenthesis))
            {
                next();
                Error.error = 0;
                current_node = current_node->parent;
                return;
            }
            else
            {
                Error.error = 1;
                Error.error_message = string_format("expected ) at %s", linepos().c_str());
                return;
            }
        }
        else if (Match(TokenOpenParenthesis))
        {
            next();
            parseExpr();
            if (Error.error == 1)
            {
                return;
            }
            // NodeToken d=parseExpr();
            if (Match(TokenCloseParenthesis))
            {
                next();
                Error.error = 0;
                return;
            }
            else
            {
                Error.error = 1;
                Error.error_message = string_format("expected ) at %s", linepos().c_str());
                return;
            }
        }

        else if (Match(TokenIdentifier) && !Match(TokenOpenParenthesis, 1))
        {
            getVariable(false);
            if (Error.error == 1)
            {
                // next();
                return;
            }

            return;
        }

        else if (Match(TokenIdentifier) && Match(TokenOpenParenthesis, 1))
        {
            // Serial.printf("eeaze\r\n");
            parseFunctionCall();
            // Serial.printf("eeazsfdsfdsfe\r\n");
            return;
        }

        else if (Match(TokenKeywordVarType) && Match(TokenOpenParenthesis, 1))
        {
            // on tente CRGB()
            // token *typeVar = current();
            sav_t.push_back(current());
            // NodeNumber num = NodeNumber( current());
            current_node = current_node->addChild(NodeNumber(current()));
            next();
            if (Match(TokenOpenParenthesis))
            {
                for (int i = 0; i < sav_t.back()->_vartype->size; i++)
                {
                    next();
                    parseExpr();
                    if (Error.error)
                    {
                        next();
                        return;
                    }
                    // num.addChild(res._nd);
                    if (i == sav_t.back()->_vartype->size - 1)
                    {
                        if (!Match(TokenCloseParenthesis))
                        {
                            // resParse res;
                            Error.error = 1;
                            Error.error_message = string_format(" ecpected  ) %s", linepos().c_str());
                            next();
                            return;
                        }
                    }
                    else
                    {
                        if (!Match(TokenComma))
                        {
                            // resParse res;
                            Error.error = 1;
                            Error.error_message = string_format(" ecpected  , %s", linepos().c_str());
                            next();
                            return;
                        }
                    }
                }
                next();
                current_node = current_node->parent;
                // resParse result;
                Error.error = 0;

                // result._nd=num;
                sav_t.pop_back();
                return;
            }
            else
            {
                // resParse res;
                Error.error = 1;
                Error.error_message = string_format(" expected ( %s", linepos().c_str());
                next();
                return;
            }
        }
        else if (Match(TokenString))
        {
            current_node->addChild(NodeString(current()));
            next();
            return;
        }
        Error.error = 1;
        Error.error_message = string_format(" impossible to find Token %s", linepos().c_str());
        next();
        return;
    }

    void parseTerm()
    {
        // printf("entering term line:%d pos:%d\n",current()->line,current()->pos);
        // NodeToken *sav_pa = current_node;
        sav_token.push_back(current_node);
        parseFactor();
        if (Error.error)
        {
            return;
        }
        while (Match(TokenStar) || Match(TokenSlash) || Match(TokenModulo) || Match(TokenKeywordOr) || Match(TokenKeywordAnd) || Match(TokenPower))
        {
            // token *op = current();
            sav_t.push_back(current());
            next();
            // NodeBinOperator nodeopt;
            _node_token_stack.push_back(current_node->children.back());
            // NodeToken d = current_node->children.back();
            current_node->children.pop_back();
            // current_node->children.erase( --current_node->children.end());
            current_node = current_node->addChild(NodeBinOperator());
            current_node->addChild(_node_token_stack.back());
            _node_token_stack.pop_back();
            // current_node->parent->children.remove(current_node->parent->children.back());
            // NodeOperator opt = NodeOperator(op);
            current_node->addChild(NodeOperator(sav_t.back()));
            sav_t.pop_back();
            parseFactor();
            if (Error.error == 1)
            {
                return;
            }
            current_node = current_node->parent;
            // left._nd = NodeBinOperator(left._nd, opt, right._nd);
        }
        // next();
        current_node = sav_token.back();
        sav_token.pop_back();
        // current_node = sav_pa;
        // printf("exit term\n");
        return;
    }

    void parseExpr()
    {
        // NodeToken *sav_pa = current_node;
        // Serial.printf("eee  term1\r\n");
        sav_token.push_back(current_node);
        // Serial.printf("eee  term\r\n");
        parseTerm();
        // Serial.printf("exit  term\r\n");
        if (Error.error == 1)
        {
            return;
        }
        while (Match(TokenAddition) || Match(TokenSubstraction) || Match(TokenShiftLeft) || Match(TokenShiftRight))
        {

            // token *op = current();
            sav_t.push_back(current());
            next();
            // NodeBinOperator nodeopt;
            /*
                        NodeToken d = current_node->children.back();
                        current_node->children.pop_back();
                        current_node = current_node->addChild(NodeBinOperator());
                        current_node->addChild(d);
            */
            _node_token_stack.push_back(current_node->children.back());
            // NodeToken d = current_node->children.back();
            current_node->children.pop_back();
            current_node = current_node->addChild(NodeBinOperator());
            current_node->addChild(_node_token_stack.back());
            _node_token_stack.pop_back();
            // current_node->parent->children.remove(current_node->parent->children.back());
            current_node->addChild(NodeOperator(sav_t.back()));
            sav_t.pop_back();
            parseTerm();
            if (Error.error == 1)
            {
                return;
            }
            current_node = current_node->parent;
            // left._nd = NodeBinOperator(left._nd, opt, right._nd);
        }
        // next();
        current_node = sav_token.back();
        sav_token.pop_back();
        // current_node = sav_pa;
        // printf("exit expr");
        Error.error = 0;
        return;
    }

    void parseArguments()
    {
        // resParse result;
        nb_argument = 0;
        nb_args.push_back(0);
        // NodeInputArguments arg;
        current_node = current_node->addChild(NodeInputArguments());
        if (Match(TokenCloseParenthesis))
        {
            // resParse result;
            Error.error = 0;
            // result._nd = arg;
            // printf("on retourne with argh ide\n");
            current_node = current_node->parent;
            next();
            return;
        }
        nb_args.pop_back();
        nb_args.push_back(1);
        // nb_argument = 1;
        // Serial.printf("lkklqdqsdksmdkqsd\r\n");
        parseExpr();
        // Serial.printf("lkklqdqsdksm excut dkqsd\r\n");
        if (Error.error)
        {
            return;
        }
        // arg.addChild(res._nd);
        while (Match(TokenComma))
        {
            next();
            __sav_arg = nb_args.back();
            nb_args.pop_back();
            nb_args.push_back(__sav_arg + 1);
            // nb_argument++;
            parseExpr();
            if (Error.error)
            {
                return;
            }
            // arg.addChild(res._nd);
        }
        if (!Match(TokenCloseParenthesis))
        {

            Error.error = 1;
            Error.error_message = string_format("Expected ) %s", linepos().c_str());
            next();
            return;
        }
        next();
        Error.error = 0;
        // result._nd = arg;
        current_node = current_node->parent;
        return;
    }

    void parseFunctionCall()
    {
        // Serial.printf("serial %s\r\n",current()->text.c_str());
        // int sav_nb_arg;
        // NodeToken *t = current_cntx->findFunction(current());

        main_cntx.findFunction(current());
        // NodeToken *t =search_result;
        if (search_result == NULL)
        {
            Error.error = 1;
            Error.error_message = string_format("function %s not found %s", current()->text.c_str(), linepos().c_str());
            return;
        }
        next();
        next();
        if (search_result->_nodetype == defExtFunctionNode)
        {
            // Serial.printf("serial2\r\n");
            // NodeExtCallFunction function = NodeExtCallFunction(t);
            current_node = current_node->addChild(NodeExtCallFunction(search_result));
            // sav_nb_arg = function._link->getChildAtPos(1)->children.size();
            nb_sav_args.push_back(current_node->_link->getChildAtPos(1)->children.size());
            // Serial.printf("serial3\r\n");
        }
        else
        {
            // Serial.printf("serial2\r\n");
            // NodeCallFunction function = NodeCallFunction(t);
            current_node = current_node->addChild(NodeCallFunction(search_result));
            // sav_nb_arg = function._link->getChildAtPos(1)->children.size();
            nb_sav_args.push_back(current_node->_link->getChildAtPos(1)->children.size());
            // Serial.printf("serial3\r\n");
        }
        parseArguments();
        // Serial.printf("serial4\r\n");
        if (Error.error)
        {
            return;
        }

        if (nb_sav_args.back() != nb_args.back()) // if (sav_nb_arg != nb_args.back())
        {
            Error.error = 1;
            // sav_nb_args
            Error.error_message = string_format("Expected %d arguments got %d %s", nb_sav_args.back(), nb_args.back(), linepos().c_str());
            return;
        }
        nb_args.pop_back();
        nb_sav_args.pop_back();
        Error.error = 0;
        current_node = current_node->parent;

        return;
    }

    void parseComparaison(string target)
    {
        // resParse res;
        Error.error = 0;
        //  NodeComparator cn = NodeComparator();
        current_node = current_node->addChild(NodeComparator());

        // res._nd=NodeToken();
        parseExpr();
        if (Error.error)
        {
            return;
        }
        // token *t=current();
        current_node->_token = current();
        next();
        parseExpr();
        if (Error.error)
        {
            return;
        }
        next();

        current_node->target = target;
        // cn.target=target;
        // cn.addChild(left._nd);
        // cn.addChild(right._nd);

        Error.error = 0;
        current_node = current_node->parent;
        return;
    }
    void parseStatement()
    {
        Error.error = 0;
        // asPointer =false;
        // resParse result;
        // NodeStatement statement;
        // current_node=current_node->addChild(statement);
        #ifndef __TEST_DEBUG
        printf("line:%d mem:%u\r\n",current()->line,esp_get_free_heap_size());
        #endif
        // on demarre avec la function
        if (Match(TokenString))
        {
            current_node->addChild(NodeString(current()));
            next();
            return;
        }
        else if(Match(TokenKeywordBreak))
        {
            string c=findForWhile();
            if(c.compare("__")==0)
            {
                Error.error = 1;
                Error.error_message = string_format("nor For ar while found for break %s", linepos().c_str());
                return;  
            }
            next();
            if (Match(TokenSemicolon))
            {
                current_node->addChild(NodeBreak(c));
                next();
                return;
            }
            else
            {
                Error.error = 1;
                Error.error_message = string_format("d Expected ; %s", linepos().c_str());
                return;             
            }
        }
        else if(Match(TokenKeywordContinue))
        {
            string c=findForWhile();
            if(c.compare("__")==0)
            {
                Error.error = 1;
                Error.error_message = string_format("nor For ar while found for continue %s", linepos().c_str());
                return;  
            }
            next();
            if (Match(TokenSemicolon))
            {
                current_node->addChild(NodeContinue(c));
                next();
                return;
            }
            else
            {
                Error.error = 1;
                Error.error_message = string_format("d Expected ; %s", linepos().c_str());
                return;             
            }
        }
        else if (Match(TokenKeywordReturn))
        {
            next();
            if (Match(TokenSemicolon))
            {
                current_node->addChild(NodeReturn());
                next();
                return;
            }
            else
            {
                current_node = current_node->addChild(NodeReturn());
                // next();
                parseExpr();
                if (Error.error)
                {
                    return;
                }
                if (Match(TokenSemicolon))
                {
                    Error.error = 0;
                    current_node = current_node->parent;
                    // res._nd = var;
                    next();
                    return;
                }
                else
                {
                    Error.error = 1;
                    Error.error_message = string_format("d Expected ; %s", linepos().c_str());
                    return;
                }
            }
        }
        else if (Match(TokenIdentifier) && Match(TokenOpenParenthesis, 1))
        {
            parseFunctionCall();
            if (Error.error)
            {
                return;
            }
            else
            {
                if (Match(TokenSemicolon))
                {
                    Error.error = 0;
                    // current_node->addChild(res._nd);
                    // current_node=current_node->parent;
                    // result._nd = res._nd;
                    next();
                    return;
                }
                else
                {
                    Error.error = 1;
                    Error.error_message = string_format("Expected ; %s", linepos().c_str());
                    next();
                    return;
                }
            }
        }
        if (Match(TokenIdentifier) && Match(TokenPlusPlus, 1))
        {
            // NodeAssignement d = NodeAssignement();
            current_node = current_node->addChild(NodeAssignement());
            getVariable(true);
            if (Error.error)
            {
                return;
            }
            // NodeUnitary g = NodeUnitary();
            current_node = current_node->addChild(NodeUnitary());
            prev();
            getVariable(false);
            if (Error.error)
            {
                return;
            }
            current_node->addChild(NodeOperator(current()));
            next();
            current_node = current_node->parent;
            current_node = current_node->parent;
            if (!Match(TokenSemicolon) && !Match(TokenCloseParenthesis))
            {
                Error.error = 1;
                Error.error_message = string_format("Expected ; %s", linepos().c_str());
                next();
                return;
            }
            Error.error = 0;
            next();
            return;
        }
        else if (Match(TokenStar) && Match(TokenIdentifier, 1))
        {
            _asPointer = true;
            // printf("qsldkqsld\n");
            next();
        }
        else if (Match(TokenIdentifier))
        {
            // NodeAssignement nd;
            current_node = current_node->addChild(NodeAssignement());
            getVariable(true);

            _asPointer = false;
            if (Error.error)
            {
                return;
            }
            if (Match(TokenEqual))
            {
                next();
                parseExpr();
                if (Error.error)
                {
                    return;
                }

                if (!Match(TokenSemicolon)) //&& !Match(TokenCloseParenthesis))
                {
                    Error.error = 1;
                    Error.error_message = string_format("Expected ici ; %s", linepos().c_str());
                    // next();
                    return;
                }
                // current_node->addChild(left._nd);
                // current_node->addChild(right._nd);
                Error.error = 0;
                // result._nd = nd;
                // current_node=current_node->parent;
                current_node = current_node->parent;
                next();
                return;
            }
            else
            {
                Error.error = 1;
                Error.error_message = string_format("Expected = %s", linepos().c_str());
                return;
            }
        }

        else if (Match(TokenKeywordElse))
        {
            // on tente le for(){}
            // token *fort = current();
            // Context cntx;
            // cntx.name = current()->text;
            // //printf("entering f %d %s %s %x\n", current_cntx->_global->children.size(), current_cntx->_global->name.c_str(), current()->text.c_str(), (uint64_t)current_cntx->_global);
            // current_cntx = (*(current_cntx)).addChild(cntx);
            current_cntx = current_cntx->addChild(Context(current()->text));
            // current_cntx = k;
            //  string target =string_format("label_%d%s",for_if_num,k->name.c_str());
            targetList.push(string_format("label_%d%s", for_if_num, current_cntx->name.c_str()));
            //=target;
            for_if_num++;

            // NodeElse ndf = NodeElse(fort);
            // ndf.target = targetList.pop();
            // current_node = current_node->addChild(ndf);
            current_node = current_node->addChild(NodeElse(current(), targetList.pop()));
            next();

            parseBlockStatement();
            if (Error.error)
            {
                return;
            }

            // current_node->target=target;

            // resParse result;

            Error.error = 0;
            // result._nd = ndf;
            // next();
            current_cntx = current_cntx->parent;
            current_node = current_node->parent;
            //  current_node=current_node->parent;
            return;
        }
        else if (Match(TokenKeywordWhile))
        {
            // on tente le for(){}
            sav_t.push_back(current());
            // Context cntx;
            // cntx.name = current()->text;
            //  //printf("entering f %d %s %s %x\n", current_cntx->_global->children.size(), current_cntx->_global->name.c_str(), current()->text.c_str(), (uint64_t)current_cntx->_global);
            // Context *k = (*(current_cntx)).addChild(cntx);
            // current_cntx = (*(current_cntx)).addChild(cntx);;
            current_cntx = current_cntx->addChild(Context(current()->text));
            targetList.push(string_format("label_%d%s", for_if_num, current_cntx->name.c_str()));
            //=target;
            for_if_num++;
            next();
            if (Match(TokenOpenParenthesis))
            {
                // NodeWhile ndf = NodeWhile(fort);
                // ndf.target = target;
                // current_node = current_node->addChild(ndf);
                current_node = current_node->addChild(NodeWhile(sav_t.back(), targetList.get()));
                next();

                // printf(" *************** on parse comp/n");
                parseComparaison(targetList.get());
                if (Error.error)
                {
                    return;
                }
                targetList.pop();
                ////printf("on a parse %s\n",comparator._nd._token->text.c_str());
                // printf(" *************** on parse inc/n");

                parseBlockStatement();
                if (Error.error)
                {
                    return;
                }

                // current_node->target=target;

                // resParse result;

                Error.error = 0;
                // result._nd = ndf;
                // next();
                current_cntx = current_cntx->parent;
                current_node = current_node->parent;
                sav_t.pop_back();
                //  current_node=current_node->parent;
                return;
            }
            else
            {
                // resParse res;
                Error.error = 1;
                Error.error_message = string_format("expecting ( %s", linepos().c_str());
                next();
                return;
            }
        }
        else if (Match(TokenKeywordIf))
        {
            // on tente le for(){}
            // token *fort=current();
            // Context cntx;
            // cntx.name = current()->text;
            // //printf("entering f %d %s %s %x\n", current_cntx->_global->children.size(), current_cntx->_global->name.c_str(), current()->text.c_str(), (uint64_t)current_cntx->_global);
            // Context *k = (*(current_cntx)).addChild(cntx);
            current_cntx = current_cntx->addChild(Context(current()->text));
            // string target =string_format("label_%d%s",for_if_num,k->name.c_str());
            targetList.push(string_format("label_%d%s", for_if_num, current_cntx->name.c_str()));
            //=target;
            for_if_num++;
            // next();
            if (Match(TokenOpenParenthesis, 1))
            {
                // NodeIf ndf = NodeIf(current());
                // ndf.target = targetList.get();

                // current_node = current_node->addChild(ndf);
                current_node = current_node->addChild(NodeIf(current(), targetList.get()));
                next();
                next();

                // printf(" *************** on parse comp/n");
                parseComparaison(targetList.pop());
                if (Error.error)
                {
                    return;
                }
                ////printf("on a parse %s\n",comparator._nd._token->text.c_str());
                // printf(" *************** on parse inc/n");

                parseBlockStatement();
                if (Error.error)
                {
                    return;
                }

                // current_node->target=target;

                // resParse result;

                Error.error = 0;
                // result._nd = ndf;
                // next();
                current_cntx = current_cntx->parent;
                current_node = current_node->parent;
                //  current_node=current_node->parent;
                return;
            }
            else
            {
                // resParse res;
                Error.error = 1;
                Error.error_message = string_format("expecting ( %s", linepos().c_str());
                next();
                return;
            }
        }
        else if (Match(TokenKeywordFor))
        {
            // on tente le for(){}
            // token *fort=current();
            // Context cntx;
            // cntx.name = current()->text;
            // //printf("entering f %d %s %s %x\n", current_cntx->_global->children.size(), current_cntx->_global->name.c_str(), current()->text.c_str(), (uint64_t)current_cntx->_global);
            // current_cntx = current_cntx->addChild(cntx);
            current_cntx = current_cntx->addChild(Context(current()->text));
            // current_cntx = k;
            // string target =string_format("label_%d%s",for_if_num,current_cntx->name.c_str());
            targetList.push(string_format("label_%d%s", for_if_num, current_cntx->name.c_str()));
            //=target;
            for_if_num++;
            // next();
            if (Match(TokenOpenParenthesis, 1))
            {
                // NodeFor ndf = NodeFor(current());
                // ndf.target = targetList.get();
                next();
                // current_node = current_node->addChild(ndf);
                current_node = current_node->addChild(NodeFor(current(), targetList.get()));
                next();
                current_node = current_node->addChild(NodeStatement());
                parseStatement();
                if (Error.error)
                {
                    return;
                }
                current_node = current_node->parent;
                // printf(" *************** on parse comp/n");
                parseComparaison(targetList.pop());
                if (Error.error)
                {
                    return;
                }
                ////printf("on a parse %s\n",comparator._nd._token->text.c_str());
                // printf(" *************** on parse inc/n");
                parseStatement();
                if (Error.error)
                {
                    return;
                }

                parseBlockStatement();
                if (Error.error)
                {
                    return;
                }

                // current_node->target=target;

                // resParse result;

                Error.error = 0;
                // result._nd = ndf;
                // next();
                current_cntx = current_cntx->parent;
                current_node = current_node->parent;
                //  current_node=current_node->parent;
                return;
            }
            else
            {
                // resParse res;
                Error.error = 1;
                Error.error_message = string_format("expecting ( %s", linepos().c_str());
                next();
                return;
            }
        }

        else if (Match(TokenKeywordVarType))
        {
            // printf("trying to create %s\n", current()->text.c_str());
            parseType();
            if (Error.error)
            {
                Error.error = 1;

                return;
            }

            parseVariableForCreation();
            if (Error.error)
            {

                return;
            }
            // NodeToken nd = nodeTokenList.pop();
            // NodeToken f = nodeTokenList.pop();
            // auto var = createNodeLocalVariableForCreation(&nd, &f);
            nodeTokenList.push(createNodeLocalVariableForCreation(nodeTokenList.pop(), nodeTokenList.pop()));
            // printf("111&&&&&&&dddddddddd&&&&qssdqsdqsd& %s\n", nodeTypeNames[var._nodetype].c_str());
            // string var_name = nd._token->text;
            // pritnf()
            current_cntx->addVariable(nodeTokenList.get());

            if (Match(TokenSemicolon))
            {
                Error.error = 0;
                // result._nd = var;
                current_node->addChild(nodeTokenList.pop());
                // current_node = current_node->parent;
                next();
                return;
            }

            if (Match(TokenEqual))
            {
                //  NodeStatement ndsmt;
                current_node->addChild(nodeTokenList.get());
                // NodeAssignement nd;
                current_node = current_node->addChild(NodeAssignement());
                next();
                // auto left = createNodeLocalVariableForStore(var);
                // copyPrty(type._nd, &var);
                // current_node->addChild(left);
                current_node->addChild(createNodeLocalVariableForStore(nodeTokenList.pop()));
                parseExpr();

                if (Error.error)
                {

                    return;
                }

                if (!Match(TokenSemicolon))
                {
                    Error.error = 1;
                    Error.error_message = string_format("Expected ; %s", linepos().c_str());
                    next();
                    return;
                }
                // nd.addChild(left);
                // nd.addChild(right._nd);
                // ndsmt.addChild(nd);
                Error.error = 0;
                // result._nd = ndsmt;
                // current_node->addChild(ndsmt);
                current_node = current_node->parent;
                next();
                return;
            }
            else
            {
                Error.error = 1;
                Error.error_message = string_format(" expecting ;  ou = %s", linepos().c_str());
                return;
            }

            return;
        }
        else
        {
            Error.error = 1;
            Error.error_message = string_format(" Unexpected %s  %s", current()->text.c_str(), linepos().c_str());
            return;
        }
    }

    void parseBlockStatement()
    {
        // resParse result;

        // NodeBlockStatement nbStmnt;
        // Context cntx;
        // cntx.name = string_format("%d", block_statement_num);
        // current_cntx = current_cntx->addChild(cntx);
        current_cntx = current_cntx->addChild(Context(string_format("%d", block_statement_num)));
        block_statement_num++;

        current_node = current_node->addChild(NodeBlockStatement(current()));
        next();
        while (!Match(TokenCloseCurlyBracket) && !Match(TokenEndOfFile))
        {
            // printf("on tente aouter un stamt\n");
            parseStatement();
            if (Error.error)
            {
                return;
            }
        }
        if (Match(TokenEndOfFile))
        {
            Error.error = 1;
            Error.error_message = string_format("expected } %s", linepos().c_str());
            next();
            return;
        }
        Error.error = 0;
        // result._nd = nbStmnt;
        current_cntx = current_cntx->parent;
        current_node = current_node->parent;
        next();
        return;
    }

    void parseDefFunction(NodeToken oritype)
    {
        Error.error = 0;
        bool ext_function = false;
        bool is_asm = false;
        //printf("entering function %s\r\n",current()->text.c_str());
        if (isExternal)
        {
            ext_function = true;
            isExternal = false;
        }
        if (isASM)
        {
            isASM = false;
            is_asm = true;
        }
        // resParse result;
        token *func = current();

        main_cntx.findFunction(current());
        if (search_result != NULL) // if (current_cntx->findFunction(current()) != NULL)
        {

            Error.error = 1;
            Error.error_message = string_format("function already declared in the scope for %s", linepos().c_str());
            next();
            return;
        }
        if (ext_function)
        {
            NodeDefExtFunction function = NodeDefExtFunction(func);
            function.addChild(oritype);
            //  function.addChild(arguments._nd);

            current_node = current_node->addChild(function);
            // current_cntx->parent->addFunction(current_node);
            main_cntx.addFunction(current_node);
        }
        else if (is_asm)
        {
            NodeDefAsmFunction function = NodeDefAsmFunction(func);
            function.addChild(oritype);
            //  function.addChild(arguments._nd);

            current_node = current_node->addChild(function);
            // current_cntx->parent->addFunction(current_node);
            main_cntx.addFunction(current_node);
        }
        else
        {
            NodeDefFunction function = NodeDefFunction(func);
            function.addChild(oritype);
            //  function.addChild(arguments._nd);

            current_node = current_node->addChild(function);
            // current_cntx->parent->addFunction(current_node);
            main_cntx.addFunction(current_node);
        }
        // on ajoute un nouveau contexte
        Context cntx;
        cntx.name = current()->text;
        Context *k = current_cntx->addChild(cntx);
        current_cntx = k;
        stack_size = _STACK_SIZE;
        block_statement_num = 0;
        next();
        next();
        parseCreateArguments();
        if (Error.error)
        {
            return;
        }
        if (!Match(TokenCloseParenthesis))
        {
            Error.error = 1;
            Error.error_message = string_format("Expected ) %s", linepos().c_str());
            next();
            return;
        }
        next();
        if (ext_function)
        {
            if (Match(TokenSemicolon))
            {
                //   NodeDefExtFunction function = NodeDefExtFunction(func);
                //  function.addChild(oritype);
                //  function.addChild(arguments._nd);
                // current_cntx->parent->addFunction(function);
                // current_node->addChild(function);
                // result._nd = function;
                Error.error = 0;
                current_cntx = current_cntx->parent;
                current_node = current_node->parent;
                next();
                return;
            }
            else
            {
                Error.error = 1;
                Error.error_message = string_format("Impossible to redifine external function expected ; %s", linepos().c_str());
                next();
                return;
            }
        }
        else
        {
            if (Match(TokenOpenCurlyBracket))
            {
                // NodeDefFunction function = NodeDefFunction(func);
                // function.addChild(oritype);
                // function.addChild(arguments._nd);
                // current_cntx->parent->addFunction(function);
                // current_node = current_node->addChild(function);

                parseBlockStatement();
                if (Error.error)
                {
                    return;
                }

                // current_node->addChild(blocsmt._nd);
                // current_node = current_node->parent;
                current_node->stack_pos = stack_size;
                // result._nd = function;
                Error.error = 0;
                current_cntx = current_cntx->parent;
                 point_regnum=4;
                 //printf("on visit la function %s %d\r\n",current_node->_token->text.c_str(),_tks.position);
                 __sav_pos=_tks.position;
                  buildParents(current_node);
                  __current=current();
                current_node->visitNode(current_node);
                //printf("on a visité\r\n");

                current_node = current_node->parent;
               _tks.position=__sav_pos;
               
                return;
            }
            else
            {
                Error.error = 1;
                Error.error_message = string_format("Expected { %s", linepos().c_str());
                next();
                return;
            }
        }
 
        return;
    }

    void parseType()
    {

        // resParse res;
        NodeToken _nd;
        if (Match(TokenExternal))
        {
            isExternal = true;
            next();
        }
        else if (Match(TokenKeywordASM))
        {
            isASM = true;
            next();
        }

        if (Match(TokenKeywordVarType))
        {
            _nd._nodetype = typeNode;
            _nd._token = current();

            next();
            if (Match(TokenStar))
            {
                _nd.isPointer = true;
                next();
            }
            else
            {
                _nd.isPointer = false;
            }
        }
        else
        {
            Error.error = 1;
            Error.error_message = string_format("expecting external, __ASM__  or variable type %s", linepos().c_str());
            next();
            return;
        }
        Error.error = 0;
        nodeTokenList.push(_nd);
        return;
    }

    void parseVariableForCreation()
    {
        current_cntx->findVariable(current(), true);
        if (search_result != NULL)
        {
            // resParse res;
            Error.error = 1;
            Error.error_message = string_format("variable %s already declared in the scope for %s", current()->text.c_str(), linepos().c_str());
            next();
            return;
        }
        if (Match(TokenOpenBracket, 1))
        {
            // we are in the case led[];

            NodeToken var = NodeToken(current());
            next();
            next();
            if (Match(TokenNumber))
            {
                token *num = current();
                if (num->_vartype->_varType == __uint16_t__)
                {
                    next();
                    if (Match(TokenCloseBracket))
                    {
                        var.isPointer = true;
                        var._nodetype = defGlobalVariableNode; // we can't have arrays in the stack
                        var._total_size = stringToInt(num->text);
                        next();
                        // resParse result;
                        Error.error = 0;
                        nodeTokenList.push(var);
                        return;
                    }
                    else
                    {
                        // resParse res;
                        Error.error = 1;
                        Error.error_message = string_format("expecting ] %s", linepos().c_str());
                        next();
                        return;
                    }
                }
                else
                {
                    // resParse res;
                    Error.error = 1;
                    Error.error_message = string_format("expecting an integer %s", linepos().c_str());
                    next();
                    return;
                }
            }
            else if(Match(TokenCloseBracket))
            {
                 var.isPointer = true;
                        var._nodetype = defGlobalVariableNode; // we can't have arrays in the stack
                       // var._total_size = stringToInt(num->text);
                        next();
                        // resParse result;
                        Error.error = 0;
                        nodeTokenList.push(var);
                        return;
            }
            else
            {
                    Error.error = 1;
                    Error.error_message = string_format("expecting an integer %s or ]", linepos().c_str());
                    next();
                    return;
            }
        }
        else
        {
            NodeToken nd = NodeToken(current());
            // resParse result;
            // result._nd = nd;
            Error.error = 0;
            nodeTokenList.push(nd);
            next();
            return;
        }
    }

    void parseProgram()
    {

        // NodeProgram program;
        // Context cntx = Context();
        current_cntx->name = "main";
        current_node = &program;
        // current_cntx = &cntx;

        // resParse result;
        Error.error = 0;
        while (Match(TokenEndOfFile) == false)
        {
            if (Match(TokenKeywordSafeMode))
            {
                safeMode = true;
                next();
            }
            else if (Match(TokenKeywordSaveReg))
            {
                saveReg = true;
                next();
            }
             else if (Match(TokenKeywordSaveRegAbs))
            {
                saveRegAbs = true;
                next();
            }
            else if (Match(TokenKeywordImport))
            {
                next();
                if (Match(TokenIdentifier))
                {
                    sav_t.push_back(current());
                    next();
                    if (Match(TokenKeywordFrom))
                    {
                        if (Match(TokenIdentifier))
                        {
                            next();
                        }
                        else
                        {
                            Error.error = 1;
                            Error.error_message = string_format("expected identifier at %s", linepos().c_str());
                            next();
                            return;
                        }
                    }
                    else
                    {
                        if (findLibFunction(sav_t.back()->text) > -1)
                        {
                            Error.error = 0;
                            // current_node->addChild(NodeImport(sav_t.back(),findLibFunction(sav_t.back()->text)));
                            // add_on.push_back(findLibFunction(sav_t.back()->text));
                            sav_t.pop_back();
                            // next();
                        }
                        else
                        {
                            Error.error = 1;
                            Error.error_message = string_format("Import nor found at %s", linepos().c_str());
                            next();
                            return;
                        }
                    }
                }
                else
                {
                    Error.error = 1;
                    Error.error_message = string_format("expected identifier at %s", linepos().c_str());
                    next();
                    return;
                }
            }
            else
            {
                parseType();
                if (Error.error)
                {

                    return;
                }

                if (Match(TokenIdentifier))
                {
                    if (Match(TokenOpenParenthesis, 1))
                    {
                        parseDefFunction(nodeTokenList.get());
                        if (Error.error)
                        {
                            return;
                        }
                        // program.addChild(res._nd);
                    }
                    else
                    {

                        parseVariableForCreation();
                        if (Error.error)
                        {

                            return;
                        }
                        NodeToken nd = nodeTokenList.pop();
                        if (isExternal)
                        {

                            NodeDefExtGlobalVariable var = NodeDefExtGlobalVariable(nd);
                            NodeToken t = nodeTokenList.pop();
                            copyPrty(&t, &var);
                            current_node = program.addChild(var);
                            current_cntx->addVariable(var);
                            isExternal = false;
                        }
                        else
                        {
                            NodeDefGlobalVariable var = NodeDefGlobalVariable(nd);
                            NodeToken t = nodeTokenList.pop();
                            copyPrty(&t, &var);
                            current_node = program.addChild(var);
                            current_cntx->addVariable(var);
                        }
                        if (Match(TokenSemicolon))
                        {
                            current_node = current_node->parent;
                            next();
                        }
                        else if(Match(TokenEqual) && Match(TokenString,1))
                        {
                            next();
                            current_node->addChild(NodeString(current()));
                            next();
                             if (!Match(TokenSemicolon))
                            {

                                Error.error = 1;
                                Error.error_message = string_format("Expected ; %s", linepos().c_str());
                                next();
                                return;
                            }
                            next();
                            Error.error = 0;

                            current_node = current_node->parent;
                        }
                        else if (Match(TokenEqual) && Match(TokenOpenCurlyBracket, 1))
                        {
                            next();
                            next();
                            parseFactor();
                            if (Error.error)
                            {
                                return;
                            }
                            while (Match(TokenComma))
                            {
                                next();
                                parseFactor();
                                if (Error.error)
                                {
                                    return;
                                }
                            }
                            if (!Match(TokenCloseCurlyBracket))
                            {

                                Error.error = 1;
                                Error.error_message = string_format("Expected ) %s", linepos().c_str());
                                next();
                                return;
                            }
                            next();
                            if (!Match(TokenSemicolon))
                            {

                                Error.error = 1;
                                Error.error_message = string_format("Expected ; %s", linepos().c_str());
                                next();
                                return;
                            }
                            next();
                            Error.error = 0;

                            current_node = current_node->parent;
                        }
                        else
                        {
                            Error.error = 1;
                            Error.error_message = string_format(" expecting ;  ou = %s", linepos().c_str());
                            return;
                        }
                    }
                }
                else
                {
                    Error.error = 1;
                    Error.error_message = string_format("expected identifier %s", linepos().c_str());
                    next();
                    return;
                }
            }
        }
        // result._nd = program;
        Error.error = 0;
        // printf("exit programm\n");
        return;
    }
    /*
        void setPrekill(void (*function)(), void (*function2)())
        {
            prekill = function;
            postkill = function2;
        }*/
    /*
        bool run()
        {
            if (exeExist == true)
                {
                    // _push(termColor.Cyan);
                    // //Serial.printf(config.ENDLINE);
                    // Serial.print("Executing ...\r\n");
                    _exe_args df;
                    df.args = args;
                    df.exe = executecmd;
                    // executeBinary("main",executecmd);
                    xTaskCreateUniversal(_run_task, "_run_task", 4096, &df, CONFIG_ARDUINO_UDP_TASK_PRIORITY, (TaskHandle_t *)&__run_handle, 0);

                    // xTaskCreate(_udp_task_subrarnet, "_udp_task_subrarnet", 4096, &df, CONFIG_ARDUINO_UDP_TASK_PRIORITY, (TaskHandle_t *)&_udp_task_handle);

                    // delay(10);
                    return true;
                    // //Serial.printf(config.ESC_RESET);
                }
                else
                {
                    return false;
                }
        }*/
    // void (*prekill)() = NULL;
    // void (*postkill)() = NULL;

private:
    Tokens _tks;
};
#ifdef __CONSOLE_ESP32
/*
static volatile TaskHandle_t __run_handle = NULL;
executable executecmd;
// string strcompile;
bool exeExist;
typedef struct
{
    vector<string> args;
    executable exe;
} _exe_args;

static void _run_task(void *pvParameters)
{

    _exe_args *_fg = ((_exe_args *)pvParameters);
    if (_fg->args.size() > 0)
    {
        executeBinary(_fg->args[0], _fg->exe);
    }
    else
    {
        executeBinary("main", _fg->exe);
    }
    LedOS.pushToConsole("Execution done.", true, true);
    __run_handle = NULL;
    vTaskDelete(NULL);
}*/
Parser p = Parser();
// Executable consExecutable = Executable();

void kill(Console *cons, vector<string> args)
{
    if (__run_handle != NULL)
    {

        SCExecutable._kill();
    }
    else
    {
        LedOS.pushToConsole("Nothing is currently running.", true);
    }
}
void run(Console *cons, vector<string> args)
{
    if (__run_handle != NULL)
    {
        LedOS.pushToConsole("Something Already running kill it first ...");
        kill(cons, args);
    }
SCExecutable.executeAsTask("main");
    //SCExecutable._run(args, true);
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
    bool othercore = false;
    executecmd = createExectutable(&cons->script, true);
    // strcompile = "";
    p.clean2();
    // //Serial.printf(config.ESC_RESET);

    if (executecmd.error.error == 0)
    {

        exeExist = true;
        if (othercore)
        {
            vector<string> d;
            d.push_back("main");
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
            LedOS.pushToConsole("***********START RUN*********");
            LedOS.pushToConsole("Execution asm ...", true);
            executeBinary("main", executecmd);
            LedOS.pushToConsole("Execution done.", true, true);
        }
    }
    else
    {
        exeExist = false;
        // Serial.printf(termColor.Red);
        LedOS.pushToConsole(executecmd.error.error_message.c_str());
        // Serial.printf(config.ESC_RESET);
    }
}
void parse_c(Console *cons, vector<string> args)
{
    if (__run_handle != NULL)
    {
        LedOS.pushToConsole("Something Already running kill it first ...");
        kill(cons, args);
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

    if (p.parse_c(&cons->script))
    {

        exeExist = true;
        if (othercore)
        {
            vector<string> d;
            d.push_back("main");
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

class __INIT_PARSER
{
public:
    __INIT_PARSER()
    {
        __run_handle = NULL;
        LedOS.addKeywordCommand("compile", parse_c, "Compile and run a program add '&' for run on the second core");
        LedOS.addKeywordCommand("run", run, "Run an already compiled program (always second Core)");
        LedOS.addKeywordCommand("kill", kill, "Stop a running program");
        LedOS.addKeywordCommand("parseasm", parseasm, "Parse assembly program");
        LedOS.addEscCommand(18, parsec_cEsc, "Compile and execute a program (always second Core)");
        LedOS.addEscCommand(11, kill_cEsc, "Stop a running program");
        addExternal("__feed", externalType::function, (void *)feedTheDog);
        LedOS.script.clear();
        LedOS.script.push_back("stack:");
        LedOS.script.push_back(".bytes 120");
        LedOS.script.push_back(".global main");
        LedOS.script.push_back("main:");
        LedOS.script.push_back("entry a1,72");
        for (int i = 1; i < 400; i++)
        {
            LedOS.script.push_back("nop");
        }
        LedOS.script.push_back("retw.n");
    }
};
__INIT_PARSER _init_parser;
#endif
