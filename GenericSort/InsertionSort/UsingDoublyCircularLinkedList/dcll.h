#ifndef _DCLL_H
#define _DCLL_H

//HEADER FILES
#include<stdio.h>
#include<stdlib.h>

//SYMBOLIC CONSTANTS
#define SUCCESS 1
#define FAILURE 0
#define TRUE SUCCESS
#define FALSE FAILURE

#define ONE_ELEMENT 1

#define NODE_SIZE sizeof(node_t)
#define LIST_SIZE sizeof(dcll_t)

//TYPE DEFINITIONS
typedef int status_t;
typedef void* data_t;
typedef data_t* p_data_t;

typedef struct doubly_circular_linked_list dcll_t;
typedef dcll_t* p_dcll_t;
typedef p_dcll_t* pp_dcll_t;

typedef struct list_node node_t;
typedef node_t* p_node_t;

typedef void(*printDataProc)(data_t);
typedef void(*deleteDataProc)(data_t);
typedef status_t(*compareDataProc)(data_t, data_t);

//STRUCTURE TYPE DEFINITIONS
struct list_node 
{
    data_t data;
    node_t* p_next;
    node_t* p_prev;
};

struct doubly_circular_linked_list
{
    node_t* p_head;
    node_t* p_tail;
    size_t size;
};

//LIST INTERFACE FUNCTIONS

/**
* @function: create_dcll_list
* @summary: This function will create pointer to dcll list object
* @params: takes nothing
* @returns: Pointer to dcll list object
*/
extern p_dcll_t create_dcll_list(void);

/**
* @function: dcll_insert_back
* @summary: This function will add data to the back of the list
* @params: 1) List
           2) new data
* @returns: SUCCESS if inserted successfully else return FAILURE
*/
extern status_t dcll_insert_back(p_dcll_t p_list, data_t new_data);

/**
* @function: dcll_insert_front
* @summary: This function will add data to front of the list
* @params: 1) List
           2) New data
* @returns: SUCCESS if inserted successfully else return FAILURE
*/
extern status_t dcll_insert_front(p_dcll_t p_list, data_t new_data);

/**
* @function: dcll_insert_at
* @summary: This function will insert new data at specified index
* @params: 1) List
           2) new_data
           3) index
* @returns: SUCCESS if inserted successfully at specified index else return FAILURE
*/
extern status_t dcll_insert_at(p_dcll_t p_list, data_t new_data, size_t index);

/**
* @function: dcll_insert_after
* @summary: This function will insert new data after the existing data
* @params: 1) List
           2) new data
           3) existing data
           4) compare callback function pointer
* @returns: SUCCESS if inserted new data after existing data    
*/
extern status_t dcll_insert_after(p_dcll_t p_list, data_t new_data, data_t e_data,
                                    compareDataProc p_compare_func);

/**
* @function: dcll_insert_before
* @summary: This function will insert new data before existing data
* @params: 1) List
           2) new data
           3) existing data
           4) compare callback function pointer
* @returns: SUCCESS if inserted new data before existing data else return FAILURE
*/
extern status_t dcll_insert_before(p_dcll_t p_list, data_t new_data, data_t e_data,
                                     compareDataProc p_compare_func);

/**
* @function: dcll_remove_front
* @summary: This function will remove data from the front of the list
* @params: List
* @returns: Data to be removed else return NULL if any error occured
*/
extern data_t dcll_remove_front(p_dcll_t p_list);

/**
* @function: dcll_remove_back
* @summary: This function will remove data from the back of the list
* @params: List
* @returns: Data to be removed else return NULL if any error occurred
*/
extern data_t dcll_remove_back(p_dcll_t p_list);

/**
* @function: dcll_remove_at
* @summary: This function will remove data at specified index
* @params: 1) List
           2) Index
* @returns: Data at specified index if removed successfully else return null
*/
extern data_t dcll_remove_at(p_dcll_t p_list, size_t index);

/**
* @function: dcll_remove_after
* @summary: This function will remove data after the existing data 
* @params: 1) List
           2) existing data
           3) compare callback function pointer
* @returns: Data removed after existing data else return null
*/
extern data_t dcll_remove_after(p_dcll_t p_list, data_t e_data, compareDataProc p_compare_func);

/**
* @function: dcll_remove_before
* @summary: This function will remove data before existing data
* @params: 1) List
           2) existing data
           3) compare callback function pointer
* @returns: Data that is removed before existing data else return null
*/
extern data_t dcll_remove_before(p_dcll_t p_list, data_t e_data ,compareDataProc p_compare_func);

//LIST GETTER METHODS
/**
* @function: get_dcll_head
* @summary: This function will simply return data of first node of the dcll list
* @params: List
* @returns: Data if present else return null
*/
extern data_t get_dcll_head(p_dcll_t p_list);

/**
* @function: get_dcll_tail
* @summary: This function will simply return data of last node of the dcll list
* @params: List
* @returns: Data if present else return null
*/
extern data_t get_dcll_tail(p_dcll_t p_list);

/**
* @function: get_dcll_size
* @summary: This function will simply return number of data element present in the dcll list
* @params: List
* @returns: Size of the dcll list
*/
extern size_t get_dcll_size(p_dcll_t p_list);

/**
* @function: is_dcll_empty
* @summary: This function will simply check whether dcll list is empty or not
* @params: List
* @returns: TRUE if list is empty else return FALSE
*/
extern status_t is_dcll_empty(p_dcll_t p_list);

/**
* @function: is_list_sorted
* @summary: This function will simply whether dcll list is empty, sorted or not 
* @params: 1) List
           2) Compare callback function pointer
* @returns: TRUE if list is sorted else return FALSE
*/
extern status_t is_list_sorted(p_dcll_t p_list, compareDataProc p_compare_func);

/**
* @function: get_node_data
* @summary: This function will simply return data of that node
* @params: Node
* @returns: Valid data if list is non-empty else return Invalid data as Null
*/
inline extern data_t get_node_data(p_node_t p_node);

/**
* @function: dcll_show
* @summary: This function will display the state of the dcll list
* @params: 1) List
           2) Print callback function pointer
           3) Message provided by client
* @returns: Status of display operation
*/
extern status_t dcll_show(p_dcll_t p_list, printDataProc p_print_func, const char* msg);

/**
* @function: destroy_dcll
* @summary: This function will destroy the dcll list completely
* @params: 1) Pointer to List
           2) Delete Callback function pointer
* @returns: Status of destroyed dcll list object
*/
extern status_t destroy_dcll(pp_dcll_t pp_list, deleteDataProc p_delete_func);

#endif // _DCLL_H