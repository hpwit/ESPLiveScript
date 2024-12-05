#ifndef __ESP_LIVE_SCRIPT
#define __ESP_LIVE_SCRIPT
#pragma once

#include <string>
#include <variant>
#include <vector>
using namespace std;

#ifndef _TRIGGER
#define _TRIGGER 0
#endif
/*
#ifndef __TEST_DEBUG
using _arguments = std::variant<int, float>;
typedef std::vector<_arguments> Arguments;
#else
typedef int Arguments;
#endif
*/

// #include "tokenizer.h"

// #include "asm_parser.h"

#ifndef __TEST_DEBUG
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
// #include "soc/rtc_wdt.h"
#endif

#include "NodeToken.h"

class _arguments
{
public:
    _arguments()
    {
        vartype = __unknown__;
    }
    _arguments(int val)
    {
        vartype = __int__;
        intval = val;
    }
    _arguments(float val)
    {
        vartype = __float__;
        floatval = val;
    }

    varTypeEnum vartype;
    int intval;
    float floatval;
};

class Arguments
{
public:
    Arguments() {}
    void add(int val)
    {
        _args.push_back(_arguments(val));
    }
    void add(float val)
    {
        _args.push_back(_arguments(val));
    }
    void clear()
    {
        _args.clear();
        _args.shrink_to_fit();
    }
    void add(_arguments a)
    {
        _args.push_back(a);
    }
    int size()
    {
        return _args.size();
    }
    vector<_arguments> _args;
};
#include "asm_parser.h"
#include "execute.h"
void prettyPrint(NodeToken *_nd, string ident)
{
    NodeToken nd = *_nd;
    if (_nd == NULL)
    {
        printf("mlqskdlmqskdlomqksdlmfdkqsdlmfk");
    }
    // printf("%s%d\n", ident.c_str(), 1);

    printf("%s%s\tisPointer:%d\tasPointer:%d\t", ident.c_str(), nodeTypeNames[nd._nodetype].c_str(), nd.isPointer, nd.asPointer); //, tokenNames[nd._token.type].c_str());

    printf("text:%s\ttokenType:%s\t", nd.getTokenText(), tokenNames[nd.type].c_str());
    // printf("\n");
    if (nd.getVarType() != NULL)
    {

        if (nd.type == (int)TokenUserDefinedVariable)
            printf("var name:%s\t total size:%d\tstackpos:%d\t", _userDefinedTypes[nd._vartype].varName.c_str(), nd._total_size, nd.stack_pos);
        else

            printf("var type:%s\ttotal size:%d\tstackpos:%d\t", varTypeEnumNames[nd._vartype].c_str(), nd._total_size, nd.stack_pos);
    }

    printf("target :%s", nd.getTargetText());
    printf("\n");

    ident += "|--";

    int i = 0;
    for (NodeToken *t : _nd->children)
    {
        //  printf("child:%d\n",i);
        prettyPrint(t, ident);
        i++;
        // printf("on finit child:%d\n",i);
    }
    // printf("we go back\n");
}

void prettyPrint2(NodeToken nd, string ident)
{
    /*
        printf("%s%s\t isPointer:%d\t asPointer:%d\t", ident.c_str(), nodeTypeNames[nd._nodetype].c_str(), nd.isPointer, nd.asPointer); //, tokenNames[nd._token.type].c_str());

        printf("%s\t%s ", tokenNames[nd.type].c_str(), nd.getTokenText());
        // printf("\n");
        if (nd.getVarType() != NULL)
        {

            printf("%s total size:%d stackpos:%d\n", varTypeEnumNames[nd.getVarType()->_varType].c_str(), nd._total_size, nd.stack_pos);
        }
        else
        {
            printf("\n");
        }

        ident += "|--";
        // printf("nb chilrend\t\t%d\n",nd.children.size());
        // if(nd.children!=NULL)
        //{
        for (NodeToken *t : nd.children)
        {
            // printf("child:%d\n",i);
            prettyPrint2(*t, ident);
            //  printf("on finit child:%d\n",i);
        }
        // }
        // printf("we go back\n");
        */
}

class Parser
{

public:
    NodeToken root;
    Parser() {}

    string linepos()
    {
        string f = string_format(" at line:%d position:%d", current()->line, current()->pos);
        Error.line=current()->line;
        Error.pos=current()->pos;
        return f;
    }
    int size()
    {
        return _tks.size();
    }
    Token *getTokenAtPos(int pos)
    {
        return _tks.getTokenAtPos(pos);
    }
    Token *current()
    {
        return _tks.current();
    }
    Token *next()
    {
        return _tks.next();
    }
    Token *prev()
    {
        return _tks.prev();
    }
    Token *peek(int index)
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

    void parse()
    {

        point_regnum = 4;
        point_regnum = 4;
        stack_size = 0;
        for_if_num = 0;
        block_statement_num = 0;
        local_var_num = 0;
        nb_argument = 0;

        safeMode = false;
        saveReg = false;
        saveRegAbs = false;
        _asPointer = false;
        isPointer = false;
        isASM = false;
        current_node = &program;

        _userDefinedTypes.clear();
        nodeTokenList.clear();
        program.clearAll();
        sav_t.clear();
        sav_t.shrink_to_fit();
        main_context.clear();
        targetList.clear();
        sav_token.clear();
        _node_token_stack.clear();
        _functions.clear();
        _functions.shrink_to_fit();
        all_text.clear();
        content.clear();
        header.clear();

        main_script.init();
        initMem();
        _tks.tokenize(&main_script, true, true, 1);

        parseProgram();
    }

    Executable compile()
    {
        Executable results;
        _sav_token_line = 1;
        parse();
        if (Error.error)
        {
           // pushToConsole(Error.error_message.c_str(), true);
            results.error=Error;
            return results;
        }
        pushToConsole("***********PARSING DONE*********");
        updateMem();
        buildParents(&program);
#ifdef __TEST_DEBUG
        prettyPrint(&program, "");
#endif
        program.visitNode();
        pushToConsole("***********COMPILING DONE*********");
        updateMem();
        displayStat();
        main_script.clear();
        _userDefinedTypes.clear();
        nodeTokenList.clear();
        program.clearAll();
        sav_t.clear();
        sav_t.shrink_to_fit();
        main_context.clear();
        targetList.clear();
        change_type.clear();
        sav_token.clear();
        _node_token_stack.clear();
        for (NodeToken h : _functions)
        {
            h.clearAll();
        }
        _functions.clear();
        _functions.shrink_to_fit();
        all_text.clear();
        all_targets.clear();

        updateMem();
        displayStat();

        pushToConsole("***********AFTER CLEAN*********");

#ifndef __TEST_DEBUG
        pushToConsole("***********CREATE EXECUTABLE*********");
        executable _executecmd = createExectutable(&header, &content, __parser_debug);
        results.setExecutable(_executecmd);
        results.error=_executecmd.error;
        content.clear();
        header.clear();
        change_type.clear();
        updateMem();
        displayStat();
        if (_executecmd.error.error == 1)
        {
            // exeExist = false;
            // Serial.printf(termColor.Red);

           // pushToConsole(_executecmd.error.error_message.c_str(), true);
        }

#endif
        return results;
    }

    Executable parseScript(string *str)
    {

        main_script.clear();
        main_script.addContent((char *)_sync.c_str());
        main_script.addContent((char *)division.c_str());
        main_script.addContent((char *)base_ext_functions.c_str());
        main_script.addContent((char *)str->c_str());
        return compile();
    }
    Executable parse_c(list<string> *_script)
    {
        main_script.clear();
        main_script.addContent((char *)_sync.c_str());
        main_script.addContent((char *)division.c_str());
        main_script.addContent((char *)base_ext_functions.c_str());
        string sc = "";
        for (string s : *_script)
        {
            sc = sc + "\n" + s;
        }
        main_script.addContent((char *)sc.c_str());
        return compile();
    }
    void getVariable(bool isStore)
    {

        current_cntx->findVariable(current(), false); // false
        // NodeToken *nd = search_result;
        if (search_result == NULL)
        {
            // //printf("hhheeheh\n");

            Error.error = 1;
            Error.error_message = string_format("impossible to find declaraiton for %s %s", current()->getText(), linepos().c_str());
            next();
            return;
        }
        else
        {
            // token *vartoken = current();
            // auto var =
            // current_node = current_node->addChild(
            createNodeVariable(current(), isStore);

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

                    // Error.error = 0;
                    // current_node = current_node->parent;

                    next();
                    // return;
                }
                else if (Match(TokenComma))
                {
                    while (Match(TokenComma))
                    {
                        next();

                        // nb_argument++;
                        parseExpr();
                        if (Error.error)
                        {
                            return;
                        }
                        // arg.addChild(res._nd);
                    }
                    if (Match(TokenCloseBracket))
                    {

                        // var.addChild(expr._nd);
                        // resParse res;
                        // Error.error = 0;
                        //  current_node = current_node->parent;
                        // res._nd = var;
                        next();
                        // return;
                    }
                    else
                    {

                        Error.error = 1;
                        Error.error_message = string_format("expecting ]  or , %s", current()->getText());
                        next();
                        return;
                    }
                }
                else
                {

                    Error.error = 1;
                    Error.error_message = string_format("expecting ]  or , %s", current()->getText());
                    next();
                    return;
                }
            }
            // else
            // {

            if (Match(TokenMember) && Match(TokenIdentifier, 1) && !Match(TokenOpenParenthesis, 2))
            {
                next();
                int i = 0;
                varType *v = NULL;

                if (current_node->_vartype == __CRGB__ or current_node->_vartype == __CRGBW__)
                {
                    i = findMember(current_node->getVarType(), string(current()->getText()));
                    v = current_node->getVarType();
                    if (i < 0)
                    {
                        Error.error = 1;
                        Error.error_message = string_format("Member %s of struct %s does not exists", current()->getText(), v->varName.c_str());
                        next();
                        return;
                    }
                }
                else
                {
                    i = findMember(current_node->_vartype, string(current()->getText()));
                    //  int pos = 0;
                    v = &_userDefinedTypes[current_node->_vartype];

                    if (i < 0)
                    {
                        Error.error = 1;
                        Error.error_message = string_format("Member %s of struct %s does not exists", current()->getText(), v->varName.c_str());
                        next();
                        return;
                    }
                }
                // next();
                // current_node->addTargetText(string(current()->getText()));
                current_node->type = TokenUserDefinedVariableMember;
                current_node->_vartype = v->types[i];
                if (!_asPointer)
                    current_node->stack_pos = current_node->stack_pos + v->starts[i];
                else
                    current_node->stack_pos = current_node->stack_pos + 1000 * v->starts[i];
                if (current_node->isPointer)
                {
                    // current_node->_total_size =1000* current_node->_total_size +v->sizes[i];
                }
                else
                {
                    current_node->_total_size = v->sizes[i];
                }
                next();
            }
            else if (Match(TokenMember, 0) && Match(TokenIdentifier, 1) && Match(TokenOpenParenthesis, 2))
            {

                current_cntx->findVariable(current_node->getTokenText(), false);
                if (search_result == NULL)
                {

                    Error.error = 1;
                    Error.error_message = string_format("impossible to find declaraiton for %s %s", current()->getText(), linepos().c_str());
                    next();
                    return;
                }
                // next();
                next();
                /*
                NodeToken *par=current_node->parent;
                current_node->parent->children.pop_back();
                current_node=par;
                */
                // par=current_node->parent;
                // current_node->parent->children.pop_back();
                // current_node=par;
                current()->addText(string_format("%s.%s", search_result->getVarType()->varName.c_str(), current()->getText()));
                NodeToken nd = *search_result;
                nd._nodetype = globalVariableNode;
                nd.type = TokenUserDefinedVariableMemberFunction;
                nd.isPointer = true;
                nd._total_size = search_result->getVarType()->total_size; // nd.copyChildren(current_node);
                                                                          /*
                                                                         for(int i=0;i<current_node->children.size();i++)
                                                                         {
                                                                             nd.addChild(*current_node->getChildAtPos(i));
                                                                         }
                                                          
                                                                                          NodeToken *par=current_node->parent;
                                                                         current_node->parent->children.pop_back();
                                                                         current_node=par;
                                                                         */
                current_node->_nodetype = UnknownNode;
                NodeToken *par = current_node;
                current_node = current_node->parent;

                nodeTokenList.push(nd);
                isStructFunction = true;

                parseFunctionCall();

                if (Error.error)
                {
                    return;
                }
                current_node->getChildAtPos(current_node->children.size() - 1)->getChildAtPos(2)->getChildAtPos(0)->copyChildren(par);
                isStructFunction = false;
                Error.error = 0;
                return;
            }
            Error.error = 0;
            tmp_sav = current_node;
            current_node = current_node->parent;
            // res._nd = var;
            // next();
            return;

            //}
        }
    }
    void parseArguments()
    {
        // resParse result;
        nb_argument = 0;
        nb_args.push_back(0);
        // NodeInputArguments arg;
        current_node = current_node->addChild(NodeToken(inputArgumentsNode));
        if (isStructFunction)
        {
            if (struct_name.size() == 0)
            {
                current_node->addChild(nodeTokenList.pop());
                nb_args.pop_back();
                nb_args.push_back(1);
            }
            else
            {
                NodeToken nd = NodeToken();
                nd.setTargetText("pointer");
                nd.isPointer = true;
                nd.type = TokenUserDefinedVariable;
                nd._nodetype = localVariableNode;
                nd.stack_pos = _STACK_SIZE;
                current_node->addChild(nd);
                nb_args.pop_back();
                nb_args.push_back(1);
            }
        }
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
        if (isStructFunction)
        {
            nb_args.push_back(2);
        }
        else
        {
            nb_args.push_back(1);
        }
        // nb_argument = 1;
        // Serial.printf("lkklqdqsdksmdkqsd\r\n");
        NodeToken nd;
        nd._nodetype = changeTypeNode;
        nd.type = TokenKeywordVarType;
        nd._vartype = __none__;
        current_node = current_node->addChild(nd);
        change_type.push_back(current_node);
        parseExpr();
        // Serial.printf("lkklqdqsdksm excut dkqsd\r\n");
        if (Error.error)
        {
            return;
        }
        current_node = current_node->parent;
        change_type.pop_back();
        // arg.addChild(res._nd);
        while (Match(TokenComma))
        {
            next();
            __sav_arg = nb_args.back();
            nb_args.pop_back();
            nb_args.push_back(__sav_arg + 1);
            // nb_argument++;
            NodeToken nd;
            nd._nodetype = changeTypeNode;
            nd.type = TokenKeywordVarType;
            nd._vartype = __none__;
            current_node = current_node->addChild(nd);
            change_type.push_back(current_node);
            parseExpr();
            if (Error.error)
            {
                return;
            }
            current_node = current_node->parent;
            change_type.pop_back();
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

        main_context.findFunction(current());
        // NodeToken *t =search_result;
        if (search_result == NULL)
        {
            if (struct_name.size() > 0)
            {
                current()->addText(string_format("%s.%s", struct_name.c_str(), current()->getText()));
                main_context.findFunction(current());
                isStructFunction = true;
                if (search_result == NULL)
                {
                    Error.error = 1;
                    Error.error_message = string_format("function %s not found %s", current()->getText(), linepos().c_str());
                    return;
                }
            }
            else
            {
                Error.error = 1;
                Error.error_message = string_format("function %s not found %s", current()->getText(), linepos().c_str());
                return;
            }
        }
        next();
        next();
        // NodeToken *res=search_result;

        NodeToken res = NodeToken(search_result);
        if (res._nodetype == (int)defExtFunctionNode)
        {

            res._nodetype = extCallFunctionNode;
        }
        else
        {
            res._nodetype = callFunctionNode;
        }

        // NodeExtCallFunction function = NodeExtCallFunction(t);
        current_node = current_node->addChild(res);
        // current_node->copyChildren(search_result);
        current_node->addChild(search_result->getChildAtPos(0));
       // if (search_result->getChildAtPos(0)->_vartype == __float__ and change_type.size() > 0)
         //   change_type.back()->_vartype = __float__;

                       if (change_type.size() > 0)
            {
                if(change_type.back()->_vartype!=__float__)
                {
                    if (search_result->getChildAtPos(0)->_vartype == __float__ || search_result->getChildAtPos(0)->_vartype== __uint32_t__ )
                    {
                        change_type.back()->_vartype = search_result->getChildAtPos(0)->_vartype;
                    }
                }
           
            } 
        current_node->addChild(search_result->getChildAtPos(1));

        // sav_nb_arg = function._link->getChildAtPos(1)->children.size();
        nb_sav_args.push_back(current_node->getChildAtPos(1)->children.size());
        for (int i = 0; i < current_node->getChildAtPos(1)->children.size(); i++)
        {
            if (current_node->getChildAtPos(1)->getChildAtPos(i)->_vartype == __Args__)
            {

                nb_sav_args.pop_back();
                nb_sav_args.push_back(999);
            }
        }
        // Serial.printf("serial2\r\n");
        // NodeCallFunction function = NodeCallFunction(t);

        // sav_nb_arg = function._link->getChildAtPos(1)->children.size();
        // nb_sav_args.push_back(current_node->getChildAtPos(1)->children.size());
        // Serial.printf("serial3\r\n");

        current_node->_vartype = current_node->getChildAtPos(0)->_vartype;
        parseArguments();
        // Serial.printf("serial4\r\n");
        if (Error.error)
        {
            return;
        }

        if (nb_sav_args.back() != nb_args.back() and nb_sav_args.back() != 999) // if (sav_nb_arg != nb_args.back())
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

    void parseComparaison()
    {
        // resParse res;
        Error.error = 0;
        //  NodeComparator cn = NodeComparator();
        current_node = current_node->addChild(NodeToken(current(), comparatorNode));

        // res._nd=NodeToken();

        /*
                parseExpr();
                if (Error.error)
                {
                    return;
                }
                // token *t=current();
                // current_node->type = current()->type;
                current_node = current_node->parent;
                current_node->type = current()->type;
                change_type.pop_back();
                // current_node->ad
                next();
                nd._nodetype = changeTypeNode;
                nd.type = TokenKeywordVarType;
                nd._vartype = __none__;
                current_node = current_node->addChild(nd);
                change_type.push_back(current_node);
                parseExpr();
                if (Error.error)
                {
                    return;
                }
                next();
                current_node = current_node->parent;
                change_type.pop_back();*/

        current_node->setTargetText(targetList.pop());
        parseExpr();
        // cn.target=target;
        // cn.addChild(left._nd);
        // cn.addChild(right._nd);
        next();
        Error.error = 0;
        current_node = current_node->parent;
        // current_node = current_node->parent;
        return;
    }
    void parseStatement()
    {
        //_asPointer=false;
        isPointer = false;
        sav_token.clear();
        Error.error = 0;
        // asPointer =false;
        // resParse result;
        // NodeStatement statement;
        // current_node=current_node->addChild(statement);
        current_node->addChild(NodeToken(statementNode));
#ifndef __TEST_DEBUG
        // printf("line:%d mem:%u\r\n",current()->line,esp_get_free_heap_size());
#endif
        // printf("line:%d mem:%u\r\n",current()->line,esp_get_free_heap_size());
        //  on demarre avec la function
        updateMem();
        if (Match(TokenString))
        {
            current_node->addChild(NodeToken(current(), stringNode));
            next();
            return;
        }
        else if (Match(TokenKeywordBreak))
        {
            string c = findForWhile();
            if (c.compare("__") == 0)
            {
                Error.error = 1;
                Error.error_message = string_format("nor For ar while found for break %s", linepos().c_str());
                return;
            }
            next();
            if (Match(TokenSemicolon))
            {
                current_node->addChild(NodeToken(c, breakNode));
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
        else if (Match(TokenKeywordContinue))
        {
            string c = findForWhile();
            if (c.compare("__") == 0)
            {
                Error.error = 1;
                Error.error_message = string_format("nor For ar while found for continue %s", linepos().c_str());
                return;
            }
            next();
            if (Match(TokenSemicolon))
            {
                current_node->addChild(NodeToken(c, continueNode));
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
                current_node->addChild(NodeToken(returnNode));
                next();
                return;
            }
            else
            {
                current_node = current_node->addChild(NodeToken(returnNode));
                // next();
                NodeToken nd;
                nd._nodetype = changeTypeNode;
                nd.type = TokenKeywordVarType;
                nd.setTokenText("yevbs");
                nd._vartype = __none__;
                if (lasttype != NULL)
                {
                    nd._vartype=lasttype->_vartype;
                    /*
                    if (lasttype->_vartype == __float__)
                    {
                        nd._vartype = __float__;
                    }
                    */
                }

                current_node = current_node->addChild(nd);
                change_type.push_back(current_node);
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

                    current_node = current_node->parent;
                    change_type.pop_back();
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
        /*
        else  if (Match(TokenIdentifier) &&  Match(TokenMember,1) && Match(TokenIdentifier, 2)  && Match(TokenOpenParenthesis, 3))
            {
                 current_cntx->findVariable(current(), false);
                        if (search_result == NULL)
        {


            Error.error = 1;
            Error.error_message = string_format("impossible to find declaraiton for %s %s", current()->getText(), linepos().c_str());
            next();
            return;
        }
                next();
                next();
                current()->addText(string_format("%s.%s",search_result->getVarType()->varName.c_str() ,current()->getText()));
                NodeToken nd=*search_result;
                nd._nodetype=globalVariableNode;
                 nd.isPointer=true;
                 nodeTokenList.push(nd);
                isStructFunction=true;

                 parseFunctionCall();
                 isStructFunction=false;
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

                 next();
                return;

            }*/
        else if (Match(TokenIdentifier) && Match(TokenOpenParenthesis, 1))
        {
            bool sav_b = isStructFunction;
            isStructFunction = false;
            parseFunctionCall();
            isStructFunction = sav_b;
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
        if (Match(TokenIdentifier) && (Match(TokenPlusPlus, 1) or Match(TokenMinusMinus, 1)))
        {
            // NodeAssignement d = NodeAssignement();
            // printf("on est ici %s\r\n",current()->text.c_str());
            current_node = current_node->addChild(NodeToken(assignementNode));
            getVariable(true);
            if (Error.error)
            {
                return;
            }
            // NodeUnitary g = NodeUnitary();
            current_node = current_node->addChild(NodeToken(unitaryOpNode));
            prev();
            // printf("on est ici %s\r\n",current()->text.c_str());
            getVariable(false);
            if (Error.error)
            {
                return;
            }
            current_node->addChild(NodeToken(current(), operatorNode));
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
            current_node = current_node->addChild(NodeToken(assignementNode));
            getVariable(true);

            _asPointer = false;
            isPointer = false;
            if (Error.error)
            {
                return;
            }

            if (Match(TokenSemicolon))
            {
                Error.error = 0;
                // result._nd = nd;
                // current_node=current_node->parent;
                current_node = current_node->parent;
                next();
                return;
            }
            if (Match(TokenEqual))
            {
                NodeToken nd;
                nd._nodetype = changeTypeNode;
                nd.type = TokenKeywordVarType;
                nd.setTokenText("yves");
                nd._vartype=tmp_sav->_vartype;
                /*
                if (tmp_sav->_vartype == __float__)
                    nd._vartype = __float__;
                else
                    nd._vartype = __none__;
                */
                current_node = current_node->addChild(nd);
                change_type.push_back(current_node);
                next();
                parseExpr();
                if (Error.error)
                {
                    return;
                }

                if (!Match(TokenSemicolon) && !Match(TokenCloseParenthesis))
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
                current_node = current_node->parent;
                current_node = current_node->parent; // new expr

                change_type.pop_back();
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
            current_cntx = current_cntx->addChild(Context());
            // current_cntx = k;
            //  string target =string_format("label_%d%s",for_if_num,k->name.c_str());
            targetList.push(string_format("label_%d", for_if_num));
            //=target;
            for_if_num++;

            // NodeElse ndf = NodeElse(fort);
            // ndf.target = targetList.pop();
            // current_node = current_node->addChild(ndf);
            current_node = current_node->addChild(NodeToken(current(), elseNode, targetList.pop()));
            next();

            if (Match(TokenOpenCurlyBracket))
            {
                parseBlockStatement();
                if (Error.error)
                {
                    return;
                }
            }
            else
            {
                // next();
                parseStatement();
                if (Error.error)
                {
                    return;
                }
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
            sav_t.push_back(*current());
            // Context cntx;
            // cntx.name = current()->text;
            //  //printf("entering f %d %s %s %x\n", current_cntx->_global->children.size(), current_cntx->_global->name.c_str(), current()->text.c_str(), (uint64_t)current_cntx->_global);
            // Context *k = (*(current_cntx)).addChild(cntx);
            // current_cntx = (*(current_cntx)).addChild(cntx);;
            current_cntx = current_cntx->addChild(Context());
            targetList.push(string_format("label_%d", for_if_num));
            //=target;
            for_if_num++;
            next();
            if (Match(TokenOpenParenthesis))
            {
                // NodeWhile ndf = NodeWhile(fort);
                // ndf.target = target;
                // current_node = current_node->addChild(ndf);
                current_node = current_node->addChild(NodeToken(&sav_t.back(), whileNode, targetList.get()));
                next();

                // printf(" *************** on parse comp/n");
                parseComparaison();
                // parseExpr();
                if (Error.error)
                {
                    return;
                }
                // targetList.pop();
                ////printf("on a parse %s\n",comparator._nd._token->text.c_str());
                // printf(" *************** on parse inc/n");
                // next();
                if (Match(TokenOpenCurlyBracket))
                {
                    parseBlockStatement();
                    if (Error.error)
                    {
                        return;
                    }
                }
                else
                {
                    // next();
                    parseStatement();
                    if (Error.error)
                    {
                        return;
                    }
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
            current_cntx = current_cntx->addChild(Context());
            // string target =string_format("label_%d%s",for_if_num,k->name.c_str());
            targetList.push(string_format("label_%d", for_if_num));
            //=target;
            for_if_num++;
            // next();
            if (Match(TokenOpenParenthesis, 1))
            {
                // NodeIf ndf = NodeIf(current());
                // ndf.target = targetList.get();

                // current_node = current_node->addChild(ndf);
                current_node = current_node->addChild(NodeToken(current(), ifNode, targetList.get()));
                next();
                next();

                // printf(" *************** on parse comp/n");
                parseComparaison();
                if (Error.error)
                {
                    return;
                }
                ////printf("on a parse %s\n",comparator._nd._token->text.c_str());
                // printf(" *************** on parse inc/n");
                if (Match(TokenOpenCurlyBracket))
                {
                    parseBlockStatement();
                    if (Error.error)
                    {
                        return;
                    }
                }
                else
                {
                    // next();
                    parseStatement();
                    if (Error.error)
                    {
                        return;
                    }
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
            current_cntx = current_cntx->addChild(Context());
            // current_cntx = k;
            // string target =string_format("label_%d%s",for_if_num,current_cntx->name.c_str());
            targetList.push(string_format("label_%d", for_if_num));
            //=target;
            for_if_num++;
            // next();
            if (Match(TokenOpenParenthesis, 1))
            {
                // NodeFor ndf = NodeFor(current());
                // ndf.target = targetList.get();
                next();
                // current_node = current_node->addChild(ndf);
                current_node = current_node->addChild(NodeToken(current(), forNode, targetList.get()));
                next();
                current_node = current_node->addChild(NodeToken(statementNode));
                //  __current.push( current());

                parseStatement();
                // __sav_pos = _tks.position;
                // deleteNotNeededToken(__current.pop(), current());
                //  _tks.position = __sav_pos;
                // parseStatement();
                if (Error.error)
                {
                    return;
                }
                current_node = current_node->parent;
                // printf(" *************** on parse comp/n");
                parseComparaison();
                if (Error.error)
                {
                    return;
                }
                ////printf("on a parse %s\n",comparator._nd._token->text.c_str());
                // printf(" *************** on parse inc/n");
                //  __current.push( current());
                current_node = current_node->addChild(NodeToken(statementNode));
                parseStatement();
                // __sav_pos = _tks.position;
                // deleteNotNeededToken(__current.pop(), current());
                // _tks.position = __sav_pos;
                // parseStatement();
                if (Error.error)
                {
                    return;
                }
                current_node = current_node->parent;
                if (Match(TokenOpenCurlyBracket))
                {
                    parseBlockStatement();
                    if (Error.error)
                    {
                        return;
                    }
                }
                else
                {
                    // next();
                    parseStatement();
                    if (Error.error)
                    {
                        return;
                    }
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

        else if (Match(TokenKeywordVarType) or Match(TokenUserDefinedVariable))
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
                // _uniquesave=
                current_node->addChild(nodeTokenList.pop());
                // current_cntx->variables.back().text;
                // current_node = current_node->parent;
                // _uniquesave->text=current_cntx->variables.back().text;
                next();
                return;
            }

            if (Match(TokenEqual))
            {
                //  NodeStatement ndsmt;
                tmp_sav = current_node->addChild(nodeTokenList.get());
                // NodeAssignement nd;
                current_node = current_node->addChild(NodeToken(assignementNode));
                next();
                // auto left = createNodeLocalVariableForStore(var);
                // copyPrty(type._nd, &var);
                // current_node->addChild(left);
                // _uniquesave=
                _uniquesave = nodeTokenList.pop();
                if (_uniquesave.getNodeTokenType() == defLocalVariableNode)
                {
                    _uniquesave._nodetype = (int)storeLocalVariableNode;
                }
                else
                {
                    _uniquesave._nodetype = (int)storeGlobalVariableNode;
                }
                current_node->addChild(_uniquesave); // createNodeLocalVariableForStore(nodeTokenList.pop()));
                                                     // _uniquesave->text=current_cntx->variables.back().text;
                NodeToken nd;
                nd._nodetype = changeTypeNode;
                nd.type = TokenKeywordVarType;
                nd.setTokenText("yevbs");
                 nd._vartype = tmp_sav->_vartype;
                /*
                if (tmp_sav->_vartype == __float__)
                    nd._vartype = __float__;
                else
                    nd._vartype = __none__;
                    */
                current_node = current_node->addChild(nd);
                change_type.push_back(current_node);
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

                current_node = current_node->parent; // new expr
                change_type.pop_back();
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
            parseExpr();
                       
                if (Error.error)
                {
                    return;
                }

                if (!Match(TokenSemicolon) && !Match(TokenCloseParenthesis))
                {
                    Error.error = 1;
                    Error.error_message = string_format("Expected ici ; %s", linepos().c_str());
                    // next();
                    return;
                }
                                current_node = current_node->parent; // new expr

                change_type.pop_back();
                next();
                return;
        }
        /*
        else
        {
            Error.error = 1;
            Error.error_message = string_format(" statetenUnexpected %s  %s", current()->getText(), linepos().c_str());
            return;
        }
        */
    }

    void parseBlockStatement()
    {
        // resParse result;

        // NodeBlockStatement nbStmnt;
        // Context cntx;
        // cntx.name = string_format("%d", block_statement_num);
        // current_cntx = current_cntx->addChild(cntx);
        updateMem();
        current_cntx = current_cntx->addChild(Context());
        block_statement_num++;

        current_node = current_node->addChild(NodeToken(current(), blockStatementNode));
        next();
        while (!Match(TokenCloseCurlyBracket) && !Match(TokenEndOfFile))
        {
            // printf("on tente aouter un stamt\n");
            //.push( current());

            parseStatement();
            //  __sav_pos = _tks.position;
            //  deleteNotNeededToken(__current.pop(), current());
            //  _tks.position = __sav_pos;
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

    void parseCreateArguments()
    {
        Error.error = 0;
        // NodeInputArguments arg;

        NodeToken arg = NodeToken(inputArgumentsNode);
        current_node = current_node->addChild(arg);
        if (isStructFunction)
        {
            NodeToken nd = NodeToken();
            nd.setTargetText("pointer");
            nd.isPointer = true;
            nd.type = TokenUserDefinedVariable;
            nd._nodetype = defLocalVariableNode;
            nd.stack_pos = _STACK_SIZE;
            current_node->addChild(nd);
            current_cntx->addVariable(nd);
        }
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
        //_nd._nodetype=(int)defLocalVariableNode;
        NodeToken t = nodeTokenList.pop();

        copyPrty(&t, &_nd);
        _nd = NodeToken(_nd, defLocalVariableNode);

        // NodeDefLocalVariable var = NodeDefLocalVariable(_nd);
        // _nd._nodetype=(int)defLocalVariableNode;
        // copyPrty(type._nd,&var);
        current_node->addChild(_nd);
        current_cntx->addVariable(_nd);
        //_uniquesave->text=current_cntx->variables.back().text;
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
            // NodeToken _nd = nodeTokenList.pop();
            NodeToken _nd = nodeTokenList.pop();
            //_nd._nodetype=(int)defLocalVariableNode;
            NodeToken t = nodeTokenList.pop();

            copyPrty(&t, &_nd);
            _nd = NodeToken(_nd, defLocalVariableNode);
            //  NodeDefLocalVariable var = NodeDefLocalVariable(_nd);
            // _nd._nodetype=(int)defLocalVariableNode;
            // arg.addChild(var);
            current_node->addChild(_nd);
            current_cntx->addVariable(_nd);
            //_uniquesave->text=current_cntx->variables.back().text;
            // next();
        }
        // prev();
        // resParse result;
        Error.error = 0;

        current_node = current_node->parent;
        return;
    }

    void parseDefFunction()
    {

        Error.error = 0;
        bool ext_function = false;
        bool is_asm = false;
        // printf("entering function %s with %ur\n",current()->text.c_str(),esp_get_free_heap_size());
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
        // Token func = *current();

        main_context.findFunction(current());
        if (search_result != NULL) // if (current_cntx->findFunction(current()) != NULL)
        {

            Error.error = 1;
            Error.error_message = string_format("function already %s  declared in the scope for %s", current()->getText(), linepos().c_str());
            next();
            return;
        }
        if (ext_function)
        {
            NodeToken function = NodeToken(current(), defExtFunctionNode);
            // function.addChild( nodeTokenList.pop());
            //   function.addChild(arguments._nd);

            current_node = current_node->addChild(function);
            lasttype = current_node->addChild(nodeTokenList.pop());

            lasttype = current_node;
            // current_cntx->parent->addFunction(current_node);
            // main_context.addFunction(current_node);
        }
        else if (is_asm)
        {
            NodeToken function = NodeToken(current(), defAsmFunctionNode);
            // function.addChild( nodeTokenList.pop());
            //   function.addChild(arguments._nd);

            current_node = current_node->addChild(function);
            lasttype = current_node->addChild(nodeTokenList.pop());

            // lasttype=current_node;
            // current_cntx->parent->addFunction(current_node);
        }
        else
        {
            NodeToken function = NodeToken(current(), defFunctionNode);
            // function.addChild( nodeTokenList.pop());
            //  function.addChild(arguments._nd);

            current_node = current_node->addChild(function);
            lasttype = current_node->addChild(nodeTokenList.pop());

            // lasttype=current_node;
            //  current_cntx->parent->addFunction(current_node);
            //   main_context.addFunction(current_node);
        }
        // on ajoute un nouveau contexte

        Context *k = current_cntx->addChild(Context());
        current_cntx = k;
        if (isStructFunction)
        {
            stack_size = _STACK_SIZE + 4;
            current_node->type = TokenUserDefinedVariableMemberFunction;
        }
        else
        {
            stack_size = _STACK_SIZE;
        }
        block_statement_num = 0;
        next();
        next();
        parseCreateArguments();
        main_context.addFunction(current_node);

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

                point_regnum = 4;

#ifndef __MEM_PARSER
                buildParents(current_node);
                current_node->visitNode();
                current_node->clear();
                //  current_cntx->clear();
                _node_token_stack.clear();
                // printf("after clean function %s\n",current_node->getTokenText());
                updateMem();
#endif

                /*
                #ifndef __MEM_PARSER
                               printf("on compile %s\r\n",current_node->text.c_str());
                                __sav_pos = _tks.position;
                                buildParents(current_node);

                                current_node->visitNode(current_node);
                                clearContext(tobedeted);
                                _tks.position = __sav_pos;
                #endif
                */
                // printf("on a visité\r\n");
                current_cntx = current_cntx->parent;
                current_node = current_node->parent;

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
if(Match(TokenQuestionMark))
        {
            next();
            current_node=current_node->addChild(NodeToken(ternaryIfNode));
            current_node->addTargetText(string_format("label_tern_%d",for_if_num));
            for_if_num++;
            parseExprAddMinus();
                    if(Match(TokenColon))
        {
            next();
            
            parseExprAddMinus();
        }
        else
        {
                Error.error = 1;
                Error.error_message = string_format("Expected : %s", linepos().c_str());
        }
        current_node=current_node->parent;
        }

        while (Match(TokenStar) || Match(TokenSlash) || Match(TokenModulo) || Match(TokenKeywordOr) || Match(TokenKeywordAnd) || Match(TokenPower))
        {
            // token *op = current();
            sav_t.push_back(*current());
            next();
            // NodeBinOperator nodeopt;
            _node_token_stack.push_back(current_node->children.back());
            // NodeToken d = current_node->children.back();
            current_node->children.pop_back();
            // current_node->children.erase( --current_node->children.end());
            current_node = current_node->addChild(NodeToken(binOpNode));
            current_node->addChild(_node_token_stack.back());
            _node_token_stack.pop_back();
            // current_node->parent->children.remove(current_node->parent->children.back());
            // NodeOperator opt = NodeOperator(op);
            current_node->addChild(NodeToken(&sav_t.back(), operatorNode));
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
              // Serial.printf("eee  term1\r\n");

        sav_token.push_back(current_node);
        /*
        NodeToken nd;
        nd._nodetype=changeTypeNode;
        nd.type=TokenKeywordVarType;
        nd._vartype=__none__;
        if(current_node->_nodetype==changeTypeNode and strlen(current_node->getTokenText())>0)
        {
            nd._vartype=current_node->_vartype;
        }
        current_node=current_node->addChild(nd);
        change_type.push_back(current_node);
        */
        // Serial.printf("eee  term\r\n");
        parseExprConditionnal();
        // Serial.printf("exit  term\r\n");
        if (Error.error == 1)
        {
            return;
        }
        while (Match(TokenDoubleUppersand) || Match(TokenDoubleOr))
        {

            // token *op = current();
            sav_t.push_back(*current());
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
            current_node = current_node->addChild(NodeToken(binOpNode));
            current_node->addChild(_node_token_stack.back());
            _node_token_stack.pop_back();
            // current_node->parent->children.remove(current_node->parent->children.back());
                        if((&sav_t.back())->type ==  TokenDoubleUppersand)
                (&sav_t.back())->type =TokenKeywordAnd;
            else
             (&sav_t.back())->type =TokenKeywordOr;
            current_node->addChild(NodeToken(&sav_t.back(), operatorNode));

            sav_t.pop_back();
            parseExprConditionnal();
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
        //  lasttype=change_type.back();
        // printf("last type:%d\n",lasttype->_vartype);
        // change_type.pop_back();
        // current_node = sav_pa;
        // printf("exit expr");
        Error.error = 0;
        return;
    
    }
    void parseExprConditionnal()
    {

        sav_token.push_back(current_node);
        parseExprAddMinus();
        if (Error.error == 1)
        {
            return;
        }
        while (Match(TokenDoubleEqual) || Match(TokenLessOrEqualThan) || Match(TokenLessThan) || Match(TokenMoreOrEqualThan) || Match(TokenMoreThan))
        {

            // token *op = current();
            targetList.push(string_format("label_%d", for_if_num));
            //=target;
            for_if_num++;
            sav_t.push_back(*current());
            next();
            _node_token_stack.push_back(current_node->children.back());

            current_node->children.pop_back();
            current_node = current_node->addChild(NodeToken(testNode));
            // current_node->addChild(NodeToken(&sav_t.back(), operatorNode));
            current_node->type = sav_t.back().type;
            current_node->setTargetText(targetList.pop());
            NodeToken nd;
            nd._nodetype = changeTypeNode;
            nd.type = TokenKeywordVarType;
            nd._vartype = findfloat(_node_token_stack.back());
            if(nd._vartype!=__float__)
            {
                nd._vartype = finduint32_t(_node_token_stack.back());
            }
            current_node = current_node->addChild(nd);
            change_type.push_back(current_node);
            current_node->addChild(_node_token_stack.back());
            _node_token_stack.pop_back();
            current_node = current_node->parent;
            nd._vartype=__none__;
            current_node = current_node->addChild(nd);
            // current_node->type=sav_t.back().type;
            change_type.push_back(current_node);
            sav_t.pop_back();
            parseExprAddMinus();
            if (Error.error == 1)
            {
                return;
            }
            current_node = current_node->parent;
            current_node = current_node->parent;
            change_type.pop_back();
        }

        current_node = sav_token.back();
        sav_token.pop_back();

        Error.error = 0;
        return;
    }
    void parseExprAddMinus()
    {

        sav_token.push_back(current_node);
        
        parseTerm();
        if (Error.error == 1)
        {
            return;
        }
        while (Match(TokenAddition) || Match(TokenSubstraction) || Match(TokenShiftLeft) || Match(TokenShiftRight))
        {

            // token *op = current();
            sav_t.push_back(*current());
            next();
            _node_token_stack.push_back(current_node->children.back());

            current_node->children.pop_back();
            current_node = current_node->addChild(NodeToken(binOpNode));
            current_node->addChild(_node_token_stack.back());
            _node_token_stack.pop_back();
            current_node->addChild(NodeToken(&sav_t.back(), operatorNode));
            sav_t.pop_back();
            parseTerm();
            if (Error.error == 1)
            {
                return;
            }
            current_node = current_node->parent;
        }

        current_node = sav_token.back();
        sav_token.pop_back();

        Error.error = 0;
        return;
    }

    void parseFactor()
    { // resParse result;
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
        if (Match(TokenUppersand) && Match(TokenIdentifier, 1))
        {
            isPointer = true;
            // printf("qsldkqsld\n");
            next();
            // return;
        }
        if (current()->getType() == TokenEndOfFile)
        {

            next();
            return;
        }

        else if (Match(TokenNumber))
        {

            // NodeNumber g = NodeNumber(current());
            current_node->addChild(NodeToken(current(), numberNode));
            if (change_type.size() > 0)
            {
                if(change_type.back()->_vartype!=__float__)
                {
                    if (current()->_vartype == __float__ || current()->_vartype == __uint32_t__ )
                    {
                        change_type.back()->_vartype = current()->_vartype;
                    }
                }
                /*
                if (current()->_vartype == __float__)
                    change_type.back()->_vartype = current()->_vartype;
                    */
            }
            next();

            Error.error = 0;
            // result._nd = g;
            // printf("exit factor\n");

            return;
        }

        else if (Match(TokenNot) || Match(TokenAddition) || Match(TokenSubstraction) || Match(TokenUppersand) || Match(TokenKeywordFabs) || Match(TokenKeywordAbs))
        {
            // token *t = current();
            // NodeUnitary g = NodeUnitary();
            current_node = current_node->addChild(NodeToken(unitaryOpNode));
            sav_t.push_back(*current());

            next();

            parseFactor();
            if (Error.error == 1)
            {
                return;
            }
            // NodeUnitary g = NodeUnitary(NodeOperator(t), res._nd);
            current_node->addChild(NodeToken(&sav_t.back(), operatorNode));
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

            current_node = current_node->addChild(NodeToken(current(), changeTypeNode));

            // change_type.push_back(current_node);

            // if(change_type.size()>0)
            //    change_type.back()->_vartype=current()->_vartype;
            next(); //)
            next(); //(
            next();
            NodeToken nd;
            nd._nodetype = changeTypeNode;
            nd.type = TokenKeywordVarType;
            nd._vartype=current_node->_vartype;
            /*
            if (current_node->_vartype == __float__)
                nd._vartype = __float__;
            else
                nd._vartype = __none__;
                */
            current_node = current_node->addChild(nd);
            change_type.push_back(current_node);

            parseExpr();
            if (Error.error == 1)
            {
                return;
            }
            if (Match(TokenCloseParenthesis))
            {
                next();
                Error.error = 0;
                // current_node = current_node->parent;

                current_node = current_node->parent;
                current_node = current_node->parent;

                // current_node=current_node->parent;
                change_type.pop_back();
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
            // printf("one est icic\n\r");
            parseExpr();
            // if(lasttype->_vartype==__float__)
            // {
            //     change_type.back()->_vartype=__float__;
            // }

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
            //         if(current_node->getChildAtPos(current_node->children.size()-1)->_vartype==__float__)
            // change_type.back()->_vartype=__float__;
            if (change_type.size() > 0)
            {
                if(change_type.back()->_vartype!=__float__)
                {
                    if (tmp_sav->_vartype == __float__ || tmp_sav->_vartype == __uint32_t__ )
                    {
                        change_type.back()->_vartype = tmp_sav->_vartype;
                    }
                }
                /*
                if (tmp_sav->_vartype == __float__)
                    change_type.back()->_vartype = __float__;
                    */
            }
            if (Error.error == 1)
            {
                // next();
                return;
            }

            return;
        }

        else if (Match(TokenIdentifier) && Match(TokenOpenParenthesis, 1))
        {
            bool sav_b = isStructFunction;
            isStructFunction = false;
            parseFunctionCall();
            if (Error.error)
            {
                return;
            }
            // if(current_node->getChildAtPos(current_node->children.size()-1)->getChildAtPos(0)->_vartype==__float__)
            // change_type.back()->_vartype=__float__;
            isStructFunction = sav_b;
            return;
        }

        else if (Match(TokenKeywordVarType) && Match(TokenOpenParenthesis, 1))
        {
            // on tente CRGB()
            // token *typeVar = current();
            sav_t.push_back(*current());
            // NodeNumber num = NodeNumber( current());
            current_node = current_node->addChild(NodeToken(current(), numberNode));
            next();
            if (Match(TokenOpenParenthesis))
            {
                for (int i = 0; i < sav_t.back().getVarType()->size; i++)
                {
                    next();
                    parseExpr();
                    if (Error.error)
                    {
                        next();
                        return;
                    }
                    // num.addChild(res._nd);
                    if (i == sav_t.back().getVarType()->size - 1)
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
            NodeToken nd; //=NodeToken();
            nd._nodetype = defGlobalVariableNode;
            nd.type = TokenKeywordVarType;

            nd._vartype = __char__;
            nd.isPointer = true;
            nd.textref = all_text.addText(string_format("local_string_%d", for_if_num));
            for_if_num++;

            // nd.addChild(NodeToken(current(), stringNode));
            current_cntx->addVariable(nd);
            NodeToken *f = program.addChildFront(nd);
            f->addChild(NodeToken(current(), stringNode));
            nd._nodetype = globalVariableNode;
            // nd.children.clear();
            current_node->addChild(nd);
            next();
            return;
        }
        Error.error = 1;
        Error.error_message = string_format(" impossible to find Token %s %s", current()->getText(), linepos().c_str());
        next();
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

        if (Match(TokenKeywordVarType) or Match(TokenUserDefinedVariable))
        {

            _nd._nodetype = typeNode;
            _nd.type = current()->type;
            _nd._vartype = current()->_vartype;
            _nd.textref = current()->textref;

            // printf("pqoidpoqsidpoqisdopiqsopdiqsd %s\r\n",current()->text.c_str());
            int k = findStruct(current()->getText());
            if (k > -1)
            {
                _nd._vartype = k;
                _nd._total_size = _userDefinedTypes[k]._varSize;
            }

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
            Error.error_message = string_format("expecting external, __ASM__  or variable type %s %d %s", linepos().c_str(), current()->type, current()->getText());
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
            Error.error_message = string_format("variable %s already declared in the scope for %s", current()->getText(), linepos().c_str());
            next();
            return;
        }
        if (Match(TokenOpenBracket, 1))
        {
            // we are in the case led[];
            string sizestr = "";
            int j = 0;
            NodeToken var = NodeToken(current());
            next();
            next();
            var._total_size = 1;
            if (Match(TokenNumber))
            {
                j++;
                // Token num = *current();
                if (current()->getVarType()->_varType == __int__)
                {
                    var._total_size *= stringToInt(current()->getText());
                    sizestr = sizestr + " " + string(current()->getText());
                    next();
                }
                else
                {
                    // resParse res;
                    Error.error = 1;
                    Error.error_message = string_format("expecting an integer %s", linepos().c_str());
                    next();
                    return;
                }
                while (Match(TokenComma))
                {
                    next();
                    if (current()->getVarType()->_varType == __int__)
                    {
                        j++;
                        var._total_size *= stringToInt(current()->getText());
                        sizestr = sizestr + " " + string(current()->getText());
                        next();
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
                if (Match(TokenCloseBracket))
                {
                    var.isPointer = true;
                    var._nodetype = defGlobalVariableNode; // we can't have arrays in the stack
                                                           // var._total_size = stringToInt(num.getText());
                    sizestr = string_format("@%d%s", j, sizestr.c_str());
                    var.addTargetText(sizestr);
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
            else if (Match(TokenCloseBracket))
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
        int memberpos = 0;
        int _start = 0;
        int _pos = 0;
        //  int _totalsize = 0;

        current_cntx = &main_context;
        current_node = &program;
        Error.error = 0;
        while (Match(TokenEndOfFile) == false)
        {
            isStructFunction = false;
            updateMem();
            if (Match(TokenKeywordStruct))
            {

                oneFunction = false;
                current_cntx = current_cntx->addChild(Context());
                next();
                if (Match(TokenUserDefinedName))
                {

                    usded._varType = __userDefined__;
                    memberpos = 0;
                    _start = 0;
                    _pos = 0;
                    // _totalsize = 0;
                    usded.varName = current()->getText();
                    struct_name = usded.varName;

                    next(); //{

                    next(); // int
                    while (!Match(TokenCloseCurlyBracket) and !Match(TokenEndOfFile))
                    {

                        if (Match(TokenKeywordVarType) and Match(TokenIdentifier, 1) and Match(TokenOpenParenthesis, 2))
                        {
                            isStructFunction = true;
                            /* if (!oneFunction)
                             {
                                 current_cntx = current_cntx->parent;
                                 oneFunction = true;
                             }
                             */
                            parseType();
                            if (Error.error)
                            {

                                return;
                            }

                            current()->addText(string_format("%s.%s", usded.varName.c_str(), current()->getText()));
                            parseDefFunction();

                            if (Error.error)
                            {
                                return;
                            }
                            isStructFunction = false;
                        }
                        else
                        {
                            usded.starts[memberpos] = _start;

                            __v = *current()->getVarType();
                            usded.types[memberpos] = __v._varType;
                            usded.memberSize[memberpos] = __v.size;
                            _start += __v.total_size;
                            for (int _var = 0; _var < __v.size; _var++)
                            {
                                usded.load[_pos] = __v.load[_var];
                                usded.store[_pos] = __v.store[_var];
                                usded.sizes[_pos] = __v.sizes[_var];
                                _pos++;
                            }
                            next(); // name
                            NodeToken nd = NodeToken(current(), defLocalVariableNode);
                            nd.type = TokenUserDefinedVariableMember;
                            nd._vartype = __v._varType;
                            nd.stack_pos = 1000 * (_start - __v.total_size) + _STACK_SIZE;
                            nd._total_size = __v.size;
                            nd.asPointer = true;
                            nd.isPointer = true;
                            current_cntx->addVariable(nd);

                            usded.membersNames[memberpos] = current()->getText();
                            // printf(" addinr %s.%s\r\n",usded.varName.c_str(),current()->text.c_str());
                            next(); // ;
                            if (!Match(TokenSemicolon))
                            {
                                Error.error = 1;
                                Error.error_message = string_format("missing ; at line %s", linepos().c_str());
                                return;
                            }
                            next();
                            memberpos++;
                        }
                    }
                    usded.size = _pos;
                    usded.total_size = _start;
                    usded._varSize = _start;
                    _userDefinedTypes.push_back(usded);
                    next();
                }
                // if (!oneFunction)
                //{
                current_cntx = current_cntx->parent;
                struct_name = "";
                isStructFunction = false;
                // }
            }
            else if (Match(TokenKeywordSafeMode))
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

                        parseDefFunction();

                        if (Error.error)
                        {
                            return;
                        }
                    }
                    else
                    {
                        parseVariableForCreation();
                        if (Error.error)
                        {

                            return;
                        }
                        NodeToken nd = nodeTokenList.pop();
                        NodeToken t = nodeTokenList.pop();
                        if (isExternal)
                        {
                            nd._nodetype = (int)defExtGlobalVariableNode;

                            isExternal = false;
                        }
                        else
                        {
                            nd._nodetype = (int)defGlobalVariableNode;
                        }
                        copyPrty(&t, &nd);

                        current_node = program.addChild(nd);
                        tmp_sav = current_node;
                        current_cntx->addVariable(nd);
                        if (Match(TokenSemicolon))
                        {

                            current_node = current_node->parent;
                            next();
                        }
                        else if (Match(TokenEqual) && Match(TokenString, 1))
                        {
                            next();
                            current_node->addChild(NodeToken(current(), stringNode));
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
                            // __sav_pos = _tks.position;

                            // _tks.position = __sav_pos;
                            current_node = current_node->parent;
                        }
                        else if (Match(TokenEqual) and Match(TokenNumber, 1))
                        {
                            next();
                            current_node->addChild(NodeToken(current(), numberNode));
                            next();
                            if (!Match(TokenSemicolon))
                            {

                                Error.error = 1;
                                Error.error_message = string_format("Expected ; %s", linepos().c_str());
                                next();
                                return;
                            }
                            next();
                            current_node = current_node->parent;
                            Error.error = 0;
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
        Error.error = 0;

        return;
    }
};

#ifdef __CONSOLE_ESP32

Parser p = Parser();
// Executable consExecutable = Executable();
vector<Executable> scExecutables;

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
        /*
        if (progToRun > scExecutables.size())
        {
            LedOS.pushToConsole("No executable ...", true);
        }
        else
        {
            scExecutables[progToRun - 1].executeAsTask("main", _args);
        }
        */
    }
    // SCExecutable._run(args, true);
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
    /*
    bool othercore = false;
    executecmd = createExectutable(&cons->script, true);
    // strcompile = "";
   // p.clean2();
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
    */
}
void compile_c(Console *cons, vector<string> args)
{
    pushToConsole("Compiling ...", true);
    Executable _scExec = p.parse_c(&cons->script);
    if (_scExec.exeExist)
    {

        _scExec.name = cons->filename;
        scriptRuntime.addExe(_scExec);
        pushToConsole(string_format("Compiling done. Handle number:%d", scExecutables.size()), true);
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
        LedOS.addKeywordCommand("list", listExec, "list the compiled programs");
        LedOS.addKeywordCommand("free", free, "free the binary free x will free the program with handle x");
        LedOS.addKeywordCommand("run", run, "Run an already compiled program (always second Core) run x run program with handle x");
        LedOS.addKeywordCommand("kill", kill, "Stop a running program kill x kill program with handle x");
        LedOS.addKeywordCommand("parseasm", parseasm, "Parse assembly program");
        LedOS.addEscCommand(18, parsec_cEsc, "Compile and execute a program (always second Core)");
        LedOS.addEscCommand(11, kill_cEsc, "Stop a running program");
        addExternal("__feed", externalType::function, (void *)feedTheDog);

        /*
        LedOS.script.clear();
         LedOS.script.push_back("stack:");
         LedOS.script.push_back(".bytes 120");
         LedOS.script.push_back(".global @_main");
         LedOS.script.push_back("@_main:");
         LedOS.script.push_back("entry a1,72");
         for (int i = 1; i < 400; i++)
         {
             LedOS.script.push_back("nop");
         }
         LedOS.script.push_back("retw.n");
         */
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

void formatNewLine()
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
    all_text.clear();
    main_script.addContent((char *)sd.c_str());
    main_script.init();
    // Serial.printf("streing:%s\r\n",str.c_str());
    _tks.tokenize(&main_script, true, true, 1);
    // _tks.init();
    // Serial.printf("streing:%s\r\n",str.c_str());
    string res = "";

    _parenthesis.clear();
    _bracket.clear();
    _parenthesis.clear();
    _prevparenthesis = 0;
    _prevcurlybracket = 0;
    _prevbracket = 0;

    while (_tks.current()->getType() != TokenEndOfFile) // for (int i = 0; i < _tks.size(); i++)
    {
        Token tk = *_tks.current();
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
            Token tkn = *_tks.peek(1);
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
            // _tks.prev();
        }
        _tks.next();
    }

    _tks.clear();
    //  _parent.clear();

    _for_display = false;
    return res;
}

class __INIT_TOKEN
{
public:
    __INIT_TOKEN()
    {
        LedOS.addHightLightinf("sc", formatLine, formatInit, formatNewLine);
    }
};
__INIT_TOKEN _init_token;
#endif
void artiPrintf(char const *format, ...)
{
    va_list argp;
    va_start(argp, format);
    vprintf(format, argp);
    // printf("\r\n");
    va_end(argp);
}
void artiPrintfln(char const *format, ...)
{
    va_list argp;
    va_start(argp, format);
    vprintf(format, argp);
    printf("\r\n");
    va_end(argp);
}
void showError(int line, uint32_t size, uint32_t got)
{
    pushToConsole(string_format("Overflow error  max size: %d got %d", size, got), true);
}
class INIT_PARSER
{
public:
    INIT_PARSER()
    {
        addExternal("printf", externalType::function, (void *)artiPrintf);
        addExternal("printfln", externalType::function, (void *)artiPrintfln);
        addExternal("error", externalType::function, (void *)&showError);
    }
};
INIT_PARSER initialization_parser;
#endif