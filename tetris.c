#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 20

int field[HEIGHT][WIDTH] = {0};
int block[4][4];
int x = 3, y = 0;
int timer = 0;
int score = 0; // Tambahkan variabel skor

int shapes[7][4][4] = {
    {{0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0}}, // I
    {{0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0}}, // J
    {{1,0,0,0}, {1,0,0,0}, {1,1,0,0}, {0,0,0,0}}, // L
    {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}, // O
    {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}, // S
    {{1,1,1,0}, {0,1,0,0}, {0,0,0,0}, {0,0,0,0}}, // T
    {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}  // Z
};

void gotoxy(int x, int y) {
    COORD coord = { (short)x, (short)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setNewBlock() {
    int shapeID = rand() % 7;
    int i, j;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            block[i][j] = shapes[shapeID][i][j];
        }
    }
}

void rotate() {
    int temp[4][4];
    int i, j;
    for(i=0; i<4; i++)
        for(j=0; j<4; j++)
            temp[j][3-i] = block[i][j];
    
    for(i=0; i<4; i++)
        for(j=0; j<4; j++)
            block[i][j] = temp[i][j];
}

void draw() {
    int i, j;
    gotoxy(0, 0);
    for(i = 0; i < HEIGHT; i++) {
        for(j = 0; j < WIDTH; j++) {
            int isBlockPart = 0;
            if (i >= y && i < y + 4 && j >= x && j < x + 4) {
                if (block[i - y][j - x]) isBlockPart = 1;
            }

            if (field[i][j]) printf("#");
            else if (isBlockPart) printf("O");
            else printf(".");
        }
        printf("\n");
    }
    // Tampilkan skor di bawah area permainan
    printf("\nYOUR SKOR: %d\n", score);
    printf("Kontrol: W(TURN), A(LEFT), S(lOWER), D(RIGHT)");
}

int collision(int nx, int ny) {
    int i, j;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            if(block[i][j]) {
                int fx = nx + j;
                int fy = ny + i;
                if(fx < 0 || fx >= WIDTH || fy >= HEIGHT) return 1;
                if(fy >= 0 && field[fy][fx]) return 1;
            }
        }
    }
    return 0;
}

void checkLines() {
    int i, j, k;
    for(i = HEIGHT - 1; i >= 0; i--) {
        int count = 0;
        for(j = 0; j < WIDTH; j++) {
            if(field[i][j]) count++;
        }
        if(count == WIDTH) {
            score += 100; // Tambah 100 poin setiap satu baris hancur
            for(k = i; k > 0; k--) {
                for(j = 0; j < WIDTH; j++) {
                    field[k][j] = field[k-1][j];
                }
            }
            i++; 
        }
    }
}

void place() {
    int i, j;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            if(block[i][j]) {
                if (y + i >= 0 && y + i < HEIGHT)
                    field[y + i][x + j] = 1;
            }
        }
    }
    checkLines();
}

int main() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ConCurInf; 	
    GetConsoleCursorInfo(hOut, &ConCurInf);
    ConCurInf.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &ConCurInf);

    srand(time(NULL));
    setNewBlock();
    system("cls");

    while(1) {
        if(kbhit()) {
            char c = getch();    
            if(c == 'a' && !collision(x - 1, y)) x--;
            if(c == 'd' && !collision(x + 1, y)) x++;
            if(c == 's' && !collision(x, y + 1)) y++;
            if(c == 'w') rotate(); 
            draw();
        }

        if(timer >= 5) {
            if(!collision(x, y + 1)) {
                y++;
            } else {
                place();
                x = 3; y = 0;
                setNewBlock();
                
                if(collision(x, y)) {
                    draw();
                    gotoxy(0, HEIGHT + 2);
                    printf("GAME OVER! Skor Akhir: %d\n", score);
                    system("pause");
                    return 0;
                }
            }
            timer = 0;
            draw();
        }

        timer++;
        Sleep(100); 
    }
    return 0;
}
