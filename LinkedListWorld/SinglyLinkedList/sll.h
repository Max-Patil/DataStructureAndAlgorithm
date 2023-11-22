//HEADER GUARD
#ifndef _SLL_H
#define _SLL_H

//SYMBOLIC CONSTANT
#define SUCCESS 1
#define FAILURE 0
#define TRUE SUCCESS 
#define FALSE FAILURE


#define LIST_SIZE (sizeof(sll_t))
#define NODE_SIZE (sizeof(node_t))

//TYPE DEFINITIONS
typedef int status_t;
typedef void* data_t;
typedef data_t* p_data_t;

typedef struct list_node node_t;
typedef node_t* p_node_t;
typedef p_node_t* pp_node_t;

typedef struct singlylinkedlist sll_t;
typedef sll_t* p_sll_t;
typedef p_sll_t* pp_sll_t;

typedef void(* printDataProc)(data_t);
typedef void(* deleteDataProc)(data_t);
typedef status_t (*compareDataEqualityProc)(data_t, data_t);

//STRUCTURE TYPE DEFINITION
struct list_node 
{
    data_t data;
    p_node_t p_next;
};

struct singlylinkedlist
{
    p_node_t p_head;    //Head is pointing to sentinel node of list
    p_node_t p_tail;    //Tail is pointing to last node of the list
    size_t   size;      //Holds total number of nodes present in the list
};

//LIST INTERFACE FUNCTION DECLARATION
/**
* @function: create_singly_list
* @summary: This function will create singly linked list
* @params: takes nothing
* @returns: pointer to singly linked list object
*/
extern p_sll_t 
create_singly_list(void);

/**
* @function: sll_insert_front
* @summary: This function will insert the data to front i.e head of list
* @params: 1) Pointer to sll list
           2) Data 
* @returns: success if operation performed successfully otherwise failure
*/
extern status_t 
sll_insert_front(p_sll_t p_list, data_t data);

/**
* @func_name: sll_insert_back
* @summary: This function will insert back i.e at tail of list
* @params: It takes pointer to sll list and data to be inserted
* @returns: success if inserted successfully else failure
*/
extern status_t
sll_insert_back(p_sll_t p_list, data_t new_data);

/**
* @function: sll_insert_at
* @summary: This function insert data at particular index in the list
* @params: 1) List
           2) New data
           3) Index to which new data is inserted
* @returns: success if operation performed successfully otherwise failure
*/
extern status_t 
sll_insert_at(p_sll_t p_list, data_t new_data, size_t index);

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
sll_insert_before(p_sll_t p_list, data_t new_data, data_t e_data, compareDataEqualityProc p_compare_func);

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
sll_insert_after(p_sll_t p_list, data_t new_data, data_t e_data, compareDataEqualityProc p_compare_func);

//LIST DATA REMOVAL FUNCTIONS
/**
* @function: sll_remove_front
* @summary: This function will remove data from front of the list
* @params: List
* @returns: Data if removed successfully else null
*/
extern data_t
sll_remove_front(p_sll_t p_list);

/**
* @function: sll_remove_back
* @summary: This function will remove data from the back of the list
* @params: List
* @returns: Data if removed sucessfully else null
*/
extern data_t
sll_remove_back(p_sll_t p_list);

/**
* @function: sll_remove_at
* @summary: This function will remove data from specified index of the list
* @params: 1) List
           2) Index 
* @returns: Data if removed successfully else null
*/
extern data_t
sll_remove_at(p_sll_t p_list, size_t index);

/**
* @function: sll_remove_before
* @summary: This function will remove data before existing data
* @params: 1) List
           2) existing data
           3) Compare Callback function pointer
* @returns: Data if removed successfully else null
*/
extern data_t
sll_remove_before(p_sll_t p_list, data_t e_data, compareDataEqualityProc p_compare_func);

/**
* @function: sll_remove_after
* @summary: This function will remove data after the existing data
* @params: 1) List
           2) Existing data
           3) Compare Callback function pointer 
* @returns: Data if removed successfully else null
*/
extern data_t
sll_remove_after(p_sll_t p_list, data_t e_data, compareDataEqualityProc p_compare_func);

/**
* @function: sll_print_forward
* @summary: This function will display the state of the sll list in forward order
* @params: 1) List
           2) Print Callback function pointer
* @returns: success if displayed successfully else failure
*/
extern status_t
sll_print_forward(p_sll_t p_list, printDataProc p_print_func, const char* msg);

//LIST GETTER FUNCTIONS
/**
* @function: sll_get_head
* @summary: This function will return first element of the list
* @params: List
* @returns: first element else return null
*/
extern data_t
sll_get_head(p_sll_t p_list);

/**
* @function: sll_get_tail
* @summary: This function will return last element of the list
* @params: List
* @returns: last element else return null
*/

extern data_t
sll_get_tail(p_sll_t p_list);

/**
* @function: sll_get_size
* @summary: This function will return number of element in the list 
* @params: List
* @returns: number of elements counted
*/
extern size_t 
sll_get_size(p_sll_t p_list);

/**
* @function: sll_get_data_at
* @summary: This function will return data of particular index
* @params: 1) List
           2) Index
* @returns: Data if provided valid index else null
*/
extern data_t 
sll_get_data_at(p_sll_t p_list, size_t index);

//LIST DESTRUCTION FUNCTIONS
/**
* @function: destroy_singly_list
* @summary: This function will destroy whole singly linked list
* @params: 1) Pointer List
           2) Delete callback function pointer
* @returns: success if destroyed sucessfully else failure
*/
extern status_t
destroy_singly_list(pp_sll_t pp_list, deleteDataProc p_delete_func);

#endif
