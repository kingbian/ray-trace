#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define NUM_RAYS 300

// define ray struct
typedef struct Rays {
    int x, y;
    int angle;
} Rays;

typedef struct Circle {
    double x;
    double y;
    double radius;
    double vx, vy;
} Circle;

// define struct to hold 2D intersection points
typedef struct vec2f {
    float x, y;
} vec2f;

void checkBounds(Circle* circle);
void drawCircle(SDL_Renderer* renderer, Circle circle);
void createRays(Circle circle, Rays ray[]);
bool quadratic(float a, float b, float c, float* x0, float* x1);
void drawRays(SDL_Renderer* renderer, Circle circle, Circle shadowCircle, Rays rays[]);
void bounce(Circle* circle);
