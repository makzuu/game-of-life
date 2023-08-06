#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define WIN_WIDTH 640
#define WIN_HEIGHT 480
#define RECT_WIDTH 100
#define RECT_HEIGHT 100
#define RECT_X (WIN_WIDTH/2 - RECT_WIDTH/2)
#define RECT_Y (WIN_HEIGHT/2 - RECT_HEIGHT/2)

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Game of Life",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WIN_WIDTH, WIN_HEIGHT, 0);
    if (window == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    bool running = true;

    SDL_Rect rect = { RECT_X, RECT_Y, RECT_WIDTH, RECT_HEIGHT };

    while (running) {
        if (SDL_SetRenderDrawColor(renderer, 24, 24, 24, SDL_ALPHA_OPAQUE))
            fprintf(stderr, "%s\n", SDL_GetError());
        if (SDL_RenderClear(renderer))
            fprintf(stderr, "%s\n", SDL_GetError());
        if (SDL_SetRenderDrawColor(renderer, 204, 204, 204, SDL_ALPHA_OPAQUE))
            fprintf(stderr, "%s\n", SDL_GetError());
        if (SDL_RenderDrawRect(renderer, &rect))
            fprintf(stderr, "%s\n", SDL_GetError());
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: 
                    running = false;
                    printf("bai\n");
                    break;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
