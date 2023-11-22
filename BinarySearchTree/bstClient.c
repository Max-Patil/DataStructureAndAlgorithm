//HEADER FILES
#include<stdio.h>
#include<assert.h>
#include"BST.h"

//ENTRY POINT OF ALL FUNCTIONS
int main(void) {

	bst_t* p_bst = NULL;
	create_bst(&p_bst);

	int bst_data[] = { 100, 50, 150, 25, 75, 65, 125, 175, 30 };
	size_t size = sizeof(bst_data) / sizeof(bst_data[0]);
	
	for (int i = 0; i < size; i++) {
		assert(insert_bst_node(p_bst, bst_data[i]) == SUCCESS);
	}

	puts("PreOrder Traversal Recursive :");
	preorder_rc(p_bst);

	puts("PostOrder Traversal Recursive :");
	postorder_rc(p_bst);

	puts("InOrder Traversal Recursive :");
	inorder_rc(p_bst);

	puts("PreOrder Traversal Non-Recursive :");
	preorder_nrc(p_bst);

	puts("InOrder Traversal Non-Recursive :");
	inorder_nrc(p_bst);

	puts("PostOrder Traversal Non-Recursive :");
	postorder_nrc(p_bst);

	data_t data = 0;
	assert(minimum_bst(p_bst, &data) == SUCCESS);
	printf("\nMinimum element in BST : %d\n", data);

	assert(maximum_bst(p_bst, &data) == SUCCESS);
	printf("Maximum element in BST : %d\n", data);

	puts("\nBreadth First Traversal of BST :");
	breadth_first_traversal(p_bst);

	for (int i = 0; i < size; i++) {
		//printf("%d\n", bst_data[i]);
		assert(remove_bst(p_bst, bst_data[i]) == SUCCESS);
	}
	
	assert(p_bst->p_root_node == NULL);
	//Destroying Binary Search Tree

	destroy_bst_rc(&p_bst);
	return 0;
}