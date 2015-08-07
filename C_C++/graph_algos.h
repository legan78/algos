#ifndef GRAPH_ALGOS_H
#define GRAPH_ALGOS_H

#include "algosTools.h"
#include <set>

namespace algos {
  namespace graph_algos {


    class Node {
    public:
      Node();
      Node(const Node& n);
      Node(unsigned int v);

      Node& operator=(const Node& n);

      unsigned int get_index();

    protected:
      unsigned int vID;
    };

    class SuperNode {
    public:
      SuperNode();
      SuperNode(const SuperNode& sn);
      SuperNode(unsigned int v);

      SuperNode& operator=(const SuperNode& sn);

      unsigned int get_index();
      void push(unsigned int vID);
      bool find(unsigned int vID);

    protected:
      std::set<unsigned int> nodes;
      unsigned int vID;
    };


    class Edge {
    public:
      Edge();
      Edge(const Edge& e);
      Edge(Node _head, Node _tail);
      
      Edge& operator=(const Edge& other);
      
    protected:
      Node head;
      Node tail;
      unsigned int id;
    };


    class AdjacencyList {
    public:
      AdjacencyList();
      AdjacencyList(const AdjacencyList& list);
      AdjacencyList(unsigned int nNodes);

      AdjacencyList& operator=(const AdjacencyList& list);

      void set_edge(const Edge& _e);

      unsigned int compute_min_cut();

      const std::vector<unsigned int>& get_vertices() const;
      const std::vector<Edge>& get_edges() const;

    protected:
      std::vector<unsigned int> vertices;
      std::vector<Edge> edges;
    };


  }
}



#endif
