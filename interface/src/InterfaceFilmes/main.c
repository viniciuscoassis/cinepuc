// sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev
// gcc -o main main.c -lSDL2 -lSDL2_image -lSDL2_ttf
//./main
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define ROWS 2
#define COLS 2

#define MOVIE_WIDTH 200
#define MOVIE_HEIGHT 300
#define PADDING 100

typedef struct
{
    SDL_Texture *image;
    char name[50];
} Movie;

Movie movies[ROWS][COLS];

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Movie Selection", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load movie images
    movies[0][0].image = IMG_LoadTexture(renderer, "assets/image.jpg");
    movies[0][1].image = IMG_LoadTexture(renderer, "assets/image1.jpg");
    movies[1][0].image = IMG_LoadTexture(renderer, "assets/image2.jpg");
    movies[1][1].image = IMG_LoadTexture(renderer, "assets/image3.jpg");

    // Set movie names
    strcpy(movies[0][0].name, "Super Mario Bros - O Filme");
    strcpy(movies[0][1].name, "Guardioes da Galaxia: Volume 3");
    strcpy(movies[1][0].name, "Velozes e Furiosos 10");
    strcpy(movies[1][1].name, "Avatar: O Caminho da Agua");

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("assets/arial/arial.ttf", 24);
    if (!font)
    {
        printf("Failed to load font: %s\n", TTF_GetError());
        return -1;
    }

    int totalMovieWidth = COLS * MOVIE_WIDTH;
    int totalMovieHeight = ROWS * MOVIE_HEIGHT;
    int totalTextHeight = ROWS * 30;
    int totalPaddingWidth = (COLS - 1) * PADDING;
    int totalPaddingHeight = (ROWS - 1) * PADDING;
    int gridWidth = totalMovieWidth + totalPaddingWidth;
    int gridHeight = totalMovieHeight + totalPaddingHeight + totalTextHeight;

    int offsetX = (WINDOW_WIDTH - gridWidth) / 2;
    int offsetY = (WINDOW_HEIGHT - gridHeight) / 2;

    SDL_Rect movieRect = {0, 0, MOVIE_WIDTH, MOVIE_HEIGHT};
    SDL_Rect textRect[2][2];

    int textWidth, textHeight;
    int maxTextWidth = 0; // Maximum text width across all rows

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            TTF_SizeText(font, movies[row][col].name, &textWidth, &textHeight);

            if (textWidth > maxTextWidth)
            {
                maxTextWidth = textWidth;
            }

            textRect[row][col].x = 0;
            textRect[row][col].y = MOVIE_HEIGHT;
            textRect[row][col].w = textWidth;
            textRect[row][col].h = 30;
        }
    }

    SDL_Event event;
    int running = 1;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
        SDL_RenderClear(renderer);

        for (int row = 0; row < ROWS; row++)
        {
            for (int col = 0; col < COLS; col++)
            {
                int x;
                if (maxTextWidth > MOVIE_WIDTH)
                {
                    x = offsetX + col * (MOVIE_WIDTH + PADDING + (maxTextWidth - MOVIE_WIDTH));
                }
                else
                {
                    x = offsetX + col * (MOVIE_WIDTH + PADDING);
                }
                int y = offsetY + row * (MOVIE_HEIGHT + PADDING + 30);

                movieRect.x = x;
                movieRect.y = y;
                textRect[row][col].x = x + (MOVIE_WIDTH - textRect[row][col].w) / 2; // Center horizontally within image box
                textRect[row][col].y = y + MOVIE_HEIGHT;

                SDL_RenderCopy(renderer, movies[row][col].image, NULL, &movieRect);

                SDL_Color textColor = {0, 0, 0}; // Black color
                SDL_Surface *textSurface = TTF_RenderText_Blended(font, movies[row][col].name, textColor);
                SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

                SDL_RenderCopy(renderer, textTexture, NULL, &textRect[row][col]);

                SDL_DestroyTexture(textTexture);
                SDL_FreeSurface(textSurface);
            }
        }

        SDL_RenderPresent(renderer);
    }

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            SDL_DestroyTexture(movies[row][col].image);
        }
    }

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
