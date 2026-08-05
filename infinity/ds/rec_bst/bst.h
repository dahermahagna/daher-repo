#ifndef __BST_H__
#define __BST_H__

#include <stddef.h> /* size_t */

typedef struct binary_search_tree BST_ty;
typedef struct tree_node bst_node_ty;
typedef bst_node_ty *BST_iter_ty;


/* DESCRIPTION: 
 *compare two elements data;
 * 0 -if same
 * positive if data > data_to_compare
 * negative if data < data_to_compare
 */
typedef int(*BST_cmp_func_ty)(const void *data, const void *data_to_compare, void *params);
/* DESCRIPTION: 
 * perform action on elements in BST
 * this action should not change the sorting criteria 
 */
typedef int(*BST_action_func_ty)(const void *data, void * param); 



/*-------------------------------------------------------*/

/* DESCRIPTION: 
 *  create binary search tree,
 *  tree created by this function should be destroy using BDTDestroy function 
 *
 *		@param
 *		compare - pointer to function to sort the tree by
 *
 * @return
 * pointer to created BST, 
 */
BST_ty *BSTCreate(BST_cmp_func_ty compare, void *params);
/* DESCRIPTION: 
 * destroy BST and free all allocated memory
 *
 *		@param
 *		pointer to BST to be destroyed
 *
 * @return
 * 
 */
void BSTDestroy(BST_ty *tree);
/* DESCRIPTION: 
 * find a element wich holds key and return pointer to this element.
 * BSTEnd in case of failure
 *
 *		@param
 *		tree - pinter to BST to look in
 *      data_to_match - datato look for
 *
 * @return
 *  iterator to the element, iterator to BSTEnd if not found
 */
BST_iter_ty BSTSearch(BST_ty *tree, void *data_to_match);
/* DESCRIPTION: 
 * insert new element to the tree in right location.
 * duplicate values will inserted to larger subtree
 *
 *		@param
 *		tree -	tree - pinter to BST to insert to
 *      data - data of new element 
 *
 * @return
 *  iterator to inserted element, iterator to BSTEnd in case of faliure
 */
BST_iter_ty BSTInsert(BST_ty *tree, void *data);
/* DESCRIPTION: 
 * deletes element from the tree  
 *
 *		@param
 *		iter - iterator to element to delete
 *      
 * @return
 *  data of the deleted element
 */
void *BSTRemove(BST_iter_ty iter);
/*DESCRIPTION:
* checks if there are any elements in the tree
*
*	@param
* 	tree - pointer to the tree
*
*@return
* 1 if empty, 0 if not
*/
int BSTIsEmpty(const BST_ty *tree);

/*DESCRIPTION:
* gets the amounts of elements in the tree pre order travers
*
*	@param
* 	tree - pointer to the tree
*
*@return
*number of elements in the tree
*/
size_t BSTSize(const BST_ty *tree);
/*DESCRIPTION:
* activate operation function on each element in given range [from, to)
* operation should not change the compare critiria in order travers
*	@param
* 	tree - pointer to the tree
    operation - function to aactivate
    from -iter to element to start
    to - iter to element to end
    param - parameter to operation function
*
*@return
*0 on success, non-zero on faliure
*/
int BSTForEach(BST_iter_ty from, BST_iter_ty to, BST_action_func_ty operation, void  *param);
/* DESCRIPTION: 
 * return next iterator in order  
 *  
 *		@param
 *		iter - iterator to element
 *
 * @return
 *  iterator to next element
 */
BST_iter_ty BSTIterNext(BST_iter_ty iter);
/* DESCRIPTION: 
 * return previous iterator in order  
 *
 *		@param
 *		iter - iterator to element
 *
 * @return
 *  iterator to previous element
 */
BST_iter_ty BSTIterPrev(BST_iter_ty iter);
/* DESCRIPTION: 
 * return iterator first element by order  
 *
 *		@param
 *		tree -pointer to tree
 *
 * @return
 *  iterator to first element by order
 */
BST_iter_ty BSTBegin(const BST_ty *tree);
/* DESCRIPTION: 
 * return iterator last element by order  
 * this iterator should not be removed
 *		@param
 *		tree -pointer to tree
 *
 * @return
 *  iterator to last element by order
 */
BST_iter_ty BSTEnd(const BST_ty *tree);
/* DESCRIPTION: 
 * return data of element pointed by iter  
 * 
 *		@param
 *		tree -pointer to tree
 *
 * @return
 *  data of element
 */

void *BSTGetData(BST_iter_ty iter);
/* DESCRIPTION: 
 * compare two iterators 
 * 	@param
 *		iter1
        iter2
 *
 * @return
 *  1 - if same, 0- if not 
 */

int BSTIsSameIter(BST_iter_ty iter1, BST_iter_ty iter2);

#endif 