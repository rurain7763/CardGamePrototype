#include"Engine.h"
#include"Actor.h"

Engine* Engine::s = nullptr;

Engine::Engine()
{
	actors = new Actor * [MIN];
	num = 0;
	size = MIN;
}

Engine::~Engine()
{
	delete actors;
}

Engine* Engine::share()
{
	if (!s) s = new Engine();
	return s;
}

void Engine::addActor(Actor* a)
{
	if (num >= size)
	{
		Actor** c = new Actor * [size *= 2];

		for (int i = 0; i < num; i++)
		{
			c[i] = actors[i];
		}
		delete actors;

		actors = c;
	}

	actors[num] = a;
	num++;
}

void Engine::sortByOrder()
{
	for (int i = 0; i < num - 1; i++)
	{
		for (int j = i; j < num; j++)
		{
			if (actors[i]->order>actors[j]->order)
			{
				Actor* c = actors[i];
				actors[i] = actors[j];
				actors[j] = c;
			}
		}
	}
}

void Engine::start()
{
	for (int i = 0; i < num; i++)
	{
		actors[i]->start();
	}
}

void Engine::update(float dt)
{
	sortByOrder();

	for (int i = 0; i < num; i++)
	{
		if (!actors[i]->active) continue;

		actors[i]->update(dt);
		actors[i]->draw(dt);
	}
}