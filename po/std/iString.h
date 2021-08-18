#pragma once

class iString
{
public:
    iString();
    iString(const char* str);
    iString(const iString& s);
    virtual ~iString();

    iString operator+(const iString& str) const;
    iString& operator=(const char* str);
    iString& operator=(const iString& str);
    iString& operator+=(const iString& str);
    iString& operator+=(const char* str);
    bool operator==(const char* str);
    bool operator==(const iString& str);

    bool operator<(const iString& str) const;

    void append(const char c);
    iString subString(int s, int e) const;
    void find(const char* str ,int& s, int& e) const;
    void erase(int s, int num);
    void insert(int s,const char* str);
    void setFormattedString(const char* szFormat, ...);

    char** getStringLine(int& num , char ch = '\n',bool last=true) const;
    static void freeStringLine(char** line,int lineNum);
    static iString upperCase(const iString& str);
    static iString lowerCase(const iString& str);
    static iString* makeFormattediString(const char* sFormat, ...);

public:
    char* str;
    int size;
    int len;
};