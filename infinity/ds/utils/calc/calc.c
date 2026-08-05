#include <math.h> /* pow */
#include <stdlib.h> /* malloc*/
#include <stdio.h>

#include "calc.h"
#include "../parser/parser.h"
#include "stack.h"

/*------------------------MACRO---------------------------*/

#define NOT_CORRECT_OPERATOR (0)
#define LUT_LENGTH (256)


/*-----------------------STRUCTS--------------------------*/

typedef struct calc
{
    stack_t *num_stack;
    stack_t *op_stack;
    char *math_exp;
    int curren_state;
    int parantheses_counter;
}calc_t;


typedef double (*operator_func_ptr_t)(double num1, double num2,int *exit_status);
typedef double (*state_func_t)(calc_t *calc, int *exit_status);



enum state
{
    WAIT_FOR_NUM = 0,
    WAIT_FOR_OP
};
/*---------------------  GLOBALS  ------------------------*/

static char operators_lut[LUT_LENGTH][2] = {NOT_CORRECT_OPERATOR};
static state_func_t main_func_lut[LUT_LENGTH] = {NULL};
static operator_func_ptr_t func_lut[LUT_LENGTH] = {NULL};
static state_func_t state_lut[2];


    /*---------------FUNCTION DECLERATION---------------------*/

static double Power(double num1, double num2, int *exit_status);
static double Divide(double num1, double num2, int *exit_status);
static double Multiply(double num1, double num2, int *exit_status);
static double Minus(double num1, double num2, int *exit_status);
static double Plus(double num1, double num2, int *exit_status);
static void CalcDestroy(calc_t *calc);
static void CalcInit(calc_t *calc, char *math_exp);
static void LutInit();
static double WaitForOp(calc_t *calc, int *exit_status);
static double WaitForNum(calc_t *calc, int *exit_status);
static int GetOperatorPriority(char operator);
static double CalcOperation(calc_t *calc, int *exit_status);
static int TwoPowerInRow(calc_t *calc, char oper);
static double EndOfEx(calc_t *calc, int *exit_status);
static double NotCorrectOperator(calc_t *calc, int *exit_status);
static double CorrectOperator(calc_t *calc, int *exit_status);
static double Parantheses(calc_t *calc, int *exit_status);
double round(double x);
/*--------------------------------------------------------*/

double Calculate(char *math_exp, int *exit_status)
{
    calc_t *new_calc = (calc_t *)malloc(sizeof(calc_t));
    double res = 0;
    static int init_flag = 1;

    CalcInit(new_calc, math_exp);
    if(1 ==  init_flag)
    {
        LutInit(new_calc);
        init_flag = 0;
    }
    while ('\0' != *new_calc->math_exp)
    {
        state_lut[new_calc->curren_state](new_calc, exit_status);

        if(*exit_status != SUCCESS)
        {
            CalcDestroy(new_calc);
            return 0;
        }
       
    }
    if(new_calc->parantheses_counter != 0)
    {
        *exit_status = SYNTAX_ERROR;
        CalcDestroy(new_calc);
        return 0;
    }
    res = state_lut[new_calc->curren_state](new_calc, exit_status);
    CalcDestroy(new_calc);
    return res;
}
static void CalcInit(calc_t *calc, char *math_exp)
{
    calc->curren_state = WAIT_FOR_NUM;
    calc->num_stack = StackCreate(100);
    calc->op_stack = StackCreate(1000);
    calc->math_exp = math_exp;
    calc->parantheses_counter = 0;
    StackPush(calc->op_stack, (void *)'#');
}



static void LutInit()
{
    int i = 0;
    

    state_lut[WAIT_FOR_NUM] = WaitForNum;
    state_lut[WAIT_FOR_OP] = WaitForOp;

    
    operators_lut['+'][0] = '+';
    operators_lut['-'][0] = '-';
    operators_lut['*'][0] = '*';
    operators_lut['/'][0] = '/';
    operators_lut['^'][0] = '^';
    operators_lut[')'][0] = ')';

    operators_lut['+'][1] = 1;
    operators_lut['-'][1] = 1;
    operators_lut['*'][1] = 2;
    operators_lut['/'][1] = 2;
    operators_lut['^'][1] = 3;
    operators_lut[')'][1] = '(';
    operators_lut[']'][1] = '[';
    operators_lut['}'][1] = '{';

    for (i = 0; i < LUT_LENGTH; ++i)
    {
        main_func_lut[i] = NotCorrectOperator;
    }
    main_func_lut['\0'] = EndOfEx;
    main_func_lut['+'] = CorrectOperator;
    main_func_lut['-'] = CorrectOperator;
    main_func_lut['*'] = CorrectOperator;
    main_func_lut['/'] = CorrectOperator;
    main_func_lut['^'] = CorrectOperator;
    main_func_lut[')'] = Parantheses;
    main_func_lut[']'] = Parantheses;
    main_func_lut['}'] = Parantheses;

    func_lut['+'] = Plus;
    func_lut['-'] = Minus;
    func_lut['*'] = Multiply;
    func_lut['/'] = Divide;
    func_lut['^'] = Power;
}

/*--------------------------------------------------------*/
static double WaitForNum(calc_t *calc, int *exit_status)
{
    double get_num_return = 0;
    char parantheses = 0;
    char *check_for_getnum = NULL;

    check_for_getnum = calc->math_exp;
    get_num_return = GetNum(calc->math_exp, &calc->math_exp, &parantheses);
    if (calc->math_exp == check_for_getnum || '\0' == *check_for_getnum )
    {
        *exit_status = SYNTAX_ERROR;
        return 0;
    }
    else
    {
        if ('(' == parantheses || '[' == parantheses || '{' == parantheses)
        {
            StackPush(calc->op_stack, *(void **)&parantheses);
            ++calc->parantheses_counter;
        }
        else
        {
            StackPush(calc->num_stack, *(void **)&get_num_return);
            calc->curren_state = WAIT_FOR_OP;
        }
    }
    
    *exit_status = SUCCESS;
    return 0;
}

static double WaitForOp(calc_t *calc, int *exit_status)
{
    char get_op_return = 0;

    double operand = 0;

    get_op_return = GetOp(calc->math_exp, &calc->math_exp);

    operand = main_func_lut[(int)get_op_return](calc, exit_status);
    if (*exit_status != SUCCESS)
    {
        return 0;
    }

    return operand;

}

static double Parantheses(calc_t *calc, int *exit_status)
{
    char operator = *(calc->math_exp - 1);
    char current_operator = (char)(long)StackPeek(calc->op_stack);
    double res = 0;
    void *peek_num = NULL;

    peek_num = StackPeek(calc->num_stack);
    res = *(double *)&peek_num;
    while ('[' != current_operator && 
           '{' != current_operator &&
           '(' != current_operator &&
           '#' != current_operator)
    {
        res = CalcOperation(calc, exit_status);
        current_operator = (char)(long)StackPeek(calc->op_stack);
    }

    if ('#' == current_operator || operators_lut[(int)operator][1] != current_operator)
    {
        *exit_status = SYNTAX_ERROR;
        return 0;
    }
    --calc->parantheses_counter;
    StackPop(calc->op_stack);
    return res;
}
static double NotCorrectOperator(calc_t *calc, int *exit_status)
{
    (void)calc;
    *exit_status = SYNTAX_ERROR;
    return 0;
}

static double CorrectOperator(calc_t *calc, int *exit_status)
{
    char operator = *(calc->math_exp - 1);
    if (1 == TwoPowerInRow(calc, operator) || GetOperatorPriority(operator) > GetOperatorPriority((char)(long)StackPeek(calc->op_stack)))
    {
        StackPush(calc->op_stack, *(void **)&operator);
        calc->curren_state = WAIT_FOR_NUM;
    }
    else
    {
        CalcOperation(calc, exit_status);
        --calc->math_exp;
    }
    return 0;
}

static double EndOfEx(calc_t *calc, int *exit_status)
{
    double operand = 0;
    void *peek_num = NULL;
    peek_num = StackPeek(calc->num_stack);
    operand = *(double *)&peek_num;
    while ('#' != (char)(long)StackPeek(calc->op_stack))
    {
        operand = CalcOperation(calc, exit_status);
    }

    return operand;
}


static int TwoPowerInRow(calc_t *calc, char oper)
{
    if (oper == '^')
    {
        return (char)(long)StackPeek(calc->op_stack) == '^';
    }
    return 0;
}
static double CalcOperation(calc_t *calc, int *exit_status)
{
    double num1 = 0, num2 = 0;
    double operand = 0;
    void * peek_num = NULL;
    char operator = 0;

    peek_num = StackPeek(calc->num_stack);
    num2 = *(double *)&peek_num;
    StackPop(calc->num_stack);

    peek_num = StackPeek(calc->num_stack);
    num1 = *(double *)&peek_num;
    StackPop(calc->num_stack);
    operator = (int)(long)StackPeek(calc->op_stack);
    operand = func_lut[(int)operator](num1, num2, exit_status);
    StackPush(calc->num_stack, *(void **)&operand);
    StackPop(calc->op_stack);
    return operand;
}
static int GetOperatorPriority(char operator)
{
    return operators_lut[(int)operator][1];

}


static void CalcDestroy(calc_t *calc)
{
   
    StackDestroy(calc->num_stack);
    StackDestroy(calc->op_stack);

    free(calc);
    calc = NULL;
}

static double Plus(double num1, double num2, int *exit_status)
{
    (void)exit_status;
    return num1 + num2;
}
static double Minus(double num1, double num2, int *exit_status)
{
    (void)exit_status;
    return num1 - num2;
}
static double Multiply(double num1, double num2, int *exit_status)
{
    (void)exit_status;
    return num1 * num2;
}
static double Divide(double num1, double num2, int *exit_status)
{
    if(0 != num2)
    {
        return num1 / num2;
    }
    *exit_status = MATH_ERROR;
    return 0;

}
static double Power(double num1, double num2, int *exit_status)
{
    if((num1 == 0 && num2 <= 0) || (num1 < 0 && round(num2) != num2))
    {
        
        *exit_status = MATH_ERROR;
        return 0;
    }
    return pow(num1,num2);
}