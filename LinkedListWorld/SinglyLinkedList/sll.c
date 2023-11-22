//HEADER FILES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sll.h"

//SYMBOLIC CONSTANT & MACROS
#define CHECK_LIST 0x00000001
#define CHECK_EMPTY 0x00000002
#define CHECK_LIST_EMPTY CHECK_LIST | CHECK_EMTPY
#define END_LINE (puts("-------------------------------------------------\n{\t\tEND\t\t\t\t}\n\n"))
#define START_LINE (puts("{\t\tSTART\t\t\t\t}\n-------------------------------------------------\n"))

#define ERROR_CHECKING(assert_condition, error_condition, err_msg, error_handler, error_handler_params) ( \
                       error_checking(assert_condition,                                                   \
                                     #assert_condition,                                                   \
                                     __FILE__,                                                            \
                                     __LINE__,                                                            \
                                     error_condition,                                                     \
                                     err_msg,                                                             \
                                     error_handler,                                                       \
                                     error_handler_params))

#define RETURN_FAILURE(status) { if(!status) return FAILURE; }
#define RETURN_NULL(status)    { if(!status) return NULL; }

#define CREATE_NODE(new_node, new_data, exit_proc){ new_node = create_node(new_data); ERROR_CHECKING(NULL != new_node, FALSE, NULL, exit_proc, NULL); }

//AUXILLARY FUNCTION DECLARATIONS
/**
* @func_name: error_checking 
* @summary: This function will check error, trigger error handler and also do assertion
* @params: 1) Assert condition 
           2) Assert condition string
           3) File name in which assertion fails
           4) Line number at which assertion fails
           5) Error condition
           6) Error message
           7) Error handler
           8) Error handler params  
* @returns: status of error condition or assert condition
*/
static status_t error_checking(const int assert_condition, 
                                   const char* assert_condtion_str,
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
        if(!assertion_condition)
        {
            fprintf(stderr, "Assertion Failed : %s in file %s at line %d\n",
                    assert_condtion_str, filename, line_no);
            status = FAILURE;
        }
    #else
        //Error checking
        if(error_condition && err_msg)
        {
            fprintf(stderr, err_msg);
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
* @summary: This function will check for an error and hand it error checking function
* @params: 1) List
           1) Conditions
* @returns: status of error condition
*/
static status_t
check_list_object_condition(p_sll_t p_list, const int conditions)
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

/**
* @func_name: exit_program
* @summary: This function will handle error by simply exiting the program
* @params: NULL
* @returns: NULL
*/
void* 
exit_program(void* params)
{
    exit(EXIT_FAILURE); return NULL;
}

/**
* @func_name: xmalloc
* @summary: This function wrapper function will provide requestion memory
* @params: number of bytes
* @returns: pointer to allocated memory else exit the whole program if request failed
*/
static void* xmalloc(size_t nr_bytes)
{
    void* ptr = malloc(nr_bytes);
    ERROR_CHECKING(TRUE, NULL == ptr, "ERROR : Out of memory", exit_program, NULL);
    memset(ptr, 0, nr_bytes);
    return ptr;
}

/**
* @func_name: create_node
* @summary: This function will create new node and assign new data
* @params: new_data
* @returns: pointer to new node else return null if 
*/
static p_node_t
create_node(data_t new_data)
{
    p_node_t new_node = (p_node_t)xmalloc(NODE_SIZE);

    RETURN_NULL(new_node);

    new_node->data = new_data;
    new_node->p_next = NULL;
    return new_node;
}

/**
* @func_name: create_list
* @summary: This function will create pointer to list i.e sentinel
* @params: nothing
* @returns: pointer to create list
*/

static p_sll_t
create_list(void)
{
    p_sll_t p_list = xmalloc(LIST_SIZE);

    ERROR_CHECKING(NULL != p_list, FALSE, NULL, exit_program, NULL);
    p_list->size = 0;

    p_node_t sentinel = NULL;
    CREATE_NODE(sentinel, NULL, exit_program);

    p_list->p_head = sentinel;
    p_list->p_tail = sentinel;

    return p_list;
}

/**
* @func_name: generic_list_insert
* @summary: This function will add new node after existing node
* @params: 1) List
           2) Existing node
           3) Node
* @returns: success if successfully inserted else return failure
*/
static status_t 
generic_list_insert(p_sll_t p_list, p_node_t e_node, p_node_t new_node)
{
    //Attacking the links of new node with existing node's next
    new_node->p_next = e_node->p_next;

    //Adding new node into list after existing node
    e_node->p_next = new_node;

    //Increment number of element
    p_list->size++;

    //If existing node'next is tail then after adding new node itself become tail of list
    if(new_node->p_next == NULL)
        p_list->p_tail = new_node;

    return SUCCESS;
}

/**
* @func_name: generic_list_remove
* @summary: This function will remove node after existing node
* @params: 1) List
           2) Pointer existing node
* @returns: Removed node's node
*/
static data_t
generic_list_remove(p_sll_t p_list, p_node_t e_node)
{
    p_node_t delete_node = e_node->p_next;

    //Attaching links of two adjacent nodes of delete node
    e_node->p_next = delete_node->p_next;

    //Deattaching the node to be delete
    delete_node->p_next = NULL;

    //Decrementing number of elements
    p_list->size--;

    data_t old_data = delete_node->data;

    //If node to be delete is tail is tail of list then make existing node as tail
    if(delete_node == p_list->p_tail)
        p_list->p_tail = e_node;

    free(delete_node); delete_node = NULL;

    return old_data;
}

/**
* @func_name: create_singly_list
* @summary: This function will create singly linked list
* @params: takes nothing
* @returns: pointer to singly linked list
*/
extern p_sll_t
create_singly_list(void)
{
    return (create_list());
}

/**
* @func_name: sll_insert_front
* @summary: This function will insert data to front i.e head of list
* @params: It takes pointer to sll list and data 
* @returns: status of inserted operation
*/
extern status_t
sll_insert_front(p_sll_t p_list, data_t new_data)
{
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data, exit_program);

    return (generic_list_insert(p_list, p_list->p_head, new_node));
}

/**
* @func_name: sll_insert_back
* @summary: This function will insert back i.e at tail of list
* @params: It takes pointer to sll list and data to be inserted
* @returns: success if inserted successfully else failure
*/
extern status_t
sll_insert_back(p_sll_t p_list, data_t new_data)
{
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data, exit_program);

    return (generic_list_insert(p_list, p_list->p_tail, new_node));
}

/**
* @function: sll_insert_at
* @summary: This function insert data at particular index in the list
* @params: 1) List
           2) New data
           3) Index to which new data is inserted
* @returns: success if operation performed successfully otherwise failure
*/
extern status_t 
sll_insert_at(p_sll_t p_list, data_t new_data, size_t index)
{
    //Checking object if it is valid or not
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    if(index == 0)
        return (sll_insert_front(p_list, new_data));
    else if(index == p_list->size)
        return (sll_insert_back(p_list, new_data));

    //Assertion and error checking for index
    RETURN_FAILURE( ERROR_CHECKING( index < p_list->size, index > p_list->size, 
                                   "ERROR : Index out of Bound", NULL, NULL));

    p_node_t p_run = p_list->p_head;

    for(size_t le = 0; le < index; ++le)
        p_run = p_run->p_next;

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data, exit_program);
    
    return (generic_list_insert(p_list, p_run, new_node));
}

/**
* @function: sll_insert_before
* @summary: This function will insert the data before existing data of the list
* @params: 1) List
           2) new data
           3) existing data
           4) Compare callback function pointer
* @returns: success if inserted successfully else failure
*/
extern status_t 
sll_insert_before(p_sll_t p_list, data_t new_data, data_t e_data, 
                 compareDataEqualityProc p_compare_func)
{
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));

    RETURN_FAILURE( ERROR_CHECKING( NULL != p_compare_func, NULL == p_compare_func, 
                                  "ERROR : Compare callback function not found", NULL, NULL));

    //Searching the node before existed node
    p_node_t p_run = p_list->p_head;

    while(p_run->p_next)
    {
        if(p_compare_func(p_run->p_next->data, e_data) == SUCCESS)
            break;
        
        p_run = p_run->p_next;
    }

    RETURN_FAILURE(ERROR_CHECKING( NULL == p_run->p_next, NULL != p_run->p_next,
                                      "ERROR : Nothing exist before first entry", NULL, NULL));

    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data, exit_program);

    //Inserting new node between existing node prev(p_run) and existing node
    return (generic_list_insert(p_list, p_run, new_node));
}

/**
* @function: sll_insert_after
* @summary: This function will insert the data after the existing data of list
* @params: 1) List
           2) New data
           3) Existing data
           4) Compare callback function pointer
* @returns: success if operation performed successfully otherwise failure
*/
extern status_t 
sll_insert_after(p_sll_t p_list, data_t new_data, data_t e_data, compareDataEqualityProc p_compare_func)
{
    //Checking list object
    RETURN_FAILURE( check_list_object_condition(p_list, CHECK_LIST) );

    //Checking callback function
    RETURN_FAILURE( ERROR_CHECKING( NULL != p_compare_func, NULL == p_compare_func, 
                                   "ERROR : Compare Callback function not found", NULL, NULL));

    p_node_t p_run = p_list->p_head->p_next;

    //Searching existing node
    while(p_run)
    {
        if(SUCCESS == p_compare_func(p_run->data, e_data))
            break;

        p_run = p_run->p_next;
    }

    RETURN_FAILURE( ERROR_CHECKING( p_run != NULL,  p_run == NULL, 
                                  "Existing data not found", NULL, NULL));

    //Creating new node 
    p_node_t new_node = NULL;
    CREATE_NODE(new_node, new_data, exit_program);

    //Inserting new node after existing node
    return (generic_list_insert(p_list, p_run, new_node));
}

//LIST DATA REMOVAL FUNCTIONS
/**
* @function: sll_remove_front
* @summary: This function will remove data from front of the list
* @params: List
* @returns: Data if removed successfully else null
*/
extern data_t
sll_remove_front(p_sll_t p_list)
{
    //Checking for list object or error whether list is empty or not
    RETURN_NULL( check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY ));
    return (generic_list_remove(p_list, p_list->p_head));
}

/**
* @function: sll_remove_back
* @summary: This function will remove data from the back of the list
* @params: List
* @returns: Data if removed sucessfully else null
*/
extern data_t
sll_remove_back(p_sll_t p_list)
{
    //Checking list object whether is present or not along with empty error
    RETURN_NULL( check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY ));

    //Searching node before tail
    p_node_t p_run = p_list->p_head;

    for(p_run; p_run->p_next != p_list->p_tail; p_run = p_run->p_next);

    /* Note : With the help of prev node of tail, we are able to remove tail and
    therefore, we need to traverse till one node before tail
    */

   return (generic_list_remove(p_list, p_run));
}

/**
* @function: sll_remove_at
* @summary: This function will remove data from specified index of the list
* @params: 1) List
           2) Index 
* @returns: Data if removed successfully else null
*/
extern data_t
sll_remove_at(p_sll_t p_list, size_t index)
{
    RETURN_NULL( check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY));
    if(index == 0)
        return (sll_remove_front(p_list));
    else if(index == p_list->size - 1)
        return sll_remove_back(p_list);
    
    RETURN_NULL(ERROR_CHECKING(index < p_list->size, index >= p_list->size,
                               "ERROR : Index out of bound", NULL, NULL));

    p_node_t p_run = p_list->p_head;

    //Traverse through list to reach one index before actual index in order to remove node at actual index
    for(size_t le = 0; le < index; ++le)
        p_run = p_run->p_next;
    
    return (generic_list_remove(p_list, p_run));
}

/**
* @function: sll_remove_before
* @summary: This function will remove data before existing data
* @params: 1) List
           2) existing data
           3) Compare Callback function pointer
* @returns: Data if removed successfully else null
*/
extern data_t
sll_remove_before(p_sll_t p_list, data_t e_data, compareDataEqualityProc p_compare_func)
{
    //Checkling whether list object is present or not and also check whether list is empty
    RETURN_NULL( check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY ));

    //Error checking for attempt to delete node before first node
    p_node_t p_delete = p_list->p_head;
    if(SUCCESS == p_compare_func(p_delete->p_next->data, e_data))
    {
        fprintf(stderr, "ERROR : Nothing present before head to remove\n");
        return NULL;
    }

    //Searching node to be deleted i.e present before existing node
    while(p_delete->p_next)
    {
       if(SUCCESS == p_compare_func(p_delete->p_next->data, e_data))
        break;
    
        p_delete = p_delete->p_next;
    }

    RETURN_NULL( ERROR_CHECKING(TRUE, p_delete->p_next == NULL, "ERROR : Existing data not found", 
                                NULL, NULL));
    
    //In order to remove node before existed node, we need to search prev node of node to be removed
    p_node_t p_run = p_list->p_head;
    
    for(p_run; p_run->p_next != p_delete; p_run = p_run->p_next);

    return (generic_list_remove(p_list, p_run));
}

/**
* @function: sll_remove_after
* @summary: This function will remove data after the existing data
* @params: 1) List
           2) Existing data
           3) Compare Callback function pointer 
* @returns: Data if removed successfully else null
*/
extern data_t
sll_remove_after(p_sll_t p_list, data_t e_data, compareDataEqualityProc p_compare_func)
{
    RETURN_NULL( check_list_object_condition(p_list, CHECK_EMPTY | CHECK_LIST ));

    p_node_t p_run = p_list->p_head;

    while(p_run)
    {
        if(SUCCESS == p_compare_func(p_run->data, e_data))
            break;
        p_run = p_run->p_next;
    }

    RETURN_NULL( ERROR_CHECKING(TRUE, p_run != NULL, "ERROR : Existing data not found",
                                NULL, NULL));

    if(p_list->p_tail == p_run)
    {
        fprintf(stderr, "ERROR : No data present after tail\n");
        return NULL;
    }

    return (generic_list_remove(p_list, p_run));
}

/**
* @function: sll_print_forward
* @summary: This function will display the state of the sll list in forward order
* @params: 1) List
           2) Print Callback function pointer
* @returns: success if displayed successfully else failure
*/
extern status_t
sll_print_forward(p_sll_t p_list, printDataProc p_print_func, const char* msg)
{
    RETURN_FAILURE(check_list_object_condition(p_list, CHECK_LIST));
    RETURN_FAILURE(ERROR_CHECKING(NULL != p_print_func, NULL == p_print_func,
                                  "ERROR : print callback function not found", NULL, NULL));

    if(msg)
        puts(msg);
    puts("Singly linked list in forward order : ");

    START_LINE;
    for(p_node_t p_run = p_list->p_head->p_next; p_run != NULL; p_run = p_run->p_next)
        p_print_func(p_run->data);

    END_LINE;
    return SUCCESS;
}

//LIST GETTER FUNCTIONS
/**
* @function: sll_get_head
* @summary: This function will return first element of the list
* @params: List
* @returns: first element else return null
*/
extern data_t
sll_get_head(p_sll_t p_list)
{
    RETURN_NULL( check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY));
    return (p_list->p_head->p_next->data);
}

/**
* @function: sll_get_tail
* @summary: This function will return last element of the list
* @params: List
* @returns: last element else return null
*/

extern data_t
sll_get_tail(p_sll_t p_list)
{
    RETURN_NULL( check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY ));
    return (p_list->p_tail->data);
}

/**
* @function: sll_get_size
* @summary: This function will return number of element in the list 
* @params: List
* @returns: number of elements counted
*/
extern size_t 
sll_get_size(p_sll_t p_list)
{
    RETURN_FAILURE( check_list_object_condition(p_list, CHECK_LIST) );
    return (p_list->size);
}

/**
* @function: sll_get_data_at
* @summary: This function will return data of particular index
* @params: 1) List
           2) Index
* @returns: Data if provided valid index else null
*/
extern data_t 
sll_get_data_at(p_sll_t p_list, size_t index)
{
    RETURN_NULL( check_list_object_condition(p_list, CHECK_LIST | CHECK_EMPTY ));
    RETURN_NULL( ERROR_CHECKING(index < p_list->size, index >= p_list->size, 
                                "ERROR : Index out of bound", NULL, NULL));
    
    p_node_t p_run = p_list->p_head;
    for(size_t le = 0; le < index; ++le)
        p_run = p_run->p_next;

    return (p_run->data);
}

//LIST DESTRUCTION FUNCTIONS
/**
* @function: destroy_singly_list
* @summary: This function will destroy whole singly linked list
* @params: 1) Pointer List
           2) Delete callback function pointer
* @returns: success if destroyed sucessfully else failure
*/
extern status_t
destroy_singly_list(pp_sll_t pp_list, deleteDataProc p_delete_func)
{
    RETURN_FAILURE(ERROR_CHECKING( pp_list != NULL, pp_list == NULL, "ERROR : List not found", NULL, NULL))

    p_sll_t  p_list = *pp_list;
    RETURN_FAILURE( check_list_object_condition(p_list, CHECK_LIST ));

    RETURN_FAILURE( ERROR_CHECKING(p_delete_func != NULL, p_delete_func == NULL, 
                                   "ERROR : delete callback function not found", NULL, NULL));

    p_node_t p_run = p_list->p_head->p_next, p_run_next = NULL;
    while(p_run != NULL)
    {
        p_run_next = p_run->p_next;
        p_delete_func(p_run->data);
        free(p_run);
        p_run = p_run_next;
    }
    if(p_list->p_head)
    {
        free(p_list->p_head);
        p_list->p_head = NULL;
    }
    p_list->p_tail = NULL;
    free(*pp_list); *pp_list = NULL;

    return SUCCESS;
}

