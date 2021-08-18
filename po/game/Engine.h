#pragma once
#define MIN 16

class Actor;

class Engine
{
public:
	~Engine();

	static Engine* share();
	void addActor(Actor* a);
	void sortByOrder();
	void start();
	void update(float dt);

private:
	Engine();
	static Engine* s;

public:
	Actor** actors;
	int size;
	int num;
};
