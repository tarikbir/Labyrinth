#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
/* GLOBAL DEFINES */
#define MAX 40
#define X 50

char moves[] = {0, 1, 2, 3};
/*  0 = Up
    1 = Right
    2 = Down
    3 = Left */

struct stack
{
    char move;
    struct stack *next;
};

struct node
{
    struct stack movesToDo;
    int x;
    int y;
    char remainingMove;
};

void push(struct stack* top, int nextmove)
{
    struct stack *ekle=(struct stack*)malloc(sizeof(struct stack));
    if (ekle == NULL){
        printf("\nERROR: No more memory to add to the stack!");
    }
    else{
        ekle->move=nextmove;
        ekle->next=NULL;
        if(top!=NULL)
        {
            ekle->next=top;
            top=ekle;
        }
    }
}

char pop(struct stack top)
{
    if(top->move==NULL)
    {
        printf("\nERROR: Stack is empty!");
        return -1;
    }
    else
    {
        struct stack *gecici;
        char returnVal;
        gecici=top;
        returnVal = top->move;
        top=top->next;
        free(gecici);
        return returnVal;
    }
}

void drawMap(char map[X][X])
{
    /**Draws the map onto the cmd screen*/
    printf("╔");
    for (int i=0; i<=3*X+2; i++)
    {
        printf("═");
    }
    printf("╗\n║");
    for (int i=0; i<=X; i++)
    {
        printf("%02d ",i);
    }
    printf("║\n");
    for (int i=0; i<X; i++)
    {
        printf("║%02d ",i+1);
        for (int j=0; j<X; j++)
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
    for (int i=0; i<=3*X+2; i++)
    {
        printf("═");
    }
    printf("╝");
}

void generateRandomMap(char map[X][X])
{
    /**Generates a random map between 0 and 1*/
    /* FULL RANDOM */
    for (int i=0; i<X; i++)
    {
        for (int j=0; j<X; j++)
        {
            map[i][j] = rand()%2;
        }
    }
    /* SMOOTHING THE FIRST FULL RANDOM */
    int doSmoothing = 1;
    if (doSmoothing)
    {
        short int chance = 33; //Chance multiplier for smoothing algorithm
        char xmap[X][X] = {0};
        for (int i=0; i<X; i++)
        {
            for (int j=0; j<X; j++)
            {
                if (i-1>=0 && j-1>=0 && i+1<X && j+1<X)
                {
                    char amount_of_adjacent_available_tiles = checkAround(map,i,j,1);
                    char inner_chance = 2-abs(2-amount_of_adjacent_available_tiles); // 2-|2-x|
                    if (chance*inner_chance > (rand()%101))
                        xmap[i][j] = 1;
                    else
                        xmap[i][j] = 0;
                }
            }
        }
        for (int i=1; i<X-1; i++)
        {
            for (int j=1; j<X-1; j++)
                map[i][j] = xmap[i][j];
        }
    }
}

int getNumber(int max)
{
    /**Gets an integer from stdin within 0 and max.*/
    char integerToReturn[MAX];
    char *p;
    do
    {
        fgets(integerToReturn,MAX,stdin);
        int toReturn = (int)strtol(integerToReturn,&p,10);
        if ( toReturn >= 0 && toReturn <= max && toReturn != NULL )
            return toReturn;
        printf("\nERROR: Wrong input.\nTry again: ");
    }
    while(1);
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

int getPoint(char map[X][X], int x, int y)
{
    if (x>X || x<0 || y>X || y<0)
        return 0; //If edges, return 0
    return map[x][y];
}

int checkAround(char map[X][X], int x, int y, int valueToCheck)
{
    int sum = 0;
    if (getPoint(map, x-1, y)==valueToCheck) sum++;
    if (getPoint(map, x, y-1)==valueToCheck) sum++;
    if (getPoint(map, x, y+1)==valueToCheck) sum++;
    if (getPoint(map, x+1, y)==valueToCheck) sum++;
    return sum;
}

int main()
{
    /* INITIALIZATION */
    system("echo Initializing...\nIf you see this, your computer is slow.");
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
    do
    {
        printf("\nPick the entrance:\n1 = Up\t2 = Right\t3 = Down\t4 = Left\nSide: ");
        int side = getNumber(4);
        printf("Point on side?: ");
        int point = getNumber(X);
        int check = writeMapBySide(side, point, map, X, 2, 1, 1);
        if (check) break;
        printf("\nERROR: Wrong input! That point is not a valid target for an entrance!");
    }
    while(1);
    system("cls");
    drawMap(map);
    //EXIT
    do
    {
        printf("\nPick the exit:\n1 = Up\t2 = Right\t3 = Down\t4 = Left\nSide: ");
        int side = getNumber(4);
        printf("Point on side?: ");
        int point = getNumber(X);
        int check = writeMapBySide(side, point, map, X, 3, 1, 1);
        if (check) break;
        printf("\nERROR: Wrong input! That point is not a valid target for an exit!");
    }
    while(1);
    system("cls");
    drawMap(map);
    /* START ITERATING THROUGH THE MAP */
    struct move *current;

    return 0;
}
