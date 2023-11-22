//HEADER FILES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scll.h"

//MACROS & SYMBOLIC CONSTANTS
#define CHECK_LIST 0x00000001
#define CHECK_EMPTY 0x00000002
#define CHECK_LIST_EMPTY CHECK_LIST | CHECK_EMPTY
#define END_LINE (puts("-------------------------------------------------\n{\t\tEND\t\t\t\t}\n\n"))
#define START_LINE (puts("{\t\tSTART\t\t\t\t}\n-------------------------------------------------\n"))

#define ERROR_CHECKING(assert_condition, error_condition, err_msg, error_handler, error_handler_params)(    \
    error_checking(assert_condition,                                                                        \
                   #assert_condition,                                                                       \
                   __FILE__,                                                                                \
                   __LINE__,                                                                                \
                   error_condition,                                                                         \
                   err_msg,                                                                                 \
                   error_handler,                                                                           \
                   error_handler_params))

#define ASSERT(assert_condition) (ERROR_CHECKING(assert_condition, FALSE, NULL, exit_program, NULL))
#define CREATE_NODE(new_node, new_data) { new_node = create_node(new_data); ASSERT(new_node != NULL);}

#define RETURN_FAILURE(status)  { if(!status) return FAILURE; }
#define RETURN_NULL(status)     { if(!status) return NULL; }

//DEFINITION OF AUXILLARY FUNCTIONS
static void* exit_program(void* params)
{
    exit(EXIT_FAILURE); return NULL;
}


static void* xmalloc(size_t nr_bytes)
{
    void* ptr = malloc(nr_bytes);
    ERROR_CHECKING(TRUE, ptr == NULL, "ERROR : Out of memory", exit_program, NULL);
    memset(ptr, 0, nr_bytes);
    return ptr;
}

/**
* @function: error_checking
* @summary: This function will check error, triggers error handler and also do assertion
* @params: 1) Assert condition
           2) Assert condition string
           3) File name in which assertion fails
           4) Line number at which assertion fails
           5) Error condition
           6) Error message
           7) Error handler
           8) Error handler parameters
* @returns: status of error condition or assert condition
*/
error_checking(const int assert_condition,
               const char* assert_condition_str,
               const char* filename,
               const int line_no,
               const int error_condition,
               const char* err_msg,
               void*(*error_handler)(void*),
               void* error_handler_params)
{
    status_t status = SUCCESS;
    #ifdef DEBUG
        //Assertion
        if(!assert_condition)
        {
            fprintf(stderr, "Assertion Failed: %s, File %s, Line %d\n", 
                    assert_condition_str, filename, line_no);
            status = FAILURE;
        }
    #else
        //Error checking
        if(error_condition && err_msg)
        {
            fprintf(stderr, err_msg);
            fprintf(stderr,"\n");
            status = FAILURE;
        }
    #endif

    if(status == FAILURE && error_handler)
        error_handler(error_handler_params);

    return status;
}

/**
* @function: check_list_object_condition
* @summary: This function will check the state of object and if error is found
then trigger appropriate error handler
* @params: 1) List
           2) Conditions
* @returns: state of list object
*/
static status_t check_list_object_condition(p_scll_t p_list, const int conditions)
{
    if(CHECK_LIST & conditions)
    {
        RETURN_FAILURE(ERROR_CHECKING(TRUE, NULL == p_list, "ERROR : List not found",
                                    NULL, NULL));
    }

    if(CHECK_EMPTY & conditions)
    {
        RETURN_FAILURE(ERROR_CHECKING(TRUE, p_list->p_head == p_list->p_tail, 
                                    "ERROR : List not found", NULL, NULL));
    }
    return SUCCESS;
}

static data_t search_existing_node(p_scll_t p_list, data_t e_data, compareDataProc p_compare_func)
{
    p_node_t p_run = p_list->p_head->p_next, e_node = NULL;
    while(p_run != p_list->p_head)
    {
        if(SUCCESS == p_compare_func(p_run->data, e_data))
        {
            e_node = p_run;
            break;
        }
        p_run = p_run->p_next;
    }
    return e_node;
}

static data_t search_existing_node_before(p_scll_t p_list, data_t e_data, compareDataProc p_compare_func)
{
     p_node_t p_run = p_list->p_head, e_node = NULL;
    while(p_run->p_next != p_list->p_head)
    {
        if(SUCCESS == p_compare_func(p_run->p_next->data, e_data))
        {
            e_node = p_run;
            break;
        }
        p_run = p_run->p_next;
    }
    return e_node;
}

static p_node_t create_node(data_t new_data)
{
    p_node_t new_node = (p_node_t)xmalloc(NODE_SIZE);
    RETURN_NULL(new_node);

    //Assign new data to key of new node
    new_node->data = new_data;

    //Assign self to self's next
    new_node->p_next = new_node;

    return new_node;
}

static p_scll_t create_list(void)
{
    p_scll_t p_list = (p_scll_t)xmalloc(LIST_SIZE);

    RETURN_NULL(p_list);

    p_node_t sentinel = NULL;
    CREATE_NODE(sentinel, NULL);

    p_list->p_head = p_list->p_tail = sentinel;
    p_list->nr_elements = 0;

    return p_list;
}

static status_t generic_list_insert(p_scll_t p_list, p_node_t new_node, p_node_t e_node)
{
    //Increment the size of list
    p_list->nr_elements++;

     //Modifying tail appropriately
    if(e_node == p_list->p_tail)
        p_list->p_tail = new_node;

    //Inserting the new node after the existing node
    new_node->p_next = e_node->p_next;
    e_node->p_next = new_node;
    
    return SUCCESS;
}

static data_t generic_list_remove(p_scll_t p_list, p_node_t e_node)
{
    p_node_t p_delete = e_node->p_next;
  
    //Attaching and deattaching adjacent links of node to be deleted
    e_node->p_next = p_delete->p_next;
    p_delete->p_next = NULL;

    //Decrementing number of elements
    p_list->nr_elements--;

    data_t old_data = p_delete->data;

    //Manipulate tail appropriately
    if(p_list->p_tail == p_delete)
        p_list->p_tail = e_node;
   
    free(p_delete); p_delete = NULL;
    
    return old_data;
}

//LIST INTERFACE ROUTINES

extern p_scll_t create_singly_circular_list(void) { return (create_list()); }


extern status_t scll_insert_back(p_scll_t p_list, data_t new_data)
{
    ASSERT(p_list != NULL);
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data);

    return generic_list_insert(p_list, new_node, p_list->p_tail);
}

extern status_t scll_insert_front(p_scll_t p_list, data_t new_data)
{
    ASSERT(p_list != NULL);
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data);

    return generic_list_insert(p_list, new_node, p_list->p_head);
}

extern status_t scll_insert_at(p_scll_t p_list, data_t new_data, size_t index)
{
    ASSERT(p_list != NULL);
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    if(index == 0)
        return (scll_insert_front(p_list, new_data));
    else if(index == p_list->nr_elements)
        return (scll_insert_back(p_list, new_data));

    RETURN_FAILURE(ERROR_CHECKING(index < p_list->nr_elements, index > p_list->nr_elements,
                                 "ERROR : Index out of bound", NULL, NULL));
    p_node_t p_run = p_list->p_head;

    for(size_t le = 0; le < index; le++)
        p_run = p_run->p_next;

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data);

    return (generic_list_insert(p_list, new_node, p_run));
}

extern status_t scll_insert_before(p_scll_t p_list, data_t new_data, data_t e_data, compareDataProc p_compare_func)
{
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    RETURN_FAILURE(ERROR_CHECKING(NULL != p_compare_func, NULL == p_compare_func,
                                  "ERROR : Compare callback function", NULL, NULL));
   
    p_node_t e_node = search_existing_node_before(p_list, e_data, p_compare_func);
    RETURN_FAILURE(e_node);

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data);

    return (generic_list_insert(p_list, new_node, e_node));
}

extern status_t scll_insert_after(p_scll_t p_list, data_t new_data, data_t e_data, compareDataProc p_compare_func)
{
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    RETURN_FAILURE(ERROR_CHECKING(NULL != p_compare_func, NULL == p_compare_func,
                                  "ERROR : Compare callback function", NULL, NULL));

    p_node_t e_node = search_existing_node(p_list, e_data, p_compare_func);
    RETURN_FAILURE(e_node);

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data);

    return (generic_list_insert(p_list, new_node, e_node));
}

//LIST REMOVAL OPERATIONS

extern data_t scll_remove_back(p_scll_t p_list)
{
     RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY));

    p_node_t p_run = p_list->p_head->p_next;
   
    while(p_run->p_next != p_list->p_tail)
        p_run = p_run->p_next;
    
     return (generic_list_remove(p_list, p_run));
}

extern data_t scll_remove_front(p_scll_t p_list)
{
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY));
    return (generic_list_remove(p_list, p_list->p_head));
}

extern data_t scll_remove_at(p_scll_t p_list, size_t index)
{
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY));
    if(index == 0)
        return (scll_remove_front(p_list));
    else if(index == p_list->nr_elements)
        return (scll_remove_back(p_list));

    RETURN_FAILURE(ERROR_CHECKING(index < p_list->nr_elements, index > p_list->nr_elements,
                                 "ERROR : Index out of range", NULL, NULL));

    p_node_t p_run = p_list->p_head;
    for(size_t le = 0; le < index; le++)
        p_run = p_run->p_next;
    
    return (generic_list_remove(p_list, p_run));
}

extern data_t scll_remove_after(p_scll_t p_list, data_t e_data, compareDataProc p_compare_func)
{
    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY));
    RETURN_NULL(ERROR_CHECKING(NULL != p_compare_func, NULL == p_compare_func,
                                  "ERROR : Compare callback function", NULL, NULL));

    if(p_compare_func(e_data, p_list->p_tail->data) == SUCCESS)
        return scll_remove_front(p_list);

    p_node_t e_node = search_existing_node(p_list, e_data, p_compare_func);
    RETURN_NULL(e_node);
    if(p_list->p_tail == e_node)
        return scll_remove_front(p_list);

    return (generic_list_remove(p_list, e_node));
}

extern data_t scll_remove_before(p_scll_t p_list, data_t e_data, compareDataProc p_compare_func)
{
    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST_EMPTY));

    if(p_compare_func(e_data, p_list->p_head->data) == SUCCESS)
        return scll_remove_back(p_list);

    p_node_t e_node_prev = search_existing_node_before(p_list, e_data, p_compare_func);
    RETURN_NULL(e_node_prev);

    p_node_t e_node_prev_prev = search_existing_node_before(p_list, e_node_prev->data, p_compare_func);
    RETURN_NULL(e_node_prev_prev);

    return (generic_list_remove(p_list, e_node_prev_prev));
}

//LIST GETTER FUNCTIONS

extern data_t scll_get_head(p_scll_t p_list)
{
    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST  | CHECK_EMPTY));
    return (p_list->p_head->p_next->data);
}

extern data_t scll_get_tail(p_scll_t p_list)
{
    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY));
    return (p_list->p_tail->data);
}

extern size_t scll_get_size(p_scll_t p_list)
{
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));
    return (p_list->nr_elements);
}

extern data_t scll_get_data_at(p_scll_t p_list, size_t index)
{
    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY));
    RETURN_NULL(ERROR_CHECKING(index < p_list->nr_elements, index >= p_list->nr_elements,
                              "ERROR : Index out of bound", NULL, NULL));
    p_node_t p_run = p_list->p_head;
    for(size_t le = 0; le <= index; ++le)
        p_run = p_run->p_next;
    return (p_run->data);
}

extern status_t scll_is_scll_empty(p_scll_t p_list)
{
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));
    return(p_list->p_head == p_list->p_tail);
}

extern status_t scll_print_forward(p_scll_t p_list, printDataProc p_print_func, const char* msg)
{
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));
    RETURN_FAILURE(ERROR_CHECKING(NULL != p_print_func, NULL == p_print_func, 
                                 "ERROR : print callback function not found", NULL, NULL));
    if(msg) puts(msg);
    puts("Singly Circular Linked List in forward order: ");

    START_LINE;
    for(p_node_t p_run = p_list->p_head->p_next; p_run != p_list->p_head; p_run = p_run->p_next)
        p_print_func(p_run->data);
    END_LINE;
    return SUCCESS;
}

extern status_t destroy_scll(pp_scll_t pp_list, deleteDataProc p_delete_func)
{
    RETURN_FAILURE(ERROR_CHECKING(pp_list != NULL, pp_list == NULL,
                                  "ERROR : List not found", NULL, NULL));
    
    p_scll_t p_list = *pp_list;
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    p_node_t p_run = p_list->p_head->p_next, p_run_next = NULL;
    while(p_run != p_list->p_head)
    {
        p_run_next = p_run->p_next;

        p_delete_func(p_run->data);
        free(p_run); p_run = NULL;

        p_run = p_run_next;
    }

    if(p_list->p_head)
    {
        free(p_list->p_head); p_list->p_head = NULL;
    }
    p_list->p_tail = NULL;
    free(*pp_list); *pp_list = NULL;
    
    return SUCCESS;
}
