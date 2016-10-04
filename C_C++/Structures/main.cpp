
#include "BinarySearchTree_core.h"
#include <iostream>
#include <random>
#include <chrono>

template<size_t d = 1, typename type=float>
class Vector {
public:
protected:
	type data[d];
};


template<size_t dim=1, typename _Tp = float[dim]>
class BSP {
public:
	class Node {
	friend class BSP;
	public:

		Node()
			:left(NULL),
			right(NULL), 
			parent(NULL) {
		}

	protected:
		Node* left;
		Node* right;
		Node* parent;

		float boundaries[dim][2]; // min and max limits

		float key;
		std::vector<_Tp> objs;
	};

	BSP() : root(NULL) {
	}

	BSP(const std::vector<_Tp>& objs)
	: root(new Node) {
		computeSpaceBoundaries(objs);
	}

protected:

	void computeSpaceBoundaries(const std::vector<_Tp>& objs) {
		for (size_t i = 0; i < dim; i++) {
			root->boundaries[i][0] = std::numeric_limits<float>::max(); // min
			root->boundaries[i][1] = -std::numeric_limits<float>::max(); // max
		}

		for (size_t i = 0; i < objs.size(); i++) {
			for (size_t j = 0; j < dim; j++) {
				if (root->boundaries[j][0] < objs[i][j]) root->boundaries[j][0];
				if (root->boundaries[j][1] > objs[i][j]) root->boundaries[j][1];
			}
		}
	}

	void splitNode(Node* node, const std::vector<_Tp>& objs) {
		if (objs.size() == 0) return;

		float dst = 0.0f;
		std::vector<float> plane = computePlane(node);

		for (size_t i = 0; i < objs.size(); i++) {
			dst = distanceToPlane();
			if (dst > 0) {
			} else {
			}
		}

	}

	std::vector<float> computePlane(Node* node) {
	}



	Node* root;
};



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