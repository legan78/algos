#ifndef SORT_ALGOS_H
#define SORT_ALGOS_H

#include "algosTools.h"

namespace algos{
  namespace sorting_algos {

    enum{
      SORT_ALGOS_INCREASING = 0,
      SORT_ALGOS_DECREASING
    };
          

    /** 
     * This is a simple implementation of the insertion sort algorithm.
     * This algorithm works as follows: Imagine that you have a hand of cards on
     * the right hand, but you haven't seen if they are in sorted order. The task
     * is then to uncover one card at the time and set onto your left hand in sorted
     * order. This is done by comparing the new card to the ones already in the hand
     * from right to left. For this, we make the asumption that at the begining of
     * each iteration controled by index i, the elements in the array up to
     * i-1 are in sorted order.
     * Use this algorithm only if you have few numbers to sort. < 100 
     * @COMPLEXITIES:
     * @WORST_CASE:   O(n²), i.e. reverse sorted order.
     * @BEST_CASE:    O(n), i.e. in sorted order.
     * @AVERAGE_CASE: O(n²).
     */
     template < typename T >
     void insertion_sort( std::vector< T >& array, int order )throw();
     
     /**
      *
      *
      *
      */
     class recursive_insertion_sort {
   	   public:

 	     template< typename T>
	     static void sort( std::vector< T >&array, int order = SORT_ALGOS_INCREASING)throw();

	   private:
  	     /// This class cannot be instantiated (singleton)
	     recursive_insertion_sort();
         recursive_insertion_sort(const recursive_insertion_sort& );       
         recursive_insertion_sort& operator=(const recursive_insertion_sort& );
         
         template< typename T >
         static void recursive_call(std::vector< T >&array, unsigned int size);
    };

    /**
     * This is a class that implements the merge sort algorithm. This is the most
     * basic example of the divide and conquer paradigm. As the DAC approach
     * algorithms, this divides the input into two chunks until the base case. 
     * Having reached the base case, it merges the two sorted lists into a 
     * soley list with the keys in sorted order. Since by recursively calling
     * the routine with the half of the array at each step, i.e. dividing the
     * current list into two chunks, to reach the base case costs log_{2}(n).
     * The trick is then to compute the merging step in linear time.
     * @COMPLEXITIES:
     * @WORST_CASE:     O( nlog2(n) )
     * @BEST_CASE:      O( nlog2(n) )
     * @AVERAGE_CASE:   O( nlog2(n) )                
     */
     class merge_sort {
       public:
         /**
          * @brief Call this method through the scope to sort the 
          * numbers in array using the merge sort algorithm
          */
         template< typename T >
         static void sort( std::vector<T>& array, int order = SORT_ALGOS_INCREASING )throw();             
       protected:
             
         /// Declare these here just to avoid a unauthorized instantiation
         /// NOTE: This is not a singleton, we only need the sort function
         
         merge_sort();
         merge_sort(const merge_sort& dummy);
         merge_sort& operator=(const merge_sort& dummy);
         /**
          * @brief Implementation of the dividing step of the algorithm
          */
         template< typename T >
         static void divide_conquer( std::vector<T>& array,    // The array of numbers
                                    unsigned int begin,     // The initial point in the array
                                    unsigned int end);      // The ending point in the array

       /**
        * @brief Implementation of the merging step
        */
       template< typename T >
       static void merge( std::vector<T>& array, unsigned int begin, 
                          unsigned int middle, unsigned int end );
    };

     class quick_sort {
       public:
       template<typename T>
	 static void sort(std::vector<T>& _array, int order = SORT_ALGOS_INCREASING);
       
       static unsigned int compCount;

       static int pivotType;

       protected:

       quick_sort();
       quick_sort(const quick_sort& other);
       quick_sort& operator=(const quick_sort& other);

       template<typename T>
	 static void sort_array(std::vector<T>& _array
				, unsigned int l
				, unsigned int r);
       
       template<typename T>
	 static unsigned int pivot_partition(std::vector<T>& _array
					     , unsigned int p
					     , unsigned int l
					     , unsigned int r );

       template<typename T>
	 static unsigned int get_pivot(std::vector<T>& _array
				       ,unsigned int l
				       ,unsigned int r);

       template<typename T>
	 static unsigned int median_of_three(const std::vector<T>& _array
					     ,unsigned int l
					     ,unsigned int r);

     };

  }
}
#endif
