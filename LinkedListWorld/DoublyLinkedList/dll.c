//HEADER FILES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dll.h"

//MACROS & SYMBOLIC CONSTANTS
#define CHECK_LIST 0x0000001
#define CHECK_EMPTY 0x0000002
#define CHECK_LIST_EMPTY CHECK_EMPTY | CHECK_LIST

#define ERROR_CHECKING(assert_condition, error_condition, err_msg, error_handler, error_handler_params)( \
    error_checking(assert_condition,                                                                     \
                   #assert_condition,                                                                    \
                   __FILE__,                                                                             \
                   __LINE__,                                                                             \
                   error_condition,                                                                      \
                   err_msg,                                                                              \
                   error_handler,                                                                        \
                   error_handler_params))

#define ASSERT(assert_condition) (ERROR_CHECKING(assert_condition, FALSE, NULL, exit_program, NULL))

#define CREATE_NODE(new_node, new_data) { new_node = create_node(new_data); ASSERT(new_node != NULL); }

#define RETURN_FAILURE(status) { if(!status) return FAILURE; }
#define RETURN_NULL(status)    { if(!status) return NULL; }
#define END_LINE (puts("-------------------------------------------------\n{\t\tEND\t\t\t\t}\n\n"))
#define START_LINE (puts("{\t\tSTART\t\t\t\t}\n-------------------------------------------------\n"))

//AUXILLARY FUNCTIONS DEFINITIONS
/**
* @function: exit_program
* @summary: This function will simply exit the program
* @params: Pointer to void      
* @returns: Pointer to void
*/
static void* exit_program(void* params) { exit(EXIT_FAILURE); return NULL; }


/**
* @function: xmalloc
* @summary: This function will provide request mapped memory
* @params: Number of bytes
* @returns: Pointer to allocated memory of size (nr_bytes)
*/
static void* xmalloc(size_t nr_bytes)
{
    void* ptr = malloc(nr_bytes);
    ERROR_CHECKING(TRUE, NULL == ptr, "ERROR : Out of memory", exit_program, NULL);
    memset(ptr, 0, nr_bytes);
    return ptr;
}

/**
* @function: error_checking
* @summary: This function will check error, display error message and do assertion
* @params: 1) Assert condition
           2) Assert condition string
           3) File where assertion occured
           4) Line number where assertion occured
           5) Error condition
           6) Error Message
           7) Error Handler
           8) Paramters for Error handler
* @returns: Status of error or assert condition
*/
static status_t error_checking(const int assert_condition,
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
            fprintf(stderr, "Assertion failed : %s, File %s, Line no %d\n", assert_condition_str,
            filename, line_no);
            status = FAILURE;
        }
    #else
        //Error checking
        if(error_condition && err_msg)
        {
            fprintf(stderr,err_msg);
            fprintf(stdout, "\n");
            status = FAILURE;
        }
    #endif

    if(status == FAILURE && error_handler)
        error_handler(error_handler_params);

    return status;
}



/**
* @function: search_existing_node
* @summary: This function will simply find node based on compare callback function provided by insert routines
* @params: 1) List
           2) Existing data
           3) Compare callback function pointer
* @returns: Node that is need to search based on its value/data if present else return NULL
*/
static data_t search_existing_node(p_dll_t p_list, data_t e_data, compareDataProc p_compare_func)
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
* @function: create_node
* @summary: This function will allocate node of dll list
* @params: New data
* @returns: Pointer to allocated node to which new data is assigned
*/
static p_node_t create_node(data_t new_data)
{
    p_node_t new_node = (p_node_t)xmalloc(NODE_SIZE);

    //If assertion fails in xmalloc then we are going to handle it by return NULL
    RETURN_NULL(new_node);

    //Assigning new data to new node
    new_node->data = new_data;

    //Assigning next and prev pointer of new node to itself
    new_node->p_next = new_node->p_prev = NULL;

    return new_node;
}

/**
* @function: create_list
* @summary: This function will create pointer to object of dll list
* @params: takes nothing
* @returns: Pointer to dll list object
*/
static p_dll_t create_list(void)
{
    p_dll_t p_list = xmalloc(LIST_SIZE);

    //If assertion fails then it will get handle by returning null
    RETURN_NULL(p_list);

    //Create sentinel node
    p_node_t sentinel = NULL;
    CREATE_NODE(sentinel, NULL);

    //Assigning sentinel to head and tail of dll list
    p_list->p_head = p_list->p_tail = sentinel;
    p_list->size = 0;

    return p_list;
}

/**
* @function: generic_list_insert
* @summary: This function will insert node in the dll list
* @params: 1) List
           2) new node prev i.e first node
           3) new node  i.e mid node
           4) new node next i.e last node
* @returns: status of generic insertion
*/
static status_t generic_list_insert(p_dll_t p_list, p_node_t first, p_node_t mid, p_node_t last)
{
    //Adding new node in the dll list
    mid->p_next = last;
    mid->p_prev = first;

    if(last != NULL) 
        last->p_prev = mid;
        
    first->p_next = mid;

    //Increment size
    p_list->size++;

    //Modify the tail
    if(first == p_list->p_tail)
        p_list->p_tail = mid;
    
    return SUCCESS;
}

/**
* @function: generic_list_remove
* @summary: This function will remove the existing node 
* @params: 1) List
           2) Node to be deleted i.e existing node
* @returns: Data of node to be removed
*/
static data_t generic_list_remove(p_dll_t p_list, p_node_t p_delete)
{
    p_node_t p_delete_prev = p_delete->p_prev, p_delete_next = p_delete->p_next;

    //Re-establishing adjacent links of node to be deleted
    p_delete_prev->p_next = p_delete->p_next;
    if(p_list->p_tail != p_delete)
        p_delete_next->p_prev = p_delete->p_prev;

    data_t old_data = p_delete->data;

    //Modifying tail if required
    if(p_delete == p_list->p_tail)
        p_list->p_tail = p_delete_prev;

    //freeing the allocated resource
    free(p_delete); p_delete = NULL;

    //Decrementing the number of elements present in list
    p_list->size--;

    return old_data;
}

/**
* @function: check_list_object_condition
* @summary: This function will invoke error checking macro in presence of error
* @params: 1) List
           2) Conditions
* @returns: Status of error condition
*/
static status_t check_list_object_condition(p_dll_t p_list, const int conditions)
{
    if(CHECK_LIST & conditions)
    {
        RETURN_FAILURE(ERROR_CHECKING(TRUE, NULL == p_list, 
                                     "ERROR : List not found", NULL, NULL));
    }

    if(CHECK_EMPTY & conditions)
    {
        RETURN_FAILURE(ERROR_CHECKING(TRUE, p_list->p_head == p_list->p_tail,
                                      "ERROR : List is empty", NULL, NULL));
    }
    return SUCCESS;
}

//LIST INTERFACE FUNCTIONS
/**
* @function: create_dll
* @summary: This function will create pointer to dll 
* @params: takes nothing
* @returns: return pointer to dll
*/
extern p_dll_t create_dll(void) { return (create_list()); }

//LIST DATA INSERTION FUNCTIONS
/**
* @function: dll_insert_back
* @summary: This function will add new data to back of dll list
* @params: 1) List
           2) new data
* @returns: SUCCESS if inserted successfully else return FAILURE
*/
extern status_t dll_insert_back(p_dll_t p_list, data_t new_data)
{
    ASSERT(NULL != p_list);
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data);

    return(generic_list_insert(p_list, p_list->p_tail, new_node, p_list->p_tail->p_next));
}

/**
* @function: dll_insert_front
* @summary: This function will insert new data at the front of dll
* @params: 1) List
           2) new data
* @returns: SUCCESS if inserted successfully else return FAILURE
*/
extern status_t dll_insert_front(p_dll_t p_list, data_t new_data)
{
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));
    ASSERT(p_list != NULL);

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data);

    return(generic_list_insert(p_list, p_list->p_head, new_node, p_list->p_head->p_next));
}

/**
* @function: dll_insert_at
* @summary: This function will insert new data at specified index
* @params: 1) List
           2) new_data
           3) index
* @returns: SUCCESS if inserted successfully at specified index else return FAILURE
*/
extern status_t dll_insert_at(p_dll_t p_list, data_t new_data, size_t index)
{
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST)); 
    ASSERT(p_list != NULL);

    if(index == 0)
        return (dll_insert_front(p_list, new_data));
    else if(index == p_list->size)
        return(dll_insert_back(p_list, new_data));

    RETURN_FAILURE(ERROR_CHECKING(index < p_list->size, index > p_list->size,
                                 "ERROR : Index out of range", NULL, NULL));
    
    p_node_t p_run = p_list->p_head;
    for(size_t le = 0; le < index; le++, p_run = p_run->p_next);

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data);

    return (generic_list_insert(p_list, p_run, new_node, p_run->p_next));
}
/**
* @function: dll_insert_after
* @summary: This function will insert new data after the existing data
* @params: 1) List
           2) new data
           3) existing data
           4) compare callback function pointer
* @returns: SUCCESS if inserted new data after existing data    
*/
extern status_t dll_insert_after(p_dll_t p_list, data_t new_data, data_t e_data,
                                    compareDataProc p_compare_func)
{
    ASSERT(p_list != NULL);
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    p_node_t e_node = search_existing_node(p_list, e_data, p_compare_func);
    RETURN_FAILURE(e_node);

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data);

    return (generic_list_insert(p_list, e_node, new_node, e_node->p_next));
}

/**
* @function: dll_insert_before
* @summary: This function will insert new data before existing data
* @params: 1) List
           2) new data
           3) existing data
           4) compare callback function pointer
* @returns: SUCCESS if inserted new data before existing data else return FAILURE
*/
extern status_t dll_insert_before(p_dll_t p_list, data_t new_data, data_t e_data,
                                     compareDataProc p_compare_func)
{
    ASSERT(p_list != NULL);
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    p_node_t e_node = search_existing_node(p_list, e_data, p_compare_func);
    RETURN_FAILURE(e_node);

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data);

    return (generic_list_insert(p_list, e_node->p_prev, new_node, e_node));
}

//LIST REMOVAL FUNCTIONS
/**
* @function: dll_remove_back
* @summary: This function will remove data from the back of dll list
* @params: List
* @returns: Data if removed successfully else return null
*/
extern data_t dll_remove_back(p_dll_t p_list)
{
    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY));
    ASSERT(p_list != NULL || is_dll_empty(p_list) != TRUE);

    return (generic_list_remove(p_list, p_list->p_tail));
}

/**
* @function: dll_remove_front
* @summary: This function will remove data from the front of dll list
* @params: List
* @returns: Data if removed successfully else return null
*/
extern data_t dll_remove_front(p_dll_t p_list)
{
    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY));
    ASSERT(p_list != NULL || is_dll_empty(p_list) != TRUE);

    return(generic_list_remove(p_list, p_list->p_head->p_next));
}

/**
* @function: dll_remove_at
* @summary: This function will remove data at specified index
* @params: 1) List
           2) Index
* @returns: Data at specified index if removed successfully else return null
*/
extern data_t dll_remove_at(p_dll_t p_list, size_t index)
{
    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY));
    ASSERT(p_list != NULL || is_dll_empty(p_list) != TRUE);

    if(index == 0)
        return (dll_remove_front(p_list));
    else if(index == p_list->size)
        return (dll_remove_back(p_list));

    RETURN_NULL(ERROR_CHECKING(index < p_list->size, index > p_list->size,
                               "ERROR : Index out of range", NULL, NULL));
    
    p_node_t p_run = p_list->p_head;
    for(size_t le = 0; le < index; le++)
        p_run = p_run->p_next;

    return (generic_list_remove(p_list, p_run));
}

/**
* @function: dll_remove_after
* @summary: This function will remove data after the existing data 
* @params: 1) List
           2) existing data
           3) compare callback function pointer
* @returns: Data removed after existing data else return null
*/
extern data_t dll_remove_after(p_dll_t p_list, data_t e_data, compareDataProc p_compare_func)
{
    ASSERT(NULL != p_list || p_list->p_head != p_list->p_tail);
    RETURN_NULL(check_list_object_condition(p_list, CHECK_EMPTY | CHECK_LIST));
    RETURN_NULL(ERROR_CHECKING(p_compare_func != NULL, p_compare_func == NULL,
                              "ERROR : Compare callback function", NULL, NULL));
    
    p_node_t e_node = search_existing_node(p_list, e_data, p_compare_func);
    RETURN_NULL(e_node);

    RETURN_NULL(ERROR_CHECKING(e_node != p_list->p_tail, e_node == p_list->p_tail, 
                               "ERROR : No node exist after tail of the list", NULL, NULL));
    return(generic_list_remove(p_list, e_node->p_next));
}

/**
* @function: dll_remove_before
* @summary: This function will remove data before existing data
* @params: 1) List
           2) existing data
           3) compare callback function pointer
* @returns: Data that is removed before existing data else return null
*/
extern data_t dll_remove_before(p_dll_t p_list, data_t e_data ,compareDataProc p_compare_func)
{
    ASSERT(NULL != p_list || is_dll_empty(p_list) != TRUE);
    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY));
    RETURN_NULL(ERROR_CHECKING(p_compare_func != NULL, NULL == p_compare_func,
                              "ERROR : Compare callback function not found" , NULL, NULL));
    
    p_node_t e_node = search_existing_node(p_list, e_data, p_compare_func);
    RETURN_NULL(e_node);

    RETURN_NULL(ERROR_CHECKING(e_node != p_list->p_head->p_next, e_node == p_list->p_head->p_next,
                               "ERROR : No node exist before head of the list", NULL, NULL));
    return (generic_list_remove(p_list, e_node->p_prev));
}

//LIST GETTER FUNCTIONS
/**
* @function: dll_get_head
* @summary: This function will return first node's data of dll list  
* @params: List
* @returns: Data if present else return null
*/
extern data_t dll_get_head(p_dll_t p_list)
{
    ASSERT(NULL != p_list || is_dll_empty(p_list) != TRUE);
    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY));
    return (p_list->p_head->p_next->data);
}

/**
* @function: dll_get_tail
* @summary: This function will return last node's data of dll list
* @params: List
* @returns: Data if present else return null
*/
extern data_t dll_get_tail(p_dll_t p_list)
{
    ASSERT(NULL != p_list || is_dll_empty(p_list) != TRUE);
    RETURN_NULL(check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY));
    return (p_list->p_tail->data);
}

/**
* @function: dll_get_size
* @summary: This function will return size i.e number of elements
* @params: List
* @returns: Number of elements present in list
*/
extern size_t dll_get_size(p_dll_t p_list)
{
    ASSERT(NULL != p_list);
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST ));
    return (p_list->size);
}

/**
* @function: is_dll_empty
* @summary: This function will return boolean value to signify its emptiness
* @params: List
* @returns: TRUE if empty else return FALSE
*/
extern status_t is_dll_empty(p_dll_t p_list)
{
    return (p_list->p_head == p_list->p_tail);
}

/**
* @function: dll_print_forward
* @summary: This function will display the state of list in forward order
* @params: 1) List
           2) Print callback function pointer
* @returns: Status of print operation performed
*/
extern status_t dll_print_forward(p_dll_t p_list, printDataProc p_print_func, const char* msg)
{
    ASSERT(p_list != NULL);
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));
    RETURN_FAILURE(ERROR_CHECKING(p_print_func != NULL, NULL == p_print_func, 
                                 "ERROR : Print callback function not found", NULL, NULL));
    if(msg) puts(msg);
    puts("Doubly linked List : ");
    START_LINE;
    for(p_node_t p_run = p_list->p_head->p_next; p_run != NULL; p_run = p_run->p_next)
        p_print_func(p_run->data);
    END_LINE;
    return SUCCESS;
}

/**
* @function: dll_print_reverse
* @summary: This function will display the state of list in reverse order
* @params: 1) List
           2) Print callback function pointer   
* @returns: Status of print operation performed
*/
extern status_t dll_print_reverse(p_dll_t p_list, printDataProc p_print_func, const char* msg)
{
    ASSERT(p_list != NULL);
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));
    RETURN_FAILURE(ERROR_CHECKING(p_print_func != NULL, NULL == p_print_func, 
                                 "ERROR : Print callback function not found", NULL, NULL));
    if(msg) puts(msg);
    puts("Doubly linked List : ");
    START_LINE;
    for(p_node_t p_run = p_list->p_tail; p_run != NULL; p_run = p_run->p_prev)
        p_print_func(p_run->data);
    END_LINE;
    return SUCCESS;
}

/**
* @function: destroy_dll
* @summary: This function will destroy the whole list along with deletion of client side data
* @params: 1) Pointer to list
           2) Delete callback function pointer 
* @returns: Status of deleted operation performed
*/
extern status_t destroy_dll(pp_dll_t pp_list, deleteDataProc p_delete_func)
{
    ASSERT(pp_list != NULL);
    RETURN_FAILURE(ERROR_CHECKING(pp_list != NULL, NULL == pp_list, 
                                   "ERROR : List not found", NULL, NULL));
    p_dll_t p_list = *pp_list;
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));
    RETURN_FAILURE(ERROR_CHECKING(p_delete_func != NULL, NULL == p_delete_func,
                                 "ERROR : Delete callback function not found", NULL, NULL));
    
    p_node_t p_run = p_list->p_head->p_next, p_run_next = NULL;

    while(p_run != NULL)
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
    free(*pp_list); *pp_list = NULL;
    
    return SUCCESS;
}