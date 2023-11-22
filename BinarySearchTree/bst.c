//HEADER FILES
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"BST.h"

// FUNCTION DEFINITIONS

//BST INTERFACE ROUTINE
void create_bst(bst_t** pp_bst) {
	*pp_bst = xmalloc(sizeof(bst_t));
	(*pp_bst)->p_root_node = NULL ;
	(*pp_bst)->nr_elements = 0;
}

status_t insert_bst_node(bst_t* p_bst, data_t data) {
	bst_node_t* new_node = get_bst_node(data);

	// Case 1: Given BST is empty
	if (p_bst->p_root_node == NULL) {
		p_bst->p_root_node = new_node;
		p_bst->nr_elements++;
		return SUCCESS;
	}

	// Case 2: Given BST is non-empty
	bst_node_t* p_run = p_bst->p_root_node;
	while (TRUE) {
		if (p_run->data >= data) {
			if (p_run->left == NULL) {
				p_run->left = new_node;
				new_node->parent = p_run;
				break;
			}
			else {
				p_run = p_run->left;
			}
		}
		else {
			if (p_run->right == NULL) {
				p_run->right = get_bst_node(data);
				new_node->parent = p_run;
				break;
			}
			else {
				p_run = p_run->right;
			}
		}
	}
	p_bst->nr_elements++;
	return SUCCESS;
}

status_t destroy_bst_rc(bst_t** pp_bst) {
	destroy_bst_node((*pp_bst)->p_root_node);
	free(*pp_bst);
	*pp_bst = NULL;
	return SUCCESS;
}

void postorder_rc(bst_t* p_bst) {
	postorder_traversal(p_bst->p_root_node);
	puts("[END]\n");
}

void preorder_rc(bst_t* p_bst) {
	
	preorder_traversal(p_bst->p_root_node);
	puts("[END]\n");
}

void inorder_rc(bst_t* p_bst) {

	preorder_traversal(p_bst->p_root_node);
	puts("[END]\n");
}

status_t minimum_bst(bst_t* p_bst, data_t* p_data) {
	if (p_bst->p_root_node == NULL) return BST_EMPTY;
	bst_node_t* p_run = p_bst->p_root_node;
	while (p_run->left != NULL) {
		p_run = p_run->left;
	}
	*p_data = p_run->data;
	return SUCCESS;
}

status_t maximum_bst(bst_t* p_bst, data_t* p_data) {
	if (p_bst->p_root_node == NULL) return BST_EMPTY;
	bst_node_t* p_run = p_bst->p_root_node;

	while (p_run->right != NULL) {
		p_run = p_run->right;
	}
	*p_data = p_run->data;
	return SUCCESS;
}
//DEPTH FIRST TRAVERSAL 
void inorder_nrc(bst_t* p_bst) {

	if (p_bst->p_root_node == NULL) return;

	stack_t* p_stack = NULL;
	create_bstptr_stack(&p_stack);
	bst_node_t* p_run = p_bst->p_root_node;

	printf("[BEG]<->");
	while (TRUE) {
		while (p_run != NULL) {
			push_bstptr(p_stack, p_run);
			p_run = p_run->left;
		}

		if (pop_bstptr(p_stack, &p_run) == STACK_EMPTY) break;
		printf("[%d]<->", p_run->data);

		p_run = p_run->right;
	}
	puts("[END]\n");

	destroy_bstptr_stack(&p_stack);

}

void preorder_nrc(bst_t* p_bst) {
	if (p_bst->p_root_node == NULL) return;
	stack_t* p_stack = NULL;

	create_bstptr_stack(&p_stack);
	bst_node_t* p_run = p_bst->p_root_node;
	printf("[BEG]<->");
	while (TRUE) {

		while (p_run != NULL) {
			printf("[%d]<->", p_run->data);
			push_bstptr(p_stack, p_run);
			p_run = p_run->left;
		}

		if (pop_bstptr(p_stack, &p_run) == STACK_EMPTY)
			break;
		p_run = p_run->right;
	}
	puts("[END]\n");

	destroy_bstptr_stack(&p_stack);
}

void postorder_nrc(bst_t* p_bst) {
	bst_node_t* p_run = p_bst->p_root_node;
	stack_t* p_stack = NULL;
	color_t color;

	create_bstptr_stack(&p_stack);
	printf("[BEG]<->");

	while (TRUE) {

		while (p_run != NULL) {

			push_bstptr(p_stack, p_run);
			set_bstptr_top_color(p_stack, GREY);
			p_run = p_run->left;

		}

		if (get_bstptr_top_color(p_stack, &color) == STACK_EMPTY) break;

		if (color == GREY) {
			top_bstptr(p_stack, &p_run);
			set_bstptr_top_color(p_stack, BLACK);
			p_run = p_run->right;
		}
		else if (color == BLACK) {
			pop_bstptr(p_stack, &p_run);
			printf("[%d]<->", p_run->data);
			p_run = NULL;
		}
	}
	puts("[END]");
}

//BFS TRAVERSAL
void breadth_first_traversal(bst_t* p_bst) {
	if (p_bst->p_root_node == NULL) return;

	queue_t* p_queue = NULL;
	create_bstptr_queue(&p_queue);

	bst_node_t* p_run = p_bst->p_root_node;
	enqueue_bstptr(p_queue, p_run);

	printf("[BEG]<->");
	while (is_bstptr_queue_empty(p_queue) != TRUE) {

		dequeue_bstptr(p_queue, &p_run);
		printf("[%d]<->", p_run->data);
		if (p_run->left != NULL)
			enqueue_bstptr(p_queue, p_run->left);

		if (p_run->right != NULL)
			enqueue_bstptr(p_queue, p_run->right);
	}
	puts("[END]\n");
}


status_t inorder_successor(bst_t* p_bst, data_t existing_data, data_t* p_succ_data)
{
	bst_node_t* p_existing_node = search_bst_nrc(p_bst, existing_data);
	if (p_existing_node == NULL) return BST_DATA_NOT_FOUND;

	bst_node_t* p_successor = inorder_successor_node(p_existing_node);
	if (p_successor != NULL) {
		*p_succ_data = p_successor->data;
		return SUCCESS;
	}
	return BST_NO_SUCCESSOR;
}

status_t inorder_predecessor(bst_t* p_bst, data_t existing_data, data_t* p_pred_data) {
	bst_node_t* p_existing_node = search_bst_rc(p_bst, existing_data);
	if (p_existing_node == NULL) return BST_DATA_NOT_FOUND;
	bst_node_t* p_predecessor = inorder_successor_node(p_existing_node);
	if (p_predecessor != NULL) {
		*p_pred_data = p_predecessor->data;
		return SUCCESS;
	}
	return BST_NO_PREDECESSOR;
}

status_t remove_bst_extended(bst_t* p_bst, data_t existing_data) {
	bst_node_t* p_existing_node = search_bst_nrc(p_bst, existing_data);
	if (p_existing_node == NULL) return BST_DATA_NOT_FOUND;

	bst_node_t* x = p_existing_node;
	/* left link may be NULL or may be not */
	if (x->right == NULL) {
		if (x == p_bst->p_root_node) {
			p_bst->p_root_node = x->left;
		}
		else if (x == x->parent->right) {
			x->parent->right = x->left;
		}
		else {
			x->parent->left = x->left;
		}

		if (x->left) {
			x->left->parent = x->parent;
		}
	}
	else if (x->left == NULL ) { /* right link != NULL && left link == NULL*/
		if (x == p_bst->p_root_node) {
			p_bst->p_root_node = x->right;
		}
		else if (x == x->parent->right) {
			x->parent->right = x->right;
		}
		else {
			x->parent->left = x->right;
		}

		if (x->right) { // This is not need to checked as it is checked in case 1
			x->right->parent = x->parent;
		}
	}
	else {
		//Case 3 : Left link != NULL && right link != NULL
		bst_node_t* z = inorder_successor_node(x);
		//printf("data : %d\n", z->data);
		/*Delegate responsibilty of z's right subtree( NULL OR NOT NULL )
		* to z's parent
		* Please Note :
		*		1] z cannot be root ( as it is a child of x), 
		*			therefore, usual if( z->parent == NULL) need not be checked
		*		
		*		2] z cannot be a right child of its parent because right child cannot
		*			be a min in subtree, therefore, if( z->parent->right == z ) need to checked
		* 
		*		3] If z->right is NULL then it will simply z's parent's left NULL which is desired
		*			we do not dereference z->right unless we are sure that it is NON-NULL
		* 
		*/
		if (x->right != z) { //This condition checks whether successor of x is right node or not
		
			z->parent->left = z->right;

			if (z->right)
				z->right->parent = z->parent;
			
			//Win Over x's right by z
			z->right = x->right;
			z->right->parent = z;
			

		} 
		/* Win Over x's parent by z */
		//Note : x can be a root node 

		if (x->parent == NULL) 
			p_bst->p_root_node = z;
		else if (x == x->parent->left) 
			x->parent->left = z;
		else
			x->parent->right = z;

		z->parent = x->parent;
		 
		/* Win Over x's left */
		z->left = x->left;
		z->left->parent = z;
	}

	free(x);
	x = NULL;
	p_bst->nr_elements--;

	return SUCCESS; 
}


status_t remove_bst(bst_t* p_bst, data_t existing_data) {
	bst_node_t* p_existing_node = search_bst_rc(p_bst, existing_data);
	if (p_existing_node == NULL) return BST_DATA_NOT_FOUND;
	bst_node_t* x = p_existing_node;
	if (x->left == NULL)
		transplant_bst_node(p_bst, x, x->right);
	else if (x->right == NULL)
		transplant_bst_node(p_bst, x, x->left);
	else {
		bst_node_t* z = inorder_successor_node(x);
		if (x->right != z) {
			transplant_bst_node(p_bst, z, z->right);
	
			//Win over x's right subtree
			z->right = x->right;
			z->right->parent = z;
		
		}
		//Win over x's parent 
		transplant_bst_node(p_bst, x, z); //It is not transplant_bst_node(p_bst, x->parent, z);
	
		//Win over x's left subtree
		z->left = x->left;
		z->left->parent = z;
	}

	free(x);
	x = NULL;
	p_bst->nr_elements--;
	return SUCCESS;
}


//BST AUXILLARY ROUTINES
bst_node_t* get_bst_node(data_t data) {
	bst_node_t* new_node = xmalloc(sizeof(bst_node_t));
	new_node->data = data;
	return new_node;
}

bst_node_t* search_bst_nrc(bst_t* p_bst, data_t data) {
	bst_node_t* p_run = p_bst->p_root_node;
	bst_node_t* p_found = NULL;
	while (p_run != NULL) {

		if (p_run->data == data) {
			p_found = p_run;
			break;
		}
		else if (p_run->data > data) {
			p_run = p_run->left;
		}
		else {
			p_run = p_run->right;
		}
	}
	return p_found;
}

bst_node_t* search_bst_rc(bst_t* p_bst, data_t data) {
	return search_bst_node(p_bst->p_root_node, data);
}

bst_node_t* search_bst_node(bst_node_t* p_bst_node, data_t data) {

	if (p_bst_node == NULL) {
		return NULL;
	}

	if (p_bst_node->data == data) {
		return p_bst_node;
	}
	else if (p_bst_node->data > data) {
		return search_bst_node(p_bst_node->left, data);
	}
	else
	{
		return search_bst_node(p_bst_node->right, data);
	}
}

bst_node_t* inorder_successor_node(bst_node_t* p_bst_node) {
	bst_node_t* x = p_bst_node;
	//Case 1: right node of a given node is non-empty
	if (x->right != NULL) {
		x = x->right;
		while (x->left != NULL) {
			x = x->left;
		}
		return x;
	}

	//Case 2: right node of a given node is empty
	bst_node_t* y = x->parent;

	//Note : if given node i.e x is root node then x do not have any successor and
	// x->parent will be NULL which will terminate below condition

	// x will query parent whether it is right child of parent and follow that uplink 
	//untill x becomes left child of its parent in that case its parent will be our successor
	while (y != NULL && x == y->right) {
		x = y;
		y = x->parent;
	}

	return y;
}

bst_node_t* inorder_predecessor_node(bst_node_t* p_bst_node) {
	bst_node_t* x = p_bst_node;
	if (x->left != NULL) {
		x = x->left;
		while (x->right != NULL) {
			x = x->right;
		}
		return x;
	}

	bst_node_t* y = x->parent;
	while (y != NULL && x == y->left) {
		x = y;
		y = x->parent;
	}
	return y;
}

void destroy_bst_node(bst_node_t* p_node) {
	if (p_node == NULL) {
		return;
	}
	destroy_bst_node(p_node->left);
	destroy_bst_node(p_node->right);
	free(p_node);
}

void inorder_traversal(bst_node_t* p_node) {
	if (p_node == NULL) {
		return;
	}
	inorder_traversal(p_node->left);
	printf("[%d]<->", p_node->data);
	inorder_traversal(p_node->right);
	return;
}

void preorder_traversal(bst_node_t* p_node) {
	if (p_node == NULL) {
		return;
	}
	printf("[%d]<->", p_node->data);
	preorder_traversal(p_node->left);
	preorder_traversal(p_node->right);
}

void postorder_traversal(bst_node_t* p_node) {
	if (p_node == NULL) {
		return;
	}
	postorder_traversal(p_node->left);
	postorder_traversal(p_node->right);
	printf("[%d]<->", p_node->data);
}

void transplant_bst_node(bst_t* p_bst, bst_node_t* u, bst_node_t* v) {
	
	if (u->parent == NULL)
		p_bst->p_root_node = v;
	else if (u == u->parent->right)
		u->parent->right = v;
	else
		u->parent->left = v;

	if (v) {
		v->parent = u->parent;
	}
}

//BST AUXILLARY ROUTINES - LIST INTERFACE ROUTINES
void create_bstptr_list(list_t** pp_list) {
	*pp_list = xmalloc(sizeof(list_t));
	bstptr_t* p_list = *pp_list;
	p_list->p_bst_node = NULL;
	p_list->color = 0;
	p_list->prev = p_list;
	p_list->next = p_list;
}

status_t insert_list_end(list_t* p_list, bst_node_t* p_bst_node) {
	generic_bstptr_insert(p_list->prev, get_bstptr_node(p_bst_node), p_list);
	return SUCCESS;
}

status_t insert_list_beg(list_t* p_list, bst_node_t* p_bst_node) {
	generic_bstptr_insert(p_list, get_bstptr_node(p_bst_node), p_list->next);
	return SUCCESS;
}

status_t remove_list_end(list_t* p_list) {
	if (is_bstptr_list_empty(p_list) == TRUE) {
		return LIST_EMPTY;
	}
	generic_bstptr_delete(p_list->prev);
	return SUCCESS;
}

status_t remove_list_beg(list_t* p_list) {
	if (is_bstptr_list_empty(p_list) == TRUE) {
		return LIST_EMPTY;
	}

	generic_bstptr_delete(p_list->next);
	return SUCCESS;
}


status_t destroy_bstptr_list(list_t** pp_list) {
	bstptr_t* p_run = (*pp_list)->next;
	bstptr_t* p_run_next = NULL;
	list_t* p_list = *pp_list;
	while (p_run != p_list) {
		p_run_next = p_run;
		free(p_run);
		p_run = p_run_next;
	}
	free(*pp_list);
	return SUCCESS;
}


// LIST AUXILLARY ROUTINES
void generic_bstptr_insert(bstptr_t* p_beg, bstptr_t* p_mid, bstptr_t* p_end) {
	p_mid->prev = p_beg;
	p_mid->next = p_end;
	p_beg->next = p_mid;
	p_end->prev = p_mid;
}

void generic_bstptr_delete(bstptr_t* p_delete) {
	p_delete->prev->next = p_delete->next;
	p_delete->next->prev = p_delete->prev;
	free(p_delete);
}

status_t is_bstptr_list_empty(list_t* p_list) {
	if (p_list->next == p_list && p_list->prev == p_list) {
		return TRUE;
	}
	return FALSE;
}

bstptr_t* get_bstptr_node(bst_node_t* p_bst_node) {
	bstptr_t* new_node = xmalloc(sizeof(bstptr_t));
	new_node->color = WHITE;
	new_node->p_bst_node = p_bst_node;
	return new_node;
}

bstptr_t* search_bstptr_node(list_t* p_list, bst_node_t* p_bst_node) {
	bstptr_t* p_run = p_list->next;
	bstptr_t* p_search = NULL;
	while (p_run != p_list) {
		if (p_run->p_bst_node == p_bst_node) {
			p_search = p_run;

			break;
		}
		p_run = p_run->next;
	}
	return p_search;
}

//STACK INTERFACE ROUTINES
void create_bstptr_stack(stack_t** pp_stack) {
	create_bstptr_list(pp_stack);
}

status_t push_bstptr(stack_t* p_stack, bst_node_t* p_bst_node) {
	return insert_list_end(p_stack, p_bst_node);
}

status_t top_bstptr(stack_t* p_stack, bst_node_t** pp_bst_node) {
	*pp_bst_node = p_stack->prev->p_bst_node;
	return SUCCESS;
}

status_t pop_bstptr(stack_t* p_stack, bst_node_t** pp_bst_node) {
	*pp_bst_node = p_stack->prev->p_bst_node;
	return remove_list_end(p_stack);
}

status_t set_bstptr_top_color(stack_t* p_stack, color_t new_color) {
	if (is_bstptr_stack_empty(p_stack) == TRUE) return STACK_EMPTY;
	p_stack->prev->color = new_color;
	return SUCCESS;
}

status_t get_bstptr_top_color(stack_t* p_stack, color_t* p_top_color) {
	if (is_bstptr_stack_empty(p_stack) == TRUE) return STACK_EMPTY;
	*p_top_color = p_stack->prev->color;
	return SUCCESS;
}

status_t is_bstptr_stack_empty(stack_t* p_stack) {
	return is_bstptr_list_empty(p_stack);
}

status_t destroy_bstptr_stack(stack_t** pp_stack) {
	return destroy_bstptr_list(pp_stack);
}

void create_bstptr_queue(queue_t** pp_queue) {
	create_bstptr_list(pp_queue);
}

status_t enqueue_bstptr(queue_t* p_queue, bst_node_t* p_bst_node) {
	return insert_list_end(p_queue, p_bst_node);
}

status_t dequeue_bstptr(queue_t* p_queue, bst_node_t** pp_bst_node) {
	*pp_bst_node = p_queue->next->p_bst_node;
	return remove_list_beg(p_queue);
}

status_t destroy_bstptr_queue(queue_t** pp_queue) {
	return destroy_bstptr_list(pp_queue);
}

status_t is_bstptr_queue_empty(queue_t* p_queue) {
	return is_bstptr_list_empty(p_queue);
}

void* xmalloc(size_t nr_bytes) {
	void* ptr = malloc(nr_bytes);
	if (ptr == NULL) {
		fprintf(stderr, "Malloc : Out of virtual Memory\n");
		exit(0);
	}
	memset(ptr, 0, nr_bytes);
	return ptr;
}