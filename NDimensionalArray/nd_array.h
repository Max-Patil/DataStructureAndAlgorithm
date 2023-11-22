#ifndef _ND_ARRAY_H
#define _ND_ARRAY_H

//HEADER FILE
#include <stdlib.h>

//SYMBOLIC CONSTANT
#define SUCCESS 1
#define FAILURE 0
#define ARRAY_SIZE (sizeof(nd_array_t))
#define DATA_SIZE   (sizeof(data_t))

//TYPE DEFINITION
typedef struct nd_array nd_array_t;
typedef nd_array_t* p_nd_array_t;
typedef void* data_t;
typedef data_t* p_data_t;
typedef int status_t;
typedef size_t offset_t;

//STRUCTURE TYPE DEFINITION
struct nd_array
{
    data_t* ptr;                //Pointer to internal array data structure
    size_t* p_dimensions;       //Array for storing size of each dimension
    size_t nr_dimensions;       //Total number of dimensions
    size_t* p_jmp_dimensions;   //Array of stride 
};

//INTERFACE FUNCTION DECLARATIONS
extern p_nd_array_t create_nd_array(size_t nr_dimensions, ...);

extern status_t set_nd_array_data(p_nd_array_t p_nd_aray, data_t data, ...);

extern data_t get_nd_array_data(p_nd_array_t p_nd_array, ...);

extern size_t nd_array_dimension_size(p_nd_array_t p_nd_array, size_t index);

extern size_t nr_array_nr_dimensions(p_nd_array_t p_nd_array);

extern status_t print_nd_array(p_nd_array_t p_nd_array);

extern status_t destroy_nd_array(p_nd_array_t* pp_nd_array);

#endif //_ND_ARRAY_H