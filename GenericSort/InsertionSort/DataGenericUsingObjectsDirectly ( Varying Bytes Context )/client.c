//HEADER FILES
#include<stdio.h>
#include<stdlib.h>
#include"insertion_sort.h"

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
    return ((job_t*)job1)->profit > ((job_t*)job2)->profit;
}

static status_t compareProfitsInDecreasingOrder(data_t job1, data_t job2)
{
     return ((job_t*)job1)->profit < ((job_t*)job2)->profit;
}

//Below function accept array of object 
static void show_all_jobs_1(job_t* jobs, size_t nr_jobs, const char* msg)
{
    if(msg) puts(msg);
    for(size_t i = 0; i < nr_jobs; i++)
        printf("[ id : %c | deadline : %d | profit : %d ]\n", jobs[i].id, jobs[i].deadline, jobs[i].profit);
    printf("\n");
}

//Below function accept pointer to array of pointer to object
static void show_all_jobs_2(job_t** p_jobs, size_t nr_jobs, const char* msg)
{
    if(msg) puts(msg);
    for(size_t i = 0; i < nr_jobs; i++)
        printf("[ id : %c | deadline : %d | profit : %d ]\n", p_jobs[i]->id, p_jobs[i]->deadline, p_jobs[i]->profit);
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
    
    job_t* pJobs[] = { &jobs[0], &jobs[1], &jobs[2], &jobs[3], &jobs[4] };

    show_all_jobs_1(jobs, 5, "Before sorting by profits in increasing order: ");
    insertion_sort((arr_t)jobs, 5, sizeof(job_t), compareProfitsInIncreasingOrder);
    show_all_jobs_1(jobs, 5, "After sorting by profits in increasing order: ");

    // show_all_jobs_2(pJobs, 5, "Before sorting by profits in decreasing order : ");
    // insertion_sort((arr_t)pJobs, 5, sizeof(void*), compareProfitsInDecreasingOrder);
    // show_all_jobs_2(pJobs, 5, "After sorting by profits in decreasing order : ");

    return 0;

}