#ifndef BINARYSPACEPARTITION_H
#define BINARYSPACEPARTITION_H

#define SCENE_LENGTH 500.0f


//#define BOUNDARIES_FROM_OBJECTS
#define SPLIT_AXIS_MAX
#define OPENCV_VISUALIZATION


#ifdef OPENCV_VISUALIZATION
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
cv::Mat scene = cv::Mat::zeros(SCENE_LENGTH, SCENE_LENGTH, CV_8UC3);
#endif

#include "Tools.h"

template<size_t dim=2, size_t p=dim+1>
float distanceToPlane(const Vector<dim>& point, const Vector<p>& plane) {
	float eval = 0.0f;

	for (size_t i = 0; i < dim; i++) eval += plane[i] * point[i];

	return eval + plane[dim];
}

template<typename ObjType>
class BoundaryExtractor {
public:
  void operator()(const ObjType& obj, Vector<2>& min, Vector<2>& max) { 
		obj->getBoundingBoxLimits(min, max);       
 	}
};

/**
 * @brief Clase que implementa el binary space partition para proporcionar
 * una jerarquia a los objetos que estan en la escena. Actualmente las
 * particiones estan alineadas a los ejes y las dimensiones son obtenidas
 * mediante los limites del bounding box de los objetos en cada particion.
 */
template<typename ObjType, 
				 size_t SpaceDim=3, 
				 size_t PlaneDim=SpaceDim+1,
         typename BExtractor=BoundaryExtractor<ObjType> >
class BSPTree {
	static const size_t planeDim;
public:

	static size_t OBJECT_THRESHOLD;

	/**
   * @brief Clase nodo para implementar la particion jerarquica en el arbol 
   * binario de la scena.
   */
	class Node {
	friend class BSPTree;
	public:
		Node();

		size_t getSplitingAxisIdx();
	  Vector<SpaceDim> getSplitingPoint();
		Vector<PlaneDim> getSplitingPlane();

	protected:
		Node* left;
		Node* right;

		std::vector<ObjType> objs;
		Vector<PlaneDim> splitingPlane;
    Vector<SpaceDim> splitingPoint;

    size_t splitingAxisIdx;
		float boundaries[SpaceDim][2]; // Vertices of box
	};

	/**
   * @brief Constructor por default.
	 */
	BSPTree(const std::vector<ObjType>& objs);

	/**
   * @brief Destructor.
   */
	~BSPTree();

	 void fillNonEmptyPartition() {
		paintPartition(root);
		}


protected:


	/**
   * @brief Inicializador mediante los objetos en la scena.
   * @param objs Objectos en la escena que se particionara.
   */
	void onInit(const std::vector<ObjType>& objs);

	/**
	 * @brief Encuentra el eje de maxima extension, i.e. el eje para el cual
   * la diferencia entre las coordenadas minima y maxima es mayor. Este metodo
   * debe ser llamado una vez que los limites de la rejilla hayan sido encontrados.
   * @param node Nodo que representa la rejilla.
   * @param objs Contenedor de los objetos que contiene la rejilla.
   * @return Indice del eje de maxima extension.
	 */
  size_t getAxisOfMaxLength(Node* node, const std::vector<ObjType>& objs);

	/**
   * @brief Genera los limites de la rejilla actual en base a las dimensiones
   * de los objetos que contiene. Actualmente la dimension de los objetos
   * estan dadas por el bounding box del objeto. Ademas, el bounding box 
   * esta alineado a los ejes coordenados.
   * @param node Nodo que representa la rejilla.
   * @param objs Objetos que contiene la rejilla.
   */
	void computeSpaceBoundaries(Node* node, const std::vector<ObjType>& objs); 

	/**
   * @brief Genera los limites de las rejillas hijo a partir de los limites
   * de la rejilla padre y el indice del eje sobre el cual se dividira. De 
   * esta manera el tamaño de las rejillas estan en base al tamaño del 
   * la rejilla de mayor jerarquia.
   * @param node Nodo padre.
   * @param maxDim Indice del eje sobre el cual se dividira.
   */
	void computeSpaceBoundaries(Node* node, size_t maxDim);

	/**
   * @brief Verifica si la condicion de paro para la division de la scena
   * se ha satisfecho.
   * @param node Nodo que representa la division actual.
   * @param objs Objetos que se insertaran en la rejilla o sus divisiones.
   * @return true si la condicion de paro se ha alcanzado, false de otro modo.
   */
  bool meetStopCriteria(Node* node, const std::vector<ObjType>& objs); 

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
	void splitNode(Node* node, const std::vector<ObjType>& objs, size_t maxDim);

	/**
   * @brief Genera el plano divisor que particiona la celda actual representada 
   * por un nodo dado. El plano divisor es alineado de modo que sea perpendicular
   * al eje que tiene la mayor longitud en la celda actual.
   * @param node Nodo que representa la rejilla que se particionara.
   * @param maxDim Indice del eje que el plano generado dividira perpendicularmente.
   * @return Contenedor de los coeficientes del plano.
   */
	Vector<PlaneDim> computeSplitingPlane(Node* node, size_t maxDim);

#ifdef OPENCV_VISUALIZATION
  void drawLine(Node* node, Vector<3>& plane);

  void paintPartition(Node* n) {
		if(!n) return;
		static size_t count=0;

		if(!n->left && !n->right) {
			printf("Painint nodes: %lu\n", ++count);
			cv::Point2f p1, p2;
      p1 = cv::Point2f(n->boundaries[0][0], n->boundaries[1][0]);
      p2 = cv::Point2f(n->boundaries[0][1], n->boundaries[1][1]);
			cv::rectangle(scene, p1, p2,cv::Scalar::all(255), -1);	
		} else {
    	if(n->left) paintPartition(n->left);
			if(n->right) paintPartition(n->right);
		}
	}
#endif

	/**
   * @brief Libera memoria del nodo y todos sus descendientes. La 
   * liberacion de memoria esta dada mediante el corrimiento post orden.
   */
	void release(Node* node);

	Node* root; // Nodo raiz de la particion jerarquica
  BExtractor bextractor;
};

#endif
