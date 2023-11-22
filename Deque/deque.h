//HEADER FILES
#include<stdio.h>
#include<stdlib.h>

//AVOID REPEATED EXPANSION
#ifndef _DEQUE_H
#define _DEQUE_H

//SYMBOLIC CONSTANTS
#ifndef SUCCESS
#define SUCCESS 1
#endif 

#ifndef FAILURE 
#define FAILURE 0
#endif

#define TRUE SUCCESS
#define FALSE FAILURE
#define DATA_SIZE (sizeof(data_t))
#define DEQUE_SIZE (sizeof(deque_t))

//TYPE DEFINITIONS
typedef struct deque deque_t;
typedef deque_t * p_deque_t;
typedef p_deque_t* pp_deque_t;
typedef int status_t;
typedef void* data_t;
typedef data_t* p_data_t;
typedef void(*printDataProc)(data_t);
typedef void(*deleteDataProc)(data_t);
typedef void* (*errorHandler)(void*);

//STRUCTURE TYPE DEFINITIONS
struct deque
{
    p_data_t p_array;
    long head;
    long tail;
    size_t length;
    size_t nr_elements;
};

//FUNCTION DECLARATIONS

//INTERFACE FUNCTION DECLARATIONS

//Data structure creation methods
extern p_deque_t create_default_deque(void);
extern p_deque_t create_custom_deque(size_t nr_elements);

//Data insertion methods
extern status_t deque_enqueue_front(p_deque_t p_deque, data_t data);
extern status_t deque_enqueue_rear(p_deque_t p_deque, data_t data);

//Data removal methods
extern data_t deque_dequeue_front(p_deque_t p_deque);
extern data_t deque_dequeue_rear(p_deque_t p_deque);

//Data members accessor methods
extern size_t deque_size(const p_deque_t p_deque);
extern size_t deque_nr_elements(const p_deque_t p_deque);

//Data display method
extern status_t deque_print(p_deque_t p_deque, printDataProc p_print_func);

//Data structure destruction method
extern status_t deque_destroy(pp_deque_t pp_deque, deleteDataProc p_delete_func);

#endif //_DEQUE_H
