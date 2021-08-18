#include"iTextReader.h"
#include"iStd.h"

iString* sliceData(iString& s, const char* title, int c)
{
	int titleLen = strlen(title);
	int dataLen = strlen(s.str);

	for (int i = 0; i < dataLen; i++)
	{
		if (s.str[i] == c)
		{
			char* cmpt = new char[titleLen + 1];
			memcpy(cmpt, &s.str[i + 1], sizeof(char) * titleLen);
			cmpt[titleLen] = 0;

			if (!strcmp(cmpt, title))
			{
				delete cmpt;
				cmpt = new char[titleLen + 2];
				memcpy(cmpt, &s.str[i + 1], sizeof(char) * titleLen);
				cmpt[titleLen] = '/';
				cmpt[titleLen + 1] = 0;

				for (int j = i + (titleLen + 2); j < dataLen; j++)
				{
					if (s.str[j] == c)
					{
						char* cmp = new char[titleLen + 2];
						memcpy(cmp, &s.str[j + 1], sizeof(char) * titleLen + 1);
						cmp[titleLen + 1] = 0;

						if (!strcmp(cmp, cmpt))
						{
							int start;
							int end;
#ifdef _WIN32
							start = i + (titleLen + 3);
							end = j - 2;
#endif // _WIN32						
							int rLen = (end - start) + 1;
							char* r = new char[rLen + 1];
							memcpy(r, &s.str[start], sizeof(char) * rLen);
							r[rLen] = 0;
							iString* anw = new iString(r);

							delete cmpt;
							delete cmp;
							return anw;
						}

						delete cmp;
					}
				}

				delete cmpt;
				Log("Data slice error : can't find end line\n");
				return NULL; // can't find end line
			}

			delete cmpt;
		}
	}

	Log("Data slice error : can't find title\n");
	return NULL; // can't find title
}

iString* findMainTitle(iString& s, const char* title)
{
	return sliceData(s, title, '[');
}

iString* findMiddleTitle(iString& s, const char* title)
{
	return sliceData(s, title, '{');
}

iString* findSubTitle(iString& s, const char* title)
{
	return sliceData(s, title, '(');
}

char* getText(iString& str, int start, int c, int ignore = ' ')
{
	int len = 0;
	char copy[255];
	for (int i = start; str.str[i] != c; i++)
	{
		if (str.str[i] == ignore) continue;
		copy[len] = str.str[i];
		len++;
	}
	copy[len] = 0;

	char* r = new char[len + 1];
	memcpy(r, copy, sizeof(char) * len + 1);

	return r;
}

iString* getTitle(iString& s, int start, int end)
{
	int dataLen = strlen(s.str);
	iString* r = new iString();

	for (int i = 0; i < dataLen; i++)
	{
		//main title
		if (s.str[i] == start)
		{
			char* title = getText(s, i + 1, end);
			int len = strlen(title);

			if (title[len - 1] == '/') continue;

			for (int j = i + (len + 4); j < dataLen; j++)
			{
				if (s.str[j] == start)
				{
					char* endTitle = getText(s, j + 1, '/');

					if (!strcmp(title, endTitle))
					{
						*r += title;
						*r += " ";
						delete endTitle;
						break;
					}

					delete endTitle;
				}
			}

			delete title;
		}
	}

	return r;
}

void readValue(iString& s, map<iString, iString>& dm)
{
	int len = strlen(s.str);

	for (int i = 0; i < len; i++)
	{
		const char c = s.str[i];

		if (c == '[' || c == '{' || c == '(')
		{
			while (i < len)
			{
				i++;
				if (s.str[i] == ']' || s.str[i] == '}' || s.str[i] == ')') break;
			}

			continue;
		}

		if (c >= 'A' && c <= 'Z' ||
			c >= 'a' && c <= 'z')
		{
			char* _idx = getText(s, i, '=');
			iString idx = _idx;
			char* _v = getText(s, i + strlen(idx.str) + 1, '\r','`');
			iString v = _v;

			dm[idx]=v;

			i += strlen(idx.str) + strlen(v.str) + 2;
			delete _idx;
			delete _v;
		}
	}
}

iTextReader::iTextReader()
{
	mt = new MainTitle();
	midt = new MiddleTitle();
	subt = new SubTitle();
}

iTextReader::~iTextReader()
{
	delete mt;
	delete midt;
	delete subt;
}

iString iTextReader::operator[](const iString& s)
{
	int pathNum = 0;
	for (int i = 0; i < s.len; i++)
	{
		if (s.str[i] == '/') pathNum++;
	}

	int lineNum = -1;
	char** line = s.getStringLine(lineNum, '/', false);

	if (pathNum == 1)
	{
		return (*mt)[line[0]][line[1]];
	}
	else if (pathNum == 2)
	{
		return (*midt)[line[0]][line[1]][line[2]];
	}
	else if (pathNum == 3)
	{
		return (*subt)[line[0]][line[1]][line[2]][line[3]];
	}

	s.freeStringLine(line, lineNum);
}

void iTextReader::read(iString& data)
{
	iString* main = getTitle(data, '[', ']');

	int lineNum = -1;
	char** mainLine = main->getStringLine(lineNum, ' ');
	for (int i = 0; i < lineNum; i++)
	{
		iString* tm = findMainTitle(data, mainLine[i]);

		iString* mid = getTitle(*tm, '{', '}');
		int midLineNum = -1;
		char** midLine = mid->getStringLine(midLineNum, ' ');
		for (int j = 0; j < midLineNum; j++)
		{
			iString* tmid = findMiddleTitle(*tm, midLine[j]);
			iString* _tmid = findMiddleTitle(*tm, midLine[j]);

			iString* sub = getTitle(*tmid, '(', ')');
			int subLineNum = -1;
			char** subLine = sub->getStringLine(subLineNum, ' ');
			for (int k = 0; k < subLineNum; k++)
			{
				iString* tsub = findSubTitle(*tmid, subLine[k]);
				readValue(*tsub, (*subt)[mainLine[i]][midLine[j]][subLine[k]]);

				int start, end = -1;
				_tmid->find(tsub->str, start, end);
				_tmid->erase(start, end - start + 1);

				delete tsub;
			}

			readValue(*_tmid, (*midt)[mainLine[i]][midLine[j]]);
			delete _tmid;

			int start, end = -1;
			tm->find(tmid->str, start, end);
			tm->erase(start, end - start + 1);

			iString::freeStringLine(subLine, subLineNum);
			delete sub;
			delete tmid;
		}

		readValue(*tm, (*mt)[mainLine[i]]);

		iString::freeStringLine(midLine, midLineNum);
		delete mid;
		delete tm;
	}

	iString::freeStringLine(mainLine, lineNum);
	delete main;
}
