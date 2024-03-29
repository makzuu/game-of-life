#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define RECT_WIDTH 20
#define RECT_HEIGHT 20
#define COLUMNS (WIN_WIDTH / RECT_WIDTH)
#define ROWS (WIN_HEIGHT / RECT_HEIGHT)

#define FPS 30
#define TARGET_FRAME_TIME (1000 / FPS)

typedef struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color;

typedef enum state {
    SETUP,
    RUNNING,
    NOT_RUNNING,
} state;
state game_state = NOT_RUNNING;

typedef struct cell {
    SDL_Rect rect;
    bool alive;
} cell;

cell cells[ROWS][COLUMNS];

int init(SDL_Window **, SDL_Renderer **);
void set_cells();
void input();
void update(SDL_Renderer *);
void destroy_everything(SDL_Window *, SDL_Renderer *);

int main(void) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    int init_status = init(&window, &renderer);
    if (init_status) {
        fprintf(stderr, "%s\n", SDL_GetError());
        destroy_everything(window, renderer);
        exit(1);
    }

    set_cells();
    game_state = SETUP;

    while (game_state != NOT_RUNNING) {
        input();
        update(renderer);
    }

    destroy_everything(window, renderer);
    return 0;
}

int init(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return 1;

    *window = SDL_CreateWindow("Game of Life",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WIN_WIDTH, WIN_HEIGHT, 0);
    if (*window == NULL)
        return 1;

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL)
        return 1;

    return 0;
}

void set_cells() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            cells[i][j].alive = false;
            cells[i][j].rect = (SDL_Rect) {
                .x = (j * RECT_WIDTH),
                .y = (i * RECT_HEIGHT),
                .w = RECT_WIDTH,
                .h = RECT_HEIGHT
            };
        }
    }
}

void set_cells_to_original_state() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            cells[i][j].alive = false;
        }
    }
}

void toggle_cell_state(SDL_Point *point) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (SDL_PointInRect(point, &cells[i][j].rect)) {
                cells[i][j].alive = !cells[i][j].alive;
            }
        }
    }
}

void input() {
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
                        toggle_cell_state(&point);
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
                            set_cells_to_original_state();
                        break;
                }
                break;
        }
    }
}

void wait_till_ready(void) {
    static int last_render = 0;
    int elapsed = SDL_GetTicks() - last_render;

    if (elapsed < TARGET_FRAME_TIME) {
        SDL_Delay(TARGET_FRAME_TIME - elapsed);
    }
    last_render = SDL_GetTicks();
}

void compute_next_generation() {
    static int ticks = 0;
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

void draw_cells(SDL_Renderer *renderer, color *colors) {
    if (game_state == SETUP)
        SDL_SetRenderDrawColor(renderer, colors[1].r, colors[1].g, colors[1].b, SDL_ALPHA_OPAQUE);
    if (game_state == RUNNING)
        SDL_SetRenderDrawColor(renderer, colors[2].r, colors[2].g, colors[2].b, SDL_ALPHA_OPAQUE);

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (cells[i][j].alive) {
                SDL_RenderFillRect(renderer, &cells[i][j].rect);
            } else {
                SDL_RenderDrawRect(renderer, &cells[i][j].rect);
            }
        }
    }
}

void update(SDL_Renderer *renderer) {
    wait_till_ready();

    if (game_state == RUNNING) {
        compute_next_generation();
    }

    color colors[] = {
        { .r = 10, .g = 10, .b = 10 },    /* background */
        { .r = 204, .g = 204, .b = 204 }, /* foreground */
        { .r = 65, .g = 105, .b = 225 },  /* foreground 2 */
    };

    SDL_SetRenderDrawColor(renderer, colors[0].r, colors[0].g, colors[0].b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    draw_cells(renderer, colors);

    SDL_RenderPresent(renderer);
}

void destroy_everything(SDL_Window *window, SDL_Renderer *renderer) {
    if (window != NULL)
        SDL_DestroyWindow(window);
    if (renderer != NULL)
        SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
