#include "iString.h"
#include "iStd.h"

iString::iString()
{
    size = 10;
    str = new char[size];
    str[0] = 0;
    len = 0;
}

iString::iString(const char* s)
{
    len = strlen(s);

    if(!len)
    {
        size = 10;
        str = new char[size];
        str[0] = 0;
        return;
    }

    size = len * 2;
    str = new char[size];
    memcpy(str,s,sizeof(char)*len);
    str[len] = 0;
}

iString::iString(const iString& s)
{
    len = s.len;

    if(len == 0)
    {
        size = 10;
        str = new char[size];
        str[0] = 0;
        return;
    }

    size = len * 2;
    str = new char[size];
    memcpy(str,s.str,sizeof(char)*len);
    str[len] = 0;
}

iString::~iString()
{
    delete str;
}

iString iString::operator+(const iString& s) const
{
    int tlen = len + s.len;

    char* r = new char[tlen+1];
    memcpy(r,str,sizeof(char)*len);
    memcpy(&r[len],s.str,sizeof(char)*s.len);
    r[tlen] = 0;

    return r;
}

iString& iString::operator=(const char* s)
{
    len = strlen(s);

    if(len >= size)
    {
        delete str;
        size = len*2;
        str = new char[size];
    }

    memcpy(str,s,sizeof(char)*len);
    str[len] = 0;

    return *this;
}

iString& iString::operator=(const iString& s)
{
    len = s.len;

    if(len >= size)
    {
        delete str;
        size = len*2;
        str = new char[size];
    }

    memcpy(str,s.str,sizeof(char)*len);
    str[len] = 0;

    return *this;
}

iString& iString::operator+=(const iString& s)
{
    int curLen = len;
    len += s.len;

    if(len >= size)
    {
        size = len * 2;
        char* c = new char[size];
        memcpy(c,str,sizeof(char)*curLen);
        delete str;
        str = c;
    }

    memcpy(&str[curLen],s.str,sizeof(char)*s.len);
    str[len] = 0;

    return *this;
}

iString& iString::operator+=(const char* s)
{
    int curLen = len;
    int sLen = strlen(s);
    len += sLen;

    if (len >= size)
    {
        size = len * 2;
        char* c = new char[size];
        memcpy(c, str, sizeof(char) * curLen);
        delete str;
        str = c;
    }

    memcpy(&str[curLen], s, sizeof(char) * sLen);
    str[len] = 0;

    return *this;
}

bool iString::operator==(const char* s)
{
    return !strcmp(str, s);
}

bool iString::operator==(const iString& s)
{
    return !strcmp(str, s.str);
}

bool iString::operator<(const iString& s) const
{ 
    if (len == s.len)
    {
        int lc = -1;
        int rc = 0;
        int mi = min(len, s.len);

        for (int i = 0; i<mi ; i++)
        {
            lc = str[i]; rc = s.str[i];
            if (lc != rc) break;
        }

        return len + lc < s.len + rc;
    }

    return len < s.len;
}

void iString::append(const char c)
{
    if (len+1 >= size)
    {
        size = (len + 1) * 2;
        char* c = new char[size];
        memcpy(c, str, sizeof(char) * len);
        delete str;
        str = c;
    }

    str[len] = c;
    str[len + 1] = 0;
    len += 1;
}

iString iString::subString(int s, int e) const
{
    iString r;

    for (int i = s; i <= e; i++)
    {
        r.append(str[i]);
    }

    return r;
}

void iString::find(const char* ts,int& s, int& e) const
{
    s = -1; e = -1;
    int len = strlen(ts);

    for (int i = 0; i < this->len; i++)
    {
        if (str[i] == ts[0])
        {
            bool same = true;
            int cp = 0;
            int tp = i + len;
            for (int j = i; j < tp; j++)
            {
                if (str[j] != ts[cp])
                {
                    same = false;
                    break;
                }

                cp++;
            }

            if (same)
            {
                s = i; e = i + (len-1);
                return;
            }
        }
    }
}

void iString::erase(int s, int num)
{
   int count = len - (s + num);
   for (int i = 0; i <= count; i++)
   {
       str[s] = str[s + num];
       s++;
   }

   len = strlen(str);
}

void iString::insert(int start,const char* s)
{
    int totalLen = len + strlen(s);

    char* c = new char[totalLen*2];
    memcpy(c, str, sizeof(char) * start+1);
    memcpy(&c[start], s, sizeof(char) * strlen(s));
    memcpy(&c[start + strlen(s)], &str[start], sizeof(char) * strlen(&str[start]));
    c[totalLen] = 0;

    len = totalLen;
    size = len * 2;
    delete str;
    str = c;
}

void iString::setFormattedString(const char* szFormat, ...)
{
    char szText[255];
    formating_text(szText, szFormat);
    
    int len = strlen(szText);
    if (len >= size)
    {
        delete str;
        str = new char[len * 2];
        size = len * 2;
    }

    memcpy(str, szText, sizeof(char) * len);
    str[len] = 0;
    this->len = len;
}

char** iString::getStringLine(int& num ,char ch,bool last) const
{
    iString copy = str;
    if (!last) copy.append(ch);

    int len = strlen(copy.str);

    num = 0;
    for (int i = 0; i < len; i++)
    {
        if (copy.str[i] == ch)
        {
            num++;
        }
    }

    char** r = new char* [num];

    int i, j = 0;
    int start = 0;
    for (i = 0; i < num; i++)
    {
        int size = 0;

        for (j = start; j < len; j++)
        {
            if (copy.str[j] == ch)
            {
                char* s = new char[size+1];
                memcpy(s, &copy.str[start], sizeof(char) * size);
                s[size] = 0;

                r[i] = s;
                start = j + 1;
                break;
            }

            size++;
        }
    }

    return r;
}

void iString::freeStringLine(char** line,int lineNum)
{
    for (int i = 0; i < lineNum; i++)
        delete line[i];
    delete line;
}

iString iString::upperCase(const iString& str)
{
    iString r = str;

    for (int i = 0; i < r.len; i++)
    {
        if (r.str[i] >= 'a' && r.str[i] <= 'z') r.str[i] -= 'a' - 'A';
    }

    return r;
}

iString iString::lowerCase(const iString& str)
{
    iString r = str;

    for (int i = 0; i < r.len; i++)
    {
        if (r.str[i] >= 'A' && r.str[i] <= 'Z') r.str[i] += 'a' - 'A';
    }

    return r;
}

iString* iString::makeFormattediString(const char* sFormat, ...)
{
    char szText[255];
    formating_text(szText, sFormat);
    iString* s = new iString(szText);
    return s;
}


