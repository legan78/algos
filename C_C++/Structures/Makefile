
# Compiler
CXX=g++

# C++11 Flag + debug
CFLAGS=-std=c++0x -g

OPENCV_LIBS=-lopencv_core \
			-lopencv_highgui \
			-lopencv_imgproc \
			-lopencv_imgcodecs



#all:	extractFilterResponse

main:	main.o
	$(CXX) $(CFLAGS) main.o $(OPENCV_LIBS) -o main.out

main.o: main.cpp BinarySearchTree_core.h BinarySearchTree.h
	$(CXX) $(CFLAGS) -c main.cpp

clean:
	rm *.o 
