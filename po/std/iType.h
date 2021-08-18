#pragma once

#define SCENE_NONE 0
#define SCENE_MENU 1
#define SCENE_PROC 2

typedef unsigned char uint8;
typedef unsigned int uint32;

typedef void (*SCENE_METHOD)();
typedef void (*SCENE_UPDATE_METHOD)(float dt);

#define TOP		1
#define LEFT	2
#define BOTTOM	4
#define RIGHT	8

#define VCENTER 16
#define HCENTER 32

#define REVERSE_NONE	0
#define REVERSE_WIDTH	1
#define REVERSE_HEIGHT	2