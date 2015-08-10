#include "sort_algos_core.h"
#include "algos_apps.h"
#include "graph_algos.h"
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


int main(int argc, char** argv){


  /*
  algos::sorting_algos::quick_sort::pivotType = atoi(argc[2]);
  std::vector<unsigned int> array = load_file<unsigned int>(argc[1]);
  //   std::vector<unsigned int> array = {3,7,1,2,4,5,6};//{4,2,6,3,7,1,5};

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

    std::cout << "N. of comparisons: " 
	      << algos::sorting_algos::quick_sort::compCount
	      << std::endl;
  */

  algos::graph_algos::AdjacencyList AdList =  algos::graph_algos::AdjacencyList::load_from_file(argv[1]);

  //  std::cout << algos::graph_algos::sampling_no_replace(200,200 ) 
  //	    << std::endl;

  algos::graph_algos::AdjacencyList::MinCutTraits traits = AdList.compute_min_cut(atoi(argv[2]));

  std::cout << " Min cut value: " 
	    << traits.first
	    << std::endl;

  std::cout << " The cut is: [";
 
  for (unsigned int i = 0; i < traits.second.size(); i++) 
    std::cout << traits.second[i] 
	      << ", ";
  
  std::cout << "]"
	    << std::endl;




  return EXIT_SUCCESS;
}
