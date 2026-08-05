#include <stdlib.h>
#include <unistd.h> /*sleep*/
#include <unistd.h>
#include <time.h> /*time, clock*/
#include <stdio.h>


#include "vector.h"
#include "bit_array.h"

/*-------      macro        ---------*/

#define ALL_BITS_ON (0xFFFFFFFFFFFFFFFF)


/* ------  function decleration ------*/ 

void InitPossibleMoves();
bit_array_t WalkThrowChessBackTracking(Vector_t *vector, bit_array_t is_visited, int index);
bit_array_t WalkThrowChessWarnsdorff(Vector_t *vector, bit_array_t is_visited, int index);

void PrintThePath(Vector_t *vector);
void PrintTheProgress(Vector_t *vector, int flag);
int IsFound(int arr[], int num);

/*----------- global lut --------------*/
static int possible_moves[9][64] = {0};

void InitPossibleMoves()
{
   
    static int moves[3][8] = {{-17, -10, 6, 15, 17, 10, -6, -15},
                              {-2, -1, 1, 2, 2, 1, -1, -2},
                              {-1, -2, -2, -1, 1, 2, 2, 1}};

    int location = 0, index = 0;
    while (location < 64)
    {
        index = 0;
        while (index < 8)
        {
            if (location / 8 + moves[1][index] >= 0 && location / 8 + moves[1][index] < 8)
            {
                if (location % 8 + moves[2][index] >= 0 && location % 8 + moves[2][index] < 8)
                {
                    possible_moves[index][location] = moves[0][index];
                    ++possible_moves[8][location];
                }
            }
            ++index;
        }
        ++location;
    }

}

/*---------- main function -------------*/

int main()
{
    Vector_t *vector = VectorCreate(10);
    Vector_t *vector2 = VectorCreate(10);

    bit_array_t is_visited = 0;
    clock_t start, end;
    InitPossibleMoves();

    WalkThrowChessWarnsdorff(vector, is_visited, 63);

    start = clock();
    WalkThrowChessBackTracking(vector2, is_visited, 63);
    end = clock();
    PrintTheProgress(vector,0);
    PrintTheProgress(vector2,1);
    printf("\ntime for BackTracking: %ld", end - start);
    /* PrintThePath(vector); */
    VectorDestroy(vector);
    VectorDestroy(vector2);

    return 0;
}

/*------- function definenitions -----------*/

void PrintTheProgress(Vector_t *vector, int flag)
{
    size_t i = 0, j = 0;
    int arr[64];
    void * res = NULL;
  
    for (j = 0; j < 64; ++j)
        arr[j] = -1;

    system("setterm -cursor off");
    while (i < 64)
    {
        system("clear");
        res = VectorGetElement(vector, i + 1);
        if (flag == 1)
        {
            printf("backtracking\n");
        }
        else
        {
            printf("warnsdorff’s algorithm\n");
        }
        j = 0;
        arr[i] = *(int *)&res;
        while (j < 64)
        {

            if (j != 0 && j % 8 == 0)
            {
                printf("\n");
            }

            if (1 == IsFound(arr, j))
            {
                if(i == 63 )
                {
                    printf("\033[1;31m%02ld \033[0m", j);
                }
                else
                {
                    printf("\033[1;32m%02ld \033[0m", j);
                }
                
   
            }
            else
            {
                printf("\033[0;30m%02ld \033[0m", j);
            }
            ++j;
        }
        printf("\n");
        
        sleep(1);

        ++i;
    }


    
}

int IsFound(int arr[], int num)
{
    int i = 0;
    for(; i < 64; ++i)
    {
        if(arr[i] == num)
        {
            return 1;
        }
    }
    return 0;
}

void PrintThePath(Vector_t *vector)
{
    size_t i = 0;
    void *res = NULL;
    res = VectorGetElement(vector, i + 1);
    ++i;
    while (i < 64)
    {
        printf("%d->", *(int *)&res);
        res = VectorGetElement(vector, i + 1);
        printf("%d\n", *(int *)&res);
        ++i;
    }
}

bit_array_t WalkThrowChessBackTracking(Vector_t *vector, bit_array_t is_visited, int index)
{
    int i = 0, next_move = 0;
    is_visited = BitsArrSetBit(is_visited, index + 1, 1);
    VectorPushBack(vector, *(void **)&index);
    if (ALL_BITS_ON == is_visited)
    {
        return ALL_BITS_ON;
    }

    /*if you want to manually sit the order of the backtracking traverse: 
                 0 	 1	 2	3	4	5	 6	 7
                -17	-10	 6	15	17	10	-6	-15

                	-17		-15	
                -10				-6
				        C
                 6				10
	                15		17	

    */
    /*
     next_move = possible_moves[0][index];
    if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
    {
        if (ALL_BITS_ON == WalkThrowChessBackTracking(vector, is_visited, index + next_move))
        {
            return ALL_BITS_ON;
        }
    }
    next_move = possible_moves[1][index];
    if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
    {
        if (ALL_BITS_ON == WalkThrowChessBackTracking(vector, is_visited, index + next_move))
        {
            return ALL_BITS_ON;
        }
    }
    next_move = possible_moves[2][index];
    if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
    {
        if (ALL_BITS_ON == WalkThrowChessBackTracking(vector, is_visited, index + next_move))
        {
            return ALL_BITS_ON;
        }
    }
    next_move = possible_moves[3][index];
    if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
    {
        if (ALL_BITS_ON == WalkThrowChessBackTracking(vector, is_visited, index + next_move))
        {
            return ALL_BITS_ON;
        }
    }
    next_move = possible_moves[4][index];
    if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
    {
        if (ALL_BITS_ON == WalkThrowChessBackTracking(vector, is_visited, index + next_move))
        {
            return ALL_BITS_ON;
        }
    }
    next_move = possible_moves[5][index];
    if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
    {
        if (ALL_BITS_ON == WalkThrowChessBackTracking(vector, is_visited, index + next_move))
        {
            return ALL_BITS_ON;
        }
    }
    next_move = possible_moves[6][index];
    if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
    {
        if (ALL_BITS_ON == WalkThrowChessBackTracking(vector, is_visited, index + next_move))
        {
            return ALL_BITS_ON;
        }
    }
    next_move = possible_moves[7][index];
    if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
    {
        if (ALL_BITS_ON == WalkThrowChessBackTracking(vector, is_visited, index + next_move))
        {
            return ALL_BITS_ON;
        }
    } */
    while (i < 8)
    {
        next_move = possible_moves[i][index];
        if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
        {
            if (ALL_BITS_ON == WalkThrowChessBackTracking(vector, is_visited, index + next_move))
            {
                return ALL_BITS_ON;
            }            
        }
        ++i; 
    }
    
    VectorPopBack(vector);

    return 0;
}

bit_array_t WalkThrowChessWarnsdorff(Vector_t *vector, bit_array_t is_visited, int index)
{
    int i = 0, next_move = 0;
    int min = 9;
    int min_index = 0;
    is_visited = BitsArrSetBit(is_visited, index + 1, 1);
    VectorPushBack(vector, *(void **)&index);
    if (ALL_BITS_ON == is_visited)
    {
        return ALL_BITS_ON;
    }
    while (i < 8)
    {
        next_move = possible_moves[i][index];
        if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
        {
            if (min > possible_moves[8][index + next_move]--)
            {
                min = possible_moves[8][index + next_move];
                min_index = next_move;
            }
        }
        ++i;
    }
    WalkThrowChessWarnsdorff(vector, is_visited, index + min_index);
    return 0;
}
