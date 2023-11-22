#ifndef _INSERTION_SORT_H
#define _INSERTION_SORT_H

//HEADER FILES
#include <stdio.h>
#include <stdlib.h>
#include "dcll.h"

//TYPE DEFINITION
typedef p_data_t arr_t;

/**
* @function: insertion_sort
* @summary: This function will sort the data according to sorting condition provided
* @params: 1) List
           2) Size
           3) Compare callback function
* @returns: Status of sorted operation
*/
extern status_t insertion_sort(p_dcll_t p_list, size_t size, compareDataProc p_compare_func);


#endif // _INSERTION_SORT_H