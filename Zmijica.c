#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define WIDTH 20
#define HEIGHT 20

int gameOver;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;

void setup() {
    gameOver = 0;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
    nTail = 0;
}

void draw() {
    system("cls"); // Brisanje ekrana
    for (int i = 0; i < WIDTH + 2; i++) 
        printf("#");
    printf("\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0)
                printf("#");
            if (i == y && j == x)
                printf("O"); // Glava zmije
            else if (i == fruitY && j == fruitX)
                printf("F"); // Voće
            else {
                int print = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("o"); // Rep zmije
                        print = 1;
                    }
                }
                if (!print)
                    printf(" ");
            }

            if (j == WIDTH - 1)
                printf("#");
        }
        printf("\n");
    }

    for (int i = 0; i < WIDTH + 2; i++) 
        printf("#");
    printf("\n");
    printf("Score: %d\n", score);
}

int isTailPosition(int newX, int newY) {
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == newX && tailY[i] == newY) {
            return 1; // Nađena je pozicija repa
        }
    }
    return 0; // Pozicija nije deo repa
}

// Jednostavna funkcija za kretanje ka voću
void moveTowardsFruit() {
    int rightSafe = (x + 1 < WIDTH && !isTailPosition(x + 1, y));
    int leftSafe = (x - 1 >= 0 && !isTailPosition(x - 1, y));
    int upSafe = (y - 1 >= 0 && !isTailPosition(x, y - 1));
    int downSafe = (y + 1 < HEIGHT && !isTailPosition(x, y + 1));

    // Pokušaj da se kreće prema voću
    if (x < fruitX && rightSafe) {
        x++; // Pomeri se desno
    } else if (x > fruitX && leftSafe) {
        x--; // Pomeri se levo
    } else if (y < fruitY && downSafe) {
        y++; // Pomeri se dole
    } else if (y > fruitY && upSafe) {
        y--; // Pomeri se gore
    } else {
        // Ako ne može da se kreće prema voću, tražimo slobodan pravac
        if (rightSafe) {
            x++; // Pomeri se desno
        } else if (leftSafe) {
            x--; // Pomeri se levo
        } else if (downSafe) {
            y++; // Pomeri se dole
        } else if (upSafe) {
            y--; // Pomeri se gore
        } else {
            // Ako nema slobodnih pravaca, kraj igre
            gameOver = 1;
        }
    }
}

void logic() {
    // Pomera rep zmije kao i pre
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Kretanje prema voću
    moveTowardsFruit();

    // Kada zmija pojede voće
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        nTail++;
    }

    // Ako zmija udari zidove (teleportacija na suprotnu stranu)
    if (x >= WIDTH) x = 0; else if (x < 0) x = WIDTH - 1;
    if (y >= HEIGHT) y = 0; else if (y < 0) y = HEIGHT - 1;

    // Provera udarca u rep
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameOver = 1;
        }
    }
}

int main() {
    setup();
    while (!gameOver) {
        draw();
        logic();
        Sleep(100); // Pauza između poteza zmije
    }
    return 0;
}
