#include "graph_algos.h"
#include <fstream>
#include <sstream>
#include <chrono>

//#define DEBUG_VERSION

/**
 * @brief Ostream operator for edges.
 */
std::ostream& operator << ( std::ostream& _os, const algos::graph_algos::Edge& e ) {
  _os << "("
      << e.get_tail()
      << ","
      << e.get_head()
      << ")";

  return _os;
}

/**
 * @brief Comparing operator for edges.
 * @param e1 First edge to be compared.
 * @param e2 Second edge to be compared.
 * @return True if edges are the same.
 */
bool operator==(const algos::graph_algos::Edge& e1, const algos::graph_algos::Edge& e2) {
  if( (e1.get_head() == e2.get_head() &&
       e1.get_tail() == e2.get_tail()) ||
      (e1.get_head() == e2.get_tail() &&
       e1.get_tail() == e2.get_head()) ) return true;

  return false;
}


namespace algos {
  namespace graph_algos {


    /**
     * @brief Constructor.
     */
    Node::Node() {
    }

    /**
     * @brief Copy constructor.
     * @param n Node to copy
     */
    Node::Node(const Node& n) 
      : vID(n.vID)
      , incidentEdges(n.incidentEdges) {
    }

    /**
     * @brief Constructor by index id.
     * @param id Index of node.
     */
    Node::Node(unsigned int id)
      :vID(id) {
    }

    /**
     * @brief Assignation operator.
     * @param n Node to copy.
     */
    Node& Node::operator=(const Node& n) {
      vID = n.vID;
      incidentEdges = n.incidentEdges;

      return *this;
    }

    /**
     * @brief Comparing operator for nodes.
     * @param n1 First node.
     * @param n2 Second node.
     * @return True if n1 and n2 have same index.
     */
    bool operator<(const Node& n1, const Node& n2) {
      return (n1.get_index() < n2.get_index());
    }

    /**
     * @brief Access to node index.
     * @return Index value of node.
     */
    unsigned int Node::get_index() const {
      return vID;
    }

    /**
     * @brief Add edge incidenting the node.
     * @param it Index of edge.
     */
    void Node::set_incident_edge(unsigned int it) {
      incidentEdges.insert(it);
      /*	  std::cout << "Insident edge for node " 
		    << get_index() 
		    << ": " 
		    << it
		    << "  size: "
		    << incidentEdges.size()
		    << std::endl;*/
    }

    /**
     * @brief Get the set of incident edges to the current node.
     * @return Constant reference to the set of incident edges.
     */
    const std::set<unsigned int>& Node::get_incident_edges() const {
      return incidentEdges;
    }

    /**
     * @brief Constructor.
     */
    SuperNode::SuperNode() {
    }
    
    /**
     * @brief Copy constructor.
     * @param sn Supernode to be copied.
     */
    SuperNode::SuperNode(const SuperNode& sn)
      : vID(sn.vID)
      , nodes(sn.nodes) {
    }

    /**
     * @brief Constructor by supernode index.
     */
    SuperNode::SuperNode(unsigned int v)
      : vID(v) {
    }

    /**
     * @brief Assignation operator.
     * @param sn Node to be copied.
     * @return Reference to this pointer.
     */
    SuperNode& SuperNode::operator=(const SuperNode& sn) {
      vID = sn.vID;
      nodes = sn.nodes;
      
      return *this;
    }

    /**
     * @brief Add node to the cluster.
     * @param n Node to be added.
     */
    bool operator<(const SuperNode& sn1, const SuperNode& sn2) {
      return (sn1.get_index() < sn2.get_index());
    }

    /**
     * @brief Get index of super node.
     * @return Value of node index.
     */
    unsigned int SuperNode::get_index()const {
      return vID;
    }

    /**
     * @brief Add node to the cluster node.
     * @param node_index Index of single node to be added.
     */
    void SuperNode::push(unsigned int node_index) {
      nodes.insert(Node(node_index));
    }

    /**
     * @brief Add a set of nodes to the cluster node.
     * @param nodeSet Set of nodes to be added to the cluster.
     */
    void SuperNode::push(const std::set<Node>& nodeSet) {
      nodes.insert( nodeSet.begin(), nodeSet.end());
    }

    /**
     * @brief Find node in the cluster node.
     * @param vID Index of node to find.
     * @return True if node was found in cluster.
     */
    bool SuperNode::find(unsigned int _vID) {

      std::set<Node>::iterator it = nodes.begin();

      for (; it!= nodes.end(); it++)
	if (it->get_index() == _vID) return true;

      return false;
    }

    /**
     * @brief Check if any of the nodes of a given edge appear in the cluster.
     * @param e Edge that contains the node indices to be searched for.
     * @return 2 if both nodes where found, 1 for one node and 0 for none.
     */
    unsigned int SuperNode::find(const Edge& e) {
      unsigned int headF = (find(e.get_head()))?1:0;
      unsigned int tailF = (find(e.get_tail()))?1:0;

      return headF + tailF;
    }

    /**
     * @brief Access to the node set of the cluster.
     * @brief Constant reference to the node set in the cluster.
     */
    const std::set<Node>& SuperNode::get_node_set()const {
      return nodes;
    }

    /**
     * @brief Access to the size of the current nodes in the cluster.
     * @return Size of the node set of the cluster index.
     */
    unsigned int SuperNode::internal_size()const {
      return nodes.size();
    }

    /**
     * @brief Add node to the cluster.
     * @param n Node to be added.
     */
    void SuperNode::push(const Node& n) {
      nodes.insert(n);
    }

    /**
     * @brief Constructor.
     */
    Edge::Edge() {
    }

    /**
     * @brief Copy constructor.
     * @param e Edge to copy.
     */
    Edge::Edge(const Edge& e)
      : head(e.head)
      , tail(e.tail)
      , id(e.id)
      , weight(e.weight) {
    }

    /**
     * @brief Constructor by parameter specification.
     * @param _head Node pointing arrow of the edge.
     * @param _tail Node tailing arrow of the edge.
     * @param id Id for the edge in the graph.
     * @param weight Length of the edge.
     */
    Edge::Edge(unsigned int _head, unsigned int _tail, unsigned int _id, unsigned int _weight) 
      : head(_head)
      , tail(_tail)
      , id(_id)
      , weight(_weight) {
    }

    /**
     * @brief Asignation operator.
     * @param other Length to copy. 
     */
    Edge& Edge::operator=(const Edge& other) {
      head = other.head;
      tail = other.tail;
      id = other.id;
      weight = other.weight;

      return *this;
    }

    /**
     * @brief Access to edge head pointing node index.
     * @return Index value of head node.
     */
    unsigned int Edge::get_head() const {
      return head;
    }

    /**
     * @brief Access to edge tail pointing node index.
     * @return Index value of tail node.
     */
    unsigned int Edge::get_tail() const {
      return tail;
    }

    /**
     * @brief Access to edge index.
     * @return Index value of edge.
     */
    unsigned int Edge::get_index() const {
      return id;
    }

    /**
     * @brief Access to edge weight.
     * @return Length value of edge.
     */
    unsigned int Edge::get_weight() const {
      return weight;
    }

    /**
     * @brief Constructor.
     */
    AdjacencyList::AdjacencyList() 
    { }

    /**
     * @brief Copy constructor.
     * @param list Adjacency list to be copied.
     */
    AdjacencyList::AdjacencyList(const AdjacencyList& list)
      : vertices(list.vertices)
      , edges(list.edges) {
    }
    
    /**
     * @brief Constructor by number of nodes
     * @param nNodes Number of nodes containing the graph.
     */
    AdjacencyList::AdjacencyList(unsigned int nNodes) {
      for(unsigned int i = 0; i< nNodes; i++)
	vertices.insert(Node(i));
    }

    /**
     * @brief Assignation operator.
     * @param list Adjacency list to be copied.
     * @return Reference to this.
     */
    AdjacencyList& AdjacencyList::operator=(const AdjacencyList& list) {
      vertices = list.vertices;
      edges = list.edges;

      return *this;
    }

    /**
     * @brief Load undirected graphs from file.
     * @param fileName Name of the file to be loaded.
     * @return Adjacency list representing the loaded graph.
     */
    AdjacencyList AdjacencyList::load_from_file(const char* fileName) {
      std::ifstream inputFile(fileName);
      std::string line;
      unsigned int nodeId, linkedId, edgeCounter = 0;

      AdjacencyList G;

      while (std::getline(inputFile, line)) {
	std::stringstream ss;
	ss.str(line.c_str());

	ss >> nodeId;
	G.vertices.insert(nodeId);

	while ( ss >> linkedId ) {
	  Edge e = Edge(nodeId, linkedId, edgeCounter);

	  if(!G.find_edge(e)) {
	    G.vertices.insert( Node(linkedId) );
	    G.set_edge(e);
	    edgeCounter++;
	  }
	}
      }

      return G;
    }


    /**
     * @brief Load weighted graphs from file.
     * @param fileName Name of the file to be loaded.
     * @param _offset Set to true if the nodes in the file start on 0. False for otherwise.
     * @return Adjacency list representing the loaded graph.
     */
    AdjacencyList AdjacencyList::load_weighted_graph(const char* fileName, bool _offset) {

      std::ifstream inputFile(fileName);
      std::string line, chunk;
      unsigned int nodeId, linkedId, edgeCounter = 0, weight;

      unsigned int offset = (_offset)?1:0;

      AdjacencyList G;

      while (std::getline(inputFile, line)) {
	std::stringstream ss;
	ss.str(line.c_str());

	ss >> nodeId;
	G.vertices.insert(nodeId+offset);

	while ( ss >> chunk ) {
	  sscanf(chunk.c_str(), "%d,%d", &linkedId, &weight);
	  Edge e = Edge(linkedId+offset, nodeId+offset, edgeCounter++, weight);

	  if(!G.find_edge(e)) {
	    G.vertices.insert( Node(linkedId+offset) );
	    G.set_edge(e);
	  }
	}
      }

      return G;
    }

    void AdjacencyList::set_edge(const Edge& _e) {
      edges.push_back(_e);

      for ( std::set<Node>::iterator it = vertices.begin(); it!=vertices.end(); it++) {

	if (_e.get_head() == it->get_index() ||
	    _e.get_tail() == it->get_index() ) { 
	  const Node& tmp = *it;
	  Node& tmp2 = const_cast<Node&>(tmp);
	  tmp2.set_incident_edge(edges.back().get_index());

	}
      }
    }


    bool AdjacencyList::find_edge(const Edge& _e) {
      for(unsigned int i = 0; i<edges.size(); i++)
	if(_e == edges[i]) return true;

      return false;
    }


    std::set<Node>::iterator AdjacencyList::find_node(unsigned int nodeId)const {
      std::set<Node>::iterator it = vertices.begin();

      for (; it!=vertices.end(); it++)
	if (nodeId == it->get_index())
	  return it;

      return it;
    }

    /**
     * @brief Merge supernodes into one single node.
     * @param ptrs Vector of iterators of supernodes.
     * @return Super node containing all nodes in the list of supernodes iterator.
     */
    SuperNode AdjacencyList::merge_super_nodes(const std::vector<std::list<SuperNode>::iterator>& ptrs) {

      SuperNode sp;

      for (unsigned int i = 0; i<ptrs.size(); i++) {
	sp.push(ptrs[i]->get_node_set());
      }

      return sp;
    }

    /**
     * @brief Computes min cut by random contraction.
     * @param _times Number of times to execute the random contraction algorithm.
     * @return Value of the cut and the edges in the cut.
     */
    AdjacencyList::MinCutTraits AdjacencyList::compute_min_cut(unsigned int _times) {

      MinCutTraits outPut, it;

      outPut.first = 1E100;

      for (unsigned int i=0; i<_times; i++) {
	it = compute_random_contraction();
	if ( it.first < outPut.first) {
	  outPut.first = it.first;
	  outPut.second = it.second;
	}
      }

      return outPut;
    }

    /**
     * @brief Runs the random contraction algorithm one time to find a min cut.
     * @return Value of the cut and the edges in the cut.
     */
    AdjacencyList::MinCutTraits AdjacencyList::compute_random_contraction() {

      std::list<SuperNode> auxVertices;
      std::vector<bool> deletedEdges(edges.size(), false);
      std::vector<unsigned int> randSamp = sampling_no_replace(edges.size(), edges.size());

      MinCutTraits outPut;

      unsigned int delEdgesCount = 0;

      // Save all nodes as a super node
      for(unsigned int i=1; i<=vertices.size(); i++) {
	SuperNode sn(i);
	sn.push(i);
	auxVertices.push_back(sn);
      }

      unsigned int eIndex = 0, f;
      std::vector<std::list<SuperNode>::iterator > ptrs;

      // Until two clusters in the list
      while (auxVertices.size() > 2) {
	std::list<SuperNode>::iterator it = auxVertices.begin();
	ptrs.clear();

	for(; it!=auxVertices.end(); it++) {
	  f = it->find(edges[randSamp[eIndex]]);

	  if (f==1) { // One one was found
	    std::list<SuperNode>::iterator _it = it;
	    ptrs.push_back(_it);
	    delEdgesCount += (!deletedEdges[randSamp[eIndex]])?1:0;
	    deletedEdges[randSamp[eIndex]] = true;
	  } else if(f==2) { // Two nodes were found
	    delEdgesCount += (!deletedEdges[randSamp[eIndex]])?1:0;
	    deletedEdges[randSamp[eIndex]] = true;
	  }
	}

	if (ptrs.size()) {
	  // Merge all nodes for contraction in one cluster
	  SuperNode sn = merge_super_nodes(ptrs);
	  // Delete all nodes from list
	  for(unsigned int i = 0; i<ptrs.size(); i++)
	    auxVertices.erase(ptrs[i]);

	  // Add cluster to the adjacency list
	  auxVertices.push_back(sn);
	}

	eIndex++;
      }

      // Delete all self edges
      for (unsigned int i = 0; i < deletedEdges.size(); i++) {
	for (std::list<SuperNode>::iterator it = auxVertices.begin(); 
	     it!=auxVertices.end(); 
	     it++ ) {
	  if (!deletedEdges[i] && it->find(edges[i]) == 2) {
	    deletedEdges[i] =  true;
	    delEdgesCount++;
	    break;
	  }
	}
      }

      // Count the edges in the cut
      outPut.first = edges.size() - delEdgesCount;

      // Retrieve deleted edges to output
      for (unsigned int i=0; i<deletedEdges.size(); i++)
	if(!deletedEdges[i])
	  outPut.second.push_back(edges[i]);

      return outPut;
    }

    /**
     * @brief Access to the set of nodes in the graph.
     * @return Constant reference to the set of nodes.
     */
    const std::set<Node>& AdjacencyList::get_vertices() const {
      return vertices;
    }

    /**
     * @brief Access to the vector of edges in the graph.
     * @return Constant reference to the vector of edges.
     */
    const std::vector<Edge>& AdjacencyList::get_edges()const {
      return edges;
    }


    std::vector<unsigned int> sampling_no_replace(int N, int n) {
      // Generator of random numbers
      std::default_random_engine generator
        ( std::chrono::system_clock::now().time_since_epoch().count() );
      // Distribution to generate probabilities
      std::uniform_real_distribution<double> unif(0,1);
      // Indicator vector
      std::vector<unsigned int> indicator(N, 1);
      std::vector<unsigned int> index;
        
      // Loop to select points in the sample
      for(int i=0; i<n; i++) {
	// Generate probability and weight selection
	double tau = unif(generator);
	double p = 1.0/(double(N-i));

	// Cumulative weight
	double cummP = 0.0;
	// Move to select data from population
	for(int j=0; j<N; j++) {
	  cummP += double(indicator[j])*p;
	  // Select data if the cummulative is greater
	  if(cummP > tau) {
	    indicator[j]=0;
	    index.push_back(j);
	    break;
	  }
	}
      }

      return index;
    } 

    /**
     * @brief Find the shortest path to all the nodes starting from a given node using 
     * the Dijkstras shortest path algorithm.
     * @param source Source node index.
     * @return 
     */
    unsigned int AdjacencyList::dijkstra_shortest_path( unsigned int source ) {

      std::vector<unsigned int> dijkstraPath;
      std::vector<unsigned int> shortDist(vertices.size(), 0);
      unsigned int select = 0, nodeHeadIndex;

      SuperNode X; // Set the node

      X.push(*find_node(source));
	
      while(X.internal_size() != vertices.size()) {
	unsigned int minCriteria = 1E100;

	const std::set<Node>& xNodes = X.get_node_set();
	std::set<Node>::iterator itn = xNodes.begin();

	nodeHeadIndex = 0;

	for(; itn!=xNodes.end(); itn++) {
	  
	  const std::set<unsigned int>& es = itn->get_incident_edges();

	  std::set<unsigned int>::iterator ite = es.begin();
	  for (; ite!=es.end(); ite++ ) {

	    if ( edges[*ite].get_head() == itn->get_index() ||
		 X.find(edges[*ite].get_head()) ) continue;

	    if (shortDist[itn->get_index()-1] + edges[*ite].get_weight() < minCriteria) {
	      minCriteria = shortDist[itn->get_index()-1] + edges[*ite].get_weight();
	      select = *ite;
	      nodeHeadIndex = edges[*ite].get_head();
	    }

	  }
	}

        
	if (nodeHeadIndex != 0) {
	  X.push(*find_node(nodeHeadIndex));
	  shortDist[nodeHeadIndex-1] = minCriteria;
	  dijkstraPath.push_back(select);
	}

      }// while

      int indices[] = {6,36,58,81,98,114,132,164,187,196};

      for (unsigned int i=0; i<sizeof(indices)/sizeof(int); i++) {
	std::cout << shortDist[indices[i]] <<"," ;
      }

      std::cout << std::endl;

      return shortDist[shortDist.size()-1];
    }


//======================================================================
//======================================================================


    /**
     * @brief Constructor.
     */
    Node_::Node_()
      : id(0) {
    }

    /**
     * @brief Copy constructor.
     * @param n Node to be copied.
     */
    Node_::Node_(const Node_& n) 
      : id(n.id)
      , neighborhood(n.neighborhood) {
    }

    /**
     * @brief Asignation operator.
     * @param n Node to be copied.
     * @return Reference to this node.
     */
    Node_& Node_::operator=(const Node_& n) {
      id = n.id;
      neighborhood = n.neighborhood;

      return *this;
    }

    /**
     * @brief Set a new neigboring node to the list of adjacent nodes.
     * Before adding, verifies if the neighboring node is already in the list
     * @param n Node to be added to the neigboring list of nodes.
     */
    void Node_::set_neighbor(const Node_::Neighbor& n) {
      std::list<Neighbor>::iterator it = neighborhood.begin();

      for (; it != neighborhood.end(); it++) {
	if (it->first->get_index() == n.first->get_index())
	  return;
      }

      neighborhood.push_back(n);
    }

    /**
     * @brief Set a new neigbooring node to the list of adjacent nodes.
     * Before adding, verifies if the neighboring node is already in the list
     * @param n Pointer to the node to be added to the neigboring list of nodes.
     * @param weight Edge length linking to node n.
     */
    void Node_::set_neighbor(Node_* n, unsigned int weight) {
      Neighbor _n(n, weight);

      set_neighbor(_n);
    }

    /**
     * @brief Access to edge index.
     * @return Index value of edge.
     */
    unsigned int Node_::get_index()const {
      return id;
    }

    /**
     * @brief Get the set of neigbooring nodes.
     * @return Constant reference to the set of incident edges.
     */
    const std::list<Node_::Neighbor>& Node_::get_neighborhood() const {
      return neighborhood;
    }




  }
}



