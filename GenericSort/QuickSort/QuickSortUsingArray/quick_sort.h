#ifndef _QUICK_SORT_H
#define _QUICK_SORT_H

//HEADER FILES
#include<stdlib.h>

//SYMBOLIC CONSTANTS
#ifndef SUCCESS
#define SUCCESS 1
#endif

#ifndef FAILURE
#define FAILURE 0
#endif

//TYPE DEFINITIONS
typedef int status_t;
typedef void* data_t;
typedef data_t* arr_t;
typedef status_t (*compareDataProc)(data_t, data_t);

/**
* @func_name: quick_sort
* @summary: This function sort the array of any object type according to condition specified
* @params: 1) Generic Array of Void pointer
           2) Begin Index
           3) Last Index
           4) Object Size
           5) Callback Function ( Condition )
* @returns: SUCCESS if sorted successfully else FAILURE
*/

extern status_t quick_sort(arr_t p_arr, long first, long last, size_t obj_size, compareDataProc p_compare_func);

#endif