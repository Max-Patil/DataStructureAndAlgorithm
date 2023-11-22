//HEADER FILES
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Hash.h"

//FUNCTION DEFINITIONS
void create_hashq(hashq_t** pphashq, data_t bucket_size) {

	if (!bucket_size)
	{
		*pphashq = NULL; 
		return;
	}

	*pphashq = (hashq_t*)xmalloc(sizeof(hashq_t));
	hashq_t* p_hashq = *pphashq;
	p_hashq->pparr_list = (list_t**)xmalloc(bucket_size * sizeof(list_t*));

	for (int i = 0; i < bucket_size; i++)
		p_hashq->pparr_list[i] = create_list();

	p_hashq->bucket_size = bucket_size;
}

key_t hash(data_t data, data_t bucket_size) {
	//printf("data : %d and bucket size : %d  is %d\n", data, bucket_size, data % bucket_size);
	return (data % bucket_size);
}

status_t add_hashq_data(hashq_t* phashq, data_t data) {
	key_t key = hash(data, phashq->bucket_size);
	
	/*if (is_list_empty(phashq->pparr_list[key]) == TRUE)
		phashq->pparr_list[key] = create_list();*/

	if (search_hashq_data(phashq, data) == FALSE)
		return insert_list_end(phashq->pparr_list[key], data);
	else
		return DATA_EXIST;
}

status_t search_hashq_data(hashq_t* phashq, data_t data) {

	key_t key = hash(data, phashq->bucket_size);

	list_t* p_list = phashq->pparr_list[key];

	return (search_list_node(p_list, data) != NULL);
}

status_t remove_hashq_data(hashq_t* phashq, data_t data) {
	key_t key = hash(data, phashq->bucket_size);


	if (search_list_node(phashq->pparr_list[key], data) != NULL) 
		return remove_list_data(phashq->pparr_list[key], data);
	else
		return DATA_NOT_EXIST;
}

void show_hashq(hashq_t* phashq, const char* msg) {
	if (msg) puts(msg);
	list_t* p_list = nullptr, * p_run = nullptr;

	for (int i = 0; i < phashq->bucket_size; i++) {
		p_list = phashq->pparr_list[i];
		p_run = p_list->next;
		printf("hashqueue[%d] : ", i);
		while (p_run != p_list) {
			printf("[%d]<->", p_run->data);
			p_run = p_run->next;
		}
		printf("[END]\n\n");
	}
	printf("\n");
}

void show_distribution(hashq_t* phashq) {

	printf("Size of Hash Queue : %d\n", phashq->bucket_size);

	for (int i = 0; i < phashq->bucket_size; i++)
		printf("hashqueue(%d) : %d\n", i, get_list_count(phashq->pparr_list[i]));
}

status_t destroy_hashq(hashq_t** pphashq) {

	if (*pphashq == NULL)
		return SUCCESS;

	hashq_t* p_hashq = *pphashq;

	/* Note: destroy_list Routines is not decrementing bucket size and therefore
	* we have directly use phashq->bucket in condition as it is not modified after
	* destroy_list routine is called */

	for (int i = 0; i < p_hashq->bucket_size; i++) 
		destroy_list(&(p_hashq->pparr_list[i]));

	free(p_hashq->pparr_list);
	free(p_hashq);
	*pphashq = NULL;

	return SUCCESS;
}

//Auxillary Routines for Hash Queue - Linked list Routines
list_t* create_list()
{
	list_t* p_list = get_list_node(-1);
	p_list->next = p_list;
	p_list->prev = p_list;
	return p_list;
}

status_t insert_list_end(list_t* p_list, data_t data) {
	generic_list_insert(p_list->prev, get_list_node(data), p_list);
	return SUCCESS;
}

status_t remove_list_data(list_t* p_list, data_t data) {
	node_t* p_node = search_list_node(p_list, data);
	if (p_node == NULL)
		return DATA_NOT_EXIST;
	generic_list_delete(p_node);
	return SUCCESS;
}

status_t destroy_list(list_t** pp_list) {
	if (*pp_list == NULL)
		return SUCCESS;
	list_t* p_list = *pp_list;
	node_t* p_run = p_list->next, * p_run_next = NULL;
	while (p_run != p_list) {
		p_run_next = p_run->next;
		free(p_run);
		p_run = p_run_next;
	}
	free(*pp_list);
	*pp_list = NULL;
	return SUCCESS;
}

//List Auxillary Routines
node_t* search_list_node(list_t* p_list, data_t data) {
	node_t* p_run = p_list->next;
	node_t* p_node = NULL;
	while (p_run != p_list) {
		if (p_run->data == data) {
			p_node = p_run;
			break;
		}
		p_run = p_run->next;
	}
	return p_node;
}

void generic_list_insert(node_t* p_beg, node_t* p_mid, node_t* p_end) {
	p_mid->next = p_end;
	p_mid->prev = p_beg;
	p_beg->next = p_mid;
	p_end->prev = p_mid;
}

void generic_list_delete(node_t* p_delete) {
	p_delete->prev->next = p_delete->next;
	p_delete->next->prev = p_delete->prev;
	free(p_delete);
}

node_t* get_list_node(data_t data) {
	node_t* new_node = (node_t*)malloc(sizeof(node_t));
	new_node->data = data;
	return new_node;
}

status_t is_list_empty(list_t* p_list) {
	return (p_list->next == p_list && p_list->prev == p_list);
}

status_t get_list_count(list_t* p_list) {
	node_t* p_run = p_list->next;
	int count = 0;
	while (p_run != p_list) {
		count++;
		p_run = p_run->next;
	}
	return count;
}

//GENERAL ROUTINE
void* xmalloc(size_t nr_bytes) {
	void* ptr = malloc(nr_bytes);
	if (ptr == NULL) {
		fprintf(stderr, "Malloc : Out of virtual memory");
		exit(EXIT_FAILURE);
	}
	memset(ptr, 0, nr_bytes);
	return ptr;
}