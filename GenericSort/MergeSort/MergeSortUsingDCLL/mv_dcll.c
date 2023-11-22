/**
* @file : mv_dcll.c
* @author : MaxPatil (patilmitesh8746@gmail.com)

* @brief : This source file contains definition of doubly circular linked 
list interface functionalities as well as auxillary functionalities

* @date : 13-12-2022
* @version : 1.1
* @copyright   Copyrights (c) 2022
*/

//HEADER FILES
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"mv_dcll.h"

//SYMBOLIC CONSTANTS & MACROS
#define CHECK_LIST 0x00000001
#define CHECK_EMPTY 0x00000002
#define CHECK_LIST_EMPTY CHECK_LIST | CHECK_EMPTY

#define ERROR_CHECKING(assert_condition, error_condition, error_msg, error_handler, error_handler_params)(      \
    error_checking(assert_condition,                                                                            \
                  #assert_condition,                                                                            \
                  __FILE__,                                                                                     \
                  __LINE__,                                                                                     \
                  error_condition,                                                                              \
                  error_msg,                                                                                    \
                  error_handler,                                                                                \
                  error_handler_params))                                                                        \

#define ASSERT(assert_condition) (ERROR_CHECKING(assert_condition, FALSE, NULL, exit_program, NULL))

#define RETURN_NULL(status) { if(!status) return NULL; }
#define RETURN_FAILURE(status) { if(!status) return FAILURE; }

#define CREATE_NODE(new_node, new_data) { new_node = create_node(new_data); ASSERT(new_node != NULL); }

//LIST AUXILLARY FUNCTIONS

/**
* @func_name: exit_program
* @summary: This function will simply exit the whole program if it is invoked by Assertion
* @params: Void pointer
* @returns: Void pointer
*/
static void* exit_program(void* params){ exit(EXIT_FAILURE); return NULL; }

/**
* @func_name: xmalloc
* @summary: This function will simply allocated memory based on number of bytes requested
* @params: Number of bytes
* @returns: Allocated Memory
*/
static void* xmalloc(size_t nr_bytes)
{
    void* ptr = malloc(nr_bytes);
    ERROR_CHECKING(TRUE, NULL == ptr, "ERROR : Out of virtual memory", exit_program, NULL);
    if(ptr)
        memset(ptr, 0, nr_bytes);
    return ptr;
}

/**
* @func_name: error_checking
* @summary: This function will check for an error, display it and handle it appropriately
* @params: 1) Assert condition
           2) Assert condition as string
           3) filename in which assertion is failed
           4) Line number where assertion is occured
           5) Error condition
           6) Error message
           7) Error handler
           8) Error handler params
* @returns: Status of assertion condition or error condition based on mode
*/
static status_t error_checking(const int assert_condition,
                               const char* assert_condition_str,
                               const char* filename, 
                               const int lineno,
                               const int error_condition,
                               const char* error_msg,
                               void*(*error_handler)(void*),
                               void* error_handler_params)
{

    status_t status = SUCCESS;
    #ifdef MV_DEBUG
        //For assertion
        if(!assert_condition)
        {
            fprintf(stderr, "Assertion failed %s, File name %s, Line no. %d\n", 
                    assert_condition_str, filename, lineno);
            status = FAILURE;
        }
    #else
        if(error_condition && error_msg)
        {
            fprintf(stderr, error_msg);
            fprintf(stderr, "\n");
            status = FAILURE;
        }
    #endif

    if(status == FAILURE && error_handler)
        error_handler(error_handler_params);
    
    return status;
}

/**
* @func_name: check_list_object_condition
* @summary: This function will simply check for a presence of error in list object and invoke error_checking macro is present

* @params: 1) List
           2) Conditions

* @returns: Status of error checking function
*/
static status_t check_list_object_condition(const p_mv_dcll_t p_list, const int conditions)
{
    if(CHECK_LIST & conditions)
    {
        RETURN_FAILURE(ERROR_CHECKING(p_list != NULL, NULL == p_list, 
                       "ERROR : List object not found", NULL, NULL));
    }

    if(CHECK_EMPTY & conditions)
    {
        RETURN_FAILURE(ERROR_CHECKING(mv_is_dcll_empty(p_list) != TRUE, mv_is_dcll_empty(p_list) == TRUE,
                      "ERROR : List is empty", NULL, NULL));
    }

    return SUCCESS;
}

/**
* @func_name: generic_list_insert
* @summary: This function will simply insert new node in the list
* @params: 1) List
           2) First node i.e prev node of new node to be inserted
           3) Middle node i.e new node
           4) Last node i.e next node of new node to be inserted    
* @returns: Status of inserted operation 
*/
static status_t generic_list_insert(p_mv_dcll_t p_list, p_node_t first, p_node_t mid, p_node_t last)
{
    //Establishing the links 
    mid->p_prev = first;
    mid->p_next = last;
    first->p_next = mid;
    last->p_prev = mid;

    //Increase the size of the linked list
    p_list->size++;

    //Modify tail appropriately
    p_list->p_tail = p_list->p_head->p_prev;

    return SUCCESS;
}

/**
* @func_name: generic_list_delete
* @summary: This function will delete particular node from the list
* @params: 1) List
           2) Node to be deleted
* @returns: Data of deleted node else return NULL
*/
static data_t generic_list_remove(p_mv_dcll_t p_list, p_node_t p_delete)
{
    //Re-establishing links to delete node
    p_node_t p_delete_prev = p_delete->p_prev;
    p_node_t p_delete_next = p_delete->p_next;

    p_delete_prev->p_next = p_delete_next;
    p_delete_next->p_prev = p_delete_prev;

    p_delete->p_prev = p_delete->p_next = NULL;

    //Freeing the allocated resource
    data_t old_data = p_delete->data;
    free(p_delete); p_delete = NULL;

    //Decrease the size of the list
    p_list->size--;

    //Modify tail appropriately
    p_list->p_tail = p_list->p_head->p_prev;

    return old_data;
}

/**
* @func_name: create_node
* @summary: This function will simply allocate memory for node and assign it a value
* @params: New data
* @returns: Allocated memory for a node
*/
static p_node_t create_node(data_t new_data)
{
    p_node_t new_node = xmalloc(NODE_SIZE);

    //if xmalloc failed in debug mode, then it is need to be asserted by CREATE_NODE as xmalloc returns null
    RETURN_NULL(new_node);

    new_node->data = new_data;
    new_node->p_prev = new_node;
    new_node->p_next = new_node;

    return new_node;
}

/**
* @func_name: create_list
* @summary: This function will simply allocate memory for dcll list object
* @params: Takes nothing
* @returns: Allocated memory for dcll list object
*/
static p_mv_dcll_t create_list(void)
{
    p_mv_dcll_t p_list = xmalloc(LIST_SIZE);

    //if xmalloc fails in debug mode, then it is needed to be asserted by CREATE_NODE as xmalloc returned null
    RETURN_NULL(p_list);

    p_node_t sentinel = NULL;
    CREATE_NODE(sentinel, NULL);

    p_list->p_head = sentinel;
    p_list->p_tail = sentinel;
    p_list->size = 0;

    return p_list;
}

/**
* @func_name: establish_links_between
* @summary: This function will simply attach, the already deattached node
* @params: 1) p_node
           2) p_node_prev
* @returns: Status of newly established links
*/
extern status_t establish_links_between(p_node_t p_node_prev, p_node_t p_node)
{
    //Re-establish links between p_node & p_node_prev i.e overriding previous links
    p_node->p_prev = p_node_prev;
    p_node_prev->p_next = p_node;

    return SUCCESS;
}

//LIST INTERFACE FUNCTIONS

/**
* @function: create_mv_dcll_list
* @summary: This function will create pointer to dcll list object
* @params: takes nothing
* @returns: Pointer to dcll list object
*/
extern p_mv_dcll_t create_mv_dcll_list(void){   return create_list();  }
/**
* @function: mv_dcll_insert_back
* @summary: This function will add data to the back of the list
* @params: 1) List
           2) new data
* @returns: SUCCESS if inserted successfully else return FAILURE
*/
extern status_t mv_dcll_insert_back(p_mv_dcll_t p_list, data_t new_data)
{
    ASSERT(p_list != NULL);

    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data);

    return generic_list_insert(p_list, p_list->p_tail, new_node, p_list->p_tail->p_next);
}

/**
* @function: mv_dcll_insert_front
* @summary: This function will add data to front of the list
* @params: 1) List
           2) New data
* @returns: SUCCESS if inserted successfully else return FAILURE
*/
extern status_t mv_dcll_insert_front(p_mv_dcll_t p_list, data_t new_data)
{
    ASSERT(p_list != NULL);

    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data);

    return generic_list_insert(p_list, p_list->p_head, new_node, p_list->p_head->p_next);
}

/**
* @function: mv_dcll_remove_front
* @summary: This function will remove data from the front of the list
* @params: List
* @returns: Data to be removed else return NULL if any error occured
*/
extern data_t mv_dcll_remove_front(p_mv_dcll_t p_list)
{
    ASSERT(p_list != NULL || mv_is_dcll_empty(p_list) != TRUE);

    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST_EMPTY));

    return generic_list_remove(p_list, p_list->p_head->p_next);
}

/**
* @function: mv_dcll_remove_back
* @summary: This function will remove data from the back of the list
* @params: List
* @returns: Data to be removed else return NULL if any error occurred
*/
extern data_t mv_dcll_remove_back(p_mv_dcll_t p_list)
{
    ASSERT(p_list != NULL || mv_is_dcll_empty(p_list) != TRUE);

    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST_EMPTY));

    return generic_list_remove(p_list, p_list->p_tail);
}

//LIST GETTER METHODS
/**
* @function: mv_get_dcll_head
* @summary: This function will simply return data of first node of the dcll list
* @params: List
* @returns: Data if present else return null
*/
extern data_t mv_get_dcll_head(p_mv_dcll_t p_list)
{
    ASSERT(p_list != NULL || mv_is_dcll_empty(p_list) != TRUE);

    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST_EMPTY));

    return p_list->p_head->p_next->data;
}

/**
* @function: mv_get_dcll_tail
* @summary: This function will simply return data of last node of the dcll list
* @params: List
* @returns: Data if present else return null
*/
extern data_t mv_get_dcll_tail(p_mv_dcll_t p_list)
{
    ASSERT(p_list != NULL || mv_is_dcll_empty(p_list) != TRUE);

    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST_EMPTY));

    return p_list->p_tail->data;
}

/**
* @function: mv_get_dcll_size
* @summary: This function will simply return number of data element present in the dcll list
* @params: List
* @returns: Size of the dcll list
*/
extern size_t mv_get_dcll_size(p_mv_dcll_t p_list)
{
    RETURN_FAILURE(ERROR_CHECKING(p_list != NULL, NULL == p_list, 
                  "ERROR : List object not found", NULL, NULL));
    
    return p_list->size;
}

/**
* @function: mv_is_dcll_empty
* @summary: This function will simply check whether dcll list is empty or not
* @params: List
* @returns: TRUE if list is empty else return FALSE
*/
extern inline status_t mv_is_dcll_empty(p_mv_dcll_t p_list){ return mv_get_dcll_size(p_list) == 0; }

/**
* @function: mv_is_list_sorted
* @summary: This function will simply whether dcll list is empty, sorted or not 
* @params: 1) List
           2) Compare callback function pointer
* @returns: TRUE if list is sorted else return FALSE
*/
extern status_t mv_is_list_sorted(p_mv_dcll_t p_list, compareDataProc p_compare_func)
{
    ASSERT(p_list != NULL || mv_is_dcll_empty(p_list) != TRUE);

    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST_EMPTY));

    if(mv_get_dcll_size(p_list) == ONE_ELEMENT)
    {
        fprintf(stderr, "ERROR : List is already sorted, as one element is present in the list\n");
        return TRUE;
    }

    p_node_t p_run = p_list->p_head->p_next->p_next;
    p_node_t p_run_prev = p_run->p_prev;

    while(p_run != p_list->p_head)
    {
        p_run_prev = p_run->p_prev;
        
        if(FAILURE == p_compare_func(mv_get_node_data(p_run_prev), mv_get_node_data(p_run)))
            break;
        p_run = p_run->p_next;
    }

    return p_run == p_list->p_head;
}

/**
* @function: mv_get_node_data
* @summary: This function will simply return data of that node
* @params: Node
* @returns: Valid data if list is non-empty else return Invalid data as Null
*/
inline extern data_t mv_get_node_data(p_node_t p_node)
{
    return p_node->data;
}

/**
* @function: mv_dcll_print
* @summary: This function will display the state of the dcll list
* @params: 1) List
           2) Print callback function pointer
           3) Message provided by client
* @returns: Status of display operation
*/
extern status_t mv_dcll_show(p_mv_dcll_t p_list, printDataProc p_print_func, const char* msg)
{
    //Error checking
    ASSERT(p_list != NULL || mv_is_dcll_empty(p_list) != TRUE);

    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST_EMPTY));

    RETURN_FAILURE(ERROR_CHECKING(p_print_func != NULL, NULL == p_print_func,
                  "ERROR : Print callback function is not provided", NULL, NULL));
    
    if(msg) puts(msg);
    printf("[START]\n");

    p_node_t p_run = p_list->p_head->p_next;
    while(p_run != p_list->p_head)
    {
        p_print_func(p_run->data);
        p_run = p_run->p_next;
    }

    printf("[END]\n\n");

    return SUCCESS;
}

/**
* @function: destroy_mv_dcll
* @summary: This function will destroy the dcll list completely
* @params: 1) Pointer to List
           2) Delete Callback function pointer
* @returns: Status of destroyed dcll list object
*/
extern status_t destroy_mv_dcll(pp_mv_dcll_t pp_list, deleteDataProc p_delete_func)
{
    RETURN_FAILURE(ERROR_CHECKING(pp_list != NULL, NULL == pp_list,
                "ERROR : List object not found", NULL, NULL));

    p_mv_dcll_t p_list = *pp_list;

    RETURN_FAILURE(ERROR_CHECKING(p_list != NULL, NULL == p_list,
                   "ERROR : List object not found", NULL, NULL))

    RETURN_FAILURE(ERROR_CHECKING(p_delete_func != NULL, NULL == p_delete_func,
                  "ERROR : Delete callback function is not provided", NULL, NULL));
    
    p_node_t p_run = p_list->p_head->p_next, p_run_next = NULL;

    while(p_run != p_list->p_head)
    {
        p_run_next = p_run->p_next;

        p_delete_func(p_run->data);
        free(p_run); p_run = NULL;

        p_run = p_run_next;
    }

    if(p_list->p_head)
        free(p_list->p_head); p_list->p_head = NULL;

    if(*pp_list)
        free(*pp_list); *pp_list = NULL;

    return SUCCESS;
}
