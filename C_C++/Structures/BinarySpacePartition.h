#ifndef BINARYSPACEPARTITION_H
#define BINARYSPACEPARTITION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat scene = cv::Mat::zeros(500, 500, CV_8UC3);

//#define BOUNDARIES_FROM_OBJECTS
#define SPLIT_AXIS_MAX


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

	void getBoundingBoxLimits(Vector<d, type>& mins, Vector<d, type>& maxs) const {
		for(size_t i=0; i< d; i++) { // It's a point!!
			mins[i] = maxs[i] = data[i];
		}
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
	// The node in the binary space partition tree
	class Node {
	friend class BSP;
	public:
		Node()
		:left(NULL),
		right(NULL), 
		parent(NULL) {
			for (size_t i = 0; i < dim; i++) {
				boundaries[i][0] = std::numeric_limits<float>::max(); // min
				boundaries[i][1] = -std::numeric_limits<float>::max(); // max
			}
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

	/**
   * @brief Constructor by objects in the space.
   * @param objs Objects in the space to partitionate.
   */
	BSP(const std::vector<_Tp>& objs)
	: root(new Node) {
		size_t maxDim = 0;

#ifdef BOUNDARIES_FROM_OBJECTS
		computeSpaceBoundaries(root, objs);
		float prevLength = -std::numeric_limits<float>::max(), length = 0.0f;
		maxDim = getAxisOfMaxLength(root, objs);
#else
		for(size_t i=0; i<dim; i++) {
			root->boundaries[i][0] = 0.0f;
			root->boundaries[i][1] = 500.0f;
		}
#endif

#ifdef SPLIT_AXIS_MAX
		maxDim = getAxisOfMaxLength(root, objs);
#endif

		for (size_t i = 0; i < dim; i++) 
			printf("dim %lu -> min: %f max: %f", i, 
				root->boundaries[i][0], root->boundaries[i][1]);
		
		printf("\n");

		splitNode(root, objs, maxDim);
	}

protected:

  size_t getAxisOfMaxLength(Node* node, const std::vector<_Tp>& objs) {
		float prevLength = -std::numeric_limits<float>::max(), length = 0.0f;
		size_t maxDim=0;

		for (size_t i = 0; i < dim; i++) {
			length = node->boundaries[i][1] - node->boundaries[i][0];
			if (length > prevLength) {
				prevLength = length;
				maxDim = i; 
			}
		}

		return maxDim;
	}

	void computeSpaceBoundaries(Node* node, const std::vector<_Tp>& objs) {
		Vector<dim> maxs, mins;

		for (size_t i = 0; i < objs.size(); i++) {
			objs[i].getBoundingBoxLimits(mins, maxs);
			for (size_t j = 0; j < dim; j++) {
				if (mins[j] < node->boundaries[j][0]) node->boundaries[j][0] = objs[i][j];
				if (maxs[j] > node->boundaries[j][1]) node->boundaries[j][1] = objs[i][j];
			}
		}
	}

	void computeSpaceBoundaries(Node* node, size_t maxDim) {
		float length = 0.0f;
		for(size_t i=0; i<dim; i++) {
			for(size_t j=0; j<2; j++ ) {
				if(node->left) node->left->boundaries[i][j] = node->boundaries[i][j];
				if(node->right) node->right->boundaries[i][j] = node->boundaries[i][j];
			}
		}

		length = node->boundaries[maxDim][1] - node->boundaries[maxDim][0];
		if(node->left) node->left->boundaries[maxDim][1] = length/2.0f + node->left->boundaries[maxDim][0];
		if(node->right) node->right->boundaries[maxDim][0] = length/2.0f + node->boundaries[maxDim][0];
	}

	void splitNode(Node* node, const std::vector<_Tp>& objs, size_t maxDim) {
		if (node == NULL || objs.size() == 0) return;
		else if (objs.size() == 1) { 
			node->objs.push_back(objs[0]);
			return; 
		}

#ifdef BOUNDARIES_FROM_OBJECTS
		computeSpaceBoundaries(node, objs);
#endif

		float dst = 0.0f;
		std::vector<_Tp> leftObjs, rightObjs;
		Vector<dim+1> plane = computeSplitingPlane(node, maxDim);

		for (size_t i = 0; i < objs.size(); i++) {
			dst = distanceToPlane(objs[i], plane);

			if (dst > 0) { 
				rightObjs.push_back(objs[i]);
				cv::circle(scene, cv::Point(objs[i][0], objs[i][1]), 5, cv::Scalar(0,0,255), -1);
			}	else {
				leftObjs.push_back(objs[i]);
				cv::circle(scene, cv::Point(objs[i][0], objs[i][1]), 5, cv::Scalar(255,0,0), -1);
			}
		}

		printf("Size of left %lu  size of right %lu\n", leftObjs.size(), rightObjs.size());
		drawLine(node, plane);

		if(leftObjs.size()) node->left = new Node;
		if(rightObjs.size()) node->right = new Node;

#ifndef BOUNDARIES_FROM_OBJECTS
		computeSpaceBoundaries(node, maxDim);
#endif

#ifdef SPLIT_AXIS_MAX
		size_t _maxDim = getAxisOfMaxLength(node, objs);
#else
    size_t _maxDim = (int(maxDim+1) % int(dim) == 0)? 0: maxDim+1;
#endif

		splitNode(node->left, leftObjs, _maxDim);
		splitNode(node->right, rightObjs, _maxDim);
	}

	Vector<dim+1> computeSplitingPlane(Node* node, size_t maxDim) {  // Axis aligned planes
		float length =  node->boundaries[maxDim][1] - node->boundaries[maxDim][0];
		printf("The spliting axis is %lu\n", maxDim);

		Vector<dim + 1> plane;
		plane[maxDim] = 1.0;
		plane[dim] = -length / 2.0f - node->boundaries[maxDim][0] ;

		printf("The plane is %f %f %f\n", plane[0], plane[1], plane[2]);

		return plane;
	}

  void drawLine(Node* node, Vector<3>& plane) {
		cv::Point2f p1, p2;

		p1.x = (plane[0] > 0.0f)? -plane[2]:node->boundaries[0][0];
		p1.y = (plane[1] > 0.0f)? -plane[2]:node->boundaries[1][0];

		p2.x = (plane[0] > 0.0f)? -plane[2]:node->boundaries[0][1];
		p2.y = (plane[1] > 0.0f)? -plane[2]:node->boundaries[1][1];


		cv::line(scene, p1, p2, cv::Scalar::all(255));
		cv::imshow("Scene", scene);
//    cv::waitKey();
  }

	Node* root;
};

#endif
