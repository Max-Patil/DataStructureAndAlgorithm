#pragma once
/* Design and implement a hashing strategy based on modulo function/ The bucket size will be decided 
by the client */

//SYMBOLIC CONSTANT
#define TRUE 1
#define FALSE 0
#define SUCCESS TRUE
#define DATA_NOT_EXIST FALSE;
#define DATA_EXIST TRUE;

//TYPE DEFINITIONS
typedef int key_t;
typedef int status_t;
typedef int data_t;
typedef struct node node_t;
typedef node_t list_t;
typedef struct hashqueue hashq_t;

//STRUCTURES
struct hashqueue {
	list_t** pparr_list; //Array of linked list
	size_t bucket_size;  //Table size
};
//Note : hashtable, hashMap and hash queue are same 

struct node{

	data_t data;
	node_t* next;
	node_t* prev;

};

//FUNCTION DECLARATIONS

//Interface Routines for Hash Queue
void create_hashq(hashq_t** pphashq, data_t bucket_size);
key_t hash(data_t data, data_t bucket_size);
status_t add_hashq_data(hashq_t* phashq, data_t data);
status_t search_hashq_data(hashq_t* phashq, data_t data);
status_t remove_hashq_data(hashq_t* phashq, data_t data);
void show_hashq(hashq_t* phashq, const char* msg);
void show_distribution(hashq_t* phashq);
status_t destroy_hashq(hashq_t** pphashq);

//Auxillary Routines for Hash Queue - Linked list Routines
list_t* create_list();
status_t insert_list_end(list_t* p_list, data_t data);
status_t remove_list_data(list_t* p_list, data_t data);
status_t destroy_list(list_t** pp_list);


void generic_list_insert(node_t* p_beg, node_t* p_mid, node_t* p_end);
void generic_list_delete(node_t* p_delete);
node_t* search_list_node(list_t* p_list, data_t data);
node_t* get_list_node(data_t);
status_t is_list_empty(list_t* p_list);
status_t get_list_count(list_t* p_list);

//GENERAL ROUTINE
void* xmalloc(size_t nr_bytes);
