#ifndef _MERGE_SORT_H
#define _MERGE_SORT_H

//HEADER FILES
#include<stdio.h>
#include<stdlib.h>

//SYMBOLIC CONSTANTS
#define SUCCESS 1
#define FAILURE 0
#define TRUE SUCCESS
#define FALSE FAILURE

#define ONE_ELEMENT 1

//TYPE DEFINITIONS
typedef void* data_t;
typedef int status_t;
typedef data_t* arr_t;
typedef status_t(*compareDataProc)(data_t, data_t);

//MERGE SORT INTERFACE FUNCTION

/**
* @function: merge_sort
* @summary: This function will simply re-arranged element according to sorting condition
* @params: 1) Array of any compound object as well as pointer object ( varying type of context )
           2) Size of an array
           3) Size of object
           4) Compare Callback function
* @returns: Status of sorted operation
*/
extern status_t merge_sort(arr_t arr, size_t size, size_t obj_size, compareDataProc p_compare_func);

#endif