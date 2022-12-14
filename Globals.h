#pragma once
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define DEGTORAD 0.01745329251f
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960
#define FULLSCREEN false
#define BORDERLESS false
#define RESIZABLE true
#define VSYNC true
#define TITLE "Super Awesome Engine"
#define GLSL_VERSION "#version 460"