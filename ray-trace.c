
#include "ray-trace.h"
/**
 *
 * function to check if the current circle
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

void drawCircle(SDL_Renderer* renderer, Circle circle) {
    for (int i = circle.x - circle.radius; i <= circle.x + circle.radius; ++i) {
        // loop over the y axis
        for (int j = circle.y - circle.radius; j <= circle.y + circle.radius; ++j) {
            double distanceX = i - circle.x;
            double distanceY = j - circle.y;
            double distanceSquared = distanceX * distanceX + distanceY * distanceY;

            if (distanceSquared < circle.radius * circle.radius) {
                SDL_Rect rectangle = (SDL_Rect){i, j, 1, 1};

                SDL_RenderFillRect(renderer, &rectangle);
            }
        }
    }
}

/**
 * use the logic to find the end point on a unit circle given theta and starting (x,y), we can find the end point of  a line given its
 * starting (x,y) and an angle (theta)
 *
 * we save the rays into array to later check for possible collisions with objects
 */
void createRays(Circle circle, Rays ray[]) {
    int length = SCREEN_HEIGHT * 2;

    for (int i = 0; i < NUM_RAYS; ++i) {
        double angle = 2 * M_PI / (double)NUM_RAYS * i;
        int x = circle.x;
        int y = circle.y;
        int end_x = x + length * cos(angle);  // get the end point x
        int end_y = y + length * sin(angle);  // get the end point y
        ray[i].x = x;
        ray[i].y = y;
        ray[i].angle = angle;
    }
}

bool quadratic(float a, float b, float c, float* x0, float* x1) {
    float discr = b * b - 4 * a * c;

    if (discr < 0) {
        return false;  // unable to take the square of 0
    } else if (discr == 0) {
        *x0 = *x1 = -0.5 * b / a;
    } else {
        float q;
        if (b > 0) {
            q = -0.5 * (b + sqrt(discr));
        } else {
            q = -0.5 * (b - sqrt(discr));
        }

        *x0 = q / a;
        *x1 = c / q;
    }

    if (*x0 > *x1) {
        int temp = *x0;
        *x0 = *x1;
        *x1 = temp;
    }

    return true;
}

void drawRays(SDL_Renderer* renderer, Circle circle, Circle shadowCircle, Rays rays[]) {
    SDL_SetRenderDrawColor(renderer, 247, 234, 10, 255);

    for (int i = 0; i < 40; ++i) {
        vec2f rayDirection = (vec2f){cos(rays[i].angle), sin(rays[i].angle)};
        vec2f origin = (vec2f){rays[i].x, rays[i].y};
    }
}

void bounce(Circle* circle) {
    circle->y += circle->vy;
    if ((circle->y - circle->radius <= 0 || circle->y + circle->radius >= SCREEN_HEIGHT)) {
        if (circle->radius + circle->y >= SCREEN_HEIGHT || circle->y >= SCREEN_HEIGHT) {
            circle->y = SCREEN_HEIGHT - circle->radius;
        } else if (circle->y - circle->radius <= 0 || circle->y <= 0) {
            circle->y = circle->radius;
        }

        circle->vy *= -1;  // change direction
    }
}
