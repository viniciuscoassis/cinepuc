/*
Para baixar as dependencias (Linux):
    sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev libncurses5-dev
    
Para rodar o código:
    -Abra o terminal na pasta cinepuc
    -Compile o código:
        gcc interface/src/InterfaceLogin/main.c backend/lib/funcoesCadastro.c -I backend/include -o CINEMA -lSDL2 -lSDL2_image -lSDL2_ttf -lncurses
    -Rode o código:
        ./CINEMA
*/

//Bibliotecas
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <curses.h>
#include <string.h>
#include "cadastro.h"
#include "carrinho.h"
//#include "estruturaCinema.h"

// Estrutura para armazenar os dados no frontend e passar pro backend
typedef struct {
    char usuarioLogado[50];
    int idFilme;
    int idSessao;
} dadosFrontEnd;

//TELA LOGIN/REGISTRO
//Flags para determinar a tela, o campo de escrita ou a possibidade de esconder/visualizar a senha.
#define MAX_TEXT_LENGTH 50
enum { LOGIN_SCREEN, REGISTER_SCREEN, MOVIE_SCREEN, TIME_SELECTION_SCREEN, SEAT_SELECTION_SCREEN } currentScreen = LOGIN_SCREEN;
enum { LOGIN_FIELD, PASSWORD_FIELD, CONFIRM_FIELD } activeField = LOGIN_FIELD;
enum { SHOW, HIDE } showPassword = SHOW;
enum { SHOW_CONFIRM, HIDE_CONFIRM  } showConfirmPassword = SHOW_CONFIRM;
//Flag para limpar os campos de texto (Usado na troca de telas ex: login para cadastro)
enum { CLEAR, DONT_CLEAR } clearInput = DONT_CLEAR;

//Flag para determinar a cor da caixa de texto (Usado quando o usuario registra um nome/email ja existentes)
enum boxColor { RED, BLACK };
typedef enum boxColor boxColor;

typedef struct {
    boxColor color;
} Box;

//TELA PARA ESCOLHER O FILME
#define ROWS 2
#define COL 2

#define MOVIE_WIDTH 200
#define MOVIE_HEIGHT 300
#define PADDING 50

//Struct para determinar o nome e a imagem do filme
typedef struct{
    SDL_Texture *image;
    char name[50];
} Movie;

//Matriz da struct acima, no caso de ROWS 2 e COLS 2 teremos 4 filmes
Movie movies[ROWS][COL];

//TELA PARA SELECIONAR OS ASSENTOS
#define HEIGHT 1            //altura da cadeira
#define WIDTH 3             //largura da cadeira 
#define TOTAL_WIDTH ((WIDTH + GRID_SPACING) * (CHAIR_COL_AMOUNT + 1) - GRID_SPACING)    //largura de toda a interface somada
#define TOTAL_HEIGHT ((HEIGHT + GRID_SPACING) * (CHAIR_LINE_AMOUNT + 3) - GRID_SPACING) //altura de toda a interface somada
#define GRID_SPACING 1      //espaço entre as cadeiras
#define CHAIR_LINE_AMOUNT 8 //numero de cadeiras por linha
#define CHAIR_COL_AMOUNT 8  //numero de cadeiras por coluna
#define CHAIR_AMOUNT (CHAIR_LINE_AMOUNT * CHAIR_COL_AMOUNT) // total de cadeiras
#define STARTX ((COLS - TOTAL_WIDTH)/ 2)    //formula para centralizar a interface horizontalmente
#define STARTY ((LINES - TOTAL_HEIGHT) /2)  //formula para centralizar a interface verticalmente
#define COLOR_GRAY 9

WINDOW *my_win[CHAIR_AMOUNT]; //Vetor de Janelas com tamanho de [número de cadeiras].
int status[CHAIR_AMOUNT]; // Disponivel (0), ocupado (1) ou selecionado (2)

/*
Inits: são usados para iniciar certas funcoes:
    -Iniciar a Tela
    -Iniciar o Renderizador
    -Iniciar a fonte de texto
    -Iniciar as Imagens
    -Inicias as Surfaces e Texture (Necessarias para a renderizacao de um texto, caixa ou qualquer outra coisa)
*/
int initSDL();
SDL_Window* initWindow();
SDL_Renderer* initRender(SDL_Window* window);
TTF_Font* initFont(SDL_Window* window, SDL_Renderer* renderer);
SDL_Texture* initImage(SDL_Window* window, SDL_Renderer* renderer, char* link);
SDL_Surface* initSurface(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, char* text, SDL_Color textColor);
SDL_Texture* initTexture(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, SDL_Surface* surface);

/*
Handles: são usados para lidar com certos eventos:
    -handleKeyboard: lida com a utilizacao do teclado
    -handleMouse: lida com a utilizacao do mouse
    -handleWindow: lida com o resize da tela
*/
void handleKeyboardInput(SDL_Renderer* renderer, SDL_Event* event, SDL_Window* window, char* loginInputText, char* passwordInputText, char* passwordMask, char* confirmPasswordInputText, char* confirmPasswordMask, Box* loginBoxColor, Box* passwordBoxColor, Box* confirmPasswordBoxColor, dadosFrontEnd* dados);
void handleMouseClick(SDL_Renderer* renderer, SDL_Event* event, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* confirmPasswordBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* registerButtonBoxRect, SDL_Rect* viewImageRect, SDL_Rect* hideImageRect, SDL_Rect* confirmViewImageRect, SDL_Rect* confirmHideImageRect, SDL_Rect movieRect[][COL], SDL_Rect* firstTimeButtonBoxRect, SDL_Rect* secondTimeButtonBoxRect, SDL_Rect* thirdTimeButtonBoxRect, SDL_Rect* fourthTimeButtonBoxRect, char* loginInputText, char* passwordInputText, char* confirmPasswordInputText, char* passwordMask, char* confirmPasswordMask, int* quit, Box* loginBoxColor, Box* passwordBoxColor, Box* confirmPasswordBoxColor, dadosFrontEnd* dados);
void handleWindowEvent(SDL_Event* event, SDL_Window *window, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* confirmPasswordBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* cinepucImageRect, SDL_Rect* registerButtonBoxRect, SDL_Rect* viewImageRect, SDL_Rect* hideImageRect, SDL_Rect* confirmViewImageRect, SDL_Rect* confirmHideImageRect, int windowWidth, int windowHeight, int gridWidth, int gridHeight, int maxTextWidth, SDL_Rect* selectMovieRect, SDL_Rect movieRect[][COL], SDL_Rect textRect[][COL], SDL_Renderer* renderer, TTF_Font* font);

//RENDERIZADORES: são usados para renderizar telas especificas
//TELA DE LOGIN
void renderLoginScreen(SDL_Renderer* renderer, TTF_Font* font, SDL_Color textColor,
            SDL_Texture* cinepucImage, SDL_Rect cinepucImageRect, SDL_Texture* viewImage, SDL_Rect viewImageRect, SDL_Texture* hideImage, SDL_Rect hideImageRect,
            SDL_Rect loginBoxRect, SDL_Rect passwordBoxRect, SDL_Rect cancelButtonBoxRect, SDL_Rect loginButtonBoxRect, SDL_Rect registerButtonBoxRect,
            SDL_Texture* loginTextTexture, SDL_Texture* passwordTextTexture, SDL_Texture* cancelButtonTextTexture, SDL_Texture* loginButtonTextTexture, SDL_Texture* registerButtonTextTexture,
            SDL_Surface* loginInputTextSurface, SDL_Texture* loginInputTextTexture, SDL_Surface* passwordInputTextSurface, SDL_Texture* passwordInputTextTexture,
            char* passwordInputText, char* loginInputText, int loginTextHeight, int loginTextWidth, int passwordTextHeight, int passwordTextWidth, char* passwordMask,
            int cancelButtonTextHeight, int cancelButtonTextWidth, int loginButtonTextHeight, int loginButtonTextWidth, int registerButtonTextHeight, int registerButtonTextWidth, Box* loginBoxColor, Box* passwordBoxColor);
void centerLoginHUD(SDL_Rect* cinepucImageRect, SDL_Rect* viewImageRect, SDL_Rect* hideImageRect, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* registerButtonBoxRect, int windowWidth, int windowHeight);

//TELA DE REGISTRO
void renderRegisterScreen(SDL_Renderer* renderer, TTF_Font* font, SDL_Color textColor,
            SDL_Texture* cinepucImage, SDL_Rect cinepucImageRect, SDL_Texture* viewImage, SDL_Rect viewImageRect, SDL_Texture* hideImage, SDL_Rect hideImageRect, SDL_Rect confirmViewImageRect, SDL_Rect confirmHideImageRect,
            SDL_Rect loginBoxRect, SDL_Rect passwordBoxRect, SDL_Rect confirmPasswordBoxRect, SDL_Rect cancelButtonBoxRect, SDL_Rect loginButtonBoxRect,
            SDL_Texture* loginTextTexture, SDL_Texture* passwordTextTexture, SDL_Texture* confirmPasswordTextTexture, SDL_Texture* cancelButtonTextTexture, SDL_Texture* loginButtonTextTexture,
            SDL_Surface* loginInputTextSurface, SDL_Texture* loginInputTextTexture,
            SDL_Surface* passwordInputTextSurface, SDL_Texture* passwordInputTextTexture,
            SDL_Surface* confirmPasswordInputTextSurface, SDL_Texture* confirmPasswordInputTextTexture,
            char* passwordInputText, char* loginInputText, char* confirmPasswordInputText,
            int loginTextHeight, int loginTextWidth,
            int passwordTextHeight, int passwordTextWidth, char* passwordMask,
            int confirmPasswordTextHeight, int confirmPasswordTextWidth, char* confirmPasswordMask,
            int cancelButtonTextHeight, int cancelButtonTextWidth,
            int loginButtonTextHeight, int loginButtonTextWidth,
            Box* loginBoxColor, Box* passwordBoxColor, Box* confirmPasswordBoxColor);
void centerRegisterHUD(SDL_Rect* cinepucImageRect, SDL_Rect* viewImageRect, SDL_Rect* hideImageRect, SDL_Rect* confirmViewImageRect, SDL_Rect* confirmHideImageRect, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* confirmPasswordBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* cancelButtonBoxRect, int windowWidth, int windowHeight);

//TELA PARA SELECIONAR O FILME
void renderMovieScreen(SDL_Renderer* renderer, TTF_Font* font, char* selectMovieText, int selectMovieWidth, int selectMovieHeight, SDL_Rect* selectMovieRect,
                       int maxTextWidth, int offsetX, int offsetY, SDL_Rect movieRect[][COL], SDL_Rect textRect[][COL], SDL_Surface* selectMovieTextSurface, SDL_Texture* selectMovieTextTexture, int windowWidth, int windowHeight, int gridWidth, int gridHeight);
void centerMovieHUD(int windowWidth, int windowHeight, int gridWidth, int gridHeight, int maxTextWidth, SDL_Rect* selectMovieRect, SDL_Rect movieRect[][COL], SDL_Rect textRect[][COL], SDL_Renderer* renderer, TTF_Font* font);

//TELA PARA ESCOLHER O HORARIO
void renderTimeSelectionScreen(SDL_Renderer* renderer, SDL_Texture* selectTimeTextTexture, int selectTimeTextWidth, int selectTimeTextHeight,
                               SDL_Rect* firstTimeButtonBoxRect, SDL_Texture* firstTimeButtonTextTexture, int firstTimeButtonTextWidth, int firstTimeButtonTextHeight,
                               SDL_Rect* secondTimeButtonBoxRect, SDL_Texture* secondTimeButtonTextTexture, int secondTimeButtonTextWidth, int secondTimeButtonTextHeight,
                               SDL_Rect* thirdTimeButtonBoxRect, SDL_Texture* thirdTimeButtonTextTexture, int thirdTimeButtonTextWidth, int thirdTimeButtonTextHeight,
                               SDL_Rect* fourthTimeButtonBoxRect, SDL_Texture* fourthTimeButtonTextTexture, int fourthTimeButtonTextWidth, int fourthTimeButtonTextHeight,
                               SDL_Rect selectTimeTextRect,
                               SDL_Rect* cancelButtonBoxRect, SDL_Texture* cancelButtonTextTexture,
                               SDL_Texture* cinepucImage, SDL_Rect cinepucImageRect,
                               int cancelButtonTextWidth, int cancelButtonTextHeight);
void centerTimeSelectionHUD(SDL_Rect* selectTimeTextRect, SDL_Rect* firstTimeButtonBoxRect, SDL_Rect* secondTimeButtonBoxRect, SDL_Rect* thirdTimeButtonBoxRect, SDL_Rect* fourthTimeButtonBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* cinepucImageRect, int windowWidth, int windowHeight);

//TELA PARA ESCOLHER OS ASSENTOS
WINDOW *create_newwin(int height, int width, int starty, int startx); //função para criar uma janela
void initColors(); //função para iniciar as cores usadas no menu
void drawChairMenu();   //função para desenhar o menu (cadeiras + letras e numeros)
void drawTELA();  //função para criar o texto TELA
WINDOW* drawCANCEL(); //função pra criar o botão Cancelar
WINDOW* drawCONFIRM(); //função pra criar o botão Confirmar
void mouseFunc(WINDOW* CANCEL, WINDOW* CONFIRM, dadosFrontEnd* dados, Carrinho* carrinho);  //função para detectar se o mouse clicou na respectiva cadeira

//Funcao cleanup usada para limpar todas as textures, surfaces entre outras coisa necessarias em SDL para evitar vazamento de memória
void cleanUp(SDL_Texture* cinepucImage, SDL_Texture* viewImage, SDL_Texture* hideImage, SDL_Texture* loginTextTexture, SDL_Texture* passwordTextTexture, SDL_Texture* loginButtonTextTexture, SDL_Texture* cancelButtonTextTexture, SDL_Texture* registerButtonTextTexture, SDL_Texture* loginInputTextTexture, SDL_Texture* passwordInputTextTexture, SDL_Surface* loginTextSurface, SDL_Surface* passwordTextSurface, SDL_Surface* loginButtonTextSurface, SDL_Surface* cancelButtonTextSurface, SDL_Surface* registerButtonTextSurface, SDL_Surface* loginInputTextSurface, SDL_Surface* passwordInputTextSurface, SDL_Surface* confirmPasswordTextSurface, SDL_Texture* confirmPasswordTextTexture, SDL_Surface* confirmPasswordInputTextSurface, SDL_Texture* confirmPasswordInputTextTexture, SDL_Texture *selectMovieTextTexture, SDL_Surface *selectMovieTextSurface, SDL_Surface* selectTimeTextSurface, SDL_Surface* firstTimeButtonTextSurface, SDL_Surface* secondTimeButtonTextSurface, SDL_Surface* thirdTimeButtonTextSurface, SDL_Surface* fourthTimeButtonTextSurface, SDL_Texture* selectTimeTextTexture, SDL_Texture* firstTimeButtonTextTexture, SDL_Texture* secondTimeButtonTextTexture, SDL_Texture* thirdTimeButtonTextTexture, SDL_Texture* fourthTimeButtonTextTexture, SDL_Window* window, TTF_Font* font, SDL_Renderer* renderer);

int main(int argc, char* argv[]) {
    //Init SDL, tela, renderizador e fonte de texto
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

    //STRUCTS PARA INTERAGIR COM O BACKEND
    dadosFrontEnd dados;

    //VARIAVEIS DE TEXTO
    //Variaveis tela de login
    char* loginText = "E-mail:";
    char* passwordText = "Senha:";
    char* loginButtonText = "Confirmar";
    char* cancelButtonText = "Cancelar";
    char* registerButtonText = "Cadastrar";
    //Variaveis tela de registro
    char* confirmPasswordText = "Confirmar senha:";
    //Variaveis tela de selecionar o filme
    strcpy(movies[0][0].name, "Super Mario Bros - O Filme");
    strcpy(movies[0][1].name, "Guardioes da Galaxia: Volume 3");
    strcpy(movies[1][0].name, "Velozes e Furiosos 10");
    strcpy(movies[1][1].name, "Avatar: O Caminho da Agua");
    char selectMovieText[40] = "Selecione o filme que deseja assistir";
    //Variaveis tela de selecionar o horario do filme
    char* selectTimeText = "Selecione o horario desejado";
    char* firstTimeButtonText = "17:00";
    char* secondTimeButtonText = "19:00";
    char* thirdTimeButtonText = "21:00";
    char* fourthTimeButtonText = "23:00";

    SDL_Color textColor = { 52, 52, 54 };  // Texto Preto

    //INIT DAS IMAGENS
    //Init da imagem Cinepuc (Usada na tela inicial)
    SDL_Texture* cinepucImage = initImage(window, renderer, "interface/src/assets/image1.jpg");
    if(!cinepucImage) {
        return 1;
    }
    //Init da imagem do Olho (Usada nas caixas de senha para poder ver ou nao a senha)
    SDL_Texture* viewImage = initImage(window, renderer, "interface/src/assets/view.png");
    if(!viewImage) {
        return 1;
    }
    //Init da imagem do Olho "Fechado" (Usada nas caixas de senha para poder ver ou nao a senha)
    SDL_Texture* hideImage = initImage(window, renderer, "interface/src/assets/hide.png");
    if(!hideImage) {
        return 1;
    }
    //Init das imagens dos filmes
    movies[0][0].image = IMG_LoadTexture(renderer, "interface/src/assets/movieImage.jpg");
    movies[0][1].image = IMG_LoadTexture(renderer, "interface/src/assets/movieImage1.jpg");
    movies[1][0].image = IMG_LoadTexture(renderer, "interface/src/assets/movieImage2.jpg");
    movies[1][1].image = IMG_LoadTexture(renderer, "interface/src/assets/movieImage3.jpg");

    // INIT DAS SURFACES
    // TELA DE LOGIN
    SDL_Surface* loginTextSurface = initSurface(window, renderer, font, loginText, textColor); //Texto "E-mail" acima da caixa de login
    SDL_Surface* passwordTextSurface = initSurface(window, renderer, font, passwordText, textColor); //Texto "Senha" acima da caixa de senha
    SDL_Surface* cancelButtonTextSurface = initSurface(window, renderer, font, cancelButtonText, textColor); //Texto "Cancelar" dentro do botao
    SDL_Surface* loginButtonTextSurface = initSurface(window, renderer, font, loginButtonText, textColor); //Texto "Confirmar" dentro do botao
    SDL_Surface* registerButtonTextSurface = initSurface(window,renderer, font, registerButtonText, textColor); //Texto "Cadastrar" dentro do botao
    //TELA DE REGISTRO
    SDL_Surface* confirmPasswordTextSurface = initSurface(window, renderer, font, confirmPasswordText, textColor); //Texto "Confirmar senha" acima da caixa de confirmar senha
    //TELA PARA SELECIONAR OS FILMES
    SDL_Surface* selectMovieTextSurface = initSurface(window, renderer, font, selectMovieText, textColor); //Texto "Selecione o filme que deseja assistir" no topo da tela
    //TELA PARA SELECIONAR OS HORARIOS
    SDL_Surface* selectTimeTextSurface = initSurface(window, renderer, font, selectTimeText, textColor); //Texto "Selecione o horario desejado" no topo da tela
    SDL_Surface* firstTimeButtonTextSurface = initSurface(window, renderer, font, firstTimeButtonText, textColor); //Texto do primeiro horário
    SDL_Surface* secondTimeButtonTextSurface = initSurface(window, renderer, font, secondTimeButtonText, textColor); //Texto do segundo horário
    SDL_Surface* thirdTimeButtonTextSurface = initSurface(window, renderer, font, thirdTimeButtonText, textColor); //Texto do terceiro horário
    SDL_Surface* fourthTimeButtonTextSurface = initSurface(window, renderer, font, fourthTimeButtonText, textColor); //Texto do quarto horário

    //Verificador para ver se todas as Surfaces foram devidamente iniciadas
    if (!loginTextSurface || !passwordTextSurface || !loginButtonTextSurface || !cancelButtonTextSurface || !registerButtonTextSurface || !confirmPasswordTextSurface || !selectMovieTextSurface || !selectTimeTextSurface || !firstTimeButtonTextSurface || !secondTimeButtonTextSurface || !thirdTimeButtonTextSurface || !fourthTimeButtonTextSurface) {
        SDL_FreeSurface(loginTextSurface);
        SDL_FreeSurface(passwordTextSurface);
        SDL_FreeSurface(loginButtonTextSurface);
        SDL_FreeSurface(cancelButtonTextSurface);
        SDL_FreeSurface(registerButtonTextSurface);
        SDL_FreeSurface(confirmPasswordTextSurface);
        SDL_FreeSurface(selectMovieTextSurface);
        SDL_FreeSurface(selectTimeTextSurface);
        SDL_FreeSurface(firstTimeButtonTextSurface);
        SDL_FreeSurface(secondTimeButtonTextSurface);
        SDL_FreeSurface(thirdTimeButtonTextSurface);
        SDL_FreeSurface(fourthTimeButtonTextSurface);
        return 1;
    }

    //INIT DAS TEXTURES USANDO AS SURFACES CRIADAS ACIMA (NOMENCLATURA SEGUE O MESMO PADRAO E ORDEM DAS SURFACES ACIMA)
    //TELA DE LOGIN
    SDL_Texture* loginTextTexture = initTexture(window, renderer, font, loginTextSurface);
    SDL_Texture* passwordTextTexture = initTexture(window, renderer, font, passwordTextSurface);
    SDL_Texture* loginButtonTextTexture = initTexture(window, renderer, font, loginButtonTextSurface);
    SDL_Texture* cancelButtonTextTexture = initTexture(window, renderer, font, cancelButtonTextSurface);
    SDL_Texture* registerButtonTextTexture = initTexture(window, renderer, font, registerButtonTextSurface);
    //TELA DE REGISTRO
    SDL_Texture* confirmPasswordTextTexture = initTexture(window, renderer, font, confirmPasswordTextSurface);
    //TELA PARA SELECIONAR O FILME
    SDL_Texture* selectMovieTextTexture = initTexture(window, renderer, font, selectMovieTextSurface);
    //TELA PARA SELECIONAR O HORARIO
    SDL_Texture* selectTimeTextTexture = initTexture(window, renderer, font, selectTimeTextSurface);
    SDL_Texture* firstTimeButtonTextTexture = initTexture(window, renderer, font, firstTimeButtonTextSurface);
    SDL_Texture* secondTimeButtonTextTexture = initTexture(window, renderer, font, secondTimeButtonTextSurface);
    SDL_Texture* thirdTimeButtonTextTexture = initTexture(window, renderer, font, thirdTimeButtonTextSurface);
    SDL_Texture* fourthTimeButtonTextTexture = initTexture(window, renderer, font, fourthTimeButtonTextSurface);
    
    //Verificao para ver se as textures foram devidamente iniciadas
    if (!loginTextTexture || !passwordTextTexture || !loginButtonTextTexture || !cancelButtonTextTexture || !registerButtonTextTexture || !confirmPasswordTextTexture || !selectMovieTextTexture || !selectTimeTextTexture || !firstTimeButtonTextTexture || !secondTimeButtonTextTexture || !thirdTimeButtonTextTexture || !fourthTimeButtonTextTexture) {
        SDL_FreeSurface(loginTextSurface);
        SDL_FreeSurface(passwordTextSurface);
        SDL_FreeSurface(loginButtonTextSurface);
        SDL_FreeSurface(cancelButtonTextSurface);
        SDL_FreeSurface(registerButtonTextSurface);
        SDL_FreeSurface(confirmPasswordTextSurface);
        SDL_FreeSurface(selectMovieTextSurface);
        SDL_FreeSurface(selectTimeTextSurface);
        SDL_FreeSurface(firstTimeButtonTextSurface);
        SDL_FreeSurface(secondTimeButtonTextSurface);
        SDL_FreeSurface(thirdTimeButtonTextSurface);
        SDL_FreeSurface(fourthTimeButtonTextSurface);
        return 1;
    }

    //Tamanho da tela
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    //VARIAVEIS PARA DETERMINAR O TAMANHO (ALTURA E LARGURA) DAS SURFACES CRIADAS ANTERIORMENTE
    //  -Segue tambem a mesma logica de nomenclatura das textures e surfaces, sendo width a largura e height a altura
    //TELA DE LOGIN
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
    //TELA DE REGISTRO
    int confirmPasswordTextWidth = confirmPasswordTextSurface->w;
    int confirmPasswordTextHeight = confirmPasswordTextSurface->h;
    //TELA PARA SELECIONAR O FILME
    //Funcoes usadas para centralizar todo o grid/hud
    int totalMovieWidth = COL * MOVIE_WIDTH;
    int totalMovieHeight = ROWS * MOVIE_HEIGHT;
    int totalTextHeight = ROWS * 30;
    int totalPaddingWidth = (COL - 1) * PADDING;
    int totalPaddingHeight = (ROWS - 1) * PADDING;
    int gridWidth = totalMovieWidth + totalPaddingWidth + (341 - MOVIE_WIDTH);
    int gridHeight = totalMovieHeight + totalPaddingHeight + totalTextHeight;

    int offsetX = (windowWidth - gridWidth) / 2;
    int offsetY = (windowHeight - gridHeight) / 2;

    int textWidth, textHeight;
    int maxTextWidth = 0; 

    int selectMovieWidth, selectMovieHeight;
    //TELA PARA SELECIONAR O HORARIO
    int selectTimeTextWidth = selectTimeTextSurface->w;
    int selectTimeTextHeight = selectTimeTextSurface->h;
    int firstTimeButtonTextWidth = firstTimeButtonTextSurface->w;
    int firstTimeButtonTextHeight = firstTimeButtonTextSurface->h;
    int secondTimeButtonTextWidth = secondTimeButtonTextSurface->w;
    int secondTimeButtonTextHeight = secondTimeButtonTextSurface->h ;
    int thirdTimeButtonTextWidth = thirdTimeButtonTextSurface->w;
    int thirdTimeButtonTextHeight = thirdTimeButtonTextSurface->h;
    int fourthTimeButtonTextWidth = fourthTimeButtonTextSurface->w;
    int fourthTimeButtonTextHeight = fourthTimeButtonTextSurface->h;
    

    //DECLARACAO DOS RECTS: são usados para determinar o tamanho das imagens, o tamanho das caixas de texto (login, senha, confirmar senha) e o tamanho dos botoes (cancelar, confirmar e cadastrar)
    //TELA DE LOGIN
    //Tamanho da imagem cinepuc
    SDL_Rect cinepucImageRect = { 0, 0, 131, 172 };

    //Tamanho da imagem do olho aberto
    SDL_Rect viewImageRect = { 0, 0, 24, 24 };

    //Tamanho da imagem do olho fechado
    SDL_Rect hideImageRect = { 0, 0, 24, 24 };

    //Tamanho da caixa de texto para digitar o usuario/email
    SDL_Rect loginBoxRect = { 0, 0, 300, loginTextHeight + 20 };

    //Tamanho da caixa de texto para digitar a senha
    SDL_Rect passwordBoxRect = { 0, 0, 300, passwordTextHeight + 20 };

    //Tamanho do botao "Confirmar"
    SDL_Rect loginButtonBoxRect = { 0, 0, 130, loginButtonTextHeight + 15};

    //Tamanho do botao "Cancelar"
    SDL_Rect cancelButtonBoxRect = { 0, 0, 130, cancelButtonTextHeight + 15};

    //Tamanho do botao "Cadastrar"
    SDL_Rect registerButtonBoxRect = { 0, 0, 300, registerButtonTextHeight + 15};

    //TELA DE REGISTRO
    //Tamanho da caixa de texto para digitar a segunda senha (Confirmar senha)
    SDL_Rect confirmPasswordBoxRect = { 0, 0, 300, confirmPasswordTextHeight + 20 };

    //Tamanho da imagem do olho aberto (segunda senha)
    SDL_Rect confirmViewImageRect = { 0, 0, 24, 24 };

    //Tamanho da imagem do olho fechado (segunda senha)
    SDL_Rect confirmHideImageRect = { 0, 0, 24, 24 };

    //TELA PARA ESCOLHER O FILME
    //Vetor para determinar o tamanho das imagens dos filmes
    SDL_Rect movieRect[2][2];

    //Vetor para determinar o texto abaixo das imagens
    SDL_Rect textRect[2][2];

    //Rect para o texto "Selecione o filme que deseja assistir"
    SDL_Rect selectMovieRect;

    //TELA PARA SELECIONAR O HORARIO
    //Rect para determinar o tamanho do texto "Selecione o horario desejado"
    SDL_Rect selectTimeTextRect = {0, 0, selectTimeTextWidth, selectTimeTextHeight};

    //Rect para determinar o tamanho do botao do primeiro horario
    SDL_Rect firstTimeButtonBoxRect = { 0, 0, 300, firstTimeButtonTextHeight + 15};

    //Rect para determinar o tamanho do botao do segundo horario
    SDL_Rect secondTimeButtonBoxRect = { 0, 0, 300, secondTimeButtonTextHeight + 15};

    //Rect para determinar o tamanho do botao do terceiro horario
    SDL_Rect thirdTimeButtonBoxRect = { 0, 0, 300, thirdTimeButtonTextHeight + 15};

    //Rect para determinar o tamanho do botao do quarto horario
    SDL_Rect fourthTimeButtonBoxRect = { 0, 0, 300, fourthTimeButtonTextHeight + 15};

    //Loop para setar o tamanho do texto e da imagem de cada filme (Usando os vetores criado anteriormente) 
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COL; col++)
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
            movieRect[row][col].x = 0;
            movieRect[row][col].y = 0;
            movieRect[row][col].w = MOVIE_WIDTH;
            movieRect[row][col].h = MOVIE_HEIGHT;
        }
    }

    //CRIACAO DAS STRINGS QUE SERAO MOSTRADAS DE ACORDO COM O QUE O USUARIO DIGITA
    //TELA DE LOGIN
    //String em que o usuario digitara o seu login/email
    char loginInputText[MAX_TEXT_LENGTH + 1];
    //A funcao memset eh usada para iniciar a string toda com 0, evita possivel lixo de memoria na tela (usada da mesma forma nos memsets abaixo)
    memset(loginInputText, 0, sizeof(loginInputText));

    //String em que o usuario digitara a sua senha
    char passwordInputText[MAX_TEXT_LENGTH + 1];
    memset(passwordInputText, 0, sizeof(passwordInputText));

    //String usada para esconder a senha na tela (tem o mesmo numero de caracteres da string acima porem somente com *)
    char passwordMask[MAX_TEXT_LENGTH + 1];
    memset(passwordMask, 0, sizeof(passwordMask));


    //TELA DE REGISTRO
    //String em que o usuario digitara a senha pela segunda vez (confirma senha)
    char confirmPasswordInputText[MAX_TEXT_LENGTH + 1];
    memset(confirmPasswordInputText, 0, sizeof(confirmPasswordInputText));

    //String usada para esconder a senha na tela (no caso a segunda senha)
    char confirmPasswordMask[MAX_TEXT_LENGTH + 1];
    memset(confirmPasswordMask, 0, sizeof(confirmPasswordMask));

    //TELA DE LOGIN
    //CRIACAO DAS SURFACES PARA O LOGIN E SENHA QUE SERA DIGITADO NA TELA
    //Nao foi necessario criar surface e texture pra PasswordMask pois eh possivel renderizar em cima das textures abaixo
    SDL_Surface* loginInputTextSurface = NULL;
    SDL_Surface* passwordInputTextSurface = NULL;

    //CRIACAO DAS TEXTURES PARA AS SURFACES ACIMA
    SDL_Texture* loginInputTextTexture = NULL;
    SDL_Texture* passwordInputTextTexture = NULL;

    //TELA DE REGISTRO
    //CRIACAO DA SURFACE E TEXTURE PARA A SEGUNDA CAIXA DE SENHA (confirma senha)
    SDL_Surface* confirmPasswordInputTextSurface = NULL;
    SDL_Texture* confirmPasswordInputTextTexture = NULL;

    //Structs para determinar qual vai ser a cor da caixa de login, senha e confirma senha
    //A mudanca de cor no codigo eh usada para determinar um erro (ex: login invalido)
    //Cor da caixa de login
    Box loginBoxColor;
    loginBoxColor.color = BLACK;
    //Cor da caixa de senha
    Box passwordBoxColor;
    passwordBoxColor.color = BLACK;
    //Cor da caixa de confirmar senha
    Box confirmPasswordBoxColor;
    confirmPasswordBoxColor.color = BLACK;

    //LOOP PRINCIPAL
    SDL_Event event; //usado para verificar se ocorreu algum evento (clique de mouse, uso do teclado, resize da tela)
    int quit = 0;
    while (!quit) {
        //Loop para determinar se ocorreu algum evento
        while (SDL_PollEvent(&event)) {
            //Evento para sair do loop/encerrar o programa
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            //Evento para lidar com o uso de teclado
            else if (event.type == SDL_KEYDOWN || event.type == SDL_TEXTINPUT) {
                // Handle keyboard input
                handleKeyboardInput(renderer, &event, window, loginInputText, passwordInputText, passwordMask, confirmPasswordInputText, confirmPasswordMask, &loginBoxColor, &passwordBoxColor, &confirmPasswordBoxColor, &dados);
            }
            //Evento para lidar com o uso de mouse
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                handleMouseClick(renderer, &event, &loginBoxRect, &passwordBoxRect, &confirmPasswordBoxRect, &cancelButtonBoxRect, &loginButtonBoxRect, &registerButtonBoxRect, &viewImageRect,
                &hideImageRect, &confirmViewImageRect, &confirmHideImageRect, movieRect, &firstTimeButtonBoxRect, &secondTimeButtonBoxRect, &thirdTimeButtonBoxRect, &fourthTimeButtonBoxRect, loginInputText, passwordInputText, confirmPasswordInputText, passwordMask, confirmPasswordMask, &quit, &loginBoxColor, &passwordBoxColor, &confirmPasswordBoxColor, &dados);
            }
            //Evento para lidar com o resize da tela
            else if (event.type == SDL_WINDOWEVENT) {
                handleWindowEvent(&event, window, &loginBoxRect, &passwordBoxRect, &confirmPasswordBoxRect, &cancelButtonBoxRect, &loginButtonBoxRect, &cinepucImageRect, &registerButtonBoxRect, &viewImageRect, &hideImageRect, &confirmViewImageRect, &confirmHideImageRect, windowWidth, windowHeight, gridWidth, gridHeight, maxTextWidth, &selectMovieRect, movieRect, textRect, renderer, font);
            }
        }
        //IF para determinar se a tela atual eh a de login
        //a variavel currentScreen muda dentro dos eventos acima de acordo com a necessidade
        if (currentScreen == LOGIN_SCREEN) {
            //ShowWindow, RaiseWindow e SetWindowInputFocus eh usado para mostrar a janela (nesse caso eh usado para alternar entre ncurses e sdl)
            SDL_ShowWindow(window);
            SDL_RaiseWindow(window);
            SDL_SetWindowInputFocus(window);
            //Funcao para conseguir o tamanho da Janela
            SDL_GetWindowSize(window, &windowWidth, &windowHeight);
            //Centralizar a interface de acordo com o tamanho da janela
            centerLoginHUD(&cinepucImageRect, &viewImageRect, &hideImageRect, &loginBoxRect, &passwordBoxRect, &loginButtonBoxRect, &cancelButtonBoxRect, &registerButtonBoxRect, windowWidth, windowHeight);
            //Renderizar a tela de Login usando as Surfaces, Textures e Rects criados anteriormente
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
            registerButtonTextHeight, registerButtonTextWidth, &loginBoxColor, &passwordBoxColor);
        }
        //ELSE IF para determinar se a tela atual eh a de cadastro
        else if (currentScreen == REGISTER_SCREEN) {
            //Funcao para conseguir o tamanho da janela
            SDL_GetWindowSize(window, &windowWidth, &windowHeight);
            //Centralizar a interface de acordo com o tamanho da janela
            centerRegisterHUD(&cinepucImageRect, &viewImageRect, &hideImageRect, &confirmViewImageRect, &confirmHideImageRect, &loginBoxRect, &passwordBoxRect, &confirmPasswordBoxRect, &loginButtonBoxRect, &cancelButtonBoxRect, windowWidth, windowHeight);
            //Renderizar a tela de cadastro usando as Surfaces, Textures e Rects criados anteriormente
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
            loginButtonTextHeight, loginButtonTextWidth, &loginBoxColor, &passwordBoxColor, &confirmPasswordBoxColor);
        }
        //ELSE IF para determinar se a tela atual eh a de selecionar filmes
        else if (currentScreen == MOVIE_SCREEN) {
            //Funcao para conseguir o tamanho da janela
            SDL_GetWindowSize(window, &windowWidth, &windowHeight);
            //Renderizar a tela de selecionar filmes usando as Surfaces, Textures e Rects criados anteriormente
            renderMovieScreen(renderer, font, selectMovieText, selectMovieWidth, selectMovieHeight, &selectMovieRect, maxTextWidth, offsetX, offsetY, movieRect, textRect, selectMovieTextSurface, selectMovieTextTexture, windowWidth, windowHeight, gridWidth, gridHeight);
        }
        //ELSE IF para determinar se a tela atual eh a de selecionar horarios
        else if (currentScreen == TIME_SELECTION_SCREEN) {
            //Mesma ideia da tela de login, funcoes usadas para alternar entre ncurses e sdl
            SDL_ShowWindow(window);
            SDL_RaiseWindow(window);
            SDL_SetWindowInputFocus(window);
            //Funcao para conseguir o tamanho da tela
            SDL_GetWindowSize(window, &windowWidth, &windowHeight);
            //Centralizar a interface de acordo com o tamanho da janela
            centerTimeSelectionHUD(&selectTimeTextRect, &firstTimeButtonBoxRect, &secondTimeButtonBoxRect, &thirdTimeButtonBoxRect, &fourthTimeButtonBoxRect, &cancelButtonBoxRect, &cinepucImageRect, windowWidth, windowHeight);
            //Renderizar a tela de selecionar horarios usando as Surfaces, Textures e Rects criados anteriormente
            renderTimeSelectionScreen(renderer, selectTimeTextTexture, selectTimeTextWidth, selectTimeTextHeight, &firstTimeButtonBoxRect, firstTimeButtonTextTexture, firstTimeButtonTextWidth, firstTimeButtonTextHeight, &secondTimeButtonBoxRect, secondTimeButtonTextTexture, secondTimeButtonTextWidth, secondTimeButtonTextHeight, &thirdTimeButtonBoxRect, thirdTimeButtonTextTexture, thirdTimeButtonTextWidth, thirdTimeButtonTextHeight, &fourthTimeButtonBoxRect, fourthTimeButtonTextTexture, fourthTimeButtonTextWidth, fourthTimeButtonTextHeight, selectTimeTextRect, &cancelButtonBoxRect, cancelButtonTextTexture, cinepucImage, cinepucImageRect, cancelButtonTextWidth, cancelButtonTextHeight);
        }
        //ELSE IF para determinar se a tela atual eh a de selecionar assentos
        else if (currentScreen == SEAT_SELECTION_SCREEN) {
            for (int i = 0; i < CHAIR_AMOUNT; i++) {
                status[i] = 0;
            }
            Carrinho carrinho;
            cria(&carrinho);
            //Funcao para esconder a tela SDL
            SDL_HideWindow(window);
            //Init no Ncurses
            initscr();
            //Funcoes para inicializar o teclado
            cbreak();
            keypad(stdscr, TRUE);
            //Funcao para inicializar o mouse
            mousemask(ALL_MOUSE_EVENTS, NULL); 
            //Funcao para esconder o cursor (a area de texto que fica piscando esperando pelo texto)
            curs_set(0);
            //Init das cores
            initColors();
            //Funcao para desenhar as cadeiras
            drawChairMenu();
            //Funcao para desenhar a TELA abaixo das cadeiras
            drawTELA();
            //Funcoes para desenhar os botoes Cancelar e Confirmar
            WINDOW* CANCEL = drawCANCEL();
            WINDOW* CONFIRM = drawCONFIRM();
            //Funcao para determinar se alguma cadeira/botao foi clicado 
            mouseFunc(CANCEL, CONFIRM, &dados, &carrinho);
            getch();
            //Funcao para retornar o cursor (a area de texto que fica piscando esperando pelo texto)
            curs_set(1);
            //Funcao para encerrar o modo Ncurses
            endwin();
            mostra(carrinho);
            int esvazia = esvaziaCarrinho(&carrinho);
            printf("\nCadeiras liberadas: %d\n", esvazia);
            lerArquivo("cadeirasAVT.bin");
        }
    }

    // Limpar todas as surfaces e textures para evitar vazamento de memoria
    cleanUp (cinepucImage, viewImage, hideImage, loginTextTexture, passwordTextTexture, loginButtonTextTexture, cancelButtonTextTexture, registerButtonTextTexture, loginInputTextTexture, passwordInputTextTexture, loginTextSurface, passwordTextSurface, loginButtonTextSurface, cancelButtonTextSurface, registerButtonTextSurface, loginInputTextSurface, passwordInputTextSurface, confirmPasswordTextSurface, confirmPasswordTextTexture, confirmPasswordInputTextSurface, confirmPasswordInputTextTexture, selectMovieTextTexture, selectMovieTextSurface, selectTimeTextSurface, firstTimeButtonTextSurface, secondTimeButtonTextSurface, thirdTimeButtonTextSurface, fourthTimeButtonTextSurface, selectTimeTextTexture, firstTimeButtonTextTexture, secondTimeButtonTextTexture, thirdTimeButtonTextTexture, fourthTimeButtonTextTexture, window, font, renderer);
}

int initSDL() {
      // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Falha ao iniciar o SDL: %s", SDL_GetError());
        return 1;
    }

    // Init SDL_ttf (fontes)
    if (TTF_Init() != 0) {
        SDL_Log("Falha ao iniciar o SDL_ttf: %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    return 0;
}

SDL_Window* initWindow() {
    //Criar Janela
    SDL_Window* window = SDL_CreateWindow(
        "Cinepuc",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1200, 800,  //Tamanho Janela
        SDL_WINDOW_RESIZABLE
    );
    //Verificacao para ver se foi possivel criar a janela
    if (!window) {
        SDL_Log("Falha ao abrir a Janela: %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return NULL;
    }

    return window;
}

SDL_Renderer* initRender(SDL_Window* window) {
    //Criar Render
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //Verificacao para ver se foi possivel criar o renderer
    if (!renderer) {
        SDL_Log("Falha ao criar o Render: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return NULL;
    }

    return renderer;
}

TTF_Font* initFont(SDL_Window* window, SDL_Renderer* renderer) {
    //Iniciar a fonte de texto
    TTF_Font* font = TTF_OpenFont("interface/src/assets/arial.ttf", 24);
    //Verificacao para ver se foi possivel abrir a fonte
    if (!font) {
        SDL_Log("Falha ao carregar a fonte: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return NULL;
    }

    return font;
}

SDL_Texture* initImage(SDL_Window* window, SDL_Renderer* renderer, char* link){
    //Carregar a textura da imagem no Render
    SDL_Texture* cinepucImage = IMG_LoadTexture(renderer, link);
    //Verificacao para ver se foi possivel carregar a imagem
    if(!cinepucImage) {
        SDL_Log("Falha ao abrir a imagem: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return NULL;
    }
    return cinepucImage;
}

//Funcao para inicializar as Surfaces
SDL_Surface* initSurface(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, char* text, SDL_Color textColor) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, textColor);
    if (!surface) {
        SDL_Log("Falha ao criar Surface: %s", TTF_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return NULL;
    }
    return surface;
}

//Funcao para inicializar as Textures
SDL_Texture* initTexture(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, SDL_Surface* surface) {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Falha ao criar Texture: %s", SDL_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return NULL;
    }
    return texture;
}

//Funcao para lidar com o teclado
void handleKeyboardInput(SDL_Renderer* renderer, SDL_Event* event, SDL_Window* window, char* loginInputText, char* passwordInputText, char* passwordMask, char* confirmPasswordInputText, char* confirmPasswordMask, Box* loginBoxColor, Box* passwordBoxColor, Box* confirmPasswordBoxColor, dadosFrontEnd* dados) {
    //TELA DE LOGIN
    if (currentScreen == LOGIN_SCREEN) {
        //BACKSPACE/APAGAR
        if (event->key.keysym.sym == SDLK_BACKSPACE) {
            //Remove a ultima letra da string de acordo com a caixa selecionada (login ou senha)
            if (event->window.windowID == SDL_GetWindowID(window)) {
                if (activeField == LOGIN_FIELD && strlen(loginInputText) > 0) {
                    loginInputText[strlen(loginInputText) - 1] = '\0';
                }
                else if (activeField == PASSWORD_FIELD && strlen(passwordInputText) > 0) {
                    //atualizacao da mask simultameamente com o texto de senha
                    passwordInputText[strlen(passwordInputText) - 1] = '\0';
                    passwordMask[strlen(passwordMask) - 1] = '\0';
                }
            }
        }
        //ENTER
        else if (event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_KP_ENTER) {
            //printf para mostrar os dados inseridos
            printf("Login: %s\n", loginInputText);
            printf("Senha: %s\n", passwordInputText);
            fflush(stdout);
            //Funcao para validar o usuario (ver se a o login/email e senha estao corretos)
            if(validarUsuario(loginInputText, passwordInputText)){ //funcao do backend
                //se o usuario for validado segue para a tela de selecionar filmes
                currentScreen = MOVIE_SCREEN;
                //copia o usuario logado para a struct dados (sera utilizada para armazenar o ingresso no email/login)
                strcpy(dados->usuarioLogado, loginInputText);
            }
            else{
                loginBoxColor->color = RED;
                passwordBoxColor->color = RED;
            }
        }
        //TAB
        else if (event->key.keysym.sym == SDLK_TAB) {
            //se estiver no campo de senha e apertar tab, muda para o campo de login, alternando entre si
            if (activeField == PASSWORD_FIELD) {
                activeField = LOGIN_FIELD;
            } else if (activeField == LOGIN_FIELD) {
                activeField = PASSWORD_FIELD;
            }
        }
        //IFS PARA DETECTAR O INPUT DO USUARIO
        //logo apos digitar uma letra ela eh concatenada na string login ou senha
        else if (activeField == LOGIN_FIELD && strlen(loginInputText) < MAX_TEXT_LENGTH) {
            //Concatena a letra
            if (event->type == SDL_TEXTINPUT) {
                strcat(loginInputText, event->text.text);
            }
        }
        else if (activeField == PASSWORD_FIELD && strlen(passwordInputText) < MAX_TEXT_LENGTH) {
            //Concatena a letra
            if (event->type == SDL_TEXTINPUT) {
                //event->text.text eh o texto digitado
                strcat(passwordInputText, event->text.text);
                strcat(passwordMask, "*");
            }
        }
    }
    //TELA DE REGISTRO
    else if (currentScreen == REGISTER_SCREEN) {
        //BACKSPACE/APAGAR
        if (event->key.keysym.sym == SDLK_BACKSPACE) {
            //Remove a ultima letra da string de acordo com a caixa selecionada (login ou senha)
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
        //ENTER
        else if (event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_KP_ENTER) {
            printf("Login: %s\n", loginInputText);
            printf("Senha: %s\n", passwordInputText);
            printf("Confirma senha: %s\n", confirmPasswordInputText);
            fflush(stdout);
            //Verificacao se as duas senha sao iguais
            if(strcmp(passwordInputText, confirmPasswordInputText) == 0) {
                //SALVA USUARIO NO ARQUIVO
                int verificar = salvarUsuarioArquivo(loginInputText, passwordInputText);
                //verifica se o nome de usuario existe ou nao
                //se ja existir, a cor da caixa de login muda para vermelho
                //se nao, voltamos para tela de Login
                if(verificar == 0){
                    printf("Usuario repetido.\n");
                    loginBoxColor->color = RED;
                }
                else if(verificar == 1){
                    printf("Usuario cadastrado com sucesso.\n");
                    currentScreen = LOGIN_SCREEN;
                    clearInput = CLEAR;
                }
            }
            else{
                //se as senhas nao sao iguais muda as caixas para vermelho
                passwordBoxColor->color = RED;
                confirmPasswordBoxColor->color = RED;
            }
        }
        //TAB
        else if (event->key.keysym.sym == SDLK_TAB) {
            //se estiver no campo de login e apertar tab, muda para o campo de senha, se estiver no campo de senha muda para o de confirma senha, alternando entre si
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
        //Os proximos 3 ELSE IFs sao usados para realizar o armazenamento do input realizado na tela
        //CAIXA DE LOGIN
        else if (activeField == LOGIN_FIELD && strlen(loginInputText) < MAX_TEXT_LENGTH) {
            //Concatena a letra digitada na tela na string loginInputText
            if (event->type == SDL_TEXTINPUT) {
                strcat(loginInputText, event->text.text);
            }
        }
        //CAIXA DA SENHA
        else if (activeField == PASSWORD_FIELD && strlen(passwordInputText) < MAX_TEXT_LENGTH) {
            if (event->type == SDL_TEXTINPUT) {
                //Concatena a letra digitada na tela na string passwordInputText
                strcat(passwordInputText, event->text.text);
                //Concatena um asterisco ("*") na passwordMask (usado para esconder a senha)
                strcat(passwordMask, "*");
            }
        }
        //SEGUNDA CAIXA DA SENHA (confirma senha)
        else if (activeField == CONFIRM_FIELD && strlen(confirmPasswordInputText) < MAX_TEXT_LENGTH) {
            if (event->type == SDL_TEXTINPUT) {
                //Concatena a letra digitada na tela na string confirmPasswordInputText
                strcat(confirmPasswordInputText, event->text.text);
                //Concatena um asterisco ("*") na confirmPasswordMask (usado para esconder a segunda senha (confirma senha))
                strcat(confirmPasswordMask, "*");
            }
        }
    }
}

void handleMouseClick(SDL_Renderer* renderer, SDL_Event* event, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* confirmPasswordBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* registerButtonBoxRect, SDL_Rect* viewImageRect, SDL_Rect* hideImageRect, SDL_Rect* confirmViewImageRect, SDL_Rect* confirmHideImageRect, SDL_Rect movieRect[][COL], SDL_Rect* firstTimeButtonBoxRect, SDL_Rect* secondTimeButtonBoxRect, SDL_Rect* thirdTimeButtonBoxRect, SDL_Rect* fourthTimeButtonBoxRect, char* loginInputText, char* passwordInputText, char* confirmPasswordInputText, char* passwordMask, char* confirmPasswordMask, int* quit, Box* loginBoxColor, Box* passwordBoxColor, Box* confirmPasswordBoxColor, dadosFrontEnd* dados) {
    //IF que determina se o botão esquerdo do mouse foi clicado
    if (event->button.button == SDL_BUTTON_LEFT) {
        //variaveis para determinar onde o clique foi feito
        int mouseX = event->button.x;
        int mouseY = event->button.y;

        //TELA DE LOGIN
        if (currentScreen == LOGIN_SCREEN) {
            //Verifica se o clique foi dentro da caixa de login
            if (mouseX >= loginBoxRect->x && mouseX <= loginBoxRect->x + loginBoxRect->w &&
                mouseY >= loginBoxRect->y && mouseY <= loginBoxRect->y + loginBoxRect->h) {
                activeField = LOGIN_FIELD;
                loginBoxColor->color = BLACK;
                passwordBoxColor->color = BLACK;
            }
            //Verifica se o clique foi dentro da imagem para mostrar a senha (Olho)
            else if (mouseX >= viewImageRect->x && mouseX <= viewImageRect->x + viewImageRect->w &&
                mouseY >= viewImageRect->y && mouseY <= viewImageRect->y + viewImageRect->h && showPassword == HIDE) {
                showPassword = SHOW;
            }
            //Verifica se o clique foi dentro da imagem para esconder a senha (Olho)
            else if (mouseX >= hideImageRect->x && mouseX <= hideImageRect->x + hideImageRect->w &&
                mouseY >= hideImageRect->y && mouseY <= hideImageRect->y + hideImageRect->h && showPassword == SHOW) {
                showPassword = HIDE;
            }
            //Verifica se o clique foi dentro da caixa de senha
            else if (mouseX >= passwordBoxRect->x && mouseX <= passwordBoxRect->x + passwordBoxRect->w &&
                mouseY >= passwordBoxRect->y && mouseY <= passwordBoxRect->y + passwordBoxRect->h) {
                activeField = PASSWORD_FIELD;
                loginBoxColor->color = BLACK;
                passwordBoxColor->color = BLACK;
            }
            //Verifica se o clique foi dentro do botao "Cancelar"
            else if (mouseX >= cancelButtonBoxRect->x && mouseX <= cancelButtonBoxRect->x + cancelButtonBoxRect->w &&
                mouseY >= cancelButtonBoxRect->y && mouseY <= cancelButtonBoxRect->y + cancelButtonBoxRect->h) {
                //Encerra a aplicacao
                *quit = 1;
            }
            //Verifica se o clique foi dentro do botao "Confirmar"
            else if (mouseX >= loginButtonBoxRect->x && mouseX <= loginButtonBoxRect->x + loginButtonBoxRect->w &&
                mouseY >= loginButtonBoxRect->y && mouseY <= loginButtonBoxRect->y + loginButtonBoxRect->h) {
                //printf para mostrar os dados inseridos
                printf("Login: %s\n", loginInputText);
                printf("Senha: %s\n", passwordInputText);
                fflush(stdout);
                //Funcao para validar o usuario (ver se a o login/email e senha estao corretos)
                if(validarUsuario(loginInputText, passwordInputText)){ //funcao do backend
                    //se o usuario for validado segue para a tela de selecionar filmes
                    currentScreen = MOVIE_SCREEN;
                    //copia o usuario logado para a struct dados (sera utilizada para armazenar o ingresso no email/login)
                    strcpy(dados->usuarioLogado, loginInputText);
                }
                else{
                    loginBoxColor->color = RED;
                    passwordBoxColor->color = RED;
                }
            }
            //Verifica se o clique foi dentro do botao "Cadastrar"
            else if (mouseX >= registerButtonBoxRect->x && mouseX <= registerButtonBoxRect->x + registerButtonBoxRect->w &&
                mouseY >= registerButtonBoxRect->y && mouseY <= registerButtonBoxRect->y + registerButtonBoxRect->h) {
                fflush(stdout);
                //muda para a tela de cadastro
                currentScreen = REGISTER_SCREEN;
                //limpa as entradas de texto
                clearInput = CLEAR;
            }
        }
        //TELA DE REGISTRO
        else if (currentScreen == REGISTER_SCREEN) {
            //Verifica se o clique foi dentro da caixa de login
            if (mouseX >= loginBoxRect->x && mouseX <= loginBoxRect->x + loginBoxRect->w &&
                mouseY >= loginBoxRect->y && mouseY <= loginBoxRect->y + loginBoxRect->h) {
                activeField = LOGIN_FIELD;
                loginBoxColor->color = BLACK;
            }
            //Verifica se o clique foi dentro da imagem (Olho) para mostrar a senha
            else if (mouseX >= viewImageRect->x && mouseX <= viewImageRect->x + viewImageRect->w &&
                mouseY >= viewImageRect->y && mouseY <= viewImageRect->y + viewImageRect->h && showPassword == HIDE) {
                showPassword = SHOW;
            }
            //Verifica se o clique foi dentro da imagem (Olho) para esconder a senha
            else if (mouseX >= hideImageRect->x && mouseX <= hideImageRect->x + hideImageRect->w &&
                mouseY >= hideImageRect->y && mouseY <= hideImageRect->y + hideImageRect->h && showPassword == SHOW) {
                showPassword = HIDE;
            }
            //Verifica se o clique foi dentro da imagem (Olho) para mostrar a SEGUNDA senha (confirma senha) 
            else if (mouseX >= confirmViewImageRect->x && mouseX <= confirmViewImageRect->x + confirmViewImageRect->w &&
                mouseY >= confirmViewImageRect->y && mouseY <= confirmViewImageRect->y + confirmViewImageRect->h && showConfirmPassword == HIDE) {
                showConfirmPassword = SHOW;
            }
            //Verifica se o clique foi dentro da imagem (Olho) para esconder a SEGUNDA senha (confirma senha)
            else if (mouseX >= confirmHideImageRect->x && mouseX <= confirmHideImageRect->x + confirmHideImageRect->w &&
                mouseY >= confirmHideImageRect->y && mouseY <= confirmHideImageRect->y + confirmHideImageRect->h && showConfirmPassword == SHOW) {
                showConfirmPassword = HIDE;
            }
            //Verifica se o clique foi dentro da caixa de senha
            else if (mouseX >= passwordBoxRect->x && mouseX <= passwordBoxRect->x + passwordBoxRect->w &&
                mouseY >= passwordBoxRect->y && mouseY <= passwordBoxRect->y + passwordBoxRect->h) {
                activeField = PASSWORD_FIELD;
                passwordBoxColor->color = BLACK;
            }
            //Verifica se o clique foi dentro da segunda caixa de senha (confirma senha)
            else if (mouseX >= confirmPasswordBoxRect->x && mouseX <= confirmPasswordBoxRect->x + confirmPasswordBoxRect->w &&
                mouseY >= confirmPasswordBoxRect->y && mouseY <= confirmPasswordBoxRect->y + confirmPasswordBoxRect->h) {
                activeField = CONFIRM_FIELD;
                confirmPasswordBoxColor->color = BLACK;
            }
            //Verifica se o clique foi dentro do botao "Cancelar"
            else if (mouseX >= cancelButtonBoxRect->x && mouseX <= cancelButtonBoxRect->x + cancelButtonBoxRect->w &&
                mouseY >= cancelButtonBoxRect->y && mouseY <= cancelButtonBoxRect->y + cancelButtonBoxRect->h) {
                //retorna para a tela de login
                currentScreen = LOGIN_SCREEN;
                //limpa as entradas de texto
                clearInput = CLEAR;
            }
            //Verifica se o clique foi dentro do botao "Confirmar"
            else if (mouseX >= loginButtonBoxRect->x && mouseX <= loginButtonBoxRect->x + loginButtonBoxRect->w &&
                mouseY >= loginButtonBoxRect->y && mouseY <= loginButtonBoxRect->y + loginButtonBoxRect->h) {
                //mostra os dados usados no cadastro
                printf("Entered login: %s\n", loginInputText);
                printf("Entered password: %s\n", passwordInputText);
                printf("Entered confirm password: %s\n", confirmPasswordInputText);
                fflush(stdout);
                //verifica se a senha digitada na primeira e na segunda caixa sao iguais
                if(strcmp(passwordInputText, confirmPasswordInputText) == 0) {
                    //Salva o usuario no arquivo e tem como retorno 0 (se o usuario ja existe) ou 1 (cadastro com sucesso)
                    int verificar = salvarUsuarioArquivo(loginInputText, passwordInputText);
                    //verifica se o usuario ja existe
                    if(verificar == 0){
                        printf("Usuario repetido.\n");
                        //muda a caixa de login para vermelho (para demonstrar que o erro eh no login)
                        loginBoxColor->color = RED;
                    }
                    else if(verificar == 1){
                        //cadastro com sucesso, volta para a tela de login
                        printf("Usuario cadastrado com sucesso.\n");
                        currentScreen = LOGIN_SCREEN;
                        clearInput = CLEAR;
                    }
                }
                else{
                    //muda as caixas das senhas para vermelho (para demonstrar que o erro eh na senha)
                    passwordBoxColor->color = RED;
                    confirmPasswordBoxColor->color = RED;
                }
            }
        }
        //TELA PARA SELECIONAR FILME
        else if (currentScreen == MOVIE_SCREEN) {
            //Verifica se o clique foi na primeira imagem/filme
            if (mouseX >= movieRect[0][0].x && mouseX <= movieRect[0][0].x + movieRect[0][0].w &&
                mouseY >= movieRect[0][0].y && mouseY <= movieRect[0][0].y + movieRect[0][0].h) {
                //copia o id do filme para a struct dados (sera utilizada para armazenar o ingresso no email/login)
                dados->idFilme = 1;
                //muda para a tela de selecionar horarios
                currentScreen = TIME_SELECTION_SCREEN;
            }
            //Verifica se o clique foi na segunda imagem/filme
            else if (mouseX >= movieRect[0][1].x && mouseX <= movieRect[0][1].x + movieRect[0][1].w &&
                mouseY >= movieRect[0][1].y && mouseY <= movieRect[0][1].y + movieRect[0][1].h) {
                //copia o id do filme para a struct dados (sera utilizada para armazenar o ingresso no email/login)
                dados->idFilme = 2;
                //muda para a tela de selecionar horarios
                currentScreen = TIME_SELECTION_SCREEN;
            }
            //Verifica se o clique foi na terceira imagem/filme
            else if (mouseX >= movieRect[1][0].x && mouseX <= movieRect[1][0].x + movieRect[1][0].w &&
                mouseY >= movieRect[1][0].y && mouseY <= movieRect[1][0].y + movieRect[1][0].h) {
                //copia o id do filme para a struct dados (sera utilizada para armazenar o ingresso no email/login)
                dados->idFilme = 3;
                //muda para a tela de selecionar horarios
                currentScreen = TIME_SELECTION_SCREEN;
            }
            //Verifica se o clique foi na quarta imagem/filme
            else if (mouseX >= movieRect[1][1].x && mouseX <= movieRect[1][1].x + movieRect[1][1].w &&
                mouseY >= movieRect[1][1].y && mouseY <= movieRect[1][1].y + movieRect[1][1].h) {
                //copia o id do filme para a struct dados (sera utilizada para armazenar o ingresso no email/login)
                dados->idFilme = 4;
                //muda para a tela de selecionar horarios
                currentScreen = TIME_SELECTION_SCREEN;
            }
        }
        //TELA DE SELECIONAR HORARIOS
        else if (currentScreen == TIME_SELECTION_SCREEN) {
            //verifica se o clique foi no primeiro horario
            if(mouseX >= firstTimeButtonBoxRect->x && mouseX <= firstTimeButtonBoxRect->x + firstTimeButtonBoxRect->w &&
                mouseY >= firstTimeButtonBoxRect->y && mouseY <= firstTimeButtonBoxRect->y + firstTimeButtonBoxRect->h){
                    //copia o id da sessao/filme para a struct dados (sera utilizada para armazenar o ingresso no email/login)
                    dados->idSessao = 1;
                    //muda para a tela de selecionar Assentos
                    currentScreen = SEAT_SELECTION_SCREEN;
            }
            //verifica se o clique foi no segundo horario
            else if(mouseX >= secondTimeButtonBoxRect->x && mouseX <= secondTimeButtonBoxRect->x + secondTimeButtonBoxRect->w &&
                mouseY >= secondTimeButtonBoxRect->y && mouseY <= secondTimeButtonBoxRect->y + secondTimeButtonBoxRect->h){
                    //copia o id da sessao/filme para a struct dados (sera utilizada para armazenar o ingresso no email/login)
                    dados->idSessao = 2;
                    //muda para a tela de selecionar Assentos
                    currentScreen = SEAT_SELECTION_SCREEN;
            }
            //verifica se o clique foi no terceiro horario
            else if(mouseX >= thirdTimeButtonBoxRect->x && mouseX <= thirdTimeButtonBoxRect->x + thirdTimeButtonBoxRect->w &&
                mouseY >= thirdTimeButtonBoxRect->y && mouseY <= thirdTimeButtonBoxRect->y + thirdTimeButtonBoxRect->h){
                    //copia o id da sessao/filme para a struct dados (sera utilizada para armazenar o ingresso no email/login)
                    dados->idSessao = 3;
                    //muda para a tela de selecionar Assentos
                    currentScreen = SEAT_SELECTION_SCREEN;
            }
            //verifica se o clique foi no quarto horario
            else if(mouseX >= fourthTimeButtonBoxRect->x && mouseX <= fourthTimeButtonBoxRect->x + fourthTimeButtonBoxRect->w &&
                mouseY >= fourthTimeButtonBoxRect->y && mouseY <= fourthTimeButtonBoxRect->y + fourthTimeButtonBoxRect->h){
                    //copia o id da sessao/filme para a struct dados (sera utilizada para armazenar o ingresso no email/login)
                    dados->idSessao = 4;
                    //muda para a tela de selecionar Assentos
                    currentScreen = SEAT_SELECTION_SCREEN;
            }
            else if (mouseX >= cancelButtonBoxRect->x && mouseX <= cancelButtonBoxRect->x + cancelButtonBoxRect->w &&
                mouseY >= cancelButtonBoxRect->y && mouseY <= cancelButtonBoxRect->y + cancelButtonBoxRect->h) {
                currentScreen = MOVIE_SCREEN;
            }
        }
    }
}

void handleWindowEvent(SDL_Event* event, SDL_Window *window, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* confirmPasswordBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* cinepucImageRect, SDL_Rect* registerButtonBoxRect, SDL_Rect* viewImageRect, SDL_Rect* hideImageRect, SDL_Rect* confirmViewImageRect, SDL_Rect* confirmHideImageRect, int windowWidth, int windowHeight, int gridWidth, int gridHeight, int maxTextWidth, SDL_Rect* selectMovieRect, SDL_Rect movieRect[][COL], SDL_Rect textRect[][COL], SDL_Renderer* renderer, TTF_Font* font) {
    //evento para verificar se a tela teve algum resize
    if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
        //get novo tamanho da tela
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        //TELA DE LOGIN
        if (currentScreen == LOGIN_SCREEN) {
            //centraliza a hud de acordo com o nov tamanho da tela
            centerLoginHUD(cinepucImageRect, viewImageRect, hideImageRect, loginBoxRect, passwordBoxRect, loginButtonBoxRect, cancelButtonBoxRect, registerButtonBoxRect, windowWidth, windowHeight);
        }
        //TELA DE CADASTRO
        else if (currentScreen == REGISTER_SCREEN) {
            //centraliza a hud de acordo com o nov tamanho da tela
            centerRegisterHUD(cinepucImageRect, viewImageRect, hideImageRect, confirmViewImageRect, confirmHideImageRect, loginBoxRect, passwordBoxRect, confirmPasswordBoxRect, loginButtonBoxRect, cancelButtonBoxRect, windowWidth, windowHeight);
        }
        //TELA DE SELECIONAR FILMES
        else if (currentScreen == MOVIE_SCREEN) {
            //centraliza a hud de acordo com o nov tamanho da tela
            centerMovieHUD(windowWidth, windowHeight, gridWidth, gridHeight, maxTextWidth, selectMovieRect, movieRect, textRect, renderer, font);
        }
    }
}

//RENDERIZAR A TELA DE LOGIN
void renderLoginScreen(SDL_Renderer* renderer, TTF_Font* font, SDL_Color textColor,
            SDL_Texture* cinepucImage, SDL_Rect cinepucImageRect, SDL_Texture* viewImage, SDL_Rect viewImageRect, SDL_Texture* hideImage, SDL_Rect hideImageRect,
            SDL_Rect loginBoxRect, SDL_Rect passwordBoxRect, SDL_Rect cancelButtonBoxRect, SDL_Rect loginButtonBoxRect, SDL_Rect registerButtonBoxRect,
            SDL_Texture* loginTextTexture, SDL_Texture* passwordTextTexture, SDL_Texture* cancelButtonTextTexture, SDL_Texture* loginButtonTextTexture, SDL_Texture* registerButtonTextTexture,
            SDL_Surface* loginInputTextSurface, SDL_Texture* loginInputTextTexture, SDL_Surface* passwordInputTextSurface, SDL_Texture* passwordInputTextTexture,
            char* passwordInputText, char* loginInputText, int loginTextHeight, int loginTextWidth, int passwordTextHeight, int passwordTextWidth, char* passwordMask,
            int cancelButtonTextHeight, int cancelButtonTextWidth, int loginButtonTextHeight, int loginButtonTextWidth, int registerButtonTextHeight, int registerButtonTextWidth, Box* loginBoxColor, Box* passwordBoxColor) {
    //Limpa a tela
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color
    SDL_RenderClear(renderer);

    //Limpa os inputs de texto + cor das caixas + mostrar/esconder senha
    if(clearInput == CLEAR) {
        strcpy(loginInputText, "");
        strcpy(passwordInputText, "");
        strcpy(passwordMask, "");
        activeField = LOGIN_FIELD;
        showPassword = SHOW;
        loginBoxColor->color = BLACK;
        passwordBoxColor->color = BLACK;
        clearInput = DONT_CLEAR;
    }

    //Renderiza a caixa de Login de acordo com a cor necessaria
    if(loginBoxColor->color == BLACK){
        SDL_SetRenderDrawColor(renderer, 52, 52, 54, 255);  // Preto
    }
    else if(loginBoxColor->color == RED){
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Vermelho
    }
    SDL_RenderDrawRect(renderer, &loginBoxRect);

    //Renderiza a imagem Cinepuc
    SDL_RenderCopy(renderer, cinepucImage, NULL, &cinepucImageRect);

    //Renderiza a imagem do olho
    if(showPassword == SHOW){
        SDL_RenderCopy(renderer, viewImage, NULL, &viewImageRect);
    }
    //Renderiza a image do outro olho
    else if(showPassword == HIDE){
        SDL_RenderCopy(renderer, hideImage, NULL, &hideImageRect);
    }

    //Determina a posicao do texto "E-mail"
    SDL_Rect loginTextRect = { loginBoxRect.x, loginBoxRect.y - loginTextHeight, loginTextWidth, loginTextHeight };
    //Renderiza o texto "E-mail:" logo acima da caixa de input para o login/email
    SDL_RenderCopy(renderer, loginTextTexture, NULL, &loginTextRect);
    //Variaveis para determinar onde sera colocado o input text da caixa login
    int loginTextPosX = loginBoxRect.x + 10;
    int loginTextPosY = loginBoxRect.y + (loginBoxRect.h - loginTextHeight) / 2;

    //If necessario pois quando tentamos renderizar um texto vazio ocorre o erro: Segmentation fault (core dumped)
    if (strlen(loginInputText) > 0) {
        //Cria a surface para o input text da caixa login
        loginInputTextSurface = TTF_RenderText_Solid(font, loginInputText, textColor);
        if (!loginInputTextSurface) {
            SDL_Log("Falha ao criar Surface: %s", TTF_GetError());
            return;
        }
        
        //Cria a texture para o input text da caixa login
        loginInputTextTexture = SDL_CreateTextureFromSurface(renderer, loginInputTextSurface);
        if (!loginInputTextTexture) {
            SDL_Log("Falha ao criar Texture %s", SDL_GetError());
            SDL_FreeSurface(loginInputTextSurface);
            loginInputTextSurface = NULL;
            return;
        }

        //Variaveis para determinar o tamanho do texto
        int loginInputTextWidth = loginInputTextSurface->w;
        int loginInputTextHeight = loginInputTextSurface->h;
        //Posiciona a caixa de acordo com as variaveis criadas acima do if
        SDL_Rect loginInputTextRect = { loginTextPosX, loginTextPosY, loginInputTextWidth, loginInputTextHeight };
        //Renderiza o texto de input da caixa de login
        SDL_RenderCopy(renderer, loginInputTextTexture, NULL, &loginInputTextRect);
    }

    //Renderiza a caixa de senha de acordo com a cor necessaria
    if(passwordBoxColor->color == BLACK){
        SDL_SetRenderDrawColor(renderer, 52, 52, 54, 255);  // Preto
    }
    else if(passwordBoxColor->color == RED){
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Vermelho
    }
    SDL_RenderDrawRect(renderer, &passwordBoxRect);

    //Determina a posicao do texto "Senha:"
    SDL_Rect passwordTextRect = { passwordBoxRect.x, passwordBoxRect.y - passwordTextHeight, passwordTextWidth, passwordTextHeight };
    //Renderiza o texto "Senha:"
    SDL_RenderCopy(renderer, passwordTextTexture, NULL, &passwordTextRect);
    //Variaveis para determinar onde sera colocado o input text da caixa senha
    int passwordTextPosX = passwordBoxRect.x + 10;
    int passwordTextPosY = passwordBoxRect.y + (passwordBoxRect.h - passwordTextHeight) / 2;

    //If necessario pois quando tentamos renderizar um texto vazio ocorre o erro: Segmentation fault (core dumped)
    if (strlen(passwordInputText) > 0) {
        //Cria a surface para o input text da caixa senha, baseado na opcao do usuario de querer mostrar ou nao a senha
        if(showPassword == SHOW)
            //se nao quiser mostrar, cria a surface dos asteriscos
            passwordInputTextSurface = TTF_RenderText_Solid(font, passwordMask, textColor);
        else if (showPassword == HIDE)
            //se quiser mostrar, cria a surface da propria senha
            passwordInputTextSurface = TTF_RenderText_Solid(font, passwordInputText, textColor);

        if (!passwordInputTextSurface) {
            SDL_Log("Falha ao criar Surface: %s", TTF_GetError());
            return;
        }

        //Cria a texture para o input text da caixa senha, baseado na escolha de Surface acima
        passwordInputTextTexture = SDL_CreateTextureFromSurface(renderer, passwordInputTextSurface);
        if (!passwordInputTextTexture) {
            SDL_Log("Falha ao criar Texture: %s", SDL_GetError());
            SDL_FreeSurface(passwordInputTextSurface);
            passwordInputTextSurface = NULL;
            return;
        }

        //Variaveis para determinar o tamanho do texto
        int passwordInputTextWidth = passwordInputTextSurface->w;
        int passwordInputTextHeight = passwordInputTextSurface->h;
        //Posiciona a caixa de acordo com o texto "Senha:" (no caso logo abaixo)
        SDL_Rect passwordInputTextRect = { passwordTextPosX, passwordTextPosY, passwordInputTextWidth, passwordInputTextHeight };
        //Renderiza o texto de input da caixa de senha
        SDL_RenderCopy(renderer, passwordInputTextTexture, NULL, &passwordInputTextRect);
    }

    //Posiciona e faz uma copia do render do botao "Cancelar"
    SDL_Rect cancelButtonTextRect = { cancelButtonBoxRect.x + (cancelButtonBoxRect.w - cancelButtonTextWidth) / 2, cancelButtonBoxRect.y + (cancelButtonBoxRect.h - cancelButtonTextHeight) / 2, cancelButtonTextWidth, cancelButtonTextHeight };
    SDL_RenderCopy(renderer, cancelButtonTextTexture, NULL, &cancelButtonTextRect);

    //Posiciona e faz uma copia do render do botao "Confirmar"
    SDL_Rect loginButtonTextRect = { loginButtonBoxRect.x + (loginButtonBoxRect.w - loginButtonTextWidth) / 2, loginButtonBoxRect.y + (loginButtonBoxRect.h - loginButtonTextHeight) / 2, loginButtonTextWidth, loginButtonTextHeight };
    SDL_RenderCopy(renderer, loginButtonTextTexture, NULL, &loginButtonTextRect);

    //Posiciona e faz uma copia do render do botao "Cadastrar"
    SDL_Rect registerButtonTextRect = { registerButtonBoxRect.x + (registerButtonBoxRect.w - registerButtonTextWidth) / 2, registerButtonBoxRect.y + (registerButtonBoxRect.h - registerButtonTextHeight) / 2, registerButtonTextWidth, registerButtonTextHeight };
    SDL_RenderCopy(renderer, registerButtonTextTexture, NULL, &registerButtonTextRect);

    //Renderiza os botoes de acordo com a copia acima
    SDL_SetRenderDrawColor(renderer, 52, 52, 54, 255);  // Black color
    SDL_RenderDrawRect(renderer, &loginButtonBoxRect);
    SDL_RenderDrawRect(renderer, &cancelButtonBoxRect);
    SDL_RenderDrawRect(renderer, &registerButtonBoxRect);
    //Atualiza o Render com todas as mudancas necessarias
    SDL_RenderPresent(renderer);
}

void centerLoginHUD(SDL_Rect* cinepucImageRect, SDL_Rect* viewImageRect, SDL_Rect* hideImageRect, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* registerButtonBoxRect, int windowWidth, int windowHeight) {
    //Calcula a nova posicao da caixa de login
    loginBoxRect->x = (windowWidth - loginBoxRect->w) / 2;
    loginBoxRect->y = (windowHeight - loginBoxRect->h - passwordBoxRect->h - 40) / 2;

    //Calcula a nova posicao da caixa de senha
    passwordBoxRect->x = (windowWidth - passwordBoxRect->w) / 2;
    passwordBoxRect->y = loginBoxRect->y + 27 + loginBoxRect->h + 10;

    //Calcula a nova posicao do botao "Cancelar"
    cancelButtonBoxRect->x = (windowWidth - loginBoxRect->w) / 2; 
    cancelButtonBoxRect->y = passwordBoxRect->y + passwordBoxRect->h + 27;
    cancelButtonBoxRect->w = 130;

    //Calcula a nova posicao do botao "Confirmar"
    loginButtonBoxRect->x = cancelButtonBoxRect->x + cancelButtonBoxRect->w + 40;
    loginButtonBoxRect->y = passwordBoxRect->y + passwordBoxRect->h + 27;

    //Calcula a nova posicao do botao "Cadastrar"
    registerButtonBoxRect->x = (windowWidth - registerButtonBoxRect->w) / 2;
    registerButtonBoxRect->y = loginButtonBoxRect->y + loginButtonBoxRect->h + 27;

    //Calcula a nova posicao da imagem Cinepuc
    cinepucImageRect->x = (windowWidth - cinepucImageRect->w) / 2;
    cinepucImageRect->y = loginBoxRect->y - 220;

    //Calcula a nova posicao da imagem do Olho (de acordo com a caixa de senha)
    viewImageRect->x = (passwordBoxRect->x + passwordBoxRect->w) - viewImageRect->w - 5;
    viewImageRect->y = (passwordBoxRect->y + (passwordBoxRect->h/2) - viewImageRect->h/2);

    //Calcula a nova posicao da imagem do Olho (de acordo com a caixa de senha)
    hideImageRect->x = (passwordBoxRect->x + passwordBoxRect->w) - hideImageRect->w - 5;
    hideImageRect->y = (passwordBoxRect->y + (passwordBoxRect->h/2) - hideImageRect->h/2);
}

//FUNCAO PARA RENDERIZAR A TELA DE REGISTRO
void renderRegisterScreen(SDL_Renderer* renderer, TTF_Font* font, SDL_Color textColor,
            SDL_Texture* cinepucImage, SDL_Rect cinepucImageRect, SDL_Texture* viewImage, SDL_Rect viewImageRect, SDL_Texture* hideImage, SDL_Rect hideImageRect, SDL_Rect confirmViewImageRect, SDL_Rect confirmHideImageRect,
            SDL_Rect loginBoxRect, SDL_Rect passwordBoxRect, SDL_Rect confirmPasswordBoxRect, SDL_Rect cancelButtonBoxRect, SDL_Rect loginButtonBoxRect,
            SDL_Texture* loginTextTexture, SDL_Texture* passwordTextTexture, SDL_Texture* confirmPasswordTextTexture, SDL_Texture* cancelButtonTextTexture, SDL_Texture* loginButtonTextTexture,
            SDL_Surface* loginInputTextSurface, SDL_Texture* loginInputTextTexture,
            SDL_Surface* passwordInputTextSurface, SDL_Texture* passwordInputTextTexture,
            SDL_Surface* confirmPasswordInputTextSurface, SDL_Texture* confirmPasswordInputTextTexture,
            char* passwordInputText, char* loginInputText, char* confirmPasswordInputText,
            int loginTextHeight, int loginTextWidth,
            int passwordTextHeight, int passwordTextWidth, char* passwordMask,
            int confirmPasswordTextHeight, int confirmPasswordTextWidth, char* confirmPasswordMask,
            int cancelButtonTextHeight, int cancelButtonTextWidth,
            int loginButtonTextHeight, int loginButtonTextWidth,
            Box* loginBoxColor, Box* passwordBoxColor, Box* confirmPasswordBoxColor) {
    //Limpa a tela
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color
    SDL_RenderClear(renderer);

    //Limpa os inputs de texto + cor das caixas + mostrar/esconder senha
    if(clearInput == CLEAR){
        strcpy(loginInputText, "");
        strcpy(passwordInputText, "");
        strcpy(passwordMask, "");
        strcpy(confirmPasswordInputText, "");
        strcpy(confirmPasswordMask, "");
        activeField = LOGIN_FIELD;
        showPassword = SHOW;
        showConfirmPassword = SHOW_CONFIRM;
        loginBoxColor->color = BLACK;
        passwordBoxColor->color = BLACK;
        confirmPasswordBoxColor->color = BLACK;
        clearInput = DONT_CLEAR;
    }

    //Renderiza a caixa de Login de acordo com a cor necessaria
    if(loginBoxColor->color == BLACK){
        SDL_SetRenderDrawColor(renderer, 52, 52, 54, 255);  // Preto
    }
    else if(loginBoxColor->color == RED){
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Vermelho
    }
    SDL_RenderDrawRect(renderer, &loginBoxRect);

    //Renderiza a imagem Cinepuc
    SDL_RenderCopy(renderer, cinepucImage, NULL, &cinepucImageRect);

    //Renderiza a imagem do olho
    if(showPassword == SHOW){
        SDL_RenderCopy(renderer, viewImage, NULL, &viewImageRect);
    }
    //Renderiza a imagem do outro olho
    else if(showPassword == HIDE){
        SDL_RenderCopy(renderer, hideImage, NULL, &hideImageRect);
    }

    //Renderiza a imagem do olho para segunda caixa de senha
    if(showConfirmPassword == SHOW_CONFIRM && currentScreen == REGISTER_SCREEN) {
        SDL_RenderCopy(renderer, viewImage, NULL, &confirmViewImageRect);
    }
    //Renderiza a imagem do outro olho para segunda caixa de senha
    else if(showConfirmPassword == HIDE_CONFIRM && currentScreen == REGISTER_SCREEN) {
        SDL_RenderCopy(renderer, hideImage, NULL, &confirmHideImageRect);
    }

    //Determina a posicao do texto "E-mail"
    SDL_Rect loginTextRect = { loginBoxRect.x, loginBoxRect.y - loginTextHeight, loginTextWidth, loginTextHeight };
    //Renderiza o texto "E-mail:" logo acima da caixa de input para o login/email
    SDL_RenderCopy(renderer, loginTextTexture, NULL, &loginTextRect);
    //Variaveis para determinar onde sera colocado o input text da caixa login
    int loginTextPosX = loginBoxRect.x + 10;
    int loginTextPosY = loginBoxRect.y + (loginBoxRect.h - loginTextHeight) / 2;

    //If necessario pois quando tentamos renderizar um texto vazio ocorre o erro: Segmentation fault (core dumped)
    if (strlen(loginInputText) > 0) {
        //Cria a surface para o input text da caixa login
        loginInputTextSurface = TTF_RenderText_Solid(font, loginInputText, textColor);
        if (!loginInputTextSurface) {
            SDL_Log("Falha ao criar surface: %s", TTF_GetError());
            return;
        }

        //Cria a texture para o input text da caixa login
        loginInputTextTexture = SDL_CreateTextureFromSurface(renderer, loginInputTextSurface);
        if (!loginInputTextTexture) {
            SDL_Log("Falha ao criar texture: %s", SDL_GetError());
            SDL_FreeSurface(loginInputTextSurface);
            loginInputTextSurface = NULL;
            return;
        }

        //Variaveis para determinar o tamanho do texto
        int loginInputTextWidth = loginInputTextSurface->w;
        int loginInputTextHeight = loginInputTextSurface->h;
        //Posiciona a caixa de acordo com as variaveis criadas acima do if
        SDL_Rect loginInputTextRect = { loginTextPosX, loginTextPosY, loginInputTextWidth, loginInputTextHeight };
        //Renderiza o texto de input da caixa de login
        SDL_RenderCopy(renderer, loginInputTextTexture, NULL, &loginInputTextRect);
    }

    //Renderiza a caixa de senha de acordo com a cor necessaria
    if(passwordBoxColor->color == BLACK){
        SDL_SetRenderDrawColor(renderer, 52, 52, 54, 255);  // Preto
    }
    else if(passwordBoxColor->color == RED){
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Vermelho
    }
    SDL_RenderDrawRect(renderer, &passwordBoxRect);
    //Renderiza a segunda caixa de senha de acordo com a cor necessaria
    if(confirmPasswordBoxColor->color == BLACK){
        SDL_SetRenderDrawColor(renderer, 52, 52, 54, 255);  // Preto
    }
    else if(confirmPasswordBoxColor->color == RED){
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Vermelho
    }
    SDL_RenderDrawRect(renderer, &confirmPasswordBoxRect);

    //Determina a posicao do texto "Senha:"
    SDL_Rect passwordTextRect = { passwordBoxRect.x, passwordBoxRect.y - passwordTextHeight, passwordTextWidth, passwordTextHeight };
    //Renderiza o texto "Senha:"
    SDL_RenderCopy(renderer, passwordTextTexture, NULL, &passwordTextRect);
    //Variaveis para determinar onde sera colocado o input text da caixa senha
    int passwordTextPosX = passwordBoxRect.x + 10;
    int passwordTextPosY = passwordBoxRect.y + (passwordBoxRect.h - passwordTextHeight) / 2;

    //Determina a posicao do texto "Confirmar senha:"
    SDL_Rect confirmPasswordTextRect = { confirmPasswordBoxRect.x, confirmPasswordBoxRect.y - confirmPasswordTextHeight, confirmPasswordTextWidth, confirmPasswordTextHeight };
    //Renderiza o texto "Confirmar senha:"
    SDL_RenderCopy(renderer, confirmPasswordTextTexture, NULL, &confirmPasswordTextRect);
    //Variaveis para determinar onde sera colocado o input text da segunda caixa de senha (confirma, senha)
    int confirmPasswordTextPosX = confirmPasswordBoxRect.x + 10;
    int confirmPasswordTextPosY = confirmPasswordBoxRect.y + (confirmPasswordBoxRect.h - confirmPasswordTextHeight) / 2;

    //If necessario pois quando tentamos renderizar um texto vazio ocorre o erro: Segmentation fault (core dumped)
    if (strlen(passwordInputText) > 0) {
        //Cria a surface para o input text da caixa senha, baseado na opcao do usuario de querer mostrar ou nao a senha
        if(showPassword == SHOW)
            //se nao quiser mostrar, cria a surface dos asteriscos
            passwordInputTextSurface = TTF_RenderText_Solid(font, passwordMask, textColor);
        else if (showPassword == HIDE)
            //se quiser mostrar, cria a surface da propria senha
            passwordInputTextSurface = TTF_RenderText_Solid(font, passwordInputText, textColor);

        if (!passwordInputTextSurface) {
            SDL_Log("Falha ao criar surface: %s", TTF_GetError());
            return;
        }

        //Cria a texture para o input text da caixa senha, baseado na escolha de Surface acima
        passwordInputTextTexture = SDL_CreateTextureFromSurface(renderer, passwordInputTextSurface);
        if (!passwordInputTextTexture) {
            SDL_Log("Falha ao criar texture: %s", SDL_GetError());
            SDL_FreeSurface(passwordInputTextSurface);
            passwordInputTextSurface = NULL;
            return;
        }

        //Variaveis para determinar o tamanho do texto
        int passwordInputTextWidth = passwordInputTextSurface->w;
        int passwordInputTextHeight = passwordInputTextSurface->h;
        //Posiciona a caixa de acordo com o texto "Senha:" (no caso logo abaixo)
        SDL_Rect passwordInputTextRect = { passwordTextPosX, passwordTextPosY, passwordInputTextWidth, passwordInputTextHeight };
        //Renderiza o texto de input da caixa de senha
        SDL_RenderCopy(renderer, passwordInputTextTexture, NULL, &passwordInputTextRect);
    }

    //If necessario pois quando tentamos renderizar um texto vazio ocorre o erro: Segmentation fault (core dumped)
    if (strlen(confirmPasswordInputText) > 0) {
        //Cria a surface para o input text da segunda caixa de senha (confirma senha), baseado na opcao do usuario de querer mostrar ou nao a senha
        if(showConfirmPassword == SHOW_CONFIRM){
            //se nao quiser mostrar, cria a surface dos asteriscos
            confirmPasswordInputTextSurface = TTF_RenderText_Solid(font, confirmPasswordMask, textColor);
        }
        else if (showConfirmPassword == HIDE_CONFIRM){
            //se quiser mostrar, cria a surface da propria senha
            confirmPasswordInputTextSurface = TTF_RenderText_Solid(font, confirmPasswordInputText, textColor);
        }
        if (!confirmPasswordInputTextSurface) {
            SDL_Log("Falha ao criar surface: %s", TTF_GetError());
            return;
        }


        //Cria a texture para o input text da segunda caixa de senha, baseado na escolha de Surface acima
        confirmPasswordInputTextTexture = SDL_CreateTextureFromSurface(renderer, confirmPasswordInputTextSurface);
        if (!confirmPasswordInputTextTexture) {
            SDL_Log("Falha ao criar texture: %s", SDL_GetError());
            SDL_FreeSurface(confirmPasswordInputTextSurface);
            confirmPasswordInputTextSurface = NULL;
            return;
        }

        //Variaveis para determinar o tamanho do texto
        int confirmPasswordInputTextWidth = confirmPasswordInputTextSurface->w;
        int confirmPasswordInputTextHeight = confirmPasswordInputTextSurface->h;
        //Posiciona a caixa de acordo com o texto "Confirmar senha:" (no caso logo abaixo)
        SDL_Rect confirmPasswordInputTextRect = { confirmPasswordTextPosX, confirmPasswordTextPosY, confirmPasswordInputTextWidth, confirmPasswordInputTextHeight };
        //Renderiza o texto de input da caixa de senha
        SDL_RenderCopy(renderer, confirmPasswordInputTextTexture, NULL, &confirmPasswordInputTextRect);
    }

    //Posiciona e faz uma copia do render do botao "Cancelar"
    SDL_Rect cancelButtonTextRect = { cancelButtonBoxRect.x + (cancelButtonBoxRect.w - cancelButtonTextWidth) / 2, cancelButtonBoxRect.y + (cancelButtonBoxRect.h - cancelButtonTextHeight) / 2, cancelButtonTextWidth, cancelButtonTextHeight };
    SDL_RenderCopy(renderer, cancelButtonTextTexture, NULL, &cancelButtonTextRect);

    //Posiciona e faz uma copia do render do botao "Confirmar"
    SDL_Rect loginButtonTextRect = { loginButtonBoxRect.x + (loginButtonBoxRect.w - loginButtonTextWidth) / 2, loginButtonBoxRect.y + (loginButtonBoxRect.h - loginButtonTextHeight) / 2, loginButtonTextWidth, loginButtonTextHeight };
    SDL_RenderCopy(renderer, loginButtonTextTexture, NULL, &loginButtonTextRect);

    //Renderiza os botoes de acordo com a copia acima
    SDL_SetRenderDrawColor(renderer, 52, 52, 54, 255);  // Black color
    SDL_RenderDrawRect(renderer, &loginButtonBoxRect);
    SDL_RenderDrawRect(renderer, &cancelButtonBoxRect);
    //Atualiza o Render com todas as mudancas necessarias
    SDL_RenderPresent(renderer);
}

void centerRegisterHUD(SDL_Rect* cinepucImageRect, SDL_Rect* viewImageRect, SDL_Rect* hideImageRect, SDL_Rect* confirmViewImageRect, SDL_Rect* confirmHideImageRect, SDL_Rect* loginBoxRect, SDL_Rect* passwordBoxRect, SDL_Rect* confirmPasswordBoxRect, SDL_Rect* loginButtonBoxRect, SDL_Rect* cancelButtonBoxRect, int windowWidth, int windowHeight) {
    //Calcula a nova posicao da caixa de login
    loginBoxRect->x = (windowWidth - loginBoxRect->w) / 2;
    loginBoxRect->y = (windowHeight - loginBoxRect->h - passwordBoxRect->h - 40) / 2;

    //Calcula a nova posicao da caixa de senha
    passwordBoxRect->x = (windowWidth - passwordBoxRect->w) / 2;
    passwordBoxRect->y = loginBoxRect->y + 27 + loginBoxRect->h + 10;

    //Calcula a nova posicao da segunda caixa de senha (confirma senha)
    confirmPasswordBoxRect->x = (windowWidth - confirmPasswordBoxRect->w) / 2;;
    confirmPasswordBoxRect->y = passwordBoxRect->y + passwordBoxRect->h + 37;

    //Calcula a nova posicao do botao "Cancelar"
    cancelButtonBoxRect->x = (windowWidth - loginBoxRect->w) / 2; 
    cancelButtonBoxRect->y = confirmPasswordBoxRect->y + confirmPasswordBoxRect->h + 27;

    //Calcula a nova posicao do botao "Confirmar"
    loginButtonBoxRect->x = cancelButtonBoxRect->x + cancelButtonBoxRect->w + 40;
    loginButtonBoxRect->y = confirmPasswordBoxRect->y + confirmPasswordBoxRect->h + 27;

    //Calcula a nova posicao da imagem Cinepuc
    cinepucImageRect->x = (windowWidth - cinepucImageRect->w) / 2;
    cinepucImageRect->y = loginBoxRect->y - 220;

    //Calcula a nova posicao da imagem do Olho (de acordo com a caixa de senha)
    viewImageRect->x = (passwordBoxRect->x + passwordBoxRect->w) - viewImageRect->w - 5;
    viewImageRect->y = (passwordBoxRect->y + (passwordBoxRect->h/2) - viewImageRect->h/2);

    //Calcula a nova posicao da imagem do Olho (de acordo com a caixa de senha)
    hideImageRect->x = (passwordBoxRect->x + passwordBoxRect->w) - hideImageRect->w - 5;
    hideImageRect->y = (passwordBoxRect->y + (passwordBoxRect->h/2) - hideImageRect->h/2);

    //Calcula a nova posicao da imagem do Olho (de acordo com a segunda caixa de senha (confirma senha))
    confirmViewImageRect->x = (confirmPasswordBoxRect->x + confirmPasswordBoxRect->w) - confirmViewImageRect->w - 5;
    confirmViewImageRect->y = (confirmPasswordBoxRect->y + (confirmPasswordBoxRect->h/2) - confirmViewImageRect->h/2);

    //Calcula a nova posicao da imagem do Olho (de acordo com a segunda caixa de senha (confirma senha))
    confirmHideImageRect->x = (confirmPasswordBoxRect->x + confirmPasswordBoxRect->w) - confirmHideImageRect->w - 5;
    confirmHideImageRect->y = (confirmPasswordBoxRect->y + (confirmPasswordBoxRect->h/2) - confirmHideImageRect->h/2);
}

//FUNCAO PARA RENDERIZAR A TELA DE SELECIONAR FILMES
void renderMovieScreen(SDL_Renderer* renderer, TTF_Font* font, char* selectMovieText, int selectMovieWidth, int selectMovieHeight, SDL_Rect* selectMovieRect,
                       int maxTextWidth, int offsetX, int offsetY, SDL_Rect movieRect[][COL], SDL_Rect textRect[][COL], SDL_Surface* selectMovieTextSurface, SDL_Texture* selectMovieTextTexture, int windowWidth, int windowHeight, int gridWidth, int gridHeight) {
    //Limpa a tela
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
    SDL_RenderClear(renderer);

    //Seta o tamanho da fonte, altura e largura do texto "Selecione o filme que deseja assistir"
    TTF_SizeText(font, selectMovieText, &selectMovieWidth, &selectMovieHeight);
    selectMovieRect->w = selectMovieWidth;
    selectMovieRect->h = 30;
    SDL_Color textColor = {0, 0, 0}; // Seta a cor para Preto

    //Cria surface para o texto
    selectMovieTextSurface = TTF_RenderText_Solid(font, selectMovieText, textColor);
    //Cria texture de acordo com a surface acima
    selectMovieTextTexture = SDL_CreateTextureFromSurface(renderer, selectMovieTextSurface);

    //Renderiza o texto "Selecione o filme que deseja assistir"
    SDL_RenderCopy(renderer, selectMovieTextTexture, NULL, selectMovieRect);

    SDL_DestroyTexture(selectMovieTextTexture);
    SDL_FreeSurface(selectMovieTextSurface);

    //Centralizar a interface
    centerMovieHUD(windowWidth, windowHeight, gridWidth, gridHeight, maxTextWidth, selectMovieRect, movieRect, textRect, renderer, font);
}

void centerMovieHUD(int windowWidth, int windowHeight, int gridWidth, int gridHeight, int maxTextWidth, SDL_Rect* selectMovieRect, SDL_Rect movieRect[][COL], SDL_Rect textRect[][COL], SDL_Renderer* renderer, TTF_Font* font) {
    //variaveis para centralizar a interface
    int offsetX = (windowWidth - gridWidth) / 2;
    int offsetY = (windowHeight - gridHeight) / 2;

    //posiciona o texto "Selecione o filme que deseja assistir"
    selectMovieRect->x = (windowWidth - selectMovieRect->w) / 2;
    selectMovieRect->y = offsetY - selectMovieRect->h - 10;

    //for para posicionar os filmes e seus nomes
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COL; col++)
        {
            //variavel para determinar a coordenada horizontal
            int x;
            //if e else para aumentar/manter o espacamento horizontal da interface de acordo com o tamanho do texto
            if (maxTextWidth > MOVIE_WIDTH)
            {
                x = offsetX + col * (MOVIE_WIDTH + PADDING + (341 - MOVIE_WIDTH));
            }
            else
            {
                x = offsetX + col * (MOVIE_WIDTH + PADDING);
            }
            //variavel para determinal a coordenada vertical
            int y = offsetY + row * (MOVIE_HEIGHT + PADDING + 30);
            
            //posiciona a imagem de acordo com as variaveis acima
            movieRect[row][col].x = x;
            movieRect[row][col].y = y;
            //posiciona o nome dos filmes de acordo com a posicao dos filmes
            textRect[row][col].x = x + (MOVIE_WIDTH - textRect[row][col].w) / 2; // centralizar horizontalmente com o filme
            textRect[row][col].y = y + MOVIE_HEIGHT;

            //renderiza as imagens
            SDL_RenderCopy(renderer, movies[row][col].image, NULL, &movieRect[row][col]);

            SDL_Color textColor = {0, 0, 0}; // Texto Preto
            //cria surface para o nome do filme
            SDL_Surface *textSurface = TTF_RenderText_Solid(font, movies[row][col].name, textColor);
            //cria texture de acordo com a surface acima
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            //renderiza o nome do filme
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect[row][col]);

            SDL_DestroyTexture(textTexture);
            SDL_FreeSurface(textSurface);
        }
    }
    //Renderiza a interface necessaria
    SDL_RenderPresent(renderer);
}

void renderTimeSelectionScreen(SDL_Renderer* renderer, SDL_Texture* selectTimeTextTexture, int selectTimeTextWidth, int selectTimeTextHeight,
                               SDL_Rect* firstTimeButtonBoxRect, SDL_Texture* firstTimeButtonTextTexture, int firstTimeButtonTextWidth, int firstTimeButtonTextHeight,
                               SDL_Rect* secondTimeButtonBoxRect, SDL_Texture* secondTimeButtonTextTexture, int secondTimeButtonTextWidth, int secondTimeButtonTextHeight,
                               SDL_Rect* thirdTimeButtonBoxRect, SDL_Texture* thirdTimeButtonTextTexture, int thirdTimeButtonTextWidth, int thirdTimeButtonTextHeight,
                               SDL_Rect* fourthTimeButtonBoxRect, SDL_Texture* fourthTimeButtonTextTexture, int fourthTimeButtonTextWidth, int fourthTimeButtonTextHeight,
                               SDL_Rect selectTimeTextRect,
                               SDL_Rect* cancelButtonBoxRect, SDL_Texture* cancelButtonTextTexture,
                               SDL_Texture* cinepucImage, SDL_Rect cinepucImageRect,
                               int cancelButtonTextWidth, int cancelButtonTextHeight) {
    //Limpa a tela
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Branco
    SDL_RenderClear(renderer);

    //Posiciona e cria uma copia do Render do primeiro botao (primeiro horario)
    SDL_Rect firstTimeButtonTextRect = { firstTimeButtonBoxRect->x + (firstTimeButtonBoxRect->w - firstTimeButtonTextWidth) / 2, firstTimeButtonBoxRect->y + (firstTimeButtonBoxRect->h - firstTimeButtonTextHeight) / 2, firstTimeButtonTextWidth, firstTimeButtonTextHeight };
    SDL_RenderCopy(renderer, firstTimeButtonTextTexture, NULL, &firstTimeButtonTextRect);

    //Posiciona e cria uma copia do Render do segundo botao (segundo horario)
    SDL_Rect secondTimeButtonTextRect = { secondTimeButtonBoxRect->x + (secondTimeButtonBoxRect->w - secondTimeButtonTextWidth) / 2, secondTimeButtonBoxRect->y + (secondTimeButtonBoxRect->h - secondTimeButtonTextHeight) / 2, secondTimeButtonTextWidth, secondTimeButtonTextHeight };
    SDL_RenderCopy(renderer, secondTimeButtonTextTexture, NULL, &secondTimeButtonTextRect);

    //Posiciona e cria uma copia do Render do terceiro botao (terceiro horario)
    SDL_Rect thirdTimeButtonTextRect = { thirdTimeButtonBoxRect->x + (thirdTimeButtonBoxRect->w - thirdTimeButtonTextWidth) / 2, thirdTimeButtonBoxRect->y + (thirdTimeButtonBoxRect->h - thirdTimeButtonTextHeight) / 2, thirdTimeButtonTextWidth, thirdTimeButtonTextHeight };
    SDL_RenderCopy(renderer, thirdTimeButtonTextTexture, NULL, &thirdTimeButtonTextRect);

    //Posiciona e cria uma copia do Render do quarto botao (quarto horario)
    SDL_Rect fourthTimeButtonTextRect = { fourthTimeButtonBoxRect->x + (fourthTimeButtonBoxRect->w - fourthTimeButtonTextWidth) / 2, fourthTimeButtonBoxRect->y + (fourthTimeButtonBoxRect->h - fourthTimeButtonTextHeight) / 2, fourthTimeButtonTextWidth, fourthTimeButtonTextHeight };
    SDL_RenderCopy(renderer, fourthTimeButtonTextTexture, NULL, &fourthTimeButtonTextRect);

    //Posiciona e cria uma copia do Render do botao "Cancelar"
    SDL_Rect cancelButtonTextRect = { cancelButtonBoxRect->x + (cancelButtonBoxRect->w - cancelButtonTextWidth) / 2, cancelButtonBoxRect->y + (cancelButtonBoxRect->h - cancelButtonTextHeight) / 2, cancelButtonTextWidth, cancelButtonTextHeight };
    SDL_RenderCopy(renderer, cancelButtonTextTexture, NULL, &cancelButtonTextRect);

    //Renderiza a imagem Cinepuc
    SDL_RenderCopy(renderer, cinepucImage, NULL, &cinepucImageRect);

    //Renderiza os botoes de acordo com as copias acima
    SDL_SetRenderDrawColor(renderer, 52, 52, 54, 255);  // Preto
    SDL_RenderDrawRect(renderer, firstTimeButtonBoxRect);
    SDL_RenderDrawRect(renderer, secondTimeButtonBoxRect);
    SDL_RenderDrawRect(renderer, thirdTimeButtonBoxRect);
    SDL_RenderDrawRect(renderer, fourthTimeButtonBoxRect);
    SDL_RenderDrawRect(renderer, cancelButtonBoxRect);

    //Renderiza o texto "Selecione o horario desejado"
    SDL_RenderCopy(renderer, selectTimeTextTexture, NULL, &selectTimeTextRect);

    //Renderiza toda a interface
    SDL_RenderPresent(renderer);
}

void centerTimeSelectionHUD(SDL_Rect* selectTimeTextRect, SDL_Rect* firstTimeButtonBoxRect, SDL_Rect* secondTimeButtonBoxRect, SDL_Rect* thirdTimeButtonBoxRect, SDL_Rect* fourthTimeButtonBoxRect, SDL_Rect* cancelButtonBoxRect, SDL_Rect* cinepucImageRect, int windowWidth, int windowHeight) {
    //Calcula a posicao do primeiro botao
    firstTimeButtonBoxRect->x = (windowWidth - firstTimeButtonBoxRect->w) / 2;
    firstTimeButtonBoxRect->y = (windowHeight - (4*firstTimeButtonBoxRect->h)) / 2;
    
    //Calcula a posicao do segundo botao
    secondTimeButtonBoxRect->x = (windowWidth - secondTimeButtonBoxRect->w) / 2;
    secondTimeButtonBoxRect->y = firstTimeButtonBoxRect->y + secondTimeButtonBoxRect->h + 10;

    //Calcula a posicao do terceiro botao
    thirdTimeButtonBoxRect->x = (windowWidth - thirdTimeButtonBoxRect->w) / 2;
    thirdTimeButtonBoxRect->y = secondTimeButtonBoxRect->y + thirdTimeButtonBoxRect->h + 10;

    //Calcula a posicao do quarto botao
    fourthTimeButtonBoxRect->x = (windowWidth - fourthTimeButtonBoxRect->w) / 2;
    fourthTimeButtonBoxRect->y = thirdTimeButtonBoxRect->y + fourthTimeButtonBoxRect->h + 10;

    //Calcula a posicao do botao "Cancelar"
    cancelButtonBoxRect->w = 300;
    cancelButtonBoxRect->x = (windowWidth - cancelButtonBoxRect->w) / 2;
    cancelButtonBoxRect->y = fourthTimeButtonBoxRect->y + cancelButtonBoxRect->h + 10;

    //Calcula a posicao do texto "Selecione o horario desejado"
    selectTimeTextRect->x = (windowWidth - selectTimeTextRect->w) / 2;
    selectTimeTextRect->y = firstTimeButtonBoxRect->y - selectTimeTextRect->h - 10;

    //Calcula a posicao da imagem Cinepuc
    cinepucImageRect->x = (windowWidth - cinepucImageRect->w) / 2;
    cinepucImageRect->y = selectTimeTextRect->y - 220;
}

//NCURSES
//TELA PARA SELECIONAR O HORARIO
WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx); //criar janela
    box(local_win, 0, 0); // colocar borda na janela
    wrefresh(local_win);  // atualizar a janela

    return local_win;
}

void initColors(){
    if (has_colors() == FALSE)
    {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();
    init_color(COLOR_WHITE, 1000, 1000, 1000);
    init_color(COLOR_GRAY, 250, 250, 250);
    init_pair(1, COLOR_BLACK, COLOR_WHITE); //Escrita Preta e Fundo Branco
    init_pair(2, COLOR_WHITE, COLOR_GREEN); //Escrita Branca e Fundo Verde
    init_pair(3, COLOR_WHITE, COLOR_RED);   //Escrita Branca e Fundo Vermelho
    init_pair(4, COLOR_WHITE, COLOR_GRAY);   //Escrita Branca e Fundo Cinza
    init_pair(5, COLOR_WHITE, COLOR_BLUE); //Escrita Branca e Fundo Azul
    wbkgd(stdscr, COLOR_PAIR(1)); //stdscr = Fundo do Terminal
    refresh();
}

void drawChairMenu(){
  int boxX, boxY;
  int counter = 0; //determina quantas cadeiras foram adicionadas
  

  for (int i = 0; i < CHAIR_LINE_AMOUNT + 1; i++)
  {
      for (int j = 0; j < CHAIR_COL_AMOUNT + 1; j++)
      {
          int boxX = STARTX + (WIDTH + GRID_SPACING) * j;  //determina onde será desenhado a cadeira/numero/letra (horizontal)
          int boxY = STARTY + (HEIGHT + GRID_SPACING) * i; //determina onde será desenhado a cadeira/numero/letra (vertical)
          //ifs necessarios para saber quando desenha letra/numero ou cadeira
          if(i == 0 && j == 0){  
          }
          else if(i==0 && j!=0){
            attron(A_BOLD); //Negrito
            mvprintw(boxY, boxX+1, "%c", 64+j); //Letra
            attroff(A_BOLD);
          }
          else if(i!=0 && j==0){
            attron(A_BOLD);
            mvprintw(boxY,boxX,"%d", i); //Numero
            attroff(A_BOLD);
          }
          else{
            my_win[counter] = create_newwin(HEIGHT, WIDTH, boxY, boxX); //criacao da janela (cadeira)
            if(status[counter] == 0){
                wbkgd(my_win[counter], COLOR_PAIR(2)); //mudar cor da janela
                wrefresh(my_win[counter]);  // atualizar a janela
            }
            else if(status[counter] == 1){
                wbkgd(my_win[counter], COLOR_PAIR(4)); //mudar cor da janela
                wrefresh(my_win[counter]);  // atualizar a janela
            }
            counter++;
          }
      }
  }
}

//desenha TELA
void drawTELA(){
  WINDOW *TELA;
  int boxX, boxY;

  boxX = STARTX + (WIDTH + GRID_SPACING) * 1;  //boxX = primeira coluna da interface (1)
  boxY = STARTY + (HEIGHT + GRID_SPACING) * 9; //boxY = ultima linha da interface (9)
  TELA = newwin(1, (TOTAL_WIDTH - WIDTH - GRID_SPACING), boxY, boxX);
  mvwprintw(TELA, 0,13,"TELA");
  refresh();
  wbkgd(TELA, COLOR_PAIR(4) | A_BOLD);
  wrefresh(TELA);
}

//desenha botao "Cancelar"
WINDOW* drawCANCEL(){
  WINDOW *CANCEL;
  int boxX, boxY;

  boxX = STARTX + (WIDTH + GRID_SPACING) * 1;  //boxX = primeira coluna da interface (1)
  boxY = STARTY + (HEIGHT + GRID_SPACING) * 10; //boxY = ultima linha da interface (9)
  CANCEL = newwin(1, (TOTAL_WIDTH - WIDTH - GRID_SPACING) / 2, boxY, boxX);
  mvwprintw(CANCEL, 0,4,"Cancelar");
  refresh();
  wbkgd(CANCEL, COLOR_PAIR(4) | A_BOLD);
  wrefresh(CANCEL);
  return CANCEL;
}

//desenha botao "Confirmar"
WINDOW* drawCONFIRM(){
  WINDOW *CONFIRM;
  int boxX, boxY;

  boxX = STARTX + (WIDTH + GRID_SPACING) * 1 + ((TOTAL_WIDTH - WIDTH - GRID_SPACING) / 2) + 1;  //boxX = primeira coluna da interface (1)
  boxY = STARTY + (HEIGHT + GRID_SPACING) * 10; //boxY = ultima linha da interface (9)
  CONFIRM = newwin(1, (TOTAL_WIDTH - WIDTH - GRID_SPACING) / 2, boxY, boxX);
  mvwprintw(CONFIRM, 0,3,"Confirmar");
  refresh();
  wbkgd(CONFIRM, COLOR_PAIR(4) | A_BOLD);
  wrefresh(CONFIRM);
  return CONFIRM;
}

//funcao para detectar evento de mouse
void mouseFunc(WINDOW* CANCEL, WINDOW* CONFIRM, dadosFrontEnd* dados, Carrinho* carrinho){
    int windowCoordX, windowCoordY;
    MEVENT event;
    nodelay(stdscr, TRUE);
    while (1) {
        int ch = getch();
        if (ch == KEY_MOUSE) {
            //if para determinar se o botao esquerdo foi clicado
            if (getmouse(&event) == OK && event.bstate & BUTTON1_CLICKED) {
                //for para verificar todas as cadeiras
                for (int i = 0; i < CHAIR_AMOUNT; i++){
                    //get coordenada da janela i
                    getbegyx(my_win[i], windowCoordY, windowCoordX);
                    //if que detecta se a coordenada do  clique é igual a coordenada da cadeira
                    if(event.x >= windowCoordX && event.x <= windowCoordX+2 && event.y == windowCoordY){
                        if(status[i] == 0){
                            wbkgd(my_win[i], COLOR_PAIR(5)); //mudar cadeira para azul (selecionado)
                            wrefresh(my_win[i]);
                            status[i] = 2;
                            insere(carrinho, dados->idFilme, dados->idSessao, i, dados->usuarioLogado);
                            //mvwprintw(stdscr, 0, 0, "Antes %d", cadeira[i].status);
                        }
                        else if(status[i] == 1){
                        }
                        else if(status[i] == 2){
                            wbkgd(my_win[i], COLOR_PAIR(2)); //mudar cadeira para verde (disponivel)
                            wrefresh(my_win[i]);
                            retira(carrinho, i);
                            status[i] = 0;
                        }

                        //mvwprintw(stdscr, 0, 0, "%s %d %d", dados->usuarioLogado, dados->idFilme, dados->idSessao);
                        //mvwprintw(stdscr, 1, 0, "Depois %d", cadeira[i].status);
                    }
                }
                //get coordenada do botao cancelar
                getbegyx(CANCEL, windowCoordY, windowCoordX);
                //if que detecta se a coordenada do  clique é igual a coordenada do botao "Cancelar"
                if(event.x >= windowCoordX && event.x <= windowCoordX + 14 && event.y == windowCoordY){
                    //se sim, volta para a tela de horarios
                    currentScreen = TIME_SELECTION_SCREEN;
                    break;
                }
                //get coordenada do botao confirmar
                getbegyx(CONFIRM, windowCoordY, windowCoordX);
                //if que detecta se a coordenada do  clique é igual a coordenada do botao "Confirmar"
                if(event.x >= windowCoordX && event.x <= windowCoordX + 14 && event.y == windowCoordY){
                    //se sim, vai para a tela de login
                    adicionarArquivo(carrinho);
                    currentScreen = LOGIN_SCREEN;
                    clearInput = CLEAR;
                    break;
                }
            }
        }
    }
}

//FUNCAO PARA LIMPAR TODOS OS RECURSOS DAS BIBLIOTECAS SDL, SDL_TTF E SDL_IMAGE
void cleanUp(SDL_Texture* cinepucImage, SDL_Texture* viewImage, SDL_Texture* hideImage, SDL_Texture* loginTextTexture, SDL_Texture* passwordTextTexture, SDL_Texture* loginButtonTextTexture, SDL_Texture* cancelButtonTextTexture, SDL_Texture* registerButtonTextTexture, SDL_Texture* loginInputTextTexture, SDL_Texture* passwordInputTextTexture, SDL_Surface* loginTextSurface, SDL_Surface* passwordTextSurface, SDL_Surface* loginButtonTextSurface, SDL_Surface* cancelButtonTextSurface, SDL_Surface* registerButtonTextSurface, SDL_Surface* loginInputTextSurface, SDL_Surface* passwordInputTextSurface, SDL_Surface* confirmPasswordTextSurface, SDL_Texture* confirmPasswordTextTexture, SDL_Surface* confirmPasswordInputTextSurface, SDL_Texture* confirmPasswordInputTextTexture, SDL_Texture *selectMovieTextTexture, SDL_Surface *selectMovieTextSurface, SDL_Surface* selectTimeTextSurface, SDL_Surface* firstTimeButtonTextSurface, SDL_Surface* secondTimeButtonTextSurface, SDL_Surface* thirdTimeButtonTextSurface, SDL_Surface* fourthTimeButtonTextSurface, SDL_Texture* selectTimeTextTexture, SDL_Texture* firstTimeButtonTextTexture, SDL_Texture* secondTimeButtonTextTexture, SDL_Texture* thirdTimeButtonTextTexture, SDL_Texture* fourthTimeButtonTextTexture, SDL_Window* window, TTF_Font* font, SDL_Renderer* renderer) {
    /*
        DestroyTexture: finalizar as texturas criadas
        FreeSurface: finalizar as surfaces criadas
        CloseFont: finalizar a fonte de texto
        DestroyRenderer: finalizar o renderizador/render
        DestroyWindow: finalizar a janela
    */
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
    SDL_DestroyTexture(movies[0][0].image);
    SDL_DestroyTexture(movies[0][1].image);
    SDL_DestroyTexture(movies[1][0].image);
    SDL_DestroyTexture(movies[1][1].image);
    SDL_DestroyTexture(selectMovieTextTexture);
    SDL_FreeSurface(selectMovieTextSurface);
    SDL_FreeSurface(selectTimeTextSurface);
    SDL_FreeSurface(firstTimeButtonTextSurface);
    SDL_FreeSurface(secondTimeButtonTextSurface);
    SDL_FreeSurface(thirdTimeButtonTextSurface);
    SDL_FreeSurface(fourthTimeButtonTextSurface);
    SDL_DestroyTexture(selectTimeTextTexture);
    SDL_DestroyTexture(firstTimeButtonTextTexture);
    SDL_DestroyTexture(secondTimeButtonTextTexture);
    SDL_DestroyTexture(thirdTimeButtonTextTexture);
    SDL_DestroyTexture(fourthTimeButtonTextTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}