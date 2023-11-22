//HEADER FILES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dcll.h"

//SYMBOLIC CONSTANTS & MACROS
#define CHECK_LIST  0x00000001
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

#define RETURN_NULL(status)    { if(!status) return NULL; }
#define RETURN_FAILURE(status) { if(!status) return FAILURE; }

#define CREATE_NODE(new_node, new_data) { new_node = create_node(new_data); ASSERT(new_node != NULL); }

#define END_LINE (puts("-------------------------------------------------\n{\t\tEND\t\t\t\t}\n\n"))
#define START_LINE (puts("{\t\tSTART\t\t\t\t}\n-------------------------------------------------\n"))

//LIST AUXILLARY FUNCTIONS

/**
* @func_name: exit_program
* @summary: This function will simply exit the whole program if it is invoked by Assertion
* @params:  void pointer
* @returns: void pointer
*/
static void* exit_program(void* params){ exit(EXIT_FAILURE); return NULL; }

/**
* @func_name: xmalloc
* @summary: This function will simply allocated memory based on number of bytes requested
* @params:  number of bytes
* @returns: allocated memory
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
    #ifdef DEBUG
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
            printf("%s\n", error_msg);
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
static status_t check_list_object_condition(const p_dcll_t p_list, const int conditions)
{
    if(CHECK_LIST & conditions)
    {
        RETURN_FAILURE(ERROR_CHECKING(p_list != NULL, NULL == p_list, 
                       "ERROR : List object not found", NULL, NULL));
    }

    if(CHECK_EMPTY & conditions)
    {
        RETURN_FAILURE(ERROR_CHECKING(TRUE, p_list->p_head == p_list->p_tail,
                      "ERROR : List is empty", NULL, NULL));
    }

    return SUCCESS;
}

/**
* @function: search_existing_node
* @summary: This function will search for existing data in dcll list
* @params: 1) List
           2) existing data
           3) Compare callback function pointer
* @returns: Existing data if found else return null
*/
static p_node_t search_existing_node(p_dcll_t p_list, data_t e_data, compareDataProc p_compare_func)
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

/**
* @func_name: generic_list_insert
* @summary: This function will simply insert new node in the list
* @params: 1) List
           2) First node i.e prev node of new node to be inserted
           3) Middle node i.e new node
           4) Last node i.e next node of new node to be inserted    
* @returns: Status of inserted operation 
*/
static status_t generic_list_insert(p_dcll_t p_list, p_node_t first, p_node_t mid, p_node_t last)
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
static data_t generic_list_remove(p_dcll_t p_list, p_node_t p_delete)
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
* @func_name: deattach_node
* @summary: This function will simply deattach node 
* @params: 1) List
           2) Node to be deattached
* @returns: deattached node
*/
inline extern p_node_t deattach_node(p_dcll_t p_list, p_node_t p_node)
{
    //Re-establish links of p_node
    p_node_t p_node_prev = p_node->p_prev, p_node_next = p_node->p_next;

    p_node_prev->p_next = p_node_next;
    p_node_next->p_prev = p_node_prev;
    
    //Handling tail appropriately
    p_list->p_tail = p_list->p_head->p_prev;

    //In order to deattached p_node from the list, it is neccessary to set its pointer to null
    p_node->p_prev = p_node->p_next = NULL;

    return p_node;
}

/**
* @func_name: attach_node
* @summary: This function will simply attach, the already deattached node
* @params: 1) List
           2) Deattached node
           3) Node before which deattached node to be inserted
* @returns: Status of node to be attached in the list
*/
inline extern status_t attach_node(p_dcll_t p_list, p_node_t p_deattached, p_node_t p_node_prev)
{  
    //Checking if node that is need to be attached is deattached already
    if(p_deattached->p_next != p_deattached->p_prev)
    {
        fprintf(stderr, "ERROR : Node to be attached is not already deattached\n");
        return FAILURE;
    }
    
    //Insert deattached node to the next of p_node_prev
    p_deattached->p_next = p_node_prev->p_next;
    p_deattached->p_prev = p_node_prev;
    
    p_node_prev->p_next = p_deattached;
    p_deattached->p_next->p_prev = p_deattached;

    //Handling tail appropriately
    p_list->p_tail = p_list->p_head->p_prev;

    return SUCCESS;
}

/**
* @function: dcll_get_data
* @summary: This function will simply return data that is occupied by given node
* @params: Node
* @returns: Data if present else return NULL
*/
extern inline data_t dcll_get_data(p_node_t p_node)
{
    return p_node->data;
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

    //In debug mode, if xmalloc does not allocate memory then it needed to be asserted
    RETURN_NULL(new_node);

    //Assigning new data to new node
    new_node->data = new_data;

    //Assigning new data to new node
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
static p_dcll_t create_list(void)
{
    p_dcll_t p_list = xmalloc(LIST_SIZE);

    //In debug mode, if xmalloc does not allocate memory then it is needed to be asserted
    RETURN_NULL(p_list);

    //Create sentinel node/ dummy node
    p_node_t sentinel = NULL;
    CREATE_NODE(sentinel, NULL);

    //Assign the sentinel node to head and tail of newly created dcll
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
* @function: create_dcll_list
* @summary: This function will create pointer to dcll list object
* @params: takes nothing
* @returns: Pointer to dcll list object
*/
extern p_dcll_t create_dcll_list(void){   return create_list();  }
/**
* @function: dcll_insert_back
* @summary: This function will add data to the back of the list
* @params: 1) List
           2) new data
* @returns: SUCCESS if inserted successfully else return FAILURE
*/
extern status_t dcll_insert_back(p_dcll_t p_list, data_t new_data)
{
    ASSERT(p_list != NULL);
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));
 
    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data);

    return generic_list_insert(p_list, p_list->p_tail, new_node, p_list->p_tail->p_next);
}

/**
* @function: dcll_insert_front
* @summary: This function will add data to front of the list
* @params: 1) List
           2) New data
* @returns: SUCCESS if inserted successfully else return FAILURE
*/
extern status_t dcll_insert_front(p_dcll_t p_list, data_t new_data)
{
    ASSERT(p_list != NULL);

    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data);

    return generic_list_insert(p_list, p_list->p_head, new_node, p_list->p_head->p_next);
}

/**
* @function: dcll_insert_at
* @summary: This function will insert new data at specified index
* @params: 1) List
           2) New data
           3) Index
* @returns: SUCCESS if inserted successfully at specified index
*/

extern status_t dcll_insert_at(p_dcll_t p_list, data_t new_data, size_t index)
{
    ASSERT(NULL != p_list);
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    if(index == 0)
        return (dcll_insert_front(p_list, new_data));
    else if(index == p_list->size)
        return (dcll_insert_back(p_list, new_data));

    RETURN_FAILURE(ERROR_CHECKING(index < p_list->size, index > p_list->size,
                   "ERROR : Index out of bound", NULL, NULL));
    
    p_node_t p_run = p_list->p_head;

    for(size_t le = 0; le < index; ++le)
        p_run = p_run->p_next;

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data);

    return (generic_list_insert(p_list, p_run, new_node, p_run->p_next));
}

/**
* @function: dcll_insert_after
* @summary: This function will insert new data after the existing data
* @params: 1) List
           2) New data
           3) Existing data
           4) Compare callback function pointer
* @returns: SUCCESS if inserted new data after existing data else return FAILURE
*/
extern status_t dcll_insert_after(p_dcll_t p_list, data_t new_data, data_t e_data, compareDataProc p_compare_func)
{
    //Error checking and assertions
    ASSERT(p_list != NULL);

    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    RETURN_FAILURE(ERROR_CHECKING(NULL != p_compare_func, NULL == p_compare_func, 
                  "ERROR : Compare callback function not found", NULL, NULL));
    
    p_node_t e_node = search_existing_node(p_list, e_data, p_compare_func);

    RETURN_FAILURE(ERROR_CHECKING(e_node != NULL, e_node == NULL, 
                                 "ERROR : Existing data not found", NULL, NULL));

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data);

    return (generic_list_insert(p_list, e_node, new_node, e_node->p_next));
}

/**
* @function: dcll_insert_before
* @summary: This function will insert new data before existing data
* @params: 1) List
           2) New data
           3) Existing data
           4) Compare callback function pointer
* @returns: SUCCESS if inserted new data before existing data else return FAILURE
*/
extern status_t dcll_insert_before(p_dcll_t p_list, data_t new_data, data_t e_data, compareDataProc p_compare_func)
{
    ASSERT(NULL != p_list);
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    RETURN_FAILURE(ERROR_CHECKING(NULL != p_compare_func, NULL == p_compare_func,
                  "ERROR : Compare callback function is not provided", NULL, NULL));
    
    p_node_t e_node = search_existing_node(p_list, e_data, p_compare_func);
    RETURN_FAILURE(ERROR_CHECKING(NULL != e_node, NULL == e_node, 
                   "ERROR : Existing data not found", NULL, NULL));
    
    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data);
    return (generic_list_insert(p_list, e_node->p_prev, new_node, e_node));
}


/**
* @function: dcll_remove_front
* @summary: This function will remove data from the front of the list
* @params: List
* @returns: Data to be removed else return NULL if any error occured
*/
extern data_t dcll_remove_front(p_dcll_t p_list)
{
    ASSERT(p_list != NULL || is_dcll_empty(p_list) != TRUE);

    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST_EMPTY));

    return generic_list_remove(p_list, p_list->p_head->p_next);
}

/**
* @function: dcll_remove_back
* @summary: This function will remove data from the back of the list
* @params: List
* @returns: Data to be removed else return NULL if any error occurred
*/
extern data_t dcll_remove_back(p_dcll_t p_list)
{
    ASSERT(p_list != NULL || is_dcll_empty(p_list) != TRUE);

    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST_EMPTY));

    return generic_list_remove(p_list, p_list->p_tail);
}

/**
* @function: dcll_remove_at
* @summary: This function will remove data of specified index
* @params: 1) List
           2) Index
* @returns: Data if removed successfully else return null
*/
extern data_t dcll_remove_at(p_dcll_t p_list, size_t index)
{
    ASSERT(NULL != p_list || is_dcll_empty(p_list) != TRUE);

    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY));

    RETURN_NULL(ERROR_CHECKING(index < p_list->size, index >= p_list->size, 
                "ERROR : Index out of bound", NULL, NULL));
    
    //Note : we have started from 0th index i.e our head node and went till node that denote our index in list
    p_node_t p_run = p_list->p_head;
    for(size_t le = 0; le <= index; le++)
        p_run = p_run->p_next;

    return (generic_list_remove(p_list, p_run));
}
/**
* @function: dcll_remove_before 
* @summary: This function will remove data before the existing data
* @params: 1) List
           2) Existing data
           3) Compare callback function pointer 
* @returns: Data if removed successfully before existing data else return null
*/
extern data_t dcll_remove_before(p_dcll_t p_list, data_t e_data, compareDataProc p_compare_func)
{
    ASSERT(NULL != p_list || is_dcll_empty(p_list) != TRUE);

    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY));

    RETURN_NULL(ERROR_CHECKING(p_compare_func != NULL, p_compare_func == NULL, 
                "ERROR : Compare callback function is not provided", NULL, NULL));

    p_node_t e_node = search_existing_node(p_list, e_data, p_compare_func);
    RETURN_NULL(ERROR_CHECKING(e_node != NULL, e_node == NULL, 
               "ERROR : Existing data not found", NULL, NULL));

    RETURN_NULL(ERROR_CHECKING(e_node->p_prev != p_list->p_head, e_node->p_prev == p_list->p_head,
                "ERROR : No node exist before first node", NULL, NULL));

    return (generic_list_remove(p_list, e_node->p_prev));
}

/**
* @function: dcll_remove_after
* @summary: This function will remove data after the existing data
* @params: 1) List
           2) Existing data
           3) Compare callback function pointer
* @returns: Data if removed successfully after existing data else return null
*/
extern data_t dcll_remove_after(p_dcll_t p_list, data_t e_data, compareDataProc p_compare_func)
{
    ASSERT(NULL != p_list || is_dcll_empty(p_list) != TRUE);

    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY));

    RETURN_NULL(ERROR_CHECKING(p_compare_func != NULL, p_compare_func == NULL, 
                "ERROR : Compare callback function is not provided", NULL, NULL));

    p_node_t e_node = search_existing_node(p_list, e_data, p_compare_func);
    RETURN_NULL(ERROR_CHECKING(e_node != NULL, e_node == NULL, 
               "ERROR : Existing data not found", NULL, NULL));

    RETURN_NULL(ERROR_CHECKING(e_node->p_next != p_list->p_tail, e_node->p_next == p_list->p_tail,
                "ERROR : No node exist after last node", NULL, NULL));

    return (generic_list_remove(p_list, e_node->p_next));
}

//LIST GETTER METHODS
/**
* @function: get_dcll_head
* @summary: This function will simply return data of first node of the dcll list
* @params: List
* @returns: Data if present else return null
*/
extern data_t get_dcll_head(p_dcll_t p_list)
{
    ASSERT(p_list != NULL || is_dcll_empty(p_list) != TRUE);

    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST_EMPTY));

    return p_list->p_head->p_next->data;
}

/**
* @function: get_dcll_tail
* @summary: This function will simply return data of last node of the dcll list
* @params: List
* @returns: Data if present else return null
*/
extern data_t get_dcll_tail(p_dcll_t p_list)
{
    ASSERT(p_list != NULL || is_dcll_empty(p_list) != TRUE);

    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST_EMPTY));

    return p_list->p_tail->data;
}

/**
* @function: get_dcll_size
* @summary: This function will simply return number of data element present in the dcll list
* @params: List
* @returns: Size of the dcll list
*/
extern size_t get_dcll_size(p_dcll_t p_list)
{
    RETURN_FAILURE(ERROR_CHECKING(p_list != NULL, NULL == p_list, 
                  "ERROR : List object not found", NULL, NULL));
    
    return p_list->size;
}

/**
* @function: is_dcll_empty
* @summary: This function will simply check whether dcll list is empty or not
* @params: List
* @returns: TRUE if list is empty else return FALSE
*/
extern status_t is_dcll_empty(p_dcll_t p_list){ return get_dcll_size(p_list) == 0; }

/**
* @function: is_list_sorted
* @summary: This function will simply whether dcll list is empty, sorted or not 
* @params: 1) List
           2) Compare callback function pointer
* @returns: TRUE if list is sorted else return FALSE
*/
extern status_t is_list_sorted(p_dcll_t p_list, compareDataProc p_compare_func)
{
    ASSERT(p_list != NULL || is_dcll_empty(p_list) != TRUE);

    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST_EMPTY));

    if(get_dcll_size(p_list) == ONE_ELEMENT)
    {
        fprintf(stderr, "ERROR : List is already sorted, as one element is present in the list\n");
        return TRUE;
    }

    p_node_t p_run = p_list->p_head->p_next->p_next;
    p_node_t p_run_prev = p_run->p_prev;

    while(p_run != p_list->p_head)
    {
        p_run_prev = p_run->p_prev;
        
        if(FAILURE == p_compare_func(get_node_data(p_run_prev), get_node_data(p_run)))
            break;
        p_run = p_run->p_next;
    }

    return p_run == p_list->p_head;
}

/**
* @function: get_node_data
* @summary: This function will simply return data of that node
* @params: Node
* @returns: Valid data if list is non-empty else return Invalid data as Null
*/
inline extern data_t get_node_data(p_node_t p_node)
{
    return p_node->data;
}

/**
* @function: dcll_print
* @summary: This function will display the state of the dcll list
* @params: 1) List
           2) Print callback function pointer
           3) Message provided by client
* @returns: Status of display operation
*/
extern status_t dcll_show(p_dcll_t p_list, printDataProc p_print_func, const char* msg)
{
    //Error checking
    ASSERT(p_list != NULL || is_dcll_empty(p_list) != TRUE);
  
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    RETURN_FAILURE(ERROR_CHECKING(p_print_func != NULL, NULL == p_print_func,
                  "ERROR : Print callback function is not provided", NULL, NULL));
    
    if(msg) puts(msg);

    START_LINE;
    p_node_t p_run = p_list->p_head->p_next;
    while(p_run != p_list->p_head)
    {
        p_print_func(p_run->data);
        p_run = p_run->p_next;
    }

    END_LINE;

    return SUCCESS;
}

/**
* @function: destroy_dcll
* @summary: This function will destroy the dcll list completely
* @params: 1) Pointer to List
           2) Delete Callback function pointer
* @returns: Status of destroyed dcll list object
*/
extern status_t destroy_dcll(pp_dcll_t pp_list, deleteDataProc p_delete_func)
{
    RETURN_FAILURE(ERROR_CHECKING(pp_list != NULL, NULL == pp_list,
                "ERROR : List object not found", NULL, NULL));

    p_dcll_t p_list = *pp_list;

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
