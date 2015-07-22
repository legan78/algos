#ifndef SORT_ALGOS_IMP_H
#define SORT_ALGOS_IMP_H

#include "sort_algos.h"

namespace algos{
  namespace sorting_algos {


     template< typename T>
     void recursive_insertion_sort::sort( std::vector< T >&array, int order )throw() {
     }

     template< typename T>
     void recursive_insertion_sort::recursive_call(std::vector< T >&array, unsigned int size) {
     }

    
    /**
     * Insertion sort implementation
     */
     template < typename T >
     void insertion_sort( std::vector< T >& array, int order )throw(){
        /// Validate the order to sort
        if( order != SORT_ALGOS_INCREASING && order != SORT_ALGOS_DECREASING ){
          throw order_error();
        }

        /// Select the order
        compare< T >::comp_func = ( order== SORT_ALGOS_INCREASING)? &less< T >: &greater< T >;

        for( unsigned int i = 1; i < array.size(); i++) {
           /// The elements in array up to i-1 are in sorted order
           int j = int(i) - 1;
           T a = array[i];

           /// Move from right to left and compare and swap
           while( j >= 0 &&  compare< T >::comp_func( a, array[j] ) ) //a < array[j])
             /// Here is where we do the shifting to the right
           array[j+1] = array[j--];

             /// At the end of each iteration, a has the corresponding element in i
           array[j+1] = a;
        }
     }
     
     /**
      * @brief Call this method through the scope to sort the 
      * numbers in array using the merge sort algorithm
      */
     template< typename T >
     void merge_sort::sort( std::vector<T>& array, int order)throw(){
       /// Validate the order to sort
       if( order != SORT_ALGOS_INCREASING && order != SORT_ALGOS_DECREASING ) throw order_error();
         // Nothing to sort
         if(!array.size() || array.size() == 1) return;
         /// Select the order
         if( order == SORT_ALGOS_INCREASING) {
           compare< T >::comp_func  = &lesseq< T >;
	       compare< T >::limit_func = &std::numeric_limits< T >::max;
         } else {
           compare< T >::comp_func  = &greatereq< T >;
           compare< T >::limit_func = &std::numeric_limits< T >::min;
         }
      
         /// Sort the array base case
         divide_conquer< T >(array, 0, array.size()); 
     }
     
     template< typename T >
     void merge_sort::divide_conquer( std::vector<T>& array,    // The array of numbers
                                 unsigned int begin,     // The initial point in the array
                                 unsigned int end){      // The ending point in the array
 	   /// We have reached the base case
       unsigned int currentArraySize = end - begin;
       if( currentArraySize == 1) return;
	   /// Find the middle point of the continous array
	   unsigned int middle = begin + ( end - begin ) / 2;

#ifdef DEBUG_VERSION
       std::cout << std::vector< T >( array.begin() + begin, array.begin() + end ) 
                 << std::endl;
       std::cout << "Begin: " 
                 << begin 
                 << " middle: " 
                 << middle << " end:  " 
                 << end 
                 << std::endl;
       std::cout << "First part " 
                 << std::vector< T >( array.begin() + begin, array.begin()+ middle) 
                 << std::endl;
       std::cout << "Second part "
	             << std::vector< T >(array.begin() + middle, array.begin() + end)
	             << std::endl;
#endif
       /// First recursive call of first half
       divide_conquer< T >(array, begin, middle);
       /// Second recursive call of second half	
       divide_conquer< T >(array, middle, end);
       // The mergin step that runs in O(n) where n = end - begin
       merge< T >( array, begin, middle, end);

#ifdef DEBUG_VERSION	
       std::cout << " Ordered array: " 
                 << std::vector< T >( array.begin()+ begin, array.begin() + end) 
                 << std::endl;
       getchar();
#endif
    }
       /**
        * @brief Implementation of the merging step
        */
     template< typename T >
     void merge_sort::merge( std::vector<T>& array, unsigned int begin, 
                        unsigned int middle, unsigned int end ){
       /// Implement the merging step inplace
       unsigned int leftSize  = middle - begin;
       unsigned int rightSize = end - middle;
#ifdef DEBUG_VERSION       
       std::cout << "Left size: " 
	             << leftSize
                 << " right size: "
                 << rightSize 
                 << std::endl;
#endif
       std::vector< T > left( leftSize );
       std::vector< T > right( rightSize );
       /// Copy data to not override
       for( unsigned int i = 0, j = begin; i < leftSize; i++, j++ ) left[i] = array[j];
       for( unsigned int i = 0, j = middle; i< rightSize; i++, j++) right[i] = array[j];
       
       
       /// Using sentinels to keep a sorted array, a +1 has to be added 
       /// in the declaration of such arrays
       /// left[leftSize] = sorting_algos::compare< T >::limit_func();
       /// right[rightSize] = sorting_algos::compare< T >::limit_func();
       /// do merge
       unsigned int i = 0, j = 0, k = begin;

       for( k = begin; k < end && i < leftSize && j < rightSize; k++){
         if( compare< T >::comp_func(left[i],  right[j] )) array[k] = left[i++];
         else array[k] = right[j++];
       }

       /// copy of the arrays back into the original array
       for( ; i < leftSize  && k < end; i++, k++) array[k] = left[i];
       
       for( ; j < rightSize && k < end; j++, k++) array[k] = right[j];
    }     

     template<typename T>
     void quick_sort::sort(std::vector<T>& array, int order ) {
     
       unsigned int l = 0;
       unsigned int r = array.size();

       sort_array(array, l, r);
     }

     template<typename T>
       void quick_sort::sort_array(std::vector<T>& _array
				   , unsigned int l
				   , unsigned int r) {
       if( l == r) return;

#ifdef DEBUG_VERSION
       std::cout << "Partitioned array 1: "
		 << std::vector<unsigned int>(_array.begin()+l, _array.begin()+r) 
		 << std::endl;
#endif

       unsigned int p = get_pivot(_array, l, r);
       p = pivot_partition(_array, p, l, r);

#ifdef DEBUG_VERSION
       std::cout << "Partitioned array 2: "
		 << std::vector<unsigned int>(_array.begin()+l, _array.begin()+r) 
		 << std::endl;
#endif

       if(l != p) sort_array(_array, l, p-1);
       if(p != r) sort_array(_array, p, r);
     }
       
     template<typename T>
       unsigned int quick_sort::pivot_partition(std::vector<T>& _array
						, unsigned int p
						, unsigned int l
						, unsigned int r ) {
       unsigned int i = l+1;

       for(unsigned int j=l+1; j<r; j++) {

#ifdef DEBUG_VERSION
	 std::cout << "Comparing "
		   << _array[p]
		   << " : " 
		   << _array[j];
#endif

	 if(_array[p] > _array[j]) {
	   std::swap<T>(_array[j], _array[i]);			
	   i++;

#ifdef DEBUG_VERSION
	   std::cout << " Swaping! ";
#endif

	 }

#ifdef DEBUG_VERSION
	 std::cout << std::endl;
	 getchar();
#endif

       }

       std::swap(_array[p], _array[i-1]);
       
       return i;
     }

     template<typename T>
     unsigned int quick_sort::get_pivot(std::vector<T>& _array
					,unsigned int l
					,unsigned int r) {
       

       //       std::swap(_array[l], _array[r-1]);
       return l;
     }

  }
}

#endif

