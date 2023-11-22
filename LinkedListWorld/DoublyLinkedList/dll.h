#ifndef _DLL_H
#define _DLL_H

//MACROS & SYMBOLIC CONSTANTS
#ifndef SUCCESS
#define SUCCESS 1
#endif

#ifndef FAILURE
#define FAILURE 0
#endif 

#define TRUE SUCCESS
#define FALSE FAILURE

#define LIST_SIZE sizeof(dll_t)
#define NODE_SIZE sizeof(node_t)

//TYPE DEFINITIONS
typedef int status_t;
typedef void* data_t;
typedef data_t* p_data_t;

typedef struct list_node node_t;
typedef node_t* p_node_t;
typedef node_t** pp_node_t;

typedef struct doubly_linked_list dll_t;
typedef dll_t * p_dll_t;
typedef p_dll_t* pp_dll_t;

typedef status_t (*compareDataProc)(data_t, data_t);
typedef void(*printDataProc)(data_t);
typedef void(*deleteDataProc)(data_t);

//STRUCTURE TYPE DEFINITIONS
struct list_node
{
    data_t data;
    p_node_t p_next;
    p_node_t p_prev;
};

struct doubly_linked_list
{
    p_node_t p_head;
    p_node_t p_tail;
    size_t size;
};

//LIST INTERFACE FUNCTIONS
/**
* @function: create_dll
* @summary: This function will create pointer to dll 
* @params: takes nothing
* @returns: return pointer to dll
*/
extern p_dll_t create_dll(void);

//LIST DATA INSERTION FUNCTIONS
/**
* @function: dll_insert_back
* @summary: This function will add new data to back of dll list
* @params: 1) List
           2) new data
* @returns: SUCCESS if inserted successfully else return FAILURE
*/
extern status_t dll_insert_back(p_dll_t p_list, data_t new_data);

/**
* @function: dll_insert_front
* @summary: This function will insert new data at the front of dll
* @params: 1) List
           2) new data
* @returns: SUCCESS if inserted successfully else return FAILURE
*/
extern status_t dll_insert_front(p_dll_t p_list, data_t new_data);

/**
* @function: dll_insert_at
* @summary: This function will insert new data at specified index
* @params: 1) List
           2) new_data
           3) index
* @returns: SUCCESS if inserted successfully at specified index else return FAILURE
*/
extern status_t dll_insert_at(p_dll_t p_list, data_t new_data, size_t index);

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
                                    compareDataProc p_compare_func);

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
                                     compareDataProc p_compare_func);

//LIST REMOVAL FUNCTIONS
/**
* @function: dll_remove_back
* @summary: This function will remove data from the back of dll list
* @params: List
* @returns: Data if removed successfully else return null
*/
extern data_t dll_remove_back(p_dll_t p_list);

/**
* @function: dll_remove_front
* @summary: This function will remove data from the front of dll list
* @params: List
* @returns: Data if removed successfully else return null
*/
extern data_t dll_remove_front(p_dll_t p_list);

/**
* @function: dll_remove_at
* @summary: This function will remove data at specified index
* @params: 1) List
           2) Index
* @returns: Data at specified index if removed successfully else return null
*/
extern data_t dll_remove_at(p_dll_t p_list, size_t index);

/**
* @function: dll_remove_after
* @summary: This function will remove data after the existing data 
* @params: 1) List
           2) existing data
           3) compare callback function pointer
* @returns: Data removed after existing data else return null
*/
extern data_t dll_remove_after(p_dll_t p_list, data_t e_data, compareDataProc p_compare_func);

/**
* @function: dll_remove_before
* @summary: This function will remove data before existing data
* @params: 1) List
           2) existing data
           3) compare callback function pointer
* @returns: Data that is removed before existing data else return null
*/
extern data_t dll_remove_before(p_dll_t p_list, data_t e_data ,compareDataProc p_compare_func);

//LIST GETTER FUNCTIONS
/**
* @function: dll_get_head
* @summary: This function will return first node's data of dll list  
* @params: List
* @returns: Data if present else return null
*/
extern data_t dll_get_head(p_dll_t p_list);

/**
* @function: dll_get_tail
* @summary: This function will return last node's data of dll list
* @params: List
* @returns: Data if present else return null
*/
extern data_t dll_get_tail(p_dll_t p_list);

/**
* @function: dll_get_size
* @summary: This function will return size i.e number of elements
* @params: List
* @returns: Number of elements present in list
*/
extern size_t dll_get_size(p_dll_t p_list);

/**
* @function: is_dll_empty
* @summary: This function will return boolean value to signify its emptiness
* @params: List
* @returns: TRUE if empty else return FALSE
*/
extern status_t is_dll_empty(p_dll_t);

/**
* @function: dll_print_forward
* @summary: This function will display the state of list in forward order
* @params: 1) List
           2) Print callback function pointer
* @returns: Status of print operation performed
*/
extern status_t dll_print_forward(p_dll_t p_list, printDataProc p_print_func, const char* msg);

/**
* @function: dll_print_reverse
* @summary: This function will display the state of list in reverse order
* @params: 1) List
           2) Print callback function pointer   
* @returns: Status of print operation performed
*/
extern status_t dll_print_reverse(p_dll_t p_list, printDataProc p_print_func, const char* msg);

/**
* @function: destroy_dll
* @summary: This function will destroy the whole list along with deletion of client side data
* @params: 1) Pointer to list
           2) Delete callback function pointer 
* @returns: Status of deleted operation performed
*/
extern status_t destroy_dll(pp_dll_t pp_list, deleteDataProc p_delete_func);
#endif  // __DLL_H