
// HEADER FILE
#include "insertion_sort.h"

/**
* @func_name: memcpy
* @summary: This function will simply copy the content of bytes from source to destination
* @params: 1) Destination
           2) Source
           3) Number of bytes
* @returns: Nothing
*/
static void memcpy(char *dest, char *src, size_t nr_bytes)
{
    for (size_t i = 0; i < nr_bytes; i++)
        dest[i] = src[i];
}

/**
* @function: insertion_sort
* @summary: This function will take array of generic data and re_arranged them based on sorting condition provided
* @params: 1) Generic array
           2) Size of generic array
           3) Takes size of every element
           3) Compare callback function pointer
* @returns: nothing
*/
extern void insertion_sort(arr_t arr, size_t size, size_t data_size, compareDataProc p_compare_func)
{
    //loop(i = 1 to size - 1)
    for(size_t i = 1; i < size; i++)
    {
        //key = arr[i];
        //1) Allocate adequate memory for object
        //2) Copy the content of from one memory to another
        data_t key = malloc(data_size);
        data_t src = ((char*)arr + i * data_size);
        memcpy(key, src, data_size);

        //j = i - 1
        long j = i - 1;

        //loop(j >= 0 and key < s_arr[j])
        while(j >= 0 && SUCCESS == p_compare_func((char*)arr + j * data_size, key))
        {
            //Computing appropriate address as base address of each object
            data_t src = ((char*)arr + j * data_size);
            data_t dest = ((char*)arr + (j + 1) * data_size);

            //Copy the content of previous object to current object
            //arr[j + 1] = arr[j];
            memcpy(dest, src, data_size);

            //j = j - 1;
            j = j - 1;
        }
        //show_all_jobs(s_arr, 5, NULL);

        //s_arr[j + 1] = key;
        memcpy((char*)arr + (j + 1) * data_size, key, data_size);

        //Release the allocated resource
        if(key)
            free(key); key = NULL;
    }
}

// extern void insertion_sort(arr_t arr, size_t size, size_t data_size, compareDataProc p_compare_func)
// {      
//     arr_t s_arr = (arr_t)malloc(sizeof(data_t) * size); //only sorts the array of object
//     for(size_t i = 0; i < size; i++)
//     {   
//         s_arr[i] = (data_t)malloc(data_size);
//         //(data_t)s_arr[i] = (data_t)((char*)arr + i * data_size); // only sorts the array of object
//         memcpy(s_arr[i], (data_t)((char*)arr + i * data_size), data_size);
//     }
//     show_all_jobs_1(s_arr, 5, NULL);

//     for (size_t i = 1; i < size; i++)
//     {
//         data_t key = s_arr[i];
//         long j = i - 1;
        
//         while (j >= 0 && SUCCESS == p_compare_func(s_arr[j], key))
//         {
//             s_arr[j + 1] = s_arr[j];
//             j = j - 1;
//         }
//         s_arr[j + 1] = key;
//     }
//     show_all_jobs_2(s_arr, 5, NULL);

//     for(size_t i = 0; i < size; i++)
//         memcpy((char *)arr + i * data_size, (char*)(s_arr[i * data_size]), data_size);
    
//     if (s_arr)
//         free(s_arr);
//     s_arr = NULL;
// }