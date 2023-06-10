#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string.h>

#define MAX_TEXT_LENGTH 50
enum { LOGIN_SCREEN, REGISTER_SCREEN, MOVIE_SCREEN } currentScreen = LOGIN_SCREEN;
enum { LOGIN_FIELD, PASSWORD_FIELD, CONFIRM_FIELD } activeField = LOGIN_FIELD;
enum { SHOW, HIDE } showPassword = SHOW;
enum { SHOW_CONFIRM, HIDE_CONFIRM  } showConfirmPassword = SHOW;

int initSDL();
SDL_Window* initWindow();
SDL_Renderer* initRender(SDL_Window* window);
TTF_Font* initFont(SDL_Window* window, SDL_Renderer* renderer);
SDL_Texture* initImage(SDL_Window* window, SDL_Renderer* renderer, char* link);
SDL_Surface* initSurface(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, char* text, SDL_Color textColor);
SDL_Texture* initTexture(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, SDL_Surface* surface);

void handleKeyboardInput(SDL_Event* event, SDL_Window* window, char* loginInputText, char* passwordInputText, char* passwordMask, char* confirmPasswordInputText, char* confirmPasswordMask);
void handleMouseClick(SDL_Event* event, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* confirmPasswordBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* registerButtonBoxRect, SDL_Rect* viewImageRect, SDL_Rect* hideImageRect, SDL_Rect* confirmViewImageRect, SDL_Rect* confirmHideImageRect, char* loginInputText, char* passwordInputText, char* confirmPasswordInputText, char* passwordMask, char* confirmPasswordMask, int* quit);
void handleWindowEvent(SDL_Event* event, SDL_Window *window, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* confirmPasswordBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* cinepucImageRect, SDL_Rect* registerButtonBoxRect, SDL_Rect* viewImageRect, SDL_Rect* hideImageRect, SDL_Rect* confirmViewImageRect, SDL_Rect* confirmHideImageRect, int windowWidth, int windowHeight);

void clearScreen(SDL_Renderer* renderer);

void renderLoginScreen(SDL_Renderer* renderer, TTF_Font* font, SDL_Color textColor,
            SDL_Texture* cinepucImage, SDL_Rect cinepucImageRect, SDL_Texture* viewImage, SDL_Rect viewImageRect, SDL_Texture* hideImage, SDL_Rect hideImageRect,
            SDL_Rect loginBoxRect, SDL_Rect passwordBoxRect, SDL_Rect cancelButtonBoxRect, SDL_Rect loginButtonBoxRect, SDL_Rect registerButtonBoxRect,
            SDL_Texture* loginTextTexture, SDL_Texture* passwordTextTexture, SDL_Texture* cancelButtonTextTexture, SDL_Texture* loginButtonTextTexture, SDL_Texture* registerButtonTextTexture,
            SDL_Surface* loginInputTextSurface, SDL_Texture* loginInputTextTexture, SDL_Surface* passwordInputTextSurface, SDL_Texture* passwordInputTextTexture,
            char* passwordInputText, char* loginInputText, int loginTextHeight, int loginTextWidth, int passwordTextHeight, int passwordTextWidth, const char* passwordMask,
            int cancelButtonTextHeight, int cancelButtonTextWidth, int loginButtonTextHeight, int loginButtonTextWidth, int registerButtonTextHeight, int registerButtonTextWidth);
void centerLoginHUD(SDL_Rect* cinepucImageRect, SDL_Rect* viewImageRect, SDL_Rect* hideImageRect, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* registerButtonBoxRect, int windowWidth, int windowHeight);

void renderRegisterScreen(SDL_Renderer* renderer, TTF_Font* font, SDL_Color textColor,
            SDL_Texture* cinepucImage, SDL_Rect cinepucImageRect, SDL_Texture* viewImage, SDL_Rect viewImageRect, SDL_Texture* hideImage, SDL_Rect hideImageRect, SDL_Rect confirmViewImageRect, SDL_Rect confirmHideImageRect,
            SDL_Rect loginBoxRect, SDL_Rect passwordBoxRect, SDL_Rect confirmPasswordBoxRect, SDL_Rect cancelButtonBoxRect, SDL_Rect loginButtonBoxRect,
            SDL_Texture* loginTextTexture, SDL_Texture* passwordTextTexture, SDL_Texture* confirmPasswordTextTexture, SDL_Texture* cancelButtonTextTexture, SDL_Texture* loginButtonTextTexture,
            SDL_Surface* loginInputTextSurface, SDL_Texture* loginInputTextTexture,
            SDL_Surface* passwordInputTextSurface, SDL_Texture* passwordInputTextTexture,
            SDL_Surface* confirmPasswordInputTextSurface, SDL_Texture* confirmPasswordInputTextTexture,
            char* passwordInputText, char* loginInputText, char* confirmPasswordInputText,
            int loginTextHeight, int loginTextWidth,
            int passwordTextHeight, int passwordTextWidth, const char* passwordMask,
            int confirmPasswordTextHeight, int confirmPasswordTextWidth, const char* confirmPasswordMask,
            int cancelButtonTextHeight, int cancelButtonTextWidth,
            int loginButtonTextHeight, int loginButtonTextWidth);
void centerRegisterHUD(SDL_Rect* cinepucImageRect, SDL_Rect* viewImageRect, SDL_Rect* hideImageRect, SDL_Rect* confirmViewImageRect, SDL_Rect* confirmHideImageRect, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* confirmPasswordBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* cancelButtonBoxRect, int windowWidth, int windowHeight);

void renderMovieScreen(SDL_Renderer* renderer);

void cleanUp(SDL_Texture* cinepucImage, SDL_Texture* viewImage, SDL_Texture* hideImage, SDL_Texture* loginTextTexture, SDL_Texture* passwordTextTexture, SDL_Texture* loginButtonTextTexture, SDL_Texture* cancelButtonTextTexture, SDL_Texture* registerButtonTextTexture, SDL_Texture* loginInputTextTexture, SDL_Texture* passwordInputTextTexture, SDL_Surface* loginTextSurface, SDL_Surface* passwordTextSurface, SDL_Surface* loginButtonTextSurface, SDL_Surface* cancelButtonTextSurface, SDL_Surface* registerButtonTextSurface, SDL_Surface* loginInputTextSurface, SDL_Surface* passwordInputTextSurface, SDL_Surface* confirmPasswordTextSurface, SDL_Texture* confirmPasswordTextTexture, SDL_Surface* confirmPasswordInputTextSurface, SDL_Texture* confirmPasswordInputTextTexture, SDL_Window* window, TTF_Font* font, SDL_Renderer* renderer);

int main(int argc, char* argv[]) {
    //Init Global
    initSDL();
    SDL_Window* window = initWindow();
    if (!window) {
        return 1;
    }
    SDL_Renderer* renderer = initRender(window);
    if (!renderer) {
        return 1;
    }
    TTF_Font* font = initFont(window, renderer);
    if (!font) {
        return 1;
    }

    //Login Text Variables
    char* loginText = "E-mail:";
    char* passwordText = "Senha:";
    char* loginButtonText = "Confirmar";
    char* cancelButtonText = "Cancelar";
    char* registerButtonText = "Cadastrar";
    //Register Text Variables
    char* confirmPasswordText = "Confirmar senha:";
    SDL_Color textColor = { 52, 52, 54 };  // Black color


    //Init da imagem Cinepuc
    SDL_Texture* cinepucImage = initImage(window, renderer, "../assets/image1.jpg");
    if(!cinepucImage) {
        return 1;
    }
    //Init da imagem do Olho
    SDL_Texture* viewImage = initImage(window, renderer, "../assets/view.png");
    if(!viewImage) {
        return 1;
    }
    //Init da imagem do Olho "Fechado"
    SDL_Texture* hideImage = initImage(window, renderer, "../assets/hide.png");
    if(!hideImage) {
        return 1;
    }

    // Init a surface for each screen text
    // Login Screen
    SDL_Surface* loginTextSurface = initSurface(window, renderer, font, loginText, textColor);
    SDL_Surface* passwordTextSurface = initSurface(window, renderer, font, passwordText, textColor);
    SDL_Surface* cancelButtonTextSurface = initSurface(window, renderer, font, cancelButtonText, textColor);
    SDL_Surface* loginButtonTextSurface = initSurface(window, renderer, font, loginButtonText, textColor);
    SDL_Surface* registerButtonTextSurface = initSurface(window,renderer, font, registerButtonText, textColor);
    //Register Screen
    SDL_Surface* confirmPasswordTextSurface = initSurface(window, renderer, font, confirmPasswordText, textColor);
    if (!loginTextSurface || !passwordTextSurface || !loginButtonTextSurface || !cancelButtonTextSurface || !registerButtonTextSurface || !confirmPasswordTextSurface) {
        SDL_FreeSurface(loginTextSurface);
        SDL_FreeSurface(passwordTextSurface);
        SDL_FreeSurface(loginButtonTextSurface);
        SDL_FreeSurface(cancelButtonTextSurface);
        SDL_FreeSurface(registerButtonTextSurface);
        SDL_FreeSurface(confirmPasswordTextSurface);
        return 1;
    }

    // Init textures for each surface
    //Login Screen
    SDL_Texture* loginTextTexture = initTexture(window, renderer, font, loginTextSurface);
    SDL_Texture* passwordTextTexture = initTexture(window, renderer, font, passwordTextSurface);
    SDL_Texture* loginButtonTextTexture = initTexture(window, renderer, font, loginButtonTextSurface);
    SDL_Texture* cancelButtonTextTexture = initTexture(window, renderer, font, cancelButtonTextSurface);
    SDL_Texture* registerButtonTextTexture = initTexture(window, renderer, font, registerButtonTextSurface);
    //Register Screen
    SDL_Texture* confirmPasswordTextTexture = initTexture(window, renderer, font, confirmPasswordTextSurface);
    
    if (!loginTextTexture || !passwordTextTexture || !loginButtonTextTexture || !cancelButtonTextTexture || !registerButtonTextTexture || !confirmPasswordTextTexture) {
        SDL_FreeSurface(loginTextSurface);
        SDL_FreeSurface(passwordTextSurface);
        SDL_FreeSurface(loginButtonTextSurface);
        SDL_FreeSurface(cancelButtonTextSurface);
        SDL_FreeSurface(registerButtonTextSurface);
        SDL_FreeSurface(confirmPasswordTextSurface);
        return 1;
    }

    // Get the dimensions of the rendered text
    //Login Screen
    int loginTextWidth = loginTextSurface->w;
    int loginTextHeight = loginTextSurface->h;
    int passwordTextWidth = passwordTextSurface->w;
    int passwordTextHeight = passwordTextSurface->h;
    int loginButtonTextWidth = loginButtonTextSurface->w;
    int loginButtonTextHeight =loginButtonTextSurface->h;
    int cancelButtonTextWidth = cancelButtonTextSurface->w;
    int cancelButtonTextHeight = cancelButtonTextSurface->h;
    int registerButtonTextWidth = registerButtonTextSurface->w;
    int registerButtonTextHeight = registerButtonTextSurface->h;
    //Register Screen
    int confirmPasswordTextWidth = confirmPasswordTextSurface->w;
    int confirmPasswordTextHeight = confirmPasswordTextSurface->h;


    int windowWidth, windowHeight;

    //Login Screen
    //Set up the cinepuc image size
    SDL_Rect cinepucImageRect = { 0, 0, 131, 172 };

    //Set up the view image size
    SDL_Rect viewImageRect = { 0, 0, 24, 24 };

    //Set up the hide image size
    SDL_Rect hideImageRect = { 0, 0, 24, 24 };

    // Set up the rectangle for the login box
    SDL_Rect loginBoxRect = { 0, 0, 300, loginTextHeight + 20 };

    // Set up the rectangle for the password box
    SDL_Rect passwordBoxRect = { 0, 0, 300, passwordTextHeight + 20 };

    //Set up the rectangle for the login button    
    SDL_Rect loginButtonBoxRect = { 0, 0, 130, loginButtonTextHeight + 15};

    //Set up the rectangle for the cancel button
    SDL_Rect cancelButtonBoxRect = { 0, 0, 130, cancelButtonTextHeight + 15};

    //Set up the rectangle for the register button
    SDL_Rect registerButtonBoxRect = { 0, 0, 300, registerButtonTextHeight + 15};

    //Register Screen
    //Set up the rectangle for the confirm password box
    SDL_Rect confirmPasswordBoxRect = { 0, 0, 300, confirmPasswordTextHeight + 20 };

    //Set up the view image size for the confirm password input box
    SDL_Rect confirmViewImageRect = { 0, 0, 24, 24 };

    //Set up the hide image size
    SDL_Rect confirmHideImageRect = { 0, 0, 24, 24 };


    // Create a buffer to hold the text
    //Login Screen
    char loginInputText[MAX_TEXT_LENGTH + 1];  // Add 1 for null-terminator
    memset(loginInputText, 0, sizeof(loginInputText));

    char passwordInputText[MAX_TEXT_LENGTH + 1];  // Add 1 for null-terminator
    memset(passwordInputText, 0, sizeof(passwordInputText));

    char passwordMask[MAX_TEXT_LENGTH + 1];
    //Register Screen
    char confirmPasswordInputText[MAX_TEXT_LENGTH + 1];  // Add 1 for null-terminator
    memset(passwordInputText, 0, sizeof(passwordInputText));

    char confirmPasswordMask[MAX_TEXT_LENGTH + 1];

    //Login Screen
    // Create surfaces for the input texts
    SDL_Surface* loginInputTextSurface = NULL;
    SDL_Surface* passwordInputTextSurface = NULL;

    // Create textures for the input texts
    SDL_Texture* loginInputTextTexture = NULL;
    SDL_Texture* passwordInputTextTexture = NULL;

    //Register Screen
    SDL_Surface* confirmPasswordInputTextSurface = NULL;
    SDL_Texture* confirmPasswordInputTextTexture = NULL;

    // Main loop
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        // Process events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            else if (event.type == SDL_KEYDOWN || event.type == SDL_TEXTINPUT) {
                // Handle keyboard input
                handleKeyboardInput(&event, window, loginInputText, passwordInputText, passwordMask, confirmPasswordInputText, confirmPasswordMask);
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
            // Handle mouse button click
                handleMouseClick(&event, &loginBoxRect, &passwordBoxRect, &confirmPasswordBoxRect, &cancelButtonBoxRect, &loginButtonBoxRect, &registerButtonBoxRect, &viewImageRect,
                &hideImageRect, &confirmViewImageRect, &confirmHideImageRect, loginInputText, passwordInputText, confirmPasswordInputText, passwordMask, confirmPasswordMask, &quit);
            }
            else if (event.type == SDL_WINDOWEVENT) {
                handleWindowEvent(&event, window, &loginBoxRect, &passwordBoxRect, &confirmPasswordBoxRect, &cancelButtonBoxRect, &loginButtonBoxRect, &cinepucImageRect, &registerButtonBoxRect, &viewImageRect, &hideImageRect, &confirmViewImageRect, &confirmHideImageRect, windowWidth, windowHeight);
            }
        }
        if (currentScreen == LOGIN_SCREEN) {
            SDL_GetWindowSize(window, &windowWidth, &windowHeight);
            centerLoginHUD(&cinepucImageRect, &viewImageRect, &hideImageRect, &loginBoxRect, &passwordBoxRect, &loginButtonBoxRect, &cancelButtonBoxRect, &registerButtonBoxRect, windowWidth, windowHeight);
            renderLoginScreen(renderer, font, textColor,
            cinepucImage, cinepucImageRect, viewImage, viewImageRect, hideImage, hideImageRect,
            loginBoxRect, passwordBoxRect, cancelButtonBoxRect, loginButtonBoxRect, registerButtonBoxRect,
            loginTextTexture, passwordTextTexture, cancelButtonTextTexture, loginButtonTextTexture, registerButtonTextTexture,
            loginInputTextSurface, loginInputTextTexture, 
            passwordInputTextSurface, passwordInputTextTexture,
            passwordInputText, loginInputText, loginTextHeight, loginTextWidth,
            passwordTextHeight, passwordTextWidth, passwordMask,
            cancelButtonTextHeight, cancelButtonTextWidth,
            loginButtonTextHeight, loginButtonTextWidth,
            registerButtonTextHeight, registerButtonTextWidth);
        }
        else if (currentScreen == REGISTER_SCREEN) {
            SDL_GetWindowSize(window, &windowWidth, &windowHeight);
            centerRegisterHUD(&cinepucImageRect, &viewImageRect, &hideImageRect, &confirmViewImageRect, &confirmHideImageRect, &loginBoxRect, &passwordBoxRect, &confirmPasswordBoxRect, &loginButtonBoxRect, &cancelButtonBoxRect, windowWidth, windowHeight);
            renderRegisterScreen(renderer, font, textColor,
            cinepucImage, cinepucImageRect, viewImage, viewImageRect, hideImage, hideImageRect, confirmViewImageRect, confirmHideImageRect,
            loginBoxRect, passwordBoxRect, confirmPasswordBoxRect, cancelButtonBoxRect, loginButtonBoxRect,
            loginTextTexture, passwordTextTexture, confirmPasswordTextTexture, cancelButtonTextTexture, loginButtonTextTexture,
            loginInputTextSurface, loginInputTextTexture, 
            passwordInputTextSurface, passwordInputTextTexture,
            confirmPasswordInputTextSurface, confirmPasswordInputTextTexture,
            passwordInputText, loginInputText, confirmPasswordInputText,
            loginTextHeight, loginTextWidth,
            passwordTextHeight, passwordTextWidth, passwordMask,
            confirmPasswordTextHeight, confirmPasswordTextWidth, confirmPasswordMask,
            cancelButtonTextHeight, cancelButtonTextWidth,
            loginButtonTextHeight, loginButtonTextWidth);
        }
        else if (currentScreen == MOVIE_SCREEN) {
            renderMovieScreen(renderer);
        }
    }

    // Clean up resources
    cleanUp (cinepucImage, viewImage, hideImage, loginTextTexture, passwordTextTexture, loginButtonTextTexture, cancelButtonTextTexture, registerButtonTextTexture, loginInputTextTexture, passwordInputTextTexture, loginTextSurface, passwordTextSurface, loginButtonTextSurface, cancelButtonTextSurface, registerButtonTextSurface, loginInputTextSurface, passwordInputTextSurface, confirmPasswordTextSurface, confirmPasswordTextTexture, confirmPasswordInputTextSurface, confirmPasswordInputTextTexture, window, font, renderer);
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
    return 0;
}

SDL_Window* initWindow() {
    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "Cinepuc",
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
    SDL_Texture* cinepucImage = IMG_LoadTexture(renderer, link);
    if(!cinepucImage) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return NULL;
    }
    return cinepucImage;
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

void handleKeyboardInput(SDL_Event* event, SDL_Window* window, char* loginInputText, char* passwordInputText, char* passwordMask, char* confirmPasswordInputText, char* confirmPasswordMask) {
    if (currentScreen == LOGIN_SCREEN) {
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
                fflush(stdout);
                currentScreen = MOVIE_SCREEN;
            }
        }
        else if (event->key.keysym.sym == SDLK_TAB) {
            if (activeField == PASSWORD_FIELD) {
                activeField = LOGIN_FIELD;
            } else if (activeField == LOGIN_FIELD) {
                activeField = PASSWORD_FIELD;
            }
        }
        else if (activeField == LOGIN_FIELD && strlen(loginInputText) < MAX_TEXT_LENGTH) {
            // Append the entered character to the login input
            if (event->type == SDL_TEXTINPUT) {
                strcat(loginInputText, event->text.text);
            }
        }
        else if (activeField == PASSWORD_FIELD && strlen(passwordInputText) < MAX_TEXT_LENGTH) {
            // Append the entered character to the password input
            if (event->type == SDL_TEXTINPUT) {
                strcat(passwordInputText, event->text.text);
                strcat(passwordMask, "*");
            }
        }
    }
    else if (currentScreen == REGISTER_SCREEN) {
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
                else if (activeField == CONFIRM_FIELD && strlen(confirmPasswordInputText) > 0) {
                    confirmPasswordInputText[strlen(confirmPasswordInputText) - 1] = '\0';
                    confirmPasswordMask[strlen(confirmPasswordMask) - 1] = '\0';
                }
            }
        }
        else if (event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_KP_ENTER) {
            // Handle the Enter key
            if (activeField == PASSWORD_FIELD) {
                // Handle the password input logic here
                printf("Entered login: %s\n", loginInputText);
                printf("Entered password: %s\n", passwordInputText);
                printf("Entered confirm password: %s\n", confirmPasswordInputText);
                fflush(stdout);
                if(strcmp(passwordInputText, confirmPasswordInputText) == 0) {
                    currentScreen = LOGIN_SCREEN;
                    strcpy(loginInputText, "");
                    strcpy(passwordInputText, "");
                    strcpy(confirmPasswordInputText, "");
                    strcpy(passwordMask, "");
                    strcpy(confirmPasswordMask, "");
                    showPassword = SHOW;
                }
            }
        }
        else if (event->key.keysym.sym == SDLK_TAB) {
            if (activeField == LOGIN_FIELD) {
                activeField = PASSWORD_FIELD;
            }
            else if (activeField == PASSWORD_FIELD) {
                activeField = CONFIRM_FIELD;
            }
            else if (activeField == CONFIRM_FIELD) {
                activeField = LOGIN_FIELD;
            }
        }
        else if (activeField == LOGIN_FIELD && strlen(loginInputText) < MAX_TEXT_LENGTH) {
            // Append the entered character to the login input
            if (event->type == SDL_TEXTINPUT) {
                strcat(loginInputText, event->text.text);
            }
        }
        else if (activeField == PASSWORD_FIELD && strlen(passwordInputText) < MAX_TEXT_LENGTH) {
            // Append the entered character to the password input
            if (event->type == SDL_TEXTINPUT) {
                strcat(passwordInputText, event->text.text);
                strcat(passwordMask, "*");
            }
        }
        else if (activeField == CONFIRM_FIELD && strlen(confirmPasswordInputText) < MAX_TEXT_LENGTH) {
            // Append the entered character to the confirm password input
            if (event->type == SDL_TEXTINPUT) {
                strcat(confirmPasswordInputText, event->text.text);
                strcat(confirmPasswordMask, "*");
            }
        }
    }  
    else if (currentScreen == MOVIE_SCREEN) {
        //
        //
        //
    }
}

void handleMouseClick(SDL_Event* event, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* confirmPasswordBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* registerButtonBoxRect, SDL_Rect* viewImageRect, SDL_Rect* hideImageRect, SDL_Rect* confirmViewImageRect, SDL_Rect* confirmHideImageRect, char* loginInputText, char* passwordInputText, char* confirmPasswordInputText, char* passwordMask, char* confirmPasswordMask, int* quit) {
    if (event->button.button == SDL_BUTTON_LEFT) {
        int mouseX = event->button.x;
        int mouseY = event->button.y;

        if (currentScreen == LOGIN_SCREEN) {
            // Check if the click is within the login box
            if (mouseX >= loginBoxRect->x && mouseX <= loginBoxRect->x + loginBoxRect->w &&
                mouseY >= loginBoxRect->y && mouseY <= loginBoxRect->y + loginBoxRect->h) {
                activeField = LOGIN_FIELD;
            }
            //Check if the click is within the view Image
            else if (mouseX >= viewImageRect->x && mouseX <= viewImageRect->x + viewImageRect->w &&
                mouseY >= viewImageRect->y && mouseY <= viewImageRect->y + viewImageRect->h && showPassword == HIDE) {
                showPassword = SHOW;
            }
            //Check if the click is within the hide Image
            else if (mouseX >= hideImageRect->x && mouseX <= hideImageRect->x + hideImageRect->w &&
                mouseY >= hideImageRect->y && mouseY <= hideImageRect->y + hideImageRect->h && showPassword == SHOW) {
                showPassword = HIDE;
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
            //Check if the click is within the confirm button box
            else if (mouseX >= loginButtonBoxRect->x && mouseX <= loginButtonBoxRect->x + loginButtonBoxRect->w &&
                mouseY >= loginButtonBoxRect->y && mouseY <= loginButtonBoxRect->y + loginButtonBoxRect->h) {
                printf("Entered login: %s\n", loginInputText);
                printf("Entered password: %s\n", passwordInputText);
                fflush(stdout);
                currentScreen = MOVIE_SCREEN;
            }
            //Check if the click is within the register button box
            else if (mouseX >= registerButtonBoxRect->x && mouseX <= registerButtonBoxRect->x + registerButtonBoxRect->w &&
                mouseY >= registerButtonBoxRect->y && mouseY <= registerButtonBoxRect->y + registerButtonBoxRect->h) {
                fflush(stdout);
                currentScreen = REGISTER_SCREEN;
                strcpy(loginInputText, "");
                strcpy(passwordInputText, "");
                strcpy(confirmPasswordInputText, "");
                strcpy(passwordMask, "");
                strcpy(confirmPasswordMask, "");
                showPassword = SHOW;
                showConfirmPassword = SHOW_CONFIRM;
            }
        }
        else if (currentScreen == REGISTER_SCREEN) {
            // Check if the click is within the login box
            if (mouseX >= loginBoxRect->x && mouseX <= loginBoxRect->x + loginBoxRect->w &&
                mouseY >= loginBoxRect->y && mouseY <= loginBoxRect->y + loginBoxRect->h) {
                activeField = LOGIN_FIELD;
            }
            //Check if the click is within the view Image
            else if (mouseX >= viewImageRect->x && mouseX <= viewImageRect->x + viewImageRect->w &&
                mouseY >= viewImageRect->y && mouseY <= viewImageRect->y + viewImageRect->h && showPassword == HIDE) {
                showPassword = SHOW;
            }
            //Check if the click is within the hide Image
            else if (mouseX >= hideImageRect->x && mouseX <= hideImageRect->x + hideImageRect->w &&
                mouseY >= hideImageRect->y && mouseY <= hideImageRect->y + hideImageRect->h && showPassword == SHOW) {
                showPassword = HIDE;
            }
            //Check if the click is within the Register view Image
            else if (mouseX >= confirmViewImageRect->x && mouseX <= confirmViewImageRect->x + confirmViewImageRect->w &&
                mouseY >= confirmViewImageRect->y && mouseY <= confirmViewImageRect->y + confirmViewImageRect->h && showConfirmPassword == HIDE) {
                showConfirmPassword = SHOW;
            }
            //Check if the click is within the Register hide Image
            else if (mouseX >= confirmHideImageRect->x && mouseX <= confirmHideImageRect->x + confirmHideImageRect->w &&
                mouseY >= confirmHideImageRect->y && mouseY <= confirmHideImageRect->y + confirmHideImageRect->h && showConfirmPassword == SHOW) {
                showConfirmPassword = HIDE;
            }
            // Check if the click is within the password box
            else if (mouseX >= passwordBoxRect->x && mouseX <= passwordBoxRect->x + passwordBoxRect->w &&
                mouseY >= passwordBoxRect->y && mouseY <= passwordBoxRect->y + passwordBoxRect->h) {
                activeField = PASSWORD_FIELD;
            }
            // Check if the click is within the confirm password box
            else if (mouseX >= confirmPasswordBoxRect->x && mouseX <= confirmPasswordBoxRect->x + confirmPasswordBoxRect->w &&
                mouseY >= confirmPasswordBoxRect->y && mouseY <= confirmPasswordBoxRect->y + confirmPasswordBoxRect->h) {
                activeField = CONFIRM_FIELD;
            }
            // Check if the click in within the cancel button box
            else if (mouseX >= cancelButtonBoxRect->x && mouseX <= cancelButtonBoxRect->x + cancelButtonBoxRect->w &&
                mouseY >= cancelButtonBoxRect->y && mouseY <= cancelButtonBoxRect->y + cancelButtonBoxRect->h) {
                currentScreen = LOGIN_SCREEN;
                strcpy(loginInputText, "");
                strcpy(passwordInputText, "");
                strcpy(confirmPasswordInputText, "");
                strcpy(passwordMask, "");
                strcpy(confirmPasswordMask, "");
                showPassword = SHOW;
            }
            //Check if the click is within the confirm button box
            else if (mouseX >= loginButtonBoxRect->x && mouseX <= loginButtonBoxRect->x + loginButtonBoxRect->w &&
                mouseY >= loginButtonBoxRect->y && mouseY <= loginButtonBoxRect->y + loginButtonBoxRect->h) {
                printf("Entered login: %s\n", loginInputText);
                printf("Entered password: %s\n", passwordInputText);
                printf("Entered confirm password: %s\n", confirmPasswordInputText);
                fflush(stdout);
                if(strcmp(passwordInputText, confirmPasswordInputText) == 0) {
                    currentScreen = LOGIN_SCREEN;
                    strcpy(loginInputText, "");
                    strcpy(passwordInputText, "");
                    strcpy(confirmPasswordInputText, "");
                    strcpy(passwordMask, "");
                    strcpy(confirmPasswordMask, "");
                    showPassword = SHOW;
                }
            }
        }
        else if (currentScreen == MOVIE_SCREEN) {
            //
            //
            //
            //
        }
    }
}

void handleWindowEvent(SDL_Event* event, SDL_Window *window, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* confirmPasswordBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* cinepucImageRect, SDL_Rect* registerButtonBoxRect, SDL_Rect* viewImageRect, SDL_Rect* hideImageRect, SDL_Rect* confirmViewImageRect, SDL_Rect* confirmHideImageRect, int windowWidth, int windowHeight) {
    if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
        // Window resized, re-center the HUD
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        if (currentScreen == LOGIN_SCREEN) {
            centerLoginHUD(cinepucImageRect, viewImageRect, hideImageRect, loginBoxRect, passwordBoxRect, loginButtonBoxRect, cancelButtonBoxRect, registerButtonBoxRect, windowWidth, windowHeight);
        }
        else if (currentScreen == REGISTER_SCREEN) {
            centerRegisterHUD(cinepucImageRect, viewImageRect, hideImageRect, confirmViewImageRect, confirmHideImageRect, loginBoxRect, passwordBoxRect, confirmPasswordBoxRect, loginButtonBoxRect, cancelButtonBoxRect, windowWidth, windowHeight);
        }
        else if (currentScreen == MOVIE_SCREEN) {

        }
    }
}

void clearScreen(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color
    SDL_RenderClear(renderer);
}

void renderLoginScreen(SDL_Renderer* renderer, TTF_Font* font, SDL_Color textColor,
            SDL_Texture* cinepucImage, SDL_Rect cinepucImageRect, SDL_Texture* viewImage, SDL_Rect viewImageRect, SDL_Texture* hideImage, SDL_Rect hideImageRect,
            SDL_Rect loginBoxRect, SDL_Rect passwordBoxRect, SDL_Rect cancelButtonBoxRect, SDL_Rect loginButtonBoxRect, SDL_Rect registerButtonBoxRect,
            SDL_Texture* loginTextTexture, SDL_Texture* passwordTextTexture, SDL_Texture* cancelButtonTextTexture, SDL_Texture* loginButtonTextTexture, SDL_Texture* registerButtonTextTexture,
            SDL_Surface* loginInputTextSurface, SDL_Texture* loginInputTextTexture, SDL_Surface* passwordInputTextSurface, SDL_Texture* passwordInputTextTexture,
            char* passwordInputText, char* loginInputText, int loginTextHeight, int loginTextWidth, int passwordTextHeight, int passwordTextWidth, const char* passwordMask,
            int cancelButtonTextHeight, int cancelButtonTextWidth, int loginButtonTextHeight, int loginButtonTextWidth, int registerButtonTextHeight, int registerButtonTextWidth) {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color
    SDL_RenderClear(renderer);

    // Render the login box border
    SDL_SetRenderDrawColor(renderer, 52, 52, 54, 255);  // Black color
    SDL_RenderDrawRect(renderer, &loginBoxRect);

    //Render the cinepuc image
    SDL_RenderCopy(renderer, cinepucImage, NULL, &cinepucImageRect);

    //Render the view image
    if(showPassword == SHOW){
        SDL_RenderCopy(renderer, viewImage, NULL, &viewImageRect);
    }
    //Render the hide image
    else if(showPassword == HIDE){
        SDL_RenderCopy(renderer, hideImage, NULL, &hideImageRect);
    }

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
        if(showPassword == SHOW)
            passwordInputTextSurface = TTF_RenderText_Solid(font, passwordMask, textColor);
        else if (showPassword == HIDE)
            passwordInputTextSurface = TTF_RenderText_Solid(font, passwordInputText, textColor);

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

    SDL_Rect registerButtonTextRect = { registerButtonBoxRect.x + (registerButtonBoxRect.w - registerButtonTextWidth) / 2, registerButtonBoxRect.y + (registerButtonBoxRect.h - registerButtonTextHeight) / 2, registerButtonTextWidth, registerButtonTextHeight };
    SDL_RenderCopy(renderer, registerButtonTextTexture, NULL, &registerButtonTextRect);

    //Render the buttons
    SDL_SetRenderDrawColor(renderer, 52, 52, 54, 255);  // Black color
    SDL_RenderDrawRect(renderer, &loginButtonBoxRect);
    SDL_RenderDrawRect(renderer, &cancelButtonBoxRect);
    SDL_RenderDrawRect(renderer, &registerButtonBoxRect);
    // Update the screen
    SDL_RenderPresent(renderer);
}

void centerLoginHUD(SDL_Rect* cinepucImageRect, SDL_Rect* viewImageRect, SDL_Rect* hideImageRect, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* registerButtonBoxRect, int windowWidth, int windowHeight) {
    // Calculate the new position of the login box
    loginBoxRect->x = (windowWidth - loginBoxRect->w) / 2;
    loginBoxRect->y = (windowHeight - loginBoxRect->h - passwordBoxRect->h - 40) / 2;

    // Calculate the new position of the password box
    passwordBoxRect->x = (windowWidth - passwordBoxRect->w) / 2;
    passwordBoxRect->y = loginBoxRect->y + 27 + loginBoxRect->h + 10;

    //Calculate the new position of the cancel button
    cancelButtonBoxRect->x = (windowWidth - loginBoxRect->w) / 2; 
    cancelButtonBoxRect->y = passwordBoxRect->y + passwordBoxRect->h + 27;

    //Calculate the new position of the login button
    loginButtonBoxRect->x = cancelButtonBoxRect->x + cancelButtonBoxRect->w + 40;
    loginButtonBoxRect->y = passwordBoxRect->y + passwordBoxRect->h + 27;

    //Calculate the new position of the register button
    registerButtonBoxRect->x = (windowWidth - registerButtonBoxRect->w) / 2;
    registerButtonBoxRect->y = loginButtonBoxRect->y + loginButtonBoxRect->h + 27;

    //Calculate the new position of the cinepuc image box
    cinepucImageRect->x = (windowWidth - cinepucImageRect->w) / 2;
    cinepucImageRect->y = loginBoxRect->y - 220;

    //Calculate the new position of the view image box
    viewImageRect->x = (passwordBoxRect->x + passwordBoxRect->w) - viewImageRect->w - 5;
    viewImageRect->y = (passwordBoxRect->y + (passwordBoxRect->h/2) - viewImageRect->h/2);

    //Calculate the new position of the hide image box
    hideImageRect->x = (passwordBoxRect->x + passwordBoxRect->w) - hideImageRect->w - 5;
    hideImageRect->y = (passwordBoxRect->y + (passwordBoxRect->h/2) - hideImageRect->h/2);
}

void renderRegisterScreen(SDL_Renderer* renderer, TTF_Font* font, SDL_Color textColor,
            SDL_Texture* cinepucImage, SDL_Rect cinepucImageRect, SDL_Texture* viewImage, SDL_Rect viewImageRect, SDL_Texture* hideImage, SDL_Rect hideImageRect, SDL_Rect confirmViewImageRect, SDL_Rect confirmHideImageRect,
            SDL_Rect loginBoxRect, SDL_Rect passwordBoxRect, SDL_Rect confirmPasswordBoxRect, SDL_Rect cancelButtonBoxRect, SDL_Rect loginButtonBoxRect,
            SDL_Texture* loginTextTexture, SDL_Texture* passwordTextTexture, SDL_Texture* confirmPasswordTextTexture, SDL_Texture* cancelButtonTextTexture, SDL_Texture* loginButtonTextTexture,
            SDL_Surface* loginInputTextSurface, SDL_Texture* loginInputTextTexture,
            SDL_Surface* passwordInputTextSurface, SDL_Texture* passwordInputTextTexture,
            SDL_Surface* confirmPasswordInputTextSurface, SDL_Texture* confirmPasswordInputTextTexture,
            char* passwordInputText, char* loginInputText, char* confirmPasswordInputText,
            int loginTextHeight, int loginTextWidth,
            int passwordTextHeight, int passwordTextWidth, const char* passwordMask,
            int confirmPasswordTextHeight, int confirmPasswordTextWidth, const char* confirmPasswordMask,
            int cancelButtonTextHeight, int cancelButtonTextWidth,
            int loginButtonTextHeight, int loginButtonTextWidth) {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color
    SDL_RenderClear(renderer);

    // Render the login box border
    SDL_SetRenderDrawColor(renderer, 52, 52, 54, 255);  // Black color
    SDL_RenderDrawRect(renderer, &loginBoxRect);

    //Render the cinepuc image
    SDL_RenderCopy(renderer, cinepucImage, NULL, &cinepucImageRect);

    //Render the view image
    if(showPassword == SHOW){
        SDL_RenderCopy(renderer, viewImage, NULL, &viewImageRect);
    }
    //Render the hide image
    else if(showPassword == HIDE){
        SDL_RenderCopy(renderer, hideImage, NULL, &hideImageRect);
    }

    //render the view image
    if(showConfirmPassword == SHOW_CONFIRM && currentScreen == REGISTER_SCREEN) {
        SDL_RenderCopy(renderer, viewImage, NULL, &confirmViewImageRect);
    }
    //Render the hide image
    else if(showConfirmPassword == HIDE_CONFIRM && currentScreen == REGISTER_SCREEN) {
        SDL_RenderCopy(renderer, hideImage, NULL, &confirmHideImageRect);
    }

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
    // Render the confirm password box border
    SDL_SetRenderDrawColor(renderer, 52, 52, 54, 255);  // Black color
    SDL_RenderDrawRect(renderer, &confirmPasswordBoxRect);

    // Render the password text
    int passwordTextPosX = passwordBoxRect.x + 10;
    int passwordTextPosY = passwordBoxRect.y + (passwordBoxRect.h - passwordTextHeight) / 2;
    SDL_Rect passwordTextRect = { passwordBoxRect.x, passwordBoxRect.y - passwordTextHeight, passwordTextWidth, passwordTextHeight };
    SDL_RenderCopy(renderer, passwordTextTexture, NULL, &passwordTextRect);

    // Render the confirm password text
    int confirmPasswordTextPosX = confirmPasswordBoxRect.x + 10;
    int confirmPasswordTextPosY = confirmPasswordBoxRect.y + (confirmPasswordBoxRect.h - confirmPasswordTextHeight) / 2;
    SDL_Rect confirmPasswordTextRect = { confirmPasswordBoxRect.x, confirmPasswordBoxRect.y - confirmPasswordTextHeight, confirmPasswordTextWidth, confirmPasswordTextHeight };
    SDL_RenderCopy(renderer, confirmPasswordTextTexture, NULL, &confirmPasswordTextRect);

    // Render the password input text if it's not empty
    if (strlen(passwordInputText) > 0) {
        if(showPassword == SHOW)
            passwordInputTextSurface = TTF_RenderText_Solid(font, passwordMask, textColor);
        else if (showPassword == HIDE)
            passwordInputTextSurface = TTF_RenderText_Solid(font, passwordInputText, textColor);

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

    if (strlen(confirmPasswordInputText) > 0) {
        if(showConfirmPassword == SHOW_CONFIRM){
            confirmPasswordInputTextSurface = TTF_RenderText_Solid(font, confirmPasswordMask, textColor);
        }
        else if (showConfirmPassword == HIDE_CONFIRM){
            confirmPasswordInputTextSurface = TTF_RenderText_Solid(font, confirmPasswordInputText, textColor);
        }

        if (!confirmPasswordInputTextSurface) {
            SDL_Log("Failed to create password input text surface: %s", TTF_GetError());
            return;
        }

        confirmPasswordInputTextTexture = SDL_CreateTextureFromSurface(renderer, confirmPasswordInputTextSurface);
        if (!confirmPasswordInputTextTexture) {
            SDL_Log("Failed to create password input text texture: %s", SDL_GetError());
            SDL_FreeSurface(confirmPasswordInputTextSurface);
            confirmPasswordInputTextSurface = NULL;
            return;
        }

        int confirmPasswordInputTextWidth = confirmPasswordInputTextSurface->w;
        int confirmPasswordInputTextHeight = confirmPasswordInputTextSurface->h;
        SDL_Rect confirmPasswordInputTextRect = { confirmPasswordTextPosX, confirmPasswordTextPosY, confirmPasswordInputTextWidth, confirmPasswordInputTextHeight };
        SDL_RenderCopy(renderer, confirmPasswordInputTextTexture, NULL, &confirmPasswordInputTextRect);
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

void centerRegisterHUD(SDL_Rect* cinepucImageRect, SDL_Rect* viewImageRect, SDL_Rect* hideImageRect, SDL_Rect* confirmViewImageRect, SDL_Rect* confirmHideImageRect, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* confirmPasswordBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* cancelButtonBoxRect, int windowWidth, int windowHeight) {
    // Calculate the new position of the login box
    loginBoxRect->x = (windowWidth - loginBoxRect->w) / 2;
    loginBoxRect->y = (windowHeight - loginBoxRect->h - passwordBoxRect->h - 40) / 2;

    // Calculate the new position of the password box
    passwordBoxRect->x = (windowWidth - passwordBoxRect->w) / 2;
    passwordBoxRect->y = loginBoxRect->y + 27 + loginBoxRect->h + 10;

    //Calculate the new position of the confirm password box
    confirmPasswordBoxRect->x = (windowWidth - confirmPasswordBoxRect->w) / 2;;
    confirmPasswordBoxRect->y = passwordBoxRect->y + passwordBoxRect->h + 37;

    //Calculate the new position of the cancel button
    cancelButtonBoxRect->x = (windowWidth - loginBoxRect->w) / 2; 
    cancelButtonBoxRect->y = confirmPasswordBoxRect->y + confirmPasswordBoxRect->h + 27;

    //Calculate the new position of the login button
    loginButtonBoxRect->x = cancelButtonBoxRect->x + cancelButtonBoxRect->w + 40;
    loginButtonBoxRect->y = confirmPasswordBoxRect->y + confirmPasswordBoxRect->h + 27;

    //Calculate the new position of the cinepuc image box
    cinepucImageRect->x = (windowWidth - cinepucImageRect->w) / 2;
    cinepucImageRect->y = loginBoxRect->y - 220;

    //Calculate the new position of the view image box
    viewImageRect->x = (passwordBoxRect->x + passwordBoxRect->w) - viewImageRect->w - 5;
    viewImageRect->y = (passwordBoxRect->y + (passwordBoxRect->h/2) - viewImageRect->h/2);

    //Calculate the new position of the hide image box
    hideImageRect->x = (passwordBoxRect->x + passwordBoxRect->w) - hideImageRect->w - 5;
    hideImageRect->y = (passwordBoxRect->y + (passwordBoxRect->h/2) - hideImageRect->h/2);

    //Calculate the new position of the confirm view image box
    confirmViewImageRect->x = (confirmPasswordBoxRect->x + confirmPasswordBoxRect->w) - confirmViewImageRect->w - 5;
    confirmViewImageRect->y = (confirmPasswordBoxRect->y + (confirmPasswordBoxRect->h/2) - confirmViewImageRect->h/2);

    //Calculate the new position of the confirm hide image box
    confirmHideImageRect->x = (confirmPasswordBoxRect->x + confirmPasswordBoxRect->w) - confirmHideImageRect->w - 5;
    confirmHideImageRect->y = (confirmPasswordBoxRect->y + (confirmPasswordBoxRect->h/2) - confirmHideImageRect->h/2);
}

void renderMovieScreen(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void cleanUp(SDL_Texture* cinepucImage, SDL_Texture* viewImage, SDL_Texture* hideImage, SDL_Texture* loginTextTexture, SDL_Texture* passwordTextTexture, SDL_Texture* loginButtonTextTexture, SDL_Texture* cancelButtonTextTexture, SDL_Texture* registerButtonTextTexture, SDL_Texture* loginInputTextTexture, SDL_Texture* passwordInputTextTexture, SDL_Surface* loginTextSurface, SDL_Surface* passwordTextSurface, SDL_Surface* loginButtonTextSurface, SDL_Surface* cancelButtonTextSurface, SDL_Surface* registerButtonTextSurface, SDL_Surface* loginInputTextSurface, SDL_Surface* passwordInputTextSurface, SDL_Surface* confirmPasswordTextSurface, SDL_Texture* confirmPasswordTextTexture, SDL_Surface* confirmPasswordInputTextSurface, SDL_Texture* confirmPasswordInputTextTexture, SDL_Window* window, TTF_Font* font, SDL_Renderer* renderer) {
    SDL_DestroyTexture(cinepucImage);
    SDL_DestroyTexture(viewImage);
    SDL_DestroyTexture(hideImage);
    SDL_DestroyTexture(loginTextTexture);
    SDL_DestroyTexture(passwordTextTexture);
    SDL_DestroyTexture(confirmPasswordTextTexture);
    SDL_FreeSurface(loginTextSurface);
    SDL_FreeSurface(passwordTextSurface);
    SDL_FreeSurface(confirmPasswordTextSurface);
    SDL_DestroyTexture(loginButtonTextTexture);
    SDL_FreeSurface(loginButtonTextSurface);
    SDL_DestroyTexture(cancelButtonTextTexture);
    SDL_FreeSurface(cancelButtonTextSurface);
    SDL_DestroyTexture(registerButtonTextTexture);
    SDL_FreeSurface(registerButtonTextSurface);
    SDL_DestroyTexture(loginInputTextTexture);
    SDL_FreeSurface(loginInputTextSurface);
    SDL_DestroyTexture(passwordInputTextTexture);
    SDL_FreeSurface(passwordInputTextSurface);
    SDL_DestroyTexture(confirmPasswordInputTextTexture);
    SDL_FreeSurface(confirmPasswordInputTextSurface);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}