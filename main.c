#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
/* GLOBAL DEFINES */
#define MAX 40
#define X 120
HANDLE hConsole;
char defaultColor = 15;

/*  Directions
    0 = Up
    1 = Right
    2 = Down
    3 = Left */

struct node
{
    char data;
    struct node* up;
    struct node* right;
    struct node* down;
    struct node* left;
};

struct node* newNode(char data)
{
    struct node* newN = (struct node*)malloc(sizeof(struct node));
    newN->data = data;
    newN->up = NULL;
    newN->right = NULL;
    newN->down = NULL;
    newN->left = NULL;
    return newN;
};

void convert(char map[X][X], struct node* p[X][X], int n)
{
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            p[i][j] = newNode(map[i][j]);
        }
    }
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            if ( i>0 )
            {
                p[i][j]->up = p[i-1][j];
                p[i-1][j]->down = p[i][j];
            }
            if ( j<n-1 )
            {
                p[i][j]->right = p[i][j+1];
                p[i][j+1]->left = p[i][j];
            }
            if ( i<n-1 )
            {
                p[i][j]->down = p[i+1][j];
                p[i+1][j]->up = p[i][j];
            }
            if ( j>0 )
            {
                p[i][j]->left = p[i][j-1];
                p[i][j-1]->right = p[i][j];
            }
        }
    }
}

int findPath(struct node* p)
{
    struct node* w = p;
    if (w->data == 3)
        return 1;
    else if (w->data == 1)
        p->data = 4;
    if (w->up != NULL && (w->up->data == 1 || w->up->data == 3)){ //Ust
        if (findPath(w->up))
        {
            if (p->data != 2) p->data = 5;
            return 1;
        }
    }
    if (w->right != NULL && (w->right->data == 1 || w->right->data == 3)){ //Sag
        if (findPath(w->right))
        {
            if (p->data != 2) p->data = 5;
            return 1;
        }
    }
    if (w->down != NULL && (w->down->data == 1 || w->down->data == 3)){ //Alt
        if (findPath(w->down))
        {
            if (p->data != 2) p->data = 5;
            return 1;
        }
    }
    if (w->left != NULL && (w->left->data == 1 || w->left->data == 3)){ //Sol
        if (findPath(w->left))
        {
            if (p->data != 2) p->data = 5;
            return 1;
        }
    }
    free(w);
    return 0;
}

void drawMap(char map[X][X], int n)
{
    /**Draws the map onto the cmd screen*/
    SetConsoleTextAttribute(hConsole, 7);
    printf("╔");
    for (int i=0; i<=3*n+2; i++)
    {
        printf("═");
    }
    printf("╗\n║");
    for (int i=0; i<=n; i++)
    {
        printf("%02d ",i);
    }
    printf("║\n");
    for (int i=0; i<n; i++)
    {
        printf("║%02d ",i+1);
        for (int j=0; j<n; j++)
        {
            if (map[i][j] == 0) //Walls
            {
                SetConsoleTextAttribute(hConsole, 4);
                printf("███");
            }
            else if (map[i][j] == 1) //Walkways
            {
                SetConsoleTextAttribute(hConsole, 6);
                printf("░░░");
            }
            else if (map[i][j] == 2) //Start point
            {
                SetConsoleTextAttribute(hConsole, 6);
                printf("░⌂░");
            }
            else if (map[i][j] == 3) //End point
            {
                SetConsoleTextAttribute(hConsole, 6);
                printf("░E░");
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 6);
                printf("░░░"); //NULL
            }
        }
        SetConsoleTextAttribute(hConsole, 7);
        printf("║\n");
    }
    printf("╚");
    for (int i=0; i<=3*n+2; i++)
    {
        printf("═");
    }
    printf("╝");
    SetConsoleTextAttribute(hConsole, defaultColor);
}

void drawPointerMap(struct node** p, int n)
{
    /**Draws the pointerMap onto the cmd screen*/
    struct node* w = *p;
    struct node* w_line = *p;
    int line = 0;
    SetConsoleTextAttribute(hConsole, 7);
    printf("╔");
    for (int i=0; i<=3*n+2; i++)
    {
        printf("═");
    }
    printf("╗\n║");
    for (int i=0; i<=n; i++)
    {
        printf("%02d ",i);
    }
    printf("║\n");
    do
    {
        w_line = w;
        printf("║%02d ",++line);
        do
        {
            if (w_line->data == 0) //Walls
            {
                SetConsoleTextAttribute(hConsole, 4);
                printf("███");
            }
            else if (w_line->data == 1) //Walkways
            {
                SetConsoleTextAttribute(hConsole, 6);
                printf("░░░");
            }
            else if (w_line->data == 2) //Start point
            {
                SetConsoleTextAttribute(hConsole, 14);
                printf("▓⌂▓");
            }
            else if (w_line->data == 3) //End point
            {
                SetConsoleTextAttribute(hConsole, 14);
                printf("▓E▓");
            }
            else if (w_line->data == 4) //Walked
            {
                SetConsoleTextAttribute(hConsole, 14);
                printf("░░░");
            }
            else if (w_line->data == 5) //True Path
            {
                SetConsoleTextAttribute(hConsole, 14);
                printf("▓▓▓");
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 6);
                printf("░░░"); //NULL
            }
            w_line = w_line->right;
        }while (w_line != NULL);
        SetConsoleTextAttribute(hConsole, 7);
        printf("║\n");
        w = w->down;
    }while (w != NULL);
    printf("╚");
    for (int i=0; i<=3*n+2; i++)
    {
        printf("═");
    }
    printf("╝");
    SetConsoleTextAttribute(hConsole, defaultColor);
}

void generateRandomMap(char map[X][X], int n, int doSmoothing, int smoothingFactor)
{
    /**Generates a random map between 0 and 1. Then does smoothing if enabled by the factor provided. Smoothing algorithm is x%4.*/
    /* FULL RANDOM */
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            map[i][j] = rand()%2;
        }
    }
    /* SMOOTHING THE FIRST FULL RANDOM */
    if (doSmoothing)
    {
        short int chance = 33; //Chance multiplier for smoothing algorithm
        for (int k=1; k<=smoothingFactor; k++){
            char xmap[X][X] = {0};
            for (int i=0; i<n; i++)
            {
                for (int j=0; j<n; j++)
                {
                    if (i-1>=0 && j-1>=0 && i+1<n && j+1<n)
                    {
                        char amount_of_adjacent_available_tiles = checkAround(map, n, i, j, 1);
                        char inner_chance = amount_of_adjacent_available_tiles%4; // Removes chunky intersections and randomizes again.
                        if (chance*inner_chance > (rand()%101))
                            xmap[i][j] = 1;
                        else
                            xmap[i][j] = 0;
                    }
                }
            }
            for (int i=1; i<n-1; i++)
            {
                for (int j=1; j<n-1; j++)
                    map[i][j] = xmap[i][j];
            }
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

int getPoint(char map[X][X], int x, int y, int n)
{
    if (x>n || x<0 || y>n || y<0)
        return 0; //If edges, return 0
    return map[x][y];
}

int checkAround(char map[X][X], int n, int x, int y, int valueToCheck)
{
    int sum = 0;
    if (getPoint(map, x-1, y, n)==valueToCheck) sum++;
    if (getPoint(map, x, y-1, n)==valueToCheck) sum++;
    if (getPoint(map, x, y+1, n)==valueToCheck) sum++;
    if (getPoint(map, x+1, y, n)==valueToCheck) sum++;
    return sum;
}

void findPoint(int *p, char map[X][X], int n, int search)
{
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            if (map[i][j] == search)
                {
                    *p = i;
                    *(p+1) = j;
                }
        }
    }
}

struct node* navigateTo(struct node* p, int x, int y)
{
    for (int i=0; i<x; i++)
    {
        p=p->down;
    }
    for (int i=0; i<y; i++)
    {
        p=p->right;
    }
    return p;
}

int main()
{
    /* INITIALIZATION */
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, defaultColor);
    system("echo Initializing...\nIf you see this, your computer is slow.");
    system("chcp 65001");
    system("cls");
    srand(time(NULL));
    char map[X][X] = {0};
    struct node* pointerMap[X][X] = {NULL};;
    /* GET SIZE OF THE MAP */
    printf("Please, enter the dimension you would like to generate the map for: ");
    int n = getNumber(X);
    generateRandomMap(map,n,1,10);
    system("cls");
    drawMap(map,n);
    /* GET ENTRANCE AND EXIT */
    //ENTRANCE
    do
    {
        printf("\nPick the entrance:\n1 = Up\t2 = Right\t3 = Down\t4 = Left\nSide: ");
        int side = getNumber(4);
        printf("Point on side?: ");
        int point = getNumber(n);
        int check = writeMapBySide(side, point, map, n, 2, 1, 1);
        if (check) break;
        printf("\nERROR: Wrong input! That point is not a valid target for an entrance!");
    }
    while(1);
    system("cls");
    drawMap(map,n);
    //EXIT
    do
    {
        printf("\nPick the exit:\n1 = Up\t2 = Right\t3 = Down\t4 = Left\nSide: ");
        int side = getNumber(4);
        printf("Point on side?: ");
        int point = getNumber(n);
        int check = writeMapBySide(side, point, map, n, 3, 1, 1);
        if (check) break;
        printf("\nERROR: Wrong input! That point is not a valid target for an exit!");
    }
    while(1);
    system("cls");
    drawMap(map,n);
    /* START ITERATING THROUGH THE MAP */
    int getPoint[2], isExitFound=0;
    findPoint(getPoint,map,n,2);                            //Finds the starting point coordinates on the map.
    convert(map,pointerMap,n);                              //Map conversion to linked list type map.
    struct node* start = pointerMap[0][0];                  //Sets a pointer starting from the top left corner of the map.
    start = navigateTo(start, getPoint[0], getPoint[1]);    //Navigates starting pointer to find the starting point on the pointer map.
    system("cls");
    if (findPath(start))                                    //Starts the recursive function, iterating through the map using operating system's own stack structure.
    {
        isExitFound = 1;                                    //If it returns TRUE, that means the stack has found an exit and returned back to it's root as TRUE.
    }
    drawPointerMap(&pointerMap,n);                          //Another draw function to input a pointer map and draw on the cmd window.
    if (isExitFound)
        printf("\nExit has been found!!!");
    else
        printf("\nExit cannot be found!!!");
    return 0;
}
