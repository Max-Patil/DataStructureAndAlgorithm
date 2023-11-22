//HEADER FILES
#include<stdio.h>
#include<stdlib.h>
#include"deque.h"

//MACROS DEFINITIONS
#define ERROR_CHECKING(assert_condition, error_condition, err_msg, error_handler, error_handler_params) \
        (error_checking(assert_condition,                                                         \
                       #assert_condition,                                                         \
                       __FILE__,                                                                  \
                       __LINE__,                                                                  \
                       error_condition,                                                           \
                       err_msg,                                                                   \
                       error_handler,                                                             \
                       error_handler_params))


#define DEQUE_DEFAULT_SIZE 512

#define RETURN_NULL(status)     { if(!status) return NULL; }
#define RETURN_FAILURE(status)  { if(!status) return FAILURE; }
#define EXIT_IF(condition)      { if(!condition) exit(EXIT_FAILURE); }

//ERROR CODES
#define CHECK_OBJECT    0x00000001
#define CHECK_ARRAY     0x00000002
#define CHECK_OVERFLOW  0x00000004
#define CHECK_UNDERFLOW 0x00000008
#define CHECK_OBJECT_ARRAY CHECK_OBJECT | CHECK_ARRAY

//AUXILLARY FUNCTION DECLARATIONS
static void* xmalloc(size_t nr_bytes);
static void* xcalloc(size_t nr_elements, size_t nr_bytes);
static status_t deque_error_checking(const int assert_condition,
                                     const char* assert_condition_str,
                                     const char* filename,
                                     const int line_no,
                                     const int error_condition,
                                     const char* err_msg,
                                     errorHandler perror_handler,
                                     data_t error_handler_params);

//INTERFACE FUNCTION DEFINTIONS
extern p_deque_t create_default_deque(void)
{
    p_deque_t p_deque = (p_deque_t)xmalloc(DEQUE_SIZE);

    EXIT_IF( ERROR_CHECKING(NULL != p_deque, NULL == p_deque, "ERROR : Deque cannot be created", NULL, NULL) );
    
    p_deque->p_array = (p_data_t)xcalloc(DEQUE_DEFAULT_SIZE, DATA_SIZE);

    p_deque->head = 0;
    p_deque->tail = 0;
    p_deque->length = DEQUE_DEFAULT_SIZE;
    p_deque->nr_elements = 0;

    return (p_deque);
}

extern p_deque_t create_custom_deque(size_t nr_elements)
{
    p_deque_t p_deque = (p_deque_t)xmalloc(DEQUE_SIZE);

    EXIT_IF( ERROR_CHECKING(NULL != p_deque, NULL == p_deque, "ERROR : Deque cannot be created", NULL, NULL ));

    p_deque->p_array = (p_data_t)xcalloc(nr_elements, DATA_SIZE);
    p_deque->nr_elements = nr_elements;
    p_deque->head = 0;
    p_deque->tail = 0;
    return p_deque;
}

//Data Insertions methods
extern status_t deque_enqueue_front(p_deque_t p_deque, data_t data)
{
    //Error checking for reliability
    RETURN_FAILURE(check_deque_conditions(p_deque, CHECK_OBJECT | CHECK_OVERFLOW) );

    //Data insertion
    p_deque->head--;
    if(-1 == p_deque->head)
        p_deque->head = p_deque->length - 1;
    
    p_deque->p_array[p_deque->head] = data;
    p_deque->nr_elements++;
    return SUCCESS;
}

extern status_t deque_enqueue_rear(p_deque_t p_deque, data_t data)
{
    //Error checking for reliability
    RETURN_FAILURE( check_deque_conditions(p_deque, CHECK_OBJECT | CHECK_OVERFLOW ) );

    //Data insertion
    p_deque->p_array[p_deque->tail++] = data;

    if(p_deque->tail == p_deque->length)
        p_deque->tail = 0;
    p_deque->nr_elements++;

    return SUCCESS;
}

//Data removal methods
extern data_t deque_dequeue_front(p_deque_t p_deque)
{
    //Error checking for reliability 
    RETURN_NULL( check_deque_conditions(p_deque, CHECK_OBJECT | CHECK_UNDERFLOW ) );

    //Data removal
    data_t popped_data = p_deque->p_array[p_deque->head++];
    if(p_deque->head == p_deque->length)
        p_deque->head = 0;

    p_deque->nr_elements--;
    return popped_data;
}

extern data_t deque_dequeue_rear(p_deque_t p_deque)
{
    //Error checking for reliability
    RETURN_NULL( check_deque_conditions(p_deque, CHECK_OBJECT | CHECK_UNDERFLOW ) );

    //Data removal
    if(--p_deque->tail == -1)
        p_deque->tail = p_deque->length - 1;

    data_t popped_data = p_deque->p_array[p_deque->tail];
    
    p_deque->nr_elements--;
    return popped_data;
}

//Data members accessor methods
extern size_t deque_size(const p_deque_t p_deque)
{
     RETURN_FAILURE( check_deque_conditions(p_deque, CHECK_OBJECT ) );
     return p_deque->length;
}

extern size_t deque_nr_elements(const p_deque_t p_deque)
{
    RETURN_FAILURE( check_deque_conditions(p_deque, CHECK_OBJECT) );
    return p_deque->length;
}

//Data display method
extern status_t deque_print( p_deque_t p_deque, printDataProc p_print_func)
{
    RETURN_FAILURE( check_deque_conditions(p_deque, CHECK_OBJECT) );

    RETURN_FAILURE( ERROR_CHECKING(p_print_func != NULL, p_print_func == NULL,
                    "ERROR : Print callback function not found", NULL, NULL));

    printf("\nDEQUE STATE\n");
    for(int le = 0; le < p_deque->nr_elements; le++)
    {
        int index = ( p_deque->head + le ) % p_deque->length;
        p_print_func(p_deque->p_array[index]);
    } puts("[END]\n");

    return SUCCESS;
}   

//Data structure destruction method 
extern status_t deque_destroy(pp_deque_t pp_deque, deleteDataProc p_delete_func)
{
    RETURN_FAILURE( ERROR_CHECKING(NULL != pp_deque, NULL == pp_deque,
                                  "ERROR : Deque not found", NULL, NULL) );

    p_deque_t p_deque = *pp_deque;
    RETURN_FAILURE(check_deque_conditions(p_deque, CHECK_OBJECT ));

    RETURN_FAILURE( ERROR_CHECKING(NULL != p_delete_func, NULL == p_delete_func,
                    "ERROR : Delete callback function not found", NULL, NULL) );

    for(int le = 0; le < p_deque->nr_elements; le++)
    {
        int index = ( p_deque->head + le ) % p_deque->length;
        p_delete_func(p_deque->p_array[index]);
    }

    if(p_deque && p_deque->p_array)
    {
        free(p_deque->p_array); p_deque->p_array = NULL;
        free(p_deque); p_deque = NULL;
    }
   
    *pp_deque = NULL;
    return SUCCESS;
}

//AUXILLARY FUNCTION DEFINITIONS
static status_t check_deque_conditions(p_deque_t p_deque, const int condition)
{
    status_t status = SUCCESS;
    if(condition & CHECK_OBJECT)
    {
        RETURN_FAILURE( ERROR_CHECKING(NULL != p_deque, NULL == p_deque,
                        "ERROR : Deque cannot be created", NULL, NULL ));
    }


    if(condition & CHECK_OVERFLOW )
    {
        RETURN_FAILURE( ERROR_CHECKING(p_deque->nr_elements != p_deque->length, p_deque->nr_elements == p_deque->length,
                        "ERROR : Deque is full", NULL, NULL ) );
    }

    if(condition & CHECK_UNDERFLOW )
    {
        RETURN_FAILURE( ERROR_CHECKING(p_deque->nr_elements != 0, p_deque->nr_elements == 0, 
                        "ERROR : Deque is underflow", NULL, NULL) );
    }
    return status;
}

static void* xmalloc(size_t nr_bytes)
{
    void* ptr = malloc(nr_bytes);
    EXIT_IF( ERROR_CHECKING(TRUE, ptr == NULL, "ERROR : Out of memory", NULL, NULL ) );
    if(ptr)
        memset(ptr, 0, nr_bytes);
    return ptr;
}

static void* xcalloc(size_t nr_elements, size_t nr_bytes)
{
    void* ptr = calloc(nr_elements, nr_bytes);
    EXIT_IF( ERROR_CHECKING(TRUE, NULL == ptr, "ERROR : Out of memory", NULL, NULL) );
    return ptr;
}

static status_t error_checking(const int assert_condition, const char* assert_condition_str,
                                     const char* filename, const int line_no,
                                     const int error_condition, const char* err_msg,
                                     errorHandler perror_handler, data_t error_handler_params)
{
    status_t status = SUCCESS;
    #ifdef DEBUG
    //Assertion 
        if(!assert_condition)
        {
            printf("Assertion failed %s, file %s, line_no %d\n", assert_condition_str, filename, line_no);
            status = FAILURE;
        }
    #else 
    //Error checking
        if(error_condition && err_msg)
        {
            fprintf(stderr,err_msg);
            fprintf(stderr,"\n");
            status = FAILURE;
        }
    #endif

    //Error handling
    if(status == FAILURE && perror_handler && error_handler_params)
        perror_handler(error_handler_params);
    
    return status;
}