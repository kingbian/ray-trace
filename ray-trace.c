
#include "ray-trace.h"

#include <stdlib.h>

/**
 *
 * check if the current circle
 * is out of bounds of the window
 *
 */
void checkBounds(Circle* circle) {
    if (circle->x + circle->radius >= SCREEN_WIDTH || circle->x - circle->radius <= 0) {
        if (circle->x >= SCREEN_WIDTH || circle->x + circle->radius >= SCREEN_WIDTH) {
            circle->x = SCREEN_WIDTH - circle->radius;
        } else if (circle->x - circle->radius <= 0 || circle->x <= 0) {
            circle->x = circle->radius;
        }
    }
    if ((circle->y - circle->radius <= 0 || circle->y + circle->radius >= SCREEN_HEIGHT)) {
        if (circle->radius + circle->y >= SCREEN_HEIGHT || circle->y >= SCREEN_HEIGHT) {
            circle->y = SCREEN_HEIGHT - circle->radius;
        } else if (circle->y - circle->radius <= 0 || circle->y <= 0) {
            circle->y = circle->radius;
        }
    }
}

/*
 * draw circle to the renderer
 */
void drawCircle(SDL_Renderer* renderer, const Circle* circle, bool shadowCircle) {
    if (shadowCircle) {
        SDL_SetRenderDrawColor(renderer, 33, 33, 32, 255);
    }

    int r = circle->radius;
    int x = 0;
    int y = -r;
    int p = 1 - r;  // simplified p=x^2 + y^2 -r^2

    // get center x and y
    int cx = circle->x;
    int cy = circle->y;

    while (x < -y) {
        if (p > 0) {  // pixel pts is outside the circle
            y += 1;
            p += 2 * x + 2 * y + 1;
        } else {
            p += 2 * x + 1;  // simplified
        }

        // draw line
        SDL_RenderDrawLine(renderer, cx, cy, cx + x, cy + y);
        SDL_RenderDrawLine(renderer, cx, cy, cx - x, cy + y);
        SDL_RenderDrawLine(renderer, cx, cy, cx + x, cy - y);
        SDL_RenderDrawLine(renderer, cx, cy, cx - x, cy - y);

        SDL_RenderDrawLine(renderer, cx, cy, cx + y, cy + x);
        SDL_RenderDrawLine(renderer, cx, cy, cx - y, cy + x);
        SDL_RenderDrawLine(renderer, cx, cy, cx + y, cy - x);
        SDL_RenderDrawLine(renderer, cx, cy, cx - y, cy - x);
        x += 1;
    }
}

/**
 * find the end point of line (x1,y1) on given theta and starting (x,y)
 *
 *  rays into array to later check for possible collisions with objects
 */
void createRays(const Circle* circle, Rays ray[]) {
    int i = 0;
    for (; i < NUM_RAYS; ++i) {
        float angle = 2 * M_PI / (float)NUM_RAYS * i;
        int x = circle->x;
        int y = circle->y;
        ray[i].x = x;
        ray[i].y = y;
        ray[i].angle = angle;
    }
}
/**
 * find intersection points given a quadratic equation
 */
bool quadratic(float a, float b, float c, float* x0, float* x1) {
    float discr = b * b - 4 * a * c;

    if (discr < 0) {
        return false;  // no roots or solution exists
    } else if (discr == 0) {
        *x0 = *x1 = -b / (2 * a);  // one solution that exists
    } else {
        float square_discr = sqrt(discr);

        float q = (b > 0) ? -0.5 * (b + square_discr) : -0.5 * (b - square_discr);  // account for any loss of precision

        *x0 = q / a;
        *x1 = c / q;
    }

    if (*x0 > *x1) {
        float temp = *x0;
        *x0 = *x1;
        *x1 = temp;
    }

    return true;
}

/**
 * dray rays from light source
 */
void drawRays(SDL_Renderer* renderer, const Circle* shadowCircle, const Rays rays[]) {
    SDL_SetRenderDrawColor(renderer, 247, 234, 10, 255);

    for (int i = 0; i < NUM_RAYS; ++i) {
        vec2f rayDirection = {cos(rays[i].angle), sin(rays[i].angle)};
        vec2f origin = {rays[i].x, rays[i].y};
        vec2f originToCenter = {origin.x - shadowCircle->x, origin.y - shadowCircle->y};

        float A = rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y;
        float B = 2 * (rayDirection.x * originToCenter.x + rayDirection.y * originToCenter.y);
        float C = originToCenter.x * originToCenter.x + originToCenter.y * originToCenter.y - shadowCircle->radius * shadowCircle->radius;

        float intersectPt0, intersectPt1;

        if (quadratic(A, B, C, &intersectPt0, &intersectPt1) && intersectPt0 >= 0) {  // when intersectPt0 is >= 0, then there's at least one intersection point or tangent
            // get intersection point intersectPt0 = distance to hit on shadow circle
            vec2f pHit = {origin.x + rayDirection.x * intersectPt0, origin.y + rayDirection.y * intersectPt0};
            // draw the tangent ray
            SDL_RenderDrawLine(renderer, origin.x, origin.y, pHit.x, pHit.y);
        } else if (intersectPt0 <= 0 && intersectPt1 >= 0) {
            // 2 intersect point exist, meaning the light source is behind the shadow object
            continue;
        } else {  // there was no intersection

            int length = SCREEN_HEIGHT * 2;
            float endX = origin.x + rayDirection.x * length;
            float endY = origin.y + rayDirection.y * length;
            // draw whole ray
            SDL_RenderDrawLine(renderer, origin.x, origin.y, endX, endY);
        }
    }
}

/**
 * create vertical bounce motion for a given Circle object
 */
void bounce(Circle* circle) {
    circle->y += circle->vy;

    if (circle->radius + circle->y >= SCREEN_HEIGHT || circle->y >= SCREEN_HEIGHT) {
        circle->y = SCREEN_HEIGHT - circle->radius;
        circle->vy *= -1;  // change direction
    } else if (circle->y - circle->radius <= 0 || circle->y <= 0) {
        circle->y = circle->radius;
        circle->vy *= -1;  // change direction
    }
}
