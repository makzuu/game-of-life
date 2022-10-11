#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define ALIVE   69
#define DEAD     0

int HEIGHT;
int WIDTH;

void render(int world[HEIGHT][WIDTH]);
void dourthing(int world[HEIGHT][WIDTH]);
void clear(void);
void generate(int world[HEIGHT][WIDTH]);
void usage(void);

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("usage:\n\t./life <HEIGHT> <WIDTH>\n");
        return 1;
    }
    HEIGHT = atoi(argv[1]);
    WIDTH = atoi(argv[2]);

    int world[HEIGHT][WIDTH];
    generate(world);

    for (;;)
    {
        render(world);
        clear();
        usleep(100000);
        dourthing(world);
    }
}

void generate(int world[HEIGHT][WIDTH])
{
    srand(time(NULL));
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            world[i][j] = rand() % 2 == 0 ? DEAD : ALIVE;
}

void clear(void)
{
    char clear[10];
    sprintf(clear, "\033[%dA", HEIGHT);
    printf(clear);
}

void dourthing(int world[HEIGHT][WIDTH])
{
    int tmpworld[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            int acount = 0;
            for (int x = i - 1; x <= i + 1; x++)
            {
                for (int y = j - 1; y <= j + 1; y++)
                {
                    int xx = (x + HEIGHT) % HEIGHT;
                    int yy = (y + WIDTH) % WIDTH;

                    if (world[xx][yy] == ALIVE)
                        ++acount;
                }
            }
            if (world[i][j] == ALIVE)
                --acount;

            /* 1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
             * 2. Any live cell with two or three live neighbours lives on to the next generation.
             * 3. Any live cell with more than three live neighbours dies, as if by overpopulation.
             * 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
             * */

            if (acount < 2 || acount > 3)
                tmpworld[i][j] = DEAD;
            else if (acount == 3)
                tmpworld[i][j] = ALIVE;
            else
                tmpworld[i][j] = world[i][j];

        }
    }

    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            world[i][j] = tmpworld[i][j];
}

void render(int world[HEIGHT][WIDTH])
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (world[i][j] == ALIVE)
                printf("%2c", '#');
            else
                printf("%2c", '.');
        }
        printf("\n");
    }
}
