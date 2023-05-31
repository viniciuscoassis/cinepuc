#include <stdio.h>
#include <SDL2/SDL.h>

#define HEIGHT 50
#define WIDTH 50
#define TOTAL_WIDTH ((WIDTH + GRID_SPACING) * (CHAIR_COL_AMOUNT) - GRID_SPACING)
#define TOTAL_HEIGHT ((HEIGHT + GRID_SPACING) * (CHAIR_LINE_AMOUNT) - GRID_SPACING)
#define GRID_SPACING 10
#define CHAIR_LINE_AMOUNT 8
#define CHAIR_COL_AMOUNT 8
#define CHAIR_AMOUNT (CHAIR_LINE_AMOUNT * CHAIR_COL_AMOUNT)
#define STARTX ((WINDOW_WIDTH - TOTAL_WIDTH) / 2)
#define STARTY ((WINDOW_HEIGHT - TOTAL_HEIGHT) / 2)

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Rect chairRects[CHAIR_AMOUNT];
int selectedChair = -1;

void drawChairMenu();
void handleMouseClick(int x, int y);
void handleResize(int width, int height);
void render();

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Chair Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    drawChairMenu();
    render();

    int running = 1;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT)
                        handleMouseClick(event.button.x, event.button.y);
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        int width = event.window.data1;
                        int height = event.window.data2;
                        handleResize(width, height);
                    }
            }
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void drawChairMenu()
{
    int counter = 0;

    for (int i = 0; i < CHAIR_LINE_AMOUNT; i++)
    {
        for (int j = 0; j < CHAIR_COL_AMOUNT; j++)
        {
            SDL_Rect chairRect = {
                STARTX + j * (WIDTH + GRID_SPACING),
                STARTY + i * (HEIGHT + GRID_SPACING),
                WIDTH,
                HEIGHT};
            chairRects[counter++] = chairRect;
        }
    }
}

void handleMouseClick(int x, int y)
{
    for (int i = 0; i < CHAIR_AMOUNT; i++)
    {
        if (x >= chairRects[i].x && x <= chairRects[i].x + chairRects[i].w &&
            y >= chairRects[i].y && y <= chairRects[i].y + chairRects[i].h)
        {
            selectedChair = i;
            break;
        }
    }

    render();
}

void handleResize(int width, int height)
{
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    SDL_SetWindowSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    drawChairMenu();
    render();
}

void render()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < CHAIR_AMOUNT; i++)
    {
        if (i == selectedChair)
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else
            SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);

        SDL_RenderFillRect(renderer, &chairRects[i]);
    }

    SDL_RenderPresent(renderer);
}