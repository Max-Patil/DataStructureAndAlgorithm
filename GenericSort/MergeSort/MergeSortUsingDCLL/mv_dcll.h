/**
* @file : mv_dcll.h
* @author : MaxPatil (patilmitesh8746@gmail.com)

* @brief : This header file includes the declaration of doubly circular linked list
that intended for client programmer to call

* @date : 13-12-2022
* @version : 1.1

* @copyright   Copyrights (c) 2022
*/

#ifndef __MV_DCLL_H
#define __MV_DCLL_H

//HEADER FILES
#include<stdio.h>
#include<stdlib.h>

//SYMBOLIC CONSTANTS
#ifndef SUCCESS
#define SUCCESS 1
#endif

#ifndef FAILURE
#define FAILURE 0
#endif

#define TRUE SUCCESS
#define FALSE FAILURE

#define ONE_ELEMENT 1

#define NODE_SIZE sizeof(node_t)
#define LIST_SIZE sizeof(mv_dcll_t)

//TYPE DEFINITIONS
typedef int status_t;
typedef void* data_t;
typedef data_t* p_data_t;

typedef struct mv_doubly_circular_linked_list mv_dcll_t;
typedef mv_dcll_t* p_mv_dcll_t;
typedef p_mv_dcll_t* pp_mv_dcll_t;

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

struct mv_doubly_circular_linked_list
{
    node_t* p_head;
    node_t* p_tail;
    size_t size;
};

//LIST INTERFACE FUNCTIONS

/**
* @function: create_mv_dcll_list
* @summary: This function will create pointer to dcll list object
* @params: takes nothing
* @returns: Pointer to dcll list object
*/
extern p_mv_dcll_t create_mv_dcll_list(void);

/**
* @function: mv_dcll_insert_back
* @summary: This function will add data to the back of the list
* @params: 1) List
           2) new data
* @returns: SUCCESS if inserted successfully else return FAILURE
*/
extern status_t mv_dcll_insert_back(p_mv_dcll_t p_list, data_t new_data);

/**
* @function: mv_dcll_insert_front
* @summary: This function will add data to front of the list
* @params: 1) List
           2) New data
* @returns: SUCCESS if inserted successfully else return FAILURE
*/
extern status_t mv_dcll_insert_front(p_mv_dcll_t p_list, data_t new_data);

/**
* @function: mv_dcll_remove_front
* @summary: This function will remove data from the front of the list
* @params: List
* @returns: Data to be removed else return NULL if any error occured
*/
extern data_t mv_dcll_remove_front(p_mv_dcll_t p_list);

/**
* @function: mv_dcll_remove_back
* @summary: This function will remove data from the back of the list
* @params: List
* @returns: Data to be removed else return NULL if any error occurred
*/
extern data_t mv_dcll_remove_back(p_mv_dcll_t p_list);

//LIST GETTER METHODS
/**
* @function: mv_get_dcll_head
* @summary: This function will simply return data of first node of the dcll list
* @params: List
* @returns: Data if present else return null
*/
extern data_t mv_get_dcll_head(p_mv_dcll_t p_list);

/**
* @function: mv_get_dcll_tail
* @summary: This function will simply return data of last node of the dcll list
* @params: List
* @returns: Data if present else return null
*/
extern data_t mv_get_dcll_tail(p_mv_dcll_t p_list);

/**
* @function: mv_get_dcll_size
* @summary: This function will simply return number of data element present in the dcll list
* @params: List
* @returns: Size of the dcll list
*/
extern size_t mv_get_dcll_size(p_mv_dcll_t p_list);

/**
* @function: mv_is_dcll_empty
* @summary: This function will simply check whether dcll list is empty or not
* @params: List
* @returns: TRUE if list is empty else return FALSE
*/
extern status_t mv_is_dcll_empty(p_mv_dcll_t p_list);

/**
* @function: mv_is_list_sorted
* @summary: This function will simply whether dcll list is empty, sorted or not 
* @params: 1) List
           2) Compare callback function pointer
* @returns: TRUE if list is sorted else return FALSE
*/
extern status_t mv_is_list_sorted(p_mv_dcll_t p_list, compareDataProc p_compare_func);

/**
* @function: mv_get_node_data
* @summary: This function will simply return data of that node
* @params: Node
* @returns: Valid data if list is non-empty else return Invalid data as Null
*/
inline extern data_t mv_get_node_data(p_node_t p_node);

/**
* @function: mv_dcll_show
* @summary: This function will display the state of the dcll list
* @params: 1) List
           2) Print callback function pointer
           3) Message provided by client
* @returns: Status of display operation
*/
extern status_t mv_dcll_show(p_mv_dcll_t p_list, printDataProc p_print_func, const char* msg);

/**
* @function: destroy_mv_dcll
* @summary: This function will destroy the dcll list completely
* @params: 1) Pointer to List
           2) Delete Callback function pointer
* @returns: Status of destroyed dcll list object
*/
extern status_t destroy_mv_dcll(pp_mv_dcll_t pp_list, deleteDataProc p_delete_func);

#endif // __MV_DCLL_H