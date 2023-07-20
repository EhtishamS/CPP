#include <iostream>
#include <Windows.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

using namespace std;

#define MATRIX_SIZE 23
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

char body[MATRIX_SIZE*MATRIX_SIZE][2];

void GameOver(){
    cout << "X X X X X   X X X X X   X X   X X   X X X X X\n";
    cout << "X           X       X   X  X X  X   X        \n";
    cout << "X           X       X   X   X   X   X        \n";
    cout << "X   X X X   X X X X X   X       X   X X X X  \n";
    cout << "X       X   X       X   X       X   X        \n";
    cout << "X       X   X       X   X       X   X        \n";
    cout << "X X X X X   X       X   X       X   X X X X X\n\n\n";

    cout << "X X X X X   X       X   X X X X X   X X X X X\n";
    cout << "X       X   X       X   X           X       X\n";
    cout << "X       X   X       X   X           X       X\n";
    cout << "X       X   X       X   X X X X     X X X X X\n";
    cout << "X       X    X     X    X           X  X     \n";
    cout << "X       X     X   X     X           X    X   \n";
    cout << "X X X X X       X       X X X x X   X      X \n";
}

int findBody(int bodySize, int x, int y){
    for(int i=0;i<bodySize;i++){
        if(body[i][0] == x && body[i][1] == y){
            return 1;
        }
    }
    
    return 0;
}

int main(){
    char mtx[MATRIX_SIZE][MATRIX_SIZE];
    srand(time(NULL));

    HANDLE HCmd;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

    HCmd = GetStdHandle (STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo (HCmd,&csbiInfo);

    int moveX = 1;
    int moveY = 0;
    int change = 1;
    int score = 1;

    int headX = rand() % (MATRIX_SIZE - 5 ) + 5;
    int headY = rand() % (MATRIX_SIZE - 5 ) + 5;

    int foodX = rand() % (MATRIX_SIZE - 5 ) + 5;
    int foodY = rand() % (MATRIX_SIZE - 5 ) + 5;

    int key = UP;

    while(true){
        SHORT up = GetKeyState(VK_UP);
        SHORT down = GetKeyState(VK_DOWN);
        SHORT left = GetKeyState(VK_LEFT);
        SHORT right = GetKeyState(VK_RIGHT);

        if(up & 0x8000) {
            if (key != DOWN && key != UP){
                key = UP;
            }
        } else if(down & 0x8000) {
            if (key != UP && key != DOWN){
                key = DOWN;
            }
        } else if(left & 0x8000) {
            if (key != RIGHT && key != LEFT){
                key = LEFT;
            } 
        } else if(right & 0x8000) {
            if (key != LEFT && key != RIGHT){
                key = RIGHT;
            }
        }

        switch (key){
            case UP:
                headY -= change;
                break;
            case DOWN:
                headY += change;
                break;
            case LEFT:
                headX -= change;
                break;
            case RIGHT:
                headX += change;
                break;
        
            default:
            break;
        }

        if (headX >= MATRIX_SIZE || headX < 0 ){
            break;
        } 

        if (headY >= MATRIX_SIZE || headY < 0 ){
            break;
        } 

        if(mtx[headY][headX] == '#'){
            break;
        }

        if (headX == foodX && headY == foodY){
            foodX = rand() % (MATRIX_SIZE - 5 ) + 5;
            foodY = rand() % (MATRIX_SIZE - 5 ) + 5;
            score++;
        }

         for(int i=score-1;i>0;i--){
            body[i][0] = body[i-1][0];
            body[i][1] = body[i-1][1];
        }

        body[0][0] = headX;
        body[0][1] = headY;

        for(int i=0;i<MATRIX_SIZE;i++){
            for(int j=0;j<MATRIX_SIZE;j++){
                mtx[i][j] = ' ';
            }
        }

        for(int i=0; i<score;i++){
            mtx[body[i][1]][body[i][0]] = '#';
        }

        mtx[foodY][foodX] = 'F';

        SetConsoleTextAttribute (HCmd,BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
        for(int i=0;i<MATRIX_SIZE;i++){cout << " ";}
        cout << "  \n";
        SetConsoleTextAttribute (HCmd,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

        for(int i=0;i<MATRIX_SIZE;i++){
            SetConsoleTextAttribute (HCmd,BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
            cout << " ";
            SetConsoleTextAttribute (HCmd,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

            for(int j=0;j<MATRIX_SIZE;j++){
                if (mtx[i][j] == 'F' && mtx[i][j] != '#'){
                    SetConsoleTextAttribute (HCmd,BACKGROUND_RED | BACKGROUND_INTENSITY);
                    cout << " ";
                    SetConsoleTextAttribute (HCmd,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                } else if (mtx[i][j] == '#'){
                    SetConsoleTextAttribute (HCmd,BACKGROUND_GREEN | BACKGROUND_INTENSITY);
                    cout << " ";
                    SetConsoleTextAttribute (HCmd,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                } else {
                    cout << " ";
                }
            }
            SetConsoleTextAttribute (HCmd,BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
            cout << " \n";
            SetConsoleTextAttribute (HCmd,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        }

        SetConsoleTextAttribute (HCmd,BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
        for(int i=0;i<MATRIX_SIZE;i++){
            cout << " ";
        }

        cout << "  \n";
        SetConsoleTextAttribute (HCmd,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        cout << "SCORE: " << score << "\n";

        Sleep(70);
        system("cls");
    }

    SetConsoleTextAttribute (HCmd,FOREGROUND_RED | FOREGROUND_INTENSITY);
    GameOver();
    SetConsoleTextAttribute (HCmd,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    return 0;
}
