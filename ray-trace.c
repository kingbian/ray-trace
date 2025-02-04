
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
    int i = 0;
    for (; i < NUM_RAYS; ++i) {
        double angle = 2 * M_PI / (double)NUM_RAYS * i;
        int x = circle.x;
        int y = circle.y;
        int end_x = x + length * cos(angle);  // get the end point x
        int end_y = y + length * sin(angle);  // get the end point y
        ray[i].x = x;
        ray[i].y = y;
        ray[i].angle = angle;
    }

    printf("[%d]: rays created\n", i);
}

// void drawRays(SDL_Renderer* renderer, Circle circle, Circle shadowCircle, Rays rays[]) {
//     SDL_SetRenderDrawColor(renderer, 247, 234, 10, 0);
//
//     bool hitShadowCircle = false;
//     // Loop through a portion of the circle boundary
//     double radiusSquare = shadowCircle.radius * shadowCircle.radius;
//     for (int i = 0; i < NUM_RAYS; ++i) {
//         int stepX = rays[i].x + 1;
//         int stepY = rays[i].y + 1;
//         bool windowBorder = false;
//
//         while (!hitShadowCircle || !windowBorder) {
//             SDL_Rect point = (SDL_Rect){stepX, stepY, 1, 1};
//             SDL_RenderDrawRect(renderer, &point);
//
//             if (stepY == shadowCircle.y - shadowCircle.radius || stepX == shadowCircle.x - shadowCircle.radius) {
//                 printf("hit the SHADOW CIRCLE\n");
//                 hitShadowCircle = true;
//                 break;
//             }
//
//             if (stepX >= rays[i].endX || stepY >= rays[i].endY) {
//                 printf("Hit the end of line\n");
//                 break;
//             }
//
//             stepY += 1;
//             stepX += 1;
//         }
//         // SDL_RenderDrawLine(renderer, rays[i].x, rays[i].y, rays[i].endX, rays[i].endY);
//     }
// }

bool quadratic(float a, float b, float c, float* x0, float* x1) {
    float discr = b * b - 4 * a * c;

    if (discr < 0) {
        return false;  // unable to take the square of 0
    } else if (discr == 0) {
        *x0 = *x1 = -b / (2 * a);  // one solution that exists
    } else {
        float square_discr = sqrt(discr);

        float q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));

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

void drawRays(SDL_Renderer* renderer, Circle circle, Circle shadowCircle, Rays rays[]) {
    SDL_SetRenderDrawColor(renderer, 247, 234, 10, 255);

    for (int i = 0; i < NUM_RAYS; ++i) {
        vec2f rayDirection = {cos(rays[i].angle), sin(rays[i].angle)};
        printf("current angle: %f\n", rays[i].angle);
        vec2f origin = {rays[i].x, rays[i].y};
        vec2f L = {origin.x - shadowCircle.x, origin.y - shadowCircle.y};

        float A = rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y;
        float B = 2 * (rayDirection.x * L.x + rayDirection.y * L.y);
        float C = L.x * L.x + L.y * L.y - shadowCircle.radius * shadowCircle.radius;

        float t0, t1;

        if (quadratic(A, B, C, &t0, &t1) && t0 >= 0) {  // when t0 is >= 0, then there's an intersection point
            printf("we have an intersection point\n");
            // get intersection points

            vec2f pHit = {origin.x + rayDirection.x * t0, origin.y + rayDirection.y * t0};
            // draw the line
            SDL_RenderDrawLine(renderer, origin.x, origin.y, pHit.x, pHit.y);
        } else {  // there was no intersection

            int length = SCREEN_HEIGHT * 2;
            printf("NO intersection point\n");
            float endX = origin.x + rayDirection.x * length;
            float endY = origin.y + rayDirection.y * length;

            SDL_RenderDrawLine(renderer, origin.x, origin.y, endX, endY);
        }
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
