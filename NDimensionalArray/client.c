//HEADER FILES
#include <stdio.h>
#include <stdlib.h>
#include "nd_array.h"

//SYMBOLIC CONSTANT
#define		NO_OF_ELEMENTS	4
#define		NO_OF_1D_ARRAY	3
#define		NO_OF_2D_ARRAY	3
#define		NO_OF_3D_ARRAY  4
#define		NO_OF_4D_ARRAY  2
#define		NO_OF_5D_ARRAY	3
#define		NO_OF_6D_ARRAY	3
#define		NO_OF_7D_ARRAY	2
#define		NO_OF_8D_ARRAY	6
#define		NO_OF_9D_ARRAY	1
#define		NO_OF_10D_ARRAY	3
#define     NO_OF_11D_ARRAY 2

p_nd_array_t p_8d_array = NULL;
p_nd_array_t p_12d_array = NULL;

//ENTRY POINT OF ALL FUNCTIONS
int main(void)
{
	/**********************************************************************************************************\
											8 Dimension Array
	\**********************************************************************************************************/

	p_8d_array = create_nd_array(	8,
								NO_OF_7D_ARRAY,
								NO_OF_6D_ARRAY,
								NO_OF_5D_ARRAY,
								NO_OF_4D_ARRAY,
								NO_OF_3D_ARRAY,
								NO_OF_2D_ARRAY,
								NO_OF_1D_ARRAY,
								NO_OF_ELEMENTS);

	//Writing or setting values of NDimensional array indices
	size_t count = 0;
	for( size_t index1 = 0; index1 < nd_array_dimension_size(p_8d_array, 0); ++index1 )
		for( size_t index2 = 0; index2 < nd_array_dimension_size(p_8d_array, 1); ++index2 )
			for( size_t index3 = 0; index3 < nd_array_dimension_size(p_8d_array, 2); ++index3)
				for( size_t index4 = 0; index4 < nd_array_dimension_size(p_8d_array, 3); ++index4)
					for( size_t index5 = 0; index5 < nd_array_dimension_size(p_8d_array, 4); ++index5)
						for( size_t index6 = 0; index6 < nd_array_dimension_size(p_8d_array, 5); ++index6)
							for( size_t index7 = 0; index7 <nd_array_dimension_size (p_8d_array, 6); ++index7)
								for( size_t index8 = 0; index8 < nd_array_dimension_size(p_8d_array, 7); ++index8)
									set_nd_array_data(p_8d_array, 
													(data_t)(++count), 
													index1,  index2, 
													index3, index4, 
													index5, index6, 
													index7, index8);
	print_nd_array(p_8d_array);

	destroy_nd_array( &p_8d_array );
  

    /**********************************************************************************************************\
											12 Dimension Array
	\**********************************************************************************************************/
    // p_12d_array =           create_nd_array(12, 
	// 						NO_OF_11D_ARRAY, NO_OF_10D_ARRAY,
	// 						NO_OF_9D_ARRAY, NO_OF_8D_ARRAY,
	// 						NO_OF_7D_ARRAY, NO_OF_6D_ARRAY,
	// 						NO_OF_5D_ARRAY, NO_OF_4D_ARRAY,
	// 						NO_OF_3D_ARRAY, NO_OF_2D_ARRAY,
	// 						NO_OF_1D_ARRAY, NO_OF_ELEMENTS);

    // 	for( size_t index1 = 0; index1 < nd_array_dimension_size(p_12d_array, 0); ++index1 )
	// 	    for( size_t index2 = 0; index2 < nd_array_dimension_size(p_12d_array, 1); ++index2 )
	// 		    for( size_t index3 = 0; index3 < nd_array_dimension_size(p_12d_array, 2); ++index3)
	// 			    for( size_t index4 = 0; index4 < nd_array_dimension_size(p_12d_array, 3); ++index4)
	// 				    for( size_t index5 = 0; index5 < nd_array_dimension_size(p_12d_array, 4); ++index5)
	// 					    for( size_t index6 = 0; index6 < nd_array_dimension_size(p_12d_array, 5); ++index6)
	// 						    for( size_t index7 = 0; index7 < nd_array_dimension_size(p_12d_array, 6); ++index7)
	// 							    for( size_t index8 = 0; index8 < nd_array_dimension_size(p_12d_array, 7); ++index8)
	// 								    for( size_t index9 = 0; index9 < nd_array_dimension_size(p_12d_array, 8); ++index9)
	// 									    for( size_t index10 = 0; index10 < nd_array_dimension_size(p_12d_array, 9); ++index10)
	// 										    for( size_t index11 = 0; index11 < nd_array_dimension_size (p_12d_array, 10); ++index11)
	// 											    for( size_t index12 = 0; index12 < nd_array_dimension_size(p_12d_array, 11); ++index12)
	// 												    set_nd_array_data(	p_12d_array, 
	// 																(data_t)(++count), 
	// 																index1,  index2, 
	// 																index3, index4, 
	// 																index5, index6, 
	// 																index7, index8, 
	// 																index9, index10, 
	// 																index11, index12);
    
    //     print_nd_array(p_12d_array);

    //     destroy_nd_array(&p_12d_array);
}