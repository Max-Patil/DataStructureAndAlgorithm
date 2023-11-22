//HEADER FILES
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>
#include "nd_array.h"

//MACROS & SYMBOLIC CONSTANTS
#define CHECK_OBJECT 0x00000001
#define CHECK_INDICES 0x00000002
#define ERROR_CHECKING(assert_condition, error_condition, err_msg)  \
         error_checking(assert_condition,                           \
                        #assert_condition,                          \
                        __FILE__,                                   \
                        __LINE__,                                   \
                        error_condition,                            \
                        err_msg)

#define RETURN_NULL(status)     { if(!status) return NULL; }
#define RETURN_FAILURE(status)  { if(!status) return FAILURE; }


//AUXILLARY FUNCTIONS
static void* xcalloc(size_t nr_elements, size_t nr_bytes);

static offset_t compute_offset(const p_nd_array_t p_nd_array, const size_t *p_indices);

static status_t error_checking(const int assert_condition,
                           const char* assert_condition_str,
                           const char* filename,
                           const int lineno,
                           const int error_condition,
                           const char* err_msg);

static status_t check_nd_array_object_conditions(const p_nd_array_t p_nd_array, const int conditions)
{
    if(CHECK_OBJECT & conditions)
    {
        RETURN_FAILURE( ERROR_CHECKING(p_nd_array || p_nd_array->p_jmp_dimensions || p_nd_array->p_dimensions || p_nd_array->ptr , 
                                       !p_nd_array || !p_nd_array->p_jmp_dimensions || !p_nd_array->p_dimensions || !p_nd_array->ptr,
                                       "ERROR : ND ARRAY Object is INVALID"));
    }
    return SUCCESS;
}

//INTERFACE FUNCTION DECLARATIONS
extern p_nd_array_t create_nd_array(size_t nr_dimensions, ...)
{
    //Error checking for reliability
    RETURN_NULL( ERROR_CHECKING(nr_dimensions != 0, nr_dimensions == 0, "ERROR : 0 dimension is not possible") );

    //Allocating heap memory and initalizing it with default and client data
    p_nd_array_t p_nd_array = xcalloc(1, ARRAY_SIZE);
    p_nd_array->nr_dimensions = nr_dimensions;
    p_nd_array->p_dimensions = xcalloc(nr_dimensions, sizeof(size_t));

    //Extracting size of each dimension from variable argument list of size nr_dimensions
    va_list indices;
    va_start(indices, nr_dimensions);

    for(size_t i = 0; i < nr_dimensions; ++i)
        p_nd_array->p_dimensions[i] = va_arg(indices, size_t);

    va_end(indices);

    //Enumerating each dimensions to compute total number of elements
    size_t total_element = 1;
    for(size_t i = 0; i < nr_dimensions; ++i)
        total_element *= p_nd_array->p_dimensions[i];
    
    //Precomputing stride for each dimension and allocating memory for internal array
    p_nd_array->ptr = (p_data_t)xcalloc(total_element, DATA_SIZE);
    p_nd_array->p_jmp_dimensions = (size_t*)xcalloc(nr_dimensions, sizeof(size_t));

    //Initializing and computing dimensional offset
    p_nd_array->p_jmp_dimensions[nr_dimensions - 1] = 1;

    for(long i = nr_dimensions - 2; i >= 0; i--)
        p_nd_array->p_jmp_dimensions[i] = p_nd_array->p_dimensions[i + 1] * p_nd_array->p_jmp_dimensions[i + 1];

    //Note : stride for each dimension is cummulative from highest dimension
    return p_nd_array;
}

//Data setter method
extern status_t set_nd_array_data(p_nd_array_t p_nd_array, data_t data, ...)
{
    RETURN_FAILURE(check_nd_array_object_conditions(p_nd_array, CHECK_OBJECT));

    size_t* p_indices = (size_t*)xcalloc(p_nd_array->nr_dimensions, sizeof(size_t));

    va_list indices;
    va_start(indices, data);

    for(size_t i = 0; i < p_nd_array->nr_dimensions; ++i)
    {
        size_t idx = va_arg(indices, size_t);

        RETURN_FAILURE( ERROR_CHECKING(idx < p_nd_array->p_dimensions[i], idx >= p_nd_array->p_dimensions[i], "ERROR : Index out of Bound"));

        p_indices[i] = idx;
    }

    p_nd_array->ptr[compute_offset(p_nd_array, p_indices)] = data;

    free(p_indices);
    va_end(indices);

    return SUCCESS;
}

extern data_t get_nd_array_data(p_nd_array_t p_nd_array, ...)
{
    RETURN_NULL(check_nd_array_object_conditions(p_nd_array, CHECK_OBJECT));

    size_t* p_indices = (size_t*) xcalloc(p_nd_array->nr_dimensions, sizeof(size_t));

    //Extracting the indices in order to compute offset
    va_list indices;
    va_start(indices, p_nd_array);

    for(size_t i = 0; i < p_nd_array->nr_dimensions; i++)
    {
        size_t idx = va_arg(indices, size_t);

        RETURN_NULL( ERROR_CHECKING(idx < p_nd_array->p_dimensions[i], idx >= p_nd_array->p_dimensions[i],  "ERROR : Index out of Bound"));

        p_indices[i] = idx;
    }

    data_t data = p_nd_array->ptr[compute_offset(p_nd_array, p_indices)];

    free(p_indices);
    va_end(indices);

    return data;
}

status_t print_nd_array(const p_nd_array_t p_nd_array)
{
    RETURN_FAILURE(check_nd_array_object_conditions(p_nd_array, CHECK_OBJECT));

    size_t idx = 0, offset = 0;

    //Size of N dimensional array = N * sizeof(N - 1)Array
    //Size of N - 1 Dimensional array = sizeof(N - 2)Array * size of(N - 3) ... size of element
    size_t total_elems = p_nd_array->p_dimensions[0] * p_nd_array->p_jmp_dimensions[0];

    size_t dimension = p_nd_array->nr_dimensions - 1;

    //Allocating array in order to modify indices with respect to given size of each dimension
    size_t* p_indices = (size_t*)xcalloc(p_nd_array->nr_dimensions, sizeof(size_t));

    while(idx++ < total_elems)
    {
        //Printing every index of multi-dimensional array based on total number of dimensions
        for(size_t indices = 0; indices < p_nd_array->nr_dimensions; indices++)
            printf("[%d]", p_indices[indices]);
        
        printf(" : %u\n", (size_t)p_nd_array->ptr[compute_offset(p_nd_array, p_indices)]);

        //Steps to modify pattern of indices with respect to size of each dimensions
        if(++p_indices[dimension] >= p_nd_array->p_dimensions[dimension])
        {
            --p_indices[dimension];
            while(dimension > 0 && p_indices[dimension] == (p_nd_array->p_dimensions[dimension] - 1))
                p_indices[dimension--] = 0;

            ++p_indices[dimension];
            dimension = p_nd_array->nr_dimensions - 1;
            printf("\n");
        }
    }

    free(p_indices);
    return SUCCESS;
}

extern size_t nd_array_dimension_size(p_nd_array_t p_nd_array, size_t idx)
{
    RETURN_FAILURE(check_nd_array_object_conditions(p_nd_array, CHECK_OBJECT));
    return (p_nd_array->p_dimensions[idx]);
}

extern status_t destroy_nd_array(p_nd_array_t* pp_nd_array)
{
    RETURN_FAILURE( ERROR_CHECKING(pp_nd_array != NULL, pp_nd_array == NULL, "ERROR : ND ARRAY Object is INVALID"));

    p_nd_array_t p_nd_array  = *pp_nd_array;
    RETURN_FAILURE( check_nd_array_object_conditions(p_nd_array, CHECK_OBJECT) );

    //free allocated memory in reverse order
    free(p_nd_array->p_jmp_dimensions);
    free(p_nd_array->p_dimensions);
    free(p_nd_array->ptr);
    free(p_nd_array);
    *pp_nd_array = NULL;

    return SUCCESS;
}

static void* xcalloc(size_t nr_elements, size_t nr_bytes)
{
    void* ptr = calloc(nr_elements, nr_bytes);

    RETURN_NULL( ERROR_CHECKING(true, ptr == NULL, "ERROR : Out of virtual memory"));

    return ptr;
}

static offset_t compute_offset(const p_nd_array_t p_nd_array, const size_t* p_indices)
{
    offset_t offset = 0;
    for(size_t i = 0; i < p_nd_array->nr_dimensions; ++i)
        offset += p_indices[i] * p_nd_array->p_jmp_dimensions[i];

    return offset;
}

static status_t error_checking(const int assert_condition,
                           const char* assert_condition_str,
                           const char* filename,
                           const int lineno,
                           const int error_condition,
                           const char* err_msg)
{
    status_t status = SUCCESS;
    #ifdef DEBUG
        if(!assert_condition)
        {
            fprintf(stderr, "Assertion failed : %s, File %s, Line : %d\n",
                    assert_condition, filename, lineno);
            status = FAILURE;
        }
    #else
        if(error_condition && err_msg)
        {
            fprintf(stderr, err_msg);
            fprintf(stderr, "\n");
            status = FAILURE;
        }
    #endif

    return status;
}
