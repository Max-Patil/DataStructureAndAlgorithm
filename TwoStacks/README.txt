Explaination : Two stack in a array
	
	1) Two stack can be visualize from both ends of a single array whose size is fixed, once created

	2) Two stack of opposite end, also had head against each other in such a way that they move towards
	each other while pushing data on stacks

	Note : overflow condition will meet only when we try pushing more data than total number of elements 
        in array and both stacks are dynamically growing and therefore size of stack can vary even though 
	size of array is fixed

	3) Underflow condition of two stacks will occur respective to ends of an array and therefore popping
	element from array as if we are popping element from stack, will come to halt after reaching to end point
	of array that needed to checked

	Note 1: ERROR CHECKING is crucial part of handling errors especially in case of underflow and overflow

	Note 2: In this 'two stacks', head is always pointing to that container of array that has no data and therefore
		it is need to handle properly in push and pop methods of 'two_stacks'
