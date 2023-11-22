//HEADER FILES
#include<stdio.h>
#include<stdlib.h>
#include"mv_dcll.h"
#include"mv_merge_sort.h"

//MERGE SORT AUXILLARY FUNCTION DEFINITIONS
/**
* @function: merge
* @summary: This function will simply conquer by merging the two atomic elements of an array according sorting condition
* @params: 1) Array
           2) Begin index
           3) Middle index
           4) Last Index
           5) Compare callback function pointer
* @returns: nothing
*/
static void merge(arr_t arr, size_t beg, size_t mid, size_t last, compareDataProc p_compare_func)
{
    size_t arr1_size = mid - beg + 1;
    size_t arr2_size = last - mid; // last - (mid + 1) + 1 => last - mid - 1 + 1 => last - mid

    arr_t arr1 = (arr_t)malloc(arr1_size * sizeof(*arr1));
    arr_t arr2 = (arr_t)malloc(arr2_size * sizeof(*arr2));

    for(size_t idx = 0; idx < arr1_size; idx++)
        arr1[idx] = arr[beg + idx];

    for(size_t idx = 0; idx < arr2_size; idx++)
        arr2[idx] = arr[(mid + 1) + idx];

    size_t arr1_idx = 0, arr2_idx = 0, arr_idx = beg;
    
    while(TRUE)
    {
        if(arr1_idx == arr1_size)
        {
            while(arr2_idx < arr2_size)
                arr[arr_idx++] = arr2[arr2_idx++];
            break;
        }

        if(arr2_idx == arr2_size)
        {
            while(arr1_idx < arr1_size)
                arr[arr_idx++] = arr1[arr1_idx++];
            break;
        }

        if(p_compare_func(mv_get_node_data(arr1[arr1_idx]), mv_get_node_data(arr2[arr2_idx])))
        {
            arr[arr_idx++] = arr1[arr1_idx++];
        }
        else
             arr[arr_idx++] = arr2[arr2_idx++];
    }

    if(arr1)
        free(arr1); arr1 = NULL;
    if(arr2)
        free(arr2); arr2 = NULL;
}

/**
* @function: partition
* @summary: This function will simply divide whole linked list repeatedly 
* @params: 1) Array
           2) start index
           3) last index
* @returns: nothing
*/
static void partition(arr_t arr, size_t start, size_t end, compareDataProc p_compare_func)
{
    if(start < end)
    {
        size_t mid = (end - start)/ 2 + start;
        partition(arr, start, mid, p_compare_func);
        partition(arr, mid + 1, end, p_compare_func);
        merge(arr, start, mid, end, p_compare_func);
    }
}

/**
* @function: mv_merge_sort
* @summary: This function will simply sort the doubly circular linked list 
using divide & conquer method
* @params: 1) List
           2) Size of the list
           3) Compare callback function pointer ( sorting condition )
* @returns: Status of sorting function
*/
extern status_t mv_merge_sort(p_mv_dcll_t p_list, size_t size, compareDataProc p_compare_func)
{
    if(mv_is_list_sorted(p_list, p_compare_func) == TRUE)
    {
        fprintf(stdout, "List is already sorted\n");
        return SUCCESS;
    }

    p_node_t p_run = p_list->p_head->p_next;

    p_node_t* arr = (p_node_t*)calloc(size, sizeof(p_node_t));
   
    for(size_t i = 0; i < size; p_run = p_run->p_next, i++)
    {
        arr[i] = p_run;
    }

    partition((arr_t)arr, 0, size - 1, p_compare_func);
 
    establish_links_between(p_list->p_head, arr[0]);

    for(size_t i = 0; i < size - 1; i++)
        establish_links_between(arr[i], arr[i + 1]);

    establish_links_between(arr[size - 1], p_list->p_head);

    if(arr)
        free(arr); arr = NULL;

    return SUCCESS;
}