#ifndef GRAPH_ALGOS_H
#define GRAPH_ALGOS_H

#include "algosTools.h"

namespace algos {
  namespace graph_algos {

    template<typename T>
    class AdjacencyList {
    public:

      class Edge {
      public:
	Edge();
	Edge(const Edge& e);
	Edge(unsigned int _head, unsigned int _tail, T weight = 0);

	Edge& operator=(const Edge& other);
	
      protected:
	unsigned int head;
	unsigned int tail;
	unsigned int id;
	T weight;
      };

      AdjacencyList();
      AdjacencyList(const AdjacencyList& list);

      AdjacencyList(unsigned int nNodes);

      AdjacencyList& operator=(const AdjacencyList& list);


      void set_edge(const Edge& _e);


    protected:
      std::vector<unsigned int> vertices;
      std::vector<Edge> edges;
    };

    


  }
}



#endif
