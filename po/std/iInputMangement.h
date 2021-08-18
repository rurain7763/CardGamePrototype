#pragma once
#include"iVector2D.h"

#define MOUSE_NONE	0
#define MOUSE_LDOWN	1
#define MOUSE_LUP	2
#define MOUSE_MOVE	4

enum KeySate
{
	KeySateDown = 0,
	KeyStateUP
};

#define KEY_NONE	0
#define KEY_SPACE	1

class iInputManagement
{
public:
	virtual ~iInputManagement();

	void update();
	void mouseUpdate(unsigned int ms,iVector2D mp);
	void keyUpdate(KeySate ks, int k);

	static iInputManagement* share();

private:
	iInputManagement();
	static iInputManagement* s;
public:
	unsigned int ms;
	unsigned int mso;
	iVector2D mp;

	unsigned int buf;
	unsigned int ks;
	unsigned int kso;
};