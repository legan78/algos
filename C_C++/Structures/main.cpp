
#include "BinarySearchTree_core.h"
#include <iostream>
#include <random>
#include <chrono>

template<size_t d = 1, typename type=float>
class Vector {
public:

	Vector() {
		for (size_t i = 0; i < d; i++)
			data[i] = 0;
	}

	Vector(const Vector& v) {
		for (size_t i = 0; i < d; i++)
			data[i] = v.data[i];
	}

	Vector<d, type>& operator=(const Vector<d, type>& v) {
		for (size_t i = 0; i < d; i++)
			data[i] = v.data[i];

		return *this;
	}

	type& operator[](size_t i) {
		return data[i];
	}

	const type& operator[](size_t i) const {
		return data[i];
	}

protected:
	type data[d];
};

template<size_t dim=2, size_t p=dim+1>
float distanceToPlane(const Vector<dim>& point, const Vector<p>& plane) {
	float eval = 0.0f;

	for (size_t i = 0; i < dim; i++) eval += plane[i] * point[i];

	return eval + plane[dim];
}

template<size_t dim=1, typename _Tp = Vector<dim> >
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

		for (size_t i = 0; i < dim; i++)
			printf("dim %lu -> min: %f max: %f", i, root->boundaries[i][0], root->boundaries[i][1]);
		
		printf("\n");

		splitNode(root, objs);
	}

protected:

	void computeSpaceBoundaries(const std::vector<_Tp>& objs) {
		for (size_t i = 0; i < dim; i++) {
			root->boundaries[i][0] = std::numeric_limits<float>::max(); // min
			root->boundaries[i][1] = -std::numeric_limits<float>::max(); // max
		}

		for (size_t i = 0; i < objs.size(); i++) {
			for (size_t j = 0; j < dim; j++) {
				if (objs[i][j] < root->boundaries[j][0]) root->boundaries[j][0] = objs[i][j];
				if (objs[i][j] > root->boundaries[j][1]) root->boundaries[j][1] = objs[i][j];
			}
		}
	}

	void splitNode(Node* node, const std::vector<_Tp>& objs) {
		if (objs.size() == 0) return;
		if (objs.size() == 1) { 
			node->objs.push_back(objs[0]);
			return; 
		}

		float dst = 0.0f;
		std::vector<_Tp> leftObjs, rightObjs;

		Vector<dim+1> plane = computeSplitingPlane(node);

		for (size_t i = 0; i < objs.size(); i++) {
			dst = distanceToPlane<>(objs[i], plane);

			if (dst > 0) leftObjs.push_back(objs[i]);
			else rightObjs.push_back(objs[i]);
		}

		printf("Size of left %lu  size of right %lu\n", leftObjs.size(), rightObjs.size());

		node->left = new Node;
		node->right = new Node;
		//splitNode(node->left, leftObjs);
		/*splitNode(node->right, rightObjs);*/
	}

	Vector<dim+1> computeSplitingPlane(Node* node) {
		float prevLenght = -std::numeric_limits<float>::max(), length = 0.0f;
		size_t maxDim = 0;

		for (size_t i = 0; i < dim; i++) {
			length = node->boundaries[i][1] - node->boundaries[i][0];
			if (length > prevLenght) {
				prevLenght = length;
				maxDim = i; 
			}
		}

		Vector<dim + 1> plane;
		plane[maxDim] = 1.0;
		plane[dim] = -prevLenght / 2.0f;

		return plane;
	}

	Node* root;
};



int main(int argc, char** argv) {
	const size_t dim=2;

	std::default_random_engine generator
	(std::chrono::system_clock::now().time_since_epoch().count());

	std::uniform_real_distribution<float> d(0, 10);
	std::vector<Vector<dim> > objs;
	Vector<dim> v;

	for (size_t i = 0; i < 10; i++) {
		for (size_t j = 0; j < dim; j++) {
			v[j] = d(generator);
		}
		objs.push_back(v);
	}

	BSP<dim> bsp(objs);


	getchar();

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
