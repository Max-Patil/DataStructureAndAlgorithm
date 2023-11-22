/**
* @file : mv_client
* @author : MaxPatil (patilmitesh8746@gmail.com)

* @brief : This source file contains merge sort test i.e tested by
client programmer

* @date : 19-12-2022
* @version : 1.0
* @copyright   Copyrights (c) 2022
*/

//HEADER FILES
#include<stdio.h>
#include<stdlib.h>
#include"mv_dcll.h"
#include"mv_merge_sort.h"

//TYPE DEFINITIONS
typedef struct job job_t;

//STRUCTURE TYPE DEFINITION FOR JOB SCHEDULING PROBLEM
struct job
{
    char id;        
    int deadline;
    int profit;
};

static status_t compareProfits(data_t job1, data_t job2)
{
    return ((job_t*)job1)->profit > ((job_t*)job2)->profit;
}

static void display_job(data_t job)
{
    printf("[ id : %c | deadline : %d | profit : %d ]\n", ((job_t*)job)->id, ((job_t*)job)->deadline, ((job_t*)job)->profit);
}

static void delete_job(data_t job)
{
    printf("Deleting -> [ id : %c | deadline : %d | profit : %d ]\n", ((job_t*)job)->id, ((job_t*)job)->deadline, ((job_t*)job)->profit);
}


//ENTRY POINT OF ALL FUNCTIONS
int main(void)
{

     job_t jobs[] = {
                         {'a', 2, 10},
                         {'b', 2, 15},
                         {'c', 1, 20},
                         {'d', 3, 1 },
                         {'e', 3, 5 },
                    }; 

    p_mv_dcll_t jobs_list = create_mv_dcll_list();

    for(size_t i = 0; i < 5; i++)
        mv_dcll_insert_back(jobs_list, (data_t)(&jobs[i]));

    mv_dcll_show(jobs_list, display_job, "Before sorting jobs by profits : ");
    mv_merge_sort(jobs_list, 5, compareProfits);
    mv_dcll_show(jobs_list, display_job, "After sorting jobs by profit :  ");

    destroy_mv_dcll(&jobs_list, delete_job); jobs_list = NULL;
    return 0;
}