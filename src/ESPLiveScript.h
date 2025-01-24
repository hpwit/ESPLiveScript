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

#ifndef __TEST_DEBUG
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
// #include "soc/rtc_wdt.h"
#endif

#include "NodeToken.h"
/*
NodeToken d;
NodeToken nd;
NodeToken _nd;
NodeToken _t;
NodeToken var;
NodeToken *_d;
*/
string signature;
Token __t;
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
#ifdef __TEST_DEBUG
void prettyPrint(NodeToken *_nd, string ident)
{
    NodeToken nd = NodeToken(*_nd);
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
    if (_nd->children_size() > 0)
    {
        for (NodeToken *t : *_nd->children)
        {
            //  printf("child:%d\n",i);
            prettyPrint(t, ident);
            i++;
            // printf("on finit child:%d\n",i);
        }
    }
    // printf("we go back\n");
}

#endif

class Parser
{

public:
    NodeToken root;
    Parser() {}

    string linepos()
    {
        string f = string_format(" at line:%d position:%d", current()->line, current()->pos);
        Error.line = current()->line;
        Error.pos = current()->pos;
        return f;
    }
    int size()
    {
        return _tks->size();
    }
    Token *getTokenAtPos(int pos)
    {
        return _tks->getTokenAtPos(pos);
    }
    Token *current()
    {
        return _tks->current();
    }
    Token *next()
    {
        return _tks->next();
    }
    Token *prev()
    {
        return _tks->prev();
    }
    Token *peek(int index)
    {
        return _tks->peek(index);
    }
    bool Match(tokenType tt)
    {
        return _tks->Match(tt);
    }
    bool Match(tokenType tt, int index)
    {
        return _tks->Match(tt, index);
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
        sigs.clear();
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
        footer.clear();

        main_script.init();
        initMem();
        _tks = &__tks;
        _tks->tokenize(&main_script, true, true, 1);
        Error.error = 0;
        Error.error_message = "";
        Error.line = 0;
        Error.pos = 0;
        parseProgram();
    }

    Binary compileBinary()
    {
        Binary bin;
        bin.error.error = 0;
        _sav_token_line = 1;
        parse();
        bin.error = Error;
        if (Error.error)
        {
            pushToConsole(Error.error_message.c_str(), true);

            return bin;
        }
        pushToConsole("***********PARSING DONE*********");
        updateMem();
        buildParents(&program);
#ifdef __TEST_DEBUG
        pushToConsole("***********dispalying  DONE*********");
        prettyPrint(&program, "");
#endif
        program.visitNode();
        pushToConsole("***********COMPILING DONE*********");
        updateMem();
        displayStat();
        sigs.clear();
        sigs.shrink_to_fit();
        main_script.clear();
        _userDefinedTypes.clear();
        _userDefinedTypes.shrink_to_fit();
        nodeTokenList.clear();
        program.clearAll();
        sav_t.clear();
        sav_t.shrink_to_fit();
        main_context.clear();
        targetList.clear();
        change_type.clear();
        sav_token.clear();
        _node_token_stack.clear();
        for (NodeToken *h : _functions)
        {
        
            h->clearAll();
            free(h);
        }
        _functions.clear();
        _functions.shrink_to_fit();
        all_text.clear();
        all_targets.clear();

        updateMem();
        displayStat();

        pushToConsole("***********AFTER CLEAN*********");

#ifndef __TEST_DEBUG
        pushToConsole("***********CREATE BINARY*********");
        bin = createBinary(&footer, &header, &content, __parser_debug);
        content.clear();
        header.clear();
        footer.clear();
        change_type.clear();
        updateMem();
        displayStat();
        if (bin.error.error == 1)
        {
            //pushToConsole("WTF", true);
            pushToConsole(bin.error.error_message.c_str(), true);
            freeBinary(&bin);
        }
#endif
        return bin;
    }
    Executable compile()
    {

        Executable results;

        Binary bin = compileBinary();
        if (bin.error.error == 0)
        {
#ifndef __TEST_DEBUG
            pushToConsole("***********CREATE EXECUTABLE*********");

            executable _executecmd = createExectutable(&bin);
            results.setExecutable(_executecmd);
            results.error = _executecmd.error;
            updateMem();
            displayStat();
            if (_executecmd.error.error == 1)
            {
                // exeExist = false;
                // Serial.printf(termColor.Red);

                pushToConsole(_executecmd.error.error_message.c_str(), true);
            }
#endif
        }
        else
        {
           // pushToConsole(bin.error.error_message.c_str(), true);
        }

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

    Binary parseScriptBinary(string *str)
    {

        main_script.clear();
        main_script.addContent((char *)_sync.c_str());
        main_script.addContent((char *)division.c_str());
        main_script.addContent((char *)base_ext_functions.c_str());
        main_script.addContent((char *)str->c_str());
        return compileBinary();
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
    Binary parse_cBinary(list<string> *_script)
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
        return compileBinary();
    }
    void getVariable(bool isStore)
    {

        current_cntx->findVariable(current(), false); // false
        // NodeToken *nd = search_result;
        if (search_result == NULL)
        {

            found = false;
            for (int i = 0; i < external_links.size(); i++)
            {
                if (strcmp(current()->getText(), external_links[i].name.c_str()) == 0)
                {
                    sav_t.push_back(*current());
                    found = true;
                    _node_token_stack.push_back(current_node);
                    current_node = &program;
                    // string toinsert = external_links[i].name;
                    extra_script.clear();
                    _extra_tks.clear();
                    // printf("on iserset %s\n", external_links[i].signature.c_str());
                    extra_script.addContent((char *)(external_links[i].signature.c_str()));
                    extra_script.init();
                    __isBlockComment = false;
                    _tks = &_extra_tks;
                    for (int i = 0; i < __DEPTH; i++)
                    {
                        _tks->push(Token());
                    }
                    insecond = true;
                    _tks->tokenizelow(&extra_script, true, true, 20);
                    insecond = false;

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
                    NodeToken nd = nodeTokenList.pop();
                    NodeToken _t = nodeTokenList.pop();
                    if (isExternal)
                    {
                        nd._nodetype = (int)defExtGlobalVariableNode;

                        isExternal = false;
                    }
                    else
                    {
                        nd._nodetype = (int)defGlobalVariableNode;
                    }
                    copyPrty(&_t, &nd);

                    current_node = program.addChild(nd);

                    main_context.addVariable(nd);
                    current_node = _node_token_stack.back();
                    _node_token_stack.pop_back();

                    _tks = &__tks;
                    extra_script.clear();
                    _extra_tks.clear();
                    current_cntx->findVariable(&sav_t.back(), false);
                    sav_t.pop_back();
                    break;
                }
            }
            if (!found)

            {
                Error.error = 1;
                Error.error_message = string_format("impossible to find declaraiton for %s %s", current()->getText(), linepos().c_str());
                next();
                return;
            }
        }

        // token *vartoken = current();
        // auto var =
        // current_node = current_node->addChild(
        createNodeVariable(current(), isStore);

        next();
        if (Match(TokenOpenBracket))
        {
            // on parse
            next();

            // NodeToken nd;
           NodeToken nd = NodeToken(changeTypeNode);
            nd._nodetype = changeTypeNode;
            nd.type = TokenKeywordVarType;
            nd._vartype = __none__;
            current_node = current_node->addChild(nd);
            change_type.push_back(current_node);
            parseExpr();
            if (Error.error)
            {
                next();
                return;
            }
            current_node = current_node->parent;
            change_type.pop_back();
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
                    // NodeToken nd;
                   NodeToken nd = NodeToken(changeTypeNode);
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
                if (Match(TokenCloseBracket))
                {

                    next();
                    vector<string> tile;
                    int _s = current_node->children_size();
                    int nb = 0;
                    string sd = string(current_node->getTargetText());
                    if (sd.compare(0, 1, "@") == 0)
                    {
                        tile = split(sd, " ");

                        sscanf(tile[0].c_str(), "@%d", &nb);
                    }
                    if (nb < _s)
                    {
                        Error.error = 1;
                        Error.error_message = string_format("too many arguments expected less than %d got %d at %s", nb, _s, linepos().c_str());
                        next();
                        return;
                    }
                }
                else
                {

                    Error.error = 1;
                    Error.error_message = string_format("expecting ]  or , %s at %s", current()->getText(), linepos().c_str());
                    next();
                    return;
                }
            }
            else
            {

                Error.error = 1;
                Error.error_message = string_format("expecting ]  or , %s at %s", current()->getText(), linepos().c_str());
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

            current()->addText(string_format("%s.%s", search_result->getVarType()->varName.c_str(), current()->getText()));
            // nd = *search_result; //30/12
           NodeToken nd = NodeToken(*search_result);
            // nd.copyChildren(search_result);
            if (search_result->_nodetype == defGlobalVariableNode)
                nd._nodetype = globalVariableNode;
            else
                nd._nodetype = localVariableNode;

            nd.type = TokenUserDefinedVariableMemberFunction;
            nd.isPointer = true;
            nd._total_size = search_result->getVarType()->total_size; // nd.copyChildren(current_node);
                                                                      /*
                                                                     for(int i=0;i<current_node->children_size();i++)
                                                                     {
                                                                         nd.addChild(*current_node->getChildAtPos(i));
                                                                     }
                                                          
                                                                                      NodeToken *par=current_node->parent;
                                                                     current_node->parent->children->pop_back();
                                                                     current_node=par;
                                                                     */
            current_node->_nodetype = UnknownNode;
            // NodeToken *par = current_node;
            _node_token_stack.push_back(current_node);
            current_node = current_node->parent;

            nodeTokenList.push(nd);
            isStructFunction = true;
            // printf("her\n");
            parseFunctionCall();

            if (Error.error)
            {
                return;
            }
            current_node->getChildAtPos(current_node->children_size() - 1)->getChildAtPos(2)->getChildAtPos(0)->copyChildren(_node_token_stack.back());
            _node_token_stack.pop_back();
            isStructFunction = false;
            Error.error = 0;
            return;
        }
        Error.error = 0;
        tmp_sav = current_node;
        current_node = current_node->parent;

        return;

        //}
    }
    void parseArguments()
    {
#ifdef PARSER_DEBUG
        updateMem();
#endif
        // resParse result;
        // signature="(";
        sigs.push_back("(");
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
            string _signature = sigs.back() + ")";
            sigs.pop_back();
            sigs.push_back(_signature);
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
        // NodeToken nd;
        NodeToken nd = NodeToken(changeTypeNode);
        nd._nodetype = changeTypeNode;
        nd.type = TokenKeywordVarType;
        nd._vartype = __none__;
        current_node = current_node->addChild(nd);
        change_type.push_back(current_node);
        // printf("lkklqdqsdksm excut dkqsd\r\n");
        parseExpr();
        // printf("lkklqdqsdksm excut dkqsd\r\n");
        if (Error.error)
        {
            return;
        }
        string _signature = sigs.back() + current_node->getVarType()->varName;
        sigs.pop_back();
        sigs.push_back(_signature);
        if (current_node->isPointer)
        {

            string _signature = sigs.back() + "*";
            sigs.pop_back();
            sigs.push_back(_signature);
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
            // NodeToken nd;
           NodeToken nd =NodeToken(changeTypeNode);
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

            string _signature = sigs.back() + "|" + current_node->getVarType()->varName;
            sigs.pop_back();
            sigs.push_back(_signature);
            if (current_node->isPointer)
            {

                string _signature = sigs.back() + "*";
                sigs.pop_back();
                sigs.push_back(_signature);
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
        _signature = sigs.back() + ")";
        sigs.pop_back();
        sigs.push_back(_signature);
        current_node = current_node->parent;
        return;
    }
    void parseFunctionCall()
    {
#ifdef PARSER_DEBUG
        pushToConsole(string_format("functions:%s", __FUNCTION__));
        updateMem();
#endif
        // printf("calling  function yves %s\r\n", current()->getText());

        sav_t.push_back(*current());
        next();
        next();

        main_context.findCandidate(sav_t.back().getText());
        if (!main_context.findCandidate(sav_t.back().getText()))
        {
            if (struct_name.size() > 0)
            {
                string v_tmp = string_format("%s.%s", struct_name.c_str(), sav_t.back().getText());
                if (main_context.findCandidate((char *)v_tmp.c_str()))
                    isStructFunction = true;
            }
        }
        // printf("calling  function suite\r\n");
        parseArguments();
        //       printf("calling  function suite\r\n");
        if (Error.error)
        {
            return;
        }

        _node_token_stack.push_back(current_node->children->back());
        // NodeToken d = current_node->children->back();
        current_node->children->pop_back();

        (&sav_t.back())->addText(string_format("%s%s", sav_t.back().getText(), sigs.back().c_str()));
        sigs.pop_back();
        main_context.findFunction(&sav_t.back());
        // NodeToken *t =search_result;
        if (search_result == NULL)
        {

            if (struct_name.size() > 0)
            {
                sav_t.push_back(sav_t.back());
                (&sav_t.back())->addText(string_format("%s.%s", struct_name.c_str(), sav_t.back().getText()));
                main_context.findFunction(&sav_t.back());
                isStructFunction = true;
                sav_t.pop_back();
                // sav_t.pop_back();
            }
            if (search_result == NULL)
            {

                for (int i = 0; i < external_links.size(); i++)
                {
                    // printf("comparing %s ,%s \n\r", external_links[i].signature.c_str(), external_links[i].signature.c_str());
                    //   bool
                    found = false;

                    if (strstr(external_links[i].signature.c_str(), "Args") != NULL)
                    {
                        int l = strstr(external_links[i].signature.c_str(), "Args") - external_links[i].signature.c_str();
                        if (l > 0)
                            l--;
                        if (strncmp(external_links[i].signature.c_str(), sav_t.back().getText(), l) == 0)
                        {
                            found = true;
                        }
                    }
                    else if (external_links[i].signature.compare(string(sav_t.back().getText())) == 0)
                    {
                        found = true;
                    }
                    if (found)
                    {
                        //   printf("her\n\r");
                        savestacksize = stack_size;
                        _node_token_stack.push_back(current_node);
                        current_node = &program;
                        // string toinsert = external_links[i].name; //"external " + external_links[i].out + " " + external_links[i].name + "("+external_links[i].in + ");";
                        // printf("on inseet %s\n", external_links[i].name.c_str());
                        //  main_script.previousChar();
                        extra_script.clear();

                        extra_script.addContent((char *)(external_links[i].name.c_str()));
                        extra_script.init();
                        _tks = &_extra_tks;
                        // extra_script.nextChar();

                        _tks->clear();

                        // next();
                        // prev();
                        pos_in_line = 0;
                        insecond = true;
                        _tks->tokenizelow(&extra_script, true, true, 20);
                        insecond = false;
                        //  printf("%s \n\r",next()->getText());
                        // _for_display=false;

                        // printf("%s \n\r",next()->getText());
                        // printf("%s \n\r",next()->getText());

                        // prev();
                        sav_b = isStructFunction;
                        isStructFunction = false;
                        parseType();

                        if (Error.error)
                        {
                            //         printf("ice\n\r");
                            return;
                        }

                        parseDefFunction();
                        if (Error.error)
                        {
                            //           printf("cold\n\r");
                            return;
                        }
                        current_node = _node_token_stack.back();
                        _node_token_stack.pop_back();
                        isStructFunction = sav_b;
                        break;
                        // return;
                    }
                }
                _tks = &__tks;
                extra_script.clear();
                _extra_tks.clear();
                stack_size = savestacksize;
                // printf("looking for %s\n\r", sav_t.back().getText());
                main_context.findFunction(&sav_t.back());
                if (search_result == NULL)
                {
                    Error.error = 1;
                    Error.error_message = string_format("function %s not found %s", sav_t.back().getText(), linepos().c_str());
                    return;
                }
                // current()->type=TokenSemicolon;
            }
        }

        // NodeToken *res=search_result;

       NodeToken _nd = NodeToken(*search_result);
        if (_nd._nodetype == (int)defExtFunctionNode)
        {

            _nd._nodetype = extCallFunctionNode;
        }
        else // if (_nd._nodetype == (int)defFunctionNode)
        {
            _nd._nodetype = callFunctionNode;
        }

        // NodeExtCallFunction function = NodeExtCallFunction(t);
        current_node = current_node->addChild(_nd);
        // current_node->copyChildren(search_result);
        current_node->addChild(NodeToken(search_result->getChildAtPos(0)));

        // if (search_result->getChildAtPos(0)->_vartype == __float__ and change_type.size() > 0)
        //   change_type.back()->_vartype = __float__;

        if (change_type.size() > 0)
        {
            if (search_result->getChildAtPos(0)->children_size() == 0 && !search_result->getChildAtPos(0)->asPointer)
                change_type.back()->isPointer = search_result->getChildAtPos(0)->isPointer; // n,ew modif here
            if (change_type.back()->_vartype != __float__)
            {
                if (search_result->getChildAtPos(0)->_vartype == __float__ || search_result->getChildAtPos(0)->_vartype == __uint32_t__)
                {
                    change_type.back()->_vartype = search_result->getChildAtPos(0)->_vartype;
                }
            }
        }
        NodeToken *o = current_node->addChild(NodeToken(search_result->getChildAtPos(1)));
        o->copyChildren(search_result->getChildAtPos(1));

        // sav_nb_arg = function._link->getChildAtPos(1)->children_size();

        nb_sav_args.push_back(current_node->getChildAtPos(1)->children_size());
        if (isStructFunction)
        {
            // nb_sav_args.push_back( nb_sav_args.back()-1);
            isStructFunction = false;
        }
        for (int i = 0; i < current_node->getChildAtPos(1)->children_size(); i++)
        {
            if (current_node->getChildAtPos(1)->getChildAtPos(i)->_vartype == __Args__)
            {

                nb_sav_args.pop_back();
                nb_sav_args.push_back(999);
            }
        }

        current_node->_vartype = current_node->getChildAtPos(0)->_vartype;

        current_node->addChild(_node_token_stack.back());
        _node_token_stack.pop_back();

        if (nb_sav_args.back() != nb_args.back() and nb_sav_args.back() != 999) // if (sav_nb_arg != nb_args.back())
        {
            Error.error = 1;

            Error.error_message = string_format("Expected %d arguments got %d %s", nb_sav_args.back(), nb_args.back(), linepos().c_str());
            return;
        }
        nb_args.pop_back();
        nb_sav_args.pop_back();
        sav_t.pop_back();
        Error.error = 0;
        current_node = current_node->parent;

        return;
    }

    void parseComparaison()
    {
#ifdef PARSER_DEBUG
        updateMem();
#endif

        Error.error = 0;

        current_node = current_node->addChild(NodeToken(current(), comparatorNode));

        current_node->setTargetText(targetList.pop());
        parseExprAndOr();
      //  parseExprConditionnal();
        if (Error.error)
        {
            return;
        }

        next();
        Error.error = 0;
        current_node = current_node->parent;

        return;
    }
    void parseStatement()
    {
#ifdef PARSER_DEBUG
        updateMem();
#endif

        isPointer = false;
        sav_token.clear();
        Error.error = 0;
        current_node->addChild(NodeToken(statementNode));
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

                NodeToken nd = NodeToken(changeTypeNode);
                nd._nodetype = changeTypeNode;
                nd.type = TokenKeywordVarType;
                nd.setTokenText("yevbs");
                nd._vartype = __none__;
                if (lasttype != NULL)
                {
                    nd._vartype = lasttype->_vartype;
                    /*
                    if (lasttype->_vartype == __float__)
                    {
                        nd._vartype = __float__;
                    }
                    */
                }
                else
                {
                    Error.error = 1;
                    Error.error_message = string_format("issue with return %s", linepos().c_str());
                    return;
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

        else if (Match(TokenIdentifier) && Match(TokenOpenParenthesis, 1))
        {
            sav_b = isStructFunction;
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
            current_node->type = current()->type;
            // current_node->addChild(NodeToken(current(), operatorNode));
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
            next();

            Error.error = 0;
            // return;
        }
        else if (Match(TokenIdentifier))
        {
            // NodeAssignement nd;

            current_node = current_node->addChild(NodeToken(assignementNode));
            getVariable(true);
            if (Error.error)
            {
                return;
            }
           NodeToken d = NodeToken(current_node->getChildAtPos(0));

            nodeTokenList.push(d);
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
                nodeTokenList.pop();
                return;
            }
            if (Match(TokenEqual))
            {
                nodeTokenList.pop();
                // NodeToken nd;
                NodeToken nd = NodeToken(changeTypeNode);
                nd._nodetype = changeTypeNode;
                nd.type = TokenKeywordVarType;
                nd.setTokenText("yves");
                nd._vartype = tmp_sav->_vartype;
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
                current_node = current_node->parent; //  expr

                change_type.pop_back();
                next();
                return;
            }
            else if (Match(TokenPlusEqual) || Match(TokenMinusEqual) || Match(TokenStarEqual) || Match(TokenSlashEqual))
            {
                sav_t.push_back(*current());
                // NodeToken nd;
                NodeToken nd = NodeToken(changeTypeNode);
                nd._nodetype = changeTypeNode;
                nd.type = TokenKeywordVarType;
                nd.setTokenText("yves");
                nd._vartype = tmp_sav->_vartype;

                current_node = current_node->addChild(nd);
                change_type.push_back(current_node);
                next();
                current_node = current_node->addChild(NodeToken(binOpNode));
                NodeToken *_d = current_node->addChild(nodeTokenList.pop());
                switch (_d->_nodetype)
                {
                case storeExtGlocalVariableNode:
                    _d->_nodetype = extGlobalVariableNode;
                    break;
                case storeGlobalVariableNode:
                    _d->_nodetype = globalVariableNode;
                    break;
                case storeLocalVariableNode:
                    _d->_nodetype = localVariableNode;
                    break;
                }
                __t = sav_t.back();
                switch (__t.type)
                {
                case TokenPlusEqual:
                    __t.type = TokenAddition;
                    break;
                case TokenMinusEqual:
                    __t.type = TokenSubstraction;
                    break;
                case TokenStarEqual:
                    __t.type = TokenStar;
                    break;
                case TokenSlashEqual:
                    __t.type = TokenSlash;
                    break;
                }
                // current_node->addChild(NodeToken(&t,operatorNode));
                current_node->type = __t.type;
                sav_t.pop_back();

                parseExpr();
                if (Error.error)
                {
                    return;
                }
                if (Match(TokenSemicolon))
                {
                    Error.error = 0;
                    // result._nd = nd;
                    current_node = current_node->parent;
                    current_node = current_node->parent;
                    current_node = current_node->parent;
                    next();
                    // nodeTokenList.pop();
                    return;
                }
                else
                {
                    Error.error = 1;
                    Error.error_message = string_format("Expected ; %s", linepos().c_str());
                    return;
                }
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
            // //printf("entering f %d %s %s %x\n", current_cntx->_global->children_size(), current_cntx->_global->name.c_str(), current()->text.c_str(), (uint64_t)current_cntx->_global);
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
            //  //printf("entering f %d %s %s %x\n", current_cntx->_global->children_size(), current_cntx->_global->name.c_str(), current()->text.c_str(), (uint64_t)current_cntx->_global);
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
            // //printf("entering f %d %s %s %x\n", current_cntx->_global->children_size(), current_cntx->_global->name.c_str(), current()->text.c_str(), (uint64_t)current_cntx->_global);
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
            // //printf("entering f %d %s %s %x\n", current_cntx->_global->children_size(), current_cntx->_global->name.c_str(), current()->text.c_str(), (uint64_t)current_cntx->_global);
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
                // __sav_pos = _tks->position;
                // deleteNotNeededToken(__current.pop(), current());
                //  _tks->position = __sav_pos;
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
                // __sav_pos = _tks->position;
                // deleteNotNeededToken(__current.pop(), current());
                // _tks->position = __sav_pos;
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
            // printf("trying to create %s\n", current()->getText());
            parseType();

            if (Error.error)
            {
                Error.error = 1;

                return;
            }
            nodeTokenList.push(nodeTokenList.get());
            parseVariableForCreation();
            if (Error.error)
            {

                return;
            }
            nodeTokenList.push(createNodeLocalVariableForCreation(nodeTokenList.pop(), nodeTokenList.pop()));
            current_cntx->addVariable(nodeTokenList.get()); // 31/12
            if (Match(TokenComma))
            {
                while (Match(TokenComma))
                {
                    next();
                    nodeTokenList.push(nodeTokenList.get());
                    parseVariableForCreation();
                    if (Error.error)
                    {

                        return;
                    }
                    nodeTokenList.push(createNodeLocalVariableForCreation(nodeTokenList.pop(), nodeTokenList.pop()));
                    current_cntx->addVariable(nodeTokenList.get());
                }
                if (!Match(TokenSemicolon))
                {
                    Error.error = 1;
                    Error.error_message = string_format("Expected ; %s", linepos().c_str());
                    next();
                    return;
                }
                else
                {
                    next();
                    nodeTokenList.pop();
                    return;
                }
            }
            //       nodeTokenList.pop();
            if (Match(TokenSemicolon))
            {
                Error.error = 0;
                // result._nd = var;
                // _uniquesave=
                if (nodeTokenList.get().type == TokenUserDefinedVariable)
                {

                    // printf("check if coinstructo\r\n");
                    current()->addText(string_format("%s._@%s()", nodeTokenList.get().getVarType()->varName.c_str(), nodeTokenList.get().getVarType()->varName.c_str()));
                    main_context.findFunction(current());

                    if (search_result != NULL)
                    {

                        // printf("  printf("we have contructore\r\n"); if coinstructo\r\n");
                        NodeToken nd = nodeTokenList.get();
                        nd._nodetype = callConstructorNode;
                        current_node->addChild(nodeTokenList.pop());
                        current_node->addChild(nd);
                        nodeTokenList.pop();
                        next();
                        return;
                    }
                    else
                    {
                        current_node->addChild(nodeTokenList.pop());
                        nodeTokenList.pop();
                        next();
                        return;
                    }
                }
                current_node->addChild(nodeTokenList.pop());
                nodeTokenList.pop();
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

                if (nodeTokenList.get().type == TokenUserDefinedVariable)
                {

                    current_cntx->findVariable(nodeTokenList.get().getTokenText(), false);
                    if (search_result == NULL)
                    {

                        Error.error = 1;
                        Error.error_message = string_format("impossible to find declaraiton for %s %s", current()->getText(), linepos().c_str());
                        next();
                        return;
                    }

                    next();
                    current()->addText(string_format("%s._@%s", search_result->getVarType()->varName.c_str(), current()->getText()));
                    NodeToken nd = NodeToken(*search_result);
                    // nd.copyChildren(search_result);//30/12

                    if (search_result->_nodetype == defGlobalVariableNode)
                        nd._nodetype = globalVariableNode;
                    else
                        nd._nodetype = localVariableNode; // globalVariableNode;
                    nd.type = TokenUserDefinedVariableMemberFunction;
                    nd.isPointer = true;
                    nd._total_size = search_result->getVarType()->total_size;

                    nodeTokenList.push(nd);
                    isStructFunction = true;

                    parseFunctionCall();

                    if (Error.error)
                    {
                        return;
                    }
                    if (!Match(TokenSemicolon))
                    {
                        Error.error = 1;
                        Error.error_message = string_format("Missing ; %s", linepos().c_str());
                        return;
                    }
                    next();
                    //       current_node->getChildAtPos(current_node->children_size() - 1)->getChildAtPos(2)->getChildAtPos(0)->copyChildren(par);
                    isStructFunction = false;
                    Error.error = 0;
                    // current_node = current_node->parent;
                    return;
                }
                // NodeAssignement nd;
                current_node = current_node->addChild(NodeToken(assignementNode));
                next();
                // auto left = createNodeLocalVariableForStore(var);
                // copyPrty(type._nd, &var);
                // current_node->addChild(left);
                // _uniquesave=
                NodeToken _uniquesave = nodeTokenList.pop();
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
                                                     // NodeToken nd;
                NodeToken nd = NodeToken(changeTypeNode);
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

                current_node = current_node->parent; //  expr
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
            current_node = current_node->parent; //  expr

            change_type.pop_back();
            next();
            return;
        }
    }

    void parseBlockStatement()
    {
#ifdef PARSER_DEBUG
        updateMem();
#endif

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
            //  __sav_pos = _tks->position;
            //  deleteNotNeededToken(__current.pop(), current());
            //  _tks->position = __sav_pos;
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
#ifdef PARSER_DEBUG
        updateMem();
#endif
        Error.error = 0;
        signature = "(";

        current_node = current_node->addChild(NodeToken(defInputArgumentsNode));
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
            signature = signature + ")";

            // printf("on retourne with argh ide\n");
            return;
        }
        parseType();
        if (Error.error)
        {
            return;
        }
        signature = signature + nodeTokenList.get().getVarType()->varName;
        if (nodeTokenList.get().isPointer)
            signature = signature + "*";

        parseVariableForCreation();
        if (Error.error)
        {
            return;
        }
        NodeToken _nd = nodeTokenList.pop();
        //_nd._nodetype=(int)defLocalVariableNode;
        NodeToken _t = nodeTokenList.pop();

        copyPrty(&_t, &_nd);
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
            signature = signature + "|" + nodeTokenList.get().getVarType()->varName;
            if (nodeTokenList.get().isPointer)
                signature = signature + "*";
            parseVariableForCreation();
            if (Error.error)
            {
                return;
            }
            // NodeToken _nd = nodeTokenList.pop();
            NodeToken _nd = nodeTokenList.pop();
            //_nd._nodetype=(int)defLocalVariableNode;
            NodeToken _t = nodeTokenList.pop();

            copyPrty(&_t, &_nd);
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
        signature = signature + ")";

        current_node = current_node->parent;
        return;
    }

    void parseDefFunction()
    {
#ifdef PARSER_DEBUG
        updateMem();
#endif
        Error.error = 0;
        bool ext_function = false;
        bool is_asm = false;
       // printf("entering function %s \r\n", current()->getText());
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
        NodeToken nd;
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

            nd = NodeToken(current(), defExtFunctionNode);
            // function.addChild( nodeTokenList.pop());
            //   function.addChild(arguments._nd);

            current_node = current_node->addChild(nd);
            lasttype = current_node->addChild(nodeTokenList.pop());

            // lasttype = current_node; //modif  30/12
        }
        else if (is_asm)
        {
            nd = NodeToken(current(), defAsmFunctionNode);
            // function.addChild( nodeTokenList.pop());
            //   function.addChild(arguments._nd);

            current_node = current_node->addChild(nd);
            lasttype = current_node->addChild(nodeTokenList.pop());
        }
        else
        {
            nd = NodeToken(current(), defFunctionNode);
            // function.addChild( nodeTokenList.pop());
            //  function.addChild(arguments._nd);

            current_node = current_node->addChild(nd);
            lasttype = current_node->addChild(nodeTokenList.pop());
        }
        // on ajoute un nouveau contexte

        current_cntx = current_cntx->addChild(Context());
        // current_cntx = k;
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
        if (Error.error)
        {
            return;
        }
        // printf("signature %s%s\r\n", current_node->getTokenText(), signature.c_str());
        current_node->setTokenText(string_format("%s%s", current_node->getTokenText(), signature.c_str()));
        main_context.addFunction(current_node);

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
                ;
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
                current_cntx->clear();
               _node_token_stack.clear();
                sav_token.clear();
                change_type.clear();
                // printf("after clean function %s\n",current_node->getTokenText());
                updateMem();
#endif

                /*
                #ifndef __MEM_PARSER
                               printf("on compile %s\r\n",current_node->text.c_str());
                                __sav_pos = _tks->position;
                                buildParents(current_node);

                                current_node->visitNode(current_node);
                                clearContext(tobedeted);
                                _tks->position = __sav_pos;
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
#ifdef PARSER_DEBUG
        updateMem();
#endif
        sav_token.push_back(current_node);
        parseFactor();
        if (Error.error)
        {
            return;
        }
        if (Match(TokenQuestionMark))
        {
            next();
            _node_token_stack.push_back(current_node->children->back());
            // NodeToken d = current_node->children->back();
            current_node->children->pop_back();
            current_node = current_node->addChild(NodeToken(ternaryIfNode));
            current_node->addChild(_node_token_stack.back());
            _node_token_stack.pop_back();
            current_node->addTargetText(string_format("label_tern_%d", for_if_num));
            for_if_num++;
            parseExpr();
            if (Error.error)
            {
                return;
            }
            if (Match(TokenColon))
            {
                next();

                parseExpr();
                if (Error.error)
                {
                    return;
                }
            }
            else
            {
                Error.error = 1;
                Error.error_message = string_format("Expected : %s", linepos().c_str());
            }
            current_node = current_node->parent;
        }

        while (Match(TokenStar) || Match(TokenSlash) || Match(TokenModulo) || Match(TokenKeywordOr) || Match(TokenKeywordAnd) || Match(TokenPower))
        {
            // token *op = current();
            sav_t.push_back(*current());
            next();
            // NodeBinOperator nodeopt;
            _node_token_stack.push_back(current_node->children->back());
            // NodeToken d = current_node->children->back();
            current_node->children->pop_back();
            // current_node->children->erase( --current_node->children->end());
            current_node = current_node->addChild(NodeToken(binOpNode));
            current_node->addChild(_node_token_stack.back());
            _node_token_stack.pop_back();
            // current_node->parent->children->remove(current_node->parent->children->back());
            // NodeOperator opt = NodeOperator(op);
            // current_node->addChild(NodeToken(&sav_t.back(), operatorNode));
            current_node->type = sav_t.back().type;
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

    void parseExprAndOr()
    {
#ifdef PARSER_DEBUG
        updateMem();
#endif

        sav_token.push_back(current_node);

        parseExprConditionnal();

        if (Error.error == 1)
        {
            return;
        }
        while (Match(TokenDoubleUppersand) || Match(TokenDoubleOr))
        {

            sav_t.push_back(*current());
            next();

            _node_token_stack.push_back(current_node->children->back());
            // NodeToken d = current_node->children->back();
            current_node->children->pop_back();
            current_node = current_node->addChild(NodeToken(binOpNode));
            current_node->addChild(_node_token_stack.back());
            _node_token_stack.pop_back();
            // current_node->parent->children->remove(current_node->parent->children->back());
            if ((&sav_t.back())->type == TokenDoubleUppersand)
                (&sav_t.back())->type = TokenKeywordAnd;
            else
                (&sav_t.back())->type = TokenKeywordOr;
            // current_node->addChild(NodeToken(&sav_t.back(), operatorNode));
            current_node->type = sav_t.back().type;
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
#ifdef PARSER_DEBUG
        updateMem();
#endif
        sav_token.push_back(current_node);
        parseExpr();
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
            _node_token_stack.push_back(current_node->children->back());

            current_node->children->pop_back();
            current_node = current_node->addChild(NodeToken(testNode));
            // current_node->addChild(NodeToken(&sav_t.back(), operatorNode));
            current_node->type = sav_t.back().type;
            current_node->setTargetText(targetList.pop());
            // NodeToken nd;
            NodeToken nd = NodeToken(changeTypeNode);
           // nd._nodetype = changeTypeNode;
            nd.type = TokenKeywordVarType;
            nd._vartype = findfloat(_node_token_stack.back());
            if (nd._vartype != __float__)
            {
                nd._vartype = finduint32_t(_node_token_stack.back());
            }
            current_node = current_node->addChild(nd);
            change_type.push_back(current_node);
            current_node->addChild(_node_token_stack.back());
            _node_token_stack.pop_back();
            current_node = current_node->parent;
                        NodeToken nd2 = NodeToken(changeTypeNode);
           // nd._nodetype = changeTypeNode;
            nd2.type = TokenKeywordVarType;
            nd2._vartype = __none__;
            current_node = current_node->addChild(nd);
            // current_node->type=sav_t.back().type;
            change_type.push_back(current_node);
            sav_t.pop_back();
            parseExpr();
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
    void parseExpr()
    {
#ifdef PARSER_DEBUG
        updateMem();
#endif
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
            _node_token_stack.push_back(current_node->children->back());

            current_node->children->pop_back();
            current_node = current_node->addChild(NodeToken(binOpNode));
            current_node->addChild(_node_token_stack.back());
            _node_token_stack.pop_back();
            // current_node->addChild(NodeToken(&sav_t.back(), operatorNode));
            current_node->type = sav_t.back().type;
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
    {
#ifdef PARSER_DEBUG
        updateMem();
#endif

        Error.error = 0;

        if (Match(TokenStar) && Match(TokenIdentifier, 1))
        {
            _asPointer = true;

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
                if (change_type.back()->_vartype != __float__)
                {
                    if (current()->_vartype == __float__ || current()->_vartype == __uint32_t__)
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
            // current_node->addChild(NodeToken(&sav_t.back(), operatorNode));
            current_node->type = sav_t.back().type;
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
            // NodeToken nd;
            NodeToken nd = NodeToken(changeTypeNode);
            nd._nodetype = changeTypeNode;
            nd.type = TokenKeywordVarType;
            nd._vartype = current_node->_vartype;
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
            // csprintf("one est icic\n\r");
            parseExprAndOr();
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
            if (Error.error)
            {
                return;
            }
            //         if(current_node->getChildAtPos(current_node->children_size()-1)->_vartype==__float__)
            // change_type.back()->_vartype=__float__;
            if (change_type.size() > 0)
            {

                if (tmp_sav->children_size() == 0 && !(tmp_sav->asPointer))
                    change_type.back()->isPointer = tmp_sav->isPointer;
                if (change_type.back()->_vartype != __float__)
                {
                    if (tmp_sav->_vartype == __float__ || tmp_sav->_vartype == __uint32_t__)
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

        else if (Match(TokenUserDefinedVariable) && Match(TokenOpenParenthesis, 1))
        {
            sav_b = isStructFunction;
            isStructFunction = true;
           NodeToken d = NodeToken(current_node->parent->getChildAtPos(0));
            // NodeToken *par = current_node->parent->getChildAtPos(0);
            _node_token_stack.push_back(current_node->parent->getChildAtPos(0));
            // printf("num of cheiold:%d\n",current_node->parent->getChildAtPos(0)->children_size());
            // nd=NodeToken(*current_node->parent->getChildAtPos(0)->getChildAtPos(0)->getChildAtPos(0));
            //  d.addChild(NodeToken(current_node->parent->getChildAtPos(0)->getChildAtPos(0)));

            current_node = current_node->parent;

            if (d._nodetype == storeGlobalVariableNode)
                d._nodetype = globalVariableNode;
            else
                d._nodetype = localVariableNode;
            d.type = TokenUserDefinedVariableMemberFunction;
            current_node->children->pop_back();
            current_node->children->pop_back();
            nodeTokenList.push(d);
           
            // current_node->children->clear();
            current()->addText(string_format("%s._@%s", current()->getText(), current()->getText()));
            parseFunctionCall();
            if (Error.error)
            {
                return;
            }
            current_node->getChildAtPos(current_node->children_size() - 1)->getChildAtPos(2)->getChildAtPos(0)->copyChildren(_node_token_stack.back());
            _node_token_stack.pop_back();
            // if(current_node->getChildAtPos(current_node->children_size()-1)->getChildAtPos(0)->_vartype==__float__)
            // change_type.back()->_vartype=__float__;
            isStructFunction = sav_b;
            return;
        }
        else if (Match(TokenIdentifier) && Match(TokenOpenParenthesis, 1))
        {
            sav_b = isStructFunction;
            isStructFunction = false;
            parseFunctionCall();
            if (Error.error)
            {
                return;
            }
            // if(current_node->getChildAtPos(current_node->children_size()-1)->getChildAtPos(0)->_vartype==__float__)
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
                current_node->_vartype = __CRGB__;
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
            // printf("in a stirn\n");
            //  NodeToken nd; //=NodeToken();
            NodeToken nd = NodeToken(defGlobalVariableNode);
            nd._nodetype = defGlobalVariableNode;
            nd.type = TokenKeywordVarType;

            nd._vartype = __char__;
            nd.isPointer = true;
            nd.textref = all_text.addText(string_format("local_string_%d", for_if_num));
            for_if_num++;

            // nd.addChild(NodeToken(current(), stringNode));
            current_cntx->addVariable(nd);
            program.addChildFront(nd)->addChild(NodeToken(current(), stringNode));
            // printf("in a stirn end\n");
            // NodeToken *f = program.addChildFront(nd)->addChild(NodeToken(current(), stringNode));
            //  f->addChild(NodeToken(current(), stringNode));
            nd._nodetype = globalVariableNode;
            // nd.children->clear();
            if (current_node->_nodetype == changeTypeNode)
            {
                current_node->_vartype = __char__;
                current_node->isPointer = true;
            }
            current_node->addChild(nd);
            next();
            // printf("in a stirn end\n");
            return;
        }
        Error.error = 1;
        Error.error_message = string_format(" impossible to find Token %d %s %s", current()->getType(),current()->getText(), linepos().c_str());
        next();
        return;
    }
    void parseType()
    {
#ifdef PARSER_DEBUG
        updateMem();
#endif
        // resParse res;
        NodeToken _nd = NodeToken(UnknownNode);
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
        else if (Match(TokenSemicolon))
        {
            next();
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
#ifdef PARSER_DEBUG
        updateMem();
#endif
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
           NodeToken var=NodeToken(current());
            next();
            next();
            var._total_size = 1;
            if (Match(TokenNumber))
            {
                j++;
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
        struct_name = "";
        //  int _totalsize = 0;

        current_cntx = &main_context;
        current_node = &program;
        Error.error = 0;
        while (Match(TokenEndOfFile) == false)
        {
            isStructFunction = false;
            updateMem();
            if (Match(TokenUnknown))
            {
                Error.error = 1;
                Error.error_message = string_format("Unkown Token %s", linepos().c_str());
                return;
            }
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

                        if (Match(TokenUserDefinedVariable) and Match(TokenOpenParenthesis, 1))
                        {
                            isStructFunction = true;
                            NodeToken _nd = NodeToken(UnknownNode);
                            _nd._nodetype = typeNode;
                            _nd.type = TokenKeywordVarType;
                            _nd._vartype = __void__;
                            _nd.textref = EOF_TEXT;

                            nodeTokenList.push(_nd);

                            current()->addText(string_format("%s._@%s", usded.varName.c_str(), current()->getText()));
                            parseDefFunction();

                            if (Error.error)
                            {
                                return;
                            }
                            isStructFunction = false;
                        }
                        else if (Match(TokenKeywordVarType) and Match(TokenIdentifier, 1) and Match(TokenOpenParenthesis, 2))
                        {
                            isStructFunction = true;
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
                        else if (Match(TokenKeywordVarType) and Match(TokenIdentifier, 1) and !Match(TokenOpenParenthesis, 2))
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
                            while (Match(TokenComma))
                            {
                                // next();
                                memberpos++;
                                usded.starts[memberpos] = _start;

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
                            }
                            if (!Match(TokenSemicolon))
                            {
                                Error.error = 1;
                                Error.error_message = string_format("missing ; at line %s", linepos().c_str());
                                return;
                            }
                            next();
                            memberpos++;
                        }
                        else
                        {
                            Error.error = 1;
                            Error.error_message = string_format("Invalid statement line %s", linepos().c_str());
                            return;
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
                        nodeTokenList.push(nodeTokenList.get());
                        parseVariableForCreation();
                        if (Error.error)
                        {

                            return;
                        }
                        NodeToken nd = nodeTokenList.pop();
                        NodeToken _t = nodeTokenList.pop();
                        if (isExternal)
                        {
                            nd._nodetype = (int)defExtGlobalVariableNode;

                            isExternal = false;
                        }
                        else
                        {
                            nd._nodetype = (int)defGlobalVariableNode;
                        }
                        copyPrty(&_t, &nd);

                        current_node = program.addChild(nd);
                        tmp_sav = current_node;
                        current_cntx->addVariable(nd);
                        if (Match(TokenComma))
                        {
                            while (Match(TokenComma))
                            {
                                next();
                                nodeTokenList.push(nodeTokenList.get());
                                parseVariableForCreation();
                                if (Error.error)
                                {

                                    return;
                                }
                                NodeToken nd = nodeTokenList.pop();
                                NodeToken _t = nodeTokenList.pop();
                                if (isExternal)
                                {
                                    nd._nodetype = (int)defExtGlobalVariableNode;

                                    isExternal = false;
                                }
                                else
                                {
                                    nd._nodetype = (int)defGlobalVariableNode;
                                }
                                copyPrty(&_t, &nd);

                                current_node = program.addChild(nd);
                                tmp_sav = current_node;
                                current_cntx->addVariable(nd);
                            }
                            if (!Match(TokenSemicolon))
                            {
                                Error.error = 1;
                                Error.error_message = string_format("Expected ; %s", linepos().c_str());
                                next();
                                return;
                            }
                            else
                            {
                                next();
                                nodeTokenList.pop();
                                current_node = current_node->parent;
                                // return;
                            }
                        }
                        else if (Match(TokenSemicolon))
                        {
                            nodeTokenList.pop();
                            if (current_node->type == TokenUserDefinedVariable)
                            {
                                current()->addText(string_format("%s._@%s()", current_node->getVarType()->varName.c_str(), current_node->getVarType()->varName.c_str()));
                                main_context.findFunction(current());

                                if (search_result != NULL)
                                {
                                   NodeToken d = NodeToken(*current_node);
                                    d._nodetype = callConstructorNode;
                                    current_node = current_node->parent;
                                    current_node = current_node->addChild(d);
                                }
                            }

                            current_node = current_node->parent;
                            next();
                        }
                        else if (Match(TokenEqual) && Match(TokenString, 1))
                        {
                            nodeTokenList.pop();
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
                            nodeTokenList.pop();
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
                                Error.error_message = string_format("Expected } %s", linepos().c_str());
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
                            // __sav_pos = _tks->position;

                            // _tks->position = __sav_pos;
                            current_node = current_node->parent;
                        }

                        else if (Match(TokenEqual) and Match(TokenUserDefinedVariable, 1) and Match(TokenOpenParenthesis, 2) and current_node->type == TokenUserDefinedVariable)
                        {

                            nodeTokenList.pop();
                            next();
                            current()->addText(string_format("%s._@%s", current_node->getVarType()->varName.c_str(), current()->getText()));
                            NodeToken nd = NodeToken(*current_node);
                            if (current_node->_nodetype == defGlobalVariableNode)
                                nd._nodetype = globalVariableNode;
                            else
                                nd._nodetype = localVariableNode; // globalVariableNode;
                            nd.type = TokenUserDefinedVariableMemberFunction;
                            nd.isPointer = true;
                            nd._total_size = current_node->getVarType()->total_size;

                            nodeTokenList.push(nd);
                            isStructFunction = true;
                            current_node = current_node->parent;
                            parseFunctionCall();

                            if (Error.error)
                            {
                                return;
                            }
                            if (!Match(TokenSemicolon))
                            {
                                Error.error = 1;
                                Error.error_message = string_format("Missing ; %s", linepos().c_str());
                                return;
                            }
                            next();
                            //       current_node->getChildAtPos(current_node->children_size() - 1)->getChildAtPos(2)->getChildAtPos(0)->copyChildren(par);
                            isStructFunction = false;
                            Error.error = 0;
                            // current_node = current_node->parent;
                            // return;
                        }
                        else if (Match(TokenEqual))
                        {
                            nodeTokenList.pop();
                            next();
                            NodeToken nd = NodeToken(*current_node);
                            nd._nodetype = storeGlobalVariableNode;
                            current_node = current_node->parent;
                            current_node = current_node->addChild(NodeToken(assignementNode));
                            current_node->addChild(nd);

                            parseExpr();
                            if (Error.error)
                            {
                                return;
                            }
                            if (!Match(TokenSemicolon))
                            {
                                Error.error = 1;
                                Error.error_message = string_format("Missing ; %s", linepos().c_str());
                                return;
                            }

                            next();
                            current_node = current_node->parent;
                            //  current_node=current_node->parent;
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
   // printf("we are here\n");
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
            //Serial.printf("oioioioe ZZZ");
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
         Serial.printf("oioioioe");

            pushToConsole(_executecmd.error.error_message.c_str(), true);
        }
        else
        {
           // if (SCExecutable.exeExist)
            //{

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
           // }
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
        addExternalFunction("printf", "void", "char *,Args", (void *)artiPrintf);
        addExternalFunction("printfln", "void", "char *,Args", (void *)artiPrintfln);
        addExternalFunction("error", "void", "int,uint32_t,uint32_t", (void *)&showError);
    }
};
INIT_PARSER initialization_parser;
#endif