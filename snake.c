/*
Thomas Weichhart
04.08.2020
Snake in C
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <time.h>
#include "console.c"

#define XSIZE 80
#define YSIZE 23

int getmove(int move, int *length);
void shift(int array[]);
void printLength(int length);
void displayWin(int length, int olength, int milsec);
int checkdead(int xpos[], int ypos[], int length);
void setxy(int move, int x[], int y[]);
void deathplay(int xpos[], int ypos[]);
void gameOver(int length, int olength, int milsec);
void printsnake(int length, int xpos[], int ypos[], int move);
void deletesnake(int length, int xpos[], int ypos[]);
void genfood(int *randx, int *randy, int *length, int xpos[], int ypos[]);
void fprintsnake(int length, int xpos[], int ypos[], int move, int oldmove);
void initUI();

int main()
{
    setCursorType(0);
    initConsole();
    srand(time(0));
    int move = 4, oldmove = move, length = 2, olength = length, xpos[YSIZE * XSIZE] = {}, ypos[YSIZE * XSIZE] = {}, run = 1, ax, ay; //length = length-2
    initUI();
    genfood(&ax, &ay, &length, xpos, ypos);
    clock_t start = clock(), diff;
    while (run == 1)
    {
        shift(xpos);
        shift(ypos);
        setxy(move, xpos, ypos);
        fprintsnake(length, xpos, ypos, move, oldmove);
        delay(100);
        deletesnake(length, xpos, ypos);
        printLength(length);
        run = checkdead(xpos, ypos, length);
        genfood(&ax, &ay, &length, xpos, ypos);
        oldmove = move;
        if (kbhit())
            move = getmove(move, &length);
    }
    diff = clock() - start;
    int milsec = diff * 1000 / CLOCKS_PER_SEC;
    if (run == 2)
        displayWin(length, olength, milsec);
    else
    {
        deathplay(xpos, ypos);
        gameOver(length, olength, milsec);
    }
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

void fprintsnake(int length, int xpos[], int ypos[], int move, int oldmove)
{
    /*
    old = 1 & new = 2 == 187
    old = 1 & new = 4 == 201

    old = 2 & new = 1 == 200
    old = 2 & new = 3 == 201

    old = 3 & new = 2 == 188
    old = 3 & new = 4 == 200

    old = 4 & new = 1 == 188
    old = 4 & new = 3 == 187
    */
    gotoxy(xpos[1], ypos[1]);
    switch (oldmove)
    {
    case 1:
        switch (move)
        {
        case 1:
            printf("%c", 186);
            break;
        case 2:
            printf("%c", 187);
            break;
        case 4:
            printf("%c", 201);
            break;
        }
        break;

    case 2:
        switch (move)
        {
        case 2:
            printf("%c", 205);
            break;
        case 1:
            printf("%c", 200);
            break;
        case 3:
            printf("%c", 201);
            break;
        }
        break;
    case 3:
        switch (move)
        {
        case 3:
            printf("%c", 186);
            break;
        case 2:
            printf("%c", 188);
            break;
        case 4:
            printf("%c", 200);
            break;
        }
        break;
    case 4:
        switch (move)
        {
        case 4:
            printf("%c", 205);
            break;
        case 1:
            printf("%c", 188);
            break;
        case 3:
            printf("%c", 187);
            break;
        }
        break;
    }
    textcolor(2);
    gotoxy(xpos[0], ypos[0]);
    switch (move)
    {
    case 1:
        printf("^");
        break;
    case 2:
        printf("<");
        break;
    case 3:
        printf("v");
        break;
    case 4:
        printf(">");
        break;
    }
    textcolor(WHITE);
}

void deletesnake(int length, int xpos[], int ypos[])
{
    gotoxy(xpos[length], ypos[length]);
    printf(" ");
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
    if (xpos[0] == XSIZE)
    {
        xpos[0] = XSIZE - 1;
        return 0;
    }
    if (xpos[0] == -1)
    {
        xpos[0] = 0;
        return 0;
    }
    if (ypos[0] == YSIZE)
    {
        ypos[0] == YSIZE - 1;
        return 0;
    }
    if (ypos[0] == -1)
    {
        ypos[0] = 0;
        return 0;
    }
    for (int i = 0; i < length - 1; i++)
    {
        if (xpos[0] == xpos[i + 1] && ypos[0] == ypos[i + 1])
            return 0;
    }
    return 1;
}

void deathplay(int xpos[], int ypos[])
{
    int x = xpos[0];
    int y = ypos[0];
    for (int i = 0; i < 4; i++)
    {
        gotoxy(x, y);
        printf("-");
        delay(50);
        gotoxy(x, y);
        printf("%c", 92);
        delay(50);
        gotoxy(x, y);
        printf("|");
        delay(50);
        gotoxy(x, y);
        printf("/");
        delay(50);
    }
    gotoxy(x, y);
    printf("X");
    delay(900);
}

void genfood(int *randx, int *randy, int *length, int xpos[], int ypos[])
{
    int i, run;
    if (xpos[0] == *randx && ypos[0] == *randy)
    {
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
}

void initUI()
{
    int i;
    gotoxy(0, YSIZE);
    for (i = 0; i < XSIZE; i++)
        printf("-");
    // for (i = 0; i < YSIZE; i++)
    // {
    //     gotoxy(XSIZE, i);
    //     printf("|");
    // }
    gotoxy(0, YSIZE + 1);
    printf("L%cnge: ", 132);
}

void gameOver(int length, int olength, int milsec)
{
    clrscr();
    gotoxy(35, 15);
    printf("score: %d", length - olength - 1);
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
    delay(800);
    getch();
}

void printLength(int length)
{
    gotoxy(8, YSIZE + 1);
    printf("%d ", length + 1);
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
