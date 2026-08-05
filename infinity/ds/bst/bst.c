#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include "bst.h"

/*------------------------MACRO---------------------------*/

/*---------------FUNCTION DECLERATION---------------------*/
/* static BST_iter_ty RemoveALlLeft(BST_iter_ty iter); */
static BST_iter_ty InsertByDirection(BST_ty *tree, int direction, BST_iter_ty new_tree_node, BST_iter_ty where) ;
static BST_iter_ty FindWhereToInsert(BST_ty *tree, void *data, int *direction);
static BST_iter_ty NodeCreat(void *data);
static void UpdateBeginByData(BST_ty *tree, void *data, BST_iter_ty where_to_insert);
static int GetNumOfSons(BST_iter_ty iter);
static void RemoveWithNoSons(BST_iter_ty iter);
static void RemoveWithLeftSon(BST_iter_ty iter);
static void RemoveWithRightSon(BST_iter_ty iter);
static void RemoveWithTwoSons(BST_iter_ty iter);

/*-----------------------TYPEDEF--------------------------*/
#define TWO_SONS (3)
#define NO_SONS (0)
/*-----------------------STRUCTS--------------------------*/

enum raltives {
    PARENT,
    LEFT,
    RIGHT,
    NUM_OF_RELATIVES
};
typedef struct tree_node
{
    void *data;
    struct tree_node *relatives[NUM_OF_RELATIVES];
} tree_node_t;

struct binary_search_tree
{
    BST_cmp_func_ty compare;
    void *params;
    bst_node_ty dummy_root;/* the left relative will point to the root; right will always be NULL; parent will always point to NULL*/
};

/*--------------------------------------------------------*/

BST_ty *BSTCreate(BST_cmp_func_ty compare, void *params)
{
    BST_ty *new_bst = NULL;
    
    assert(NULL != compare);
    new_bst = (BST_ty *)calloc(sizeof(struct binary_search_tree),1);
    if (NULL == new_bst)
    {
        return NULL;
    }

    new_bst->compare = compare;
    new_bst->params = params;
    /* return InitBst(new_bst); memset*/ 
    return new_bst;
}

/*--------------------------------------------------------*/

void BSTDestroy(BST_ty *tree)
{
    tree_node_t *runner = NULL; /* use the other functions*/ 
    tree_node_t *next_to_runner=NULL;
    assert(NULL != tree);

    runner = BSTBegin(tree);
    
    while(!BSTIsEmpty(tree))
    {
        next_to_runner = BSTIterNext(runner);
        BSTRemove(runner);
        runner = next_to_runner;
    }
    /* while(!BSTIsEmpty(tree))
    {
        runner = RemoveALlLeft(runner);
        if(NULL != runner->relatives[PARENT] && NULL != runner->relatives[RIGHT])
        {
             runner = runner->relatives[RIGHT];
        }
    } */
    free(tree);
    tree = NULL;
}

/*--------------------------------------------------------*/

BST_iter_ty BSTSearch(BST_ty *tree, void *data_to_match)
{
    BST_iter_ty runner = NULL;
    assert(NULL != tree);

    runner = BSTBegin(tree);
    while(BSTEnd(tree) != runner)
    {
        if(tree->compare(BSTGetData(runner),data_to_match,NULL) == 0)
        {
            return runner;
        }
        runner = BSTIterNext(runner);
    }
    return runner;
}
/*--------------------------------------------------------*/

BST_iter_ty BSTInsert(BST_ty *tree, void *data)
{
    BST_iter_ty new_tree_node = NULL;
    BST_iter_ty where_to_insert = NULL;
    int direction = 0;
    
    assert(NULL != tree);
    
    new_tree_node = NodeCreat(data);
    if (NULL == new_tree_node)
    {
        return BSTEnd(tree);
    }
    where_to_insert = FindWhereToInsert(tree, data, &direction);
    where_to_insert = InsertByDirection(tree, direction, new_tree_node, where_to_insert);
    UpdateBeginByData(tree, data, where_to_insert);

    return new_tree_node;
}
static void UpdateBeginByData(BST_ty *tree, void *data, BST_iter_ty where_to_insert)
{
    BST_iter_ty node_to_check = (BST_iter_ty)tree->dummy_root.data;
    if (0 < tree->compare(node_to_check->data , data, tree->params))
    {
        tree->dummy_root.data = (void*)where_to_insert;
    }
}

static BST_iter_ty FindWhereToInsert(BST_ty *tree, void *data, int *direction)
{
    BST_iter_ty runner = runner = tree->dummy_root.relatives[LEFT];
    BST_iter_ty before_runner = NULL;

    while (runner != NULL)
    {
        before_runner = runner;
        if (tree->compare(runner->data, data, tree->params) > 0)
        {
            runner = runner->relatives[LEFT];
            *direction = LEFT;
        }
        else
        {
            runner = runner->relatives[RIGHT];
            *direction = RIGHT;
        }
    }
    return before_runner;
}
static BST_iter_ty InsertByDirection(BST_ty *tree, int direction, BST_iter_ty new_tree_node, BST_iter_ty where)
{
    switch (direction) /*insert by direction function */
    {
    case 0: /*this case is when this is the first element*/
        tree->dummy_root.relatives[LEFT] = new_tree_node;
        new_tree_node->relatives[PARENT] = &tree->dummy_root;
        tree->dummy_root.data = (void*)new_tree_node;
        break;
    case LEFT: /*in this case the node will be added in the left*/
        where->relatives[LEFT] = new_tree_node;
        new_tree_node->relatives[PARENT] = where;

        break;
    case RIGHT: /*in this case the node will be added in the right*/
        where->relatives[RIGHT] = new_tree_node;
        new_tree_node->relatives[PARENT] = where;
        break;
    }

    return new_tree_node;
}
static BST_iter_ty NodeCreat(void *data)
{
    BST_iter_ty new_tree_node = NULL;
    int index = 0;

    new_tree_node = (tree_node_t *)malloc(sizeof(tree_node_t));
    if (NULL == new_tree_node)
    {
        return NULL;
    }

    new_tree_node->data = data;
    while (NUM_OF_RELATIVES > index)
    {
        new_tree_node->relatives[index] = NULL;
        ++index;
    }

    return new_tree_node;
   
}

/*--------------------------------------------------------*/

void *BSTRemove(BST_iter_ty iter)
{
    void *data_to_return = NULL;
    int sons_num = GetNumOfSons(iter);
    

    assert(NULL != iter->relatives[PARENT]);
    data_to_return = BSTGetData(iter);

    switch (sons_num)
    {
    case NO_SONS:
        RemoveWithNoSons(iter);
        break;
    case LEFT:
        RemoveWithLeftSon(iter);
        break;
    case RIGHT:
        RemoveWithRightSon(iter);
        break;
    case TWO_SONS:
        RemoveWithTwoSons(iter);
        break;
    }

    return data_to_return;
}
static void RemoveWithNoSons(BST_iter_ty iter)
{
    if (iter == iter->relatives[PARENT]->relatives[LEFT])
    {
        iter->relatives[PARENT]->relatives[LEFT] = NULL;
    }
    else
    {
        iter->relatives[PARENT]->relatives[RIGHT] = NULL;
    }
    free(iter);
    iter = NULL;
}
static void RemoveWithLeftSon(BST_iter_ty iter)
{
    iter->relatives[LEFT]->relatives[PARENT] = iter->relatives[PARENT];
    if (iter == iter->relatives[PARENT]->relatives[LEFT])
    {
        iter->relatives[PARENT]->relatives[LEFT] = iter->relatives[LEFT];
    }
    else
    {
        iter->relatives[PARENT]->relatives[RIGHT] = iter->relatives[LEFT];
    }
    free(iter);
    iter = NULL;
}
static void RemoveWithRightSon(BST_iter_ty iter)
{
    iter->relatives[RIGHT]->relatives[PARENT] = iter->relatives[PARENT];

    if (iter == iter->relatives[PARENT]->relatives[RIGHT])
    {
        iter->relatives[PARENT]->relatives[RIGHT] = iter->relatives[RIGHT];
    }
    else
    {
        iter->relatives[PARENT]->relatives[LEFT] = iter->relatives[RIGHT];
    }
    free(iter);
    iter = NULL;
}
static void RemoveWithTwoSons(BST_iter_ty iter)
{
    BST_iter_ty next_iter = NULL;

    next_iter = BSTIterNext(iter);
    next_iter->relatives[LEFT] = iter->relatives[LEFT];

    iter->relatives[LEFT]->relatives[PARENT] = next_iter;
    iter->relatives[LEFT] = NULL;

    iter->relatives[RIGHT]->relatives[PARENT] = iter->relatives[PARENT];

    if (iter == iter->relatives[PARENT]->relatives[RIGHT])
    {
        iter->relatives[PARENT]->relatives[RIGHT] = iter->relatives[RIGHT];
    }
    else
    {
        iter->relatives[PARENT]->relatives[LEFT] = iter->relatives[RIGHT];
    }

    free(iter);
    iter = NULL;


}
static int GetNumOfSons(BST_iter_ty iter) /*return: 0) no sons. 1) if just left son. 2)just right son. 3)tow sons. */
{
    int count = 0;
        if(iter->relatives[LEFT] != NULL)
        {  
            ++count;
        }
        if (iter->relatives[RIGHT] != NULL)
        {
            count += 2;
        }
    
    return count;
}
/*--------------------------------------------------------*/

    int BSTIsEmpty(const BST_ty *tree)
{
    assert(NULL != tree);
    return NULL == tree->dummy_root.relatives[LEFT];
}
/*--------------------------------------------------------*/

size_t BSTSize(const BST_ty *tree)
{
    BST_iter_ty runner = NULL;
    size_t count = 0;
    assert(NULL != tree);

    runner = BSTBegin(tree);
    while (BSTEnd(tree) != runner)
    {
        ++count;
        runner = BSTIterNext(runner);
    }
    return count;
}
/*--------------------------------------------------------*/

int BSTForEach(BST_iter_ty from, BST_iter_ty to, BST_action_func_ty operation, void  *param)
{
    BST_iter_ty runner = NULL;
    int status = 0;
    assert(NULL != from);
    assert(NULL != to);
    assert(NULL != operation);

    runner = from;
    while (runner != to && !status)
    {
        status = operation(BSTGetData(runner),param);
        runner = BSTIterNext(runner);
    }
    return status;
}
/*--------------------------------------------------------*/

BST_iter_ty BSTIterNext(BST_iter_ty iter)
{
    tree_node_t *runner = NULL;
    assert(NULL != iter);

    runner = iter;

    if(NULL == iter->relatives[RIGHT])
    {
        while(runner == runner->relatives[PARENT]->relatives[RIGHT])
        {
            runner = runner->relatives[PARENT];
        }
        return runner->relatives[PARENT];
    }
   
    runner = runner->relatives[RIGHT];
    while (NULL != runner->relatives[LEFT])
    {
        runner = runner->relatives[LEFT];
    }
    return runner;
        
    
}
/*--------------------------------------------------------*/

BST_iter_ty BSTIterPrev(BST_iter_ty iter)
{
    tree_node_t *runner = NULL;
    assert(NULL != iter);

    runner = iter;

    if (NULL == iter->relatives[LEFT])
    {
        while (runner == runner->relatives[PARENT]->relatives[LEFT] && runner->relatives[PARENT] != NULL)
        {
            runner = runner->relatives[PARENT];
        }
        if (NULL == runner)
        {
            return iter;
        }
       
        return runner->relatives[PARENT];
    }
    
    runner = runner->relatives[LEFT];
    while (NULL != runner->relatives[RIGHT])
    {
        runner = runner->relatives[RIGHT];
    }
    return runner;
    
}
/*--------------------------------------------------------*/

BST_iter_ty BSTBegin(const BST_ty *tree)
{
    assert(NULL != tree);
    return (BST_iter_ty)tree->dummy_root.data;

} 
/*--------------------------------------------------------*/

BST_iter_ty BSTEnd(const BST_ty *tree)
{
    assert(NULL != tree);
    return (BST_iter_ty)&tree->dummy_root;
}
/*--------------------------------------------------------*/

void *BSTGetData(BST_iter_ty iter)
{
    assert(NULL != iter);
    return iter->data;
}
/*--------------------------------------------------------*/

int BSTIsSameIter(BST_iter_ty iter1, BST_iter_ty iter2)
{
    return iter1 == iter2;
}
/*--------------------------------------------------------*/
