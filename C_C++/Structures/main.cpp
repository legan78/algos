
#include "BinarySearchTree_core.h"
#include <iostream>
#include <random>
#include <chrono>


int main(int argc, char** argv) {
	std::default_random_engine generator
	(std::chrono::system_clock::now().time_since_epoch().count());

	std::normal_distribution<float> d(0, 200);

	std::cout << "This is a test" << std::endl;
	using namespace algos::Struct;
	BST<float> bst;

	BST<float>::Node* tmp;

	for (size_t i = 0; i < 100; i++) {
		tmp = new BST<float>::Node(d(generator));
		bst.insert(tmp);
	}

	/*bst.inOrderWalk();
	std::cout << std::endl;
	std::cout << std::endl;
	bst.postOrder();
	std::cout << std::endl;
	std::cout << std::endl;
	bst.preOrder();*/

	printf("The min: %f, the max %f.\n", bst.min(), bst.max());
	std::cout << "The erased " << *tmp << std::endl;
	bst.erase(tmp);
	bst.inOrderWalk();
	getchar();

	return 0;
}