#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string.h>

#define MAX_TEXT_LENGTH 50
enum { LOGIN_FIELD, PASSWORD_FIELD } activeField = LOGIN_FIELD;

void centerHUD(SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* imageRect,int passwordTextHeight,int windowWidth, int windowHeight);

int main(int argc, char* argv[]) {
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
        return 1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Load a font
    TTF_Font* font = TTF_OpenFont("../assets/arial.ttf", 24);  // Replace with your font file path
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Texture* image = IMG_LoadTexture(renderer, "../assets/image1.jpg");

    // Set up the text to render
    const char* loginText = "Login:";
    const char* passwordText = "Password:";
    const char* loginButtonText = "Login";
    const char* cancelButtonText = "Cancelar";
    SDL_Color textColor = { 0, 0, 0 };  // Black color

    // Create a surface for the login text
    SDL_Surface* loginTextSurface = TTF_RenderText_Solid(font, loginText, textColor);
    if (!loginTextSurface) {
        SDL_Log("Failed to create login text surface: %s", TTF_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create a surface for the password text
    SDL_Surface* passwordTextSurface = TTF_RenderText_Solid(font, passwordText, textColor);
    if (!passwordTextSurface) {
        SDL_Log("Failed to create password text surface: %s", TTF_GetError());
        SDL_FreeSurface(loginTextSurface);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    //Create a surface for the cancel button text
    SDL_Surface* cancelButtonTextSurface = TTF_RenderText_Solid(font, cancelButtonText, textColor);
    if (!cancelButtonTextSurface) {
        SDL_Log("Failed to create cancel button text surface: %s", TTF_GetError());
        SDL_FreeSurface(cancelButtonTextSurface);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    //Create a surface for the login button text
    SDL_Surface* loginButtonTextSurface = TTF_RenderText_Solid(font, loginButtonText, textColor);
    if (!loginButtonTextSurface) {
        SDL_Log("Failed to create login button text surface: %s", TTF_GetError());
        SDL_FreeSurface(loginButtonTextSurface);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create textures from the surfaces
    SDL_Texture* loginTextTexture = SDL_CreateTextureFromSurface(renderer, loginTextSurface);
    SDL_Texture* passwordTextTexture = SDL_CreateTextureFromSurface(renderer, passwordTextSurface);
    SDL_Texture* loginButtonTextTexture = SDL_CreateTextureFromSurface(renderer, loginButtonTextSurface);
    SDL_Texture* cancelButtonTextTexture = SDL_CreateTextureFromSurface(renderer, cancelButtonTextSurface);
    if (!loginTextTexture || !passwordTextTexture || !loginButtonTextTexture || !cancelButtonTextTexture) {
        SDL_Log("Failed to create text textures: %s", SDL_GetError());
        SDL_FreeSurface(loginTextSurface);
        SDL_FreeSurface(passwordTextSurface);
        SDL_FreeSurface(loginButtonTextSurface);
        SDL_FreeSurface(cancelButtonTextSurface);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
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

    SDL_Rect imageRect = { 0, 0, 150, 150 };

    // Set up the rectangle for the login box
    SDL_Rect loginBoxRect = { 0, 0, 300, loginTextHeight + 20 };

    // Set up the rectangle for the password box
    SDL_Rect passwordBoxRect = { 0, 0, 300, passwordTextHeight + 20 };

    //Set up the rectangle for the login button    
    SDL_Rect loginButtonBoxRect = { 0, 0, 130, loginButtonTextHeight + 15};

    //Set up the rectangle for the cancel button
    SDL_Rect cancelButtonBoxRect = { 0, 0, 130, cancelButtonTextHeight + 15};

    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    centerHUD(&loginBoxRect, &passwordBoxRect, &loginButtonBoxRect, &cancelButtonBoxRect, &imageRect, passwordTextHeight, windowWidth, windowHeight);
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
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    // Remove the last character if backspace is pressed
                    if (event.window.windowID == SDL_GetWindowID(window)) {
                        if (activeField == LOGIN_FIELD && strlen(loginInputText) > 0) {
                            loginInputText[strlen(loginInputText) - 1] = '\0';
                        }
                        else if (activeField == PASSWORD_FIELD && strlen(passwordInputText) > 0) {
                            passwordInputText[strlen(passwordInputText) - 1] = '\0';
                            passwordMask[strlen(passwordMask) - 1] = '\0';
                        }
                    }
                }
                else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                    // Handle the Enter key
                    if (activeField == PASSWORD_FIELD) {
                        // Handle the password input logic here
                        printf("Entered login: %s\n", loginInputText);
                        printf("Entered password: %s\n", passwordInputText);
                    }
                }
                else if (activeField == LOGIN_FIELD && strlen(loginInputText) < MAX_TEXT_LENGTH) {
                    // Append the entered character to the login input
                    char character = event.key.keysym.sym;
                    if (character >= SDLK_SPACE && character <= SDLK_z) {
                        loginInputText[strlen(loginInputText)] = character;
                    }
                }
                else if (activeField == PASSWORD_FIELD && strlen(passwordInputText) < MAX_TEXT_LENGTH) {
                    // Append the entered character to the password input
                    char character = event.key.keysym.sym;
                    if (character >= SDLK_SPACE && character <= SDLK_z) {
                        passwordInputText[strlen(passwordInputText)] = character;
                        passwordMask[strlen(passwordMask)] = '*';
                    }
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
            // Handle mouse button click
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    // Check if the click is within the login box
                    if (mouseX >= loginBoxRect.x && mouseX <= loginBoxRect.x + loginBoxRect.w &&
                        mouseY >= loginBoxRect.y && mouseY <= loginBoxRect.y + loginBoxRect.h) {
                        activeField = LOGIN_FIELD;
                    }
                    // Check if the click is within the password box
                    else if (mouseX >= passwordBoxRect.x && mouseX <= passwordBoxRect.x + passwordBoxRect.w &&
                        mouseY >= passwordBoxRect.y && mouseY <= passwordBoxRect.y + passwordBoxRect.h) {
                        activeField = PASSWORD_FIELD;
                    }
                    else if (mouseX >= cancelButtonBoxRect.x && mouseX <= cancelButtonBoxRect.x + cancelButtonBoxRect.w &&
                        mouseY >= cancelButtonBoxRect.y && mouseY <= cancelButtonBoxRect.y + cancelButtonBoxRect.h) {
                        quit = 1;
                    }
                    else if (mouseX >= loginButtonBoxRect.x && mouseX <= loginButtonBoxRect.x + loginButtonBoxRect.w &&
                        mouseY >= loginButtonBoxRect.y && mouseY <= loginButtonBoxRect.y + loginButtonBoxRect.h) {
                        printf("Entered login: %s\n", loginInputText);
                        printf("Entered password: %s\n", passwordInputText);
                        fflush(stdout);
                    }
                }
            }
            else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    // Window resized, re-center the HUD
                    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                    centerHUD(&loginBoxRect, &passwordBoxRect, &loginButtonBoxRect, &cancelButtonBoxRect, &imageRect,passwordTextHeight, windowWidth, windowHeight);
                }
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color
        SDL_RenderClear(renderer);

        // Render the login box border
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black color
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
                break;
            }

            loginInputTextTexture = SDL_CreateTextureFromSurface(renderer, loginInputTextSurface);
            if (!loginInputTextTexture) {
                SDL_Log("Failed to create login input text texture: %s", SDL_GetError());
                SDL_FreeSurface(loginInputTextSurface);
                loginInputTextSurface = NULL;
                break;
            }

            int loginInputTextWidth = loginInputTextSurface->w;
            int loginInputTextHeight = loginInputTextSurface->h;
            SDL_Rect loginInputTextRect = { loginTextPosX, loginTextPosY, loginInputTextWidth, loginInputTextHeight };
            SDL_RenderCopy(renderer, loginInputTextTexture, NULL, &loginInputTextRect);
        }

        // Render the password box border
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black color
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
                break;
            }

            passwordInputTextTexture = SDL_CreateTextureFromSurface(renderer, passwordInputTextSurface);
            if (!passwordInputTextTexture) {
                SDL_Log("Failed to create password input text texture: %s", SDL_GetError());
                SDL_FreeSurface(passwordInputTextSurface);
                passwordInputTextSurface = NULL;
                break;
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
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black color
        SDL_RenderDrawRect(renderer, &loginButtonBoxRect);
        SDL_RenderDrawRect(renderer, &cancelButtonBoxRect);

        // Cleanup input text resources
        SDL_DestroyTexture(loginInputTextTexture);
        SDL_FreeSurface(loginInputTextSurface);
        SDL_DestroyTexture(passwordInputTextTexture);
        SDL_FreeSurface(passwordInputTextSurface);

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    // Clean up resources
    SDL_DestroyTexture(image);
    SDL_DestroyTexture(loginTextTexture);
    SDL_DestroyTexture(passwordTextTexture);
    SDL_FreeSurface(loginTextSurface);
    SDL_FreeSurface(passwordTextSurface);
    SDL_DestroyTexture(loginButtonTextTexture);
    SDL_FreeSurface(loginButtonTextSurface);
    SDL_DestroyTexture(cancelButtonTextTexture);
    SDL_FreeSurface(cancelButtonTextSurface);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

void centerHUD(SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* imageRect,int passwordTextHeight,int windowWidth, int windowHeight) {
    // Calculate the new position of the login box
    loginBoxRect->x = (windowWidth - loginBoxRect->w) / 2;
    loginBoxRect->y = (windowHeight - loginBoxRect->h - passwordBoxRect->h - 40) / 2;

    //Calculate the new position of the image box
    imageRect->x = (windowWidth - imageRect->w) / 2;
    imageRect->y = loginBoxRect->y - 200;

    // Calculate the new position of the password box
    passwordBoxRect->x = (windowWidth - passwordBoxRect->w) / 2;
    passwordBoxRect->y = loginBoxRect->y + passwordTextHeight + loginBoxRect->h + 10;

    cancelButtonBoxRect->x = (windowWidth - loginBoxRect->w) / 2; 
    cancelButtonBoxRect->y = passwordBoxRect->y + passwordBoxRect->h + passwordTextHeight;

    loginButtonBoxRect->x = cancelButtonBoxRect->x + cancelButtonBoxRect->w + 40;
    loginButtonBoxRect->y = passwordBoxRect->y + passwordBoxRect->h + passwordTextHeight;
}
