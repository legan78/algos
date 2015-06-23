#include "sort_algos_core.h"
#include "algos_apps.h"
#include <random>
#include <chrono>
 
  
/// gilzhaiek@hotmail.com
 
/// #define DEBUG_VERSION


void help(){
    std::cout << "Input format: ./app < range_lower_limit > < range_upper_limit > < sample_size > < order >"
              << std::endl;
}


 /**
 * @brief Vector output format 
=======
/*
 * Definition of the pointers
 */
template<>  
bool (*algos::sorting_algos::compare< unsigned int >::comp_func)(unsigned int, unsigned int) = NULL;

template<>
unsigned int (*algos::sorting_algos::compare< unsigned int >::limit_func)() = &std::numeric_limits< unsigned int >::max;
    
 
int main(int argv, char** argc){
    /* 
     * This is why I learned from the secure code programming: Never trust user input.
     * Basically is to search for the good input format and reject everything else
     */
    /// Check if we have the good number of input arguments
    /*if( argv != 5 ){ 
        std::cerr << "Ivalid numer of input arguments" << std::endl;
        help();
        exit(-1);
    }
 
    /// Checking for valid inputse

    if( atoi( argc[3] ) <= 0 || 
        (unsigned int) atoi( argc[3] ) >= std::numeric_limits< unsigned int >::max() ||
        atoi( argc[2] ) <= atoi( argc[1] ) ){
        std::cerr << "Invalid input arguments " << std::endl;
        exit(-1);
    }
      
    /// Retrive input arguments
    unsigned int sampleSize = (unsigned int) atoi(argc[3]);
    int lowerLimit          = atoi(argc[1]);
    int upperLimit          = atoi(argc[2]);
    int order               = atoi(argc[4]);
 
    /// Initilalize uniform random distribution and array
    std::default_random_engine generator
    ( std::chrono::system_clock::now().time_since_epoch().count() );
    std::uniform_int_distribution< int > U( lowerLimit, upperLimit );
      
    std::vector< int > array( sampleSize, 0 );
  

    /// Fill data 
    for( unsigned int i = 0 ; i < sampleSize; i++)
        array[i] = U( generator );
    

    // show initial array
    std::cout << "Initial array: "
              << array 
              << std::endl;
 
 
    /// Sort and show results
    //    sorting_algos::insertion_sort< int >( array, order );
    algos::sorting_algos::merge_sort::sort< int >( array, order );

    std::cout << array << std::endl;*/
    unsigned int low, high;
    std::vector<unsigned int> list = {100,113,110,85,105,102,86,63,81,101,94,106,101,79,94,90,97};
    std::cout << "The list:" << list << std::endl;
    std::cout << "The number of inversions is: " << algos::apps::max_sub_array<unsigned int>(list,low,high) << std::endl;
    
  
    return EXIT_SUCCESS;
}
