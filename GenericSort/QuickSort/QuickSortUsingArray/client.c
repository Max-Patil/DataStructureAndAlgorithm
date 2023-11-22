// HEADER FILES
#include <stdio.h>
#include <stdlib.h>
#include "quick_sort.h"

// TYPE DEFINITIONS
typedef struct Item item_t;

int knapsack_capacity = 45;

// STRUCTURE TYPE DEFINTION OF ITEM FOR KNAPSACK PROBLEM
struct Item
{
    char item_id;
    int profit;
    int weight;
};

// Displaying the data
void print_item_details(item_t *p_items, size_t nr_items, const char *msg)
{
    if (msg)
        puts(msg);

    printf("capacity : %d\n", knapsack_capacity);

    for (size_t idx = 0; idx < nr_items; idx++)
        printf("[ item_id : %c | profit : %3d | weight : %2d | ratio : %6.2lf ]\n", p_items[idx].item_id,
               p_items[idx].profit,
               p_items[idx].weight,
               (double)(p_items[idx].profit / p_items[idx].weight));
}

static status_t compareProfitByWeightRatio(data_t item1, data_t item2)
{
    return (double)(((item_t *)item1)->profit / ((item_t*)item1)->weight) >  (double)(((item_t *)item2)->profit / ((item_t*)item2)->weight);
}

// ENTRY POINT OF ALL FUNCTIONS
int main(void)
{
    srand(time(0));
    size_t const nr_items = 26;
    item_t *p_items = (item_t *)malloc(sizeof(item_t) * nr_items);

    // Reading the data
    for (int idx = 0; idx < nr_items; idx++)
    {

        p_items[idx].item_id = (97 + idx);
        p_items[idx].profit = 1 + rand() % 500;
        p_items[idx].weight = 1 + rand() % 69;
    }

   print_item_details(p_items, nr_items, "Before Sorting Profit by weight ratio : ");
   quick_sort((arr_t)p_items, 0, nr_items - 1,  sizeof(item_t), compareProfitByWeightRatio);
   print_item_details(p_items, nr_items, "After Sorting Profit by weight ratio :  ");

    if (p_items)
        free(p_items); p_items = NULL;

    return 0;
}