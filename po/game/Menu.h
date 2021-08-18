#pragma once

extern const char* playerName;

void loadMenu();
void drawMenu(float dt);
void freeMenu();
void keyMenu();

void loadMainMenu();
void freeMainMenu();
void drawMainMenu(float dt);
void showMainMenu(bool show);

void loadSubMenuExit();
void freeSubMenuExit();
void drawSubMenuExit(float dt);
void showSubMenuExit(bool show);
