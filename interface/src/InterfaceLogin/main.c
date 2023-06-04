#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string.h>

#define MAX_TEXT_LENGTH 50
enum { LOGIN_FIELD, PASSWORD_FIELD } activeField = LOGIN_FIELD;

int initSDL();
SDL_Window* initWindow();
SDL_Renderer* initRender(SDL_Window* window);
TTF_Font* initFont(SDL_Window* window, SDL_Renderer* renderer);
SDL_Texture* initImage(SDL_Window* window, SDL_Renderer* renderer, char* link);
SDL_Surface* initSurface(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, char* text, SDL_Color textColor);
SDL_Texture* initTexture(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, SDL_Surface* surface);

void handleKeyboardInput(SDL_Event* event, SDL_Window* window, char* loginInputText, char* passwordInputText, char* passwordMask);
void handleMouseClick(SDL_Event* event, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* loginButtonBoxRect, char* loginInputText, char* passwordInputText, int* quit);
void handleWindowEvent(SDL_Event* event, SDL_Window *window, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* imageRect,int windowWidth, int windowHeight);

void clearScreen(SDL_Renderer* renderer);
void renderBorder(SDL_Renderer* renderer, const SDL_Rect* rect);
void renderText(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* rect);
void renderImage(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* rect);
void renderButton(SDL_Renderer* renderer, const SDL_Rect* rect);
void render(SDL_Renderer* renderer, SDL_Texture* image, SDL_Rect imageRect,
            SDL_Rect loginBoxRect, SDL_Rect passwordBoxRect, SDL_Rect cancelButtonBoxRect, SDL_Rect loginButtonBoxRect,
            SDL_Texture* loginTextTexture, SDL_Texture* passwordTextTexture, SDL_Texture* cancelButtonTextTexture, SDL_Texture* loginButtonTextTexture,
            SDL_Surface* loginInputTextSurface, SDL_Texture* loginInputTextTexture, char* loginInputText,
            int loginTextHeight, int loginTextWidth,
            SDL_Surface* passwordInputTextSurface, SDL_Texture* passwordInputTextTexture, char* passwordInputText,
            int passwordTextHeight, int passwordTextWidth,
            TTF_Font* font, SDL_Color textColor, const char* passwordMask,
            int cancelButtonTextHeight, int cancelButtonTextWidth,
            int loginButtonTextHeight, int loginButtonTextWidth);
void centerHUD(SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* imageRect,int windowWidth, int windowHeight);

void cleanUp(SDL_Texture* image, SDL_Texture* loginTextTexture, SDL_Texture* passwordTextTexture, SDL_Texture* loginButtonTextTexture, SDL_Texture* cancelButtonTextTexture, SDL_Texture* loginInputTextTexture, SDL_Texture* passwordInputTextTexture, SDL_Surface* loginTextSurface, SDL_Surface* passwordTextSurface, SDL_Surface* loginButtonTextSurface, SDL_Surface* cancelButtonTextSurface, SDL_Surface* loginInputTextSurface, SDL_Surface* passwordInputTextSurface, SDL_Window* window, TTF_Font* font, SDL_Renderer* renderer);

int main(int argc, char* argv[]) {
    //Main Variables
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font = NULL;
    SDL_Texture* image = NULL;

    //Text Variables
    char* loginText = "Login:";
    char* passwordText = "Password:";
    char* loginButtonText = "Login";
    char* cancelButtonText = "Cancelar";
    SDL_Color textColor = { 52, 52, 54 };  // Black color

    //Surface Variables
    SDL_Surface* loginTextSurface = NULL;
    SDL_Surface* passwordTextSurface = NULL;
    SDL_Surface* cancelButtonTextSurface = NULL;
    SDL_Surface* loginButtonTextSurface = NULL;

    //Init Global
    initSDL();
    window = initWindow();
    if (!window) {
        return 1;
    }
    renderer = initRender(window);
    if (!renderer) {
        return 1;
    }
    font = initFont(window, renderer);
    if (!font) {
        return 1;
    }

    //Init da imagem Cinepuc
    image = initImage(window, renderer, "../assets/image1.jpg");
    if(!image) {
        return 1;
    }

    // Init a surface for each screen text
    loginTextSurface = initSurface(window, renderer, font, loginText, textColor);
    passwordTextSurface = initSurface(window, renderer, font, passwordText, textColor);
    cancelButtonTextSurface = initSurface(window, renderer, font, cancelButtonText, textColor);
    loginButtonTextSurface = initSurface(window, renderer, font, loginButtonText, textColor);
    if (!loginTextSurface || !passwordTextSurface || !loginButtonTextSurface || !cancelButtonTextSurface) {
        SDL_FreeSurface(loginTextSurface);
        SDL_FreeSurface(passwordTextSurface);
        SDL_FreeSurface(loginButtonTextSurface);
        SDL_FreeSurface(cancelButtonTextSurface);
        return 1;
    }

    // Init textures for each surface
    SDL_Texture* loginTextTexture = initTexture(window, renderer, font, loginTextSurface);
    SDL_Texture* passwordTextTexture = initTexture(window, renderer, font, passwordTextSurface);
    SDL_Texture* loginButtonTextTexture = initTexture(window, renderer, font, loginButtonTextSurface);
    SDL_Texture* cancelButtonTextTexture = initTexture(window, renderer, font, cancelButtonTextSurface);
    if (!loginTextTexture || !passwordTextTexture || !loginButtonTextTexture || !cancelButtonTextTexture) {
        SDL_FreeSurface(loginTextSurface);
        SDL_FreeSurface(passwordTextSurface);
        SDL_FreeSurface(loginButtonTextSurface);
        SDL_FreeSurface(cancelButtonTextSurface);
        return 1;
    }

    // Get the dimensions of the rendered text
    int loginTextWidth = loginTextSurface->w;
    int loginTextHeight = loginTextSurface->h;
    int passwordTextWidth = passwordTextSurface->w;
    int passwordTextHeight = passwordTextSurface->h;
    int loginButtonTextWidth = loginButtonTextSurface->w;
    int loginButtonTextHeight =loginButtonTextSurface->h;
    int cancelButtonTextWidth = cancelButtonTextSurface->w;
    int cancelButtonTextHeight = cancelButtonTextSurface->h;

    int windowWidth, windowHeight;

    //Set upt the image size
    SDL_Rect imageRect = { 0, 0, 131, 172 };

    // Set up the rectangle for the login box
    SDL_Rect loginBoxRect = { 0, 0, 300, loginTextHeight + 20 };

    // Set up the rectangle for the password box
    SDL_Rect passwordBoxRect = { 0, 0, 300, passwordTextHeight + 20 };

    //Set up the rectangle for the login button    
    SDL_Rect loginButtonBoxRect = { 0, 0, 130, loginButtonTextHeight + 15};

    //Set up the rectangle for the cancel button
    SDL_Rect cancelButtonBoxRect = { 0, 0, 130, cancelButtonTextHeight + 15};

    //Position the HUD
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    centerHUD(&loginBoxRect, &passwordBoxRect, &loginButtonBoxRect, &cancelButtonBoxRect, &imageRect, windowWidth, windowHeight);

    // Create a buffer to hold the text
    char loginInputText[MAX_TEXT_LENGTH + 1];  // Add 1 for null-terminator
    memset(loginInputText, 0, sizeof(loginInputText));

    char passwordInputText[MAX_TEXT_LENGTH + 1];  // Add 1 for null-terminator
    memset(passwordInputText, 0, sizeof(passwordInputText));

    char passwordMask[MAX_TEXT_LENGTH + 1];

    // Create surfaces for the input texts
    SDL_Surface* loginInputTextSurface = NULL;
    SDL_Surface* passwordInputTextSurface = NULL;

    // Create textures for the input texts
    SDL_Texture* loginInputTextTexture = NULL;
    SDL_Texture* passwordInputTextTexture = NULL;

    // Main loop
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        // Process events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            else if (event.type == SDL_KEYDOWN) {
                // Handle keyboard input
                handleKeyboardInput(&event, window, loginInputText, passwordInputText, passwordMask);
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
            // Handle mouse button click
                handleMouseClick(&event, &loginBoxRect, &passwordBoxRect, &cancelButtonBoxRect, &loginButtonBoxRect, loginInputText, passwordInputText, &quit);
            }
            else if (event.type == SDL_WINDOWEVENT) {
                handleWindowEvent(&event, window, &loginBoxRect, &passwordBoxRect, &cancelButtonBoxRect, &loginButtonBoxRect, &imageRect, windowWidth, windowHeight);
            }
        }
        render(renderer, image, imageRect, loginBoxRect, passwordBoxRect, cancelButtonBoxRect, loginButtonBoxRect,
        loginTextTexture, passwordTextTexture, cancelButtonTextTexture, loginButtonTextTexture,
        loginInputTextSurface, loginInputTextTexture, loginInputText,
        loginTextHeight, loginTextWidth,
        passwordInputTextSurface, passwordInputTextTexture, passwordInputText,
        passwordTextHeight, passwordTextWidth,
        font, textColor, passwordMask,
        cancelButtonTextHeight, cancelButtonTextWidth,
        loginButtonTextHeight, loginButtonTextWidth);
    }

    // Clean up resources
    cleanUp (image, loginTextTexture, passwordTextTexture, loginButtonTextTexture, cancelButtonTextTexture, loginInputTextTexture, passwordInputTextTexture, loginTextSurface, passwordTextSurface, loginButtonTextSurface, cancelButtonTextSurface, loginInputTextSurface, passwordInputTextSurface, window, font, renderer); 

    return 0;
}

int initSDL() {
      // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() != 0) {
        SDL_Log("Failed to initialize SDL_ttf: %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }
}

SDL_Window* initWindow() {
    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Application",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800, 600,  // Set the window size
        SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return NULL;
    }

    return window;
}

SDL_Renderer* initRender(SDL_Window* window) {
    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return NULL;
    }

    return renderer;
}

TTF_Font* initFont(SDL_Window* window, SDL_Renderer* renderer) {
    TTF_Font* font = TTF_OpenFont("../assets/arial.ttf", 24);  // Replace with your font file path
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return NULL;
    }

    return font;
}

SDL_Texture* initImage(SDL_Window* window, SDL_Renderer* renderer, char* link){
    SDL_Texture* image = IMG_LoadTexture(renderer, link);
    if(!image) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return NULL;
    }
    return image;
}

SDL_Surface* initSurface(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, char* text, SDL_Color textColor) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, textColor);
    if (!surface) {
        SDL_Log("Failed to create text surface: %s", TTF_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return NULL;
    }
    return surface;
}

SDL_Texture* initTexture(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, SDL_Surface* surface) {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Failed to create text textures: %s", SDL_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return NULL;
    }
    return texture;
}

void handleKeyboardInput(SDL_Event* event, SDL_Window* window, char* loginInputText, char* passwordInputText, char* passwordMask) {
    if (event->key.keysym.sym == SDLK_BACKSPACE) {
        // Remove the last character if backspace is pressed
        if (event->window.windowID == SDL_GetWindowID(window)) {
            if (activeField == LOGIN_FIELD && strlen(loginInputText) > 0) {
                loginInputText[strlen(loginInputText) - 1] = '\0';
            }
            else if (activeField == PASSWORD_FIELD && strlen(passwordInputText) > 0) {
                passwordInputText[strlen(passwordInputText) - 1] = '\0';
                passwordMask[strlen(passwordMask) - 1] = '\0';
            }
        }
    }
    else if (event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_KP_ENTER) {
        // Handle the Enter key
        if (activeField == PASSWORD_FIELD) {
            // Handle the password input logic here
            printf("Entered login: %s\n", loginInputText);
            printf("Entered password: %s\n", passwordInputText);
        }
    }
    else if (activeField == LOGIN_FIELD && strlen(loginInputText) < MAX_TEXT_LENGTH) {
        // Append the entered character to the login input
        char character = event->key.keysym.sym;
        if (character >= SDLK_SPACE && character <= SDLK_z) {
            loginInputText[strlen(loginInputText)] = character;
        }
    }
    else if (activeField == PASSWORD_FIELD && strlen(passwordInputText) < MAX_TEXT_LENGTH) {
        // Append the entered character to the password input
        char character = event->key.keysym.sym;
        if (character >= SDLK_SPACE && character <= SDLK_z) {
            passwordInputText[strlen(passwordInputText)] = character;
            passwordMask[strlen(passwordMask)] = '*';
        }
    }
}

void handleMouseClick(SDL_Event* event, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* loginButtonBoxRect, char* loginInputText, char* passwordInputText, int* quit) {
    if (event->button.button == SDL_BUTTON_LEFT) {
        int mouseX = event->button.x;
        int mouseY = event->button.y;

        // Check if the click is within the login box
        if (mouseX >= loginBoxRect->x && mouseX <= loginBoxRect->x + loginBoxRect->w &&
            mouseY >= loginBoxRect->y && mouseY <= loginBoxRect->y + loginBoxRect->h) {
            activeField = LOGIN_FIELD;
        }
        // Check if the click is within the password box
        else if (mouseX >= passwordBoxRect->x && mouseX <= passwordBoxRect->x + passwordBoxRect->w &&
            mouseY >= passwordBoxRect->y && mouseY <= passwordBoxRect->y + passwordBoxRect->h) {
            activeField = PASSWORD_FIELD;
        }
        // Check if the click in within the cancel button box
        else if (mouseX >= cancelButtonBoxRect->x && mouseX <= cancelButtonBoxRect->x + cancelButtonBoxRect->w &&
            mouseY >= cancelButtonBoxRect->y && mouseY <= cancelButtonBoxRect->y + cancelButtonBoxRect->h) {
            *quit = 1;
        }
        //Check if the click is within the login button box
        else if (mouseX >= loginButtonBoxRect->x && mouseX <= loginButtonBoxRect->x + loginButtonBoxRect->w &&
            mouseY >= loginButtonBoxRect->y && mouseY <= loginButtonBoxRect->y + loginButtonBoxRect->h) {
            printf("Entered login: %s\n", loginInputText);
            printf("Entered password: %s\n", passwordInputText);
            fflush(stdout);
        }
    }
}

void handleWindowEvent(SDL_Event* event, SDL_Window *window, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* imageRect,int windowWidth, int windowHeight) {
    if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
        // Window resized, re-center the HUD
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        centerHUD(loginBoxRect, passwordBoxRect, loginButtonBoxRect, cancelButtonBoxRect, imageRect, windowWidth, windowHeight);
    }
}

void clearScreen(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color
    SDL_RenderClear(renderer);
}

void renderBorder(SDL_Renderer* renderer, const SDL_Rect* rect) {
    SDL_SetRenderDrawColor(renderer, 52, 52, 54, 255);  // Black color
    SDL_RenderDrawRect(renderer, rect);
}

void renderText(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* rect) {
    SDL_RenderCopy(renderer, texture, NULL, rect);
}

void renderImage(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* rect) {
    SDL_RenderCopy(renderer, texture, NULL, rect);
}

void renderButton(SDL_Renderer* renderer, const SDL_Rect* rect) {
    SDL_SetRenderDrawColor(renderer, 52, 52, 54, 255);  // Black color
    SDL_RenderDrawRect(renderer, rect);
}

void render(SDL_Renderer* renderer, SDL_Texture* image, SDL_Rect imageRect,
            SDL_Rect loginBoxRect, SDL_Rect passwordBoxRect, SDL_Rect cancelButtonBoxRect, SDL_Rect loginButtonBoxRect,
            SDL_Texture* loginTextTexture, SDL_Texture* passwordTextTexture, SDL_Texture* cancelButtonTextTexture, SDL_Texture* loginButtonTextTexture,
            SDL_Surface* loginInputTextSurface, SDL_Texture* loginInputTextTexture, char* loginInputText,
            int loginTextHeight, int loginTextWidth,
            SDL_Surface* passwordInputTextSurface, SDL_Texture* passwordInputTextTexture, char* passwordInputText,
            int passwordTextHeight, int passwordTextWidth,
            TTF_Font* font, SDL_Color textColor, const char* passwordMask,
            int cancelButtonTextHeight, int cancelButtonTextWidth,
            int loginButtonTextHeight, int loginButtonTextWidth) {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color
    SDL_RenderClear(renderer);

    // Render the login box border
    SDL_SetRenderDrawColor(renderer, 52, 52, 54, 255);  // Black color
    SDL_RenderDrawRect(renderer, &loginBoxRect);

    //Render the image
    SDL_RenderCopy(renderer, image, NULL, &imageRect);

    // Render the login text
    int loginTextPosX = loginBoxRect.x + 10;
    int loginTextPosY = loginBoxRect.y + (loginBoxRect.h - loginTextHeight) / 2;
    SDL_Rect loginTextRect = { loginBoxRect.x, loginBoxRect.y - loginTextHeight, loginTextWidth, loginTextHeight };
    SDL_RenderCopy(renderer, loginTextTexture, NULL, &loginTextRect);

    // Render the login input text if it's not empty
    if (strlen(loginInputText) > 0) {
        loginInputTextSurface = TTF_RenderText_Solid(font, loginInputText, textColor);
        if (!loginInputTextSurface) {
            SDL_Log("Failed to create login input text surface: %s", TTF_GetError());
            return;
        }

        loginInputTextTexture = SDL_CreateTextureFromSurface(renderer, loginInputTextSurface);
        if (!loginInputTextTexture) {
            SDL_Log("Failed to create login input text texture: %s", SDL_GetError());
            SDL_FreeSurface(loginInputTextSurface);
            loginInputTextSurface = NULL;
            return;
        }

        int loginInputTextWidth = loginInputTextSurface->w;
        int loginInputTextHeight = loginInputTextSurface->h;
        SDL_Rect loginInputTextRect = { loginTextPosX, loginTextPosY, loginInputTextWidth, loginInputTextHeight };
        SDL_RenderCopy(renderer, loginInputTextTexture, NULL, &loginInputTextRect);
    }

    // Render the password box border
    SDL_SetRenderDrawColor(renderer, 52, 52, 54, 255);  // Black color
    SDL_RenderDrawRect(renderer, &passwordBoxRect);

    // Render the password text
    int passwordTextPosX = passwordBoxRect.x + 10;
    int passwordTextPosY = passwordBoxRect.y + (passwordBoxRect.h - passwordTextHeight) / 2;
    SDL_Rect passwordTextRect = { passwordBoxRect.x, passwordBoxRect.y - passwordTextHeight, passwordTextWidth, passwordTextHeight };
    SDL_RenderCopy(renderer, passwordTextTexture, NULL, &passwordTextRect);

    // Render the password input text if it's not empty
    if (strlen(passwordInputText) > 0) {
        passwordInputTextSurface = TTF_RenderText_Solid(font, passwordMask, textColor);
        if (!passwordInputTextSurface) {
            SDL_Log("Failed to create password input text surface: %s", TTF_GetError());
            return;
        }

        passwordInputTextTexture = SDL_CreateTextureFromSurface(renderer, passwordInputTextSurface);
        if (!passwordInputTextTexture) {
            SDL_Log("Failed to create password input text texture: %s", SDL_GetError());
            SDL_FreeSurface(passwordInputTextSurface);
            passwordInputTextSurface = NULL;
            return;
        }

        int passwordInputTextWidth = passwordInputTextSurface->w;
        int passwordInputTextHeight = passwordInputTextSurface->h;
        SDL_Rect passwordInputTextRect = { passwordTextPosX, passwordTextPosY, passwordInputTextWidth, passwordInputTextHeight };
        SDL_RenderCopy(renderer, passwordInputTextTexture, NULL, &passwordInputTextRect);
    }

    SDL_Rect cancelButtonTextRect = { cancelButtonBoxRect.x + (cancelButtonBoxRect.w - cancelButtonTextWidth) / 2, cancelButtonBoxRect.y + (cancelButtonBoxRect.h - cancelButtonTextHeight) / 2, cancelButtonTextWidth, cancelButtonTextHeight };
    SDL_RenderCopy(renderer, cancelButtonTextTexture, NULL, &cancelButtonTextRect);

    SDL_Rect loginButtonTextRect = { loginButtonBoxRect.x + (loginButtonBoxRect.w - loginButtonTextWidth) / 2, loginButtonBoxRect.y + (loginButtonBoxRect.h - loginButtonTextHeight) / 2, loginButtonTextWidth, loginButtonTextHeight };
    SDL_RenderCopy(renderer, loginButtonTextTexture, NULL, &loginButtonTextRect);

    //Render the buttons
    SDL_SetRenderDrawColor(renderer, 52, 52, 54, 255);  // Black color
    SDL_RenderDrawRect(renderer, &loginButtonBoxRect);
    SDL_RenderDrawRect(renderer, &cancelButtonBoxRect);

    // Update the screen
    SDL_RenderPresent(renderer);
}

void centerHUD(SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* imageRect, int windowWidth, int windowHeight) {
    // Calculate the new position of the login box
    loginBoxRect->x = (windowWidth - loginBoxRect->w) / 2;
    loginBoxRect->y = (windowHeight - loginBoxRect->h - passwordBoxRect->h - 40) / 2;

    //Calculate the new position of the image box
    imageRect->x = (windowWidth - imageRect->w) / 2;
    imageRect->y = loginBoxRect->y - 220;

    // Calculate the new position of the password box
    passwordBoxRect->x = (windowWidth - passwordBoxRect->w) / 2;
    passwordBoxRect->y = loginBoxRect->y + 27 + loginBoxRect->h + 10;

    cancelButtonBoxRect->x = (windowWidth - loginBoxRect->w) / 2; 
    cancelButtonBoxRect->y = passwordBoxRect->y + passwordBoxRect->h + 27;

    loginButtonBoxRect->x = cancelButtonBoxRect->x + cancelButtonBoxRect->w + 40;
    loginButtonBoxRect->y = passwordBoxRect->y + passwordBoxRect->h + 27;
}

void cleanUp(SDL_Texture* image, SDL_Texture* loginTextTexture, SDL_Texture* passwordTextTexture, SDL_Texture* loginButtonTextTexture, SDL_Texture* cancelButtonTextTexture, SDL_Texture* loginInputTextTexture, SDL_Texture* passwordInputTextTexture, SDL_Surface* loginTextSurface, SDL_Surface* passwordTextSurface, SDL_Surface* loginButtonTextSurface, SDL_Surface* cancelButtonTextSurface, SDL_Surface* loginInputTextSurface, SDL_Surface* passwordInputTextSurface, SDL_Window* window, TTF_Font* font, SDL_Renderer* renderer) {
    SDL_DestroyTexture(image);
    SDL_DestroyTexture(loginTextTexture);
    SDL_DestroyTexture(passwordTextTexture);
    SDL_FreeSurface(loginTextSurface);
    SDL_FreeSurface(passwordTextSurface);
    SDL_DestroyTexture(loginButtonTextTexture);
    SDL_FreeSurface(loginButtonTextSurface);
    SDL_DestroyTexture(cancelButtonTextTexture);
    SDL_FreeSurface(cancelButtonTextSurface);
    SDL_DestroyTexture(loginInputTextTexture);
    SDL_FreeSurface(loginInputTextSurface);
    SDL_DestroyTexture(passwordInputTextTexture);
    SDL_FreeSurface(passwordInputTextSurface);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}