#ifndef _TWO_STACK_H
#define _TWO_STACK_H

//SYMBOLIC CONSTANTS
#define SUCCESS 1
#define FAILURE 0
#define TRUE SUCCESS
#define FALSE FAILURE
#define MAX_ARRAY_SIZE 2046
#define ARR_SIZE (sizeof(arr_t))
#define STACK_SIZE (sizeof(stack_t))
#define DATA_SIZE (sizeof(data_t))

//ERROR CODES
#define CHECK_ARRAY 0x00000001
#define CHECK_DATA 0x00000002
#define CHECK_OBJECT CHECK_ARRAY | CHECK_DATA 
#define CHECK_OVERFLOW 0x00000004
#define CHECK_STACK1_UNDERFLOW 0x00000008
#define CHECK_STACK2_UNDERFLOW 0x00000010

//TYPE DEFINITIONS
typedef struct array arr_t;
typedef arr_t* p_arr_t;
typedef p_arr_t* pp_arr_t;
typedef struct stack stack_t;
typedef stack_t* p_stack_t;
typedef void* data_t;
typedef int status_t;
typedef data_t* p_data_t;
typedef void(*printDataProc)(data_t);
typedef void(*deleteDataProc)(data_t);

//STRUCTURE TYPE DEFINITIONS
struct array
{
    p_data_t ptr;
    p_stack_t p_stack1;
    p_stack_t p_stack2;
    size_t arr_size;
};

struct stack
{
    int head;
    size_t nr_elements;
};

//INTERFACE FUNCTION DECLARATIONS
//Data structure 'two stack' creation method
extern p_arr_t create_two_stack_arr(size_t arr_size);

//Data structure 'two stack' array insertion method
extern status_t stack1_push(p_arr_t p_array, data_t data);
extern status_t stack2_push(p_arr_t p_array, data_t data);

//Data structure 'two stack' array deletion method
extern data_t stack1_pop(p_arr_t p_array);
extern data_t stack2_pop(p_arr_t p_array);

//Data structure 'two stack' respresentation method
extern status_t arr_print(const p_arr_t p_array, printDataProc p_print_func, const char* msg);

//Data structure 'two stack' destruction method
extern status_t destroy_two_stack_arr(pp_arr_t pp_array, deleteDataProc p_delete_func);

#endif // _TWO_STACK_H