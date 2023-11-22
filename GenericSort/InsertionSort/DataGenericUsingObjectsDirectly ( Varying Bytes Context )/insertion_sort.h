#ifndef _INSERTION_SORT_H
#define _INSERTION_SORT_H

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
typedef void* data_t;
typedef data_t* arr_t;
typedef int status_t;
typedef status_t(*compareDataProc)(data_t, data_t);

/**
* @function: insertion_sort
* @summary: This function will take array of generic data and rearranged them based on sorting condition provided
* @params: 1) Generic array
           2) Size of generic array
           3) Compare callback function pointer
* @returns: nothing
*/
extern void insertion_sort(arr_t arr, size_t size, size_t data_size, compareDataProc p_compare_func);

#endif // _INSERTION_SORT_H