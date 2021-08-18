#pragma once

class GameManager;
extern GameManager* gm;

void loadProc();
void drawProc(float dt);
void freeProc();
void keyProc();

