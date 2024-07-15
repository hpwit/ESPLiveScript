
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
    /*
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
    }*/
    void clear()
    {
        _tokens->clear();
    }
    int position;

private:
    list<token> *_tokens;
    token end_token = {.type = TokenEndOfFile};
};