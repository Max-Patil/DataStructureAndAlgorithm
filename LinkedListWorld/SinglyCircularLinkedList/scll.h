#ifndef _SCLL_H
#define _SCLL_H

//MACROS & SYMBOLIC CONSTANTS
#define SUCCESS 1
#define FAILURE 0
#define TRUE SUCCESS
#define FALSE FAILURE

#define NODE_SIZE (sizeof(node_t))
#define LIST_SIZE (sizeof(scll_t))

//TYPE DEFINITIONS
typedef int status_t;
typedef void* data_t;
typedef data_t* p_data_t;

typedef struct list_node node_t;
typedef node_t* p_node_t;

typedef struct singly_circular_linked_list scll_t;
typedef scll_t* p_scll_t;
typedef p_scll_t* pp_scll_t;

typedef void(*printDataProc)(data_t);
typedef void(*deleteDataProc)(data_t);
typedef status_t (*compareDataProc)(data_t, data_t);

struct list_node 
{
    data_t data;
    p_node_t p_next;
};

struct singly_circular_linked_list
{
    p_node_t p_head;
    p_node_t p_tail;
    size_t nr_elements;
};

//LIST INTERFACE ROUTINES
extern p_scll_t create_singly_circular_list(void);

extern status_t scll_insert_back(p_scll_t p_list, data_t new_data);

extern status_t scll_insert_front(p_scll_t p_list, data_t new_data);

extern status_t scll_insert_at(p_scll_t p_list, data_t new_data, size_t index);

extern status_t scll_insert_before(p_scll_t p_list, data_t new_data, data_t e_data, compareDataProc p_compare_func);

extern status_t scll_insert_after(p_scll_t p_list, data_t new_data, data_t e_data, compareDataProc p_compare_func);

//LIST REMOVAL OPERATIONS
extern data_t scll_remove_back(p_scll_t p_list);

extern data_t scll_remove_front(p_scll_t p_list);

extern data_t scll_remove_at(p_scll_t p_list, size_t index);

extern data_t scll_remove_after(p_scll_t p_list, data_t e_data, compareDataProc p_compare_func);

extern data_t scll_remove_before(p_scll_t p_list, data_t e_data, compareDataProc p_compare_func);

//LIST GETTER FUNCTIONS

extern data_t scll_get_head(p_scll_t p_list);

extern data_t scll_get_tail(p_scll_t p_list);

extern size_t scll_get_size(p_scll_t p_list);

extern status_t scll_is_scll_empty(p_scll_t p_list);

extern data_t scll_get_data_at(p_scll_t p_list, size_t index);

extern status_t scll_print_forward(p_scll_t p_list, printDataProc p_print_func, const char* msg);

extern status_t scll_print_reverse(p_scll_t p_list, printDataProc p_print_func, const char* msg);

extern status_t destroy_scll(pp_scll_t pp_list, deleteDataProc p_delete_func);

#endif // _SCLL_H