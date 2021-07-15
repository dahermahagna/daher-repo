#include <stdlib.h>
#include <unistd.h> /*sleep*/
#include <unistd.h>
#include <time.h> /*time, clock*/
#include <stdio.h>


#include "../dynamic_vector/vector.h"
#include "../bit_array/bit_array.h"
#include "defs.h"
/*-------      macro        ---------*/

/* ------  function decleration ------*/ 

static void InitPossibleMoves();
static bit_array_t WalkThrowChessBackTracking(Vector_t *vector, bit_array_t is_visited, int index);
static bit_array_t WalkThrowChessWarnsdorff(Vector_t *vector, bit_array_t is_visited, int index);

static void PrintThePath(Vector_t *vector);
static void PrintTheProgress(Vector_t *vector, int flag);
static int IsFound(int arr[], int num);

/*----------- global lut --------------*/
static int possible_moves_idx_lut[MAX_POS_MOVE + 1][BOARD_SIZE] = {{0}};

static void InitPossibleMoves()
{

    static int moves[3][MAX_POS_MOVE] = {{-17, -10, 6, 15, 17, 10, -6, -15},
                                         {-2, -1,   1, 2,  2,  1,  -1, -2},
                                         {-1, -2,  -2,-1,  1,  2,   2,  1}};

    int cur_pos_idx = 0, move_index = 0;
    while (BOARD_SIZE > cur_pos_idx)
    {
        move_index = 0;
        while (MAX_POS_MOVE > move_index)
        {
            if (LEGIT_ROW(move_index))
            {
                if (LEGIT_COL(move_index))
                {
                    possible_moves_idx_lut[move_index][cur_pos_idx] = moves[0][move_index];
                    ++possible_moves_idx_lut[8][cur_pos_idx];
                }
            }
            ++move_index;
        }
        ++cur_pos_idx;
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
#ifdef REG
PRINT_WITHOUT_COLOR
    PrintThePath(vector);
    printf("--------------------------\n");
    PrintThePath(vector2);   
#else
PRINT_WITH_COLOR
PrintTheProgress(vector, 0);
PrintTheProgress(vector2, 1);
#endif
    printf("\ntime for BackTracking: %ld", end - start);
    VectorDestroy(vector);
    VectorDestroy(vector2);

    return 0;
}

/*------- function definenitions -----------*/

static void PrintTheProgress(Vector_t *vector, int flag)
{
    size_t i = 0, j = 0;
    int arr[BOARD_SIZE];
    void * res = NULL;

    for (j = 0; BOARD_SIZE > j; ++j)
    {
        arr[j] = -1;
    }

    system("setterm -cursor off");
    while (BOARD_SIZE > i)
    {
        system("clear");
        res = VectorGetElement(vector, i + 1);
        if (1 == flag)
        {
            printf("backtracking\n");
        }
        else
        {
            printf("warnsdorff’s algorithm\n");
        }
        j = 0;
        arr[i] = *(int *)&res;
        while (BOARD_SIZE > j)
        {

            if (0 != j && 0 == j % 8)
            {
                printf("\n");
            }

            if (1 == IsFound(arr, j))
            {
                if (i == BOARD_SIZE - 1)
                {
                    printf("\033[1;31m%02lu \033[0m", j);
                }
                else
                {
                    printf("\033[1;32m%02lu \033[0m", j);
                }
                
   
            }
            else
            {
                printf("\033[0;30m%02lu \033[0m", j);
            }
            ++j;
        }
        printf("\n");
        
        sleep(1);

        ++i;
    }
    
}

static int IsFound(int arr[], int num)
{
    int i = 0;
    for (; i < BOARD_SIZE; ++i)
    {
        if(arr[i] == num)
        {
            return 1;
        }
    }

    return 0;
}

static void PrintThePath(Vector_t *vector)
{
    size_t i = 0;
    void *res = NULL;
    res = VectorGetElement(vector, i + 1);
    ++i;

    while (BOARD_SIZE > i)
    {
        printf("%d->", *(int *)&res);
        res = VectorGetElement(vector, i + 1);
        printf("%d\n", *(int *)&res);
        ++i;
    }
}

static bit_array_t WalkThrowChessBackTracking(Vector_t *vector, bit_array_t is_visited, int index)
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
     next_move = possible_moves_idx_lut[0][index];
    if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
    {
        if (ALL_BITS_ON == WalkThrowChessBackTracking(vector, is_visited, index + next_move))
        {
            return ALL_BITS_ON;
        }
    }
    next_move = possible_moves_idx_lut[1][index];
    if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
    {
        if (ALL_BITS_ON == WalkThrowChessBackTracking(vector, is_visited, index + next_move))
        {
            return ALL_BITS_ON;
        }
    }
    next_move = possible_moves_idx_lut[2][index];
    if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
    {
        if (ALL_BITS_ON == WalkThrowChessBackTracking(vector, is_visited, index + next_move))
        {
            return ALL_BITS_ON;
        }
    }
    next_move = possible_moves_idx_lut[3][index];
    if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
    {
        if (ALL_BITS_ON == WalkThrowChessBackTracking(vector, is_visited, index + next_move))
        {
            return ALL_BITS_ON;
        }
    }
    next_move = possible_moves_idx_lut[4][index];
    if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
    {
        if (ALL_BITS_ON == WalkThrowChessBackTracking(vector, is_visited, index + next_move))
        {
            return ALL_BITS_ON;
        }
    }
    next_move = possible_moves_idx_lut[5][index];
    if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
    {
        if (ALL_BITS_ON == WalkThrowChessBackTracking(vector, is_visited, index + next_move))
        {
            return ALL_BITS_ON;
        }
    }
    next_move = possible_moves_idx_lut[6][index];
    if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
    {
        if (ALL_BITS_ON == WalkThrowChessBackTracking(vector, is_visited, index + next_move))
        {
            return ALL_BITS_ON;
        }
    }
    next_move = possible_moves_idx_lut[7][index];
    if (next_move != 0 && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
    {
        if (ALL_BITS_ON == WalkThrowChessBackTracking(vector, is_visited, index + next_move))
        {
            return ALL_BITS_ON;
        }
    } */
    while (MAX_POS_MOVE > i)
    {
        next_move = possible_moves_idx_lut[i][index];
        if (0 != next_move && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
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

static bit_array_t WalkThrowChessWarnsdorff(Vector_t *vector, bit_array_t is_visited, int index)
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

    while (MAX_POS_MOVE > i)
    {
        next_move = possible_moves_idx_lut[i][index];
        if (0 != next_move && 0 == BitsArrGetVal(is_visited, index + next_move + 1))
        {
            if (min > possible_moves_idx_lut[8][index + next_move]--)
            {
                min = possible_moves_idx_lut[8][index + next_move];
                min_index = next_move;
            }
        }
        ++i;
    }

    if(0 == min_index)
    {
        return 0;
    }

    return WalkThrowChessWarnsdorff(vector, is_visited, index + min_index);
}
