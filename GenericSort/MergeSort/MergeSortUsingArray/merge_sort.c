//HEADER FILES
#include<stdio.h>
#include<stdlib.h>
#include"merge_sort.h"

//AUXILLARY FUNCTION DEFINITIONS
/**
* @func_name: memcpy
* @summary: This function will simply copy n# of bytes from source memory to destination memory
* @params: 1) Destination pointer
           2) Source pointer
           3) Number of bytes
* @returns: nothing
*/
static void memcpy(char* dest, const char* src, size_t nr_bytes)
{
    for(int i = 0; i < nr_bytes; i++)
        dest[i] = src[i];
}

/**
* @func_name: is_arrary_sorted
* @summary: This function will simply return whether array is empty, sorted or not
* @params: 1) Array of any compound object as well as pointer object ( varying type of context )
           2) Size of any array
           3) Size of each object
           4) Compare callback function
* @returns: TRUE if it is sorted else return FALSE
*/
static status_t is_arrary_sorted(arr_t arr, size_t size, size_t obj_size, compareDataProc p_compare_func)
{
    if(size == ONE_ELEMENT)
        return TRUE;

    size_t i = 0;
    
    while(i < size - 1 && p_compare_func(((char*)arr + i * obj_size), ((char*)arr + (i + 1) * obj_size)))
        i++;
    
    return (i == (size - 1));
}

/**
* @func_name: merge
* @summary: This function will simply merge two atomic data elements according to sorting condition
* @params: 1) Array of any compound object as well as pointer object ( varying type of context )
           2) beg index
           3) mid index
           4) end index
           5) size of object
           6) compare callback function
* @returns: return nothing
*/
static void merge(arr_t arr, size_t beg, size_t mid, size_t end, size_t obj_size, compareDataProc p_compare_func)
{
    size_t left_size = mid - beg + 1;
    size_t right_size = end - mid;

    arr_t left_arr = (arr_t)malloc(obj_size * left_size);
    arr_t right_arr = (arr_t)malloc(obj_size * right_size);

    for(size_t i = 0; i < left_size; i++)
        memcpy((char*)left_arr + i * obj_size, (char*)arr + (beg + i) * obj_size, obj_size);
    
    for(size_t i = 0; i < right_size; i++)
        memcpy((char*)right_arr + i * obj_size, (char*)arr + (mid + 1 + i) * obj_size, obj_size);

    size_t left_idx = 0, right_idx = 0, arr_idx = beg;

    while(left_idx != left_size && right_idx != right_size)
    {
        data_t data1 = (data_t)malloc(obj_size);
        data_t data2 = (data_t)malloc(obj_size);

        memcpy(data1, ((char*)left_arr + left_idx * obj_size), obj_size);
        memcpy(data2, ((char*)right_arr + right_idx * obj_size), obj_size);

        if(p_compare_func(data1, data2))
        {
            memcpy((char*)arr + arr_idx * obj_size, data1, obj_size);
            arr_idx++; left_idx++;
        }
        else
        {
            memcpy((char*)arr + arr_idx * obj_size, data2, obj_size);
            arr_idx++; right_idx++;
        }

        if(data1)
            free(data1); data1 = NULL;

        if(data2)
            free(data2); data2 = NULL;
    }

    while(left_idx != left_size)
    {
        memcpy((char*)arr + arr_idx * obj_size, (char*)left_arr + left_idx * obj_size, obj_size);
        arr_idx++; left_idx++;
    }
        
    while(right_idx != right_size)
    {
        memcpy((char*)arr + arr_idx * obj_size, (char*)right_arr + right_idx * obj_size, obj_size);
        arr_idx++; right_idx++;
    }
    //show_all_jobs(left_arr, left_size, NULL);
    //show_all_jobs(right_arr, right_size, NULL);

    if(left_arr)
        free(left_arr); left_arr = NULL;

    if(right_arr)
        free(right_arr); right_arr = NULL;
}


/**
* @func_name: partition
* @summary: This function will simply divide the array repeatitively untill we reach to atomic elements
* @params: 1) Array of any compound object as well as pointer object ( varying type of context )
           2) start index
           3) end index 
           4) Size of each object
           5) Compare call back function
* @returns: returns nothing
*/
static void partition(arr_t arr, size_t start, size_t end, size_t obj_size, compareDataProc p_compare_func)
{
    if(start < end)
    {
        size_t mid = (end - start) / 2 + start;
        partition(arr, start, mid, obj_size, p_compare_func);
        partition(arr, mid + 1, end, obj_size, p_compare_func);
        merge(arr, start, mid, end, obj_size, p_compare_func);
    }
}


/**
* @function: merge_sort
* @summary: This function will simply re-arranged element according to sorting condition
* @params: 1) Array of any compound object as well as pointer object ( varying type of context )
           2) Size of an array
           3) Size of object
           4) Compare Callback function
* @returns: Status of sorted operation
*/
extern status_t merge_sort(arr_t arr, size_t size, size_t obj_size, compareDataProc p_compare_func)
{
    if(is_arrary_sorted(arr, size, obj_size, p_compare_func))
    {
        fprintf(stdout, "NOTE : Array is already sorted\n");
        return SUCCESS;
    }

    partition(arr, 0,  size - 1, obj_size, p_compare_func);
    return SUCCESS;
}