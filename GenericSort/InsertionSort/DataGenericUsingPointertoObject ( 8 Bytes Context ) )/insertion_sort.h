#ifndef _INSERTION_SORT_H
#define _INSERTION_SORT_H

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
typedef status_t (*compareDataProc)(data_t, data_t);

/**
* @func_name: insertion_sort
* @summary: This function will sort the array in certain order 
* @params: 1) Array of generic data
           2) Size of an array
           3) Compare callback function
* @returns: Status of insertion sort
*/
extern status_t insertion_sort(arr_t arr, size_t size, compareDataProc p_compare_func);

#endif //_INSERTION_SORT_H