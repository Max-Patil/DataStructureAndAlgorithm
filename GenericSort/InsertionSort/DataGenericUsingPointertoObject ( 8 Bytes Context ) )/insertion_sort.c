//HEADER FILE
#include<stdlib.h>
#include "insertion_sort.h"

/**
* @func_name: insertion_sort
* @summary: This function will sort the array in certain order 
* @params: 1) Array of generic data
           2) Size of an array
           3) Compare callback function
* @returns: Status of insertion sort
*/
extern status_t insertion_sort(arr_t arr, size_t size, compareDataProc p_compare_func)
{
    for(size_t i = 1; i < size; i++)
    {
        data_t key = arr[i];

        long j = i - 1;

        while(j >= 0 && SUCCESS == p_compare_func(arr[j], key))
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }

        arr[j + 1] = key;
    }

    return SUCCESS;
}

