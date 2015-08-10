#ifndef GRAPH_ALGOS_H
#define GRAPH_ALGOS_H

#include "algosTools.h"
#include <list>
#include <set>

namespace algos {
  namespace graph_algos {

    std::vector<unsigned int> sampling_no_replace(int populationSize, int sampleSize);


    class Node {
    public:
      Node();
      Node(const Node& n);
      Node(unsigned int v);

      Node& operator=(const Node& n);

      unsigned int get_index()const ;

    protected:
      unsigned int vID;
    };

    bool operator<(const Node& n1, const Node& n2);

    class Edge {
    public:
      Edge();
      Edge(const Edge& e);
      Edge(unsigned int _head, unsigned int _tail, unsigned int id);
      
      Edge& operator=(const Edge& other);

      unsigned int get_head()const;
      unsigned int get_tail()const;
      
    protected:
      unsigned int head;
      unsigned int tail;
      unsigned int id;
    };

    class SuperNode {
    public:
      SuperNode();

      SuperNode(const SuperNode& sn);

      SuperNode(unsigned int v);

      SuperNode& operator=(const SuperNode& sn);

      unsigned int get_index() const;

      void push(unsigned int vID);

      void push(const std::set<unsigned int>& nodeSet);

      bool find(unsigned int vID);

      unsigned int find(const Edge& e);

      const std::set<unsigned int>& get_node_set()const;

    protected:
      std::set<unsigned int> nodes;
      unsigned int vID;
    };

    bool operator<(const SuperNode& sn1, const SuperNode& sn2);


    class AdjacencyList {
    public:
      typedef std::pair<unsigned int, std::vector<Edge> > MinCutTraits;

      AdjacencyList();

      AdjacencyList(const AdjacencyList& list);

      AdjacencyList(unsigned int nNodes);

      AdjacencyList& operator=(const AdjacencyList& list);

      void set_edge(const Edge& _e);

      bool find_edge(const Edge& _e);

      static AdjacencyList load_from_file(const char* fileName);

      const std::set<unsigned int>& get_vertices() const;

      const std::vector<Edge>& get_edges() const;


      MinCutTraits compute_min_cut(unsigned int _times);
      

    protected:

      SuperNode merge_super_nodes(const std::vector<std::list<SuperNode>::iterator>& ptrs);

      MinCutTraits compute_random_contraction();

      std::set<unsigned int> vertices;

      std::vector<Edge> edges;
    };


  }
}

bool operator==(const algos::graph_algos::Edge& e1, const algos::graph_algos::Edge& e2);
std::ostream& operator << ( std::ostream& _os, const algos::graph_algos::Edge& e );

#endif
