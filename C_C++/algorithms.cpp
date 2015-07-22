#include "sort_algos_core.h"
#include "algos_apps.h"
#include <random>
#include <chrono>
#include <fstream>
  
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
    
 
template<typename T>
std::vector<T> load_file(const char* fileName) {
  std::vector<T> data;

  std::ifstream inFile( fileName );

  T dataElem;

  while( inFile >> dataElem )
    data.push_back(dataElem);

  return data;

}


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
    }*/
 
    /// Checking for valid inputs


    unsigned int sampleSize = (unsigned int) atoi(argc[3]);
    int lowerLimit          = atoi(argc[1]);
    int upperLimit          = atoi(argc[2]);

    /// Initilalize uniform random distribution and array
    std::default_random_engine generator
    ( std::chrono::system_clock::now().time_since_epoch().count() );
    std::uniform_int_distribution< int > U( lowerLimit, upperLimit );
      
    std::vector< unsigned int > array( sampleSize, 0 );
 
    /// Fill data 
    for( unsigned int i = 0 ; i < sampleSize; i++)
        array[i] = U( generator );
    

    //std::vector<unsigned int> array = load_file<unsigned int>(argc[1]);

    // show initial array
    std::cout << "Initial array: "
              << array 
              << std::endl;

    std::cout << "Size of the array: " << array.size() << std::endl;

    //    algos::sorting_algos::merge_sort::sort(array);
    algos::sorting_algos::quick_sort::sort(array);

    // show initial array
    std::cout << "Final array: "
              << array 
              << std::endl;
    /*
    std::cout << "The number of inversions is: "
              << algos::apps::count_inversions<unsigned int>(array) 
    	      << std::endl;
    */
    return EXIT_SUCCESS;
}
