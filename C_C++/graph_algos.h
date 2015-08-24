#ifndef GRAPH_ALGOS_H
#define GRAPH_ALGOS_H

#include "algosTools.h"
#include <list>
#include <set>

namespace algos {
  namespace graph_algos {

    /**
     * @brief Sampling from a population with no replace.
     */
    std::vector<unsigned int> sampling_no_replace(int populationSize, int sampleSize);

    /**
     * @brief Edge class
     */
    class Edge {
    public:
      
      /**
       * @brief Constructor.
       */
      Edge();

      /**
       * @brief Copy constructor.
       * @param e Edge to copy.
       */
      Edge(const Edge& e);

      /**
       * @brief Constructor by parameter specification.
       * @param _head Node pointing arrow of the edge.
       * @param _tail Node tailing arrow of the edge.
       * @param id Id for the edge in the graph.
       * @param weight Length of the edge.
       */
      Edge(unsigned int _head, unsigned int _tail, unsigned int id, unsigned int weight = 0);
      
      /**
       * @brief Asignation operator.
       * @param other Length to copy. 
       */
      Edge& operator=(const Edge& other);

      /**
       * @brief Access to edge head pointing node index.
       * @return Index value of head node.
       */
      unsigned int get_head()const;

      /**
       * @brief Access to edge tail pointing node index.
       * @return Index value of tail node.
       */
      unsigned int get_tail()const;

      /**
       * @brief Access to edge index.
       * @return Index value of edge.
       */
      unsigned int get_index()const;

      /**
       * @brief Access to edge weight.
       * @return Length value of edge.
       */
      unsigned int get_weight() const;
      
    protected:

      /*
       * Head pointing node index.
       */
      unsigned int head;

      /*
       * Tail pointing node index.
       */
      unsigned int tail;

      /*
       * Index of edge.
       */
      unsigned int id;

      /*
       * Length of edge.
       */
      unsigned int weight;
    };

    class _node_ {
    public:
      virtual unsigned int get_index() = 0;

    protected:
      unsigned int vID;
    };



    /**
     * @brief Node class
     */
    class Node {
    public:
      
      /**
       * @brief Constructor.
       */
      Node();
      
      /**
       * @brief Copy constructor.
       * @param n Node to copy
       */
      Node(const Node& n);

      /**
       * @brief Constructor by index id.
       * @param id Index of node.
       */
      Node(unsigned int id);

      /**
       * @brief Assignation operator.
       * @param n Node to copy.
       */
      Node& operator=(const Node& n);

      /**
       * @brief Access to node index.
       * @return Index value of node.
       */
      unsigned int get_index()const ;

      /**
       * @brief Add edge incidenting the node.
       * @param it Index of edge.
       */
      void set_incident_edge(unsigned int it);

      /**
       * @brief Get the set of incident edges to the current node.
       * @return Constant reference to the set of incident edges.
       */
      const std::set<unsigned int>& get_incident_edges() const;

    protected:

      /*
       * Index of node.
       */
      unsigned int vID;

      /*
       * Set of incident edges indices to the node.
       */
      std::set<unsigned int> incidentEdges;
    };

    /**
     * @brief Comparing operator for nodes.
     * @param n1 First node.
     * @param n2 Second node.
     * @return True if n1 and n2 have same index.
     */
    bool operator<(const Node& n1, const Node& n2);


    /**
     * @brief Super node.
     */
    class SuperNode {
    public:

      /**
       * @brief Constructor.
       */
      SuperNode();

      /**
       * @brief Copy constructor.
       * @param sn Supernode to be copied.
       */
      SuperNode(const SuperNode& sn);

      /**
       * @brief Constructor by supernode index.
       */
      SuperNode(unsigned int v);

      /**
       * @brief Assignation operator.
       * @param sn Node to be copied.
       * @return Reference to this pointer.
       */
      SuperNode& operator=(const SuperNode& sn);

      /**
       * @brief Get index of super node.
       * @return Value of node index.
       */
      unsigned int get_index() const;

      /**
       * @brief Add node to the cluster node.
       * @param node_index Index of single node to be added.
       */
      void push(unsigned int node_index);

      /**
       * @brief Add node to the cluster.
       * @param n Node to be added.
       */
      void push(const Node& n);

      /**
       * @brief Add a set of nodes to the cluster node.
       * @param nodeSet Set of nodes to be added to the cluster.
       */
      void push(const std::set<Node>& nodeSet);

      /**
       * @brief Find node in the cluster node.
       * @param vID Index of node to find.
       * @return True if node was found in cluster.
       */
      bool find(unsigned int vID);

      /**
       * @brief Check if any of the nodes of a given edge appear in the cluster.
       * @param e Edge that contains the node indices to be searched for.
       * @return 2 if both nodes where found, 1 for one node and 0 for none.
       */
      unsigned int find(const Edge& e);

      /**
       * @brief Access to the size of the current nodes in the cluster.
       * @return Size of the node set of the cluster index.
       */
      unsigned int internal_size()const;

      /**
       * @brief Access to the node set of the cluster.
       * @brief Constant reference to the node set in the cluster.
       */
      const std::set<Node>& get_node_set()const;

    protected:

      /*
       * Node set.
       */
      std::set<Node> nodes;

      /*
       * Index of the cluster node.
       */
      unsigned int vID;
    };

    /**
     * @brief Comparing operator for Supernodes.
     * @param sn1 First node.
     * @param sn2 Second node.
     * @return True if sn1 and sn2 have same set of nodes.
     */
    bool operator<(const SuperNode& sn1, const SuperNode& sn2);

    /**
     * @brief Adjacency list class for graphs.
     */
    class AdjacencyList {
    public:
      // Data type for MinCut output data: cut value and edges in cut
      typedef std::pair<unsigned int, std::vector<Edge> > MinCutTraits;

      /**
       * @brief Constructor.
       */
      AdjacencyList();

      /**
       * @brief Copy constructor.
       * @param list Adjacency list to be copied.
       */
      AdjacencyList(const AdjacencyList& list);

      /**
       * @brief Constructor by number of nodes
       * @param nNodes Number of nodes containing the graph.
       */
      AdjacencyList(unsigned int nNodes);

      /**
       * @brief Assignation operator.
       * @param list Adjacency list to be copied.
       * @return Reference to this.
       */
      AdjacencyList& operator=(const AdjacencyList& list);

      /**
       * @brief Set an edge between two nodes in the gaph.
       * @param _e Edge to be added in the graph.
       */
      void set_edge(const Edge& _e);

      /**
       * @brief Look for a given edge in the graph.
       * @param _e Edge to be looked for in the graph.
       * @return True if the edge was found.
       */
      bool find_edge(const Edge& _e);

      /**
       * @brief Look for a given node in the graph.
       * @param nodeId Index of the node to be looked for.
       * @return Iterator to the position of the node if found.
       */
      std::set<Node>::iterator find_node(unsigned int nodeId)const;

      /**
       * @brief Load undirected graphs from file.
       * @param fileName Name of the file to be loaded.
       * @return Adjacency list representing the loaded graph.
       */
      static AdjacencyList load_from_file(const char* fileName);

      /**
       * @brief Load weighted graphs from file.
       * @param fileName Name of the file to be loaded.
       * @param _offset Set to true if the nodes in the file start on 0. False for otherwise.
       * @return Adjacency list representing the loaded graph.
       */
      static AdjacencyList load_weighted_graph(const char* fileName, bool _offset = false);

      /**
       * @brief Access to the set of nodes in the graph.
       * @return Constant reference to the set of nodes.
       */
      const std::set<Node>& get_vertices() const;

      /**
       * @brief Access to the vector of edges in the graph.
       * @return Constant reference to the vector of edges.
       */
      const std::vector<Edge>& get_edges() const;

      /**
       * @brief Computes min cut by random contraction.
       * @param _times Number of times to execute the random contraction algorithm.
       * @return Value of the cut and the edges in the cut.
       */
      MinCutTraits compute_min_cut(unsigned int _times);

      /**
       * @brief Find the shortest path to all the nodes starting from a given node using 
       * the Dijkstras shortest path algorithm.
       * @param source Source node index.
       * @return 
       */
      unsigned int dijkstra_shortest_path(unsigned int source);
      
    protected:

      /**
       * @brief Merge supernodes into one single node.
       * @param ptrs Vector of iterators of supernodes.
       * @return Super node containing all nodes in the list of supernodes iterator.
       */
      SuperNode merge_super_nodes(const std::vector<std::list<SuperNode>::iterator>& ptrs);

      /**
       * @brief Runs the random contraction algorithm one time to find a min cut.
       * @return Value of the cut and the edges in the cut.
       */
      MinCutTraits compute_random_contraction();

      /*
       * Vertices in the graph.
       */
      std::set<Node> vertices;

      /*
       * Edges in the graph.
       */
      std::vector<Edge> edges;
    };


  }
}

/**
 * @brief Comparing operator for edges.
 * @param e1 First edge to be compared.
 * @param e2 Second edge to be compared.
 * @return True if edges are the same.
 */
bool operator==(const algos::graph_algos::Edge& e1, const algos::graph_algos::Edge& e2);

/**
 * @brief Ostream operator for edges.
 */
std::ostream& operator << ( std::ostream& _os, const algos::graph_algos::Edge& e );

#endif
