#ifndef BINARYSPACEPARTITION_H
#define BINARYSPACEPARTITION_H

//#define BOUNDARIES_FROM_OBJECTS
#define SPLIT_AXIS_MAX
#define OPENCV_VISUALIZATION


#ifdef OPENCV_VISUALIZATION
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
cv::Mat scene = cv::Mat::zeros(500, 500, CV_8UC3);
#endif

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

/**
 * @brief Clase que implementa el binary space partition para proporcionar
 * una jerarquia a los objetos que estan en la escena. Actualmente las
 * particiones estan alineadas a los ejes y las dimensiones son obtenidas
 * mediante los limites del bounding box de los objetos en cada particion.
 */
template<size_t dim=1, typename _Tp = Vector<dim> >
class BSP {
public:

	static size_t OBJECT_THRESHOLD;

	/**
   * @brief Clase nodo para implementar la particion jerarquica en el arbol 
   * binario de la scena.
   */
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

		size_t getSplitingDimension() {
			return splitingDimension;
		}

	  Vector<dim> getSplitingPoint() {
			return splitingPoint;
		}

	protected:
		Node* left;
		Node* right;
		Node* parent;

		std::vector<_Tp> objs;
		Vector<dim+1> splitingPlane;
    Vector<dim> splitingPoint;
    size_t cuttingDimension;
		float boundaries[dim][2]; // min and max limits
	};

	/**
   * @brief Constructor por default.
	 */
	BSP() : root(NULL) {
	}

	/**
   * @brief Destructor.
   */
	~BSP() {
		release(root);
	}

	/**
   * @brief Inicializador mediante los objetos en la scena.
   * @param objs Objectos en la escena que se particionara.
   */
	void onInit(const std::vector<_Tp>& objs) {
		root = new Node;
		size_t maxDim = 0;

#ifdef BOUNDARIES_FROM_OBJECTS
		computeSpaceBoundaries(root, objs);
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

	/**
	 * @brief Encuentra el eje de maxima extension, i.e. el eje para el cual
   * la diferencia entre las coordenadas minima y maxima es mayor. Este metodo
   * debe ser llamado una vez que los limites de la rejilla hayan sido encontrados.
   * @param node Nodo que representa la rejilla.
   * @param objs Contenedor de los objetos que contiene la rejilla.
   * @return Indice del eje de maxima extension.
	 */
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


	/**
   * @brief Genera los limites de la rejilla actual en base a las dimensiones
   * de los objetos que contiene. Actualmente la dimension de los objetos
   * estan dadas por el bounding box del objeto. Ademas, el bounding box 
   * esta alineado a los ejes coordenados.
   * @param node Nodo que representa la rejilla.
   * @param objs Objetos que contiene la rejilla.
   */
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

	/**
   * @brief Genera los limites de las rejillas hijo a partir de los limites
   * de la rejilla padre y el indice del eje sobre el cual se dividira. De 
   * esta manera el tamaño de las rejillas estan en base al tamaño del 
   * la rejilla de mayor jerarquia.
   * @param node Nodo padre.
   * @param maxDim Indice del eje sobre el cual se dividira.
   */
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

	/**
   * @brief Verifica si la condicion de paro para la division de la scena
   * se ha satisfecho.
   * @param node Nodo que representa la division actual.
   * @param objs Objetos que se insertaran en la rejilla o sus divisiones.
   * @return true si la condicion de paro se ha alcanzado, false de otro modo.
   */
  bool meetStopCriteria(Node* node, const std::vector<_Tp>& objs) {
		if (node == NULL || objs.size() == 0) return true;
		else if (objs.size() <= OBJECT_THRESHOLD) {
			for(size_t i=0; i< objs.size(); i++)
				node->objs.push_back(objs[i]);
	
			return true; 
		}

		return false;
	}

	/**
   * @brief Divide la celda representada por un nodo dado en dos partes.
   * La division esta dada mediante un plano divisor perpendicular a un
   * eje coordenado. La division termina cuando los objetos a repartir
   * son mejores a un umbral de objetos por rejilla proporcionado 
   * mediante OBJECT_THRESHOLD.
   * @para node Nodo que representa la rejilla actual que sera dividida.
   * @param objs Objetos a repartir en las divisiones de la rejilla.
   * @param maxDim Indice del eje sobre el cual se dividira la rejilla.
   */
	void splitNode(Node* node, const std::vector<_Tp>& objs, size_t maxDim) {
		if(meetStopCriteria(node, objs)) return;

#ifdef BOUNDARIES_FROM_OBJECTS
		computeSpaceBoundaries(node, objs);
#endif

		float dst = 0.0f;
		std::vector<_Tp> leftObjs, rightObjs;
		Vector<dim+1> plane = computeSplitingPlane(node, maxDim);
    node->splitingPlane = plane;
    node->cuttingDimension = maxDim;

		// Asigna los objetos a la division correspondiente
		for (size_t i = 0; i < objs.size(); i++) {
			dst = distanceToPlane(objs[i], plane);
			if (dst > 0.0f) { 
				rightObjs.push_back(objs[i]);
			}	else {
				leftObjs.push_back(objs[i]);
			}

#ifdef OPENCV_VISUALIZATION
			cv::Scalar color = (dst>0.0f)? cv::Scalar(0,0,255):cv::Scalar(255,0,0);
			cv::circle(scene, cv::Point(objs[i][0], objs[i][1]), 5, color, -1);
#endif

		}

		printf("Size of left %lu  size of right %lu\n", leftObjs.size(), rightObjs.size());
#ifdef OPENCV_VISUALIZATION
		drawLine(node, plane);
#endif

		// Existen objetos que asignar a las nuevas divisiones
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

	/**
   * @brief Genera el plano divisor que particiona la celda actual representada 
   * por un nodo dado. El plano divisor es alineado de modo que sea perpendicular
   * al eje que tiene la mayor longitud en la celda actual.
   * @param node Nodo que representa la rejilla que se particionara.
   * @param maxDim Indice del eje que el plano generado dividira perpendicularmente.
   * @return Contenedor de los coeficientes del plano.
   */
	Vector<dim+1> computeSplitingPlane(Node* node, size_t maxDim) {  // Axis aligned planes
		float length =  node->boundaries[maxDim][1] - node->boundaries[maxDim][0];
		printf("The spliting axis is %lu\n", maxDim);

		Vector<dim + 1> plane;
		plane[maxDim] = 1.0;
		plane[dim] = -length / 2.0f - node->boundaries[maxDim][0] ;

		printf("The plane is %f %f %f\n", plane[0], plane[1], plane[2]);

		return plane;
	}

#ifdef OPENCV_VISUALIZATION
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
#endif

	/**
   * @brief Libera memoria del nodo y todos sus descendientes. La 
   * liberacion de memoria esta dada mediante el corrimiento post orden.
   */
	void release(Node* node) {
		if(node == NULL) return;

		release(node->left);
		release(node->right);
		delete node;
    node = NULL;
	}

	Node* root; // Nodo raiz de la particion jerarquica
};



template<size_t dim, typename _Tp>
size_t BSP<dim, _Tp>::OBJECT_THRESHOLD = 1;



#endif
