#include <regex.h>
#include <stdio.h>
#include <list>
#include <string>
#pragma once
using namespace std;

#define EOF_TEXT 0


#ifndef ARDUINO
template <typename... Args>
std::string string_format(const std::string &format, Args... args)

{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    if (size_s <= 0)
    {
        throw std::runtime_error("Error during formatting.");
    }
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
#endif

enum varTypeEnum
{
    __unit8_t__,
    __unit16_t__,
    __unit32_t__,
    __int__,
    __float__,
    __void__,
    __CRGB__,

};

struct varType
{
    varTypeEnum _varType;
    uint8_t _varSize;
    string load[3];
    string store[3];
    string reg_name;
    int sizes[3];
    int size;
    int total_size;
};

string varTypeEnumNames[] =
    {
        "__unit8_t__",
        "__unit16_t__",
        "__unit32_t__",
        "__int__",
        "__float__",
        "__void__",
        "__CRGB__"

};

varType _varTypes[] =
    {
        {
            ._varType = __unit8_t__,
            ._varSize = 1,
            .load = {"l8ui"},
            .store = {"s8i"},
            .reg_name = "a",
            .sizes = {1},
            .size = 1,
            .total_size=1 
        },
        {
            ._varType = __unit16_t__,
            ._varSize = 2,
            .load = {"l16ui"},
            .store = {"s16i"},
            .reg_name = "a",
            .sizes = {1},
            .size = 1,
            .total_size=2,
        },
        {
            ._varType = __unit32_t__,
            ._varSize = 4,
            .load = {"l32i"},
            .store = {"s32i"},
            .reg_name = "a",
            .sizes = {1},
            .size = 1,
            .total_size=4,
        },
        {
            ._varType = __int__,
            ._varSize = 2,
            .load = {"l16si"},
            .store = {"s16i"},
            .reg_name = "a",
            .sizes = {1},
            .size = 1,
            .total_size=2,
        },
        {
            ._varType = __float__,
            ._varSize = 4,
            .load = {"l16si"},
            .store = {"s16i"},
            .reg_name = "a",
            .sizes = {1},
            .size = 1,
            .total_size=4,
        },
        {
            ._varType = __void__,
            ._varSize = 0,
            .load = {"l16si"},
            .store = {"s16i"},
            .reg_name = "a",
            .sizes = {0},
            .size = 0,
            .total_size=0,
        },
        {
            ._varType = __CRGB__,
            ._varSize = 3,
            .load = {"l8ui", "l8ui", "l8ui"},
            .store = {"s8i", "s8i", "s8i"},
            .reg_name = "a",
            .sizes = {1, 1, 1},
            .size = 3,
            .total_size=3,
        }

};

enum KeywordType
{
    KeywordVarType,
    KeywordExternalVar,
    KeywordFor,
    KeywordIf,
    KeywordThen,
    KeywordElse,
};

KeywordType __keywordTypes[] =
    {
        KeywordVarType,
        KeywordVarType,
        KeywordVarType,
        KeywordVarType,
        KeywordVarType,
        KeywordVarType,
        KeywordVarType,
        KeywordExternalVar,
        KeywordFor,
        KeywordIf,
        KeywordThen,
        KeywordElse,
};
string keywordTypeNames[] =
    {
        "KeywordVarType",
        "KeywordVarType",
        "KeywordVarType",
        "KeywordVarType",
        "KeywordVarType",
        "KeywordVarType",
        "KeywordVarType",
        "KeywordExternalVar",
        "KeywordFor",
        "KeywordIf",
        "KeywordThen",
        "KeywordElse",

};

string keyword_array[12] = {"uint8_t", "uint16_t", "uint32_t", "int", "float", "void", "CRGB", "__ext__", "for", "if", "then", "else"};

enum tokenType
{
    TokenNumber,
    TokenAddition,
    TokenStar,
    TokenSubstraction,
    TokenOpenParenthesis,
    TokenCloseParenthesis,
    TokenOpenBracket,
    TokenCloseBracket,
    TokenOpenCurlyBracket,
    TokenCloseCurlyBracket,
    TokenEqual,
    TokenDoubleEqual,
    TokenIdentifier,
    TokenSemicolon,
    TokenUnknown,
    TokenSpace,
    TokenNewline,
    TokenEndOfFile,
    TokenSlash,
    TokenKeyword,
    TokenString,
    TokenExternal,
    TokenComma,
    TokenLessThan,
    TokenPlusPlus,
    TokenMinusMinus,
};

string tokenNames[] = {
    "TokenNumber",
    "TokenAddition",
    "TokenStar",
    "TokenSubstraction",
    "TokenOpenParenthesis",
    "TokenCloseParenthesis",
    "TokenOpenBracket",
    "TokenCloseBracket",
    "TokenOpenCurlyBracket",
    "TokenCloseCurlyBracket",
    "TokenEqual",
    "TokenDoubleEqual",
    "TokenIdentifier",
    "TokenSemicolon",
    "TokenUnknown",
    "TokenSpace",
    "TokenNewline",
    "TokenEndOfFile",
    "TokenSlash",
    "TokenKeyword",
    "TokenString",
    "TokenExternal",
    "TokenComma",
    "TokenLessThan",
    "TokenPlusPlus",
    "TokenMinusMinus",
};

typedef struct
{
    // switch to uin8_t
    tokenType type;

    // needs to find a solution for this maybe a pointer to list string et on ne garde que les identifiers ...
    string text;

    int line;

    // switch to uin8_t unn ligne ne fera pas plus de 256 caracteres
    int pos;

    // switch to uin8_t
    KeywordType _keyword;

    varType *_vartype;

    // possible sizee  1+4+4+1+1+4=15 au lieu de 44 ...

} token;

void displaytoken(token t)
{
    // printf("%s\t%d\t%f\t%s%d %d\t%s\n", tokenNames[t.type].c_str(), t.int_value, t.float_value, t.text.c_str(), t.line, t.pos, keywordTypeNames[t._keyword].c_str());
}
token transNumber(string str)
{
    token t;
    // t.float_value=0;
    // t.int_value=0;
    if (str.find(".") != string::npos)
    {
        // try convert
        float f;
        f = 0;
        float div = 10;
        bool past_point = false;
        for (int i = 0; i < str.size(); i++)
        {
            if (str[i] == '.')
            {
                past_point = true;
            }
            else
            {
                if (past_point == true)
                {
                    f += (float)(str[i] - 48) / div;
                    div = div * 10;
                }
                else
                {
                    f = 10 * f + (str[i] - 48);
                }
            }
        }
        // t.float_value = f;
        t.type = TokenNumber;
        t._vartype = &_varTypes[(int)__float__];
        t.text = str;
        return t;
    }
    // on traite un integer
    uint16_t res;
    for (int i = 0; i < str.size(); i++)
    {
        res = 10 * res + (str[i] - 48);
    }
    // t.uint_value = res;
    t.type = TokenNumber;
    t._vartype = &_varTypes[(int)__unit16_t__];
    t.text = str;

    return t;
}

int isKeyword(string str)
{
    for (int i = 0; i < 12; i++)
    {
        if (keyword_array[i].compare(str) == 0)
        {
            return i;
        }
    }

    return -1;
}

bool isIna_zA_Z_(unsigned char c)
{
    if (c >= 97 && c <= 122)
    {
        return true;
    }
    if (c >= 65 && c <= 90)
    {
        return true;
    }
    if (c == '_')
    {
        return true;
    }
    return false;
}

bool isIna_zA_Z_0_9(unsigned char c)
{
    if (c >= 97 && c <= 122)
    {
        return true;
    }
    if (c >= 65 && c <= 90)
    {
        return true;
    }
    if (c == '_')
    {
        return true;
    }
    if (c >= 48 && c <= 57)
    {
        return true;
    }
    return false;
}

bool isIn0_9(unsigned char c)
{
    if (c >= 48 && c <= 57)
    {
        return true;
    }
    return false;
}
bool isIn0_9_x_b(unsigned char c)
{
    if (c >= 48 && c <= 57)
    {
        return true;
    }
    if (c == 'b')
    {
        return true;
    }
    if (c == 'x')
    {
        return true;
    }
    if (c == '.')
    {
        return true;
    }
    return false;
}

class Script
{
public:
    Script(string input)
    {
        script = input;
        position = -1;
    }
    char nextChar()
    {
        // //printf("%d\n",position);
        if ((position + 1) < script.size())
        {
            position++;
            return script[position];
        }
        else
        {
            position++;
            return EOF_TEXT;
        }
    }

    char currentChar()
    {
        if (position < script.size())
        {
            // position++;
            return script[position];
        }
        else
        {
            return EOF_TEXT;
        }
    }

    char previousChar()
    {
        if ((position - 1) >= 0)
        {
            position--;
            return script[position];
        }
        else
        {
            return EOF_TEXT;
        }
    }

private:
    string script;
    int position;
};

list<token> list_of_token;

class Tokens
{
public:

    Tokens()
    {
        position = 0;
        _tokens = &list_of_token;
    }
    int size()
    {
        return _tokens->size();
    }
    token *getTokenAtPos(int pos)
    {
        int i = 0;
        if (pos >= _tokens->size() || pos < 0)
        {
            return &end_token;
        }
        for (list<token>::iterator it = _tokens->begin(); it != _tokens->end(); ++it)
        {
            if (i == pos)
            {

                return &*it;
            }
            i++;
        }
        return &end_token;
    }
    token *current()
    {
        return getTokenAtPos(position);
    }
    token *next()
    {
        if (position + 1 >= _tokens->size())
        {
            position = _tokens->size() - 1;
        }
        else
        {
            position++;
        }
        return getTokenAtPos(position);
    }
    token *prev()
    {
        if (position > 0)
        {
            position--;
        }
        return getTokenAtPos(position);
    }
    token *peek(int index)
    {
        if (position + index < _tokens->size() && position + index >= 0)
        {
            return getTokenAtPos(position + index);
        }
        else
        {
            return &end_token;
        }
    }
    void init()
    {
        position = 0;
    }

    bool Match(tokenType tt)
    {
        token *g = current();
        if (g->type == tt)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool Match(tokenType tt, int index)
    {
        token *g = peek(index);
        if (g->type == tt)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool MatchKeyword(KeywordType t)
    {
        if (current()->_keyword == t)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool MatchKeyword(KeywordType t, int offset)
    {
        if (peek(offset)->_keyword == t)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

private:
    int position;
    list<token> *_tokens;
    token end_token = {.type = TokenEndOfFile};
};

Tokens _tks;
void tokenizer(Script *script)
{

   // list<token> list_of_token;
    int line = 1;
    int pos = 0;
    list_of_token.clear();

    while (script->nextChar() != EOF_TEXT)
    {

        pos++;
        char c = script->currentChar();
        // printf("line : %d pos:%d char:%c token size:%d %d\n",line,pos,c,list_of_token.size(),heap_caps_get_free_size(MALLOC_CAP_8BIT));
        if (c == '=')
        {
            token t;
            t._vartype = NULL;
            t.type = TokenEqual;
            t.line = line;
            t.pos = pos;
            t.text = "=";
            list_of_token.push_back(t);
            continue;
        }
        if (c == '<')
        {
            token t;
            t._vartype = NULL;
            t.type = TokenLessThan;
            t.line = line;
            t.pos = pos;
            t.text = "<";
            list_of_token.push_back(t);
            continue;
        }
        if (c == '+')
        {
           char  c2 = script->nextChar();
           if(c2=='+')
           {
            token t;
            t._vartype = NULL;
            t.type = TokenPlusPlus;
            t.text = "++";
            t.line = line;
            t.pos = pos;
            list_of_token.push_back(t);
            continue;
           }
           else
           {
            script->previousChar();
            token t;
            t._vartype = NULL;
            t.type = TokenAddition;
            t.text = "+";
            t.line = line;
            t.pos = pos;
            list_of_token.push_back(t);
            continue;
           }
        }

        if (isIna_zA_Z_(c))
        {
            string v;
            int newpos = pos;
            while (isIna_zA_Z_0_9(c))
            {
                v += c;
                newpos++;
                c = script->nextChar();
            }
            script->previousChar(); // on revient un caractere en arriere
            token t;
            // t._vartype=NULL;
            t.line = line;
            t.pos = pos;
            if (isKeyword(v) > -1)
            {
                // printf("keyword;%s\n",v.c_str());
                t.type = TokenKeyword;
                t._keyword = __keywordTypes[isKeyword(v)];
                if (isKeyword(v) < 7)
                    t._vartype = &_varTypes[isKeyword(v)];
                if (t._keyword == KeywordExternalVar)
                {
                    t.type = TokenExternal;
                }
            }
            else
            {
                t.type = TokenIdentifier;
            }
            pos = newpos - 1;
            t.text = v;
            list_of_token.push_back(t);
            continue;
        }

        if (isIn0_9(c))
        {
            // //printf("on a %c\n",c);
            string value = "";
            int newpos = pos;
            while (isIn0_9_x_b(c))
            {
                value += c;
                c = script->nextChar();
                newpos++;
            }
            script->previousChar(); // on revient un caractere en arriere
            token t = transNumber(value);
            //  t._vartype=NULL;
            t.line = line;
            t.pos = pos;
            list_of_token.push_back(t);
            pos = newpos - 1;
            continue;
        }
        if (c == ';')
        {
            token t;
            t.type = TokenSemicolon;
            t._vartype = NULL;
            t.text = ";";
            t.line = line;
            t.pos = pos;
            list_of_token.push_back(t);
            continue;
        }
        if (c == '(')
        {
            token t;
            t.type = TokenOpenParenthesis;
            t._vartype = NULL;
            t.text = "(";
            t.line = line;
            t.pos = pos;
            list_of_token.push_back(t);
            continue;
        }
        if (c == ')')
        {
            token t;
            t.type = TokenCloseParenthesis;
            t._vartype = NULL;
            t.text = ")";
            t.line = line;
            t.pos = pos;
            list_of_token.push_back(t);
            continue;
        }
        if (c == '{')
        {
            token t;
            t.type = TokenOpenCurlyBracket;
            t._vartype = NULL;
            t.text = "{";
            t.line = line;
            t.pos = pos;
            list_of_token.push_back(t);
            continue;
        }
        if (c == '}')
        {
            token t;
            t.type = TokenCloseCurlyBracket;
            t._vartype = NULL;
            t.text = "}";
            t.line = line;
            t.pos = pos;
            list_of_token.push_back(t);
            continue;
        }
        if (c == '[')
        {
            token t;
            t.type = TokenOpenBracket;
            t._vartype = NULL;
            t.text = "[";
            t.line = line;
            t.pos = pos;
            list_of_token.push_back(t);
            continue;
        }
        if (c == ']')
        {
            token t;
            t.type = TokenCloseBracket;
            t._vartype = NULL;
            t.text = "]";
            t.line = line;
            t.pos = pos;
            list_of_token.push_back(t);
            continue;
        }
        if (c == '/')
        {
            token t;
            t.type = TokenSlash;
            t._vartype = NULL;
            t.text = "/";
            t.line = line;
            t.pos = pos;
            list_of_token.push_back(t);
            continue;
        }
        if (c == '-')
        {
            token t;
            t.type = TokenSubstraction;
            t._vartype = NULL;
            t.text = "-";
            t.line = line;
            t.pos = pos;
            list_of_token.push_back(t);
            continue;
        }
        if (c == ' ')
        {
            token t;
            t.line = line;
            t.pos = pos;
            while (c == ' ')
            {
                c = script->nextChar();
                pos++;
            }
            script->previousChar(); // on revient un caractere en arriere
            pos--;
            t.type = TokenSpace;
            t.text = "";
            // list_of_token.push_back(t);
            continue;
        }
        if (c == '"')
        {
            string v;
            token t;
            t._vartype = NULL;
            t.line = line;
            t.pos = pos;
            c = script->nextChar();
            pos++;
            while (c != '"')
            {
                v += c;
                c = script->nextChar();
                pos++;
            }
            // script->previousChar(); //on revient un caractere en arriere
            // pos--;

            t.type = TokenString;
            t.text = v;
            list_of_token.push_back(t);
            continue;
        }
        if (c == '\n')
        {
            token t;
            t.type = TokenNewline;
            t.text = "";
            t.line = line;
            t.pos = pos;
            line++;
            pos = 0;
            // list_of_token.push_back(t);
            continue;
        }
        if (c == ';')
        {
            token t;
            t._vartype = NULL;
            t.type = TokenNewline;
            t.text = "";
            t.line = line;
            t.pos = pos;
            // line++;
            // pos=1;
            pos++;
            list_of_token.push_back(t);
            continue;
        }

        if (c == '*')
        {
            token t;
            t._vartype = NULL;
            t.type = TokenStar;
            t.text = "*";
            t.line = line;
            t.pos = pos;
            list_of_token.push_back(t);
            continue;
        }
        if (c == ',')
        {
            token t;
            t._vartype = NULL;
            t.type = TokenComma;
            t.text = ",";
            t.line = line;
            t.pos = pos;
            list_of_token.push_back(t);
            continue;
        }
        // printf("Error invalid character |%c|\n", c);
    }
    token t;
    t.type = TokenEndOfFile;
    t.text = "";
    t.line = line;
    t.pos = pos + 1;
    list_of_token.push_back(t);
    //return list_of_token;
}
