//Compilar: gcc main.c -o main -lncurses
//          ./main
#include <curses.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT 1            //altura da cadeira
#define WIDTH 3             //largura da cadeira 
#define TOTAL_WIDTH ((WIDTH + GRID_SPACING) * (CHAIR_COL_AMOUNT + 1) - GRID_SPACING)    //largura de toda a interface somada
#define TOTAL_HEIGHT ((HEIGHT + GRID_SPACING) * (CHAIR_LINE_AMOUNT + 1) - GRID_SPACING) //altura de toda a interface somada
#define GRID_SPACING 1      //espaço entre as cadeiras
#define CHAIR_LINE_AMOUNT 8 //numero de cadeiras por linha
#define CHAIR_COL_AMOUNT 8  //numero de cadeiras por coluna
#define CHAIR_AMOUNT (CHAIR_LINE_AMOUNT * CHAIR_COL_AMOUNT) // total de cadeiras
#define STARTX ((COLS - TOTAL_WIDTH)/ 2)    //formula para centralizar a interface horizontalmente
#define STARTY ((LINES - TOTAL_HEIGHT) /2)  //formula para centralizar a interface verticalmente

//Variaveis Globais
WINDOW *my_win[CHAIR_AMOUNT]; //Vetor de Janelas com tamanho de [número de cadeiras].

//Funcoes
WINDOW *create_newwin(int height, int width, int starty, int startx); //função para criar uma janela
void initColors(); //função para iniciar as cores usadas no menu
void drawChairMenu();   //função para desenhar o menu (cadeiras + letras e numeros)
void mouseFunc();  //função para detectar se o mouse clicou na respectiva cadeira

int main()
{
  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  mousemask(ALL_MOUSE_EVENTS, NULL); 
  curs_set(0);
  
  initColors();
  drawChairMenu();
  mouseFunc();
  
  getch();
  curs_set(1);
  endwin();
  return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx); //criar janela
    wbkgd(local_win, COLOR_PAIR(2)); //mudar cor da janela
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
    init_pair(1, COLOR_BLACK, COLOR_WHITE); //Escrita Preta e Fundo Branco
    init_pair(2, COLOR_WHITE, COLOR_GREEN); //Escrita Branca e Fundo Vermelho
    init_pair(3, COLOR_WHITE, COLOR_RED);   //Escrita Branca e Fundo Verde
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
            counter++;
          }
      }
  }
} 

void mouseFunc(){
  int windowCoordX, windowCoordY;
  MEVENT event;
    while (1) {
        int ch = getch();
        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK && event.bstate & BUTTON1_CLICKED) {
                //for para verificar todas as cadeiras
                for (int i = 0; i < CHAIR_AMOUNT; i++){
                  getbegyx(my_win[i], windowCoordY, windowCoordX); //get coordenada da janela i
                  //if que detecta se a coordenada do  clique é igual a coordenada da cadeira
                  if(event.x >= windowCoordX && event.x <= windowCoordX+2 && event.y == windowCoordY){
                    wbkgd(my_win[i], COLOR_PAIR(3)); //mudar cadeira para vermelho
                    wrefresh(my_win[i]);
                  }
                }              
                //break;
            }
        }
    }
}
