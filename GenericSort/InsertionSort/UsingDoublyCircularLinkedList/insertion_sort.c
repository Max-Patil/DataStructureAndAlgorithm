// HEADER FILES
#include <stdio.h>
#include <stdlib.h>
#include "dcll.h"
#include "insertion_sort.h"

/**
 * @func_name: to_prev_of
 * @summary: This function will simply return previous node of the node provided
 * @params: Node
 * @returns: Node whose previous node is to be returned
 */
extern inline p_node_t to_prev_of(p_node_t p_node);

/**
* @func_name: attach_node
* @summary: This function will simply attach, the already deattached node
* @params: 1) List
           2) Deattached node
           3) Node before which deattached node to be inserted
* @returns: Status of node to be attached in the list
*/
extern status_t attach_node(p_dcll_t p_list, p_node_t p_deattached, p_node_t p_node_prev);

/**
* @func_name: deattach_node
* @summary: This function will simply deattach node
* @params: 1) List
           2) Node to be deattached
* @returns: deattached node
*/
extern p_node_t deattach_node(p_dcll_t p_list, p_node_t p_node);

/**
* @function: insertion_sort
* @summary: This function will sort the data according to sorting condition provided
* @params: 1) List
           2) Size
           3) Compare callback function
* @returns: Status of sorted operation
*/
extern status_t insertion_sort(p_dcll_t p_list, size_t size, compareDataProc p_compare_func)
{
    // As we have used linked list, there is no need to check whether list contain one element or not, to said sorted
    p_node_t p_run = p_list->p_head->p_next->p_next;
    p_node_t p_run_next = NULL, p_run_prev = p_run->p_prev;
    p_node_t p_deattached = NULL;
    size_t i = 0;

    // loop(first_node to last node)
    while (p_run != p_list->p_head)
    {
        p_run_next = p_run->p_next;

        // key = p_run
        p_node_t key = p_run;

        // j(p_run_prev) = i - 1(p_run->p_prev)
        // Note : p_run_prev is the jth node
        p_run_prev = p_run->p_prev; 
        
        p_deattached = p_run;

        // loop( j(p_run_prev) != -1(p_list->p_head) && key(p_run->data) < p_run_prev->data )
        while (p_run_prev != p_list->p_head && SUCCESS == p_compare_func(dcll_get_data(p_run_prev), dcll_get_data(key)))
        {
            // Deattached node only once and that is also only when sorting condition satisfies
            if (p_deattached->p_prev != p_deattached->p_next)
                p_deattached = deattach_node(p_list, p_run);

            // j(p_run_prev) = j - 1(p_run_prev->p_prev)
            p_run_prev = p_run_prev->p_prev;
        }

        //p_deattached->p_next == NULL && p_deattached->p_prev == NULL
        if (p_deattached->p_next == p_deattached->p_prev)
            attach_node(p_list, p_deattached, p_run_prev);

        // Note : p_run is the ith node
        p_run = p_run_next;
    }

    return SUCCESS;
}