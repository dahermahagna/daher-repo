#ifndef __AVL_H__
#define __AVL_H__

#include <stddef.h> /* size_t */


typedef struct rec_avl_bst AVL_ty;


/* DESCRIPTION: 
 *compare two elements data;
 * 0 -if same
 * positive if data > data_to_compare
 * negative if data < data_to_compare
 */
typedef int(*AVL_cmp_func_ty)(const void *data, const void *data_to_compare);

/* DESCRIPTION: 
 * perform action on elements in AVL
 * this action should not change the sorting criteria 
 */
typedef int(*AVL_action_func_ty)(const void *data, void *param); 

/*-------------------------------------------------------*/

/* DESCRIPTION: 
 *  create AVL binary search tree,
 *  tree created by this function should be destroy using AVLDestroy function 
 *
 *		@param
 *		compare - pointer to function to sort the tree by
 *
 * @return
 * pointer to created AVL
 */
AVL_ty *AVLCreate(AVL_cmp_func_ty compare);

/* DESCRIPTION: 
 * destroy AVL recursivly and free all allocated memory
 *
 *		@param
 *		pointer to AVL to be destroyed
 *
 * @return
 * 
 */
void AVLDestroy(AVL_ty *tree);

/* DESCRIPTION: 
 * find a element relevent to the key and return data to this element.
 *
 *		@param
 *		tree - pointer to AVL to look in
 *      key - key to find data according to
 *
 * @return
 *  data of found node
 * NULL if not found
 */
void *AVLFind(AVL_ty *tree, void *key);

/* DESCRIPTION: 
 * insert new element to the tree in right location.
 *
 *		@param
 *		tree -	tree - pinter to AVL to insert to
 *      data - data of new element 
 *
 * @return
 *  1 for success, 0 for failure
 */
int AVLInsert(AVL_ty *tree, void *data);

/* DESCRIPTION: 
 * deletes element from the tree  
 *
 *		@param
 * 	    tree - pointer to the tree
 *		data - data of node to remove
 *      
 * @return
 *
*/
void AVLRemove(AVL_ty *tree, void *data);

/*DESCRIPTION:
* checks if there are any elements in the tree
*
*	@param
* 	tree - pointer to the tree
*
*@return
* 1 if empty, 0 if not
*/
int AVLIsEmpty(const AVL_ty *tree);

/*DESCRIPTION:
* gets the amounts of elements in the tree
*
*	@param
* 	tree - pointer to the tree
*
*@return
*number of elements in the tree
*/
size_t AVLSize(const AVL_ty *tree);

/* DESCRIPTION: 
 * finds the height of the tree, longest route from
 * root of the tree to the most faraway leef. 
 *
 *     @param
 *        tree - tree to find the height of
 *
 * @return
 *  the height of the tree 
*/
size_t AVLHeight(const AVL_ty *tree);

/*DESCRIPTION:
* activate operation function on each element
* operation should not change the compare critiria
* travesal type can be 1 for pre order, 2 for in order, 3 for post order
*
*	@param
* 	tree - pointer to the tree
*   operation - function to aactivate
*   param - parameter to operation function
*   travesal_type - wanted order to travers the tree by
*
* @return
* 0 on success, non-zero on faliure
*/
int AVLForEach(const AVL_ty *tree, AVL_action_func_ty operation, void *param, 
                                                            int travesal_type);

void print2D(AVL_ty *tree);
void printH(AVL_ty *tree);

#endif /*__AVL_H__*/