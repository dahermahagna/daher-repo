#include <stdlib.h> /* malloc */
#include <string.h> /* memset */
#include <assert.h> /* assert */
#include <stdio.h>
#include "avl.h"

/*-----------------------STRUCTS--------------------------*/
#define ROOT_DIRECTION (2)
typedef enum relatives
{
    LEFT,
    RIGHT,
    NUM_OF_RELATIVES
} relatives;

typedef enum sons_num
{
    NO_SONS,
    ONE_LEFT_SON,
    ONE_RIGHT_SON,
    TWO_SONS
}sons_num;

typedef enum traverser
{
    PRE_ORDER = 1,
    In_ORDER,
    POST_ORDER
} traverser;
typedef struct tree_node
{
    void *data;
    struct tree_node *relatives[NUM_OF_RELATIVES];
    int height; /* not for first phase */
} AVL_node_ty;

struct rec_avl_bst
{
    AVL_cmp_func_ty compare;
    struct tree_node *root;
};
/*------------------------MACRO---------------------------*/

#define TREE_ROOT tree->root
#define MAX(x,y) x > y ? x : y

/*---------------FUNCTION DECLERATION---------------------*/
static AVL_node_ty *NodeCreat(void *data);
static void AVLDestroyByElement(AVL_node_ty *node);
static AVL_node_ty *FindToRemove(AVL_node_ty *node, void *data, AVL_cmp_func_ty compare, int *direction);
static int GetNumOfSons(AVL_node_ty *node);
static void RemoveWithNoSons(AVL_ty *tree ,AVL_node_ty *node_to_remove, AVL_node_ty *parent, int *direction);
static void RemoveWithLeftSon(AVL_ty *tree ,AVL_node_ty *node_to_remove, AVL_node_ty *parent, int *direction);
static void RemoveWithRightSon(AVL_ty *tree ,AVL_node_ty *node_to_remove, AVL_node_ty *parent, int *direction);
static void RemoveWithTwoSons(AVL_ty *tree ,AVL_node_ty *node_to_remove, AVL_node_ty *parent, int *direction);
static AVL_node_ty *MostLeftNode(AVL_node_ty *node, AVL_node_ty **befor_node);
/* static void NodeRemove(AVL_node_ty node, AVL_node_ty parent, AVL_cmp_func_ty compare); */
static void *NodeFind(AVL_node_ty *node, void* key, AVL_cmp_func_ty compare);
static int NodeIsLeaf(AVL_node_ty *node);
static int FindAndInsert(AVL_node_ty *runner, AVL_node_ty *node_to_insert, AVL_cmp_func_ty compare);
static int UpdateHeight(AVL_node_ty *runner);
static int GetDirection(void * data, void * insert_data, AVL_cmp_func_ty compare);
static AVL_node_ty *BalancedInsert(AVL_node_ty *runner, AVL_cmp_func_ty compare, void *data);
static AVL_node_ty *BalanceTreeIfNeeded(AVL_node_ty *node, AVL_cmp_func_ty compare, void *data);
static AVL_node_ty *RotateRight(AVL_node_ty *node);
static AVL_node_ty *RotateLeft(AVL_node_ty *node);
static size_t NodeTreeSize(AVL_node_ty *node);
static size_t NodeHigth(AVL_node_ty *node);
static void AVLForEachPreOrder(const AVL_node_ty *node, AVL_action_func_ty operation, void *param);
static void AVLForEachInOrder(const AVL_node_ty *node, AVL_action_func_ty operation, void *param);
static void AVLForEachPostOrder(const AVL_node_ty *node, AVL_action_func_ty operation, void *param);


/*-----------------------TYPEDEF--------------------------*/


/*--------------------------------------------------------*/


/*--------------------------------------------------------*/

AVL_ty *AVLCreate(AVL_cmp_func_ty compare)
{
    AVL_ty *new_avl = NULL;

    assert(NULL != compare);
        
    new_avl = (AVL_ty *)malloc(sizeof(AVL_ty));
    if (NULL == new_avl)
    {
        return NULL;
    }
    memset(new_avl,0,sizeof(AVL_ty));

    new_avl->compare = compare;
    new_avl->root = NULL;
    return new_avl;
}

static AVL_node_ty *NodeCreat(void *data)
{
    AVL_node_ty *new_tree_node = NULL;

    new_tree_node = (AVL_node_ty *)malloc(sizeof(AVL_node_ty));
    if(NULL == new_tree_node)
    {
        return NULL;
    }
    memset(new_tree_node,0,sizeof(AVL_node_ty));
    new_tree_node->data = data;
    new_tree_node->height = 1;
    return new_tree_node;
}
/*--------------------------------------------------------*/

void AVLDestroy(AVL_ty *tree)
{

    if(!AVLIsEmpty(tree))
    {
        AVLDestroyByElement(tree->root);
        free(tree->root);
        tree->root = NULL;
    }

    free(tree);
    tree = NULL;
       
}

static void AVLDestroyByElement(AVL_node_ty *node)
{
    if (NULL != node->relatives[LEFT])
    {
        AVLDestroyByElement(node->relatives[LEFT]);
        free(node->relatives[LEFT]);
        node->relatives[LEFT] = NULL;
    }
    if (NULL != node->relatives[RIGHT])
    {
        AVLDestroyByElement(node->relatives[RIGHT]);
        free(node->relatives[RIGHT]);
        node->relatives[RIGHT] = NULL;
    }

    return;
}
/*--------------------------------------------------------*/

void AVLRemove(AVL_ty *tree, void *data)
{
    /*find the data to remove by 
    * 
    if(tree->root->relatives[LEFT] == NULL)
    if(data > tree->root)*/
    int direction = ROOT_DIRECTION;
    int sons_num = 0;
    AVL_node_ty *before_node_to_remove = NULL;
    if(0 == tree->compare(tree->root->data,data))/*is removing root node*/
    {
        before_node_to_remove = tree->root;
        sons_num = GetNumOfSons(before_node_to_remove);
    }
    else
    {
        before_node_to_remove = FindToRemove(tree->root, data,tree->compare, &direction); 
        if(NULL == before_node_to_remove)
        {
            return;
        }
        sons_num = GetNumOfSons(before_node_to_remove->relatives[direction]);
    }
    switch (sons_num)
    {
    case NO_SONS:
        RemoveWithNoSons(tree ,before_node_to_remove->relatives[direction], before_node_to_remove, &direction);
        break;
    case ONE_LEFT_SON:
        RemoveWithLeftSon(tree, before_node_to_remove->relatives[direction], before_node_to_remove, &direction);
        break;
    case ONE_RIGHT_SON:
        RemoveWithRightSon(tree, before_node_to_remove->relatives[direction], before_node_to_remove, &direction);
        break;
    case TWO_SONS:
        RemoveWithTwoSons(tree, before_node_to_remove->relatives[direction], before_node_to_remove, &direction);
        break;
    }
}
static AVL_node_ty *FindToRemove(AVL_node_ty *node, void *data, AVL_cmp_func_ty compare, int *direction)
{
    if(NULL == node)
    {
        return NULL;
    }
    if (NULL != node->relatives[LEFT] && 0 == compare(node->relatives[LEFT]->data, data))
    {
        *direction = LEFT;
        return node;
    }
    if (NULL != node->relatives[RIGHT] && 0 == compare(node->relatives[RIGHT]->data, data))
    {
        *direction = RIGHT;
        return node;
    }

    if (NodeIsLeaf(node))
    {
        return NULL;
    }

    if (0 < compare(node->data, data))
    {
        if (NULL == (node->relatives[LEFT]))
        {
            return NULL;
        }
        node = node->relatives[LEFT];
        return FindToRemove(node, data, compare,direction);
    }
    else
    {
        if (NULL == (node->relatives[RIGHT]))
            return NULL;

        node = node->relatives[RIGHT];
        return FindToRemove(node, data, compare,direction);
    }
}

static int GetNumOfSons(AVL_node_ty *node) /*return: 0) no sons. 1) if just left son. 2)just right son. 3)tow sons. */
{
    int count = 0;
    if (node->relatives[LEFT] != NULL)
    {
        ++count;
    }
    if (node->relatives[RIGHT] != NULL)
    {
        count += 2;
    }

    return count;
}

static void RemoveWithNoSons(AVL_ty *tree, AVL_node_ty *node_to_remove, AVL_node_ty *parent, int *direction)
{
    if (ROOT_DIRECTION == *direction)
    {
        node_to_remove = tree->root;
        tree->root = NULL;
    }
    else
    {
        parent->relatives[*direction] = NULL;
    }
    
    free(node_to_remove);
    node_to_remove = NULL;
}

static void RemoveWithLeftSon(AVL_ty *tree, AVL_node_ty *node_to_remove, AVL_node_ty *parent, int *direction)
{
    (void) tree;
    if (ROOT_DIRECTION == *direction)
    {
        node_to_remove = parent->relatives[LEFT];
        parent->data = node_to_remove->data;
        parent->relatives[RIGHT] = node_to_remove->relatives[RIGHT];
        parent->relatives[LEFT] = node_to_remove->relatives[LEFT];
    }
    else
    {
        parent->relatives[*direction] = node_to_remove->relatives[LEFT];
    }
    
    
    free(node_to_remove);
    node_to_remove = NULL; 
}
static void RemoveWithRightSon(AVL_ty *tree, AVL_node_ty *node_to_remove, AVL_node_ty *parent, int *direction)
{
    (void)tree;
    if (ROOT_DIRECTION == *direction)
    {
        node_to_remove = parent->relatives[RIGHT];
        parent->data = node_to_remove->data;
        parent->relatives[RIGHT] = node_to_remove->relatives[RIGHT];
        parent->relatives[LEFT] = node_to_remove->relatives[LEFT];
    }
    else
    {
        parent->relatives[*direction] = node_to_remove->relatives[RIGHT];
    }
        
    free(node_to_remove);
    node_to_remove = NULL;
}

static void RemoveWithTwoSons(AVL_ty *tree ,AVL_node_ty *node_to_remove, AVL_node_ty *parent, int *direction)
{
    AVL_node_ty *next = NULL;
    AVL_node_ty *befor_next = NULL;
    (void) parent;
    (void) tree;
    if(ROOT_DIRECTION == *direction)
    {
        node_to_remove = parent;
        next = MostLeftNode(node_to_remove->relatives[RIGHT], &befor_next);
    }
    else
    {
        next = MostLeftNode(node_to_remove->relatives[RIGHT], &befor_next);
    }
    
    
    if(next == node_to_remove->relatives[RIGHT])
    {
        node_to_remove->data = next->data;
        node_to_remove->relatives[RIGHT] = next->relatives[RIGHT];
        
    }
    else 
    {
        befor_next->relatives[LEFT] = next->relatives[RIGHT];
        node_to_remove->data = next->data;
       
    }
    free(next);
    next = NULL;

}
static AVL_node_ty *MostLeftNode(AVL_node_ty *node, AVL_node_ty **befor_node)
{
    if(NULL == node->relatives[LEFT])
    {
        return node;
    }
    *befor_node = node;
    return MostLeftNode(node->relatives[LEFT],befor_node);
}

/* static void NodeRemove(AVL_node_ty node, AVL_node_ty parent, AVL_cmp_func_ty compare)
{

} */

/*--------------------------------------------------------*/
void *AVLFind(AVL_ty *tree, void *key)
{
    assert(NULL != tree);
    return NodeFind(tree->root,key,tree->compare);
}
static void *NodeFind(AVL_node_ty *node, void* key, AVL_cmp_func_ty compare)
{
    assert(NULL != compare);
    if(NULL == node)
    {
        return NULL;
    }

    if (0 == compare(node->data, key))
    {
        return key;
    }

    if (NodeIsLeaf(node))
    {
        return NULL;
    }

    if(0 < compare(node->data,key))
    {   if(NULL == (node->relatives[LEFT]))
        {
            return NULL;
        }
        node = node->relatives[LEFT];
        return NodeFind(node, key, compare);
    }
    else
    {
        if (NULL == (node->relatives[RIGHT]))
            return NULL;
        
        node = node->relatives[RIGHT];
        return NodeFind(node, key, compare);
    }
}
static int NodeIsLeaf(AVL_node_ty *node)
{
    return (NULL == (node->relatives[LEFT]) && NULL == (node->relatives[RIGHT]));
}
/*--------------------------------------------------------*/
int AVLInsert(AVL_ty *tree, void *data)
{
    AVL_node_ty *node = NodeCreat(data);
    if(NULL == node)
    {
        return 0;
    }
    if(AVLIsEmpty(tree))
    {
        tree->root = node;
        return 1;
    }
    return FindAndInsert(tree->root,node,tree->compare);
}

static int FindAndInsert(AVL_node_ty *runner, AVL_node_ty *node_to_insert, AVL_cmp_func_ty compare)
{

    int side = LEFT;
    side = GetDirection(runner->data, node_to_insert->data, compare);

    if (NULL == (runner->relatives[side]))
    {
        runner->relatives[side] = node_to_insert;

        return 1;
    }
    runner = runner->relatives[side];
    FindAndInsert(runner, node_to_insert, compare);
    runner->height = 1 + UpdateHeight(runner);
}

static int GetDirection(void * data, void * insert_data, AVL_cmp_func_ty compare)
{
    if (0 < compare(data, insert_data))
        return LEFT;
    return RIGHT;
}

int AVLInsertBalanced(AVL_ty *tree, void *data)
{
    assert(NULL != tree);

    tree->root = BalancedInsert(tree->root, tree->compare, data);

    return (NULL != tree->root ? 1 : 0);
}
static AVL_node_ty *BalancedInsert(AVL_node_ty *runner, AVL_cmp_func_ty compare, void *data)
{
    int side = LEFT;
    AVL_node_ty *new_node = NULL;

    assert(NULL != compare);

    if (NULL == runner)
    {
        return NodeCreat(data);
    }

    side = GetDirection(runner->data, data, compare);

    runner->relatives[side] = BalancedInsert(runner->relatives[side], compare, data);

    if (NULL == runner->relatives[side])
    {
        return NULL;
    }
    
    runner->height = 1 + UpdateHeight(runner);
    

    runner = BalanceTreeIfNeeded(runner, compare, data);

    return runner;
}

static int UpdateHeight(AVL_node_ty *runner)
{
    int left_height = 0, right_height = 0;
    left_height = (NULL == runner->relatives[LEFT] ? 0 : runner->relatives[LEFT]->height);
    right_height = (NULL == runner->relatives[RIGHT] ? 0 : runner->relatives[RIGHT]->height);

    return MAX(left_height, right_height);
}

static AVL_node_ty *BalanceTreeIfNeeded(AVL_node_ty *node, AVL_cmp_func_ty compare, void *data)
{
    int balance_factor = 0;

    assert(NULL != node);
    assert(NULL != compare);

    
    balance_factor = GetBalanceFactor(node);
    

    /* Left Left Case */
    if (1 < balance_factor && 0 < GetBalanceFactor(node->relatives[LEFT]))
    {
        return RotateRight(node);
    }

    /* Right Right Case */
    if (-1 > balance_factor && 0 > GetBalanceFactor(node->relatives[RIGHT]))
    {
        return RotateLeft(node);
    }

    /* Left Right Case */
    if (1 < balance_factor && 0 > GetBalanceFactor(node->relatives[LEFT]))
    {
        node->relatives[LEFT] = RotateLeft(node->relatives[LEFT]);
        return RotateRight(node);
    }

    /* Right Left Case */
    if (-1 > balance_factor && 0 < GetBalanceFactor(node->relatives[RIGHT]))
    {
        node->relatives[RIGHT] = RotateRight(node->relatives[RIGHT]);
        return RotateLeft(node);
    }

    return node;
}
int GetBalanceFactor(AVL_node_ty *runner)
{
    int left_height = 0, right_height = 0;
    
    left_height = (NULL == runner->relatives[LEFT] ? 0 : runner->relatives[LEFT]->height);
    right_height = (NULL == runner->relatives[RIGHT] ? 0 : runner->relatives[RIGHT]->height);

    if(NULL == runner)
    {
        return 0;
    }

    return  (int)(runner->relatives[LEFT]->height) - (int)(runner->relatives[RIGHT]->height);

}
static AVL_node_ty *RotateRight(AVL_node_ty *node)
{
    AVL_node_ty *node_left = node->relatives[LEFT];
    AVL_node_ty *node_left_right = node_left->relatives[RIGHT];

    /* Perform rotation  */
    node_left->relatives[RIGHT] = node;
    node->relatives[LEFT] = node_left_right;

    /* Update heights  */
    node->height = (node->height);
    node_left->height = (node_left->height);

    /* Return new root  */
    return node_left;
}

static AVL_node_ty *RotateLeft(AVL_node_ty *node)
{
    AVL_node_ty *node_right = node->relatives[RIGHT];
    AVL_node_ty *node_right_left = node_right->relatives[LEFT];

    /* Perform rotation  */
    node_right->relatives[LEFT] = node;
    node->relatives[RIGHT] = node_right_left;

    /*  Update heights  */
    node->height = (node->height);
    node_right->height = (node_right->height);

    /* Return new root  */
    return node_right;
}

/*--------------------------------------------------------*/
int AVLIsEmpty(const AVL_ty *tree)
{
    assert(NULL != tree);
    return NULL == tree->root;
}
/*--------------------------------------------------------*/

size_t AVLSize(const AVL_ty *tree)
{
    return NodeTreeSize(TREE_ROOT);
}
static size_t NodeTreeSize(AVL_node_ty *node)
{
    if (NULL == node)
    {
        return 0;
    }
    if (NodeIsLeaf(node))
    {
        return 1;
    }
    
    return 1 + NodeTreeSize(node->relatives[LEFT]) + NodeTreeSize(node->relatives[RIGHT]);
}
/*--------------------------------------------------------*/

size_t AVLHeight(const AVL_ty *tree)
{
    return NULL == tree->root ? 0 : (size_t)tree->root->height;
    /*  return NodeHigth(tree->root); */
}
static size_t NodeHigth(AVL_node_ty *node)
{
    if(NULL == node)
    {
        return 0;
    }
    if(NodeIsLeaf(node))
    {
        return 1;
    }
    return MAX(1 + NodeHigth(node->relatives[LEFT]),1 + NodeHigth(node->relatives[RIGHT]));
}
/*--------------------------------------------------------*/

int AVLForEach(const AVL_ty *tree, AVL_action_func_ty operation, void *param, int travesal_type)
{
    switch (travesal_type)
    {
    case PRE_ORDER:
        AVLForEachPreOrder(tree->root,operation,param);
        return 1;
        break;
    case In_ORDER:
        AVLForEachInOrder(tree->root, operation, param);
        return 1;
        break;
    case POST_ORDER:
        AVLForEachPostOrder(tree->root, operation, param);
        return 1;
        break;

    default:
    return 0;
        break;
    }
    
}
static void AVLForEachPreOrder(const AVL_node_ty *node, AVL_action_func_ty operation, void *param)
{
    if(NULL == node)
    {
        return;
    }
    operation(node->data, param);

    if(NULL != node->relatives[LEFT])
    {
        AVLForEachPreOrder(node->relatives[LEFT],operation,param);
    }
    if(NULL != node->relatives[RIGHT])
    {
        AVLForEachPreOrder(node->relatives[RIGHT], operation, param);
    }
}
static void AVLForEachInOrder(const AVL_node_ty *node, AVL_action_func_ty operation, void *param)
{
    if (NULL == node)
    {
        return;
    }
    

    if (NULL != node->relatives[LEFT])
    {
        AVLForEachInOrder(node->relatives[LEFT], operation, param);
    }

    operation(node->data, param);

    if (NULL != node->relatives[RIGHT])
    {
        AVLForEachInOrder(node->relatives[RIGHT], operation, param);
    }
}
static void AVLForEachPostOrder(const AVL_node_ty *node, AVL_action_func_ty operation, void *param)
{
    if (NULL == node)
    {
        return;
    }
    

    if (NULL != node->relatives[LEFT])
    {
        AVLForEachPostOrder(node->relatives[LEFT], operation, param);
    }

    if (NULL != node->relatives[RIGHT])
    {
        AVLForEachPostOrder(node->relatives[RIGHT], operation, param);
    }
    operation(node->data, param);

}
/*--------------------------------------------------------*/

static void print2DUtil(AVL_node_ty *root, int space)
{
    int i = 0;

    if (root == NULL)
    {
        return;
    }

    space += 10;

    print2DUtil(root->relatives[RIGHT], space);

    printf("\n");

    for (i = 10; i < space; ++i)
    {
        printf(" ");
    }
    printf("%d\n", *(int *)&root->data);

    print2DUtil(root->relatives[LEFT], space);
}

void print2D(AVL_ty *tree)
{
    print2DUtil(tree->root, 0);
}
static void printHUtil(AVL_node_ty *root, int space)
{
    int i = 0;

    if (root == NULL)
    {
        return;
    }

    space += 10;

    printHUtil(root->relatives[RIGHT], space);

    printf("\n");

    for (i = 10; i < space; ++i)
    {
        printf(" ");
    }
    printf("%d\n", root->height);

    printHUtil(root->relatives[LEFT], space);
}

void printH(AVL_ty *tree)
{
    printHUtil(tree->root, 0);
}