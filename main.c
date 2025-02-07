#include "ray-trace.h"

int main(void) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Ray Trace", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        printf("Unable to create the sdl window error: %s\n", SDL_GetError());
        return 1;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Unable to create renderer error: %s\n", SDL_GetError());
        return 0;
    }

    int w, h;

    SDL_GetWindowSize(window, &w, &h);

    Rays ray[NUM_RAYS];
    Circle circle = {740.0f, 100.0f, 40};

    Circle shadow = {250.0f, 150.0f, 150, 0, 0.1f};  // change 1 to increase/decrease speed
    createRays(&circle, ray);
    bool running = true;
    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            // do actions with the event
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    printf("quitting the program\n");
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_q) {
                        printf("Q pressed, Quittig\n");
                        running = false;
                    }
                    break;
                case SDL_MOUSEMOTION:               // get the dragging motion of the mouse
                    if (event.motion.state == 1) {  // get the left mouse button only
                        circle.x = event.motion.x;
                        circle.y = event.motion.y;
                    }
                    break;
                default:
                    // printf("Event is happening\n");
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 87, 85, 83, 0);
        SDL_RenderClear(renderer);  // clears the background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        checkBounds(&circle);
        drawCircle(renderer, &circle, false);
        drawCircle(renderer, &shadow, true);
        bounce(&shadow);

        createRays(&circle, ray);
        drawRays(renderer, &shadow, ray);
        /*         drawRays(renderer, circle, shadow); */
        SDL_RenderPresent(renderer);
    }

    // delay
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
