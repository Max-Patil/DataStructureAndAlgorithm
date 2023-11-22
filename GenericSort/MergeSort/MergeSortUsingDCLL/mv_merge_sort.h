#ifndef __MV_MERGE_SORT_H
#define __MV_MERGE_SORT_H

//HEADER FILES
#include"mv_dcll.h"

//TYPE DEFINITIONS
typedef p_node_t* arr_t;

/**
* @function: mv_merge_sort
* @summary: This function will simply sort the doubly circular linked list 
using divide & conquer method
* @params: 1) List
           2) Size of the list
           3) Compare callback function pointer ( sorting condition )
* @returns: Status of sorting function
*/
extern status_t mv_merge_sort(p_mv_dcll_t p_list, size_t size, compareDataProc p_compare_func);

#endif // __MV_MERGE_SORT_H 