#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define WIN_WIDTH 640
#define WIN_HEIGHT 480
#define ROWS 10
#define COLUMNS 10
#define RECT_WIDTH (WIN_WIDTH / COLUMNS)
#define RECT_HEIGHT (WIN_HEIGHT / ROWS)

#define ARRAY_LEN(a) (sizeof(a) / sizeof(*a))

struct cell {
    SDL_Rect rect;
    bool alive;
} cells[ROWS][COLUMNS];

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

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            cells[i][j].alive = false;
            cells[i][j].rect = (SDL_Rect) { j * RECT_WIDTH, i * RECT_HEIGHT, RECT_WIDTH, RECT_HEIGHT };
        }
    }

    while (running) {
        SDL_SetRenderDrawColor(renderer, 24, 24, 24, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 204, 204, 204, SDL_ALPHA_OPAQUE);
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                if (cells[i][j].alive) {
                    SDL_RenderFillRect(renderer, &cells[i][j].rect);
                } else {
                    SDL_RenderDrawRect(renderer, &cells[i][j].rect);
                }
            }
        }

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: 
                    running = false;
                    printf("bai\n");
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        SDL_Point point = { event.button.x, event.button.y };

                        for (int i = 0; i < ROWS; i++) {
                            for (int j = 0; j < COLUMNS; j++) {
                                if (SDL_PointInRect(&point, &cells[i][j].rect)) {
                                    cells[i][j].alive = !cells[i][j].alive;
                                }
                            }
                        }
                    }
                    break;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
