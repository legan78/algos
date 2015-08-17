#ifndef ALGOS_APPS_H
#define ALGOS_APPS_H


#include "algosTools.h"

//#define DBG_VERSION

namespace algos{
  namespace apps{
  
    template<typename T>
    unsigned int countSplit(std::vector<T>& inputList, unsigned int begin, 
                                                       unsigned int middle, 
                                                       unsigned int end){
       unsigned int inversions = 0;
       unsigned int leftSize = middle - begin;
       unsigned int rightSize = end - middle;
      
       std::vector<T> leftPart(leftSize);
       std::vector<T> rightPart(rightSize);
      
       for(unsigned int i= 0, j=begin; i<leftSize ;i++)leftPart[i] = inputList[j++]; 
       for(unsigned int i= 0, j=middle; i<rightSize ;i++)rightPart[i] = inputList[j++];
       
       
       unsigned int j = 0, i = 0, k;
       
#ifdef DBG_VERSION
       std::cout << "Left part:" << leftPart << std::endl;
       std::cout << "Right part:" << rightPart << std::endl;       
#endif

       for(k = begin; k < end && i < leftSize && j < rightSize ; k++){
          if(leftPart[i] < rightPart[j])
             inputList[k] = leftPart[i++];
          else{
#ifdef DBG_VERSION
             std::cout << "(" << leftPart[i] << "," << rightPart[j] << ")"<< std::endl;
             std::cout << "Reminding: " << leftSize - i << std::endl;
#endif
             inputList[k] = rightPart[j++];
             inversions += leftSize - i;
          }          
       }
       
       for(;i<leftSize && k<end;i++) inputList[k++] = leftPart[i];
       for(;j<rightSize&& k<end;j++) inputList[k++] = rightPart[j];

       return inversions;
    }

    template< typename T>
    unsigned int count(std::vector<T>& inputList, unsigned int begin, 
                                                  unsigned int end ){
       unsigned int currentArraySize = end - begin;
       if(currentArraySize == 1){
#ifdef DBG_VERSION
          std::cout << "Array of size 1" /*<< std::vector<T>(inputList.begin()+begin, inputList.begin() + begin + end)*/ << std::endl;         
#endif
          return 0;
       }

       unsigned int middle = begin + (end - begin)/2;
#ifdef DBG_VERSION
       std::cout << "Input array size: " << end - begin << std::endl;
       std::cout << "First half from " << begin << " To " << middle << /*std::vector<T>(inputList.begin()+begin, inputList.begin() + begin + middle) <<*/ std::endl;
       std::cout << "Second half from " << middle << " To " << end << /*std::vector<T>(inputList.begin()+begin+middle, inputList.begin() + begin + end) <<*/ std::endl; 
#endif
       unsigned int x = count<T>(inputList, begin, middle);
       unsigned int y = count<T>(inputList, middle, end);      
       unsigned int z = countSplit<T>(inputList, begin, middle, end);
#ifdef DBG_VERSION 
       std::cout << x << " " << y << " " << z << std::endl;
#endif
      
       return x+y+z;
    }
  
    template< typename T >
    unsigned int count_inversions(const std::vector<T>& inputList){
       std::vector<T> inputListCpy = inputList;
       unsigned int total =count<T>(inputListCpy, 0, inputListCpy.size());
       
//       std::cout << inputListCpy << std::endl;
       
       return total;
    }

    double get_max_joint_array(const std::vector<double>& array
                              ,unsigned int low
                              ,unsigned int middle
                              ,unsigned int high
                              ,unsigned int& leftPoint
                              ,unsigned int& rightPoint){
       double sumRight = -1e100;
       double sumLeft  = -1e100;
       double sum = 0;

       for(int i = middle; i>=0; i--){
           sum += array[i];
           if(sum > sumLeft){
              sumLeft = sum;
              leftPoint = i;
           }
       }

       sum = 0;
       for(unsigned int i = middle+1; i<high; i++){
          sum += array[i];
          if(sum > sumRight){
             sumRight = sum;
             rightPoint = i;
          }
       }

       return sumLeft + sumRight;
    }    
    
    double get_max_sub_array(const std::vector<double>& array
                            ,unsigned int low
                            ,unsigned int high
                            ,unsigned int& leftPoint
                            ,unsigned int& rightPoint){
        std::cout << high-low << std::endl;
        if(high-low == 0){
           leftPoint = low;
           rightPoint = high;
           return array[low];
        }

        unsigned int middle = low + (high - low)/2;
        
        unsigned int firstLeftPoint, firstRightPoint;
        unsigned int secondLeftPoint, secondRightPoint;
        unsigned int jointLeftPoint, jointRightPoint;
        
#ifdef DBG_VERSION
       std::cout << "Left part:" << std::vector<double>(array.begin()+low, array.begin()+middle+1) << std::endl;
       std::cout << "Right part:" << std::vector<double>(array.begin()+middle+1, array.edn()) << std::endl;       
       getchar();
#endif

        double goodSum;
        double sumLeft  = get_max_sub_array(array, low, middle, firstLeftPoint, firstRightPoint);
        double sumRight = get_max_sub_array(array, middle+1, high, secondLeftPoint, secondRightPoint);
        double sumJoint = get_max_joint_array(array, low, middle, high, jointLeftPoint, jointRightPoint);
        
        if( sumLeft > sumRight && sumLeft > sumJoint){
           goodSum = sumLeft;
           leftPoint = firstLeftPoint;
           rightPoint = firstRightPoint;
        } else if( sumRight > sumLeft && sumRight > sumJoint){
           goodSum = sumRight;
           leftPoint = secondLeftPoint;
           rightPoint = secondRightPoint;
        } else {
           goodSum = sumJoint;
           jointLeftPoint = leftPoint;
           jointRightPoint = rightPoint;
        }
        
        return goodSum;
    }
    
    template<typename T>
    double max_sub_array(const std::vector<T> array
                        ,unsigned int& low
                        ,unsigned int& high)throw(){
        if(!array.size()){
           low = high = 0;
           return 0;
        }
        
        std::vector<double> diff_array(array.size(), 0);
        
        for(unsigned int i=1; i< diff_array.size()-1; i++){
           diff_array[i] = double(array[i]) - double(array[i-1]);
        }

        unsigned int _low = 0;
        unsigned int _high = array.size();
        
        std::cout << "Initial array" << diff_array << std::endl;

        return get_max_sub_array(diff_array, _low, _high, low, high);
    }
  }
}


#endif /*ALGOS_APPS_H*/
