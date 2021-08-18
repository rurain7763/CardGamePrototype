#pragma once
#include"map"
#include"iString.h"
using namespace std;

typedef map<iString, map<iString, iString>> MainTitle;
typedef map<iString, MainTitle> MiddleTitle;
typedef map<iString, MiddleTitle> SubTitle;

class iTextReader
{
public:
	iTextReader();
	virtual ~iTextReader();

	iString operator[](const iString& s);
	void read(iString& data);

public:
	MainTitle* mt;
	MiddleTitle* midt;
	SubTitle* subt;
};
