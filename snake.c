/*
Thomas Weichhart
29.07.2020
Snake in C
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <time.h>
#include "console.c"

#define XSIZE 79
#define YSIZE 23

int getmove(int move, int *length);
void shift(int array[]);
void printLength(int length);
void displayWin(int length, int olength, int milsec);
int checkdead(int xpos[], int ypos[], int length);
void setxy(int move, int x[], int y[]);
void gameOver(int length, int olength, int milsec);
void printsnake(int length, int xpos[], int ypos[]);
void deletesnake(int length, int xpos[], int ypos[]);
void genfood(int *randx, int *randy, int *length, int xpos[], int ypos[]);
void initUI();

int main()
{
    setCursorType(0);
    initConsole();
    srand(time(0));
    int move = 4, length = 3, olength = length, xpos[YSIZE * XSIZE] = {}, ypos[YSIZE * XSIZE] = {}, run = 1, ax, ay;
    initUI();
    genfood(&ax, &ay, &length, xpos, ypos);
    clock_t start = clock(), diff;
    while (run == 1)
    {
        printsnake(length, xpos, ypos);
        // gotoxy(xpos[0], ypos[0]);
        // printf("O");
        delay(100);
        deletesnake(length, xpos, ypos);
        // gotoxy(xpos[length - 1], ypos[length - 1]);
        // printf(" ");
        printLength(length);
        shift(xpos);
        shift(ypos);
        setxy(move, xpos, ypos);
        run = checkdead(xpos, ypos, length);
        if (xpos[0] == ax && ypos[0] == ay)
            genfood(&ax, &ay, &length, xpos, ypos);
        if (kbhit())
            move = getmove(move, &length);
    }
    diff = clock() - start;
    int milsec = diff * 1000 / CLOCKS_PER_SEC;
    if (run == 2)
        displayWin(length, olength, milsec);
    else
        gameOver(length, olength, milsec);
    return 0;
}
void setxy(int move, int x[], int y[])
{
    switch (move)
    {
    case 1:
        y[0]--;
        break;
    case 2:
        x[0]--;
        break;
    case 3:
        y[0]++;
        break;
    case 4:
        x[0]++;
        break;
    }
}

int getmove(int move, int *length)
{
    switch (getch())
    {
    case 'w':
        if (move != 3)
            return 1;
        else
            return 3;
        break;
    case 'a':
        if (move != 4)
            return 2;
        else
            return 4;
        break;
    case 's':
        if (move != 1)
            return 3;
        else
            return 1;
        break;
    case 'd':
        if (move != 2)
            return 4;
        else
            return 2;
        break;
    case 224:
        switch (getch())
        {
        case 72:
            if (move != 3)
                return 1;
            else
                return 3;
            break;
        case 75:
            if (move != 4)
                return 2;
            else
                return 4;
            break;
        case 80:
            if (move != 1)
                return 3;
            else
                return 1;
            break;
        case 77:
            if (move != 2)
                return 4;
            else
                return 2;
            break;
        }
        break;
    case 13:
        gotoxy(28, 10);
        printf("Enter new length: ");
        scanf("%d", length);
        gotoxy(28, 10);
        printf("                      ");
        return move;
    default:
        return move;
        break;
    }
}

void printsnake(int length, int xpos[], int ypos[])
{
    int steps, hue;
    hue = 10;
    steps = length / 3;
    for (int i = 0; i < length; i++)
    {
        if (i == steps)
            hue = 2;
        if (i == 2 * steps)
            hue = 15;
        textcolor(hue);
        gotoxy(xpos[i], ypos[i]);
        printf("o");
    }
    gotoxy(xpos[0], ypos[0]);
    textcolor(10);
    printf("O");
    textcolor(WHITE);
}

void deletesnake(int length, int xpos[], int ypos[])
{
    for (int i = 0; i < length; i++) //delete snake
    {
        gotoxy(xpos[i], ypos[i]);
        printf(" ");
    }
}

void shift(int array[])
{
    for (int i = YSIZE * XSIZE - 1; i > 0; i--)
        array[i] = array[i - 1];
}

int checkdead(int xpos[], int ypos[], int length)
{
    if (length == XSIZE * YSIZE)
        return 2;
    if (xpos[0] == XSIZE || xpos[0] == -1 || ypos[0] == YSIZE || ypos[0] == -1)
        return 0;
    for (int i = 0; i < length - 1; i++)
    {
        if (xpos[0] == xpos[i + 1] && ypos[0] == ypos[i + 1])
            return 0;
    }
    return 1;
}

void genfood(int *randx, int *randy, int *length, int xpos[], int ypos[])
{
    int i, run;
    if (*length != XSIZE * YSIZE)
    {
        do
        {
            *randx = rand() % XSIZE;
            (*randy) = rand() % YSIZE;
            for (i = 0; i < (*length); i++)
            {
                if ((*randx) == xpos[i] && (*randy) == ypos[i])
                {
                    run = 1;
                    break;
                }
                run = 0;
            }
        } while (run == 1);
        gotoxy(*randx, *randy);
        textcolor(H_RED);
        printf("@");
        textcolor(WHITE);
        (*length)++;
    }
}

void initUI()
{
    int i;
    gotoxy(0, YSIZE);
    for (i = 0; i < XSIZE; i++)
        printf("-");
    for (i = 0; i < YSIZE; i++)
    {
        gotoxy(XSIZE, i);
        printf("|");
    }
    gotoxy(0, YSIZE + 1);
    printf("L%cnge: ", 132);
}

void gameOver(int length, int olength, int milsec)
{
    clrscr();
    gotoxy(35, 15);
    printf("score: %d", length - olength);
    gotoxy(27, 16);
    printf("Gametime: %d s, %d ms", milsec / 1000, milsec % 1000);
    gotoxy(0, 8);
    textcolor(H_RED);
    puts(
        "               _____                         ____                 \n"
        "              / ____|                       / __ \\                \n"
        "             | |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __ \n"
        "             | | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__|\n"
        "             | |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |   \n"
        "              \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|   ");
    getch();
}

void printLength(int length)
{
    gotoxy(8, YSIZE + 1);
    printf("%d ", length);
}

void displayWin(int length, int olength, int milsec)
{
    clrscr();
    gotoxy(35, 15);
    printf("score: %d", length - olength);
    gotoxy(27, 16);
    printf("Gametime: %d s, %d ms", milsec / 1000, milsec % 1000);
    gotoxy(0, 8);
    textcolor(H_GREEN);
    puts(
        "                 __     __          __          ___       \n"
        "                 \\ \\   / /          \\ \\        / (_)      \n"
        "                  \\ \\_/ /__  _   _   \\ \\  /\\  / / _ _ __  \n"
        "                   \\   / _ \\| | | |   \\ \\/  \\/ / | | '_ \\ \n"
        "                    | | (_) | |_| |    \\  /\\  /  | | | | |\n"
        "                    |_|\\___/ \\__,_|     \\/  \\/   |_|_| |_|");
    getch();
}
