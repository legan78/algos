
#include "BinarySearchTree_core.h"
#include <iostream>
#include <random>
#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "BinarySpacePartition_core.h"


int main(int argc, char** argv) {
	const size_t dim=2;

	std::default_random_engine generator
	(std::chrono::system_clock::now().time_since_epoch().count());

	std::uniform_real_distribution<float> d(0, 500);
	std::vector<Vector<dim>* > objs;
//	Vector<dim> v;


	for (size_t i = 0; i < atoi(argv[1]); i++) {
		Vector<dim> *v = new Vector<dim>;
		for (size_t j = 0; j < dim; j++) {
			(*v)[j] = d(generator);
		}
		objs.push_back(v);

		cv::circle(scene, cv::Point((*v)[0], (*v)[1]), 5, cv::Scalar::all(255), -1);
	}

	BSPTree<Vector<dim>*, dim>::OBJECT_THRESHOLD = atoi(argv[2]);
	BSPTree<Vector<dim>*, dim> bsp(objs);
	bsp.fillNonEmptyPartition();

//	bsp.onInit(objs);

	cv::imshow("Scene", scene);
	cv::waitKey();

	return 0;
}




void testBST() {
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
}
