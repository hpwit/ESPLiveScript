#include <vector>
#include <string>
// #include "tokenizer.h"
#pragma once
using namespace std;

#include "c_parser_helper.h"

class Parser
{

public:
NodeToken root;
Parser (){}

Parser(Tokens tks)
{
    _tks=tks;
}

int size()
{
    return _tks.size();
}
token * getTokenAtPos(int pos)
{
    return _tks.getTokenAtPos(pos);
}
token * current()
{
    return _tks.current();
}
token * next()
{
    return _tks.next();
}
token * prev()
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
bool Match(tokenType tt,int index)
{
    return _tks.Match(tt,index);
}
bool MatchKeyword(KeywordType t)
{
    return _tks.MatchKeyword(t);
}
bool MatchKeyword(KeywordType t, int offset)
{
    return _tks.MatchKeyword(t,offset);
}

  resParse parse()
    {
        root._nodetype = Program;
        _tks.init();
        return parseProgram();
    }

   resParse parseFunction()
    {
        printf("entering function\n");
        resParse result;
        if (Match(TokenKeyword) && (current()->_keyword == KeywordVarType))
        {
            token *t = next();
            if (Match(TokenIdentifier))
            {

                if (Match(TokenOpenParenthesis, 1) && Match(TokenCloseParenthesis, 2))
                {

                    Context cntx;
                    cntx.name = t->text;
                    //printf("entering f %d %s %s %x\n", current_cntx->_global->children.size(), current_cntx->_global->name.c_str(), t->text.c_str(), (uint64_t)current_cntx->_global);
                    Context *k = current_cntx->addChild(cntx);

                   // printf("entering funion %s|\n", k->name.c_str());
                    current_cntx = k;
                   // printf("entern\n");
                    stack_size = 32;
                    block_statement_num = 0;
                    next();
                    next();
                    next();
                    

                    //next();
                     NodeDefFunction f = NodeDefFunction(t);
                     f.stack_pos = stack_size;
                        result.error.error = 0;
                        result._nd = f;
                        //printf("exit function\n");
                        return result;
                    /*
                    resParse res = parseBlockStatement();
                    //printf("afer function1\n");
                    if (res.error.error == 0)
                    {
                        //printf("afer function\n");
                        NodeDefFunction f = NodeDefFunction(res._nd, t);
                        f.stack_pos = stack_size;
                        result.error.error = 0;
                        result._nd = f;
                        //printf("exit function\n");
                        return result;
                    }
                    else
                    {
                        return res;
                    }
                    */
                }
                else
                {

                    // test if variable;
                    result.error.error = 1;
                    result.error.error_message = "() expected";
                    return result;
                }
            }
            else
            {
                result.error.error = 1;
                result.error.error_message = "function name expected";
                return result;
            }
        }
        else
        {
            if (Match(TokenEndOfFile))
            {
                result.error.error = 0;
                return result;
            }

            result.error.error = 1;
            result.error.error_message = "Int or Float expected";
            return result;
        }
    }



 resParse parseProgram()
    {
    
        NodeProgram program;
        Context cntx = Context();
        cntx.name = "main_prog";
        cntx._global = &cntx;
        current_cntx = &cntx;
        program.cntx = &cntx;
        // Context ff=Context();
        // cntx.addChild(ff);
        resParse result;
        while (Match(TokenEndOfFile) == false)
        {
            
            if (Match(TokenKeyword) && Match(TokenIdentifier, 1) && Match(TokenOpenParenthesis, 2))
            {
                resParse res = parseFunction();
                if (res.error.error == 0)
                {
                    program.addChild(res._nd);
                }
                else
                {
                    return res;
                }
            } 
            printf("not function\n");
            next();
        }
        result._nd = program;
        result.error.error = 0;
        printf("exit programm\n");

        return result;
    }
private:
Tokens _tks;

};