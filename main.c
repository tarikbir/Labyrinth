#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
/* GLOBAL DEFINES */
#define MAX 40
#define X 50

char moves[] = {0 , 1 , 2 , 3};
/*  0 = Up
    1 = Right
    2 = Down
    3 = Left */

struct move{
    char x;
    char y;
    char remaining;
    struct move *next;
};

struct stack{
    struct move val;
    struct stack *prev;
    struct stack *next;
};

void drawMap(char map[X][X])
{
    /**Draws the map onto the cmd screen*/
    printf("╔");
    for (int i=0;i<=3*X+2;i++) {printf("═");}
    printf("╗\n║");
    for (int i=0;i<=X;i++) {printf("%02d ",i);}
    printf("║\n");
    for (int i=0;i<X;i++)
    {
        printf("║%02d ",i+1);
        for (int j=0;j<X;j++)
        {
            if (map[i][j] == 0) //Walls
                printf("   ");
            else if (map[i][j] == 1) //Walkways
                printf("███");
            else if (map[i][j] == 2) //Start point
                printf("▌⌂▐");
            else if (map[i][j] == 3) //End point
                printf("▌E▐");
            else if (map[i][j] == 4) //Walked
                printf("░░░");
            else if (map[i][j] == 5) //True Path
                printf("▓▓▓");
        }
        printf("║\n");
    }
    printf("╚");
    for (int i=0;i<=3*X+2;i++) {printf("═");}
    printf("╝");
}

void generateRandomMap(char map[X][X])
{
    /**Generates a random map between 0 and 1*/
    short int chance = 25; //Chance multiplier for smoothing algorithm
    /* FULL RANDOM */
    for (int i=0;i<X;i++)
    {
        for (int j=0;j<X;j++)
        {
            map[i][j] = rand()%2;
        }
    }
    /* SMOOTHING THE FIRST FULL RANDOM */
    char xmap[X][X] = {0};
    for (int i=0;i<X;i++)
    {
        for (int j=0;j<X;j++)
        {
            if (i-1>=0 && j-1>=0 && i+1<X && j+1<X)
            {
                char amount_of_adjacent_available_tiles = 0;
                if(map[i-1][j]) amount_of_adjacent_available_tiles++; //Up
                if(map[i][j-1]) amount_of_adjacent_available_tiles++; //Left
                if(map[i][j+1]) amount_of_adjacent_available_tiles++; //Right
                if(map[i+1][j]) amount_of_adjacent_available_tiles++; //Down
                char inner_chance = 2-abs(2-amount_of_adjacent_available_tiles); // 2-|2-x|
                if (chance*inner_chance > (rand()%101))
                    xmap[i][j] = 1;
                else
                    xmap[i][j] = 0;
            }
        }
    }
    for (int i=1;i<X-1;i++)
    {
        for (int j=1;j<X-1;j++)
            map[i][j] = xmap[i][j];
    }
}

int getNumber(int max)
{
    /**Gets an integer from stdin within 0 and max.*/
    char integerToReturn[MAX];
    char *p;
    do{
        fgets(integerToReturn,MAX,stdin);
        int toReturn = (int)strtol(integerToReturn,&p,10);
        if ( toReturn >= 0 && toReturn <= max && toReturn != NULL )
            return toReturn;
        printf("ERROR: Wrong input.\nTry again: ");
    }while(1);
}

int writeMapBySide(int side, int count, char map[X][X], int max, char info, int doCheck, int check)
{
    /**Picks given side and converts it to coordinates and writes down the information*/
    char *p;
    if (side == 1)
        p = &map[0][count-1];
    else if (side == 2)
        p = &map[count-1][max-1];
    else if (side == 3)
        p = &map[max-1][count-1];
    else if (side == 4)
        p = &map[count-1][0];
    else
        printf("\nERROR: Wrong side or info in writeMapBySide!");
    if (doCheck)
    {
        if (check == *p)
        {
            *p = info;
            return 1;
        }
        return 0;
    }
    *p = info;
    return 1;
}

int main()
{
    /* INITIALIZATION */
    system("echo Initializing...");
    system("color C");
    system("chcp 65001");
    system("cls");
    srand(time(NULL));
    char map[X][X] = {0};
    /* TODO: SCANF X MALLOC DIZI */
    generateRandomMap(map);
    drawMap(map);
    /* GET ENTRANCE AND EXIT */
    //ENTRANCE
    do{
        printf("\nPick the entrance:\n1 = Up\t2 = Right\t3 = Down\t4 = Left\nSide: ");
        int side = getNumber(4);
        printf("Point on side?: ");
        int point = getNumber(X);
        int check = writeMapBySide(side, point, map, X, 2, 1, 1);
        if (check) break;
        printf("\nERROR: Wrong input! That point is not a valid target for an entrance!");
    }while(1);
    system("cls");
    drawMap(map);
    //EXIT
    do{
        printf("\nPick the exit:\n1 = Up\t2 = Right\t3 = Down\t4 = Left\nSide: ");
        int side = getNumber(4);
        printf("Point on side?: ");
        int point = getNumber(X);
        int check = writeMapBySide(side, point, map, X, 3, 1, 1);
        if (check) break;
        printf("\nERROR: Wrong input! That point is not a valid target for an exit!");
    }while(1);
    system("cls");
    drawMap(map);
    /* START ITERATING THROUGH THE MAP */
    struct move *current;

    return 0;
}
