//HEADER FILES
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<time.h>
#include<assert.h>
#include"dll.h"

//MARCOS
#define NEW_LINE puts("{\t\tEND\t\t\t\t}\n");

//TYPE DEFINITIONS
typedef struct date date_t;
typedef date_t* p_date_t;
typedef struct credit_card credit_card_t;
typedef credit_card_t* p_credit_card_t;
typedef struct bank bank_client_t;
typedef bank_client_t* p_bank_client_t;

//STRUCTURE TYPE DEFINITION
struct date
{
    int dd, mm, yy;
};

struct credit_card
{
    char* name;             //card holder name
    uint64_t acc_no;        //Account number
    date_t expiry_date;     //expiry date
    uint16_t cvv;           //Card Verification Value
};

struct bank
{
     char* bank_info;        //Bank information
     credit_card_t* credit_info;
};

//CLIENT FUNCTION DECLARATIONS

//Client Callback helper functions
static p_bank_client_t create_client(char* bank_info, credit_card_t* credit_info);
static p_credit_card_t init_credit_card(char* name, date_t date);
static void delete_credit_card_info(p_data_t p_credit_card);
static void display_credit_card_info(data_t credit_card);

//Callback functions 
static void delete_client_info(data_t client_data);
static void display_client_info(data_t client_data);
static status_t compare_client_info(data_t client_data_1, data_t client_data_2);

date_t card_exp_date[] = { 
                             {3, 11, 2055},  {12, 11, 2055},  {1, 11, 2052}, {6, 11, 2052},  {30, 11, 2052}, 
                             {16, 11, 2052}, {18, 11, 2052},  {8, 11, 2052}, {10, 11, 2052}, {11, 11, 2052},
                             {6, 11, 2052},  {2, 11, 2052},   {9, 11, 2052}, {17, 11, 2052}, {12, 11, 2052},
                             {23, 11, 2052}, {27, 11, 2052},  {21, 9, 2055}, 
                           };

//ENTRY POINT OF ALL FUNCTIONS
int main(void)
{

p_bank_client_t p_clients[] = { 
                                    create_client("Oximore, Texas Branch", init_credit_card("Harshal", card_exp_date[0])),
                                    create_client("Oximore, Texas Branch", init_credit_card("Payal",   card_exp_date[1]) ) ,
                                    create_client("Oximore, Texas Branch", init_credit_card("Jayesh",  card_exp_date[2])) ,
                                    create_client("Oximore, Texas Branch", init_credit_card("Payal",   card_exp_date[3])),
                                    create_client("Oximore, Texas Branch", init_credit_card("Ritesh",  card_exp_date[4])),
                                    create_client("Oximore, Saint Petersburg Branch", init_credit_card("Atharv",  card_exp_date[5])),
                                    create_client("Oximore, Saint Petersburg Branch", init_credit_card("Hemanshu",card_exp_date[6])),
                                    create_client("Oximore, Saint Petersburg Branch", init_credit_card("Kirti",   card_exp_date[7])),
                                    create_client("Oximore, Belgium Branch", init_credit_card("Yogesh",  card_exp_date[8])),
                                    create_client("Oximore, Belgium Branch", init_credit_card("dinesh",  card_exp_date[9])),
                                    create_client("Oximore, Belgium Branch", init_credit_card("Rahul",   card_exp_date[10])),
                                    create_client("Oximore, Belgium Branch", init_credit_card("Manik",   card_exp_date[11])),
                                    create_client("Oximore, Belgium Branch", init_credit_card("Yevu",    card_exp_date[12])),

                                    create_client("Oximore, India Branch", init_credit_card("Surya",   card_exp_date[13])),
                                    create_client("Oximore, India Branch", init_credit_card("jeevan",  card_exp_date[14])),
                                    create_client("Oximore, India Branch", init_credit_card("sharthik",card_exp_date[15])),
                                    create_client("Oximore, India Branch", init_credit_card("Yevu", card_exp_date[16])),
                                    create_client("Oximore, India Branch", init_credit_card("aakash",  card_exp_date[17])),
                            };
//Creating Simple database that is based on linked list and its simple operation
p_dll_t p_bank_client_list = create_dll(); 
srand(time(NULL));

//Adding data to back of the list
for(int client_id = 5; client_id < 8; client_id++)
    dll_insert_back(p_bank_client_list, (data_t)p_clients[client_id]);
dll_print_forward(p_bank_client_list, display_client_info, "After pushing 3 client of Saint Petersburg to back of database(doubly linked list) : ");

//Adding data to front of the list
for(int client_id = 0; client_id < 5; client_id++)
    dll_insert_front(p_bank_client_list, (data_t)p_clients[client_id]);
dll_print_forward(p_bank_client_list, display_client_info, "After pushing 5 client of Texas to front of database(doubly linked list) : ");

//Adding data to specified index of the list
dll_insert_at(p_bank_client_list, (data_t)p_clients[8], 0);
dll_insert_at(p_bank_client_list, (data_t)p_clients[9], 0);
dll_insert_at(p_bank_client_list, (data_t)p_clients[10], dll_get_size(p_bank_client_list));
dll_insert_at(p_bank_client_list, (data_t)p_clients[11], dll_get_size(p_bank_client_list));
dll_insert_at(p_bank_client_list, (data_t)p_clients[12], 7);
dll_print_forward(p_bank_client_list, display_client_info, "After pushing 5 client of Belgium to database(doubly linked list) at specified index : ");

//Adding data before exisiting data of the list
dll_insert_before(p_bank_client_list, (data_t)p_clients[13], (data_t)p_clients[9], compare_client_info);
dll_insert_before(p_bank_client_list, (data_t)p_clients[14], (data_t)p_clients[9], compare_client_info);
//dll_insert_before(p_bank_client_list, (data_t)p_clients[15], NULL, compare_client_info);
dll_print_forward(p_bank_client_list, display_client_info, "After pushing 2 client of India before existing client of database(doubly linked list) : ");

//Adding data after existing data of the list
dll_insert_after(p_bank_client_list, p_clients[15], dll_get_tail(p_bank_client_list), compare_client_info);
dll_insert_after(p_bank_client_list, p_clients[16], dll_get_tail(p_bank_client_list), compare_client_info);
dll_insert_after(p_bank_client_list, p_clients[17], dll_get_head(p_bank_client_list), compare_client_info);
dll_print_forward(p_bank_client_list, display_client_info, "After pushing 3 client of India after existing client of database(doubly linked list) : ");

//Removing data from front of the list
puts("Removing 4 client from the front of database(doubly linked list) : ");
for(int le = 0; le < 4; le++)
    display_client_info((data_t)dll_remove_front(p_bank_client_list)); NEW_LINE
dll_print_forward(p_bank_client_list, display_client_info, "\nAfter removing above 4 client from the front of database(doubly linked list) : ");
dll_print_reverse(p_bank_client_list, display_client_info, "Displaying the database in reverse order : ");

//Removing data from back of the list
puts("Removing 4 client from the back of the database(doubly linked list) : ");
for(int le = 0; le < 4; le++)
    display_client_info((data_t)dll_remove_back(p_bank_client_list)); NEW_LINE
dll_print_forward(p_bank_client_list, display_client_info, "After removing 4 client from the back of database(doubly linked list) : ");

//Removing data of specified index of the list
puts("Removing 4 client at specified index of the database(doubly linked list)");
for(int index = 0; index < 4; index++)
    display_client_info(dll_remove_at(p_bank_client_list, index)); NEW_LINE
dll_print_forward(p_bank_client_list, (data_t)display_client_info, "\nAfter removing above 4 client from the database(doubly linked list) of specified index : ");
dll_print_reverse(p_bank_client_list, (data_t)display_client_info, "Displaying the database(doubly linked list) in reverse order : ");

//Destroying Simple database that is based on linked list and its simple operation
destroy_dll(&p_bank_client_list, delete_client_info);
return EXIT_SUCCESS;
}

p_bank_client_t create_client(char* bank_info, credit_card_t* credit_info)
{
    bank_client_t *p_client = (bank_client_t*)malloc(sizeof(bank_client_t));

    p_client->bank_info = (char*)malloc(strlen(bank_info) + 1);
    strncpy(p_client->bank_info, bank_info, strlen(bank_info));
    p_client->bank_info[strlen(bank_info)] = '\0';

    p_client->credit_info = credit_info;
    return p_client;
}

static void delete_client_info(data_t client_data)
{   
    p_bank_client_t p_client = (p_bank_client_t)client_data;
    printf("-->Deleting : %15s client from the database(doubly linked list)\n", p_client->credit_info->name);
  

    if(p_client->bank_info)
    {
         free(p_client->bank_info); 
         p_client->bank_info = NULL;
    }

    if(p_client->credit_info)
    {
        delete_credit_card_info(&p_client->credit_info);
    }

    if(p_client)
    {
        free(p_client);
        p_client = NULL;
    }
    
}

p_credit_card_t init_credit_card(char* name, date_t date)
{
    
    credit_card_t* p_credit_card = (credit_card_t*)malloc(sizeof(credit_card_t));

    //Initializing the client name
    p_credit_card->name = strdup(name);

    //Initializing randomly generated account number
    p_credit_card->acc_no = 93493493943 - rand();

    //Initailizing expiry date
    p_credit_card->expiry_date.dd = date.dd;
    p_credit_card->expiry_date.mm = date.mm;
    p_credit_card->expiry_date.yy = date.yy;

    //Initializing randomly generated cvv number
    p_credit_card->cvv = (rand() % (934 - 100 + 1)) + 100;

    return p_credit_card;

}

static void delete_credit_card_info(p_data_t p_credit_card)
{
    credit_card_t* credit_card = *((credit_card_t**)p_credit_card);
    
    if(credit_card->name)
    {
        free(credit_card->name); 
        credit_card->name = NULL;
    }

    credit_card->acc_no = credit_card->cvv = 0;

    if(credit_card)
    {
        free(credit_card);
        credit_card = NULL;
    }
    *p_credit_card = NULL;
}

static void display_credit_card_info(data_t credit_card)
{
    p_credit_card_t p_credit_card = (credit_card_t*)credit_card;
    printf("\n{%-15s | %lld | %d/%d/%d | %hu }\n\t\t |\n", p_credit_card->name, 
                                                p_credit_card->acc_no, 
                                                p_credit_card->expiry_date.dd,
                                                p_credit_card->expiry_date.mm,
                                                p_credit_card->expiry_date.yy,
                                                p_credit_card->cvv);
}

static void display_client_info(data_t data_client)
{
    bank_client_t *p_client = (bank_client_t*)data_client;

    if(p_client == NULL) return;

    printf("Bank : %s", p_client->bank_info);
    display_credit_card_info((data_t)p_client->credit_info);
}

static status_t compare_client_info(data_t client_data_1, data_t client_data_2)
{
    p_bank_client_t p_client_1 = (p_bank_client_t)client_data_1;
    p_bank_client_t p_client_2 = (p_bank_client_t)client_data_2;

    if(p_client_1 && p_client_2)
    {
        if(strcmp(p_client_1->bank_info, p_client_2->bank_info) != 0 ||
           strcmp(p_client_1->credit_info->name, p_client_2->credit_info->name) != 0 ||
           p_client_1->credit_info->acc_no != p_client_2->credit_info->acc_no ||
           p_client_1->credit_info->cvv != p_client_2->credit_info->cvv ||
           p_client_1->credit_info->expiry_date.dd != p_client_2->credit_info->expiry_date.dd ||
           p_client_1->credit_info->expiry_date.mm != p_client_2->credit_info->expiry_date.mm ||
           p_client_1->credit_info->expiry_date.yy != p_client_2->credit_info->expiry_date.yy)
        {
            return FAILURE;
        }
        return SUCCESS;
    }
    return FAILURE;
}