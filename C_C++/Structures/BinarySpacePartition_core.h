#ifndef BINARYSPACEPARTITION_CORE_H
#define BINARYSPACEPARTITION_CORE_H

#include "BinarySpacePartition.h"


template<typename ObjType, size_t SpaceDim, size_t PlaneDim>
size_t BSPTree<ObjType, SpaceDim, PlaneDim>::OBJECT_THRESHOLD = 1;

/**
 * @brief Clase que implementa el binary space partition para proporcionar
 * una jerarquia a los objetos que estan en la escena. Actualmente las
 * particiones estan alineadas a los ejes y las dimensiones son obtenidas
 * mediante los limites del bounding box de los objetos en cada particion.
 */
template<typename ObjType, size_t SpaceDim, size_t PlaneDim>
	BSPTree<ObjType, SpaceDim, PlaneDim>::Node::Node()
		:left(NULL),
		right(NULL) {
			for (size_t i = 0; i < SpaceDim; i++) {
				boundaries[i][0] = std::numeric_limits<float>::max(); // min
				boundaries[i][1] = -std::numeric_limits<float>::max(); // max
			}
		}

template<typename ObjType, size_t SpaceDim, size_t PlaneDim>
	size_t BSPTree<ObjType, SpaceDim, PlaneDim>::Node::getSplitingAxisIdx() {
			return splitingAxisIdx;
	}

template<typename ObjType, size_t SpaceDim, size_t PlaneDim>
	Vector<SpaceDim> BSPTree<ObjType, SpaceDim, PlaneDim>::Node::getSplitingPoint() {
		return splitingPoint;
	}

template<typename ObjType, size_t SpaceDim, size_t PlaneDim>
	Vector<PlaneDim> BSPTree<ObjType, SpaceDim, PlaneDim>::Node::getSplitingPlane() {
		return splitingPlane;
	}

	/**
   * @brief Constructor por default.
	 */
template<typename ObjType, size_t SpaceDim, size_t PlaneDim>
	BSPTree<ObjType, SpaceDim, PlaneDim>::BSPTree(const std::vector<ObjType>& objs) 
	: root(NULL) {
		onInit(objs);
	}

	/**
   * @brief Destructor.
   */
template<typename ObjType, size_t SpaceDim, size_t PlaneDim>
	BSPTree<ObjType, SpaceDim, PlaneDim>::~BSPTree() {
		release(root);
	}

	/**
   * @brief Inicializador mediante los objetos en la scena.
   * @param objs Objectos en la escena que se particionara.
   */
template<typename ObjType, size_t SpaceDim, size_t PlaneDim>
	void BSPTree<ObjType, SpaceDim, PlaneDim>::onInit(const std::vector<ObjType>& objs) {
		root = new Node;
		size_t maxAxisIdx = 0;

#ifdef BOUNDARIES_FROM_OBJECTS
		computeSpaceBoundaries(root, objs);
		maxAxisIdx = getAxisOfMaxLength(root, objs);
#else
		for(size_t i=0; i<SpaceDim; i++) {
			root->boundaries[i][0] = 0.0f;
			root->boundaries[i][1] = SCENE_LENGTH;
		}
#endif

#ifdef SPLIT_AXIS_MAX
		maxAxisIdx = getAxisOfMaxLength(root, objs);
#endif

		for (size_t i = 0; i < SpaceDim; i++) 
			printf("dim %lu -> min: %f max: %f", i, 
				root->boundaries[i][0], root->boundaries[i][1]);
		printf("\n");

		splitNode(root, objs, maxAxisIdx);
	}

	/**
	 * @brief Encuentra el eje de maxima extension, i.e. el eje para el cual
   * la diferencia entre las coordenadas minima y maxima es mayor. Este metodo
   * debe ser llamado una vez que los limites de la rejilla hayan sido encontrados.
   * @param node Nodo que representa la rejilla.
   * @param objs Contenedor de los objetos que contiene la rejilla.
   * @return Indice del eje de maxima extension.
	 */
template<typename ObjType, size_t SpaceDim, size_t PlaneDim>
  size_t BSPTree<ObjType, SpaceDim, PlaneDim>::getAxisOfMaxLength(Node* node, const std::vector<ObjType>& objs) {
		float prevLength = -std::numeric_limits<float>::max(), length = 0.0f;
		size_t maxAxisIdx = 0;

		for (size_t i = 0; i < SpaceDim; i++) {
			length = node->boundaries[i][1] - node->boundaries[i][0];
			if (length > prevLength) {
				prevLength = length;
				maxAxisIdx = i; 
			}
		}

		return maxAxisIdx;
	}


	/**
   * @brief Genera los limites de la rejilla actual en base a las dimensiones
   * de los objetos que contiene. Actualmente la dimension de los objetos
   * estan dadas por el bounding box del objeto. Ademas, el bounding box 
   * esta alineado a los ejes coordenados.
   * @param node Nodo que representa la rejilla.
   * @param objs Objetos que contiene la rejilla.
   */
template<typename ObjType, size_t SpaceDim, size_t PlaneDim>
	void BSPTree<ObjType, SpaceDim, PlaneDim>::computeSpaceBoundaries(Node* node, const std::vector<ObjType>& objs) {
		Vector<SpaceDim> maxs, mins;

		for (size_t i = 0; i < objs.size(); i++) {
			objs[i]->getBoundingBoxLimits(mins, maxs);
			for (size_t j = 0; j < SpaceDim; j++) {
				if (mins[j] < node->boundaries[j][0]) node->boundaries[j][0] = mins[j];
				if (maxs[j] > node->boundaries[j][1]) node->boundaries[j][1] = maxs[j];
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
template<typename ObjType, size_t SpaceDim, size_t PlaneDim>
	void BSPTree<ObjType, SpaceDim, PlaneDim>::computeSpaceBoundaries(Node* node, size_t maxAxisIdx) {
		float length = 0.0f;
		Node* left = node->left;
    Node* right = node->right;

		// Copy the boundaries of parent into the childs
		for(size_t i=0; i<SpaceDim; i++) {
			if(left) {
				left->boundaries[i][0] = node->boundaries[i][0];
				left->boundaries[i][1] = node->boundaries[i][1];
			}
			if(right) { 
				right->boundaries[i][0] = node->boundaries[i][0];
				right->boundaries[i][1] = node->boundaries[i][1];
			}
		}

		length = (node->boundaries[maxAxisIdx][1] - 
							node->boundaries[maxAxisIdx][0]) / 2.0f;

		if(left)// Set tje upper boundary of left partition
			left->boundaries[maxAxisIdx][1] = 
					length + node->boundaries[maxAxisIdx][0];
		if(right)// Set the lower boundary of right partition
			right->boundaries[maxAxisIdx][0] = 
					length + node->boundaries[maxAxisIdx][0];
	}

	/**
   * @brief Verifica si la condicion de paro para la division de la scena
   * se ha satisfecho.
   * @param node Nodo que representa la division actual.
   * @param objs Objetos que se insertaran en la rejilla o sus divisiones.
   * @return true si la condicion de paro se ha alcanzado, false de otro modo.
   */
template<typename ObjType, size_t SpaceDim, size_t PlaneDim>
  bool BSPTree<ObjType, SpaceDim, PlaneDim>::meetStopCriteria(Node* node, const std::vector<ObjType>& objs) {

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
template<typename ObjType, size_t SpaceDim, size_t PlaneDim>
	void BSPTree<ObjType, SpaceDim, PlaneDim>::splitNode(Node* node, const std::vector<ObjType>& objs, size_t maxAxisIdx) {
		if(meetStopCriteria(node, objs)) return;

#ifdef BOUNDARIES_FROM_OBJECTS
		computeSpaceBoundaries(node, objs);
#endif

		float dst = 0.0f;
		std::vector<ObjType> leftObjs, rightObjs;
		node->splitingPlane = computeSplitingPlane(node, maxAxisIdx);
    node->splitingAxisIdx = maxAxisIdx;

		// Asigna los objetos a la division correspondiente
		for (size_t i = 0; i < objs.size(); i++) {
			dst = distanceToPlane(objs[i], node->splitingPlane);

			if (dst > 0.0f) rightObjs.push_back(objs[i]);
			else leftObjs.push_back(objs[i]);

#ifdef OPENCV_VISUALIZATION
			cv::Scalar color = (dst>0.0f)? cv::Scalar(0,0,255):cv::Scalar(255,0,0);
			cv::circle(scene, cv::Point(objs[i][0], objs[i][1]), 5, color, -1);
#endif

		}

#ifdef OPENCV_VISUALIZATION
		printf("Size of left %lu  size of right %lu\n", leftObjs.size(), rightObjs.size());
		drawLine(node, node->splitingPlane);
#endif

		// Existen objetos que asignar a las nuevas divisiones
		if(leftObjs.size()) node->left = new Node;
		if(rightObjs.size()) node->right = new Node;

#ifndef BOUNDARIES_FROM_OBJECTS
		computeSpaceBoundaries(node, maxAxisIdx);
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
template<typename ObjType, size_t SpaceDim, size_t PlaneDim>
	Vector<PlaneDim> BSPTree<ObjType, SpaceDim, PlaneDim>::computeSplitingPlane(Node* node, size_t maxAxisIdx) {  // Axis aligned planes
		float length =  node->boundaries[maxAxisIdx][1] - node->boundaries[maxAxisIdx][0];
		printf("The spliting axis is %lu\n", maxAxisIdx);

		Vector<PlaneDim> plane;
		plane[maxAxisIdx] = 1.0;
		plane[SpaceDim] = -length / 2.0f - node->boundaries[maxAxisIdx][0] ;

		printf("The plane is %f %f %f\n", plane[0], plane[1], plane[2]);

		return plane;
	}

#ifdef OPENCV_VISUALIZATION
template<typename ObjType, size_t SpaceDim, size_t PlaneDim>
  void BSPTree<ObjType, SpaceDim, PlaneDim>::drawLine(Node* node, Vector<3>& plane) {
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
template<typename ObjType, size_t SpaceDim, size_t PlaneDim>
	void BSPTree<ObjType, SpaceDim, PlaneDim>::release(Node* node) {
		if(node == NULL) return;

		release(node->left);
		release(node->right);
		delete node;
    node = NULL;
	}


#endif
