#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define RECT_WIDTH 20
#define RECT_HEIGHT 20
#define COLUMNS (WIN_WIDTH / RECT_WIDTH)
#define ROWS (WIN_HEIGHT / RECT_HEIGHT)

#define FPS 60
#define TARGET_FRAME_TIME (1000 / FPS)

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
double last_render = 0;
int ticks = 0;

enum state {
    SETUP,
    RUNNING,
    NOT_RUNNING,
};
enum state game_state = NOT_RUNNING;

typedef struct cell {
    SDL_Rect rect;
    bool alive;
} cell;
cell cells[ROWS][COLUMNS];

void init(void);
void input(void);
void update(void);
void clean(void);

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
                if (game_state == SETUP) {
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
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        if (game_state == SETUP)
                            game_state = RUNNING;
                        else
                            game_state = SETUP;
                        break;
                    case SDLK_q:
                        game_state = NOT_RUNNING;
                        break;
                    case SDLK_r:
                        if (game_state == SETUP)
                            clean();
                        break;
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

        if (game_state == RUNNING) {
            if (ticks == 0) {
                cell tmp[ROWS][COLUMNS];

                for (int i = 0; i < ROWS; i++) {
                    for (int j = 0; j < COLUMNS; j++) {
                        int acount = 0;
                        for (int k = i - 1; k <= i + 1; k++) {
                            for (int l = j - 1; l <= j + 1; l++) {
                                if (k == i && l == j) continue;
                                if (cells[(k + ROWS) % ROWS][(l + COLUMNS) % COLUMNS].alive)
                                    acount++;
                            }
                        }
                        if (cells[i][j].alive == true)
                            tmp[i][j].alive = acount > 3 || acount < 2 ? false : true;
                        else
                            tmp[i][j].alive = acount == 3 ? true : false;
                    }
                }

                for (int i = 0; i < ROWS; i++) {
                    for (int j = 0; j < COLUMNS; j++) {
                        cells[i][j].alive = tmp[i][j].alive;
                    }
                }
            }
            ticks = (ticks + 1) % 5;
        }

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

void clean(void) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            cells[i][j].alive = false;
        }
    }
}
