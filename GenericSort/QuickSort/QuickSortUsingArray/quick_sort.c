//HEADER FILES
#include"quick_sort.h"

/**
* @func_name: memcpy
* @summary: This function will simply copy number of bytes from source to destination address
* @params: 1) destination address
           2) source address
           3) number of bytes
* @returns: None
*/
static void memcpy(char* dest, char* src, size_t nr_bytes)
{
    for(size_t idx = 0; idx < nr_bytes; idx++)
        dest[idx] = src[idx];
}

/**
* @func_name: swap
* @summary: This function simply swap the element provided
* @params: 1) Generic Array
           2) first index at which our element resides
           3) second index at which our another element resides
           4) Object Size
* @returns: None
*/
static void swap(arr_t p_arr, size_t idx1, size_t idx2, size_t obj_size)
{
    void* temp = (void*)malloc(obj_size);

    memcpy(temp, (char*)p_arr + idx1 * obj_size, obj_size);
    memcpy((char*)p_arr + idx1 * obj_size, (char*)p_arr + idx2 * obj_size, obj_size);
    memcpy((char*)p_arr + idx2 * obj_size, temp, obj_size);

    free(temp); temp = NULL;
}

/**
* @func_name: partition

* @summary: This function bifurcate / partitioned the array into two part based on pivot element and bring
pivot element at its correct index in an array such that value(left_part[i]) <= pivot < value(right_part[i])

* @params: 1) Generic Array of Void Pointer
           2) Start Index
           3) End Index
           4) Object Size
           5) Callback function ( condition )

* @returns: Pivot index
*/

static long partition(arr_t p_arr, long first, long last, size_t obj_size, compareDataProc p_compare_func)
{
    //i = first - 1;
    long left = first - 1;

    //pivot = arr[last]
    void *p_pivot = (void*)malloc(obj_size);
    memcpy(p_pivot, (char*)p_arr + last * obj_size, obj_size);
    

    //loop(j = first to last - 1)
    for(long right = first; right < last; right++)
    {
        //if( arr[j] <= pivot )
        if(p_compare_func( (char*)p_arr + right * obj_size, p_pivot ))
        {
            //i = i + 1
            left = left + 1;
   
            //swap(arr[i], arr[j])
            if(left != right)
                swap(p_arr, left, right, obj_size);
        }

    }
   
    //swap(arr[i + 1], arr[last])
    if(left + 1 != last)
        swap(p_arr, left + 1, last, obj_size);

    if(p_pivot)
         free(p_pivot); p_pivot = NULL;

    //return i + 1
    return left + 1;
}

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

extern status_t quick_sort(arr_t p_arr, long first, long last, size_t obj_size, compareDataProc p_compare_func)
{   

    if(first < last)
    {
        long pivot_index = partition(p_arr, first, last,obj_size, p_compare_func);
        quick_sort(p_arr, first, pivot_index - 1, obj_size, p_compare_func);
        quick_sort(p_arr, pivot_index + 1, last, obj_size, p_compare_func);
    }

    return SUCCESS;
}