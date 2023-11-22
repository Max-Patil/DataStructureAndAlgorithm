//HEADER FILES
#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include<time.h>
#include"Hash.h"

//ENTRY POINT OF ALL FUNCTIONS
int main(int argc, char* argv[]) {

	size_t bucket_size;
	size_t nr_elements;
	data_t* parr = NULL;
	srand(time(0));

	if (argc != 3)
	{
		fprintf(stderr, "Usage Error %s bucket size i.e nr_elements\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	bucket_size = atoi(argv[1]);
	nr_elements = atoi(argv[2]);

	parr = (data_t*)malloc(nr_elements * sizeof(data_t));
	for (int i = 0; i < nr_elements; i++)
		parr[i] = rand() % nr_elements;

	hashq_t* phashq = NULL;
	create_hashq(&phashq, bucket_size);

	//Adding element to hashqueue
	for (int i = 0; i < nr_elements; i++)
		add_hashq_data(phashq, parr[i]);

	show_hashq(phashq, "Display Hash Queue");

	/* NOTE: TOTAL NUMBER OF ELEMENT ACROSS EVERY BUCKET SHOULD BE DISTINCT BUT IN OUR CASE, BECAUSE OF 
	RANDOM FUNCTION WHICH GENERATED SAME NUMBER, IT IS NOT INSERT IN OUR HASH QUEUE*/

	/* RANDOM FUNCTION HAS GENERATED REPEATED NUMBER SUCH THAT WHEN THEY ADDED IN HASHQUEUE, THEY ARE IGNORE AND 
	AS A RESULT, TOTAL NUMBER OF ELEMENTS  ACROSS SLOTS(BUCKETS) ARE NOT REFLECTED */

	/*TRY TO GENERATE RANDOM NUMBER WITH EQUAL PROBABILITY OR WITH EQUAL DISTRIBUTION */
	show_distribution(phashq); 

	//Searching random element in hashqueue
	puts("\nSearch Data : Random cases");
	for (int i = 0; i < 5; i++) {
		data_t data = rand() % nr_elements;
		status_t s = search_hashq_data(phashq, data);
		if (s == TRUE)
			printf("data : %d found\n", data);
		else
			printf("data : %d not found\n", data);
	}

	//Removing elements from hashqueue
	puts("\nRemoving Data from Hash queue : ");
	for (int i = 0; i < 5; i++) {
		data_t data = parr[rand() % nr_elements];
		printf("data to be removed : %d\n", data);
		remove_hashq_data(phashq, data);
	}show_hashq(phashq, "\nAfter removing elements from hashqueue : ");

	show_distribution(phashq);

	//Destroying hashqueue
	assert(destroy_hashq(&phashq) == SUCCESS && phashq == NULL);
	free(parr); parr = NULL;

	return 0;
}