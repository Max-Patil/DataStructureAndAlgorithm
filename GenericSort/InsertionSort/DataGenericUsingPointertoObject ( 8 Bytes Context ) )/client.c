//HEADER FILES
#include<stdio.h>
#include<stdlib.h>
#include "insertion_sort.h"

//CLIENT TYPE DEFINITIONS
typedef struct Job job_t;

//STRUCTURE TYPE DEFINTION FOR JOB SCHEDULING PROBLEM
struct Job
{
    char id;         //char for job id representation
    int deadline;   //work is assumed to be done in one unit of time 
    int profit;
};

static status_t compareProfit(data_t job1, data_t job2)
{
    return (((job_t*)job1)->profit < ((job_t*)job2)->profit);
}

// static job_t* init_job_info(char _id, int _deadline, int _profit)
// {
//     job_t* pJob = malloc(sizeof(job_t));
//     pJob->id = _id; pJob->deadline = _deadline; pJob->profit = _profit;
//     return pJob;
// }

void show_all_jobs(job_t **p_jobs, size_t nr_jobs, const char* msg)
{
    if(msg) puts(msg);

    for(size_t i = 0; i < nr_jobs; i++)
        printf("[job id : %c, deadline : %d, profit : %d]\n", p_jobs[i]->id, p_jobs[i]->deadline, p_jobs[i]->profit );

    printf("\n");
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

    
    job_t* pJobs[] = { &jobs[0], &jobs[1], &jobs[2], &jobs[3], &jobs[4] };

    // job_t* pJobs[] = {
    //         init_job_info('a', 2, 10),
    //         init_job_info('b', 2, 15),
    //         init_job_info('c', 1, 20),
    //         init_job_info('d', 3, 1 ),
    //         init_job_info('e', 3, 5 ),
    // };

    //void** arr != typeof(jobs) -> job_t [] -> jobs_t* != jobs[i] -> object whose size is varies
    //job_t* []  == typeof(arr) -> job_t** -> void** -> pJobs[i] -> job_t* == arr[i] -> data_t* whose size is 8 bytes only

    show_all_jobs(pJobs, 5, "Before sorting jobs by profits : ");
    insertion_sort((arr_t)pJobs, 5, compareProfit);
    show_all_jobs(pJobs, 5, "After sorting jobs by profit :  ");

    return 0;
}