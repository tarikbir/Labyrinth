#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
/* GLOBAL DEFINES */
#define MAX 40
#define X 20

char moves = {0 , 1 , 2 , 3};
/*  0 = Up
    1 = Right
    2 = Down
    3 = Left */

struct move{
    char nextMove;
    struct move *prev;
    struct move *next;
};

struct stack{
    struct move val;
    struct stack *next;
};

void drawMap(char map[X][X])
{
    /**Draws the map onto the cmd screen*/
    for (int i=0;i<3*X;i++) {printf("-");}
    printf("---\n");
    for (int i=0;i<=X;i++) {printf("%2d ",i);}
    printf("\n");
    for (int i=0;i<X;i++)
    {
        printf("%2d ",i+1);
        for (int j=0;j<X;j++)
        {
            if (map[i][j] == 1) //Labirent yolu
                printf("[+]");
            else if (map[i][j] == 0) //Labirent duvarý
                printf("   ");
            else if (map[i][j] == 2) //Labirent baþlangýç noktasý
                printf("<o>");
            else if (map[i][j] == 3) //Labirent bitiþ noktasý
                printf("/!\\");
        }
        printf("|\n");
    }
    for (int i=0;i<3*X;i++) {printf("-");}
    printf("---");
}

void generateRandomMap(char map[X][X])
{
    /**Generates a random map between 0 and 1*/
    short int chance = 33; // Normalde %50 ihtimalle dizilen sayýlarý yine 2 elemana yakýnsa %50 ihtimalle deðilse daha düþük bir ihtimalle yeniden dizecek yumuþatma algoritmasýnýn þans deðeri.
    /* FULL RANDOM */
    for (int i=0;i<X;i++)
    {
        for (int j=0;j<X;j++)
        {
            map[i][j] = rand()%2;
        }
    }
    /* SMOOTHING THE FIRST FULL RANDOM */
    for (int i=0;i<X;i++)
    {
        for (int j=0;j<X;j++)
        {
            if (i-1>=0 && j-1>=0 && i+1<X && j+1<X)
            {
                char amount_of_adjacent_available_tiles = 0;
                if(map[i-1][j]) amount_of_adjacent_available_tiles++; //Üstte
                if(map[i][j-1]) amount_of_adjacent_available_tiles++; //Solda
                if(map[i][j+1]) amount_of_adjacent_available_tiles++; //Saðda
                if(map[i+1][j]) amount_of_adjacent_available_tiles++; //Altta
                char inner_chance = 2-abs(2-amount_of_adjacent_available_tiles); // 2-|2-x| kýrýlmalý lineer fonksiyon grafiði denklemi ile 2'de maksimum hesaplatýyor
                if (chance*inner_chance > (rand()%101))
                    map[i][j] = 1;
                else
                    map[i][j] = 0;
            }
        }
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
        if ( integerToReturn[0] == 0 )
            return 0;
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
    srand(time(NULL));
    char map[X][X] = {0};
    char naber[] = "NABER KANKA NASILSIN";
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
    struct move *start;

    return 0;
}
