#pragma once

//SYMBOLIC CONSTANT i.e ERROR CODES
#define FALSE 0
#define SUCCESS 1
#define TRUE SUCCESS
#define BST_DATA_NOT_FOUND 2
#define BST_EMPTY 3
#define BST_NO_PREDECESSOR 4
#define BST_NO_SUCCESSOR 5
#define LIST_EMPTY 6
#define STACK_EMPTY LIST_EMPTY
#define QUEUE_EMPTY LIST_EMPTY

//TYPE DEFINITIONS
typedef struct bst bst_t;
typedef struct bst_node bst_node_t;
typedef int data_t;
typedef int status_t;
typedef enum color color_t;
typedef struct bstptr bstptr_t;
typedef bstptr_t list_t;
typedef list_t stack_t;
typedef list_t queue_t;

//GLOBAL STRUCTURE
struct bst {
	bst_node_t* p_root_node;
	size_t nr_elements;
};

struct bst_node {
	data_t data;
	bst_node_t* left;
	bst_node_t* right;
	bst_node_t* parent;
};

enum color {
	WHITE,
	GREY,
	BLACK,
};

struct bstptr {
	bst_node_t* p_bst_node;
	color_t color;
	bstptr_t* prev;
	bstptr_t* next;
};

//BST INTERFACE ROUTINES
void create_bst(bst_t** pp_bst);
status_t insert_bst_node(bst_t* p_bst, data_t data);
status_t destroy_bst_rc(bst_t** pp_bst);
void inorder_rc(bst_t* p_bst);
void preorder_rc(bst_t* p_bst);
void postorder_rc(bst_t* p_bst);

void inorder_nrc(bst_t* p_bst);
void preorder_nrc(bst_t* p_bst);
void postorder_nrc(bst_t* p_bst);

void breadth_first_traversal(bst_t*);

status_t minimum_bst(bst_t* p_bst, data_t* p_data);
status_t maximum_bst(bst_t* p_bst, data_t* p_data);

status_t inorder_successor(bst_t* p_bst, data_t existing_data, data_t* p_succ_data);
status_t inorder_predecessor(bst_t* p_bst, data_t existing_data, data_t* p_pred_data);
status_t remove_bst_extended(bst_t* p_bst, data_t existing_data);
status_t remove_bst(bst_t* p_bst, data_t existing_data);


//BST AUXILLARY ROUTINES 
bst_node_t* get_bst_node(data_t);
void* xmalloc(size_t nr_bytes);
bst_node_t* search_bst_node(bst_node_t* ,data_t);
bst_node_t* search_bst_rc(bst_t*, data_t);
bst_node_t* search_bst_nrc(bst_t*, data_t);
void destroy_bst_node(bst_node_t*);

void inorder_traversal(bst_node_t*);
void preorder_traversal(bst_node_t*);
void postorder_traversal(bst_node_t*);

bst_node_t* inorder_successor_node(bst_node_t* p_node);
bst_node_t* inorder_predecessor_node(bst_node_t* p_node);
void transplant_bst_node(bst_t* p_bst, bst_node_t* u, bst_node_t* v);

//BST AUXILLARY ROUTINES - LIST INTERFACE ROUTINES
void create_bstptr_list(list_t**);
status_t insert_list_end(list_t*, bst_node_t*);
status_t insert_list_beg(list_t*, bst_node_t*);
status_t remove_list_end(list_t*);
status_t remove_list_beg(list_t*);
status_t destroy_bstptr_list(list_t**);

//LIST AUXILLARY ROUTINES
void generic_bstptr_insert(bstptr_t* p_beg, bstptr_t* p_mid, bstptr_t* p_end);
void generic_bstptr_delete(bstptr_t* p_delete);
bstptr_t* get_bstptr_node(bst_node_t*);
bstptr_t* search_bstptr_node(list_t*, bst_node_t*);
status_t is_bstptr_list_empty(list_t*);

//STACK INTERFACE ROUTINES
void create_bstptr_stack(stack_t** pp_stack);
status_t push_bstptr(stack_t* p_stack, bst_node_t* p_bst_node);
status_t top_bstptr(stack_t* p_stack, bst_node_t**);
status_t pop_bstptr(stack_t*, bst_node_t**);
status_t set_bstptr_top_color(stack_t*, color_t);
status_t get_bstptr_top_color(stack_t*, color_t*);
status_t is_bstptr_stack_empty(stack_t*);
status_t destroy_bstptr_stack(stack_t**);

//QUEUE INTERFACE ROUTINES
void create_bstptr_queue(queue_t** pp_queue);
status_t enqueue_bstptr(queue_t* p_queue, bst_node_t* p_bst_node);
status_t dequeue_bstptr(queue_t* p_queue, bst_node_t** pp_bst_node);
status_t destroy_bstptr_queue(queue_t** pp_queue);

//QUEUE AUXILLARY ROUTINE
status_t is_bstptr_queue_empty(queue_t* p_queue);


