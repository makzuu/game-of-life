#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define WIN_WIDTH 640
#define WIN_HEIGHT 480
#define ROWS 10
#define COLUMNS 10
#define RECT_WIDTH (WIN_WIDTH / COLUMNS)
#define RECT_HEIGHT (WIN_HEIGHT / ROWS)

#define FPS 30
#define TARGET_FRAME_TIME (1000 / FPS)

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
double last_render = 0;

enum state {
    SETUP,
    RUNNING,
    NOT_RUNNING,
};
enum state game_state = NOT_RUNNING;

struct cell {
    SDL_Rect rect;
    bool alive;
} cells[ROWS][COLUMNS];

void init(void);
void input(void);
void update(void);

int main(void) {

    init();

    while (game_state != NOT_RUNNING) {
        input();
        update();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void init(void) {
    printf("iniciando...\n");
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "%s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow("Game of Life",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WIN_WIDTH, WIN_HEIGHT, 0);
    if (window == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            cells[i][j].alive = false;
            cells[i][j].rect = (SDL_Rect) { j * RECT_WIDTH, i * RECT_HEIGHT, RECT_WIDTH, RECT_HEIGHT };
        }
    }

    game_state = SETUP;
}

void input(void) {
    printf("eventando\n");
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game_state = NOT_RUNNING;
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

void update(void) {
        double elapsed = SDL_GetTicks() - last_render;

        if (elapsed < TARGET_FRAME_TIME) {
            SDL_Delay(TARGET_FRAME_TIME - elapsed);
        }
        elapsed = SDL_GetTicks() - last_render;
        double delta_time = elapsed / 1000.0f;

        printf("delta time: %lf\n", delta_time);

        last_render = SDL_GetTicks();

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
}
