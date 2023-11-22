//HEADER FILES
#include<stdio.h>
#include<stdlib.h>
#include"deque.h"

//MACROS
#define CLIENT_DATA_SIZE (sizeof(dummy_client_t))

//TYPE DEFINITIONS
typedef struct dummy_client dummy_client_t;
typedef dummy_client_t* p_dummy_client_t;

//STRUCTURE TYPE DEFINTIONS
struct dummy_client
{
    int inum;
    float fnum;
    char cnum;
};

static p_dummy_client_t init_data(int inum, long fnum, char cnum)
{
    p_dummy_client_t p_client = (p_dummy_client_t) malloc(CLIENT_DATA_SIZE);
    
    if (p_client == NULL)
    {
        fprintf(stderr, "ERROR: Out Of Memory.\n");
        exit(EXIT_FAILURE);
    }

    p_client->inum = inum;
    p_client->fnum = fnum;
    p_client->cnum = cnum;
    return (p_client);
}

//CALLBACK FUNCTION PROVIDED BY CLIENT
void print_clientData(data_t data)
{
   p_dummy_client_t p_client = (p_dummy_client_t)data;
   printf("{ %3d | %c | %3.2f }\n", p_client->inum, p_client->cnum, p_client->fnum);
}

void delete_clientData(data_t data)
{
    if(data)
    {
        print_clientData(data);
        free(data);
    }
}

//ENTRY POINT OF ALL FUNCTIONS
int main(void)
{
    p_deque_t p_client = create_default_deque();

    dummy_client_t client_data[26];

    for(size_t le = 0; le <= 26; le++)
    {
        client_data[le].inum = 97 + le;
        client_data[le].fnum = 97 + le;
        client_data[le].cnum = 97 + le;
    }

    //Adding data from rear of deque
    puts("Enqueue Data to Rear : ");
    for (long le = 0; le < 15; ++le)
        deque_enqueue_rear(p_client, (data_t)(&client_data[le]));
    
    deque_print(p_client, print_clientData);

    //Adding data to front of deque
    puts("Enqueue Data to Front : ");
    for (long le = 15; le < 27; ++le)
        deque_enqueue_front(p_client, (data_t)(&client_data[le]));
    deque_print(p_client, print_clientData);

    //Removing data from front of deque
    puts("Dequeue Data from Front : ");
    for (long le = 0; le < 10; ++le)
        print_clientData(deque_dequeue_front(p_client));
    puts("[END]");
    deque_print(p_client, print_clientData);

    //Removing data from rear of deque
    puts("Dequeue Data from Rear : ");
    for (long le = 0; le < 10; ++le)
        print_clientData(deque_dequeue_rear(p_client));
    puts("[END]");
    deque_print(p_client, print_clientData);

    //Destroying long deque
    deque_destroy(&p_client, delete_clientData);

    exit(EXIT_SUCCESS);
}
