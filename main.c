#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <conio.h>
/* GLOBAL DEFINES */
#define MAX 40 //Max character lenght
#define X 98 //Max screen lenght
HANDLE hConsole;
char defaultColor = 15;

/*
::Directions::
(clockwise)
    0 = Up
    1 = Right
    2 = Down
    3 = Left
::Legend::
    0 = Wall
    1 = Path
    2 = Entrance
    3 = Exit
    4 = Walked Tiles
    5 = Path To Exit
*/

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
    /**Dynamically allocates new memory to create new node struct*/
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
    /**Converts the matrix map to nodal pointer map, dynamically allocating memory*/
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            p[i][j] = newNode(map[i][j]);   //Creates a nodal pointer map, allocating memory.
        }
    }
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            if ( i>0 && p[i][j]->up == NULL )      //Connects above node with itself
            {
                p[i][j]->up = p[i-1][j];
                p[i-1][j]->down = p[i][j];
            }
            if ( j<n-1 && p[i][j]->right == NULL )    //Connects node on the right with itself
            {
                p[i][j]->right = p[i][j+1];
                p[i][j+1]->left = p[i][j];
            }
            if ( i<n-1 && p[i][j]->down == NULL )    //Connects below node with itself
            {
                p[i][j]->down = p[i+1][j];
                p[i+1][j]->up = p[i][j];
            }
            if ( j>0 && p[i][j]->left == NULL )      //Connects node on the left with itself
            {
                p[i][j]->left = p[i][j-1];
                p[i][j-1]->right = p[i][j];
            }
        }
    }
}

int findPath(struct node* p)
{
    /**Recursive function to find a path towards the exit (3). The function calls itself to make use of operating system's own stack structure, avoiding unoptimized and redundant code*/
    struct node* w = p;
    if (w->data == 3)   //If data is 3, then exit has been found.
        return 1;
    else if (w->data == 1)  //If data is 1 here, change it to the 'walked' id
        p->data = 4;
    if (w->up != NULL && (w->up->data == 1 || w->up->data == 3)){ //Checks to see if above node is available, then checks for walkability.
        if (findPath(w->up)){
            if (p->data != 2) p->data = 5;
            return 1;
        }
    }
    if (w->right != NULL && (w->right->data == 1 || w->right->data == 3)){ //Checks to see if node on the right is available, then checks for walkability.
        if (findPath(w->right)){
            if (p->data != 2) p->data = 5;
            return 1;
        }
    }
    if (w->down != NULL && (w->down->data == 1 || w->down->data == 3)){ //Checks to see if below node is available, then checks for walkability.
        if (findPath(w->down)){
            if (p->data != 2) p->data = 5;
            return 1;
        }
    }
    if (w->left != NULL && (w->left->data == 1 || w->left->data == 3)){ //Checks to see if node on the left is available, then checks for walkability.
        if (findPath(w->left)){
            if (p->data != 2) p->data = 5;
            return 1;
        }
    }
    free(w);
    return 0;
}

void drawMap(char map[X][X], int n)
{
    /**Draws the matrix map onto the cmd screen. Not suitable for solution since it's not done by matrice operations*/
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
            if (map[i][j] == 0) //Wall
            {
                SetConsoleTextAttribute(hConsole, 4);
                printf("███");
            }
            else if (map[i][j] == 1) //Path
            {
                SetConsoleTextAttribute(hConsole, 6);
                printf("░░░");
            }
            else if (map[i][j] == 2) //Entrance
            {
                SetConsoleTextAttribute(hConsole, 6);
                printf("░⌂░");
            }
            else if (map[i][j] == 3) //Exit
            {
                SetConsoleTextAttribute(hConsole, 6);
                printf("░E░");
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 6);
                printf("░░░");
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
    /**Draws the nodal pointer map onto the cmd screen. Suitable for solution*/
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
            if (w_line->data == 0) //Wall
            {
                SetConsoleTextAttribute(hConsole, 4);
                printf("███");
            }
            else if (w_line->data == 1) //Path
            {
                SetConsoleTextAttribute(hConsole, 6);
                printf("░░░");
            }
            else if (w_line->data == 2) //Entrance
            {
                SetConsoleTextAttribute(hConsole, 14);
                printf("▓⌂▓");
            }
            else if (w_line->data == 3) //Exit
            {
                SetConsoleTextAttribute(hConsole, 14);
                printf("▓E▓");
            }
            else if (w_line->data == 4) //Walked Tile
            {
                SetConsoleTextAttribute(hConsole, 14);
                printf("░░░");
            }
            else if (w_line->data == 5) //Path To Exit
            {
                SetConsoleTextAttribute(hConsole, 14);
                printf("▓▓▓");
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 6);
                printf("░░░");
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
    /**Generates a random map between 0 and 1. Then does smoothing if enabled by the parameter. Smoothing algorithm is x%4.
    @param doSmoothing: Enables smoothing.
    @param smoothingFactor: Number of times the smoothing calculation has been done.*/
    /* FULL RANDOM */
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            map[i][j] = rand()%2;
        }
    }
    /* SMOOTHING AFTER THE FULL RANDOM */
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
    /**Gets an integer from stdin within 0 and max. Can't accept 0.*/
    char integerToReturn[MAX];
    char *p;
    do{
        fgets(integerToReturn,MAX,stdin);
        int toReturn = (int)strtol(integerToReturn,&p,10);
        if ( toReturn >= 0 && toReturn <= max && toReturn != NULL )
            return toReturn;
        printf("\nERROR: Wrong input.\nTry again: ");
    }while(1);
}

int writeMapBySide(int side, int count, char map[X][X], int max, char info, int doCheck, int check)
{
    /**Picks given side and converts it to coordinates and writes down the given information
    @param side: 1.Up 2.Right 3.Down 4.Left
    @param max: Size of the map
    @param info: Information to be written on the point
    @param doCheck: Does a check by the parameter given, returning the result
    @param check: Parameter to check if doCheck is enabled
    @return If doCheck is enabled, returns the result*/
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
    if (doCheck){
        if (check == *p){
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
    /**Gets the data at given coordinates from the matrix map*/
    if (x>n || x<0 || y>n || y<0)
        return 0; //If edges, return 0 because edges count as walls
    return map[x][y];
}

int checkAround(char map[X][X], int n, int x, int y, int valueToCheck)
{
    /**Checks around a point in matrix map for given value and counts the number of instances of it*/
    int sum = 0;
    if (getPoint(map, x-1, y, n)==valueToCheck) sum++;
    if (getPoint(map, x, y-1, n)==valueToCheck) sum++;
    if (getPoint(map, x, y+1, n)==valueToCheck) sum++;
    if (getPoint(map, x+1, y, n)==valueToCheck) sum++;
    return sum;
}

void findPoint(int *p, char map[X][X], int n, int search)
{
    /**Finds a specific data written on the map and writes the coordinates to a given pointer.
    @param p: Given pointer to write p[0] (x) and p[1] (y) for
    @param search: Point to search. Returns after the first one found. Use accordingly*/
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            if (map[i][j] == search)
                {
                    *p = i;
                    *(p+1) = j;
                    return;
                }
        }
    }
}

struct node* navigateTo(struct node* p, int x, int y)
{
    /**Navigates the starting pointer to a given coordinate on the map. Works like cursor. Returns p to avoid bugs (pointer losing track).*/
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

void clearScreen()
{
    /**Clears the cmd screen.*/
    system("cls");
}

int main()
{
    /* INITIALIZATION */

    system("echo Initializing...\nIf you are seeing this, your computer might need an upgrade! :)");
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);                                                     //Setting up the console handler for enabling formatting of the cmd screen.
    SetConsoleTextAttribute(hConsole, defaultColor);                                                //Sets the color of the cmd text to default set by global value above.
    system("chcp 65001");                                                                           //Enabling unicode to be able to print unicode characters on cmd.
    srand(time(NULL));                                                                              //Generates random seed from the current time.
    char map[X][X] = {0};                                                                           //Creates a matrix map and pointer map and sets their values to default.
    struct node* pointerMap[X][X] = {NULL};
    clearScreen();
    /* GET SIZE OF THE MAP */
    system("COLOR F");
    printf("Please, enter the dimension you would like to generate the map for: ");
    int n = getNumber(X);                                                                           //getNumber handles integer inputs, so initializing an integer while calling it, sets the value immediately.
    generateRandomMap(map,n,1,10);                                                                  //Function to generate and smooth the randomized map.
    clearScreen();
    drawMap(map,n);

    /* GET ENTRANCE AND EXIT */

    do{ //ENTRANCE
        printf("\nPick the entrance:\n1 = Up\t2 = Right\t3 = Down\t4 = Left\nSide: ");
        int side = getNumber(4);
        printf("Point on side?: ");
        int point = getNumber(n);
        int check = writeMapBySide(side, point, map, n, 2, 1, 1);
        if (check) break;
        printf("\nERROR: Wrong input! That point is not a valid target for an entrance!");
    }while(1);
    clearScreen();
    drawMap(map,n);
    do{ //EXIT
        printf("\nPick the exit:\n1 = Up\t2 = Right\t3 = Down\t4 = Left\nSide: ");
        int side = getNumber(4);
        printf("Point on side?: ");
        int point = getNumber(n);
        int check = writeMapBySide(side, point, map, n, 3, 1, 1);
        if (check) break;
        printf("\nERROR: Wrong input! That point is not a valid target for an exit!");
    }while(1);
    clearScreen();
    drawMap(map,n);

    /* START ITERATING THROUGH THE MAP */

    int getPoint[2], isExitFound=0;                         //getPoint is needed to find starting point and write to it, isExitFound is used to detect if the exit has been found or not.
    findPoint(getPoint,map,n,2);                            //Finds the starting point coordinates on the map.
    convert(map,pointerMap,n);                              //Map conversion to linked list type map.
    struct node* start = pointerMap[0][0];                  //Sets a pointer starting from the top left corner of the map.
    start = navigateTo(start, getPoint[0], getPoint[1]);    //Navigates starting pointer to find the starting point on the pointer map.
    clearScreen();
    if (findPath(start))                                    //Starts the recursive function, iterating through the map using operating system's own stack structure.
        isExitFound = 1;                                    //If it returns TRUE, that means the stack has found an exit and returned back to it's root as TRUE.
    drawPointerMap(&pointerMap,n);                          //Another draw function to input a pointer map and draw on the cmd window.
    if (isExitFound)
        printf("\nExit has been found!!!");
    else
        printf("\nExit cannot be found!!!");
    return 0;
}
