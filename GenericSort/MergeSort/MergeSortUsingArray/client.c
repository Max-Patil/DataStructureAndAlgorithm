//HEADER FILES
#include<stdio.h>
#include<stdlib.h>
#include"merge_sort.h"

//TYPE DEFINITIONS
typedef struct job job_t;

//STRUCTURE TYPE DEFINITION FOR JOB SCHEDULING PROBLEM
struct job
{
    char id;        
    int deadline;
    int profit;
};

static status_t compareProfitsInIncreasingOrder(data_t job1, data_t job2)
{
    return ((job_t*)job1)->profit < ((job_t*)job2)->profit;
}

static status_t compareProfitsInDecreasingOrder(data_t job1, data_t job2)
{
     return ((job_t*)job1)->profit > ((job_t*)job2)->profit;
}

//Below function accept array of object 
void show_all_jobs(job_t* jobs, size_t nr_jobs, const char* msg)
{
    if(msg) puts(msg);
    for(size_t i = 0; i < nr_jobs; i++)
        printf("[ id : %c | deadline : %d | profit : %d ]\n", jobs[i].id, jobs[i].deadline, jobs[i].profit);
    printf("\n");
}

//ENTRY POINT OF ALL FUNCTIONS
int main(void)
{

    job_t jobs[] = { 
                     { 'a', 2, 20 },
                     { 'b', 2, 15 },
                     { 'c', 1, 1 },
                     { 'd', 3, 5 },
                     { 'e', 3, 10 },
                   };
                   
    show_all_jobs(jobs, 5, "Before sorting by profits in increasing order: ");
    merge_sort((arr_t)jobs, 5, sizeof(job_t), compareProfitsInIncreasingOrder);
    show_all_jobs(jobs, 5, "After sorting by profits in increasing order: ");

    show_all_jobs(jobs, 5, "Before sorting by profits in decreasing order : ");
    merge_sort((arr_t)jobs, 5, sizeof(job_t), compareProfitsInDecreasingOrder);
    show_all_jobs(jobs, 5, "After sorting by profits in decreasing order : ");
    return 0;
}