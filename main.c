
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>

#include <time.h>
#include <errno.h>    

void hideCursor(){
    printf("\e[?25l");
}

/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

#define ROWS 10 // 2-9
#define COLS 30 // 2-29
#define WALL '#'

typedef struct{
    int X;
    int Y;
}Snake;

typedef struct{
    int X;
    int Y;
}Food;


#define clrscr() printf("\e[1;1H\e[2J")
void gotoxy(int x,int y){
    printf("%c[%d;%df",0x1B,y,x);
}


void placeFood(Food *food, Snake snake){
    do{
        food->X = rand() % COLS + 2;
        food->Y = rand() % ROWS + 2;
    }while(food->X == snake.X && food->Y == snake.Y);
}

void drawBoundaries(){
    for(int row = 0; row < ROWS+2;row++){
        for(int col = 0; col < COLS+2; col++){
            if(row == 0 || row == ROWS+1 || col == 0 || col == COLS+1){
                printf("%c",WALL);         
            }
            else{
                printf(" ");
            }
        }
        printf("\n");
        
    }
}


void drawSnake(Snake snake){
    gotoxy(snake.X,snake.Y);
    printf("O");
}

void drawFood(Food food){
    gotoxy(food.X,food.Y);
    printf("*");
}

void clearSnake(Snake snake){
    gotoxy(snake.X,snake.Y);
    printf(" ");
}


typedef enum {
    KeyboardDir_Left,    
    KeyboardDir_Right,
    KeyboardDir_Up,
    KeyboardDir_Down,
}KeyboardDir;


int kbhit2()
{
    return _kbhit();
}

int getNextKeyboardAction(){
    if(kbhit2()){
        char ch = getch();
        if(ch == 224 || ch == 0){
            ch = getch();
            if(ch == 72) return 'u';
            if(ch == 80) return 'd';
            if(ch == 75) return 'l';
            if(ch == 77) return 'r';
        }
        return ch;
    }
    return 0;
}



void moveSnake(Snake *snake,KeyboardDir direction){
    if(direction == KeyboardDir_Up){
        if(snake->Y == 2) snake->Y = ROWS+1;
        else snake->Y--;
    }
    if(direction == KeyboardDir_Down){
        if(snake->Y == ROWS+1) snake->Y = 2;
        else snake->Y++;
    }
    if(direction == KeyboardDir_Left){
        if(snake->X == 2) snake->X = COLS+1;
        else snake->X--;
    }
    if(direction == KeyboardDir_Right){
        if(snake->X == COLS+1) snake->X = 2;
        else snake->X++;
    }


}

int main(){
    setvbuf(stdout, NULL, _IONBF, 0);

    Snake snake;
    snake.X = 5; 
    snake.Y = 5;
    
    Food food;
    food.X = 10;
    food.Y = 5;

    srand(time(NULL));
    placeFood(&food, snake);

    clrscr();
    hideCursor();
    drawBoundaries();
    drawFood(food);
    
    while(1){
        //clrscr();
        //drawBoundaries(1);
        clearSnake(snake);
        gotoxy(0,0);
        char ch = getNextKeyboardAction();
        if(ch == 'u') moveSnake(&snake,KeyboardDir_Up);
        else if(ch == 'd') moveSnake(&snake,KeyboardDir_Down);
        else if(ch == 'l') moveSnake(&snake,KeyboardDir_Left);
        else if(ch == 'r') moveSnake(&snake,KeyboardDir_Right);
        if(snake.X == food.X && snake.Y == food.Y){
            placeFood(&food, snake);
        }
        drawSnake(snake);
        drawFood(food);
        msleep(250);
    }
    return 1;
}
