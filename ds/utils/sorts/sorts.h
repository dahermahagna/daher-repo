#ifndef __SORT_H__
#define __SORT_H__

#include <stddef.h> /* size_t */

typedef int (*cmp_func_ty)(const void *data, const void *data_to_compare);

typedef int (*key_func_t)(int num, size_t *param);
/*DESCRIPTION: 
*sort the array by Bubble algorithm.
*@param
*	arr - array to be sorted.
*	length - length of the array.
*@return
* no return.
*/
void BubbleSort(int arr[], size_t length);

/*DESCRIPTION: 
*sort the array by Selection algorithm.
*@param
*	arr - array to be sorted.
*	length - length of the array.
*@return
* no return.
*/
void SelectionSort(int arr[], size_t length);

/*DESCRIPTION: 
*sort the array by Insertion algorithm.
*@param
*	arr - array to be sorted.
*	length - length of the array.
*@return
* no return.
*/
void InsertionSort(int arr[], size_t length);

/*DESCRIPTION: 
*sort the array by Countung algorithm.
*@param
*	arr - array to be sorted.
*	length - length of the array.
*@return
* no return.
*/
void CountingSort(int arr[], size_t length);

/*DESCRIPTION: 
*sort the array by Radix algorithm.
*@param
*	arr - array to be sorted.
*	length - length of the array.
*@return
* no return.
*/
void RadixSort(int arr[], size_t length);

void *BinarySearchIterative(void *arr,  cmp_func_ty cmp_func, void *data,  size_t num_of_elements, size_t size_of_element);
void *BinarySearchRecursive(void *arr, cmp_func_ty cmp_func, void *data, size_t num_of_elements, size_t size_of_element);

int MergeSort(int *arr_to_sort, size_t num_of_elements);

void QuickSort(void *base, size_t nmemb, size_t size, cmp_func_ty compar);

#endif /*__SORT_H__*/