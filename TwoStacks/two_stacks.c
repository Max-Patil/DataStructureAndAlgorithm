//HEADER FILES
#include<stdio.h>
#include<stdlib.h>
#include"two_stacks.h"

//MACROS
#define ERROR_CHECKING(assert_condition, error_condition, err_msg)(  \
                error_checking(assert_condition,                     \
                               #assert_condition,                    \
                               __FILE__,                             \
                               __LINE__,                             \
                               error_condition,                      \
                               err_msg))

#define RETURN_FAILURE(status) { if(!status) return FAILURE; }
#define RETURN_NULL(status)    { if(!status) return NULL; }

//AUXILLARY FUNCTION DECLARATIONS
static status_t error_checking(const int assert_condition,              \
                               const char*assert_condition_str,         \
                               const char* filename,                    \
                               const int line_no,
                               const int error_condition,
                               const char* err_msg);

static void* xmalloc(size_t nr_bytes);
static void* xrealloc(void* oldptr, size_t nr_bytes);
static status_t check_array_object_condition(const p_arr_t p_array, int conditions);

//INTERFACE FUNCTION DEFINITIONS
extern p_arr_t create_two_stack_arr(size_t arr_size)
{
        RETURN_NULL ( ERROR_CHECKING(arr_size < MAX_ARRAY_SIZE,
                                     arr_size >= MAX_ARRAY_SIZE,
                                     "ERROR : Array size exceeds maximum"));

    //Allocating memory for whole array
    p_arr_t p_array = (p_arr_t)xmalloc(ARR_SIZE);
    p_array->ptr = (p_data_t)xmalloc(arr_size * DATA_SIZE);
    p_array->arr_size = arr_size;

    //Allocating memory for two stacks
    p_array->p_stack1 = (p_stack_t)xmalloc(STACK_SIZE);
    p_array->p_stack2 = (p_stack_t)xmalloc(STACK_SIZE);
   
    //Error checking for realibility 
    RETURN_NULL(check_array_object_condition(p_array, CHECK_OBJECT));

    //Two stacks initialization
    p_array->p_stack1->head = 0;
    p_array->p_stack2->head = p_array->arr_size - 1;

    p_array->p_stack1->nr_elements = p_array->p_stack2->nr_elements = 0;
    return p_array;
}

//Data structure 'two stack' insertion methods
extern status_t stack1_push(p_arr_t p_array, data_t data)
{ 
    //Error checking for reliability i.e leave no room for any error
    RETURN_FAILURE(check_array_object_condition(p_array, CHECK_OBJECT | CHECK_OVERFLOW ));
   
    //Note : head is always pointing to empty room for data
    p_array->ptr[p_array->p_stack1->head++] = data;
    
    /*Note : Here, there is no need to check whether one head over passes another head because first 
    of all it is not going to be incremented any further because of overflow condition
    being meet and second thing, as both head are incremented/decremented by 1, they are going to be 
    decremented/ incremented in stack pop routine appropriately  */
    p_array->p_stack1->nr_elements++;
    return SUCCESS;
}

extern status_t stack2_push(p_arr_t p_array, data_t data)
{
    //Error checking for reliability i.e leave no room for any error
    RETURN_FAILURE( check_array_object_condition(p_array, CHECK_OBJECT | CHECK_OVERFLOW ) );

    //Note : head is always pointing to empty room for data
    p_array->ptr[p_array->p_stack2->head--] = data;

    /*Note : Here, there is no need to check whether one head over passes another head because first 
    of all it is not going to be incremented any further because of overflow condition
    being meet and second thing, as both head are incremented/decremented by 1, they are going to be 
    decremented/ incremented in stack pop routine appropriately */
    p_array->p_stack2->nr_elements++;
    return SUCCESS;
}

//Data structure 'two stack' array deletion methods
extern data_t stack1_pop(p_arr_t p_array)
{
    //Error checking for reliability i.e leave no room for any error
    RETURN_NULL( check_array_object_condition(p_array, CHECK_OBJECT | CHECK_STACK1_UNDERFLOW ) );
 
    data_t popped_data = p_array->ptr[--p_array->p_stack1->head];
    p_array->ptr[p_array->p_stack1->head] = 0;

    if(p_array->p_stack1->head == -1)
        p_array->p_stack1->head = 0;

    p_array->p_stack1->nr_elements--;
    return popped_data;
}

extern data_t stack2_pop(p_arr_t p_array)
{
    //Error checking for reliability i.e leave no room for any error
    RETURN_NULL( check_array_object_condition(p_array, CHECK_OBJECT | CHECK_STACK2_UNDERFLOW ) );
   
    data_t popped_data = p_array->ptr[++p_array->p_stack2->head];
    p_array->ptr[p_array->p_stack2->head ] = 0;

    if(p_array->p_stack2->head == p_array->arr_size)
        p_array->p_stack2->head = p_array->arr_size - 1;

    p_array->p_stack1->nr_elements--;
    return popped_data;
}

//Data structure 'two stack' representation methods
extern status_t arr_print(const p_arr_t p_array, printDataProc p_print_func, const char* msg)
{
    RETURN_FAILURE( check_array_object_condition(p_array, CHECK_OBJECT ) );

    if(msg) puts(msg);
    for(int le = 0; le < p_array->arr_size; le++)
    {
        data_t data = p_array->ptr[le];
        if(p_array->ptr[le] == NULL) 
            printf("[\t%3d\t]\n", (int)data);
        else 
            p_print_func(data);
    }

    return SUCCESS;
}

//Data structure 'two stack' destruction method
extern status_t destroy_two_stack_arr(const pp_arr_t pp_array, deleteDataProc p_delete_func)
{
    RETURN_FAILURE(ERROR_CHECKING(pp_array != NULL,
                                  pp_array == NULL,
                                  "ERROR : Array Object not found"));

    p_arr_t p_array = *pp_array;
    RETURN_FAILURE(check_array_object_condition(p_array, CHECK_OBJECT));

    puts("Destroying Array : ");
    for(int le = 0; le < p_array->arr_size; ++le)
        p_delete_func(p_array->ptr[le]);
    
    //free the allocated resource back to system in reverse order
    free(p_array->p_stack1); p_array->p_stack1 = NULL;
    free(p_array->p_stack2); p_array->p_stack2 = NULL;
    free(p_array->ptr); p_array->ptr = NULL;
    free(p_array); p_array = NULL;
    return SUCCESS;
}

//AUXILLARY FUNCTION DEFINITIONS
static void* xmalloc(size_t nr_bytes)
{
    void* ptr = (void*)malloc(nr_bytes);
    RETURN_NULL( ERROR_CHECKING(TRUE, NULL == ptr, "ERROR : Out of memory") );
    if(ptr)
        memset(ptr, 0, nr_bytes);
    return ptr;
}

static void* xrealloc(void* oldptr, size_t nr_bytes)
{
    void* newptr = (void*)realloc(oldptr, nr_bytes);
    RETURN_NULL( ERROR_CHECKING(TRUE, NULL == newptr, "ERROR : Out of memory") );
    return newptr;
}

static status_t error_checking(const int assert_condition,
                               const char* assert_condition_str,
                               const char* filename,
                               const int line_no,
                               const int error_condition,
                               const char* err_msg)
{
    status_t status = SUCCESS;
    #ifdef DEBUG
        //Assertion
        if(!assert_condition)
        {
            fprintf(stderr, "Assertion failed %s, File %s, Line %d\n", assert_condition_str,filename, line_no);
            status = FAILURE;
        }
    #else
        //Error checking
        if(error_condition && err_msg )
        {
            fprintf(stderr, err_msg); printf("\n");
            status = FAILURE;
        }
    #endif

    return status;
}

static status_t check_array_object_condition(const p_arr_t p_array, int conditions)
{
    status_t status = SUCCESS;

    if(CHECK_ARRAY & conditions)
            RETURN_FAILURE(ERROR_CHECKING(NULL != p_array || NULL != p_array->p_stack1 || NULL != p_array->p_stack2,
                                          NULL == p_array || NULL == p_array->p_stack1 || NULL == p_array->p_stack2,
                                          "ERROR : Array not found"));


    if(CHECK_DATA & conditions)
        RETURN_FAILURE(ERROR_CHECKING(NULL != p_array->ptr, 
        NULL == p_array->ptr, "ERROR : Array Data not found"));
    

    if(CHECK_STACK1_UNDERFLOW & conditions )
        RETURN_FAILURE(ERROR_CHECKING(p_array->p_stack1->nr_elements != 0,
                                p_array->p_stack1->nr_elements == 0, 
                                "ERROR : Stack 1 of Array Underflow"));

 

    if(CHECK_STACK2_UNDERFLOW & conditions )
        RETURN_FAILURE(ERROR_CHECKING(p_array->p_stack2->nr_elements != 0,
                                p_array->p_stack2->nr_elements == 0,
                                "ERROR : Stack 2 of Array Underflow"));


    if(CHECK_OVERFLOW & conditions )
        RETURN_FAILURE (ERROR_CHECKING(p_array->p_stack1->nr_elements + p_array->p_stack2->nr_elements != p_array->arr_size,
                                p_array->p_stack1->nr_elements + p_array->p_stack2->nr_elements == p_array->arr_size,
                                "ERROR : Two Stack of Arrays Overflows"));
    return status;
}