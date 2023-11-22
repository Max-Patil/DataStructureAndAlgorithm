//HEADER FILES
#include<stdio.h>
#include<stdlib.h>
#include"two_stacks.h"

//CALLBACK FUNCTION DEFINITIONS
void printLongData(data_t data)
{
    printf("[\t%3llu\t]\n", (long long)data);
}

void deleteLongData(data_t data)
{
    printf("Deleting : [\t%3llu\t]\n", (long long)data);
}

//ENTRY POINT OF ALL FUNCTIONS
int main(void)
{

    p_arr_t p_long_array = create_two_stack_arr(15);

    for(int elem = 10; elem <= 60; elem += 10)
        stack1_push(p_long_array, (data_t) elem);

    arr_print(p_long_array, printLongData, "\nAfter Pushing data in Stack 1 : ");

    for(int elem = 10; elem <= 60; elem += 10)
        stack2_push(p_long_array, (data_t) elem);

    arr_print(p_long_array, printLongData, "\nAfter Pushing data in Stack 2 : ");

    puts("Popping Data from Stack 1 : ");
    printLongData(stack1_pop(p_long_array));
    printLongData(stack1_pop(p_long_array));
    printLongData(stack1_pop(p_long_array));
    
    arr_print(p_long_array, printLongData, "\nAfter Popping data from Stack 1 : ");

    puts("Popping Data from Stack 2 : ");
    printLongData(stack2_pop(p_long_array));
    printLongData(stack2_pop(p_long_array));
    printLongData(stack2_pop(p_long_array));
    printLongData(stack2_pop(p_long_array));
    arr_print(p_long_array, printLongData, "\nAfter Popping data from Stack 2 : ");

    destroy_two_stack_arr(&p_long_array, deleteLongData);

    return EXIT_SUCCESS;
}